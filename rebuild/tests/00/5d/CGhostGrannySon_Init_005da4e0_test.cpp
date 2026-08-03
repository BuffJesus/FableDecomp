// Behaviour test for CGhostGrannySon::Init @ 0x005da4e0
#include <cstdio>

struct CGhostGrannySon {
    char pad[0x14];
    int  m_index;   // +0x14
    bool m_flag;    // +0x18
    char tail[0x8];
};

void __fastcall Init(CGhostGrannySon *this_) {
    this_->m_index = -1;
    this_->m_flag  = false;
}

int main() {
    CGhostGrannySon obj;
    // Pre-fill with a non-target pattern so we prove Init writes.
    for (int i = 0; i < (int)sizeof(obj); ++i) ((unsigned char*)&obj)[i] = 0x5A;
    obj.m_index = 12345;
    obj.m_flag  = true;

    CGhostGrannySon *p = &obj;
    // Invoke via __fastcall shim: this in ecx, no stack args.
    __asm {
        mov  ecx, p
        call Init
    }

    if (obj.m_index != -1) { printf("BAD index=%d\n", obj.m_index); return 1; }
    if (obj.m_flag != false) { printf("BAD flag=%d\n", (int)obj.m_flag); return 1; }
    // Ensure neighbouring bytes untouched (offset 0x13 and 0x19 stay 0x5A).
    if ((unsigned char)obj.pad[0x13] != 0x5A) { printf("BAD pad\n"); return 1; }
    if ((unsigned char)obj.tail[0] != 0x5A) { printf("BAD tail\n"); return 1; }

    printf("OK_0x005da4e0 index=%d flag=%d\n", obj.m_index, (int)obj.m_flag);
    return 0;
}