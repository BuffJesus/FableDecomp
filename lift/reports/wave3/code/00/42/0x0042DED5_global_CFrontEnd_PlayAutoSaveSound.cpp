struct RefCountedHandleVTable_0042DED5
{
    void (__thiscall* Destroy)(void* self);
    void (__thiscall* ReleaseFinal)(void* self);
};

struct RefCountedHandle_0042DED5
{
    RefCountedHandleVTable_0042DED5* vtbl;
    int refCount;
};

static_assert(offsetof(RefCountedHandle_0042DED5, refCount) == 0x4);

struct AutoSaveSoundSourceData_0042DED5
{
    std::byte pad_0000[0x1CC];
    char soundPath[1];
};

static_assert(offsetof(AutoSaveSoundSourceData_0042DED5, soundPath) == 0x1CC);

struct AutoSaveSoundSource_0042DED5
{
    AutoSaveSoundSourceData_0042DED5* data;
};

static_assert(offsetof(AutoSaveSoundSource_0042DED5, data) == 0x0);

struct CFrontEndManagerVTable_0042DED5
{
    std::byte pad_0000[0xA8];
    AutoSaveSoundSource_0042DED5* (__thiscall* GetAutoSaveSoundSource)(
        NUISystem::CFrontEndManager* self,
        RefCountedHandle_0042DED5** outHandle);
};

static_assert(offsetof(CFrontEndManagerVTable_0042DED5, GetAutoSaveSoundSource) == 0xA8);

struct AutoSaveSoundPlayerVTable_0042DED5
{
    std::byte pad_0000[0x44];
    void (__thiscall* PlayAutoSaveSound)(
        void* self,
        CCharString* soundName,
        int a2,
        int a3,
        float a4,
        float a5,
        int a6);
};

static_assert(offsetof(AutoSaveSoundPlayerVTable_0042DED5, PlayAutoSaveSound) == 0x44);

extern void* DAT_013b8394;

void __stdcall CFrontEnd_PlayAutoSaveSound(float param_1)
{
    if (DAT_013b8394 == nullptr)
    {
        return;
    }

    alignas(CUserProfileManager) std::byte autoSaveFileNameStorage[sizeof(CUserProfileManager)];
    alignas(CCharString) std::byte replacementStorage[sizeof(CCharString)];
    alignas(CCharString) std::byte soundNameStorage[sizeof(CCharString)];

    auto* const autoSaveFileName = std::start_lifetime_as<CCharString>(autoSaveFileNameStorage);
    auto* const replacement = std::start_lifetime_as<CCharString>(replacementStorage);
    auto* const soundName = std::start_lifetime_as<CCharString>(soundNameStorage);

    RefCountedHandle_0042DED5* handle = nullptr;

    NUISystem::CFrontEndManager* const frontEndManager = NUISystem::CFrontEndManager::GetInstance();
    auto* const frontEndManagerVTable =
        *reinterpret_cast<CFrontEndManagerVTable_0042DED5**>(frontEndManager);
    AutoSaveSoundSource_0042DED5* const source =
        frontEndManagerVTable->GetAutoSaveSoundSource(frontEndManager, &handle);
    const char* const sourcePath = source->data->soundPath;

    CUserProfileManager::GetAutoSaveFileName(reinterpret_cast<CUserProfileManager*>(autoSaveFileName));
    soundName->AssignFromData(sourcePath);
    autoSaveFileName->~CCharString();

    if (handle != nullptr)
    {
        int* const refCount = &handle->refCount;
        *refCount = *refCount - 1;
        if (*refCount == 0)
        {
            handle->vtbl->ReleaseFinal(handle);
        }
    }

    autoSaveFileName->AssignFromWide(L".ogg");
    replacement->AssignFromWide(L".wma");
    soundName->ReplaceSubstring(*replacement, *autoSaveFileName);
    replacement->~CCharString();
    autoSaveFileName->~CCharString();

    auto* const autoSaveSoundPlayer = DAT_013b8394;
    auto* const autoSaveSoundPlayerVTable =
        *reinterpret_cast<AutoSaveSoundPlayerVTable_0042DED5**>(autoSaveSoundPlayer);

    autoSaveSoundPlayerVTable->PlayAutoSaveSound(
        autoSaveSoundPlayer,
        soundName,
        0,
        0,
        1.0f,
        param_1,
        -1);

    soundName->~CCharString();
}