#pragma optimize("s",on)
struct SubA { char x; };
struct SubB { char x; };
#pragma pack(1)
struct CTCInventoryItem {
    unsigned char _pad_0x0[0x3c];
    SubB sub3c;             // +0x3c
    unsigned char _pad_0x3d[0x13];
    SubA sub50;             // +0x50
    unsigned char _pad_0x51[0x1b];
    void* ptr6c;            // +0x6c
};
#pragma pack()
extern "C" void __cdecl engine_free(void* p);
extern void __fastcall SubA_dtor(SubA* self);
extern void __fastcall SubB_dtor(SubB* self);
extern void __fastcall CTCInventoryItem_base(CTCInventoryItem* self);

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self)
{
    if (self->ptr6c)
        engine_free(self->ptr6c);
    SubA_dtor(&self->sub50);
    SubB_dtor(&self->sub3c);
    CTCInventoryItem_base(self);
}