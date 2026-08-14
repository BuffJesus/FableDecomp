struct CDrawBase;

struct Elem { char pad[20]; };

struct Base {
    Elem elems[1];
};

// callee 0x00a35150: __fastcall(ecx=base, edx=elem, stack byte)
extern "C" void __fastcall callee(Base* base, Elem* elem, unsigned char b);

struct CSetIllustratorAsDrawing {
    unsigned char flag;   // +0
    char pad[3];
    Base* base;           // +4
    int index;            // +8
    unsigned char done;   // +0xc

    void __fastcall operator()(CDrawBase* db);
};

void __fastcall CSetIllustratorAsDrawing::operator()(CDrawBase* db)
{
    Base* b = this->base;
    unsigned char f = this->flag;
    Elem* elem = &b->elems[this->index];
    callee(b, elem, f);
    this->done = 0;
}