#include <cstddef>
#include <cstdint>
#include <cstdlib>

struct CMemoryBlock;
struct CCharString;
struct LTextBinNode;
struct LTextGroup;

enum EHeroMorphType : std::int32_t;

class CParticleMorphs {
public:
    class CEntry;
};

namespace CSimpleMemoryPool {
struct CMemoryBlock;
}

extern void __thiscall CCharString_destructor(CCharString* self);
extern void __thiscall BuildTreeArray_LTextGroup(void* self, LTextBinNode* root);
extern void __thiscall Unknown_0056D2XX(void* self);
extern void* PTR__vector_deleting_destructor__01230ba0;

namespace CSimpleMemoryPool {
extern void __thiscall CMemoryBlock_destructor(CMemoryBlock* self);
}

struct TreeHeaderOverlay {
    std::byte pad00[0x4];
    LTextBinNode* root;  // +0x4
    void* field08;       // +0x8
    void* field0C;       // +0xC
};
static_assert(offsetof(TreeHeaderOverlay, root) == 0x4);
static_assert(offsetof(TreeHeaderOverlay, field08) == 0x8);
static_assert(offsetof(TreeHeaderOverlay, field0C) == 0xC);

struct TreeWalkOverlay {
    TreeHeaderOverlay* header; // +0x0
    std::uint32_t count;       // +0x4
};
static_assert(offsetof(TreeWalkOverlay, header) == 0x0);
static_assert(offsetof(TreeWalkOverlay, count) == 0x4);

struct OpaquePairMorphSubobject {
    std::byte raw[0x4];
};
static_assert(sizeof(OpaquePairMorphSubobject) == 0x4);

