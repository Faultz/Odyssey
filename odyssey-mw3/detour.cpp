#include "stdafx.h"

sys_prx_module_info_t GetModuleInfo(sys_prx_id_t handle)
{
    sys_prx_module_info_t info {};
    static sys_prx_segment_info_t segments[10] {};
    static char filename[SYS_PRX_MODULE_FILENAME_SIZE] {};

    memset(segments, 0, sizeof(segments));
    memset(filename, 0, sizeof(filename));

    info.size = sizeof(info);
    info.segments = segments;
    info.segments_num = sizeof(segments) / sizeof(sys_prx_segment_info_t);
    info.filename = filename;
    info.filename_size = sizeof(filename);

    sys_prx_get_module_info(handle, 0, &info);
    return info;
}

#define POWERPC_REGISTERINDEX_R0      0
#define POWERPC_REGISTERINDEX_R1      1
#define POWERPC_REGISTERINDEX_R2      2
#define POWERPC_REGISTERINDEX_R3      3
#define POWERPC_REGISTERINDEX_R4      4
#define POWERPC_REGISTERINDEX_R5      5
#define POWERPC_REGISTERINDEX_R6      6
#define POWERPC_REGISTERINDEX_R7      7
#define POWERPC_REGISTERINDEX_R8      8
#define POWERPC_REGISTERINDEX_R9      9
#define POWERPC_REGISTERINDEX_R10     10
#define POWERPC_REGISTERINDEX_R11     11
#define POWERPC_REGISTERINDEX_R12     12
#define POWERPC_REGISTERINDEX_R13     13
#define POWERPC_REGISTERINDEX_R14     14
#define POWERPC_REGISTERINDEX_R15     15
#define POWERPC_REGISTERINDEX_R16     16
#define POWERPC_REGISTERINDEX_R17     17
#define POWERPC_REGISTERINDEX_R18     18
#define POWERPC_REGISTERINDEX_R19     19
#define POWERPC_REGISTERINDEX_R20     20
#define POWERPC_REGISTERINDEX_R21     21
#define POWERPC_REGISTERINDEX_R22     22
#define POWERPC_REGISTERINDEX_R23     23
#define POWERPC_REGISTERINDEX_R24     24
#define POWERPC_REGISTERINDEX_R25     25
#define POWERPC_REGISTERINDEX_R26     26
#define POWERPC_REGISTERINDEX_R27     27
#define POWERPC_REGISTERINDEX_R28     28
#define POWERPC_REGISTERINDEX_R29     29
#define POWERPC_REGISTERINDEX_R30     30
#define POWERPC_REGISTERINDEX_R31     31
#define POWERPC_REGISTERINDEX_SP      1
#define POWERPC_REGISTERINDEX_RTOC    2

#define MASK_N_BITS(N) ( ( 1 << ( N ) ) - 1 )

#define POWERPC_HI(X) ( ( X >> 16 ) & 0xFFFF )
#define POWERPC_LO(X) ( X & 0xFFFF )

// PowerPC most significant bit is addressed as bit 0 in documentation.
#define POWERPC_BIT32(N) ( 31 - N )

// Opcode is bits 0-5. 
// Allowing for op codes ranging from 0-63.
#define POWERPC_OPCODE(OP)       (uint32_t)( OP << 26 )
#define POWERPC_OPCODE_ADDI      POWERPC_OPCODE( 14 )
#define POWERPC_OPCODE_ADDIS     POWERPC_OPCODE( 15 )
#define POWERPC_OPCODE_LIS       POWERPC_OPCODE( 15 )
#define POWERPC_OPCODE_BC        POWERPC_OPCODE( 16 )
#define POWERPC_OPCODE_B         POWERPC_OPCODE( 18 )
#define POWERPC_OPCODE_BCCTR     POWERPC_OPCODE( 19 )
#define POWERPC_OPCODE_ORI       POWERPC_OPCODE( 24 )
#define POWERPC_OPCODE_EXTENDED  POWERPC_OPCODE( 31 ) // Use extended opcodes.
#define POWERPC_OPCODE_STW       POWERPC_OPCODE( 36 )
#define POWERPC_OPCODE_LWZ       POWERPC_OPCODE( 32 )
#define POWERPC_OPCODE_LD        POWERPC_OPCODE( 58 )
#define POWERPC_OPCODE_STD       POWERPC_OPCODE( 62 )
#define POWERPC_OPCODE_MASK      POWERPC_OPCODE( 63 )

