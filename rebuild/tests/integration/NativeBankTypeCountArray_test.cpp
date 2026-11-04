#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK(x) do {if(!(x)){fprintf(stderr,"TYPE_COUNT_FAILURE line=%d\n",__LINE__);exit(80);}}while(0)
static unsigned count,mode,allocations;static unsigned* object;static unsigned char* allocation;
extern "C" void* __cdecl FixtureTypeCountMalloc(size_t size){CHECK(count && size==count*8 && !allocations && !object[0] && !object[1] && !object[2]);allocation=static_cast<unsigned char*>(malloc(size+32));CHECK(allocation);memset(allocation,0xcd,size+32);++allocations;if(mode==1){object[0]=17;object[1]=23;object[2]=31;}return allocation+16;}
extern "C" void* __fastcall Construct(void*,void*,unsigned);
#pragma comment(linker,"/alternatename:@Construct@12=??0?$vector@VCBankFileDataTypeCount@@V?$allocator@VCBankFileDataTypeCount@@@_STL@@@_STL@@QAE@I@Z")
int main(){unsigned counts[]={0,1,2,7,16,31,256};unsigned cases=0;
 for(unsigned i=0;i<7;++i)for(mode=0;mode<2;++mode){unsigned words[7];for(unsigned j=0;j<7;++j)words[j]=0xcdcdcdcd;object=words+2;count=counts[i];allocations=0;allocation=0;CHECK(Construct(object,0,count)==object);CHECK(allocations==(count?1:0));unsigned expected=count?reinterpret_cast<unsigned>(allocation+16):0;CHECK(object[0]==expected && object[1]==expected+count*8 && object[2]==object[1]);CHECK(words[0]==0xcdcdcdcd && words[1]==0xcdcdcdcd && words[5]==0xcdcdcdcd && words[6]==0xcdcdcdcd);if(count){for(j=0;j<count*8+32;++j)CHECK(allocation[j]==(j>=16 && j<count*8+16?0:0xcd));free(allocation);}++cases;}
 printf("NATIVE_TYPE_COUNT_ARRAY_PASS cases=%u\n",cases);return 0;
}
