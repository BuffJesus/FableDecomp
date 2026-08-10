struct Sub { char b; };

// call target at 0xa01c10: __fastcall method (ecx = this+0x114)
void __fastcall Sub_Method(Sub* s);
// tail-call target at 0x62e900: __fastcall method (ecx = this)
struct Owner;
void __fastcall Owner_Tail(Owner* o);

void __fastcall DestVal_Music_C(Owner* self)
{
    Sub_Method((Sub*)((char*)self + 0x114));
    Owner_Tail(self);
}