#define POWERPC_EXOPCODE(OP)     ( OP << 1 )
#define POWERPC_EXOPCODE_BCCTR   POWERPC_EXOPCODE( 528 )
#define POWERPC_EXOPCODE_MTSPR   POWERPC_EXOPCODE( 467 )

// SPR field is encoded as two 5 bit bitfields.
#define POWERPC_SPR(SPR) (uint32_t)( ( ( SPR & 0x1F ) << 5 ) | ( ( SPR >> 5 ) & 0x1F ) )

// Instruction helpers.
// rD - Destination register.
// rS - Source register.
// rA/rB - Register inputs.
// SPR - Special purpose register.
// UIMM/SIMM - Unsigned/signed immediate.
#define POWERPC_ADDI(rD, rA, SIMM)  (uint32_t)( POWERPC_OPCODE_ADDI | ( rD << POWERPC_BIT32( 10 ) ) | ( rA << POWERPC_BIT32( 15 ) ) | SIMM )
#define POWERPC_ADDIS(rD, rA, SIMM) (uint32_t)( POWERPC_OPCODE_ADDIS | ( rD << POWERPC_BIT32( 10 ) ) | ( rA << POWERPC_BIT32( 15 ) ) | SIMM )
#define POWERPC_LIS(rD, SIMM)       POWERPC_ADDIS( rD, 0, SIMM ) // Mnemonic for addis %rD, 0, SIMM
#define POWERPC_LI(rD, SIMM)        POWERPC_ADDI( rD, 0, SIMM )  // Mnemonic for addi %rD, 0, SIMM
#define POWERPC_MTSPR(SPR, rS)      (uint32_t)( POWERPC_OPCODE_EXTENDED | ( rS << POWERPC_BIT32( 10 ) ) | ( POWERPC_SPR( SPR ) << POWERPC_BIT32( 20 ) ) | POWERPC_EXOPCODE_MTSPR )
#define POWERPC_MTCTR(rS)           POWERPC_MTSPR( 9, rS ) // Mnemonic for mtspr 9, rS
#define POWERPC_ORI(rS, rA, UIMM)   (uint32_t)( POWERPC_OPCODE_ORI | ( rS << POWERPC_BIT32( 10 ) ) | ( rA << POWERPC_BIT32( 15 ) ) | UIMM )
#define POWERPC_BCCTR(BO, BI, LK)   (uint32_t)( POWERPC_OPCODE_BCCTR | ( BO << POWERPC_BIT32( 10 ) ) | ( BI << POWERPC_BIT32( 15 ) ) | ( LK & 1 ) | POWERPC_EXOPCODE_BCCTR )
#define POWERPC_STD(rS, DS, rA)     (uint32_t)( POWERPC_OPCODE_STD | ( rS << POWERPC_BIT32( 10 ) ) | ( rA << POWERPC_BIT32( 15 ) ) | ( (int16_t)DS & 0xFFFF ) )
#define POWERPC_LD(rS, DS, rA)      (uint32_t)( POWERPC_OPCODE_LD | ( rS << POWERPC_BIT32( 10 ) ) | ( rA << POWERPC_BIT32( 15 ) ) | ( (int16_t)DS & 0xFFFF ) )

// Branch related fields.
#define POWERPC_BRANCH_LINKED    1
#define POWERPC_BRANCH_ABSOLUTE  2
#define POWERPC_BRANCH_TYPE_MASK ( POWERPC_BRANCH_LINKED | POWERPC_BRANCH_ABSOLUTE )

#define POWERPC_BRANCH_OPTIONS_ALWAYS ( 20 )

uint8_t detour_v::s_TrampolineBuffer[] {};
size_t detour_v::s_TrampolineSize = 0;

detour_v::detour_v()
    : m_HookTarget(nullptr), m_HookAddress(nullptr), m_TrampolineAddress(nullptr), m_OriginalLength(0)
{
    memset(m_TrampolineOpd, 0, sizeof(m_TrampolineOpd));
    memset(m_OriginalInstructions, 0, sizeof(m_OriginalInstructions));
}

