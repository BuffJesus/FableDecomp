struct CNavRegion; struct S { char pad[0xF]; unsigned char b0f; void* f10; };
const CNavRegion* __fastcall GetRegion(S* self){ if(!self->b0f) return (const CNavRegion*)self->f10; return 0; }