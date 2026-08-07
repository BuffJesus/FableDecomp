struct AT { char pad[8]; int f8; };
AT* __stdcall h_005dc1f0(AT** out);
int __fastcall fn_005dc1f0(void* self){ AT* p; h_005dc1f0(&p); return p->f8; }