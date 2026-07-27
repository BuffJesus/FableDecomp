#include <cstddef>
#include <cstdint>
#include <bit>

namespace
{
    struct CGameScriptInterfaceStartMovieSequenceOverlay
    {
        std::byte pad00[0x14];
        void* field_14;
        std::byte pad18[0x14];
        std::uint8_t field_2C;
    };
    static_assert(offsetof(CGameScriptInterfaceStartMovieSequenceOverlay, field_14) == 0x14);
    static_assert(offsetof(CGameScriptInterfaceStartMovieSequenceOverlay, field_2C) == 0x2C);

    struct CVectorMapEntryE8Overlay
    {
        std::int32_t key;
        void* value;
    };
    static_assert(offsetof(CVectorMapEntryE8Overlay, key) == 0x0);
    static_assert(offsetof(CVectorMapEntryE8Overlay, value) == 0x4);

    struct CVectorMapOverlay
    {
        std::byte pad00[0x4];
        CVectorMapEntryE8Overlay* end;
    };
    static_assert(offsetof(CVectorMapOverlay, end) == 0x4);

    struct MovieSequenceOwnerOverlay
    {
        std::byte pad00[0x3C];
        std::uint32_t flags_3C;
        CVectorMapOverlay map_44;
        std::byte pad4C[0x91 - 0x4C];
        std::uint8_t flags_91;
    };
    static_assert(offsetof(MovieSequenceOwnerOverlay, flags_3C) == 0x3C);
    static_assert(offsetof(MovieSequenceOwnerOverlay, map_44) == 0x44);
    static_assert(offsetof(MovieSequenceOwnerOverlay, flags_91) == 0x91);

    struct GlobalSequenceStateOverlay
    {
        std::byte pad00[0xF4];
        std::uint8_t field_F4;
    };
    static_assert(offsetof(GlobalSequenceStateOverlay, field_F4) == 0xF4);

    using ResolveField14Fn = void* (__thiscall*)(void*);
    using ResolveOwnerFn = MovieSequenceOwnerOverlay* (__thiscall*)(void*);
    using LowerBoundFn = CVectorMapEntryE8Overlay* (__thiscall*)(CVectorMapOverlay*, const std::int32_t*);
    using AllowUIToOpenFn = void (__thiscall*)(void*, bool);
    using ThisVtableCall1CFn = void (__thiscall*)(CGameScriptInterface*);
    using ThisCall006307Fn = void (__thiscall*)(CGameScriptInterface*);
    using ConfigureMovieSequenceFn = void (__thiscall*)(CGameScriptInterface*, int, float, float);
    using ConstructMovieBaseFn =
        void (__thiscall*)(CScriptGameResourceObjectMovieBase*, CScriptGameResourceObjectMovie*);
    using MovieCtorFn =
        CScriptGameResourceObjectMovie* (__thiscall*)(CScriptGameResourceObjectMovie*, CGameScriptInterface*, bool);

    inline auto sub_00449961 = reinterpret_cast<ResolveField14Fn>(0x00449961);
    inline auto sub_00487DC2 = reinterpret_cast<ResolveOwnerFn>(0x00487DC2);
    inline auto sub_0040F022 = reinterpret_cast<LowerBoundFn>(0x0040F022);
    inline auto sub_0056A928 = reinterpret_cast<AllowUIToOpenFn>(0x0056A928);
    inline auto sub_006E727E = reinterpret_cast<ConfigureMovieSequenceFn>(0x006E727E);
    inline auto sub_008AB967 = reinterpret_cast<ConstructMovieBaseFn>(0x008AB967);
    inline auto sub_006E7627 = reinterpret_cast<MovieCtorFn>(0x006E7627);

    inline GlobalSequenceStateOverlay* const g_GlobalSequenceState =
        reinterpret_cast<GlobalSequenceStateOverlay*>(0x013B8790);
    inline std::int32_t& DAT_01380C34 = *reinterpret_cast<std::int32_t*>(0x01380C34);
    inline std::int32_t& DAT_013BAD00 = *reinterpret_cast<std::int32_t*>(0x013BAD00);
}

void CGameScriptInterface::StartMovieSequence(
    const CCharString& movieName,
    CScriptGameResourceObjectMovieBase& movieBase)
{
    (void)movieName;

    auto* const selfOverlay =
        reinterpret_cast<CGameScriptInterfaceStartMovieSequenceOverlay*>(this);

    MovieSequenceOwnerOverlay* owner =
        sub_00487DC2(sub_00449961(selfOverlay->field_14));

    if (owner != nullptr && (owner->flags_91 & 0x01) == 0 && (owner->flags_3C & 0x00000100) != 0)
    {
        const std::int32_t key = 0xE8;
        CVectorMapEntryE8Overlay* entry = sub_0040F022(&owner->map_44, &key);
        if (entry == owner->map_44.end || entry->key > 0xE8)
        {
            entry = owner->map_44.end;
        }

        sub_0056A928(entry->value, false);
    }

    while (selfOverlay->field_2C != 0)
    {
        reinterpret_cast<ThisVtableCall1CFn>((*reinterpret_cast<void***>(this))[0x1C / 4])(this);
    }

    while (g_GlobalSequenceState->field_F4 != 0)
    {
        reinterpret_cast<ThisVtableCall1CFn>((*reinterpret_cast<void***>(this))[0x1C / 4])(this);
    }

    selfOverlay->field_2C = 1;
    sub_006E727E(
        this,
        0,
        std::bit_cast<float>(0x3F000000u),
        std::bit_cast<float>(0x3FE38E39u));

    reinterpret_cast<ThisCall006307Fn>((*reinterpret_cast<void***>(this))[0x630 / 4])(this);

    CScriptGameResourceObjectMovie* movieObject = static_cast<CScriptGameResourceObjectMovie*>(
        operator new(0x18));
    if (movieObject != nullptr)
    {
        movieObject = sub_006E7627(movieObject, this, true);
    }

    sub_008AB967(&movieBase, movieObject);

    DAT_01380C34 = -1;
    DAT_013BAD00 = 0;
}