#include <cstddef>
#include <cstdint>

struct CIPhysicsFacesSegmentBase;

struct CBaseIntelligentPointer {
    std::byte _storage[8];

    CBaseIntelligentPointer();
};
static_assert(sizeof(CBaseIntelligentPointer) == 0x8);

template <typename T>
struct CCountedPointer {
    std::byte _storage[8];

    void Reset(T* value);
};
static_assert(sizeof(CCountedPointer<const CIPhysicsFacesSegmentBase>) == 0x8);

class CCharString {
public:
    CCharString(const char* text, int length);
    ~CCharString();
};

namespace NProgressDisplay {
    void __fastcall DisplayProgress(CCharString* text, bool a2, float progress, bool a4);
}

extern double _DAT_0122ed70;
extern void* operator_new(std::size_t size);

#pragma pack(push, 1)
struct InitDisplayEngineOverlay {
    void** vftable;                                                     // 0x00
    std::byte _pad04[0x68 - 0x04];
    std::int32_t field_68;                                              // 0x68
    std::uint8_t field_6C;                                              // 0x6C
    std::byte _pad6D[0x90 - 0x6D];
    std::uint8_t field_90;                                              // 0x90
    std::byte _pad91[0x98 - 0x91];
    double field_98;                                                    // 0x98
    std::byte _padA0[0xE0 - 0xA0];
    CCountedPointer<const CIPhysicsFacesSegmentBase> field_E0;          // 0xE0
};
#pragma pack(pop)

static_assert(offsetof(InitDisplayEngineOverlay, field_68) == 0x68);
static_assert(offsetof(InitDisplayEngineOverlay, field_6C) == 0x6C);
static_assert(offsetof(InitDisplayEngineOverlay, field_90) == 0x90);
static_assert(offsetof(InitDisplayEngineOverlay, field_98) == 0x98);
static_assert(offsetof(InitDisplayEngineOverlay, field_E0) == 0xE0);

void __fastcall InitDisplayEngine_InitGraphics(InitDisplayEngineOverlay* self)
{
    {
        CCharString text("Init Display Engine: Init Graphics", -1);
        NProgressDisplay::DisplayProgress(&text, false, -1.0f, false);
    }

    self->field_6C = 1;
    self->field_68 = 0;

    {
        CCharString text("Init Display Engine: Init Engine", -1);
        NProgressDisplay::DisplayProgress(&text, false, -1.0f, false);
    }

    using RawIndirectCall = void(__thiscall*)(void*);
    reinterpret_cast<RawIndirectCall>(self->vftable[1])(self);

    self->field_98 = _DAT_0122ed70;
    self->field_90 = 1;

    void* const storage = operator_new(0x8);
    if (storage != nullptr) {
        auto* const base = new (storage) CBaseIntelligentPointer();
        self->field_E0.Reset(reinterpret_cast<CIPhysicsFacesSegmentBase*>(base));
        return;
    }

    self->field_E0.Reset(nullptr);
}