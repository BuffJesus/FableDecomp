struct Sub { char pad[0x79c]; unsigned char flag; };
struct CHTG_Executioner { char pad0[0xc]; Sub* sub; };
extern void __stdcall Helper(int);
void __fastcall OnPredicateFail_b(CHTG_Executioner* self) {
    Helper(0x12);
    self->sub->flag = 0;
}