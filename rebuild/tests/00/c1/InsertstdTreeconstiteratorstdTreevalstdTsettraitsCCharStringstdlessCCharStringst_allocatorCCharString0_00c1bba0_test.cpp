
#include <stdio.h>
struct Node { char pad[0x12]; unsigned short w; };
static unsigned short g_ret;
static Node* g_self; static void* g_p;
unsigned short Helper(Node* self, void* p){ g_self=self; g_p=p; return g_ret; }
void TreeInsert_B(Node* self, void* p){ self->w = Helper(self, p); }
int main(){
    Node n; for(int i=0;i<(int)sizeof(n);i++) ((char*)&n)[i]=0;
    void* arg=(void*)0x1234;
    g_ret=0xABCD;
    TreeInsert_B(&n, arg);
    if(n.w!=0xABCD){ printf("FAIL w=%x\n", n.w); return 1; }
    if(g_self!=&n){ printf("FAIL self\n"); return 1; }
    if(g_p!=arg){ printf("FAIL p\n"); return 1; }
    g_ret=0x0000;
    TreeInsert_B(&n, arg);
    if(n.w!=0x0000){ printf("FAIL w2=%x\n", n.w); return 1; }
    printf("OK_00c1bba0\n");
    return 0;
}