
#include <stdio.h>
struct CCharString { char* got; };
struct CCachedFile { char pad[0x18]; char m_pathname[8]; };
static void* modelCopy(CCharString* d, char* s){ d->got = s; return d; }
static void* GetPathnameModel(CCachedFile* self, CCharString* dst){
    modelCopy(dst, self->m_pathname);
    return dst;
}
int main(){
    CCachedFile f; f.m_pathname[0]='A'; f.m_pathname[1]=0;
    CCharString s; s.got=0;
    void* r = GetPathnameModel(&f, &s);
    int ok = (r == (void*)&s) && (s.got == f.m_pathname) && (s.got[0]=='A');
    // second branch: different dst
    CCharString s2; s2.got=0;
    void* r2 = GetPathnameModel(&f, &s2);
    ok = ok && (r2 == (void*)&s2) && (s2.got == f.m_pathname);
    if(ok) printf("OK_00826400\n"); else printf("FAIL\n");
    return ok?0:1;
}