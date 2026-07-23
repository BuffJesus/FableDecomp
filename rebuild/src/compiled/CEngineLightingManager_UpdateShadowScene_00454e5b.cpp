struct Sink {
    void Take(int v);
};

struct Local {
    int x;
    void Fill(int a, int* b);
};

class CEngineLightingManager {
public:
    char pad[0x18];
    int mode;      // +0x18
    char pad2[0x08];
    int obj24;     // +0x24
    Sink* sink28;  // +0x28
    void UpdateShadowScene(int* p);
};

extern void __stdcall Prep(unsigned int tag);   // push 0x122d70e; call

#pragma optimize("y", off)
#pragma optimize("s", on)
void CEngineLightingManager::UpdateShadowScene(int* p)
{
    Local h;
    Prep(0x122d70eU);
    h.x = 0;
    switch (this->mode) {
    case 2:
        h.Fill(this->obj24, p);
        break;
    case 3:
        this->sink28->Take(*p);
        break;
    }
}