#include <cstdio>

struct Renderer { char pad[0x100]; };

struct Prim {
    char pad0[0x0c];
    unsigned int flags;
    void* last_arg;
    void Add(void* p) { last_arg = p; }
};

void AddPrimitive(Renderer* self, Prim* prim)
{
    if (prim->flags & 0x400)
        prim->Add((char*)self + 0x44);
    else
        prim->Add((char*)self + 0x40);
}

int main()
{
    Renderer r;
    Prim a; a.flags = 0x400; a.last_arg = 0;
    Prim b; b.flags = 0x000; b.last_arg = 0;

    AddPrimitive(&r, &a);
    AddPrimitive(&r, &b);

    bool ok = (a.last_arg == (char*)&r + 0x44) && (b.last_arg == (char*)&r + 0x40);

    // also verify unrelated flag bits do not affect selection
    Prim c; c.flags = 0x400 | 0x1 | 0x800; c.last_arg = 0;
    AddPrimitive(&r, &c);
    ok = ok && (c.last_arg == (char*)&r + 0x44);

    Prim d; d.flags = 0x100 | 0x200 | 0x800; d.last_arg = 0; // no 0x400
    AddPrimitive(&r, &d);
    ok = ok && (d.last_arg == (char*)&r + 0x40);

    if (ok) printf("PARITY_OK_b4a670\n");
    else printf("FAIL\n");
    return 0;
}