#include "fable_quick_draw_info.h"
#include "fable_stlport_vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
namespace _STL { template<> vector<CQuickDrawTriInfo>::~vector(); }
typedef _STL::vector<CQuickDrawTriInfo> InfoVector;
static InfoVector* owner;
static unsigned char* entries;
static CQuickDrawTriInfo* replacement;
static unsigned count,mode,events,failed;
static void check(bool value) { if(!value) ++failed; }
void CTexture::Uninitialise()
{
    check(events%2==0);
    check(reinterpret_cast<unsigned char*>(this)==entries+(events/2)*60+24);
    ++events;
    memset(this,0,8);
    if(events==1 && mode) {
        owner->_M_start=mode==1?replacement:0;
        owner->_M_finish=replacement;
    }
}
CWideString::~CWideString()
{
    check(events%2==1);
    check(reinterpret_cast<unsigned char*>(this)==entries+(events/2)*60+20);
    ++events;
    memset(this,0,4);
}
int main()
{
    check(sizeof(InfoVector)==12);
    check(offsetof(CQuickDrawTriInfo,String)==20);
    check(offsetof(CQuickDrawTriInfo,UploadedTexture)==24);
    unsigned scenarios=0;
    for(unsigned fill=0;fill<256;++fill) for(count=0;count<5;++count) for(mode=0;mode<3;++mode) {
        unsigned long storage[3];owner=reinterpret_cast<InfoVector*>(storage);
        entries=static_cast<unsigned char*>(malloc(5*60));
        replacement=static_cast<CQuickDrawTriInfo*>(malloc(60));
        memset(entries,fill,5*60);
        owner->_M_start=reinterpret_cast<CQuickDrawTriInfo*>(entries);
        owner->_M_finish=owner->_M_start+count;
        owner->_M_end_of_storage._M_data=owner->_M_start+5;
        events=0;
        owner->~InfoVector();
        check(events==count*2);
        check(owner->_M_end_of_storage._M_data==reinterpret_cast<CQuickDrawTriInfo*>(entries)+5);
        if(count && mode) {
            check(owner->_M_start==(mode==1?replacement:0));
            check(owner->_M_finish==replacement);
            for(unsigned i=0;i<300;++i) check(entries[i]==((i/60<count && i%60>=20 && i%60<32)?0:fill));
            free(entries);
            if(mode==2) free(replacement);
        } else {
            check(owner->_M_start==reinterpret_cast<CQuickDrawTriInfo*>(entries));
            check(owner->_M_finish==reinterpret_cast<CQuickDrawTriInfo*>(entries)+count);
            free(replacement);
        }
        ++scenarios;
    }
    unsigned long empty[3]={0,0,0};owner=reinterpret_cast<InfoVector*>(empty);events=0;owner->~InfoVector();check(events==0 && empty[0]==0 && empty[1]==0 && empty[2]==0);
    if(failed){printf("FAIL %u\n",failed);return 1;}
    printf("QUICK_DRAW_LIFETIME_BEHAVIOR_PASS scenarios=%u\n",scenarios+1);return 0;
}
