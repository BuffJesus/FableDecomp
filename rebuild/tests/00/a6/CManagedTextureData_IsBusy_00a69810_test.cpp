#include <cstdio>

// Mirror of the layout under test.
struct CTextureResource
{
    bool CheckBusy();
};

struct CManagedTextureData
{
    char pad00[0x20];
    CTextureResource* resource;   // +0x20
    char pad24[0x4a - 0x24];
    bool busy;                    // +0x4a

    bool IsBusy();
};

// Behaviour of the queried resource: driven by a global so the test
// can decide what the re-query returns.
static bool g_resourceStillBusy = false;
static int  g_queryCount = 0;

bool CTextureResource::CheckBusy()
{
    ++g_queryCount;
    return g_resourceStillBusy;
}

bool CManagedTextureData::IsBusy()
{
    if (this->busy)
    {
        this->busy = this->resource->CheckBusy();
    }
    return this->busy;
}

int main()
{
    CTextureResource res;
    CManagedTextureData d;
    d.resource = &res;

    // Case 1: not busy -> no re-query, returns false.
    d.busy = false;
    g_queryCount = 0;
    bool r1 = d.IsBusy();
    if (r1 != false) { printf("FAIL c1\n"); return 1; }
    if (g_queryCount != 0) { printf("FAIL c1 query\n"); return 1; }

    // Case 2: busy, resource now idle -> re-query, caches false.
    d.busy = true;
    g_resourceStillBusy = false;
    g_queryCount = 0;
    bool r2 = d.IsBusy();
    if (r2 != false) { printf("FAIL c2\n"); return 1; }
    if (g_queryCount != 1) { printf("FAIL c2 query\n"); return 1; }
    if (d.busy != false) { printf("FAIL c2 cache\n"); return 1; }

    // Case 3: busy, resource still busy -> re-query, stays true.
    d.busy = true;
    g_resourceStillBusy = true;
    g_queryCount = 0;
    bool r3 = d.IsBusy();
    if (r3 != true) { printf("FAIL c3\n"); return 1; }
    if (g_queryCount != 1) { printf("FAIL c3 query\n"); return 1; }
    if (d.busy != true) { printf("FAIL c3 cache\n"); return 1; }

    // Offsets sanity.
    if ((char*)&d.resource - (char*)&d != 0x20) { printf("FAIL off20\n"); return 1; }
    if ((char*)&d.busy - (char*)&d != 0x4a) { printf("FAIL off4a\n"); return 1; }

    printf("ISBUSY_OK\n");
    return 0;
}