#include <cstdio>
static int g_base=-1, g_cancel=0, g_s4=-1, g_notify=-2;
struct Sub { virtual void s0(){} virtual void s1(){} virtual void s2(){} virtual void s3(){} virtual void s4(int e){ g_s4=e; } };
struct Obj { virtual void o0(){} virtual void o1(){} virtual void notify(void* p){ g_notify = p?1:0; } };
struct CKR { virtual void v0(){} Sub sub; void BaseChangeState(int n){ g_base=n; } void ChangeState(int newState); };
static Obj g_obj; static CKR* g_activeRedefiner=0;
static void __fastcall CKR_CancelSelection(CKR*){ g_cancel=1; }
static Obj* GetRedefinerSingleton(){ return &g_obj; }
void CKR::ChangeState(int newState){
    this->BaseChangeState(newState);
    switch (newState) {
    case 0: case 6:
        CKR_CancelSelection(this);
        this->sub.s4(0x19);
        return;
    case 1: case 5:
        if (g_activeRedefiner != this)
            GetRedefinerSingleton()->notify(this ? (char*)this + 4 : (char*)0);
        return;
    }
}
int main(){
    CKR o;
    g_base=-1;g_cancel=0;g_s4=-1; o.ChangeState(0);
    if(g_base!=0||!g_cancel||g_s4!=0x19){ std::printf("00557c10_TEST FAIL a\n"); return 1; }
    g_base=-1;g_cancel=0;g_s4=-1; o.ChangeState(6);
    if(g_base!=6||!g_cancel||g_s4!=0x19){ std::printf("00557c10_TEST FAIL b\n"); return 1; }
    g_notify=-2;g_activeRedefiner=0; o.ChangeState(1);
    if(g_notify!=1){ std::printf("00557c10_TEST FAIL c\n"); return 1; }
    g_notify=-2;g_activeRedefiner=&o; o.ChangeState(5);
    if(g_notify!=-2){ std::printf("00557c10_TEST FAIL d\n"); return 1; }
    g_base=-1;g_cancel=0;g_s4=-1;g_notify=-2; o.ChangeState(3);
    if(g_base!=3||g_cancel||g_s4!=-1||g_notify!=-2){ std::printf("00557c10_TEST FAIL e\n"); return 1; }
    g_base=-1; o.ChangeState(9);
    if(g_base!=9){ std::printf("00557c10_TEST FAIL f\n"); return 1; }
    std::printf("00557c10_TEST PASS\n"); return 0;
}