typedef void (__fastcall *pfn_t)(void* self);

struct Base { void** vtbl; };

void __fastcall CHoverable_ProcessEvent(void* self, int edx, int ev)
{
    if (ev == 0x19) {
        Base* b = (Base*)((char*)self - 4);
        ((pfn_t)b->vtbl[0x244/4])(b);
    }
}