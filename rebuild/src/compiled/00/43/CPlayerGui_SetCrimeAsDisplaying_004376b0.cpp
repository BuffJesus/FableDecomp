struct Crime { char pad24[0x24]; int m24; int m28; };
struct CPlayerGui { char pad[0x2cc]; Crime* pCrime; };
void __fastcall SetCrimeAsDisplaying(CPlayerGui* this_)
{
    this_->pCrime->m24 = 0;
    if (this_->pCrime->m28 != 0)
        this_->pCrime->m28 = 2;
}