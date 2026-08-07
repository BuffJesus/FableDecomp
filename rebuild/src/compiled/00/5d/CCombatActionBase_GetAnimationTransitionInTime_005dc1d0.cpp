struct AT { char pad[8]; int f8; };
AT* __stdcall h_005dc1d0(AT** out);
int __fastcall fn_005dc1d0(void* self){ AT* p; h_005dc1d0(&p); return p->f8; }