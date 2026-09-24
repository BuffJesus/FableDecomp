#include "UiWideStringFixture.h"
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* input=fopen(argv[1],"r"); if(!input) return 3;
    unsigned a,b,pattern;
    const wchar_t* texts[]={0,L"",L"a",L"data\\",L"textures.big",L"\x0080\xffff\xd800",L"ab\0suffix",L"longer asset directory/"};
    const long lengths[]={0,1,3,7,15,31};
    wchar_t source[64]; for(unsigned i=0;i<64;++i) source[i]=static_cast<wchar_t>(0x80+i); source[2]=0;
    while(fscanf(input,"%u %u %u %u",&a,&b,&pattern,&failAt)==4)
    {
        memset(records,pattern,sizeof(records)); memset(buffers,0xCD,sizeof(buffers)); memset(values,0,sizeof(values));
        recordCount=bufferCount=attempt=0; g_CWideStringInstanceCount_013BCA20=123; printf("TRACE");
        if(FableUiConstructWideText(values,0,texts[a])!=values) return 4; Snapshot();
        if(FableUiCopyWideString(values+1,0,values)!=values+1) return 5; Snapshot();
        FableUiConstructWideText(values+2,0,texts[b]); Snapshot();
        ++g_CWideStringInstanceCount_013BCA20;
        values[3].Storage=FableUiAllocateWideData(values+3,0,source,lengths[b]); Snapshot();
        FableUiMakeWideStringUnique(values+1,0); Snapshot(); failAt=0;
        FableUiAppendWideString(values,0,values+2); Snapshot();
        FableUiAppendWideString(values,0,values); Snapshot();
        FableUiAppendWideString(values+3,0,values+2); Snapshot();
        FableUiAppendWideString(values+3,0,values+2); Snapshot();
        if(FableUiConcatWideStrings(values+4,values+1,values+3)!=values+4) return 6; Snapshot();
        FableUiAssignWideString(values+2,0,values+4); Snapshot();
        FableUiAssignWideString(values+1,0,values+2); Snapshot();
        FableUiAssignWideString(values,0,values); FableUiAssignWideString(values+1,0,values+2); Snapshot();
        for(int k=4;k>=0;--k) { FableUiDestroyWideString(values+k,0); Snapshot(); }
        printf(" END\n");
    }
    fclose(input); return 0;
}
