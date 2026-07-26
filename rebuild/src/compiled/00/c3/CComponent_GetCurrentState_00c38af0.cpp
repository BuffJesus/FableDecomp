struct CUIState;

struct CInner {
    CUIState* Get(int* out);
};

struct CComponent {
    char pad[0xd8];
    CInner inner;
};

CUIState* __fastcall CComponent_GetCurrentState(CComponent* self)
{
    int tmp;
    return self->inner.Get(&tmp);
}