#include <cstddef>
#include <cstdint>

using undefined4 = std::uint32_t;

struct CActiveFile;

struct CActiveFile_Vector4Overlay {
    undefined4* field_0;
    undefined4* field_4;
    undefined4* field_8;
};

static_assert(offsetof(CActiveFile_Vector4Overlay, field_0) == 0x0);
static_assert(offsetof(CActiveFile_Vector4Overlay, field_4) == 0x4);
static_assert(offsetof(CActiveFile_Vector4Overlay, field_8) == 0x8);

extern int __thiscall CActiveFile_AssignVector4(
    CActiveFile* self,
    std::uint32_t count,
    undefined4* first,
    int last);
extern void __thiscall CFileInstaller__CActiveFile__OnReadFinished(CActiveFile* self);

CActiveFile* __thiscall CActiveFile_AssignUndefinedVector4(CActiveFile* self, CActiveFile* other)
{
    if (other != self) {
        auto* const dst = reinterpret_cast<CActiveFile_Vector4Overlay*>(self);
        auto* const src = reinterpret_cast<CActiveFile_Vector4Overlay*>(other);

        int iVar2 = reinterpret_cast<int>(src->field_4);
        undefined4* puVar3 = src->field_0;
        const std::uint32_t uVar4 =
            static_cast<std::uint32_t>((iVar2 - reinterpret_cast<int>(puVar3)) >> 2);

        if (static_cast<std::uint32_t>(
                (reinterpret_cast<int>(dst->field_8) - reinterpret_cast<int>(dst->field_0)) >> 2) < uVar4) {
            iVar2 = CActiveFile_AssignVector4(self, uVar4, puVar3, iVar2);
            CFileInstaller__CActiveFile__OnReadFinished(self);
            dst->field_0 = reinterpret_cast<undefined4*>(iVar2);
            dst->field_8 = reinterpret_cast<undefined4*>(iVar2 + uVar4 * 4);
        } else {
            const std::uint32_t uVar5 =
                static_cast<std::uint32_t>((reinterpret_cast<int>(dst->field_4) - reinterpret_cast<int>(dst->field_0)) >> 2);

            if (uVar5 < uVar4) {
                undefined4* puVar6 = dst->field_0;
                iVar2 = (int)(uVar5 << 2) >> 2;
                if (0 < iVar2) {
                    do {
                        *puVar6 = *puVar3;
                        ++puVar3;
                        ++puVar6;
                        iVar2 = iVar2 + -1;
                    } while (iVar2 != 0);
                }

                puVar6 = dst->field_4;
                undefined4* const puVar1 = src->field_4;
                for (puVar3 = reinterpret_cast<undefined4*>(
                         reinterpret_cast<int>(src->field_0) +
                         (((reinterpret_cast<int>(puVar6) - reinterpret_cast<int>(dst->field_0)) >> 2) * 4));
                     puVar3 != puVar1;
                     ++puVar3) {
                    if (puVar6 != nullptr) {
                        *puVar6 = *puVar3;
                    }
                    ++puVar6;
                }
            } else {
                undefined4* puVar6 = dst->field_0;
                iVar2 = (iVar2 - reinterpret_cast<int>(puVar3)) >> 2;
                if (0 < iVar2) {
                    do {
                        *puVar6 = *puVar3;
                        ++puVar3;
                        ++puVar6;
                        iVar2 = iVar2 + -1;
                    } while (iVar2 != 0);
                }
            }
        }

        dst->field_4 = reinterpret_cast<undefined4*>(reinterpret_cast<int>(dst->field_0) + uVar4 * 4);
    }

    return self;
}