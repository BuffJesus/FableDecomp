#include <cstddef>
#include <cstdint>

class CCharString;
struct EDXToken;
struct EBXToken;

struct StdVectorCCharStringOverlay {
    std::byte unknown_0x00[0x04];
    CCharString* last;     // +0x04
    CCharString* end_cap;  // +0x08
};

static_assert(offsetof(StdVectorCCharStringOverlay, last) == 0x04);
static_assert(offsetof(StdVectorCCharStringOverlay, end_cap) == 0x08);

struct LocalCCharStringStorage {
    std::byte bytes[0x1C];
};

static_assert(sizeof(LocalCCharStringStorage) == 0x1C);

extern EDXToken* __thiscall CCharString__CCharString(CCharString* self, const CCharString* other);
extern EDXToken* __thiscall CCharString__operator_assign(CCharString* self, const CCharString* other);

extern unsigned long* __cdecl std___Fill_n_unsigned_long_ptr_unsigned_int_unsigned_long(
    unsigned long* first,
    unsigned int count,
    unsigned long* value
);

// Calling-convention overlay for the observed call site:
// ECX=self, stack args=(insert_at, value, flag, count, 0).
extern void __thiscall std__vector__Reallocate__thiscall_overlay(
    StdVectorCCharStringOverlay* self,
    CCharString* insert_at,
    const CCharString* value,
    unsigned char* flag,
    unsigned int count,
    int zero
);

// Unidentified final helper observed by Ghidra as:
// _Cons_val(...)((allocator*)local_c, ppVar2, unaff_EBX)
extern void __thiscall std___Cons_val_observed_helper(
    void* local_c_as_allocator,
    EDXToken* edx_token,
    EBXToken* ebx_token
);

void __thiscall Std_Vector_InsertRange(
    StdVectorCCharStringOverlay* self,
    CCharString* insert_at,
    unsigned int count,
    const CCharString* value
) {
    if (count != 0) {
        if (static_cast<unsigned int>(self->end_cap - self->last) < count) {
            unsigned char local_5 = 0;
            std__vector__Reallocate__thiscall_overlay(self, insert_at, value, &local_5, count, 0);
        } else {
            LocalCCharStringStorage local_c_storage;
            auto* const local_c = reinterpret_cast<CCharString*>(&local_c_storage);

            EDXToken* extraout_EDX = CCharString__CCharString(local_c, value);
            EDXToken* ppVar2 = extraout_EDX;

            // Preserved incoming EBX register value as observed by the decompiler.
            EBXToken* unaff_EBX;

            CCharString* pCVar6 = self->last;
            const unsigned int uVar3 = static_cast<unsigned int>(self->last - insert_at);

            if (count < uVar3) {
                const int iVar4 = static_cast<int>(count) * 4;
                CCharString* pCVar1 = reinterpret_cast<CCharString*>(
                    reinterpret_cast<std::uintptr_t>(pCVar6) + static_cast<std::intptr_t>(count) * -4
                );

                CCharString* param_4_iter = pCVar6;
                for (CCharString* param_3_iter = pCVar1; param_3_iter != pCVar6; param_3_iter += 1) {
                    if (param_4_iter != nullptr) {
                        EDXToken* extraout_EDX_00 = CCharString__CCharString(param_4_iter, param_3_iter);
                        ppVar2 = extraout_EDX_00;
                    }
                    param_4_iter += 1;
                }

                self->last = reinterpret_cast<CCharString*>(
                    reinterpret_cast<std::uintptr_t>(self->last) + static_cast<std::uintptr_t>(iVar4)
                );

                int remaining = static_cast<int>(pCVar1 - insert_at);
                if (remaining > 0) {
                    const std::intptr_t iVar5 =
                        reinterpret_cast<std::uintptr_t>(pCVar1) - reinterpret_cast<std::uintptr_t>(pCVar6);

                    do {
                        pCVar6 -= 1;
                        EDXToken* extraout_EDX_01 = CCharString__operator_assign(
                            pCVar6,
                            reinterpret_cast<CCharString*>(reinterpret_cast<std::uintptr_t>(pCVar6) + iVar5)
                        );
                        ppVar2 = extraout_EDX_01;
                        --remaining;
                    } while (remaining != 0);
                }

                CCharString* const fill_end = reinterpret_cast<CCharString*>(
                    reinterpret_cast<std::uintptr_t>(insert_at) + static_cast<std::uintptr_t>(iVar4)
                );

                for (; insert_at != fill_end; insert_at += 1) {
                    EDXToken* extraout_EDX_02 = CCharString__operator_assign(insert_at, local_c);
                    ppVar2 = extraout_EDX_02;
                }
            } else {
                std___Fill_n_unsigned_long_ptr_unsigned_int_unsigned_long(
                    reinterpret_cast<unsigned long*>(pCVar6),
                    static_cast<unsigned int>(count - uVar3),
                    reinterpret_cast<unsigned long*>(local_c)
                );

                self->last = reinterpret_cast<CCharString*>(
                    reinterpret_cast<std::uintptr_t>(self->last) +
                    static_cast<std::uintptr_t>(static_cast<int>(count - uVar3) * 4)
                );

                CCharString* this_iter = self->last;
                ppVar2 = ppVar2;
                for (CCharString* pCVar1 = insert_at; pCVar1 != pCVar6; pCVar1 += 1) {
                    if (this_iter != nullptr) {
                        EDXToken* extraout_EDX_04 = CCharString__CCharString(this_iter, pCVar1);
                        ppVar2 = extraout_EDX_04;
                    }
                    this_iter += 1;
                }

                self->last = reinterpret_cast<CCharString*>(
                    reinterpret_cast<std::uintptr_t>(self->last) +
                    static_cast<std::uintptr_t>(uVar3 * 4)
                );

                for (; insert_at != pCVar6; insert_at += 1) {
                    EDXToken* extraout_EDX_05 = CCharString__operator_assign(insert_at, local_c);
                    ppVar2 = extraout_EDX_05;
                }
            }

            std___Cons_val_observed_helper(&local_c_storage, ppVar2, unaff_EBX);
        }
    }
}