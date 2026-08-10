struct Inner {
    char pad[0x79c];
    unsigned char flag;
};

struct CHTG_Executioner {
    char pad0[0xc];
    Inner* inner;
    void Helper(int n);
    void OnPredicateFail();
};

void __fastcall OnPredicateFail(CHTG_Executioner* self)
{
    self->Helper(0x11);
    self->inner->flag = 0;
}