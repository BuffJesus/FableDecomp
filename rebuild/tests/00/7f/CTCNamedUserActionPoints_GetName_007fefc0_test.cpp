#include <stdio.h>
struct CCharString {
    char* p;
    void Assign(const CCharString& src);
};
static const CCharString* captured_src; static CCharString* captured_self;
void CCharString::Assign(const CCharString& src){ captured_self=this; captured_src=&src; }
struct CTCNamedUserActionPoints {
    char pad[0x34];
    CCharString* arr;
    void GetName(long index, CCharString* out);
};
int main(){
  CCharString elems[4]; int i; for(i=0;i<4;i++) elems[i].p=(char*)(0x1000+i);
  CTCNamedUserActionPoints obj; obj.arr=elems;
  CCharString out; out.p=(char*)0xDEAD;
  obj.GetName(2, &out);
  if(captured_src == &elems[2] && captured_self == &out) printf("OK_007fefc0\n");
  else printf("FAIL\n");
  return 0;
}