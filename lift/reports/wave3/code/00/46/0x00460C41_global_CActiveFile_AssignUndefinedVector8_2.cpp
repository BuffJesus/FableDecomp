#include <cstddef>
#include <cstdint>

struct CActiveFile;

extern std::uint32_t* Vector_AllocateAndCopy_8(std::uint32_t count, const std::uint32_t* begin, std::uint32_t* end);

struct CActiveFileVector8Overlay {
    std::uint32_t* begin;        // 0x00
    std::uint32_t* end;          // 0x04
    std::uint32_t* capacityEnd;  // 0x08
};
static_assert(offsetof(CActiveFileVector8Overlay, begin) == 0x00);
static_assert(offsetof(CActiveFileVector8Overlay, end) == 0x04);
static_assert(offsetof(CActiveFileVector8Overlay, capacityEnd) == 0x08);

namespace CFileInstaller {
struct CActiveFile {
    void OnReadFinished();
};
}

CActiveFile* __thiscall CActiveFile_AssignUndefinedVector8_2(CActiveFile* param_1, CActiveFile* param_2)
{
    if (param_2 != param_1) {
        auto* const dst = reinterpret_cast<CActiveFileVector8Overlay*>(param_1);
        auto* const src = reinterpret_cast<CActiveFileVector8Overlay*>(param_2);

        std::uint32_t* puVar3 = src->begin;
        std::uint32_t* const srcEnd = src->end;
        const std::uint32_t uVar4 =
            static_cast<std::uint32_t>((reinterpret_cast<std::uintptr_t>(srcEnd) -
                                        reinterpret_cast<std::uintptr_t>(puVar3)) >> 3);

        if (static_cast<std::uint32_t>((reinterpret_cast<std::uintptr_t>(dst->capacityEnd) -
                                        reinterpret_cast<std::uintptr_t>(dst->begin)) >> 3) < uVar4) {
            std::uint32_t* const newBegin = Vector_AllocateAndCopy_8(uVar4, puVar3, srcEnd);
            reinterpret_cast<CFileInstaller::CActiveFile*>(param_1)->OnReadFinished();
            dst->begin = newBegin;
            dst->capacityEnd = newBegin + uVar4 * 2;
        } else {
            const std::uint32_t uVar5 =
                static_cast<std::uint32_t>((reinterpret_cast<std::uintptr_t>(dst->end) -
                                            reinterpret_cast<std::uintptr_t>(dst->begin)) >> 3);

            if (uVar5 < uVar4) {
                std::uint32_t* puVar6 = dst->begin;
                int iVar2 = static_cast<int>((uVar5 << 3) >> 3);
                if (0 < iVar2) {
                    do {
                        puVar6[0] = puVar3[0];
                        puVar6[1] = puVar3[1];
                        puVar3 += 2;
                        puVar6 += 2;
                        --iVar2;
                    } while (iVar2 != 0);
                }

                puVar6 = dst->end;
                std::uint32_t* const puVar1 = src->end;
                for (puVar3 = reinterpret_cast<std::uint32_t*>(
                         reinterpret_cast<std::uintptr_t>(src->begin) +
                         (((reinterpret_cast<std::uintptr_t>(puVar6) -
                            reinterpret_cast<std::uintptr_t>(dst->begin)) >> 3) * 8));
                     puVar3 != puVar1;
                     puVar3 += 2) {
                    if (puVar6 != nullptr) {
                        puVar6[0] = puVar3[0];
                        puVar6[1] = puVar3[1];
                    }
                    puVar6 += 2;
                }
            } else {
                std::uint32_t* puVar6 = dst->begin;
                int iVar2 = static_cast<int>((reinterpret_cast<std::uintptr_t>(srcEnd) -
                                              reinterpret_cast<std::uintptr_t>(puVar3)) >> 3);
                if (0 < iVar2) {
                    do {
                        puVar6[0] = puVar3[0];
                        puVar6[1] = puVar3[1];
                        puVar3 += 2;
                        puVar6 += 2;
                        --iVar2;
                    } while (iVar2 != 0);
                }
            }
        }

        dst->end = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uintptr_t>(dst->begin) + uVar4 * 8);
    }

    return param_1;
}