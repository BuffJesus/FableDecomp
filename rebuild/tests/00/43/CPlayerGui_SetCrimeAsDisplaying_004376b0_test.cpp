#include <cstdio>
struct Crime { char pad24[0x24]; int m24; int m28; };
struct CPlayerGui { char pad[0x2cc]; Crime* pCrime; };
void __fastcall SetCrimeAsDisplaying(CPlayerGui* this_)
{
    this_->pCrime->m24 = 0;
    if (this_->pCrime->m28 != 0)
        this_->pCrime->m28 = 2;
}
int main(){
    Crime cr; cr.m24=99; cr.m28=5;
    CPlayerGui gui; gui.pCrime=&cr;
    SetCrimeAsDisplaying(&gui);
    if(cr.m24!=0){printf("BAD m24=%d\n",cr.m24);return 1;}
    if(cr.m28!=2){printf("BAD m28=%d\n",cr.m28);return 1;}
    Crime cr2; cr2.m24=7; cr2.m28=0;
    CPlayerGui gui2; gui2.pCrime=&cr2;
    SetCrimeAsDisplaying(&gui2);
    if(cr2.m24!=0){printf("BAD2 m24=%d\n",cr2.m24);return 1;}
    if(cr2.m28!=0){printf("BAD2 m28=%d\n",cr2.m28);return 1;}
    printf("OK_0x004376b0\n");
    return 0;
}