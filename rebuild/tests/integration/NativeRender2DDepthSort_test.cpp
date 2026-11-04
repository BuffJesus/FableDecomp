#include <stdio.h>
#include <float.h>
#include "fable_render2d_sort.h"
int main(int argc,char** argv)
{
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");FILE* output=fopen(argv[2],"wb");if(!input||!output)return 3;
 unsigned control,cases=0;unsigned char records[192];
 while(fread(&control,4,1,input)==1)
 {
  if(fread(records,1,sizeof(records),input)!=sizeof(records))return 4;
  _clearfp();_controlfp(control,_MCW_PC|_MCW_RC);
  int results[]={FableCompareQuickDrawInfoDepth_009d8c30(records,records+96),FableCompareQuickDrawVerticesDepth_009d8cb0(records,records+96)};
  fwrite(results,4,2,output);fwrite(records,1,sizeof(records),output);++cases;
 }
 _clearfp();_controlfp(_PC_53|_RC_NEAR,_MCW_PC|_MCW_RC);
 if(ferror(input))return 5;fclose(input);if(fclose(output))return 6;
 printf("NATIVE_RENDER2D_DEPTH_SORT_PASS cases=%u\n",cases);return 0;
}