struct CMemoryBlock_Overlay {
    void** vfptr;                        // +0x000
    std::byte pad004[0x03C - 0x004];
    CCharString string03C;               // +0x03C
    CCharString string040;               // +0x040
    CCharString string044;               // +0x044
    CCharString string048;               // +0x048
    CCharString string04C;               // +0x04C
    CCharString string050;               // +0x050
    CCharString string054;               // +0x054
    CCharString string058;               // +0x058
    CCharString string05C;               // +0x05C
    CCharString string060;               // +0x060
    CCharString string064;               // +0x064
    CCharString string068;               // +0x068
    CCharString string06C;               // +0x06C
    CCharString string070;               // +0x070
    CCharString string074;               // +0x074
    CCharString string078;               // +0x078
    CCharString string07C;               // +0x07C
    CCharString string080;               // +0x080
    CCharString string084;               // +0x084
    CCharString string088;               // +0x088
    CCharString string08C;               // +0x08C
    CCharString string090;               // +0x090
    CCharString string094;               // +0x094
    CCharString string098;               // +0x098
    CCharString string09C;               // +0x09C
    CCharString string0A0;               // +0x0A0
    union {
        CCharString string0A4;           // +0x0A4
        void* string0A4_firstWord;       // +0x0A4, later freed after destruction
    };
    CCharString string0A8;               // +0x0A8
    CCharString string0AC;               // +0x0AC
    CCharString string0B0;               // +0x0B0
    CCharString string0B4;               // +0x0B4
    CCharString string0B8;               // +0x0B8
    CCharString string0BC;               // +0x0BC
    std::byte pad0C0[0x0D0 - 0x0C0];
    CCharString string0D0;               // +0x0D0
    CCharString string0D4;               // +0x0D4
    CCharString string0D8;               // +0x0D8
    CCharString string0DC;               // +0x0DC
    std::byte pad0E0[0x100 - 0x0E0];
    CCharString string100;               // +0x100
    CCharString string104;               // +0x104
    CCharString string108;               // +0x108
    CCharString string10C;               // +0x10C
    CCharString string110;               // +0x110
    CCharString string114;               // +0x114
    CCharString string118;               // +0x118
    CCharString string11C;               // +0x11C
    CCharString string120;               // +0x120
    CCharString string124;               // +0x124
    CCharString string128;               // +0x128
    CCharString string12C;               // +0x12C
    CCharString string130;               // +0x130
    CCharString string134;               // +0x134
    CCharString string138;               // +0x138
    CCharString string13C;               // +0x13C
    CCharString string140;               // +0x140
    CCharString string144;               // +0x144
    std::byte pad148[0x150 - 0x148];
    CCharString string150;               // +0x150
    CCharString string154;               // +0x154
    CCharString string158;               // +0x158
    CCharString string15C;               // +0x15C
    CCharString string160;               // +0x160
    CCharString string164;               // +0x164
    CCharString string168;               // +0x168
    CCharString string16C;               // +0x16C
    CCharString string170;               // +0x170
    CCharString string174;               // +0x174
    CCharString string178;               // +0x178
    CCharString string17C;               // +0x17C
    CCharString string180;               // +0x180
    CCharString string184;               // +0x184
    TreeWalkOverlay tree188;             // +0x188
    OpaquePairMorphSubobject sub194;     // +0x194
    OpaquePairMorphSubobject sub198;     // +0x198
    OpaquePairMorphSubobject sub19C;     // +0x19C
    OpaquePairMorphSubobject sub1A0;     // +0x1A0
    OpaquePairMorphSubobject sub1A4;     // +0x1A4
    OpaquePairMorphSubobject sub1A8;     // +0x1A8
    OpaquePairMorphSubobject sub1AC;     // +0x1AC
    OpaquePairMorphSubobject sub1B0;     // +0x1B0
    OpaquePairMorphSubobject sub1B4;     // +0x1B4
    OpaquePairMorphSubobject sub1B8;     // +0x1B8
    OpaquePairMorphSubobject sub1BC;     // +0x1BC
    OpaquePairMorphSubobject sub1C0;     // +0x1C0
    OpaquePairMorphSubobject sub1C4;     // +0x1C4
    OpaquePairMorphSubobject sub1C8;     // +0x1C8
    CCharString string1CC;               // +0x1CC
    std::byte pad1D0[0x21C - 0x1D0];
    TreeWalkOverlay tree21C;             // +0x21C
};
static_assert(offsetof(CMemoryBlock_Overlay, string03C) == 0x03C);
static_assert(offsetof(CMemoryBlock_Overlay, string0A4) == 0x0A4);
static_assert(offsetof(CMemoryBlock_Overlay, tree188) == 0x188);
static_assert(offsetof(CMemoryBlock_Overlay, sub194) == 0x194);
static_assert(offsetof(CMemoryBlock_Overlay, sub1C8) == 0x1C8);
static_assert(offsetof(CMemoryBlock_Overlay, string1CC) == 0x1CC);
static_assert(offsetof(CMemoryBlock_Overlay, tree21C) == 0x21C);

