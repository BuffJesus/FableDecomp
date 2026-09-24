#include "fable_ui_strings.h"
#include <stdio.h>
#include <string.h>

fable_i32 g_CCharStringInstanceCount_013BD800;
static unsigned char records[32][32],buffers[32][256];
static unsigned recordCount,bufferCount,failAt,attempt,bufferSizes[32];
static FableUiStringValue values[4];
static unsigned Id(const void* p,const void* base,unsigned stride)
{ return p ? (static_cast<const unsigned char*>(p)-static_cast<const unsigned char*>(base))/stride+1 : 0; }
void* __cdecl FableUiAllocateStringRecord(unsigned size)
{
    printf(" A%u",size); ++attempt;
    if(attempt==failAt) { printf(" FAIL"); return 0; }
    if(size!=17 || recordCount==32) return 0;
    return records[recordCount++];
}
void __cdecl FableUiFreeStringRecord(void* p)
{ printf(" R%u",Id(p,records,32)); }
void* __cdecl FableUiAllocateStringBuffer(unsigned size)
{
    printf(" B%u",size); if(size>256 || bufferCount==32) return 0;
    bufferSizes[bufferCount]=size; return buffers[bufferCount++];
}
void __cdecl FableUiFreeStringBuffer(void* p)
{ printf(" F%u",Id(p,buffers,256)); }
static void Snapshot()
{
    printf(" S:%d",g_CCharStringInstanceCount_013BD800);
    for(unsigned i=0;i<4;++i) printf(":%u",Id(values[i].Storage,records,32));
    for(unsigned j=0;j<recordCount;++j)
    {
        CCharStringData* d=reinterpret_cast<CCharStringData*>(records[j]);
        printf(" H%u:%u:%u:%08x:%02x:%d",j,Id(d->text,buffers,256),d->unknown04,d->unknown08,d->flags0C,d->owners);
        for(unsigned p=17;p<32;++p) printf("%02x",records[j][p]);
    }
    for(unsigned k=0;k<bufferCount;++k)
    { printf(" T%u:",k); for(unsigned x=0;x<bufferSizes[k]+4;++x) printf("%02x",buffers[k][x]); }
}
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* input=fopen(argv[1],"r"); if(!input) return 3;
    unsigned textCase,pattern; long length;
    char source[160];
    while(fscanf(input,"%u %ld %u %u",&textCase,&length,&pattern,&failAt)==4)
    {
        for(unsigned i=0;i<sizeof(source);++i) source[i]=static_cast<char>(33+i%90);
        if(textCase==0 || textCase==1) source[0]=0;
        if(textCase==2) strcpy(source,"GBANK_MAIN");
        if(textCase==3) strcpy(source,"GBANK_FRONT_END");
        if(textCase==4) { source[0]='a'; source[1]='b'; source[2]=0; }
        source[159]=0;
        memset(records,pattern,sizeof(records)); memset(buffers,0xCD,sizeof(buffers));
        memset(values,0,sizeof(values)); recordCount=bufferCount=attempt=0; g_CCharStringInstanceCount_013BD800=123;
        printf("TRACE");
        if(FableUiConstructBankName(&values[0],0,textCase ? source : 0,length)!=&values[0]) return 4; Snapshot();
        if(FableUiCopyString(&values[1],0,&values[0])!=&values[1]) return 5; Snapshot();
        if(values[0].Storage) FableUiAssignStringBytes(values[0].Storage,0,"replacement",3); Snapshot();
        if(FableUiConstructEmptyString(&values[2],0)!=&values[2]) return 6;
        FableUiConstructBankName(&values[3],0,"different",-1); Snapshot();
        FableUiAssignString(&values[2],0,&values[1]); Snapshot();
        FableUiAssignString(&values[0],0,&values[2]); Snapshot();
        FableUiAssignString(&values[1],0,&values[3]); Snapshot();
        FableUiAssignString(&values[0],0,&values[0]);
        FableUiAssignString(&values[2],0,&values[2]); Snapshot();
        FableUiAssignString(&values[2],0,&values[3]); Snapshot();
        FableUiMakeStringUnique(&values[2],0); Snapshot(); failAt=0;
        FableUiAppendStringLiteral(&values[2],0,"x"); Snapshot();
        FableUiAppendStringLiteral(&values[2],0,""); Snapshot();
        FableUiAppendStringLiteral(&values[0],0,"abcdefghijklmnop"); Snapshot();
        FableUiDestroyBankName(&values[1],0);
        FableUiConcatStringLiteral(&values[1],&values[2]," bank not found!"); Snapshot();
        if(values[1].Storage) values[1].Storage->flags0C^=1;
        FableUiAppendStringLiteral(&values[1],0,"0123456789abcdefghijklmnopqrstuvwxyz"); Snapshot();
        FableUiDestroyBankName(&values[3],0); Snapshot();
        FableUiDestroyBankName(&values[1],0); Snapshot();
        FableUiDestroyBankName(&values[2],0); Snapshot();
        FableUiDestroyBankName(&values[0],0); Snapshot();
        printf(" END\n");
    }
    fclose(input); return 0;
}