detour_v::~detour_v()
{
    UnHook();
}

size_t detour_v::GetHookSize(const void* branchTarget, bool linked, bool preserveRegister)
{
    return JumpWithOptions(nullptr, branchTarget, linked, preserveRegister, POWERPC_BRANCH_OPTIONS_ALWAYS, 0, POWERPC_REGISTERINDEX_R0);
}

size_t detour_v::Jump(void* destination, const void* branchTarget, bool linked, bool preserveRegister)
{
    return JumpWithOptions(destination, branchTarget, linked, preserveRegister, POWERPC_BRANCH_OPTIONS_ALWAYS, 0, POWERPC_REGISTERINDEX_R0);
}

size_t detour_v::JumpWithOptions(void* destination, const void* branchTarget, bool linked, bool preserveRegister, uint32_t branchOptions, uint8_t conditionRegisterBit, uint8_t registerIndex)
{
    uint32_t BranchFarAsm[] = {
        POWERPC_LIS(registerIndex, POWERPC_HI((uint32_t)branchTarget)),                     // lis   %rX, branchTarget@hi
        POWERPC_ORI(registerIndex, registerIndex, POWERPC_LO((uint32_t)branchTarget)),      // ori   %rX, %rX, branchTarget@lo
        POWERPC_MTCTR(registerIndex),                                                       // mtctr %rX
        POWERPC_BCCTR(branchOptions, conditionRegisterBit, linked)                          // bcctr (bcctr 20, 0 == bctr)
    };

    uint32_t BranchFarAsmPreserve[] = {
        POWERPC_STD(registerIndex, -0x30, POWERPC_REGISTERINDEX_R1),                        // std   %rX, -0x30(%r1)
        POWERPC_LIS(registerIndex, POWERPC_HI((uint32_t)branchTarget)),                     // lis   %rX, branchTarget@hi
        POWERPC_ORI(registerIndex, registerIndex, POWERPC_LO((uint32_t)branchTarget)),      // ori   %rX, %rX, branchTarget@lo
        POWERPC_MTCTR(registerIndex),                                                       // mtctr %rX
        POWERPC_LD(registerIndex, -0x30, POWERPC_REGISTERINDEX_R1),                         // ld    %rX, -0x30(%r1)
        POWERPC_BCCTR(branchOptions, conditionRegisterBit, linked)                          // bcctr (bcctr 20, 0 == bctr)
    };

    uint32_t* BranchAsm = preserveRegister ? BranchFarAsmPreserve : BranchFarAsm;
    size_t BranchAsmSize = preserveRegister ? sizeof(BranchFarAsmPreserve) : sizeof(BranchFarAsm);

    if (destination)
        memory::set((uint32_t)destination, BranchAsm, BranchAsmSize);

    return BranchAsmSize;
}

