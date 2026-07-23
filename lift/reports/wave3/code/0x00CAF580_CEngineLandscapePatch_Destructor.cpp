#include <cstddef>
#include <cstdint>
#include <windows.h>

class CCharString;
class CEngineLandscapePatch;

class CFileInstaller
{
public:
    class CActiveFile
    {
    public:
        static void OnReadFinished(CActiveFile* self);
    };
};

struct CEngineLandscapePatch_Destructor_ActiveFileOverlay
{
    std::byte pad_0000[0x04];
    std::int32_t field_0004;
    std::byte pad_0008[0x08];
};

static_assert(offsetof(CEngineLandscapePatch_Destructor_ActiveFileOverlay, field_0004) == 0x04);
static_assert(sizeof(CEngineLandscapePatch_Destructor_ActiveFileOverlay) == 0x10);

struct CEngineLandscapePatch_Destructor_Overlay
{
    std::byte pad_0000[0x08];
    CRITICAL_SECTION field_0008;
    std::byte field_0020[0x18];
    std::byte field_0038[0x18];
    CEngineLandscapePatch_Destructor_ActiveFileOverlay field_0050;
    void* field_0060;
};

static_assert(offsetof(CEngineLandscapePatch_Destructor_Overlay, field_0008) == 0x08);
static_assert(offsetof(CEngineLandscapePatch_Destructor_Overlay, field_0020) == 0x20);
static_assert(offsetof(CEngineLandscapePatch_Destructor_Overlay, field_0038) == 0x38);
static_assert(offsetof(CEngineLandscapePatch_Destructor_Overlay, field_0050) == 0x50);
static_assert(offsetof(CEngineLandscapePatch_Destructor_Overlay, field_0060) == 0x60);

extern void* ExceptionList;

std::intptr_t __thiscall CCharString::GetLength(CCharString* self, std::int32_t* value);
void __thiscall LinkedList_DestroyAll(void* self);
void __thiscall CEngineLandscapePatch::DestroyProceduralTextureVertexBuffers(void* self);

void __fastcall CEngineLandscapePatch__Destructor(std::uintptr_t param_1)
{
    auto* const self = reinterpret_cast<CEngineLandscapePatch_Destructor_Overlay*>(param_1);

    void* pvStack_c = ExceptionList;
    void* puStack_8 = reinterpret_cast<void*>(0x0121728C);
    std::uint32_t local_4 = 3;
    void** ppvVar3 = &pvStack_c;
    std::int32_t local_0c = *reinterpret_cast<std::int32_t*>(param_1 + 0x20);

    (void)puStack_8;

    while ((ExceptionList = ppvVar3), local_0c != 0)
    {
        const std::intptr_t lVar4 =
            CCharString::GetLength(reinterpret_cast<CCharString*>(param_1 + 0x20), &local_0c);

        using RawReleaseFn = void(__thiscall*)(void*);
        (*reinterpret_cast<RawReleaseFn*>(*reinterpret_cast<std::uintptr_t*>(lVar4 + 0x0C) + 0x08))(
            reinterpret_cast<void*>(lVar4 + 0x0C));

        ppvVar3 = reinterpret_cast<void**>(ExceptionList);
    }

    LinkedList_DestroyAll(reinterpret_cast<void*>(param_1 + 0x20));

    local_0c = *reinterpret_cast<std::int32_t*>(param_1 + 0x38);
    while (local_0c != 0)
    {
        const std::intptr_t lVar4 =
            CCharString::GetLength(reinterpret_cast<CCharString*>(param_1 + 0x38), &local_0c);

        using RawReleaseFn = void(__thiscall*)(void*);
        (*reinterpret_cast<RawReleaseFn*>(*reinterpret_cast<std::uintptr_t*>(lVar4 + 0x0C) + 0x08))(
            reinterpret_cast<void*>(lVar4 + 0x0C));
    }

    LinkedList_DestroyAll(reinterpret_cast<void*>(param_1 + 0x38));

    void* const piVar2 = self->field_0060;
    if (piVar2 != nullptr)
    {
        if (self->field_0050.field_0004 != 0)
        {
            using RawFn18 = void(__thiscall*)(void*, std::int32_t);
            (*reinterpret_cast<RawFn18*>(*reinterpret_cast<std::uintptr_t*>(piVar2) + 0x18))(
                piVar2,
                self->field_0050.field_0004);
            self->field_0050.field_0004 = 0;
        }

        using RawFn08 = void(__thiscall*)(void*);
        (*reinterpret_cast<RawFn08*>(*reinterpret_cast<std::uintptr_t*>(self->field_0060) + 0x08))(
            self->field_0060);
    }

    local_4 = (local_4 & 0xFFFFFF00u) | 2u;
    CFileInstaller::CActiveFile::OnReadFinished(
        reinterpret_cast<CFileInstaller::CActiveFile*>(&self->field_0050));

    local_4 = (local_4 & 0xFFFFFF00u) | 1u;
    CEngineLandscapePatch::DestroyProceduralTextureVertexBuffers(
        reinterpret_cast<void*>(param_1 + 0x38));

    local_4 &= 0xFFFFFF00u;
    CEngineLandscapePatch::DestroyProceduralTextureVertexBuffers(
        reinterpret_cast<void*>(param_1 + 0x20));

    DeleteCriticalSection(&self->field_0008);
    ExceptionList = pvStack_c;
}