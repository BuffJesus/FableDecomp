struct Inner { char pad[0x136]; unsigned char b136; };
struct S { char pad[0x10]; Inner* f10; };
bool __fastcall IsUsingProjectileTargetingAnalogueZoom(S* self){ return self->f10->b136 ? true : false; }