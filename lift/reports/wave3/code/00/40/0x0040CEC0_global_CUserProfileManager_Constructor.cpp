#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CUserProfileManager;
struct CEngineInternalPrimitiveBase;
struct CEnginePrimitive;
struct NHeroInformationScreens_CBase;

struct CCharString
{
    void* value;
};
static_assert(sizeof(CCharString) == 0x4);

struct CWideString
{
    void* value;
};
static_assert(sizeof(CWideString) == 0x4);

struct VideoCapabilityEntry
{
    std::int32_t field_0;
    std::int32_t field_4;
    std::int32_t field_8;
    std::int32_t field_c;
};
static_assert(sizeof(VideoCapabilityEntry) == 0x10);

struct VideoCapabilityVector
{
    VideoCapabilityEntry* begin;
    VideoCapabilityEntry* end;
    VideoCapabilityEntry* capacity_end;
};
static_assert(sizeof(VideoCapabilityVector) == 0x0C);

struct StdBitSetOverlay
{
    std::uint32_t field_00;
    std::uint32_t field_04;
    std::uint32_t field_08;
    std::uint32_t field_0c;
    std::uint32_t field_10;
    std::uint32_t field_14;
    std::uint32_t field_18;
    std::uint32_t field_1c;
    std::uint32_t field_20;
    std::uint32_t field_24;
};
static_assert(sizeof(StdBitSetOverlay) == 0x28);

struct IconsDefOverlay
{
    void** vftable;
    std::int32_t ref_count;
    std::byte pad_08[0x44];
    CEngineInternalPrimitiveBase* primitive_base;
    unsigned long primitive_id;
};
static_assert(offsetof(IconsDefOverlay, ref_count) == 0x04);
static_assert(offsetof(IconsDefOverlay, primitive_base) == 0x4C);
static_assert(offsetof(IconsDefOverlay, primitive_id) == 0x50);

struct VideoCapabilityArrayLocals
{
    VideoCapabilityEntry* begin;
    VideoCapabilityEntry* end;
    std::uint32_t reserved;
};
static_assert(sizeof(VideoCapabilityArrayLocals) == 0x0C);

struct CUserProfileManager_ConstructorOverlay
{
    void** vfptr_00;
    void** vfptr_04;
    CCharString field_08;
    std::byte pad_0c[0x48];
    std::uint32_t field_54;
    std::uint32_t field_58;
    std::uint32_t field_5c;
    std::uint32_t field_60;
    std::uint32_t field_64;
    std::uint32_t field_68;
    std::byte pad_6c[0x14];
    VideoCapabilityVector field_80;
    std::uint32_t field_8c;
    std::uint32_t field_90;
    std::uint32_t field_94;
    StdBitSetOverlay field_98;
    CWideString* field_c0;
    CWideString* field_c4;
    CWideString* field_c8;
    std::uint8_t field_cc;
    std::byte pad_cd[0x43];
    CCharString field_110;
    CCharString field_114;
    CCharString field_118;
    std::byte pad_11c[0x1c];
    std::uint32_t field_138;
    std::uint8_t field_13c;
};
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_08) == 0x08);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_54) == 0x54);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_80) == 0x80);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_8c) == 0x8C);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_90) == 0x90);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_94) == 0x94);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_98) == 0x98);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_c0) == 0xC0);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_c4) == 0xC4);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_c8) == 0xC8);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_cc) == 0xCC);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_110) == 0x110);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_114) == 0x114);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_118) == 0x118);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_138) == 0x138);
static_assert(offsetof(CUserProfileManager_ConstructorOverlay, field_13c) == 0x13C);

extern void* PTR_LAB_0122de50;
extern void* PTR__vector_deleting_destructor__0122e23c;
extern void* PTR__OnPreDeviceReset_CUserProfileManager__MAEXXZ_0122e230;

extern void __thiscall NHeroInformationScreens__CBase__CBase(NHeroInformationScreens_CBase* self);
extern CCharString* __thiscall CCharString__CCharString(CCharString* self);
extern void __thiscall CCharString___dtor(CCharString* self);
extern CWideString* __thiscall CWideString__CWideString(CWideString* self, const CWideString* other);
extern CWideString* __thiscall CWideString__operator_equal(CWideString* self, const CWideString* other);
extern CCharString* __thiscall CCharString__AssignFromWide(CCharString* self, const wchar_t* text);

extern void __thiscall Std_BitSet_Allocate(StdBitSetOverlay* self, int bit_count);
extern VideoCapabilityEntry* __cdecl std___Copy_impl__C2DLineF_ptr__C2DLineF_ptr(
    const VideoCapabilityEntry* first,
    const VideoCapabilityEntry* last,
    VideoCapabilityEntry* dest);
extern void __thiscall Std_Vector_Insert_Range_16Bytes(
    VideoCapabilityVector* self,
    VideoCapabilityEntry* position,
    const VideoCapabilityEntry* first,
    const void* last,
    int count,
    int one);

