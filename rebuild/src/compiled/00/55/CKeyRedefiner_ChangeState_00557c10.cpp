struct Sub { virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4(int); };
struct Obj { virtual void o0(); virtual void o1(); virtual void notify(void*); };
struct CKR { virtual void v0(); Sub sub; void BaseChangeState(int); void ChangeState(int newState); };
extern void __fastcall CKR_CancelSelection(CKR*);
extern Obj* GetRedefinerSingleton();
extern CKR* g_activeRedefiner;
void CKR::ChangeState(int newState){
    this->BaseChangeState(newState);
    switch (newState) {
    case 1: case 5:
        if (g_activeRedefiner != this)
            GetRedefinerSingleton()->notify(this ? (char*)this + 4 : (char*)0);
        return;
    case 0: case 6:
        CKR_CancelSelection(this);
        this->sub.s4(0x19);
        return;
    }
}