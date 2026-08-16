#pragma optimize("s",on)

struct CTCInGameMenu {
    CTCInGameMenu* Ctor(void* parent);   // this-call member; returns this
};
CTCInGameMenu* __fastcall Construct(void* parent /*ecx*/)
{
    CTCInGameMenu* p = (CTCInGameMenu*)::operator new(0xF8);
    if (p != 0)
        return p->Ctor(parent);
    return 0;
}