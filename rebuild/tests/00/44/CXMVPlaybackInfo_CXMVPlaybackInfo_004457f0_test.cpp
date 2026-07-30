#include <stdio.h>
#include <string.h>

struct CXMVPlaybackInfoObj {
    char pad[0x894];
    unsigned char flag;
};

void __fastcall CXMVPlaybackInfo(CXMVPlaybackInfoObj *self)
{
    self->flag = 0;
}

int main()
{
    CXMVPlaybackInfoObj obj;
    memset(&obj, 0xFF, sizeof(obj));
    void *fn = (void*)&CXMVPlaybackInfo;
    CXMVPlaybackInfoObj *p = &obj;
    __asm {
        mov ecx, p
        mov eax, fn
        call eax
    }
    if (obj.flag != 0) {
        printf("FAIL flag=%d\n", (int)obj.flag);
        return 1;
    }
    if ((unsigned char)obj.pad[0x893] != 0xFF) {
        printf("FAIL neighbour clobbered\n");
        return 1;
    }
    printf("OK_0x004457f0\n");
    return 0;
}