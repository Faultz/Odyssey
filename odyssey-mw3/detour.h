#pragma once

struct opd_s
{
    uint32_t address;
    uint32_t toc;
};

struct importStub_s
{
    int16_t ssize;
    int16_t header1;
    int16_t header2;
    int16_t imports;
    int32_t zero1;
    int32_t zero2;
    const char* name;
    uint32_t* fnid;
    opd_s** stub;
    int32_t zero3;
    int32_t zero4;
    int32_t zero5;
    int32_t zero6;
};

struct exportStub_s
{
    int16_t ssize;
    int16_t header1;
    int16_t header2;
    int16_t exports; // number of exports
    int32_t zero1;
    int32_t zero2;
    const char* name;
    uint32_t* fnid;
    opd_s** stub;
};

#define MARK_AS_EXECUTABLE __attribute__((section(".text")))

typedef void* detour_function;

opd_s* FindExportByName(const char* module, uint32_t fnid);
opd_s* FindImportByName(const char* module, uint32_t fnid);

class detour_v
{
public:
    struct HookInformation
    {
        sys_prx_module_info_t prxInfo;
        uint32_t hookBytes[4];
    };

public:
    detour_v();

    template<class R = void> detour_v(uint32_t fnAddress, R(*fnCallback), uintptr_t tocOverride = 0)
        : m_HookTarget(nullptr), m_HookAddress(nullptr), m_TrampolineAddress(nullptr), m_OriginalLength(0)
    {
        memset(m_TrampolineOpd, 0, sizeof(m_TrampolineOpd));
        memset(m_OriginalInstructions, 0, sizeof(m_OriginalInstructions));

        Hook(fnAddress, reinterpret_cast<void*>(fnCallback), tocOverride);
    }

    detour_v(detour_v const&) = delete;
    detour_v(detour_v&&) = delete;
    detour_v& operator=(detour_v const&) = delete;
    detour_v& operator=(detour_v&&) = delete;
    virtual ~detour_v();

    virtual void Hook(uintptr_t fnAddress, detour_function fnCallback, uintptr_t tocOverride = 0);
    virtual bool UnHook();

    // also works
    /*template<typename T>
    T GetOriginal() const
    {
       return T(m_TrampolineOpd);
    }*/

    template <typename R, typename... TArgs>
    R original(TArgs... args)
    {
        R(*original)(TArgs...) = (R(*)(TArgs...))m_TrampolineOpd;
        return original(args...);
    }

private:
    /***
    * Writes an unconditional branch to the destination address that will branch to the target address.
    * @param destination Where the branch will be written to.
    * @param branchTarget The address the branch will jump to.
    * @param linked Branch is a call or a jump? aka bl or b
    * @param preserveRegister Preserve the register clobbered after loading the branch address.
    * @returns size of relocating the instruction in bytes
    */
    size_t Jump(void* destination, const void* branchTarget, bool linked, bool preserveRegister);

    /***
    * Writes both conditional and unconditional branches using the count register to the destination address that will branch to the target address.
    * @param destination Where the branch will be written to.
    * @param branchTarget The address the branch will jump to.
    * @param linked Branch is a call or a jump? aka bl or b
    * @param preserveRegister Preserve the register clobbered after loading the branch address.
    * @param branchOptions Options for determining when a branch to be followed.
    * @param conditionRegisterBit The bit of the condition register to compare.
    * @param registerIndex Register to use when loading the destination address into the count register.
    * @returns size of relocating the instruction in bytes
    */
    size_t JumpWithOptions(void* destination, const void* branchTarget, bool linked, bool preserveRegister,
       uint32_t branchOptions, uint8_t conditionRegisterBit, uint8_t registerIndex);

    /***
    * Copies and fixes relative branch instructions to a new location.
    * @param destination Where to write the new branch.
    * @param source Address to the instruction that is being relocated.
    * @returns size of relocating the instruction in bytes
    */
    size_t RelocateBranch(uint32_t* destination, uint32_t* source);

    /***
    * Copies an instruction enusuring things such as PC relative offsets are fixed.
    * @param destination Where to write the new instruction(s).
    * @param source Address to the instruction that is being copied.
    * @returns size of relocating the instruction in bytes
    */
    size_t RelocateCode(uint32_t* destination, uint32_t* source);

    /***
    * Get's size of method hook in bytes
    * @param branchTarget The address the branch will jump to.
    * @param linked Branch is a call or a jump? aka bl or b
    * @param preserveRegister Preserve the register clobbered after loading the branch address.
    * @returns size of hook in bytes
    */
    size_t GetHookSize(const void* branchTarget, bool linked, bool preserveRegister);

    /***
    * Retrieve infomation about address which contains bytes and name of hook owner
    * @param addr function to check to see if it has been hooked
    * @param hookInfo structure contained sprx name, path and hook bytes. nullptr can be passed to check return value
    * @returns true address is already hooked
    */
    bool GetHookInfo(uintptr_t addr, HookInformation* hookInfo);

protected:
    const void* m_HookTarget;                // The funtion we are pointing the hook to.
    void* m_HookAddress;               // The function we are hooking.
    uint8_t* m_TrampolineAddress;         // Pointer to the trampoline for this detour.
    uint32_t     m_TrampolineOpd[2];          // opd_s of the trampoline for this detour.
    uint8_t      m_OriginalInstructions[30];  // Any bytes overwritten by the hook.
    size_t       m_OriginalLength;            // The amount of bytes overwritten by the hook.

    // Shared
    MARK_AS_EXECUTABLE static uint8_t   s_TrampolineBuffer[2048];
    static size_t                       s_TrampolineSize;
};

// list of fnids https://github.com/aerosoul94/ida_gel/blob/master/src/ps3/ps3.xml
class ImportExportDetour : public detour_v
{
public:
    enum HookType
    {
        Import = 0, Export = 1
    };
public:
    ImportExportDetour(HookType type, const std::string& libaryName, uint32_t fnid, detour_function fnCallback);
    virtual ~ImportExportDetour();

    virtual void Hook(uintptr_t fnAddress, detour_function fnCallback, uintptr_t tocOverride = 0) override;
    virtual bool UnHook() override;

private:
    void HookByFnid(HookType type, const std::string& libaryName, uint32_t fnid, detour_function fnCallback);

private:
    std::string m_LibaryName;
    uint32_t m_Fnid;
};