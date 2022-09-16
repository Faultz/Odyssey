#pragma once

typedef uint32_t audio_port;

#define AUDIO_PATH "/dev_hdd0/tmp/audio/"

struct wave_header
{
    struct riff_chunk
    {
        unsigned int chunk_id;
        unsigned int chunk_size;
        unsigned int format;
    }riff;

    struct fmt_chunk
    {
        unsigned int     chunk_id;
        unsigned int     chunk_size;
        signed short     audio_format;
        unsigned short   num_channels;
        unsigned int     sample_rate;
        unsigned int     byte_rate;
        unsigned short   block_align;
        unsigned short   bits_per_sample;
    }fmt;

    struct data_chunk
    {
        unsigned int     chunk_id;
        unsigned int     chunk_size;
    }data;
};

class audio_file
{
public:
    audio_file() = default;
    audio_file(std::string name) : name(name) { }

    std::string name;
    wave_header header;
    cell_fs handle;
    char stream_buffer[4096];

    int stream_offset;
    int stream_count;
    int stream_size;
};

enum audio_state
{
    inactive,
    ready,
    running,
    finished,
};

class audio
{
public:
    std::vector<audio_file> files;
    std::string name;
    bool play_new;
    CellVoicePortParam voice_params;
    CellVoicePortParam pcm_params;
    audio_port voice_port;
    audio_port pcm_port;
    float volume;

    void start_audio();
    void stop_audio();

    void set_volume(float volume);

    bool create_voice_port();
    bool create_pcm_port();

    void connect_ports();

    void add(std::string name);
    void play(std::string name);
    void stop_playing();

    void play_port(audio_port port);
    void pause_port(audio_port port);
    void delete_port(audio_port port);

    void clear_audio();

    void update();

    static void sound_thread(uint64_t value);

	static void start();
	static void stop();
};

extern audio g_audio;