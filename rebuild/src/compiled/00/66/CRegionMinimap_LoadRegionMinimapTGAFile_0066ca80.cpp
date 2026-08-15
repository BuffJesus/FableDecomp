// CRegionMinimap::LoadRegionMinimapTGAFile  0x0066ca80
struct CWideString {
    bool Check();               // 0x999230 : thiscall bool()
};

// stack temp: f0 set to -1 inline, then out-of-line init(8,8,8,8,1) at 0x9e3790
struct Temp {
    int f0;
    void Init(int a, int b, int c, int d, int e);   // 0x9e3790
    Temp() { f0 = -1; Init(8, 8, 8, 8, 1); }
};

// sub-object living at self+0x24
struct SubObj {
    void Build(int* pair, Temp* t, int z);    // 0x9f3230
    void Finish(CWideString* w, int one);     // 0x9f2dd0
};

struct CRegionMinimap {
    int    field0;            // +0x00
    char   gap[0x24-4];
    SubObj sub;               // +0x24
    void LoadRegionMinimapTGAFile(CWideString* ws);
};

void CRegionMinimap::LoadRegionMinimapTGAFile(CWideString* ws)
{
    Temp t;

    int pair[2];
    pair[0] = this->field0;
    pair[1] = this->field0;

    this->sub.Build(pair, &t, 0);

    if (ws->Check()) {
        this->sub.Finish(ws, 1);
    }
}