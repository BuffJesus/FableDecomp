typedef void*(__stdcall* PFN)(int,unsigned int,int,int);
extern PFN g_vp;
void* __fastcall op_new(unsigned int size){ return g_vp(0, size, 0x3000, 4); }