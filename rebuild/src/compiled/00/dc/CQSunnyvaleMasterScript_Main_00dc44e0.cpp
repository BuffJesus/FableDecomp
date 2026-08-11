struct F4;
struct F4Vtbl { void (__fastcall *slot[8])(F4*); };
struct F4 { F4Vtbl* vtbl; };
struct CQ_SunnyvaleMasterScript { void* f0; F4* f4; };

extern "C" char __fastcall helper_f35b30(CQ_SunnyvaleMasterScript* self);

void __fastcall Main(CQ_SunnyvaleMasterScript* self)
{
    do {
        F4* p = self->f4;
        p->vtbl->slot[7](p);
    } while (!helper_f35b30(self));
}