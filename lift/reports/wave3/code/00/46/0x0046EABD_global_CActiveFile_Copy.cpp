#include <cstddef>
#include <cstdint>

struct CActiveFile;

struct CActiveFileEntry8 {
    std::uint32_t dword0;
    std::uint32_t dword4;
};

struct CActiveFile_Overlay {
    std::uint32_t field_0x0;
    std::uint32_t field_0x4;
    std::uint32_t field_0x8;
};

static_assert(offsetof(CActiveFile_Overlay, field_0x0) == 0x0);
static_assert(offsetof(CActiveFile_Overlay, field_0x4) == 0x4);
static_assert(offsetof(CActiveFile_Overlay, field_0x8) == 0x8);
static_assert(sizeof(CActiveFile_Overlay) == 0xC);

extern CActiveFileEntry8* __cdecl CActiveFile_CopyConstruct(
    std::uint32_t count,
    const CActiveFileEntry8* first,
    const CActiveFileEntry8* last);

namespace CFileInstaller {
struct CActiveFile_OnReadFinished_Overlay {
    void __thiscall OnReadFinished();
};
}

CActiveFile* __thiscall CActiveFile_Copy(CActiveFile* param_1, CActiveFile* param_2)
{
    if (param_2 != param_1) {
        auto* const dst = reinterpret_cast<CActiveFile_Overlay*>(param_1);
        const auto* const src = reinterpret_cast<const CActiveFile_Overlay*>(param_2);

        const auto* puVar3 =
            reinterpret_cast<const CActiveFileEntry8*>(static_cast<std::uintptr_t>(src->field_0x0));
        const std::uint32_t iVar2 = src->field_0x4;
        const std::uint32_t uVar4 = (iVar2 - src->field_0x0) >> 3;

        if (((dst->field_0x8 - dst->field_0x0) >> 3) < uVar4) {
            CActiveFileEntry8* const new_buffer = CActiveFile_CopyConstruct(
                uVar4,
                puVar3,
                reinterpret_cast<const CActiveFileEntry8*>(static_cast<std::uintptr_t>(iVar2)));

            reinterpret_cast<CFileInstaller::CActiveFile_OnReadFinished_Overlay*>(param_1)->OnReadFinished();

            dst->field_0x0 = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(new_buffer));
            dst->field_0x8 = dst->field_0x0 + uVar4 * 8;
        } else {
            const std::uint32_t uVar5 = (dst->field_0x4 - dst->field_0x0) >> 3;

            if (uVar5 < uVar4) {
                auto* puVar6 =
                    reinterpret_cast<CActiveFileEntry8*>(static_cast<std::uintptr_t>(dst->field_0x0));
                int copy_count = static_cast<int>((uVar5 << 3) >> 3);

                if (0 < copy_count) {
                    do {
                        puVar6->dword0 = puVar3->dword0;
                        puVar6->dword4 = puVar3->dword4;
                        ++puVar3;
                        ++puVar6;
                        --copy_count;
                    } while (copy_count != 0);
                }

                puVar6 =
                    reinterpret_cast<CActiveFileEntry8*>(static_cast<std::uintptr_t>(dst->field_0x4));
                const auto* const puVar1 =
                    reinterpret_cast<const CActiveFileEntry8*>(static_cast<std::uintptr_t>(src->field_0x4));

                for (
                    puVar3 = reinterpret_cast<const CActiveFileEntry8*>(
                        static_cast<std::uintptr_t>(
                            src->field_0x0 + ((((dst->field_0x4 - dst->field_0x0) >> 3) * 8))));
                    puVar3 != puVar1;
                    ++puVar3
                ) {
                    if (puVar6 != nullptr) {
                        puVar6->dword0 = puVar3->dword0;
                        puVar6->dword4 = puVar3->dword4;
                    }
                    ++puVar6;
                }
            } else {
                auto* puVar6 =
                    reinterpret_cast<CActiveFileEntry8*>(static_cast<std::uintptr_t>(dst->field_0x0));
                int copy_count = static_cast<int>((iVar2 - src->field_0x0) >> 3);

                if (0 < copy_count) {
                    do {
                        puVar6->dword0 = puVar3->dword0;
                        puVar6->dword4 = puVar3->dword4;
                        ++puVar3;
                        ++puVar6;
                        --copy_count;
                    } while (copy_count != 0);
                }
            }
        }

        dst->field_0x4 = dst->field_0x0 + uVar4 * 8;
    }

    return param_1;
}