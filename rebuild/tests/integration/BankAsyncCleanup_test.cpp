#include "fable_threaded_file.h"
#include "fable_game_text_bank.h"
#include "fable_critical_section_lock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static CBankFileAsync* bank;
static int locked,lockCalls,unlockCalls,abortCalls,stopAt,deletes,threadCloses,baseCloses,sequence,failed;
static void* expectedSection;
CCriticalSectionLock::CCriticalSectionLock(_RTL_CRITICAL_SECTION& section,bool tryOnly){if(tryOnly||locked||&section!=expectedSection)failed=1;CriticalSection=&section;Locked=true;locked=1;++lockCalls;}
CCriticalSectionLock::~CCriticalSectionLock(){if(!locked||CriticalSection!=expectedSection)failed=2;locked=0;++unlockCalls;}
bool CBankFileAsyncData::Abort(){if(!locked||this!=(CBankFileAsyncData*)(0x1000+abortCalls*0x100))failed=3;return abortCalls++!=stopAt;}
static void FABLE_FASTCALL destroy(void* p){if(!locked||p!=(void*)0x76540000)failed=4;++deletes;}
void CThreadedFile::Close(){if(this!=(CThreadedFile*)0x12340000||sequence++)failed=5;++threadCloses;}
void CBankFile::Close(){if(this!=bank||sequence!=(bank->ThreadedFile.data_&&!bank->RetailMode?1:0))failed=6;++baseCloses;}
typedef _STL::_List_node<CCountedPointer<CBankFileAsyncData> > Node;
int main(){int cases=0;
 for(int n=0;n<=16;++n)for(int stop=0;stop<=n;++stop)for(int mode=0;mode<4;++mode)for(int shared=0;shared<2;++shared){
  union{double align;char data[0x164];} mem;memset(&mem,0xa5,sizeof(mem));bank=(CBankFileAsync*)&mem;expectedSection=bank->CriticalSection;Node sentinel;Node* nodes[16];CCPPointerInfo* controls[16];unsigned refs[16];
  sentinel._M_next=sentinel._M_prev=&sentinel;bank->DisposalList._M_node=&sentinel;locked=lockCalls=unlockCalls=abortCalls=deletes=threadCloses=baseCloses=failed=0;stopAt=stop;
  for(int i=0;i<n;++i){nodes[i]=(Node*)malloc(sizeof(Node));nodes[i]->_M_next=&sentinel;nodes[i]->_M_prev=sentinel._M_prev;sentinel._M_prev->_M_next=nodes[i];sentinel._M_prev=nodes[i];nodes[i]->_M_data.Data=(CBankFileAsyncData*)(0x1000+i*0x100);
   refs[i]=mode==0?0:mode==1?1:mode==2?2:0xffffffff;controls[i]=!mode?0:shared&&i?controls[0]:new CCPPointerInfo(destroy,(void*)0x76540000);if(controls[i])controls[i]->RefCount=shared?(unsigned)n+(mode==1?0:1):refs[i];nodes[i]->_M_data.Info=controls[i];
  }
  char before[0x164];memcpy(before,&mem,sizeof(mem));bank->ProcessDisposalList();int removed=stop;
  if(failed||locked||lockCalls!=1||unlockCalls!=1||abortCalls!=removed+(stop<n)||memcmp(before,&mem,sizeof(mem)))return 1;
  int expectedDeletes=mode==1?(shared?(n&&removed==n):removed):0;if(deletes!=expectedDeletes)return 2;
  Node* prev=&sentinel;_STL::_List_node_base* at=sentinel._M_next;for(int i=removed;i<n;++i){if(at!=nodes[i]||at->_M_prev!=prev)return 3;prev=nodes[i];at=at->_M_next;}if(at!=&sentinel||sentinel._M_prev!=prev)return 4;
  if(mode&&n){if(shared){if(!(mode==1&&removed==n)&&controls[0]->RefCount!=(unsigned)n+(mode==1?0:1)-removed)return 5;}else for(int i=0;i<n;++i)if(!(mode==1&&i<removed)&&controls[i]->RefCount!=refs[i]-(i<removed))return 6;}
  for(int i=removed;i<n;++i)free(nodes[i]);if(mode&&n){if(shared){if(!(mode==1&&removed==n))delete controls[0];}else for(int i=0;i<n;++i)if(!(mode==1&&i<removed))delete controls[i];}
  // Raw node storage was released explicitly; keep sentinel's automatic pointer empty.
  for(int present=0;present<2;++present)for(int retail=0;retail<2;++retail){bank->ThreadedFile.data_=present?(CThreadedFile*)0x12340000:0;bank->RetailMode=retail!=0;sequence=threadCloses=baseCloses=0;memcpy(before,&mem,sizeof(mem));bank->CBankFileAsync::Close();if(failed||threadCloses!=(present&&!retail)||baseCloses!=1||memcmp(before,&mem,sizeof(mem)))return 7;}
  ++cases;
 }
 printf("BANK_ASYNC_CLEANUP_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
