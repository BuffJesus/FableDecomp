#include <stdio.h>
struct CTCMapwho { int calls; int cdecls; };
static void helper(CTCMapwho* self){ self->calls++; }
static void ext(CTCMapwho* self){ self->cdecls++; }
static void OnDie(CTCMapwho* self){ if(self){ helper(self); ext(self); } }
int main(){
    CTCMapwho o; o.calls=0; o.cdecls=0;
    OnDie(&o);
    if(o.calls!=1||o.cdecls!=1){ printf("FAIL\n"); return 1; }
    OnDie(0); /* null path: no crash, no change */
    if(o.calls!=1||o.cdecls!=1){ printf("FAIL\n"); return 1; }
    printf("OK_0048d890\n");
    return 0;
}