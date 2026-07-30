#include <cstdio>
struct CTCPhysicsNavigator { char pad[0x90]; char navigatorEnabled; };
static void __declspec(naked) EnableNavigator_impl()
{
    __asm {
        mov al, byte ptr [esp+4]
        mov byte ptr [ecx+0x90], al
        ret 4
    }
}
static void call_enable(CTCPhysicsNavigator *self, char v)
{
    void *fn = (void*)&EnableNavigator_impl;
    __asm {
        mov ecx, self
        movzx eax, v
        push eax
        call fn
    }
}
int main()
{
    CTCPhysicsNavigator obj;
    obj.navigatorEnabled = 0;
    call_enable(&obj, 1);
    if (obj.navigatorEnabled != 1) { printf("BAD1\n"); return 1; }
    call_enable(&obj, 0);
    if (obj.navigatorEnabled != 0) { printf("BAD2\n"); return 1; }
    call_enable(&obj, 7);
    if (obj.navigatorEnabled != 7) { printf("BAD3\n"); return 1; }
    printf("OK_0x004c7440\n");
    return 0;
}