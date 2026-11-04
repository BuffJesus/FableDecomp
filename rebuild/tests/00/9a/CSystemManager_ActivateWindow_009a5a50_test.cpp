#include "fable_system_activation.h"
#include <stdio.h>
#include <string.h>

float FableSavedSoundVolume_013ca60c;
bool FableSoundDeactivated_013ca610;
struct CSystemManagerTestAccess {
    static void Run(CSystemManager* manager, bool active) { CSystemManager::ActivateWindow(manager, active); }
};
static unsigned char storage[0x180], input[0x60], alternateInput[0x60];
static unsigned long sound[32], alternateSound[32], table[52];
static CSystemManager* manager = reinterpret_cast<CSystemManager*>(storage);
static int scenario, count, failures;
struct Event { int kind, receiver; unsigned long a, b, c; };
static Event events[16];
static unsigned long bits(float f) { unsigned long n; memcpy(&n,&f,4); return n; }
static void record(int kind, void* receiver, unsigned long a=0, unsigned long b=0, unsigned long c=0) {
    Event& e=events[count++]; e.kind=kind;
    e.receiver=receiver==input?1:receiver==alternateInput?2:receiver==sound?3:receiver==alternateSound?4:0;
    e.a=a; e.b=b; e.c=c;
}
void CInputManager::SetHasInputFocus(bool active) {
    record(1,this,active);
    if (scenario & 128) manager->PInputManager.Data=reinterpret_cast<CInputManager*>(alternateInput);
}
void CInputManager::ClearEvents() { record(2,this); }
void CASoundManager::UnknownSetEnabled_009915e0(bool enabled) { record(6,this,enabled); }
struct SoundCallbacks {
    void Set(unsigned long group,float value,float transition) {
        record(4,this,group,bits(value),bits(transition));
        if (scenario & 256) manager->PSoundManager.Data=reinterpret_cast<CASoundManager*>(alternateSound);
    }
    float Get(unsigned long group) { record(3,this,group); return -0.375f; }
    bool Offset(float& value) {
        record(5,this, &value==&manager->ForceMusicPlayOffset);
        value=42.25f;
        return (scenario & 64)!=0;
    }
};
template<class T> static void slot(int n,T value) { memcpy(table+n,&value,4); }
static void check(bool ok) { if (!ok) ++failures; }
static int cursor;
static void expected(int kind,int receiver,unsigned long a=0,unsigned long b=0,unsigned long c=0) {
    if (cursor>=count) { ++failures; return; }
    Event& e=events[cursor++];
    check(e.kind==kind && e.receiver==receiver && e.a==a && e.b==b && e.c==c);
}
int main() {
    slot(46,&SoundCallbacks::Set); slot(47,&SoundCallbacks::Get); slot(49,&SoundCallbacks::Offset);
    sound[0]=alternateSound[0]=reinterpret_cast<unsigned long>(table);
    for (scenario=0;scenario<512;++scenario) {
        memset(storage,0xa5,sizeof(storage));
        bool active=(scenario&1)!=0, prior=(scenario&2)!=0, ever=(scenario&4)!=0;
        bool paused=(scenario&8)!=0, haveInput=(scenario&16)!=0, haveSound=(scenario&32)!=0;
        manager->ApplicationActive=prior; manager->EverBeenActive=ever;
        manager->Restore=false; manager->ApplicationHasInputFocus=!active;
        manager->ForceMusicPlay=false; manager->ForceMusicPlayOffset=-17.5f;
        manager->PInputManager.Data=haveInput?reinterpret_cast<CInputManager*>(input):0;
        manager->PSoundManager.Data=haveSound?reinterpret_cast<CASoundManager*>(sound):0;
        FableSavedSoundVolume_013ca60c=0.625f; FableSoundDeactivated_013ca610=paused;
        unsigned char before[0x180]; memcpy(before,storage,sizeof(storage));
        count=cursor=0;
        CSystemManagerTestAccess::Run(manager,active);
        if (haveInput) expected(1,1,active);
        int nextInput=(scenario&128)?2:1, nextSound=(scenario&256)?4:3;
        bool soundWork=haveSound && active==paused;
        if (active && haveInput) expected(2,nextInput);
        if (soundWork) {
            if (!active) expected(3,3,0x8000);
            expected(4,3,0x8000,active?bits(0.625f):0,0);
            if (!active) expected(5,nextSound,1);
            expected(6,nextSound,active);
        }
        if (!active && haveInput) expected(2,nextInput);
        check(cursor==count);
        check(manager->ApplicationActive==active && manager->EverBeenActive);
        check(manager->ApplicationHasInputFocus==active);
        check(manager->Restore==(active && !prior && ever));
        check(manager->ForceMusicPlay==(active && soundWork));
        check(manager->ForceMusicPlayOffset==(!active && soundWork?((scenario&64)?42.25f:0.0f):-17.5f));
        check(FableSoundDeactivated_013ca610==(soundWork?!active:paused));
        check(FableSavedSoundVolume_013ca60c==(!active && soundWork?-0.375f:0.625f));
        for (unsigned i=0;i<sizeof(storage);++i) {
            if (i==9 || i==10 || i==0xdc || i==0xea || i==0x16c || (i>=0x170 && i<0x174)
                || (i>=0x58 && i<0x5c) || (i>=0x7c && i<0x80)) continue;
            check(storage[i]==before[i]);
        }
        if (failures) { printf("FAIL scenario=%d failures=%d\n",scenario,failures); return 1; }
    }
    puts("ACTIVATION_BEHAVIOR_PASS scenarios=512"); return 0;
}
