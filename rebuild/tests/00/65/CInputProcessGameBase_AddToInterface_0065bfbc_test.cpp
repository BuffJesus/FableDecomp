#include <cstdio>
struct IF { char pad[0x98]; int f98; };
static IF* gS2;
void __fastcall h_0065bfbc(IF* self){ gS2=self; }
void __fastcall fn_0065bfbc(IF* self, int _edx, int arg){ self->f98 = arg; h_0065bfbc(self); }
int main(){ IF f; f.f98=0; fn_0065bfbc(&f,0,44); if(gS2!=&f||f.f98!=44){ printf("0065bfbc_TEST FAIL\n"); return 1; }
 printf("0065bfbc_TEST PASS\n"); return 0; }