size_t detour_v::RelocateBranch(uint32_t* destination, uint32_t* source)
{
    uint32_t Instruction = *source;
    uint32_t InstructionAddress = (uint32_t)source;

    // Absolute branches dont need to be handled.
    if (Instruction & POWERPC_BRANCH_ABSOLUTE)
    {
        memory::set((uint32_t)destination, &Instruction, sizeof(Instruction));
        return sizeof(Instruction);
    }

    int32_t  BranchOffsetBitSize = 0;
    int32_t  BranchOffsetBitBase = 0;
    uint32_t BranchOptions = 0;
    uint8_t  ConditionRegisterBit = 0;

    switch (Instruction & POWERPC_OPCODE_MASK)
    {
        // B - Branch
        // [Opcode]            [Address]           [Absolute] [Linked]
        //   0-5                 6-29                  30        31
        //
        // Example
        //  010010   0000 0000 0000 0000 0000 0001      0         0
        case POWERPC_OPCODE_B:
            BranchOffsetBitSize = 24;
            BranchOffsetBitBase = 2;
            BranchOptions = POWERPC_BRANCH_OPTIONS_ALWAYS;
            ConditionRegisterBit = 0;
            break;

            // BC - Branch Conditional
            // [Opcode]   [Branch Options]     [Condition Register]         [Address]      [Absolute] [Linked]
            //   0-5           6-10                    11-15                  16-29            30        31
            //
            // Example
            //  010000        00100                    00001             00 0000 0000 0001      0         0
        case POWERPC_OPCODE_BC:
            BranchOffsetBitSize = 14;
            BranchOffsetBitBase = 2;
            BranchOptions = (Instruction >> POWERPC_BIT32(10)) & MASK_N_BITS(5);
            ConditionRegisterBit = (Instruction >> POWERPC_BIT32(15)) & MASK_N_BITS(5);
            break;
    }

    // Even though the address part of the instruction begins from bit 29 in the case of bc and b.
    // The value of the first bit is 4 as all addresses are aligned to for 4 for code therefore,
    // the branch offset can be caluclated by anding in place and removing any suffix bits such as the 
    // link register or absolute flags.
    int32_t BranchOffset = Instruction & (MASK_N_BITS(BranchOffsetBitSize) << BranchOffsetBitBase);

    // Check if the MSB of the offset is set.
    if (BranchOffset >> ((BranchOffsetBitSize + BranchOffsetBitBase) - 1))
    {
        // Add the nessasary bits to our integer to make it negative.
        BranchOffset |= ~MASK_N_BITS(BranchOffsetBitSize + BranchOffsetBitBase);
    }

    void* BranchAddress = reinterpret_cast<void*>(InstructionAddress + BranchOffset);

    return JumpWithOptions(destination, BranchAddress, Instruction & POWERPC_BRANCH_LINKED, true, BranchOptions, ConditionRegisterBit, POWERPC_REGISTERINDEX_R0);
}

size_t detour_v::RelocateCode(uint32_t* destination, uint32_t* source)
{
    uint32_t Instruction = *source;

    switch (Instruction & POWERPC_OPCODE_MASK)
    {
        case POWERPC_OPCODE_B:  // B BL BA BLA
        case POWERPC_OPCODE_BC: // BEQ BNE BLT BGE
            return RelocateBranch(destination, source);
        default:
            memory::set((uint32_t)destination, &Instruction, sizeof(Instruction));
            return sizeof(Instruction);
    }
}

void detour_v::Hook(uintptr_t fnAddress, detour_function fnCallback, uintptr_t tocOverride)
{
    m_HookAddress = reinterpret_cast<void*>(fnAddress);
    m_HookTarget = reinterpret_cast<void*>(*reinterpret_cast<uintptr_t*>(fnCallback));

    // Get the size of the hook but don't hook anything yet
    size_t HookSize = GetHookSize(m_HookTarget, false, false);

    // Save the original instructions for unhooking later on.
    memory::set((uint32_t)m_OriginalInstructions, m_HookAddress, HookSize);

    m_OriginalLength = HookSize;

    // Create trampoline and copy and fix instructions to the trampoline.
    m_TrampolineAddress = &s_TrampolineBuffer[s_TrampolineSize];

    for (size_t i = 0; i < (HookSize / sizeof(uint32_t)); i++)
    {
        uint32_t* InstructionAddress = reinterpret_cast<uint32_t*>((uint32_t)m_HookAddress + (i * sizeof(uint32_t)));

        s_TrampolineSize += RelocateCode((uint32_t*)&s_TrampolineBuffer[s_TrampolineSize], InstructionAddress);
    }

    // Trampoline branches back to the original function after the branch we used to hook.
    void* AfterBranchAddress = reinterpret_cast<void*>((uint32_t)m_HookAddress + HookSize);

    s_TrampolineSize += Jump(&s_TrampolineBuffer[s_TrampolineSize], AfterBranchAddress, false, true);

    // Finally write the branch to the function that we are hooking.
    Jump(m_HookAddress, m_HookTarget, false, false);

    m_TrampolineOpd[0] = reinterpret_cast<uint32_t>(m_TrampolineAddress);
    m_TrampolineOpd[1] = tocOverride != 0 ? tocOverride : get_game_toc();
}

bool detour_v::UnHook()
{
    if (m_HookAddress && m_OriginalLength)
    {
        memory::set((uint32_t)m_HookAddress, m_OriginalInstructions, m_OriginalLength);

        m_OriginalLength = 0;
        m_HookAddress = nullptr;

        return true;
    }

    return false;
}

