// _Dest_val<std::allocator<CMacro>,CMacro> : destroy chain forwarder.
extern "C" void __fastcall sub_A(void* self);
extern "C" void __fastcall sub_B(void* self);
extern "C" void __fastcall sub_C(void* self);
extern "C" void __fastcall sub_D(void* self);

void __fastcall Dest_val_allocator(void* self)
{
    sub_A(self);
    sub_B(self);
    sub_C(self);
    sub_D(self);
}