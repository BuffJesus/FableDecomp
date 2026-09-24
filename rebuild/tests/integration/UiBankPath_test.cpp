#include "UiWideStringFixture.h"
#include "fable_ui_bank_registry.h"
#include <setjmp.h>
fable_i32 g_CCharStringInstanceCount_013BD800;
const char FableUiEmptyString[]={0};
const unsigned char FableUiGenericExceptionThrowInfo[]={0};
static jmp_buf missing;
static CCharStringData keys[6];
static FableUiBankRegistryView registry;
static FableUiBankTreeNode pathHead,aliasHead;
static FableUiBankPathNode pathNode;
static FableUiBankAliasNode aliasNode;
static bool found;
void* __cdecl FableUiAllocateStringBuffer(unsigned n)
{ printf(" NB%u",n); if(n>1000 || bufferCount==32) abort(); sizes[bufferCount]=n; return buffers[bufferCount++]; }
void __cdecl FableUiFreeStringBuffer(void* p) { printf(" NF%u",Id(p,buffers,1024)); }
__declspec(noreturn) void __stdcall FableUiThrowException(void*,const void* info)
{ if(info!=FableUiGenericExceptionThrowInfo) abort(); printf(" THROW_GENERIC"); longjmp(missing,1); }
int main()
{
    const char* names[]={0,"","a","b","missing","\x80"};
    const wchar_t* paths[]={0,L"",L"data\\",L"/",L"textures.big",L"\x0080\xffff",L"ab\0tail",L"a longer directory/"};
    for(unsigned seed=0;seed<64;++seed) for(unsigned q=0;q<6;++q)
    {
        memset(records,0xA5,sizeof(records)); memset(buffers,0xCD,sizeof(buffers)); memset(values,0,sizeof(values));
        recordCount=bufferCount=attempt=failAt=0; g_CWideStringInstanceCount_013BCA20=123; g_CCharStringInstanceCount_013BD800=200;
        for(unsigned i=1;i<6;++i) { keys[i].text=const_cast<char*>(names[i]); keys[i].owners=10; }
        printf("TRACE");
        FableUiConstructWideText(values,0,paths[seed%8]);
        FableUiConstructWideText(values+1,0,paths[(seed/8)%8]);
        memset(&registry,0,sizeof(registry)); memset(&pathHead,0,sizeof(pathHead)); memset(&aliasHead,0,sizeof(aliasHead));
        memset(&pathNode,0,sizeof(pathNode)); memset(&aliasNode,0,sizeof(aliasNode));
        registry.Paths.Head=&pathHead; registry.Aliases.Head=&aliasHead; registry.BasePath=values[0];
        pathHead.Parent=seed&16 ? &pathNode.Node.Links : 0;
        unsigned key=(seed/4)%4; pathNode.Node.Key.Storage=key ? keys+key : 0; pathNode.Value=values[1];
        aliasHead.Parent=seed&32 ? &aliasNode.Node.Links : 0;
        aliasNode.Node.Key.Storage=keys+2; unsigned target=seed%4; aliasNode.Value.Storage=target ? keys+target : 0;
        FableUiStringValue query; query.Storage=q ? keys+q : 0;
        if(FableUiGetStringText(&query,0)!=(q ? names[q] : FableUiEmptyString)) return 5;
        found=false;
        if(!setjmp(missing))
        { if(FableUiFindBankPath(&registry,0,values+4,&query)!=values+4) return 4; found=true; printf(" FOUND"); }
        Snapshot(); printf(" N:%d",g_CCharStringInstanceCount_013BD800);
        for(unsigned j=1;j<6;++j) printf(":%d",keys[j].owners);
        if(found) FableUiDestroyWideString(values+4,0);
        FableUiDestroyWideString(values+1,0); FableUiDestroyWideString(values,0);
        Snapshot(); printf(" END\n");
    }
    return 0;
}
