#include "fable_threaded_file.h"
#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef _STL::_Rb_tree_node<CBankFileAsyncData*> TreeNode;
typedef _STL::_List_node<CCountedPointer<CBankFileAsyncData> > ListNode;
static CBankFileAsync* bank;
static CBankFileAsyncData* entries[24];
static unsigned long seed;
static int failed,locks,locked,adds,aborts,stopAt;
CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION& s,bool only){if(locked||only||&s!=(void*)bank->CriticalSection)failed=1;CriticalSection=&s;Locked=true;locked=1;++locks;}
CCriticalSectionLock::~CCriticalSectionLock(){if(!locked||CriticalSection!=(void*)bank->CriticalSection)failed=2;locked=0;}
bool CBankFileAsyncData::Abort(){if(!locked||locks!=1||this!=(CBankFileAsyncData*)(0x1000+aborts*0x100))failed=3;return aborts++!=stopAt;}
unsigned long CThreadedFile::AddObject(const CThreadedFileObjectDesc& desc){int i=adds++;if(!locked||locks!=2||&desc!=&entries[i]->LoadDesc||this!=entries[i]->BankFile.ThreadedFile.data_||entries[i]->ThreadedFileUID!=0x55550000u+i)failed=4;return seed+i*7;}
static TreeNode* build(int first,int last,int shape,TreeNode* parent){if(first==last)return 0;int at=shape==0?(first+last)/2:shape==1?first:shape==2?last-1:(first+last-1)/2;TreeNode* node=(TreeNode*)malloc(sizeof(TreeNode));memset(node,0,sizeof(*node));node->_M_parent=parent;node->_M_value_field=entries[at];node->_M_left=build(first,at,shape,node);node->_M_right=build(at+1,last,shape,node);return node;}
int main(){int cases=0;
 for(int n=0;n<=24;++n)for(int shape=0;shape<4;++shape)for(int variant=0;variant<3;++variant)for(int disposal=0;disposal<3;++disposal){
  union{double align;char data[0x164];} bankMem,ownerMem[3];union{double align;char data[24*0x48];} dataMem;memset(&bankMem,0xa5,sizeof(bankMem));memset(&ownerMem,0xb6,sizeof(ownerMem));memset(&dataMem,0xc7,sizeof(dataMem));bank=(CBankFileAsync*)&bankMem;seed=variant==0?0:variant==1?0x87654321:0xffffffff;
  for(int i=0;i<n;++i){entries[i]=(CBankFileAsyncData*)(dataMem.data+i*0x48);CBankFileAsync* owner=(CBankFileAsync*)&ownerMem[i%3];owner->ThreadedFile.data_=(CThreadedFile*)(0x12340000+i%3*0x100);memcpy(dataMem.data+i*0x48+0x28,&owner,4);entries[i]->ThreadedFileUID=0x55550000u+i;entries[i]->LoadDesc.FilePos=i*13;entries[i]->LoadDesc.Size=i*29;}
  TreeNode header;memset(&header,0,sizeof(header));header._M_parent=build(0,n,shape,&header);header._M_left=header._M_right=&header;if(n){_STL::_Rb_tree_node_base* p=header._M_parent;while(p->_M_left)p=p->_M_left;header._M_left=p;p=header._M_parent;while(p->_M_right)p=p->_M_right;header._M_right=p;}
  bank->ReadsToRestart._M_t._M_header._M_data=&header;bank->ReadsToRestart._M_t._M_node_count=n;
  ListNode sentinel;ListNode* nodes[2];sentinel._M_next=sentinel._M_prev=&sentinel;for(int i=0;i<2;++i){nodes[i]=(ListNode*)malloc(sizeof(ListNode));nodes[i]->_M_next=&sentinel;nodes[i]->_M_prev=sentinel._M_prev;sentinel._M_prev->_M_next=nodes[i];sentinel._M_prev=nodes[i];nodes[i]->_M_data.Data=(CBankFileAsyncData*)(0x1000+i*0x100);nodes[i]->_M_data.Info=0;}bank->DisposalList._M_node=&sentinel;
  char before[24*0x48];memcpy(before,&dataMem,sizeof(dataMem));failed=locks=locked=adds=aborts=0;stopAt=disposal;bank->AsyncUpdatePending();
  if(failed||locked||locks!=2||adds!=n||aborts!=disposal+(disposal<2)||bank->ReadsToRestart._M_t._M_node_count||header._M_parent||header._M_left!=&header||header._M_right!=&header)return 1;
  for(int i=0;i<n;++i){if(entries[i]->ThreadedFileUID!=seed+i*7)return 2;memcpy(before+i*0x48+0x18,&entries[i]->ThreadedFileUID,4);}if(memcmp(before,&dataMem,sizeof(dataMem)))return 3;
  _STL::_List_node_base* at=sentinel._M_next;for(int i=disposal;i<2;++i){if(at!=nodes[i])return 4;at=at->_M_next;free(nodes[i]);}if(at!=&sentinel)return 5;
  ++cases;
 }
 printf("BANK_RESTART_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
