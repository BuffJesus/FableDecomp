struct C2DExtentsI { int a; int b; };

struct CMap {
    char pad[0x1d14];
    int f1d14; // +0x1d14
    int f1d18; // +0x1d18
    C2DExtentsI DrawGetHeightMapExtents();
};

C2DExtentsI CMap::DrawGetHeightMapExtents()
{
    C2DExtentsI r;
    r.b = this->f1d18;
    r.a = this->f1d14;
    return r;
}