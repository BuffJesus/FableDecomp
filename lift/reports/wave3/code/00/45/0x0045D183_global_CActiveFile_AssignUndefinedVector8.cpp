#include <cstddef>
#include <cstdint>

struct CActiveFile_Vector8Overlay
{
    std::uint32_t* begin;       // 0x00
    std::uint32_t* end;         // 0x04
    std::uint32_t* capacityEnd; // 0x08
};

static_assert(offsetof(CActiveFile_Vector8Overlay, begin) == 0x00);
static_assert(offsetof(CActiveFile_Vector8Overlay, end) == 0x04);
static_assert(offsetof(CActiveFile_Vector8Overlay, capacityEnd) == 0x08);

CActiveFile* __thiscall CActiveFile_AssignUndefinedVector8(CActiveFile* self, CActiveFile* other)
{
    if (other != self)
    {
        auto& dst = *reinterpret_cast<CActiveFile_Vector8Overlay*>(self);
        auto& src = *reinterpret_cast<CActiveFile_Vector8Overlay*>(other);

        int iVar2 = reinterpret_cast<int>(src.end);
        auto* puVar3 = src.begin;
        const std::uint32_t uVar4 = static_cast<std::uint32_t>((iVar2 - reinterpret_cast<int>(puVar3)) >> 3);

        if (static_cast<std::uint32_t>(
                (reinterpret_cast<int>(dst.capacityEnd) - reinterpret_cast<int>(dst.begin)) >> 3) < uVar4)
        {
            iVar2 = CActiveFile_AssignVector8(uVar4, puVar3, iVar2);
            CFileInstaller::CActiveFile::OnReadFinished(self);
            dst.begin = reinterpret_cast<std::uint32_t*>(iVar2);
            dst.capacityEnd = reinterpret_cast<std::uint32_t*>(iVar2 + uVar4 * 8);
        }
        else
        {
            const std::uint32_t uVar5 = static_cast<std::uint32_t>(
                (reinterpret_cast<int>(dst.end) - reinterpret_cast<int>(dst.begin)) >> 3);

            if (uVar5 < uVar4)
            {
                auto* puVar6 = dst.begin;
                iVar2 = static_cast<int>(uVar5 << 3) >> 3;
                if (0 < iVar2)
                {
                    do
                    {
                        *puVar6 = *puVar3;
                        puVar6[1] = puVar3[1];
                        puVar3 += 2;
                        puVar6 += 2;
                        iVar2 = iVar2 + -1;
                    } while (iVar2 != 0);
                }

                puVar6 = dst.end;
                auto* puVar1 = src.end;
                for (puVar3 = reinterpret_cast<std::uint32_t*>(
                         reinterpret_cast<int>(src.begin) +
                         (((reinterpret_cast<int>(puVar6) - reinterpret_cast<int>(dst.begin)) >> 3) * 8));
                     puVar3 != puVar1;
                     puVar3 += 2)
                {
                    if (puVar6 != nullptr)
                    {
                        *puVar6 = *puVar3;
                        puVar6[1] = puVar3[1];
                    }
                    puVar6 += 2;
                }
            }
            else
            {
                auto* puVar6 = dst.begin;
                iVar2 = (iVar2 - reinterpret_cast<int>(puVar3)) >> 3;
                if (0 < iVar2)
                {
                    do
                    {
                        *puVar6 = *puVar3;
                        puVar6[1] = puVar3[1];
                        puVar3 += 2;
                        puVar6 += 2;
                        iVar2 = iVar2 + -1;
                    } while (iVar2 != 0);
                }
            }
        }

        dst.end = reinterpret_cast<std::uint32_t*>(reinterpret_cast<int>(dst.begin) + uVar4 * 8);
    }

    return self;
}