extern void __thiscall NUISystem__CManager__GetIconsDef(CUserProfileManager* self, IconsDefOverlay** out_icons_def);
extern void __thiscall CEngineInternalPrimitiveBase__AddChildPrimitive(
    CEngineInternalPrimitiveBase* self,
    CEnginePrimitive* primitive,
    unsigned long primitive_id);
extern void __cdecl BuildVideoCapabilityTree(
    VideoCapabilityArrayLocals* out_array,
    CEngineInternalPrimitiveBase* primitive_base,
    unsigned long primitive_id,
    std::uint32_t max_x,
    std::uint32_t max_y);

extern void __thiscall CUserProfileManager__SetDefaultValues(CUserProfileManager* self);
extern void __thiscall CUserProfileManager__SetDefaultValuesForNewProfile(CUserProfileManager* self);
extern void __cdecl CMainGameComponent_AddComponentToList(void* component);

CUserProfileManager* __fastcall CUserProfileManager_Constructor(CUserProfileManager* self)
{
    auto* const overlay = reinterpret_cast<CUserProfileManager_ConstructorOverlay*>(self);

    NHeroInformationScreens__CBase__CBase(reinterpret_cast<NHeroInformationScreens_CBase*>(self));
    *reinterpret_cast<void***>(reinterpret_cast<std::byte*>(self) + 0x04) =
        reinterpret_cast<void**>(&PTR_LAB_0122de50);
    *reinterpret_cast<void***>(self) =
        reinterpret_cast<void**>(&PTR__vector_deleting_destructor__0122e23c);
    *reinterpret_cast<void***>(reinterpret_cast<std::byte*>(self) + 0x04) =
        reinterpret_cast<void**>(&PTR__OnPreDeviceReset_CUserProfileManager__MAEXXZ_0122e230);

    CCharString__CCharString(&overlay->field_08);

    *reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::byte*>(self) + 0x0C) = 0;
    *reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::byte*>(self) + 0x31) = 0;
    *reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::byte*>(self) + 0x33) = 1;
    *reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::byte*>(self) + 0x34) = 1;

    overlay->field_54 = 0;
    overlay->field_58 = 0;
    overlay->field_5c = 0;
    overlay->field_60 = 0;
    overlay->field_64 = 0;
    overlay->field_68 = 0;

    overlay->field_80.begin = nullptr;
    overlay->field_80.end = nullptr;
    overlay->field_80.capacity_end = nullptr;
    overlay->field_8c = 0;
    overlay->field_90 = 0;
    overlay->field_94 = 0;

    overlay->field_98.field_00 = 0;
    overlay->field_98.field_04 = 0;
    overlay->field_98.field_08 = 0;
    overlay->field_98.field_0c = 0;
    overlay->field_98.field_10 = 0;
    overlay->field_98.field_14 = 0;
    overlay->field_98.field_18 = 0;
    overlay->field_98.field_1c = 0;
    overlay->field_98.field_20 = 0;
    overlay->field_98.field_24 = 0;
    Std_BitSet_Allocate(&overlay->field_98, 0);

    overlay->field_c0 = nullptr;
    overlay->field_c4 = nullptr;
    overlay->field_c8 = nullptr;

    void* const profile_storage = std::malloc(200);
    overlay->field_c0 = static_cast<CWideString*>(profile_storage);
    overlay->field_c4 = static_cast<CWideString*>(profile_storage);
    overlay->field_c8 = reinterpret_cast<CWideString*>(static_cast<std::byte*>(profile_storage) + 200);

    CCharString local_28;
    CCharString__CCharString(&local_28);

    CWideString* profile_entry = overlay->field_c0;
    for (std::int32_t count = 0x32; count != 0; --count)
    {
        if (profile_entry != nullptr)
        {
            CWideString__CWideString(profile_entry, reinterpret_cast<CWideString*>(&local_28));
        }
        profile_entry += 1;
    }
    overlay->field_c4 = profile_entry;

    CCharString___dtor(&local_28);

    overlay->field_cc = 0;
    CCharString__CCharString(&overlay->field_110);
    CCharString__CCharString(&overlay->field_114);
    CCharString__CCharString(&overlay->field_118);
    overlay->field_13c = 1;

    VideoCapabilityArrayLocals local_1c{};
    local_1c.begin = nullptr;
    local_1c.end = nullptr;
    local_1c.reserved = 0;

    IconsDefOverlay* local_24 = nullptr;
    NUISystem__CManager__GetIconsDef(self, &local_24);

    CEngineInternalPrimitiveBase* primitive_base = reinterpret_cast<CEngineInternalPrimitiveBase*>(0x400);
    unsigned long primitive_id = 0x300;
    if (local_24 != nullptr)
    {
        primitive_id = local_24->primitive_id;
        primitive_base = local_24->primitive_base;
    }

    CEngineInternalPrimitiveBase__AddChildPrimitive(
        primitive_base,
        reinterpret_cast<CEnginePrimitive*>(primitive_base),
        primitive_id);
    BuildVideoCapabilityTree(&local_1c, primitive_base, primitive_id, 0x7FFF, 0x7FFF);

    std::int32_t local_10 = local_1c.begin[0].field_0;
    std::int32_t local_c = local_1c.begin[0].field_4;
    std::int32_t local_8 = local_1c.begin[0].field_8;
    std::int32_t local_4 = local_1c.begin[0].field_c;

    VideoCapabilityEntry* local_28_entries = local_1c.begin;
    if (local_1c.begin != local_1c.end)
    {
        do
        {
            if ((local_28_entries->field_0 != local_10) ||
                (local_28_entries->field_4 != local_c) ||
                (local_28_entries->field_8 != local_8))
            {
                VideoCapabilityEntry* const vector_end = overlay->field_80.end;
                if (vector_end == overlay->field_80.capacity_end)
                {
                    const std::intptr_t local_20 =
                        (reinterpret_cast<std::intptr_t>(vector_end) -
                         reinterpret_cast<std::intptr_t>(overlay->field_80.begin)) >> 4;

                    std::intptr_t local_2c = 1;
                    const std::intptr_t* source = &local_2c;
                    if (local_20 != 0)
                    {
                        source = &local_20;
                    }

                    const std::intptr_t new_count = *source + local_20;
                    VideoCapabilityEntry* new_begin = nullptr;
                    if (new_count != 0)
                    {
                        new_begin = static_cast<VideoCapabilityEntry*>(
                            std::malloc(static_cast<std::size_t>(new_count) * 0x10));
                    }

                    VideoCapabilityEntry* inserted = std___Copy_impl__C2DLineF_ptr__C2DLineF_ptr(
                        overlay->field_80.begin,
                        vector_end,
                        new_begin);

                    if (inserted != nullptr)
                    {
                        inserted->field_0 = local_10;
                        inserted->field_4 = local_c;
                        inserted->field_8 = local_8;
                        inserted->field_c = local_4;
                    }

                    if (overlay->field_80.begin != nullptr)
                    {
                        std::free(overlay->field_80.begin);
                    }

                    overlay->field_80.begin = new_begin;
                    overlay->field_80.end = inserted + 1;
                    overlay->field_80.capacity_end = new_begin + new_count;
                }
                else
                {
                    if (vector_end != nullptr)
                    {
                        vector_end->field_0 = local_10;
                        vector_end->field_4 = local_c;
                        vector_end->field_8 = local_8;
                        vector_end->field_c = local_4;
                    }
                    overlay->field_80.end = vector_end + 1;
                }
            }

            local_10 = local_28_entries->field_0;
            local_c = local_28_entries->field_4;
            local_8 = local_28_entries->field_8;
            local_4 = local_28_entries->field_c;
            local_28_entries += 1;
        } while (local_28_entries != local_1c.end);
    }

    if ((overlay->field_80.begin != overlay->field_80.end) &&
        ((overlay->field_80.end[-1].field_0 != local_10) ||
         (overlay->field_80.end[-1].field_4 != local_c) ||
         (overlay->field_80.end[-1].field_8 != local_8)))
    {
        VideoCapabilityEntry* const vector_end = overlay->field_80.end;
        if (vector_end == overlay->field_80.capacity_end)
        {
            std::uint8_t local_2d;
            VideoCapabilityEntry current;
            current.field_0 = local_10;
            current.field_4 = local_c;
            current.field_8 = local_8;
            current.field_c = local_4;
            Std_Vector_Insert_Range_16Bytes(&overlay->field_80, vector_end, &current, &local_2d, 1, 1);
        }
        else
        {
            if (vector_end != nullptr)
            {
                vector_end->field_0 = local_10;
                vector_end->field_4 = local_c;
                vector_end->field_8 = local_8;
                vector_end->field_c = local_4;
            }
            overlay->field_80.end = vector_end + 1;
        }
    }

    CUserProfileManager__SetDefaultValues(self);
    CUserProfileManager__SetDefaultValuesForNewProfile(self);

    CCharString__AssignFromWide(&local_28, L"Default");
    CWideString__operator_equal(
        reinterpret_cast<CWideString*>(reinterpret_cast<std::byte*>(self) + 0x08),
        reinterpret_cast<const CWideString*>(&local_28));
    CCharString___dtor(&local_28);

    overlay->field_138 = 0;
    CMainGameComponent_AddComponentToList(reinterpret_cast<std::byte*>(self) + 0x04);

    if (local_24 != nullptr)
    {
        local_24->ref_count -= 1;
        if (local_24->ref_count == 0)
        {
            using ReleaseFn = void(__thiscall*)(IconsDefOverlay*);
            reinterpret_cast<ReleaseFn>(local_24->vftable[1])(local_24);
        }
    }

    if (local_1c.begin != nullptr)
    {
        std::free(local_1c.begin);
    }

    return self;
}