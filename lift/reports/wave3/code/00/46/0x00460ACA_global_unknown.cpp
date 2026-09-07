#include <cstddef>
#include <cstdint>

struct CActiveFile;

struct Vector20Element {
    std::uint32_t dword0;
    std::uint32_t dword1;
    std::uint32_t dword2;
    std::uint32_t dword3;
    std::uint32_t dword4;
};
static_assert(sizeof(Vector20Element) == 0x14);

struct CActiveFileOverlay {
    Vector20Element* begin;    // 0x00
    Vector20Element* end;      // 0x04
    Vector20Element* capacity; // 0x08

    Vector20Element* AssignVector20(std::uint32_t count, Vector20Element* srcBegin, Vector20Element* srcEnd);
    void OnReadFinished();
};

static_assert(offsetof(CActiveFileOverlay, begin) == 0x00);
static_assert(offsetof(CActiveFileOverlay, end) == 0x04);
static_assert(offsetof(CActiveFileOverlay, capacity) == 0x08);
static_assert(sizeof(CActiveFileOverlay) == 0x0C);

CActiveFile* __thiscall CActiveFile_AssignUndefinedVector20(CActiveFile* self_, CActiveFile* other_)
{
    auto* self = reinterpret_cast<CActiveFileOverlay*>(self_);
    auto* other = reinterpret_cast<CActiveFileOverlay*>(other_);

    if (other_ != self_) {
        int iVar2 = static_cast<int>(reinterpret_cast<std::intptr_t>(other->end));
        Vector20Element* local_8 = other->begin;
        std::uint32_t uVar1 =
            static_cast<std::uint32_t>((iVar2 - static_cast<int>(reinterpret_cast<std::intptr_t>(local_8))) / 0x14);

        if (static_cast<std::uint32_t>(
                (static_cast<int>(reinterpret_cast<std::intptr_t>(self->capacity)) -
                 static_cast<int>(reinterpret_cast<std::intptr_t>(self->begin))) /
                0x14) < uVar1) {
            Vector20Element* const newBegin = self->AssignVector20(uVar1, local_8, other->end);
            self->OnReadFinished();
            self->begin = newBegin;
            self->capacity = reinterpret_cast<Vector20Element*>(
                static_cast<std::intptr_t>(reinterpret_cast<std::intptr_t>(newBegin) + static_cast<int>(uVar1 * 0x14)));
        } else {
            std::uint32_t uVar3 =
                static_cast<std::uint32_t>((static_cast<int>(reinterpret_cast<std::intptr_t>(self->end)) -
                                            static_cast<int>(reinterpret_cast<std::intptr_t>(self->begin))) /
                                           0x14);

            if (uVar3 < uVar1) {
                iVar2 = static_cast<int>(uVar3 * 0x14) / 0x14;
                Vector20Element* local_c = self->begin;

                if (0 < iVar2) {
                    do {
                        --iVar2;
                        *local_c = *local_8;
                        ++local_c;
                        ++local_8;
                    } while (iVar2 != 0);
                }

                Vector20Element* puVar8 = other->end;
                Vector20Element* puVar6 = self->end;
                for (Vector20Element* puVar4 = reinterpret_cast<Vector20Element*>(
                         static_cast<std::intptr_t>(reinterpret_cast<std::intptr_t>(other->begin) +
                                                    ((static_cast<int>(reinterpret_cast<std::intptr_t>(puVar6)) -
                                                      static_cast<int>(reinterpret_cast<std::intptr_t>(self->begin))) /
                                                     0x14) *
                                                        0x14));
                     puVar4 != puVar8;
                     ++puVar4) {
                    if (puVar6 != nullptr) {
                        *puVar6 = *puVar4;
                    }
                    ++puVar6;
                }
            } else {
                iVar2 = (iVar2 - static_cast<int>(reinterpret_cast<std::intptr_t>(local_8))) / 0x14;
                auto* param_2_copy = local_8;
                local_8 = self->begin;

                if (0 < iVar2) {
                    do {
                        --iVar2;
                        *local_8 = *param_2_copy;
                        param_2_copy = reinterpret_cast<Vector20Element*>(
                            static_cast<std::intptr_t>(reinterpret_cast<std::intptr_t>(param_2_copy) + 0x14));
                        ++local_8;
                    } while (iVar2 != 0);
                }
            }
        }

        self->end = reinterpret_cast<Vector20Element*>(
            static_cast<std::intptr_t>(reinterpret_cast<std::intptr_t>(self->begin) + static_cast<int>(uVar1 * 0x14)));
    }

    return self_;
}