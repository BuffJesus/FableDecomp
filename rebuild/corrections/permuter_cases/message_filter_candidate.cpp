// Preserved source-shaping candidate from the VC7.1 permuter experiments.
struct CIsOfType { long Type; };
struct CCanBeSeenOrHeard { long MaxWorldFrame; long Field2; };
struct CBind2 { CIsOfType* p0; CCanBeSeenOrHeard* p1; };
struct CMsgMgr { int FindMostRecentMessage(CBind2* filter); };
struct CWorld { char pad[0x60]; CMsgMgr* mgr; };
struct CGSI { char pad0[4]; CWorld* world; };
extern "C" long __fastcall GetMaxWorldFrameForMessages(const CGSI* self);
bool __fastcall F(const CGSI* self){
CMsgMgr* mgr=self->world->mgr;
long a=GetMaxWorldFrameForMessages(self); long b=GetMaxWorldFrameForMessages(self);
CIsOfType f0; CCanBeSeenOrHeard f1; CBind2 bind;
bind.p0=&f0; bind.p1=&f1; f0.Type=0x3E; f1.MaxWorldFrame=a; f1.Field2=b;
int r=mgr->FindMostRecentMessage(&bind); return r?true:false;
}
