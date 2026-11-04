#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_file_installer_active_files.h"
static unsigned live,baseCalls,destroyed;
static void require(bool value){if(!value)abort();}
static void* __cdecl Allocate(void*,unsigned count,unsigned width){void* p=calloc(count,width);require(p!=0);++live;return p;}
static void __cdecl Release(void*,void* p){if(p){require(live!=0);--live;free(p);}}
void CCompressorZlib::Compress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CCompressorZlib::Decompress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CCompressorZlib::CompressAsImage(const void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){abort();}
void CACompressor::Compress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CACompressor::Decompress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){abort();}
void CACompressor::CompressAsImage(void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){abort();}
void CACompressor::SetQuality(float){abort();}
extern "C" void __fastcall BDtor(void*){++baseCalls;}
static void FABLE_FASTCALL Destroy(void* p){require(live==0);++destroyed;delete static_cast<unsigned*>(p);}
static void transfer(InstallerTransferList& list,unsigned count){
 typedef InstallerTransferList::Link Link;
 Link* head=static_cast<Link*>(malloc(sizeof(Link)));require(head!=0);list.head=head;Link* last=head;
 for(unsigned i=0;i<count;++i){Link* link=static_cast<Link*>(malloc(sizeof(Link)));require(link!=0);last->next=link;link->previous=last;last=link;}
 last->next=head;head->previous=last;if(!count)head->next=head;
}
int main(){
 typedef InstallerActiveFileList::Node Node;unsigned cases=0,totalNodes=0;
 for(unsigned cycle=0;cycle!=16;++cycle)for(unsigned count=1;count!=4;++count)for(unsigned streamMode=0;streamMode!=3;++streamMode){
  unsigned long sentinelStorage[sizeof(Node)/4];Node* head=reinterpret_cast<Node*>(sentinelStorage);
  InstallerActiveFileList owner;owner.head=head;head->next=head->previous=head;Node* last=head;
  unsigned* object=new unsigned(0x12345678);CCPPointerInfo* control=new CCPPointerInfo(Destroy,object);control->RefCount=count;
  for(unsigned i=0;i<count;++i){
   Node* n=static_cast<Node*>(malloc(sizeof(Node)));require(n!=0);memset(n,0,sizeof(Node));
   ::new(&n->file)InstallerActiveFileList::Value();n->file.Data=reinterpret_cast<const CFileInstaller::CFileInfo*>(object);n->file.Info=control;
   transfer(n->input,cycle%3);transfer(n->output,(cycle+1)%3);
   CCompressorZlib& c=n->compressor;c.Stream.zalloc=Allocate;c.Stream.zfree=Release;
   if(streamMode){int status=streamMode==1?z_deflateInit_(&c.Stream,6,"1.2.1",sizeof(z_stream_s)):z_inflateInit_(&c.Stream,"1.2.1",sizeof(z_stream_s));require(status==0);c.ZlibInitialised=true;c.Compressing=streamMode==1;}
   last->next=n;n->previous=last;last=n;
  }
  last->next=head;head->previous=last;owner.Clear();totalNodes+=count;
  require(live==0&&destroyed==cases+1&&baseCalls==totalNodes&&head->next==head&&head->previous==head);
  owner.Clear();require(destroyed==cases+1&&baseCalls==totalNodes);++cases;
 }
 printf("INSTALLER_ACTIVE_LIFETIME_OK cases=%u nodes=%u objects=%u\n",cases,totalNodes,destroyed);return 0;
}
