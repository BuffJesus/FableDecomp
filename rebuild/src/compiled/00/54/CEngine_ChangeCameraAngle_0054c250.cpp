struct S; extern void __fastcall sub(S* self);
struct S { char pad[0x18c]; char b18c; };
void __fastcall ChangeCameraAngle(S* self){ sub(self); self->b18c = 0; }