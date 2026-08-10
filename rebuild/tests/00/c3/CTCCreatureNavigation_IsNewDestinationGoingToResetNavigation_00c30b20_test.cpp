#include <cstdio>
struct C3DVector { float x,y,z; };
struct Self;
struct Inner;
struct InnerVtbl {
  void* s0; void* s1; void* s2; void* s3; void* s4; void* s5;
  bool (__fastcall *slot6)(Inner* self, int edx, Self* a, C3DVector* b);
};
struct Inner { InnerVtbl* vtbl; };
struct Self { void* f0; Inner* f4; };

extern "C" bool __fastcall call_leaf(Self* self, int edx, C3DVector* vec);

static Self* g_expected_self; static C3DVector* g_expected_vec; static Inner* g_expected_inner;
static bool g_ret;
static bool __fastcall vf6(Inner* self, int edx, Self* a, C3DVector* b){
  if(self!=g_expected_inner) { printf("BAD inner\n"); return false; }
  if(a!=g_expected_self) { printf("BAD self\n"); return false; }
  if(b!=g_expected_vec) { printf("BAD vec\n"); return false; }
  return g_ret;
}

__declspec(naked) bool __fastcall call_leaf(Self* self, int edx, C3DVector* vec)
{
  __asm {
    push esi
    mov  esi, dword ptr [esp+8]
    mov  eax, ecx
    mov  ecx, dword ptr [eax+4]
    mov  edx, dword ptr [ecx]
    push esi
    push eax
    call dword ptr [edx+0x18]
    pop  esi
    ret  4
  }
}

int main(){
  InnerVtbl vt; vt.slot6=vf6;
  Inner inner; inner.vtbl=&vt;
  Self self; self.f0=0; self.f4=&inner;
  C3DVector vec; vec.x=1; vec.y=2; vec.z=3;
  g_expected_self=&self; g_expected_vec=&vec; g_expected_inner=&inner;
  g_ret=true;
  bool r1=call_leaf(&self,0,&vec);
  g_ret=false;
  bool r2=call_leaf(&self,0,&vec);
  if(r1==true && r2==false){ printf("OK_00c30b20\n"); return 0; }
  printf("FAIL r1=%d r2=%d\n", r1, r2); return 1;
}