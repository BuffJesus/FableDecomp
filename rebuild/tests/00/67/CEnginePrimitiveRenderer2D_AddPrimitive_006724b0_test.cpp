#include <cstdio>

// Behaviour model of CEnginePrimitiveRenderer2D::AddPrimitive dispatch.
struct Prim { int id; };
struct Renderer {
    char pad00[0x18];
    int  mode;          // +0x18
    char pad1c[0x24-0x1c];
    void* sub24;        // +0x24
    void* sub28;        // +0x28
};

static int g_last_path = 0;      // 24 or 28
static Prim* g_last_prim = 0;
static void* g_last_self = 0;

void path24(void* self, Prim* p){ g_last_path=24; g_last_self=self; g_last_prim=p; }
void path28(void* self, Prim* p){ g_last_path=28; g_last_self=self; g_last_prim=p; }

void addPrimitive(Renderer* self, Prim* prim){
    if (self->mode == 2) path24(self->sub24, prim);
    else                 path28(self->sub28, prim);
}

int main(){
    Renderer r; for (int i=0;i<(int)sizeof(r);++i) ((char*)&r)[i]=0;
    int a, b; r.sub24=&a; r.sub28=&b;
    Prim p; p.id=7;

    // mode == 2 -> path24
    r.mode = 2;
    addPrimitive(&r, &p);
    if (g_last_path!=24 || g_last_self!=&a || g_last_prim!=&p){ printf("BAD1\n"); return 1; }

    // mode != 2 -> path28  (several values)
    int probes[] = {0,1,3,5,-2,100};
    for (int i=0;i<6;i++){
        r.mode = probes[i];
        addPrimitive(&r, &p);
        if (g_last_path!=28 || g_last_self!=&b || g_last_prim!=&p){ printf("BAD2\n"); return 1; }
    }

    printf("PARITY_OK_006724b0\n");
    return 0;
}