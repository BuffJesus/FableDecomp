#include <stdio.h>
struct RefNode { char pad0[0x18]; unsigned char isQuest; char pad19[3]; unsigned int refCount; };
struct CItem { char pad0[8]; RefNode* node; };
__declspec(naked) void UpdateNonQuestPersistentRefCount()
{
    __asm {
        mov eax, dword ptr [ecx+8]
        xor ecx, ecx
        cmp eax, ecx
        je   L_done
        cmp byte ptr [eax+0x18], cl
        jne  L_done
        cmp byte ptr [esp+4], cl
        je   L_reset
        inc  dword ptr [eax+0x1c]
        jmp  L_done
    L_reset:
        mov  dword ptr [eax+0x1c], ecx
    L_done:
        ret  4
    }
}
static void docall(CItem* self, char flag)
{
    __asm {
        movzx eax, flag
        push eax
        mov  ecx, self
        call UpdateNonQuestPersistentRefCount
    }
}
int main()
{
    RefNode n; CItem it; it.node = &n;
    n.isQuest = 0; n.refCount = 5; docall(&it, 1); if (n.refCount != 6) { printf("FAIL inc\n"); return 1; }
    n.isQuest = 0; n.refCount = 9; docall(&it, 0); if (n.refCount != 0) { printf("FAIL reset\n"); return 1; }
    n.isQuest = 1; n.refCount = 3; docall(&it, 1); if (n.refCount != 3) { printf("FAIL quest-skip\n"); return 1; }
    it.node = 0; docall(&it, 1);
    printf("OK_0x005bc70a\n");
    return 0;
}