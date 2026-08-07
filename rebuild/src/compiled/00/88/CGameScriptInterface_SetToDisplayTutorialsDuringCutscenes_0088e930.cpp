struct GS2 { char pad[0xcc]; char fcc; };
GS2* __cdecl get_88e930();
void __fastcall fn_0088e930(void* self, int _edx, char arg){ get_88e930()->fcc = arg; }