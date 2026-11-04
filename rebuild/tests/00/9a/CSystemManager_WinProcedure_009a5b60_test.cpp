#include "fable_window_procedure.h"
#include <stdio.h>
#include <string.h>

CSystemManager* FableWindowSystemManager_013ca798;
float FableWindowGamma_0138e0b8;
float FableSavedSoundVolume_013ca60c;
bool FableSoundDeactivated_013ca610;
extern "C" unsigned long InkeyStorage[0x1a0/4]={0};
#pragma comment(linker,"/alternatename:?GInkeyBuffer@@3VCInkeyBuffer@@A=_InkeyStorage")
#pragma comment(linker,"/alternatename:__imp__DefWindowProcW@16=_MockDefWindowProcPointer")
struct CSystemManagerTestAccess {
    static long Run(HWND__* w,unsigned int m,unsigned int p,long l) { return CSystemManager::WinProcedure(w,m,p,l); }
};
struct Scenario { unsigned long message,wp,negative,flags,ime,expected; };
#include "test_support/window_procedure_expected.h"
static unsigned long firstStorage[0x180/4],secondStorage[0x180/4],inputStorage[0x60/4],soundStorage[0x80/4],displayStorage[0x2c8/4],table[54];
static CSystemManager* first=reinterpret_cast<CSystemManager*>(firstStorage);
static CSystemManager* second=reinterpret_cast<CSystemManager*>(secondStorage);
static CInputManager* input=reinterpret_cast<CInputManager*>(inputStorage);
static CASoundManager* sound=reinterpret_cast<CASoundManager*>(soundStorage);
static CDisplayManager* display=reinterpret_cast<CDisplayManager*>(displayStorage);
static Scenario scenario;
static long lp;
static unsigned errors,count;
struct Event { unsigned long kind,receiver,a,b,c,d; };
static Event events[32];
static void Check(bool value) { if(!value) ++errors; }
static unsigned long Bits(float value) { unsigned long result;memcpy(&result,&value,4);return result; }
static unsigned long Receiver(const void* p) {
    return p==first?1:p==second?2:p==input?3:p==sound?4:p==display?5:p==InkeyStorage?6:0;
}
static void Record(unsigned long kind,unsigned long receiver=0,unsigned long a=0,unsigned long b=0,unsigned long c=0,unsigned long d=0) {
    if(count>=32) {++errors;return;} Event& e=events[count++];
    e.kind=kind;e.receiver=receiver;e.a=a;e.b=b;e.c=c;e.d=d;
}
static void Mutate() {
    if(scenario.flags&128) FableWindowSystemManager_013ca798=FableWindowSystemManager_013ca798==first?second:first;
}
static bool FABLE_FASTCALL IME(HWND__* window,unsigned int message,unsigned int wp,long value,long& result) {
    Check(window==reinterpret_cast<HWND__*>(0x123456) && message==scenario.message && wp==scenario.wp && value==lp && result==0);
    Record(1,0,message,wp,scenario.negative,scenario.ime);
    result=static_cast<long>(0x87654321);
    if(scenario.ime==3) FableWindowSystemManager_013ca798=second;
    return scenario.ime==2;
}
static long FABLE_STDCALL Default(HWND__* window,unsigned int message,unsigned int wp,long value) {
    Check(window==reinterpret_cast<HWND__*>(0x123456) && message==scenario.message && wp==scenario.wp && value==lp);
    Record(2,0,message,wp,scenario.negative);return 0x76543210;
}
extern "C" void* MockDefWindowProcPointer=reinterpret_cast<void*>(&Default);
void FABLE_FASTCALL CSystemManager::ActivateWindow(CSystemManager* receiver,bool active) {
    Check(receiver==FableWindowSystemManager_013ca798);Record(3,Receiver(receiver),active);Mutate();
}
void CDisplayManager::SetGamma(float gamma,bool force) { Check(this==display && !force);Record(4,5,Bits(gamma),force);Mutate(); }
void CSystemManager::SetHasInputFocus(bool value) {
    Check(this==FableWindowSystemManager_013ca798);Record(5,Receiver(this),value);ApplicationHasInputFocus=value;Mutate();
}
void CInputManager::ClearEvents() { Check(this==input);Record(6,3);Mutate(); }
void CASoundManager::UnknownSetEnabled_009915e0(bool enabled) { Check(this==sound);Record(7,4,enabled);Mutate(); }
bool CDisplayManager::SwapScreens() { Check(this==display);Record(8,5);Mutate();return false; }
bool CSystemManager::IsApplicationActive() { Check(this==FableWindowSystemManager_013ca798);Record(9,Receiver(this));Mutate();return (scenario.flags&16)!=0; }
void CInkeyBuffer::AddChar(wchar_t character,bool pressed) { Check(this==reinterpret_cast<CInkeyBuffer*>(InkeyStorage));Record(10,6,character,pressed);Mutate(); }
void CSystemManager::SetAsActive(bool active) { Check(this==FableWindowSystemManager_013ca798);Record(11,Receiver(this),active);Mutate(); }
struct SoundCallbacks {
    void Set(unsigned long group,float value,float transition) { Check(this==reinterpret_cast<SoundCallbacks*>(sound));Record(12,4,group,Bits(value),Bits(transition));Mutate(); }
    float Get(unsigned long group) { Check(this==reinterpret_cast<SoundCallbacks*>(sound));Record(13,4,group);Mutate();return -0.375f; }
    bool Offset(float& value) {
        Check(this==reinterpret_cast<SoundCallbacks*>(sound) && &value==&FableWindowSystemManager_013ca798->ForceMusicPlayOffset);
        Record(14,4,Receiver(FableWindowSystemManager_013ca798));value=42.25f;Mutate();return (scenario.flags&32)!=0;
    }
};
template<class T> static void Slot(int n,T pointer) { memcpy(table+n,&pointer,4); }
static unsigned long hash;
static void Hash(const void* data,unsigned length) {
    const unsigned char* bytes=static_cast<const unsigned char*>(data);
    for(unsigned i=0;i<length;++i) hash=(hash^bytes[i])*16777619UL;
}
static void Initialise(CSystemManager* manager) {
    memset(manager,0xa5,0x180);manager->ApplicationActive=(scenario.flags&16)!=0;manager->EverBeenActive=false;
    manager->PInputManager.Data=(scenario.flags&1)?input:0;
    manager->PDisplayManager.Data=(scenario.flags&4)?display:0;
    manager->PSoundManager.Data=(scenario.flags&2)?sound:0;
    manager->IMEMessageFunc=scenario.ime?&IME:0;
    manager->MouseZMove=0x7fffffc0;manager->ForceMusicPlayOffset=-17.5f;
}
int main() {
    Slot(46,&SoundCallbacks::Set);Slot(47,&SoundCallbacks::Get);Slot(49,&SoundCallbacks::Offset);
    soundStorage[0]=reinterpret_cast<unsigned long>(table);display->Gamma=1.75f;
    for(unsigned i=0;i<sizeof(scenarios)/sizeof(scenarios[0]);++i) {
        scenario=scenarios[i];Initialise(first);Initialise(second);
        FableWindowSystemManager_013ca798=first;
        if ((scenario.flags&64) && (scenario.message==1 || scenario.message==0x220)) FableWindowSystemManager_013ca798=0;
        FableSavedSoundVolume_013ca60c=0.625f;FableSoundDeactivated_013ca610=(scenario.flags&8)!=0;FableWindowGamma_0138e0b8=1.25f;
        CSystemManager* create=second;
        lp=scenario.message==1?reinterpret_cast<long>(&create):static_cast<long>(scenario.negative?0x80001234:0x1234);
        count=0;
        unsigned long result=CSystemManagerTestAccess::Run(reinterpret_cast<HWND__*>(0x123456),scenario.message,scenario.wp,lp);
        hash=2166136261UL;
        CSystemManager* managers[]={first,second};
        for(int m=0;m<2;++m) {
            unsigned char state[0x180];memcpy(state,managers[m],sizeof(state));
            const unsigned offsets[]={0x58,0x60,0x7c,0x178};
            for(int n=0;n<4;++n) memset(state+offsets[n],0,4);
            Hash(state,sizeof(state));
        }
        Hash(&result,4);unsigned long receiver=Receiver(FableWindowSystemManager_013ca798);Hash(&receiver,4);
        Hash(&FableSavedSoundVolume_013ca60c,4);Hash(&FableSoundDeactivated_013ca610,1);Hash(&FableWindowGamma_0138e0b8,4);
        Hash(events,count*sizeof(Event));
        if(errors || hash!=scenario.expected) { printf("FAIL case=%u message=%lx flags=%lx ime=%lu expected=%08lx actual=%08lx errors=%u\n",i,scenario.message,scenario.flags,scenario.ime,scenario.expected,hash,errors);return 1; }
    }
    printf("WIN_PROCEDURE_BEHAVIOR_PASS scenarios=%u\n",sizeof(scenarios)/sizeof(scenarios[0]));return 0;
}