bool detour_v::GetHookInfo(uintptr_t addr, HookInformation* hookInfo)
{
    uint32_t firstSecond[2];
    memory::set((uint32_t)firstSecond, (void*)addr, sizeof(firstSecond));

    // check if the function is already hooked by us or someone else
    if (((firstSecond[0] & POWERPC_OPCODE_MASK) == POWERPC_OPCODE_LIS)
        && ((firstSecond[1] & POWERPC_OPCODE_MASK) == POWERPC_OPCODE_ORI))
    {
        // fast check if function is hooked
        if (hookInfo == nullptr)
            return true;

        memory::set((uint32_t)hookInfo->hookBytes, (void*)addr, sizeof(hookInfo->hookBytes));

        uint16_t lowFirstInstruction = static_cast<uint16_t>(hookInfo->hookBytes[0]); // first instruction
        uint16_t lowSecondInstruction = static_cast<uint16_t>(hookInfo->hookBytes[1]); // second instruction

        uint32_t hookAddr = (lowFirstInstruction << 16) | lowSecondInstruction;

        sys_prx_id_t prxId = sys_prx_get_module_id_by_address((void*)hookAddr);
        if (prxId == 0)
            return false;

        static sys_prx_module_info_t prxInfo = GetModuleInfo(prxId);

        hookInfo->prxInfo = prxInfo;

        return true;
    }

    return false;
}







ImportExportDetour::ImportExportDetour(HookType type, const std::string& libaryName, uint32_t fnid, detour_function fnCallback)
    : detour_v(), m_LibaryName(libaryName), m_Fnid(fnid)
{
    HookByFnid(type, libaryName, fnid, fnCallback);
}

ImportExportDetour::~ImportExportDetour()
{
    UnHook();
}

void ImportExportDetour::Hook(uintptr_t fnAddress, detour_function fnCallback, uintptr_t tocOverride)
{
    // not implemented
}

bool ImportExportDetour::UnHook()
{
    // not implemented
    return false;
}

void ImportExportDetour::HookByFnid(HookType type, const std::string& libaryName, uint32_t fnid, detour_function fnCallback)
{
    opd_s* fnOpd = nullptr;

    switch (type)
    {
        case HookType::Import:
        {
            fnOpd = FindImportByName(libaryName.c_str(), fnid);
            break;
        }
        case HookType::Export:
        {
            fnOpd = FindExportByName(libaryName.c_str(), fnid);
            break;
        }
    }

    if (fnOpd == nullptr)
        return;

    detour_v::Hook(fnOpd->address, fnCallback, fnOpd->toc);
}

opd_s* FindExportByName(const char* module, uint32_t fnid)
{
    uint32_t* segment15 = *reinterpret_cast<uint32_t**>(0x1008C); // 0x1008C or 0x10094
    uint32_t exportAdressTable = segment15[0x984 / sizeof(uint32_t)];
    exportStub_s* exportStub = reinterpret_cast<exportStub_s*>(exportAdressTable);

    while (exportStub->ssize == 0x1C00)
    {
        if (!strcmp(module, exportStub->name))
        {
            for (int16_t i = 0; i < exportStub->exports; i++)
            {
                if (exportStub->fnid[i] == fnid)
                {
                    return exportStub->stub[i];
                }
            }
        }
        exportStub++;
    }

    return nullptr;
}

opd_s* FindImportByName(const char* module, uint32_t fnid)
{
    uint32_t* segment15 = *reinterpret_cast<uint32_t**>(0x1008C); // 0x1008C or 0x10094
    uint32_t exportAdressTable = segment15[0x984 / sizeof(uint32_t)];
    importStub_s* importStub = reinterpret_cast<importStub_s*>(exportAdressTable);

    while (importStub->ssize == 0x2C00)
    {
        if (!strcmp(module, importStub->name))
        {
            for (int16_t i = 0; i < importStub->imports; i++)
            {
                if (importStub->fnid[i] == fnid)
                {
                    return importStub->stub[i];
                }
            }
        }
        importStub++;
    }

    return nullptr;
}