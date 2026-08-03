// CWorldMap::DrawGetDimensions @ 0x004fb2b0
struct SDimensions { int a; int b; int c; int d; };

class CWorldMap {
public:
    void DrawGetDimensions(SDimensions* out);
};

void CWorldMap::DrawGetDimensions(SDimensions* out)
{
    int* fields = reinterpret_cast<int*>(reinterpret_cast<char*>(this) + 0x58);
    out->a = fields[0];
    out->b = fields[1];
    out->c = fields[2];
    out->d = fields[3];
}