void __fastcall CMemoryBlock_Destructor(CMemoryBlock* self) {
    auto* const block = reinterpret_cast<CMemoryBlock_Overlay*>(self);

    {
        TreeWalkOverlay* const walk = &block->tree21C;
        if (walk->count != 0) {
            BuildTreeArray_LTextGroup(walk, walk->header->root);
            walk->header->field08 = walk->header;
            walk->header->root = nullptr;
            walk->header->field0C = walk->header;
            walk->count = 0;
        }
        if (walk->header != nullptr) {
            std::free(walk->header);
        }
    }

    CCharString_destructor(&block->string1CC);

    Unknown_0056D2XX(&block->sub1C8);
    Unknown_0056D2XX(&block->sub1C4);
    Unknown_0056D2XX(&block->sub1C0);
    Unknown_0056D2XX(&block->sub1BC);
    Unknown_0056D2XX(&block->sub1B8);
    Unknown_0056D2XX(&block->sub1B4);
    Unknown_0056D2XX(&block->sub1B0);
    Unknown_0056D2XX(&block->sub1AC);
    Unknown_0056D2XX(&block->sub1A8);
    Unknown_0056D2XX(&block->sub1A4);
    Unknown_0056D2XX(&block->sub1A0);
    Unknown_0056D2XX(&block->sub19C);
    Unknown_0056D2XX(&block->sub198);
    Unknown_0056D2XX(&block->sub194);

    {
        TreeWalkOverlay* const walk = &block->tree188;
        if (walk->count != 0) {
            BuildTreeArray_LTextGroup(walk, walk->header->root);
            walk->header->field08 = walk->header;
            walk->header->root = nullptr;
            walk->header->field0C = walk->header;
            walk->count = 0;
        }
        if (walk->header != nullptr) {
            std::free(walk->header);
        }
    }

    CCharString_destructor(&block->string184);
    CCharString_destructor(&block->string180);
    CCharString_destructor(&block->string17C);
    CCharString_destructor(&block->string178);
    CCharString_destructor(&block->string174);
    CCharString_destructor(&block->string170);
    CCharString_destructor(&block->string16C);
    CCharString_destructor(&block->string168);
    CCharString_destructor(&block->string164);
    CCharString_destructor(&block->string160);
    CCharString_destructor(&block->string15C);
    CCharString_destructor(&block->string158);
    CCharString_destructor(&block->string154);
    CCharString_destructor(&block->string150);
    CCharString_destructor(&block->string144);
    CCharString_destructor(&block->string140);
    CCharString_destructor(&block->string13C);
    CCharString_destructor(&block->string138);
    CCharString_destructor(&block->string134);
    CCharString_destructor(&block->string130);
    CCharString_destructor(&block->string12C);
    CCharString_destructor(&block->string128);
    CCharString_destructor(&block->string124);
    CCharString_destructor(&block->string120);
    CCharString_destructor(&block->string11C);
    CCharString_destructor(&block->string118);
    CCharString_destructor(&block->string114);
    CCharString_destructor(&block->string110);
    CCharString_destructor(&block->string10C);
    CCharString_destructor(&block->string108);
    CCharString_destructor(&block->string104);
    CCharString_destructor(&block->string100);
    CCharString_destructor(&block->string0DC);
    CCharString_destructor(&block->string0D8);
    CCharString_destructor(&block->string0D4);
    CCharString_destructor(&block->string0D0);
    CCharString_destructor(&block->string0BC);
    CCharString_destructor(&block->string0B8);
    CCharString_destructor(&block->string0B4);
    CCharString_destructor(&block->string0B0);
    CCharString_destructor(&block->string0AC);
    CCharString_destructor(&block->string0A8);
    CCharString_destructor(&block->string0A4);
    CCharString_destructor(&block->string0A0);
    CCharString_destructor(&block->string09C);
    CCharString_destructor(&block->string098);
    CCharString_destructor(&block->string094);
    CCharString_destructor(&block->string090);
    CCharString_destructor(&block->string08C);
    CCharString_destructor(&block->string088);
    CCharString_destructor(&block->string084);
    CCharString_destructor(&block->string080);
    CCharString_destructor(&block->string07C);
    CCharString_destructor(&block->string078);
    CCharString_destructor(&block->string074);
    CCharString_destructor(&block->string070);
    CCharString_destructor(&block->string06C);
    CCharString_destructor(&block->string068);
    CCharString_destructor(&block->string064);
    CCharString_destructor(&block->string060);
    CCharString_destructor(&block->string05C);
    CCharString_destructor(&block->string058);
    CCharString_destructor(&block->string054);
    CCharString_destructor(&block->string050);
    CCharString_destructor(&block->string04C);
    CCharString_destructor(&block->string048);
    CCharString_destructor(&block->string044);
    CCharString_destructor(&block->string040);
    CCharString_destructor(&block->string03C);

    if (block->string0A4_firstWord != nullptr) {
        std::free(block->string0A4_firstWord);
    }

    block->vfptr = reinterpret_cast<void**>(&PTR__vector_deleting_destructor__01230ba0);
    CSimpleMemoryPool::CMemoryBlock_destructor(reinterpret_cast<CSimpleMemoryPool::CMemoryBlock*>(self));
}