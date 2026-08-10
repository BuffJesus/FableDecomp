
#include <cstdio>
struct Sub {
    int active;
    int refreshed;
    int SetActive(int on){ active = on; return 7; }
    void Refresh(){ refreshed = 1; }
};
struct CTCElectrocutedEffect {
    void* vt;
    Sub*  sub;
};
static void OnInitialActivate(CTCElectrocutedEffect* self)
{
    self->sub->SetActive(1);
    self->sub->Refresh();
}
int main(){
    Sub s; s.active=0; s.refreshed=0;
    CTCElectrocutedEffect e; e.vt=0; e.sub=&s;
    OnInitialActivate(&e);
    if (s.active==1 && s.refreshed==1) printf("OK_007ce370\n");
    else { printf("FAIL\n"); return 1; }
    return 0;
}