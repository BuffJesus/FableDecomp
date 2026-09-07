#include <cstddef>
#include <cstdint>

struct CActiveFile;

struct CActiveFileVector20Overlay
{
    std::uint32_t begin;    // 0x00
    std::uint32_t end;      // 0x04
    std::uint32_t capacity; // 0x08
};
static_assert(offsetof(CActiveFileVector20Overlay, begin) == 0x00);
static_assert(offsetof(CActiveFileVector20Overlay, end) == 0x04);
static_assert(offsetof(CActiveFileVector20Overlay, capacity) == 0x08);
static_assert(sizeof(CActiveFileVector20Overlay) == 0x0C);

extern "C" std::int32_t __cdecl CActiveFile_AssignVector20(
    std::uint32_t count,
    const std::uint32_t* srcBegin,
    std::int32_t srcEnd);

namespace CFileInstaller
{
    struct CActiveFile
    {
        static void __thiscall OnReadFinished(::CActiveFile* self);
    };
}

CActiveFile* __thiscall CActiveFile_AssignUndefinedVector20(CActiveFile* param_1, CActiveFile* param_2)
{
    if (param_2 != param_1)
    {
        auto* const dst = reinterpret_cast<CActiveFileVector20Overlay*>(param_1);
        auto* const src = reinterpret_cast<CActiveFileVector20Overlay*>(param_2);

        std::uint32_t local_8 = src->begin;
        std::int32_t iVar2 = static_cast<std::int32_t>(src->end);
        const std::uint32_t uVar1 =
            static_cast<std::uint32_t>((iVar2 - static_cast<std::int32_t>(local_8)) / 0x14);

        if (static_cast<std::uint32_t>(
                (static_cast<std::int32_t>(dst->capacity) - static_cast<std::int32_t>(dst->begin)) / 0x14) < uVar1)
        {
            iVar2 = CActiveFile_AssignVector20(uVar1, reinterpret_cast<const std::uint32_t*>(local_8), iVar2);
            CFileInstaller::CActiveFile::OnReadFinished(param_1);
            dst->begin = static_cast<std::uint32_t>(iVar2);
            dst->capacity = static_cast<std::uint32_t>(iVar2 + static_cast<std::int32_t>(uVar1 * 0x14));
        }
        else
        {
            const std::uint32_t uVar3 =
                static_cast<std::uint32_t>(
                    (static_cast<std::int32_t>(dst->end) - static_cast<std::int32_t>(dst->begin)) / 0x14);

            if (uVar3 < uVar1)
            {
                iVar2 = static_cast<std::int32_t>(uVar3 * 0x14U) / 0x14;
                std::uint32_t local_c = dst->begin;

                if (0 < iVar2)
                {
                    do
                    {
                        --iVar2;

                        auto* puVar4 = reinterpret_cast<std::uint32_t*>(local_8);
                        auto* puVar8 = reinterpret_cast<std::uint32_t*>(local_c);
                        for (std::int32_t iVar5 = 5; iVar5 != 0; --iVar5)
                        {
                            *puVar8 = *puVar4;
                            ++puVar4;
                            ++puVar8;
                        }

                        local_c += 0x14;
                        local_8 += 0x14;
                    } while (iVar2 != 0);
                }

                const std::uint32_t puVar8_end = src->end;
                std::uint32_t puVar6 = dst->end;
                std::uint32_t puVar4_addr =
                    src->begin +
                    static_cast<std::uint32_t>(
                        ((static_cast<std::int32_t>(puVar6) - static_cast<std::int32_t>(dst->begin)) / 0x14) * 0x14);

                for (; puVar4_addr != puVar8_end; puVar4_addr += 0x14)
                {
                    if (puVar6 != 0)
                    {
                        auto* puVar7 = reinterpret_cast<std::uint32_t*>(puVar4_addr);
                        auto* puVar9 = reinterpret_cast<std::uint32_t*>(puVar6);
                        for (iVar2 = 5; iVar2 != 0; --iVar2)
                        {
                            *puVar9 = *puVar7;
                            ++puVar7;
                            ++puVar9;
                        }
                    }

                    puVar6 += 0x14;
                }
            }
            else
            {
                iVar2 = (iVar2 - static_cast<std::int32_t>(local_8)) / 0x14;
                std::uint32_t src_addr = local_8;
                local_8 = dst->begin;

                if (0 < iVar2)
                {
                    do
                    {
                        --iVar2;

                        auto* puVar4 = reinterpret_cast<std::uint32_t*>(src_addr);
                        auto* puVar8 = reinterpret_cast<std::uint32_t*>(local_8);
                        for (std::int32_t iVar5 = 5; iVar5 != 0; --iVar5)
                        {
                            *puVar8 = *puVar4;
                            ++puVar4;
                            ++puVar8;
                        }

                        src_addr += 0x14;
                        local_8 += 0x14;
                    } while (iVar2 != 0);
                }
            }
        }

        dst->end = dst->begin + uVar1 * 0x14;
    }

    return param_1;
}