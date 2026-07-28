#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

static_assert(sizeof(void*) == 4, "This reversal models a 32-bit build.");

enum EHeroMorphType : int;
struct CParticleMorphs {
    struct CEntry;
};

using ParticleMorphPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;
using ParticleMorphPairAllocator = std::allocator<ParticleMorphPair>;

// Observed call shape:
//   ecx = destination slot
//   edx = current source pair
//   edi = live page cursor
// Post-call, the caller consumes the updated source pointer from EDX.
extern void __fastcall
std___Cons_val__allocator_pair_EHeroMorphType_CParticleMorphs_CEntry(
    ParticleMorphPairAllocator* dst,
    ParticleMorphPair* src,
    void* observed_edi);

void __fastcall Vector_CopyPairRange(
    void*,
    ParticleMorphPair* src_current,
    ParticleMorphPairAllocator* dst_current,
    void*,
    ParticleMorphPairAllocator* page_limit,
    ParticleMorphPairAllocator** page_cursor,
    ParticleMorphPairAllocator* dst_end)
{
    auto* dst = reinterpret_cast<std::byte*>(dst_current);
    auto* limit = reinterpret_cast<std::byte*>(page_limit);
    auto* end = reinterpret_cast<std::byte*>(dst_end);

    while (dst != end) {
#if defined(_MSC_VER) && defined(_M_IX86)
        ParticleMorphPair* next_src;
        __asm {
            mov ecx, dst
            mov edx, src_current
            mov edi, page_cursor
            call std___Cons_val__allocator_pair_EHeroMorphType_CParticleMorphs_CEntry
            mov next_src, edx
        }
#else
#error Vector_CopyPairRange reversal requires MSVC x86 inline assembly to preserve the observed EDX post-call update.
#endif

        dst += 8;
        src_current = next_src;

        if (dst == limit) {
            dst = reinterpret_cast<std::byte*>(page_cursor[1]);
            ++page_cursor;
            limit = dst + 0x80;
        }
    }
}