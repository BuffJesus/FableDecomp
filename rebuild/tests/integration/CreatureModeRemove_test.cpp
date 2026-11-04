#include "fable_world.h"
#include "fable_game.h"
#include <stdio.h>
#include <string.h>
typedef CTCCreatureModeManager::CModeBase Mode;
typedef CCountedPointer<Mode> ModePointer;
typedef _STL::_List_node<ModePointer> Node;
typedef _STL::pair<ETCInterfaceType,CTCBase*> Entry;
long CWorld::Frame;
long CMainGameComponent::ConstantFPS;
struct WorldFrameTestAccess : CWorld { static void Set(long n){Frame=n;} };
struct CGameEventDispatchTestAccess { static void SetFPS(long n){CMainGameComponent::ConstantFPS=n;} };
struct CreatureModeManagerTestAccess {static void Setup(CTCCreatureModeManager& m,Node* sentinel){memcpy(&m.PModes,&sentinel,4);}static long Deadline(const CTCCreatureModeManager& m){return m.TimeForNextIdleMode;}};
struct CThingComponentTestAccess {static void Setup(CThing& t,Entry* e,unsigned n,bool has){t.UsingComponents.reset();t.UsingComponents.set(TCI_INVENTORY);t.UsingComponents.set(TCI_LOOK,has);t.Components._M_start=e;t.Components._M_finish=e+n;t.Components._M_end_of_storage._M_data=e+n+1;}};
struct Trace {unsigned kind,id,value;};static Trace trace[32],expectedTrace[32];static unsigned traceCount,expectedCount,errors,sideEffect;static CTCCreatureModeManager* manager;static CThing* secondParent;static CTCLook* expectedLook;static long currentFPS,currentFrame;
static void Record(unsigned kind,unsigned id,unsigned value=0){if(traceCount>=32){++errors;return;}trace[traceCount].kind=kind;trace[traceCount].id=id;trace[traceCount++].value=value;}
static void Expect(unsigned kind,unsigned id,unsigned value=0){expectedTrace[expectedCount].kind=kind;expectedTrace[expectedCount].id=id;expectedTrace[expectedCount++].value=value;}
Mode::~CModeBase(){}
void Mode::AnimUpdate(CAnimateThing&){++errors;}
void Mode::FrameUpdate(){++errors;}
const CAnimFilterBase& Mode::GetAnimFilter()const{++errors;return *reinterpret_cast<const CAnimFilterBase*>(0xdeadbeef);}
void Mode::OnActivate(){++errors;}
void Mode::OnDeactivate(){++errors;}
float Mode::GetMovementSpeed(const C3DVector&,const C3DVector&,EMovementType)const{++errors;return 0;}
bool Mode::IsIdleMode()const{++errors;return false;}
bool Mode::IsAllowingPlayerSpecialMovementMoves()const{++errors;return false;}
bool Mode::IsHoldWeaponWithBothHands()const{++errors;return false;}
bool Mode::IsRemoveOnDeactivate()const{++errors;return false;}
class TestMode : public Mode {
public:
    unsigned long Type;unsigned Id;
    unsigned long GetType()const{Record(1,Id);return Type;}
    void OnDeactivate(){Record(2,Id);if(sideEffect==1)Type^=0x80000000UL;}
    void OnActivate(){Record(4,Id);if(sideEffect==2)memcpy((unsigned char*)manager+4,&secondParent,4);}
};
// Explicit ABI boundary: unlink semantics only, without allocation/refcount coverage.
template<class T,class A> template<class P> void _STL::list<T,A>::remove_if(P predicate){unsigned long type;memcpy(&type,&predicate,4);Record(3,0,type);_List_node_base* node=_M_node->_M_next;while(node!=_M_node){_List_node_base* next=node->_M_next;if(predicate(static_cast<_List_node<T>*>(node)->_M_data))node->_M_unhook();node=next;}}
template void _STL::list<ModePointer>::remove_if<Mode::CIsType>(Mode::CIsType);
void CTCLook::UpdateTurningAnims(){if(this!=expectedLook)++errors;Record(5,0);if(sideEffect==3){++currentFPS;currentFrame=(long)((unsigned long)currentFrame+17UL);CGameEventDispatchTestAccess::SetFPS(currentFPS);WorldFrameTestAccess::Set(currentFrame);}}
static const unsigned long lists[8][4]={{0},{12},{12,1},{1,12,12},{12,12,12},{0x80000000UL,12,0x7fffffffUL},{0x7fffffffUL,0x80000000UL,0xffffffffUL},{1,0,12}};static const unsigned counts[8]={0,1,2,3,3,3,3,3};static const unsigned long queries[6]={0,1,12,0xffffffffUL,0x80000000UL,0x7fffffffUL};static const long fpsValues[7]={0,1,15,30,60,-1,-15};static const long frames[5]={0,1,-1,(long)0x80000000UL,0x7fffffff};static const long keys[5][7]={{0},{67},{(long)0x80000000UL,-1,17,67,67,68,0x7fffffff},{66,68},{0,17,49,54,273}};static const unsigned mapCounts[5]={0,1,7,2,5};
int main(){unsigned scenarios=0;for(unsigned pattern=0;pattern<2;++pattern)for(unsigned list=0;list<8;++list)for(unsigned query=0;query<6;++query)for(unsigned has=0;has<2;++has)for(unsigned map=0;map<5;++map)for(unsigned fps=0;fps<7;++fps)for(unsigned frame=0;frame<5;++frame)for(sideEffect=0;sideEffect<4;++sideEffect){
    unsigned long storage[80],p1[128],p2[128],before[80],pb1[128],pb2[128];memset(storage,pattern*71,sizeof(storage));memset(p1,pattern*71,sizeof(p1));memset(p2,pattern*71,sizeof(p2));manager=reinterpret_cast<CTCCreatureModeManager*>(storage+2);CThing* firstParent=reinterpret_cast<CThing*>(p1+2);secondParent=reinterpret_cast<CThing*>(p2+2);memcpy((unsigned char*)manager+4,&firstParent,4);
    TestMode modes[3];Node nodes[4];unsigned long types[3];unsigned n=counts[list];for(unsigned i=0;i<3;++i){modes[i].Type=types[i]=i<n?lists[list][i]:0x55555555UL;modes[i].Id=i;}for(i=0;i<=n;++i){nodes[i]._M_next=i<n?&nodes[i+1]:&nodes[0];nodes[i]._M_prev=i?&nodes[i-1]:&nodes[n];nodes[i]._M_data.Data=i?&modes[i-1]:reinterpret_cast<Mode*>(0xdeadbeef);nodes[i]._M_data.Info=0;}CreatureModeManagerTestAccess::Setup(*manager,nodes);
    Entry entries[8],eb[8];memset(entries,pattern*71,sizeof(entries));unsigned found=mapCounts[map];for(i=0;i<=mapCounts[map];++i){entries[i].first=static_cast<ETCInterfaceType>(i<mapCounts[map]?keys[map][i]:0x7fffffff);entries[i].second=reinterpret_cast<CTCBase*>(0x2500040+i*0x20);if(i<mapCounts[map] && keys[map][i]==67 && found==mapCounts[map])found=i;}expectedLook=reinterpret_cast<CTCLook*>(entries[found].second);CThingComponentTestAccess::Setup(*firstParent,entries,mapCounts[map],has!=0);CThingComponentTestAccess::Setup(*secondParent,entries,mapCounts[map],has==0);memcpy(eb,entries,sizeof(entries));memcpy(before,storage,sizeof(storage));memcpy(pb1,p1,sizeof(p1));memcpy(pb2,p2,sizeof(p2));
    traceCount=expectedCount=errors=0;currentFPS=fpsValues[fps];currentFrame=frames[frame];CGameEventDispatchTestAccess::SetFPS(currentFPS);WorldFrameTestAccess::Set(currentFrame);bool removedCurrent=n && types[0]==queries[query];if(n)Expect(1,0);if(removedCurrent){Expect(2,0);if(sideEffect==1)types[0]^=0x80000000UL;}Expect(3,0,queries[query]);unsigned live[3],liveCount=0;for(i=0;i<n;++i){Expect(1,i);if(types[i]!=queries[query])live[liveCount++]=i;}
    bool activate=removedCurrent && liveCount;bool turning=activate && (sideEffect==2?has==0:has!=0);if(activate){Expect(4,live[0]);if(sideEffect==2)memcpy((unsigned char*)(before+2)+4,&secondParent,4);}if(turning)Expect(5,0);long finalFPS=fpsValues[fps]+(turning && sideEffect==3?1:0);unsigned long finalFrame=(unsigned long)frames[frame]+(turning && sideEffect==3?17UL:0);long deadline=(long)(finalFrame+(unsigned long)(finalFPS*30));memcpy((unsigned char*)(before+2)+0x14,&deadline,4);
    manager->RemoveMode(queries[query]);if(errors || traceCount!=expectedCount || memcmp(trace,expectedTrace,traceCount*sizeof(Trace)) || currentFPS!=finalFPS || (unsigned long)CWorld::GetFrame()!=finalFrame || CMainGameComponent::GetConstantFPS()!=finalFPS || CreatureModeManagerTestAccess::Deadline(*manager)!=deadline || memcmp(before,storage,sizeof(storage)) || memcmp(pb1,p1,sizeof(p1)) || memcmp(pb2,p2,sizeof(p2)) || memcmp(eb,entries,sizeof(entries)))return 1;
    Node* node=static_cast<Node*>(nodes[0]._M_next);for(i=0;i<liveCount;++i){if(node!=&nodes[live[i]+1] || modes[live[i]].Type!=types[live[i]])return 2;node=static_cast<Node*>(node->_M_next);}if(node!=&nodes[0])return 3;++scenarios;
}
printf("CREATURE_MODE_REMOVE_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;}
