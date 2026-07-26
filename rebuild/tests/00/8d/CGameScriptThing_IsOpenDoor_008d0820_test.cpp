// Standalone behavioral test for CGameScriptThing::IsOpenDoor @ 008d0820.
// Defines its own copy of the function + recording stubs for the externs,
// exercises every branch, and prints exactly one pass line on success.
#include <cstdio>

struct CScriptThingEntity;
struct MapTreeNode;

// ---- recording stubs / state for the externs (relocation-masked in the binary) ----
static unsigned char  g_entMem[0x200];
static void*          g_entToReturn = 0;      // what GetEnt returns
static int            g_getEntCalls = 0;

struct DoorMapMem {                           // container at entity+0x44
    unsigned char pad0[0x4];
    MapTreeNode*  endNode;                     // at +0x4
    unsigned char pad2[0x40];
};

struct NodeMem {                              // node: [+0x0]=key int, [+0x4]=value ptr
    int   key;                                 // +0x0
    char* value;                               // +0x4
};

static NodeMem*  g_lbToReturn = 0;            // what LowerBound returns
static int       g_lbCalls    = 0;
static int       g_lbKeySeen  = 0;

struct DoorMap {
    MapTreeNode* LowerBound(int* keyPtr);
};

static CScriptThingEntity* __fastcall GetEnt(void* /*subobj*/)
{
    ++g_getEntCalls;
    return (CScriptThingEntity*)g_entToReturn;
}

MapTreeNode* DoorMap::LowerBound(int* keyPtr)
{
    ++g_lbCalls;
    g_lbKeySeen = *keyPtr;                      // key passed by reference (slot preloaded 0x26)
    return (MapTreeNode*)g_lbToReturn;
}

// ---- the reconstructed function under test (identical to source_cpp) ----
static bool __fastcall IsOpenDoor(void* thisptr)
{
    char* subobj = (char*)thisptr + 0xc;
    char* ent;

    if (GetEnt(subobj)
        && !(*(unsigned char*)((char*)GetEnt(subobj) + 0x91) & 0x1)
        && (ent = (char*)GetEnt(subobj), (*(unsigned char*)(ent + 0x24) & 0x40)))
    {
        DoorMap* mapObj = (DoorMap*)(ent + 0x44);

        int key = 0x26;
        MapTreeNode* node = mapObj->LowerBound(&key);
        MapTreeNode* endNode = *(MapTreeNode**)((char*)mapObj + 0x4);

        if (node != endNode && *(int*)node <= 0x26)
            ;
        else
            node = endNode;

        char* value = *(char**)((char*)node + 0x4);

        return *(unsigned char*)(value + 0x12) == 0
            && *(unsigned char*)(value + 0x13) == 0
            && *(unsigned char*)(value + 0x10) != 0;
    }
    return false;
}

// ---- harness ----
static char g_this[0x20];   // 'this'; subobject at +0xc used only as ecx for GetEnt

static void reset()
{
    for (int i = 0; i < (int)sizeof(g_entMem); ++i) g_entMem[i] = 0;
    g_entToReturn = 0; g_getEntCalls = 0;
    g_lbToReturn = 0; g_lbCalls = 0; g_lbKeySeen = 0;
}

