#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"
#include "fable_game_text_bank.h"
#include "fable_disk_file.h"
#include "fable_data_input_stream.h"
#include "fable_crc.h"
extern CBankFileManager g_FableBankFileManager_013CA79C;
extern CWideString g_CAFileCurrentPath_013BCA10;
unsigned long g_NativeTextReadContext_0138DE48=0x12345678;
unsigned long g_NativeTextReadContext_0138E064=0x87654321;
extern "C" void* _ReturnAddress();
#pragma intrinsic(_ReturnAddress)
extern "C" void ColdBoundary(){fprintf(stderr,"TEXT_PAYLOAD_COLD return=%p\n",_ReturnAddress());exit(80);}
static void require(bool ok,int line){if(!ok){fprintf(stderr,"TEXT_PAYLOAD_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
static bool observe;static unsigned observed;
class ObservedMemory:public CMemoryDataInputStream {
public:
 ObservedMemory(const void* p,unsigned long size):CMemoryDataInputStream(p,size){}
 virtual CCharString ReadNullTerminatedString(){CHECK(!observe || (g_NativeTextReadContext_0138DE48==11 && g_NativeTextReadContext_0138E064==11));++observed;return CMemoryDataInputStream::ReadNullTerminatedString();}
 virtual CWideString ReadNullTerminatedWString(){CHECK(!observe || (g_NativeTextReadContext_0138DE48==11 && g_NativeTextReadContext_0138E064==11));++observed;return CMemoryDataInputStream::ReadNullTerminatedWString();}
};
static unsigned long StringCRC(const CCharString& text){return FableCRC_Calc_004014A0(0,(const char*)text,text.GetLength());}

static unsigned char bytes[16384];static unsigned used;
static void put(const void* data,unsigned size){CHECK(used+size<sizeof(bytes));memcpy(bytes+used,data,size);used+=size;}
static void word(unsigned long value){put(&value,4);}
static void ansi(const char* value,bool sized){unsigned size=(unsigned)strlen(value);if(sized)word(size);put(value,size+(sized?0:1));}
int main(){
 unsigned cases=0;long chars=g_CCharStringInstanceCount_013BD800,wides=g_CWideStringInstanceCount_013BCA20;
 {NGameText::CTextEntry text;NGameText::CGroupEntry group;text.Owner=0;group.Owner=0;
 for(unsigned round=0;round<512;++round){
  wchar_t wide[65];unsigned length=(round*19)%65;for(unsigned i=0;i<length;++i)wide[i]=(wchar_t)(i%3==0?0xd83d:(i%3==1?0xde03:0x410+i));wide[length]=0;
  const char* names[]={"","short","LONGER_SPEECH_BANK_0123456789","narrator","sound/name"};const char* speech=names[round%5];const char* narrator=names[(round+1)%5];const char* sound=names[(round+3)%5];unsigned tags=(round*7)%19;
  used=0;put(wide,(length+1)*2);ansi(speech,true);ansi(narrator,true);ansi(sound,true);word(tags);
  for(unsigned i=0;i<tags;++i){word(0x80000000+round*97+i*17);ansi(names[(round+i)%5],false);}
  unsigned size=used;memset(bytes+size,0x7d,16);g_NativeTextReadContext_0138DE48=round*13;g_NativeTextReadContext_0138E064=~round;observed=0;observe=true;
  {ObservedMemory stream(bytes,size);text.ReadFromDataStream(stream);CHECK(stream.GetPosition()==size);}
  CHECK(observed==tags+1 && text.Text.GetLength()==length && !memcmp((const wchar_t*)text.Text,wide,(length+1)*2));CHECK(!strcmp((const char*)text.SpeechBank,speech) && !strcmp((const char*)text.Narrator,narrator) && !strcmp((const char*)text.SoundName,sound));CHECK(text.Tags.size()==tags);
  for(unsigned i=0;i<tags;++i)CHECK((unsigned long)text.Tags[i].Pos==0x80000000+round*97+i*17 && !strcmp((const char*)text.Tags[i].Name,names[(round+i)%5]));
  CHECK(g_NativeTextReadContext_0138DE48==round*13 && g_NativeTextReadContext_0138E064==~round);for(unsigned i=0;i<16;++i)CHECK(bytes[size+i]==0x7d);
  used=0;unsigned count=(round*31)%129;word(count);for(unsigned i=0;i<count;++i)word(0xf0000000+round*11+i*3);size=used;memset(bytes+size,0x7d,16);observe=false;observed=0;
  {ObservedMemory stream(bytes,size);group.ReadFromDataStream(stream);CHECK(stream.GetPosition()==size);}
  CHECK(group.Entries.size()==count && group.ResourceSize==sizeof(group)+group.Entries.capacity()*4 && !observed);for(unsigned i=0;i<count;++i)CHECK((unsigned long)group.Entries[i]==0xf0000000+round*11+i*3);for(unsigned i=0;i<16;++i)CHECK(bytes[size+i]==0x7d);++cases;
 }
 }
 CHECK(chars==g_CCharStringInstanceCount_013BD800 && wides==g_CWideStringInstanceCount_013BCA20);printf("NATIVE_TEXT_PAYLOAD_MATRIX_PASS text=%u groups=%u reused=1\n",cases,cases);return 0;
}
#include "NativeFileInputStreamColdAliases.h"
#include "NativeBankLifetimeColdAliases.h"
#include "NativeTextMetadataColdAliases.h"
#pragma comment(linker,"/alternatename:_UnusedBankBoundary=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetLastModifiedTime@CDataBank@NGameText@@MAE?AVCDateAndTime@@ABVCCharString@@@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?GetEnumerationType@CDataBank@NGameText@@MBE?AVCCharString@@K@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?UnloadNextResource@CResourceList@@QAE_NK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?HandleFailedAllocation@CResourceList@@UAE_NK@Z=_ColdBoundary")
#pragma comment(linker,"/alternatename:?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z=_ColdBoundary")

