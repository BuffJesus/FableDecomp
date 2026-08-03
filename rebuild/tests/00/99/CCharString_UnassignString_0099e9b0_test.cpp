#include <cstdio>
#include <cstdlib>
#include <cstring>

#pragma pack(push, 1)
struct CCharStringData
{
    char* text;
    unsigned int unknown04;
    unsigned int unknown08;
    unsigned char flags0C;
    int owners;
};
#pragma pack(pop)

struct CCharString
{
    CCharStringData* storage_;

    void __fastcall UnassignString();
};

void __fastcall CCharString::UnassignString()
{
    CCharStringData* p = storage_;
    if (p != 0)
    {
        if (--p->owners <= 0)
        {
            CCharStringData* data = storage_;
            if (data != 0)
            {
                if (data->text != 0)
                {
                    delete[] data->text;
                    data->text = 0;
                }
                unsigned int flag = data->unknown08 & 0x80000000;
                data->unknown04 = 0;
                data->unknown08 = flag;
                delete data;
            }
        }
        storage_ = 0;
    }
}

static int g_freeArrayCount = 0;
static int g_freeScalarCount = 0;

int main()
{
    // Case 1: null storage_ pointer -- no-op.
    CCharString cs1;
    cs1.storage_ = 0;
    cs1.UnassignString();
    if (cs1.storage_ != 0) { printf("FAIL case1\n"); return 1; }

    // Case 2: owners=2 (shared) -- decrements to 1, does NOT free, resets storage_ to 0.
    CCharStringData* d2 = (CCharStringData*)malloc(sizeof(CCharStringData));
    char* txt2 = (char*)malloc(8);
    strcpy(txt2, "hi");
    d2->text = txt2;
    d2->unknown04 = 0x1234;
    d2->unknown08 = 0x80000005;
    d2->flags0C = 0;
    d2->owners = 2;
    CCharString cs2;
    cs2.storage_ = d2;
    cs2.UnassignString();
    if (cs2.storage_ != 0) { printf("FAIL case2 storage not reset\n"); return 1; }
    if (d2->owners != 1) { printf("FAIL case2 owners=%d\n", d2->owners); return 1; }
    if (d2->text != txt2) { printf("FAIL case2 text freed early\n"); return 1; }
    free(txt2);
    free(d2);

    // Case 3: owners=1 (last owner) -- decrements to 0, frees text via delete[],
    // clears unknown04, masks unknown08 to only the high bit, frees the block itself.
    CCharStringData* d3 = new CCharStringData();
    char* txt3 = new char[8];
    strcpy(txt3, "bye");
    d3->text = txt3;
    d3->unknown04 = 0xAAAA;
    d3->unknown08 = 0x80000007;
    d3->flags0C = 0;
    d3->owners = 1;
    CCharString cs3;
    cs3.storage_ = d3;
    cs3.UnassignString();
    if (cs3.storage_ != 0) { printf("FAIL case3 storage not reset\n"); return 1; }
    // d3 and txt3 are now freed memory -- we cannot safely dereference them again,
    // so this test only validates the reachable postconditions (storage_ reset).

    // Case 4: owners=0 already (<=0 branch with no text pointer) -- frees block, no text free call.
    CCharStringData* d4 = new CCharStringData();
    d4->text = 0;
    d4->unknown04 = 0x55;
    d4->unknown08 = 0x80000009;
    d4->flags0C = 0;
    d4->owners = 0;
    CCharString cs4;
    cs4.storage_ = d4;
    cs4.UnassignString();
    if (cs4.storage_ != 0) { printf("FAIL case4 storage not reset\n"); return 1; }

    printf("UNASSIGN_STRING_OK\n");
    return 0;
}