// CManagedTextureData::IsBusy  @ retail 0x00a69810
// __fastcall bool IsBusy(CManagedTextureData* this)
//
// Reads a "busy" flag at +0x4a; if set, re-queries the underlying
// resource object at +0x20 (a __fastcall query at 0x9f9d50) and
// caches the result back into +0x4a; returns the flag.

struct CTextureResource
{
    bool CheckBusy();   // __fastcall @ 0x9f9d50
};

struct CManagedTextureData
{
    char pad00[0x20];
    CTextureResource* resource;   // +0x20
    char pad24[0x4a - 0x24];
    bool busy;                    // +0x4a

    bool IsBusy();
};

bool CManagedTextureData::IsBusy()
{
    if (this->busy)
    {
        this->busy = this->resource->CheckBusy();
    }
    return this->busy;
}