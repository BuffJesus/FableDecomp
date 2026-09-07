#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Message893ec0 {};
struct TypeFilter893ec0 { unsigned long messageType; };
struct VisibleFilter893ec0 { long maxWorldFrame0; long maxWorldFrame1; };
struct BoundFilter893ec0 { TypeFilter893ec0* filter0; VisibleFilter893ec0* filter1; };
struct MessageManager893ec0 { Message893ec0* FindMostRecentMessage(BoundFilter893ec0* filter); };
struct TimeManager893ec0 { bool IsDay(); };
struct CWorld { unsigned char unused00[0x1c]; TimeManager893ec0* time1c; unsigned char unused20[0x40]; MessageManager893ec0* messages60; };
struct CGameScriptInterface_Methods : CGameScriptInterface {
    long GetMaxWorldFrameForMessages() const;
    bool MsgOnHeroSlept(bool& outIsDay) const;
};
bool CGameScriptInterface_Methods::MsgOnHeroSlept(bool& outIsDay) const{long frame0=GetMaxWorldFrameForMessages();long frame1=GetMaxWorldFrameForMessages();MessageManager893ec0* manager=World->messages60;VisibleFilter893ec0 visible;visible.maxWorldFrame0=frame0;visible.maxWorldFrame1=frame1;TypeFilter893ec0 type;type.messageType=0x42;BoundFilter893ec0 filter;filter.filter0=&(type);filter.filter1=&(visible);Message893ec0* message=manager->FindMostRecentMessage(&(filter));if(message!=0){outIsDay=World->time1c->IsDay();return true;}return false;}