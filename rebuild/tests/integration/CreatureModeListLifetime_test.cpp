#include "fable_thing_components.h"
#include <stdio.h>
#include <string.h>
#include <new>
typedef CTCCreatureModeManager::CModeBase Mode;
typedef CCountedPointer<Mode> Pointer;
typedef _STL::_List_node<Pointer> Node;
typedef _STL::list<Pointer> List;
struct Trace {unsigned kind,id;};static Trace trace[16],expected[16];static unsigned traceCount,expectedCount,errors,lastVisited,queryValue;static bool running,mutate;static Node* nodes[4];static CCPPointerInfo* infoForNode[3];
static void Record(unsigned kind,unsigned id){if(traceCount>=16){++errors;return;}trace[traceCount].kind=kind;trace[traceCount++].id=id;}
static void Expect(unsigned kind,unsigned id){expected[expectedCount].kind=kind;expected[expectedCount++].id=id;}
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
class TestMode : public Mode {public:unsigned long Type;unsigned Id;unsigned long GetType()const{lastVisited=Id;Record(1,Id);return Type;}};
static void FABLE_FASTCALL DeleteBoundary(void* data){if(!running)return;TestMode* owner=static_cast<TestMode*>(static_cast<Mode*>(data));Record(2,owner->Id);if(!infoForNode[lastVisited] || infoForNode[lastVisited]->RefCount!=0)++errors;if(mutate){Node* next=static_cast<Node*>(nodes[lastVisited+1]->_M_next);if(next!=nodes[0])static_cast<TestMode*>(next->_M_data.Data)->Type=queryValue;nodes[lastVisited+1]->_M_next=nodes[0];}}
static const unsigned long values[8][3]={{0},{12},{12,1},{1,12,12},{12,12,12},{0x80000000UL,12,0x7fffffffUL},{0x7fffffffUL,0x80000000UL,0xffffffffUL},{1,0,12}};static const unsigned counts[8]={0,1,2,3,3,3,3,3};static const unsigned long queries[6]={0,1,12,0xffffffffUL,0x80000000UL,0x7fffffffUL};
int main(){unsigned scenarios=0;for(unsigned pattern=0;pattern<4;++pattern)for(unsigned list=0;list<8;++list)for(unsigned query=0;query<6;++query)for(unsigned ownership=0;ownership<6;++ownership)for(unsigned effect=0;effect<2;++effect){
    TestMode modes[3];unsigned long types[3],refs[3];CCPPointerInfo* infos[3]={0,0,0};unsigned infoCount=ownership==0?0:ownership==5?(counts[list]?1:0):counts[list];unsigned n=counts[list];for(unsigned i=0;i<3;++i){modes[i].Type=types[i]=i<n?values[list][i]:0x55555555UL;modes[i].Id=i;infoForNode[i]=0;}
    for(i=0;i<infoCount;++i){refs[i]=ownership==1?1:ownership==2?2:ownership==3?0xffffffffUL:ownership==4?0:n;infos[i]=new CCPPointerInfo(DeleteBoundary,&modes[ownership==5?0:i]);infos[i]->RefCount=refs[i];}
    for(i=0;i<=n;++i){nodes[i]=static_cast<Node*>(malloc(sizeof(Node)));new(nodes[i])Node;nodes[i]->_M_data.Data=i?&modes[i-1]:reinterpret_cast<Mode*>(0xdeadbeef);nodes[i]->_M_data.Info=i && infoCount?infos[ownership==5?0:i-1]:0;if(i)infoForNode[i-1]=nodes[i]->_M_data.Info;}
    for(i=0;i<=n;++i){nodes[i]->_M_next=i<n?nodes[i+1]:nodes[0];nodes[i]->_M_prev=i?nodes[i-1]:nodes[n];}
    unsigned long storage[16],before[16];memset(storage,pattern*71,sizeof(storage));List* target=reinterpret_cast<List*>(storage+2);target->_M_node=nodes[0];memcpy(before,storage,sizeof(storage));traceCount=expectedCount=errors=0;queryValue=queries[query];mutate=effect!=0;bool removed[3]={false,false,false};
    for(i=0;i<n;++i){Expect(1,i);if(types[i]==queryValue){removed[i]=true;if(infoCount){unsigned index=ownership==5?0:i;--refs[index];if(refs[index]==0){Expect(2,ownership==5?0:i);if(mutate && i+1<n)types[i+1]=queryValue;}}}}
    running=true;target->remove_if(Mode::CIsType((long)queryValue));running=false;if(errors || traceCount!=expectedCount || memcmp(trace,expected,traceCount*sizeof(Trace)) || memcmp(before,storage,sizeof(storage)))return 1;
    Node* node=static_cast<Node*>(nodes[0]->_M_next);Node* prev=nodes[0];for(i=0;i<n;++i){if(modes[i].Type!=types[i] || modes[i].Id!=i)return 2;if(!removed[i]){if(node!=nodes[i+1] || node->_M_prev!=prev || node->_M_data.Data!=&modes[i] || node->_M_data.Info!=infoForNode[i])return 3;prev=node;node=static_cast<Node*>(node->_M_next);}}if(node!=nodes[0] || nodes[0]->_M_prev!=prev)return 4;
    for(i=0;i<infoCount;++i)if(refs[i]!=0 && (infos[i]->RefCount!=refs[i] || infos[i]->DeleteFunc!=DeleteBoundary || infos[i]->Data!=&modes[ownership==5?0:i]))return 5;
    node=static_cast<Node*>(nodes[0]->_M_next);while(node!=nodes[0]){Node* next=static_cast<Node*>(node->_M_next);node->_M_data.Info=0;node->_M_data.~Pointer();free(node);node=next;}nodes[0]->_M_data.~Pointer();free(nodes[0]);for(i=0;i<infoCount;++i)if(refs[i]!=0)delete infos[i];++scenarios;
}
printf("CREATURE_MODE_LIST_LIFETIME_PASS scenarios=%u\n",scenarios);return 0;}
