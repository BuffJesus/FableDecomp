struct AT { char pad[8]; int f8; };
AT* __stdcall h_005dc210(AT** out);
int __fastcall fn_005dc210(void* self){ AT* p; h_005dc210(&p); return p->f8; }