#include <stdio.h>
#include <stdlib.h>
struct Guarded {unsigned before,value,after;};
static Guarded* object;static unsigned mode,converted,destroyed,originalType;
#define CHECK(x) do {if(!(x)){fprintf(stderr,"BANK_EXCEPTION_FAILURE line=%d\n",__LINE__);exit(80);}}while(0)
extern "C" void* __fastcall Construct(void*,void*,unsigned,unsigned);
#pragma comment(linker,"/alternatename:@Construct@16=??0CBankFileException@@QAE@VCCharString@@W4EBankFileExceptionType@0@@Z")
extern "C" const char* __fastcall Convert(unsigned* storage,void*){CHECK(!converted && !destroyed && *storage==0x11223344 && object->value==0xcdcdcdcd);++converted;if(mode==1)storage[1]=0xdeadbeef;if(mode==2)object->value=0x55667788;return "controlled";}
extern "C" void __fastcall Destroy(unsigned* storage,void*){CHECK(converted==1 && !destroyed && *storage==0x11223344 && object->value==(mode==1?0xdeadbeef:originalType));++destroyed;if(mode==3)object->value=0x87654321;*storage=0;}
#pragma comment(linker,"/alternatename:??BCCharString@@QBEPBDXZ=@Convert@8")
#pragma comment(linker,"/alternatename:??1CCharString@@QAE@XZ=@Destroy@8")
int main(){unsigned values[]={0,1,2,3,0xffffffff};unsigned cases=0;for(unsigned i=0;i<5;++i)for(mode=0;mode<4;++mode){Guarded value={0x12345678,0xcdcdcdcd,0x76543210};object=&value;converted=destroyed=0;originalType=values[i];CHECK(Construct(&value.value,0,0x11223344,originalType)==&value.value);CHECK(value.before==0x12345678 && value.after==0x76543210 && converted==1 && destroyed==1);CHECK(value.value==(mode==3?0x87654321:mode==1?0xdeadbeef:originalType));++cases;}printf("NATIVE_BANK_EXCEPTION_PASS cases=%u\n",cases);return 0;}
