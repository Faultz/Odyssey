#include "stdafx.h"

audio g_audio;
void sysutilCallback(uint64_t uiStatus, uint64_t uiParam __attribute__((unused)), void* pUserData)
{
    switch (uiStatus)
    {
        case CELL_SYSUTIL_REQUEST_EXITGAME:
            printf("sysutil callback: EXITGAME is received.\n");
            *((bool*)pUserData) = true;
            break;
        default:
            break;
    }
}

void sys_audio_thread(uint64_t /**/)
{
    while (!g_unloadModule)
    {
        //int err = cellSysutilCheckCallback();

        g_audio.update();

        sys_timer_usleep(10000);
    }
    sys_ppu_thread_exit(0);
}

void audio::start_audio()
{
    cellSysmoduleLoadModule(CELL_SYSMODULE_VOICE);

    int Result;

    CellVoiceInitParam Params;
    memset(&Params, 0, sizeof(Params));
    Result = cellVoiceInit(&Params);
    if (Result != CELL_OK)
    {
        printf("cellVoiceInit failed\n");
        cellSysmoduleUnloadModule(CELL_SYSMODULE_VOICE);
        cellSysutilUnregisterCallback(0);
        return;
    }

    Result = cellVoiceStart();
    if (Result != CELL_OK)
    {
        printf("cellVoiceStart failed\n");
        cellVoiceEnd();
        cellSysmoduleUnloadModule(CELL_SYSMODULE_VOICE);
        cellSysutilUnregisterCallback(0);
        return;
    }
}

void audio::stop_audio()
{
    clear_audio();

    delete_port(voice_port);
    delete_port(pcm_port);

    cellSysmoduleUnloadModule(CELL_SYSMODULE_VOICE);
    cellSysutilUnregisterCallback(0);
}

void audio::set_volume(float volume)
{
    cellVoiceSetVolume(pcm_port, volume);
}

bool audio::create_voice_port()
{
    voice_params.portType = CELLVOICE_PORTTYPE_OUT_SECONDARY;
    voice_params.bMute = false;
    voice_params.threshold = 100;
    voice_params.volume = 1.0f;
    voice_params.device.playerId = 0;

    int err = cellVoiceCreatePort(&voice_port, &voice_params);
    if (err == CELL_OK)
        return true;
    else
    {
        printf("cellVoiceCreatePort PCMInputPort failed\n");
        delete_port(voice_port);
        return false;
    }
}

bool audio::create_pcm_port()
{
    pcm_params.portType = CELLVOICE_PORTTYPE_IN_PCMAUDIO;
    pcm_params.bMute = false;
    pcm_params.threshold = 0;
    pcm_params.volume = 1.0f;
    pcm_params.pcmaudio.format.sampleRate = CELLVOICE_SAMPLINGRATE_16000;
    pcm_params.pcmaudio.format.dataType = CELLVOICE_PCM_INTEGER_LITTLE_ENDIAN;
    pcm_params.pcmaudio.bufSize = 2048 * 2;
    int err = cellVoiceCreatePort(&pcm_port, &pcm_params);
    if (err == CELL_OK)
        return true;
    else
    {
        printf("cellVoiceCreatePort PCMInputPort failed\n");
        delete_port(pcm_port);
        return false;
    }
}

void audio::connect_ports()
{
    int err = cellVoiceConnectIPortToOPort(pcm_port, voice_port);
    if (err != CELL_OK)
    {
        printf("cellVoiceConnectIPortToOPort PCMInPort to Headset failed %X\n", err);
        return;
    }
}

void audio::update()
{
    for (int i = 0; i < files.size();)
    {
        bool is_end_reached = false;

        audio_file& file = files[i];

        CellVoiceBasePortInfo port;
        memset(&port, 0, sizeof(port));
        int err = cellVoiceGetPortInfo(pcm_port, &port);
        if (err != CELL_OK && err != CELL_VOICE_ERROR_SERVICE_DETACHED)
        {
            printf("cellVoiceGetPortInfo PCMInputPort failed %x\n", err);
        }

        uint32_t dataSize = file.stream_size - sizeof(wave_header);
        uint32_t readSize = (port.numByte > 4096) ? 4096 : port.numByte;
        if ((file.stream_offset + readSize) >= dataSize)
        {
            readSize = (dataSize - file.stream_count);
            is_end_reached = true;
        }

        uint32_t bytes_read = file.handle.read_off(sizeof(wave_header) + file.stream_offset, file.stream_buffer, readSize, 1);

        err = cellVoiceWriteToIPort(pcm_port, file.stream_buffer, &bytes_read);
        if (err != CELL_OK)
        {
            printf("cellVoiceWriteToIPort PCMInPort failed = %0x\n", err);
        }

        file.stream_offset += readSize;
        file.stream_count += readSize;

        if (is_end_reached)
        {
            file.stream_offset = 0;
            file.stream_count = 0;
            file.name.clear();
            files.erase(files.begin() + i);
            i++;
        }
    }
}

