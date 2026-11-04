#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aliases.h"
#include "fable_game_text_bank.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" char FixtureEmpty[1]={0};
static unsigned char* arena;
static unsigned mode,traceCount,events[32][3],live[8],liveCount,copies,destroys;
static unsigned& word(unsigned address){return *(unsigned*)address;}
static unsigned bank(){return (unsigned)arena+0x100;}
static void fail(){fprintf(stderr,"INIT_ENTRY_FAILURE\n");exit(80);}
static void require(bool v){if(!v)fail();}
static void event(unsigned kind,unsigned a=0,unsigned b=0){require(traceCount<32);events[traceCount][0]=kind;events[traceCount][1]=a;events[traceCount++][2]=b;}
static bool exists(unsigned p){for(unsigned i=0;i<liveCount;++i)if(live[i]==p)return true;return false;}
static void add(unsigned p){require(!exists(p)&&liveCount<8);live[liveCount++]=p;}
extern "C" unsigned* __fastcall FixtureSymbol(unsigned self,unsigned,unsigned key){require(self==bank()+0xac&&key==(unsigned)arena+0x300);event(1,word(key));if(mode==1)word(bank()+0x78)^=0x180;return (unsigned*)(arena+0x700);}
extern "C" unsigned __fastcall FixtureCRC(unsigned seed,unsigned data,unsigned length){require(seed==0);event(2,data==(unsigned)FixtureEmpty?0x129aaf4:data,length);if(mode==2)word(bank()+0x78)^=0x80;return 0x12345678;}
extern "C" void __fastcall FixtureOverflow(unsigned self,unsigned,unsigned pos,unsigned value,unsigned tag,unsigned count,bool atend){require(self==bank()+0xd0&&pos==word(bank()+0xd4)&&count==1&&atend);event(3,word(value),word(value+4));memcpy(arena+0x600,(void*)value,8);word(bank()+0xd0)=(unsigned)arena+0x600;word(bank()+0xd4)=(unsigned)arena+0x608;word(bank()+0xd8)=(unsigned)arena+0x620;}
template<> inline void _STL::vector<_STL::pair<unsigned long,unsigned long> >::_M_insert_overflow(_STL::pair<unsigned long,unsigned long>* pos,const _STL::pair<unsigned long,unsigned long>& value,const _STL::__false_type& tag,size_t count,bool atend){FixtureOverflow((unsigned)this,0,(unsigned)pos,(unsigned)&value,(unsigned)&tag,count,atend);}
typedef _STL::pair<unsigned long,unsigned long> Pair;
void (_STL::vector<Pair>::*ForceOverflow)(Pair*,const Pair&,const _STL::__false_type&,size_t,bool)=&_STL::vector<Pair>::_M_insert_overflow;
extern "C" unsigned __fastcall FixtureConvert(unsigned self,unsigned,unsigned out,unsigned input){require(self==bank()&&input==(unsigned)arena+0x304);word(out)=0x11223344;add(out);event(4,word(input));return out;}
extern "C" unsigned __fastcall FixtureLower(unsigned self,unsigned,unsigned out){require(exists(self)&&word(self)==0x11223344);word(out)=0x55667788;add(out);event(5,word(self));return out;}
extern "C" unsigned __fastcall FixtureCopy(unsigned self,unsigned,unsigned from){require(exists(from));word(self)=word(from);add(self);++copies;event(6,word(from));if(mode==3)word(bank()+0x78)^=0x40;return self;}
extern "C" void __fastcall FixtureDestroy(unsigned self,unsigned){require(exists(self));event(7,word(self));for(unsigned i=0;i<liveCount;++i)if(live[i]==self){live[i]=live[--liveCount];break;}word(self)=0;++destroys;}
extern "C" unsigned __fastcall FixtureInsert(unsigned self,unsigned,unsigned out,unsigned pair){require(self==bank()+0xc4&&exists(pair));event(8,word(pair),word(pair+4));word((unsigned)arena+0x710)=word(pair);word((unsigned)arena+0x714)=word(pair+4);word(out)=(unsigned)arena+0x710;return out;}
extern "C" void __fastcall CallInit(unsigned self,unsigned,unsigned index,unsigned symbol,unsigned filename);
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x2000000,0x10000,0x3000,4);require(arena==(void*)0x2000000);unsigned p[6],cases=0;
 while(fread(p,4,6,input)==6){
 for(unsigned i=0;i<4096;++i)arena[i]=(unsigned char)(i*31+17);
 unsigned a=(unsigned)arena,b=bank();mode=p[5];traceCount=liveCount=copies=destroys=0;
 word(b+0x78)=p[0];word(a+0x300)=p[1]?a+0x320:0;word(a+0x304)=p[2]?a+0x340:0;
 word(a+0x320)=a+0x360;word(a+0x324)=p[1]==2?3:0;word(a+0x340)=a+0x380;word(a+0x344)=p[2]==2?5:0;
 word(b+0xd0)=a+0x400;word(b+0xd4)=a+0x408;word(b+0xd8)=a+0x408+p[3]*8;
 CallInit(b,0,p[4],a+0x300,a+0x304);require(liveCount==0&&((copies==0&&destroys==0)||(copies==2&&destroys==4)));
 fwrite(&traceCount,4,1,output);fwrite(arena,1,4096,output);fwrite(events,12,traceCount,output);++cases;
 }
 fclose(input);fclose(output);printf("NATIVE_INIT_ENTRY_PASS cases=%u\n",cases);return 0;
}