int main()
{
    int fails = 0;
    unsigned char* E = g_entMem;
    DoorMapMem* mp = (DoorMapMem*)(E + 0x44);

    static NodeMem realNode;
    static NodeMem endNode;
    static unsigned char valMem[0x20];

    // Case 1: GetEnt returns null -> false, exactly 1 call.
    reset();
    g_entToReturn = 0;
    if (IsOpenDoor(g_this) != false) { printf("c1 ret\n"); ++fails; }
    if (g_getEntCalls != 1)          { printf("c1 calls=%d\n", g_getEntCalls); ++fails; }

    // Case 2: entity present but [+0x91]&1 set -> false, 2 calls.
    reset();
    g_entToReturn = E;
    E[0x91] = 0x1;
    if (IsOpenDoor(g_this) != false) { printf("c2 ret\n"); ++fails; }
    if (g_getEntCalls != 2)          { printf("c2 calls=%d\n", g_getEntCalls); ++fails; }

    // Case 3: [+0x91]&1 clear but [+0x24]&0x40 clear -> false, 3 calls.
    reset();
    g_entToReturn = E;
    E[0x91] = 0x0; E[0x24] = 0x00;
    if (IsOpenDoor(g_this) != false) { printf("c3 ret\n"); ++fails; }
    if (g_getEntCalls != 3)          { printf("c3 calls=%d\n", g_getEntCalls); ++fails; }

    // Case 4: full success path (value[+0x12]==0 && [+0x13]==0 && [+0x10]!=0).
    reset();
    g_entToReturn = E;
    E[0x91] = 0x0; E[0x24] = 0x40;
    mp->endNode = (MapTreeNode*)&endNode;
    realNode.key = 0x26; realNode.value = (char*)valMem;
    g_lbToReturn = &realNode;
    for (int i=0;i<0x20;++i) valMem[i]=0;
    valMem[0x10] = 0x5;
    if (IsOpenDoor(g_this) != true) { printf("c4 ret\n"); ++fails; }
    if (g_getEntCalls != 3)         { printf("c4 calls=%d\n", g_getEntCalls); ++fails; }
    if (g_lbCalls != 1)             { printf("c4 lb=%d\n", g_lbCalls); ++fails; }
    if (g_lbKeySeen != 0x26)        { printf("c4 key=%x\n", g_lbKeySeen); ++fails; }

    // Case 5: value[+0x12]!=0 -> false.
    reset();
    g_entToReturn = E; E[0x91]=0; E[0x24]=0x40;
    mp->endNode = (MapTreeNode*)&endNode;
    realNode.key = 0x26; realNode.value = (char*)valMem;
    g_lbToReturn = &realNode;
    for (int i=0;i<0x20;++i) valMem[i]=0;
    valMem[0x10]=0x5; valMem[0x12]=0x1;
    if (IsOpenDoor(g_this) != false) { printf("c5 ret\n"); ++fails; }

    // Case 6: value[+0x13]!=0 -> false.
    reset();
    g_entToReturn = E; E[0x91]=0; E[0x24]=0x40;
    mp->endNode = (MapTreeNode*)&endNode;
    realNode.key = 0x26; realNode.value = (char*)valMem;
    g_lbToReturn = &realNode;
    for (int i=0;i<0x20;++i) valMem[i]=0;
    valMem[0x10]=0x5; valMem[0x13]=0x1;
    if (IsOpenDoor(g_this) != false) { printf("c6 ret\n"); ++fails; }

    // Case 7: value[+0x10]==0 -> false.
    reset();
    g_entToReturn = E; E[0x91]=0; E[0x24]=0x40;
    mp->endNode = (MapTreeNode*)&endNode;
    realNode.key = 0x26; realNode.value = (char*)valMem;
    g_lbToReturn = &realNode;
    for (int i=0;i<0x20;++i) valMem[i]=0;
    valMem[0x10]=0x0;
    if (IsOpenDoor(g_this) != false) { printf("c7 ret\n"); ++fails; }

    // Case 8: LowerBound returns the end node -> node stays end; use endNode.value.
    reset();
    g_entToReturn = E; E[0x91]=0; E[0x24]=0x40;
    mp->endNode = (MapTreeNode*)&endNode;
    endNode.key = 0x999; endNode.value = (char*)valMem;
    g_lbToReturn = &endNode;
    for (int i=0;i<0x20;++i) valMem[i]=0;
    valMem[0x10]=0x7;
    if (IsOpenDoor(g_this) != true) { printf("c8 ret\n"); ++fails; }

    // Case 9: node found but node key > 0x26 -> falls back to end node's value.
    reset();
    g_entToReturn = E; E[0x91]=0; E[0x24]=0x40;
    mp->endNode = (MapTreeNode*)&endNode;
    static unsigned char endVal[0x20];
    for (int i=0;i<0x20;++i) endVal[i]=0;
    endVal[0x10]=0x3;
    endNode.key = 0x111; endNode.value = (char*)endVal;
    realNode.key = 0x30; realNode.value = (char*)valMem;   // key>0x26 -> use end
    for (int i=0;i<0x20;++i) valMem[i]=0;
    valMem[0x10]=0x0;                                       // if wrongly used -> false
    g_lbToReturn = &realNode;
    if (IsOpenDoor(g_this) != true) { printf("c9 ret (should use endNode value)\n"); ++fails; }

    if (fails == 0) {
        printf("FSE2_008d0820_TEST PASS\n");
        return 0;
    }
    printf("FAILS=%d\n", fails);
    return 1;
}