void audio::add(std::string name)
{
    audio_file file;
    file.name = name;
    file.handle = cell_fs(va("%s", name.data()), "rb");
    file.handle.read(&file.header, sizeof(wave_header), 1);
    file.handle.seek(0, SEEK_END);
    file.stream_size = file.handle.tell() - sizeof(wave_header);

    files.push_back(file);
}

void audio::sound_thread(uint64_t value)
{
    while (!g_unloadModule)
    {
        if (g_audio.play_new)
        {
            if (g_audio.name.size() != 0)
            {
                std::string& name = g_audio.name;

                cell_fs file(name.data(), "rb");
                file.seek(0, SEEK_END);

                int current_stream = sizeof(wave_header);
                int stream_count;
                int end_stream = file.tell() - sizeof(wave_header);
                char buffer[256];
                memset(buffer, 0, 256);

                g_audio.play_new = false;

                g_audio.set_volume(g_audio.volume);

                while (current_stream < end_stream)
                {
                    bool is_end_reached = false;

                    if (g_audio.play_new)
                        is_end_reached = true;

                    if (g_unloadModule)
                        break;

                    CellVoiceBasePortInfo port;
                    memset(&port, 0, sizeof(port));
                    int err = cellVoiceGetPortInfo(g_audio.pcm_port, &port);
                    if (err != CELL_OK && err != CELL_VOICE_ERROR_SERVICE_DETACHED)
                    {
                        printf("cellVoiceGetPortInfo PCMInputPort failed %x\n", err);
                    }

                    uint32_t dataSize = end_stream;
                    uint32_t readSize = port.numByte;
                    if ((current_stream + readSize) >= dataSize)
                    {
                        readSize = (dataSize - stream_count);
                        is_end_reached = true;
                    }

                    uint32_t bytes_read = file.read_off(current_stream, buffer, readSize, 1);

                    err = cellVoiceWriteToIPort(g_audio.pcm_port, buffer, &bytes_read);
                    if (err != CELL_OK)
                    {
                        printf("cellVoiceWriteToIPort PCMInPort failed = %0x\n", err);
                    }

                    current_stream += readSize;
                    stream_count += readSize;

                    if (is_end_reached)
                    {
                        g_audio.set_volume(0.f);
                        current_stream = sizeof(wave_header);
                        stream_count = 0;
                        break;
                    }

                    sys_timer_usleep(5);
                }

                g_audio.set_volume(g_audio.volume);
                printf("playing new track\n");

                file.close();
            }
        }
        libpsutil::sleep(10);
    }

    sys_ppu_thread_exit(0);
}

void audio::play(std::string name)
{
    play_new = true;
    this->name = name;
    play_port(pcm_port);
}

void audio::stop_playing()
{
    play_new = true;
    this->name.clear();
    pause_port(pcm_port);
}

void audio::play_port(audio_port port)
{
    if (port != CELLVOICE_INVALID_PORT_ID)
    {
        int err = cellVoiceResumePort(port);
        if (err != CELL_OK)
        {
            printf("cellVoiceResumePort failed %X\n", err);
        }
    }
}

void audio::pause_port(audio_port port)
{
    if (port != CELLVOICE_INVALID_PORT_ID)
    {
        int err = cellVoicePausePort(port);
        if (err != CELL_OK)
        {
            printf("cellVoicePausePort failed %X\n", err);
        }
    }
}
    
void audio::delete_port(audio_port port)
{
    if (port != CELLVOICE_INVALID_PORT_ID)
    {
        int err = cellVoiceDeletePort(port);
        if (err != CELL_OK)
        {
            printf("cellVoiceDeletePort failed %X\n", err);
        }
    }
}

void audio::clear_audio()
{
    for (int i = 0; i < files.size(); i++)
    {
        files[i].name.clear();
        memset(&files[i], 0, sizeof(audio_file));
    }

    files.clear();
}

detour* CG_BulletHitEvent_d = nullptr;
void __cdecl CG_BulletHitEvent(int localClientNum, int sourceEntityNum, int targetEntityNum, int weaponIndex, vec3_t* startPos, vec3_t* position, const float* normal, int surfType, int event, char impactEffects, int hitContents)
{
    g_audio.play("/dev_hdd0/tmp/audio/pew.wav");
    CG_BulletHitEvent_d->invoke(localClientNum, sourceEntityNum, targetEntityNum, weaponIndex, startPos, position, normal, surfType, event, impactEffects, hitContents);
}

void audio::start()
{
    g_audio.start_audio();
    
    g_audio.create_voice_port();
    g_audio.create_pcm_port();

    g_audio.connect_ports();

    sys_ppu_thread_t thread;
    sys_ppu_thread_create(&thread, sound_thread, 0, 0x7C4, 0x100, 0, va("song"));
    sys_ppu_thread_create(&thread, sys_audio_thread, 0, 0x7CE, 0x200, 0, "bonk");

    //CG_BulletHitEvent_d = new detour(0x0092A00, CG_BulletHitEvent);
}

void audio::stop()
{
    g_audio.stop_audio();

    //delete CG_BulletHitEvent_d;
}