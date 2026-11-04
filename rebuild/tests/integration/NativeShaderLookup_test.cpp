#include "fable_shader_resource.h"
#include "fable_shader_data_bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
static unsigned char* arena;
static FILE* output;
static unsigned indexValue,mode;
static unsigned& word(unsigned offset) { return *reinterpret_cast<unsigned*>(arena+offset); }
static void requireAt(bool v,unsigned line) { if(!v) {printf("require line %u index %u mode %u\n",line,indexValue,mode);fflush(output);exit(21);} }
#define require(v) requireAt((v),__LINE__)
static void event(unsigned kind,unsigned a=0,unsigned b=0) {
    unsigned header[]={kind,a,b};fwrite(header,4,3,output);
    unsigned char copy[0x1000];memcpy(copy,arena,sizeof(copy));
    memset(copy+0xa00,0,4);memset(copy+0xa20,0,4);memset(copy+0xa40,0,4);
    fwrite(copy,1,sizeof(copy),output);
}
void* operator new(size_t n) {
    if(!arena) return malloc(n);
    require(n==8);event(2,8);
    if(mode){word(0x34)=0x12345678;word(4)=(unsigned)(arena+0x400);}
    return arena+0xa20;
}
void operator delete(void* p) { if(arena){printf("unexpected delete %p\n",p);fflush(output);exit(22);}free(p); }
unsigned long CBankFile::FindIndexBySymbol(const CCharString& name) const {
    require(this==(void*)(arena+0x200) && &name==(void*)(arena+0x700));
    event(1,(unsigned)this,*(const unsigned*)&name);
    if(mode==1){word(4)=(unsigned)(arena+0x400);word(8)=(unsigned)(arena+0x100);}
    return indexValue;
}
CCharString::CCharString() : storage_(0) {}
CCharString::CCharString(const CCharString& other) {
    bool symbolSource=false;
#ifdef FABLE_COMPOSE_BANK_SYMBOLS
    symbolSource=((unsigned)&other>=(unsigned)(arena+0xd00)&&(unsigned)&other<(unsigned)(arena+0xd20))||((unsigned)&other>=(unsigned)(arena+0xd80)&&(unsigned)&other<(unsigned)(arena+0xda0));
#endif
    require(&other==(void*)(arena+0x620)||&other==(void*)(arena+0x660)||symbolSource);
    event(3,(unsigned)&other,(unsigned)other.storage_);storage_=other.storage_;
    if(mode==1)word(4)=(unsigned)(arena+0x200);
    if(symbolSource&&mode==2)word(0x34)=0x87654321;
}
CCharString::~CCharString() {
    event(7,(unsigned)storage_);storage_=0;
    if(mode==2)word(0x34)^=0x100;
}
extern "C" CCharString* __fastcall LookupSymbolBoundary(const CBankFile* bank,void*,CCharString* result,unsigned long index) {
    require(index==indexValue && (bank==(void*)(arena+0x200)||bank==(void*)(arena+0x400)));
    event(4,(unsigned)bank,index);
    *(unsigned*)result=0x222000+(unsigned)bank-(unsigned)arena;
    if(mode==2)word(0x34)=0x87654321;
    return result;
}
#ifndef FABLE_COMPOSE_BANK_SYMBOLS
#pragma comment(linker,"/alternatename:?GetEntrySymbol@CBankFile@@QBE?AVCCharString@@K@Z=@LookupSymbolBoundary@16")
#else
CCharString::CCharString(const char*,long){puts("unexpected empty symbol table");exit(26);}
#endif
CVertexShader::CVertexShader(const CArrayVector<unsigned long>& code,const D3DVERTEXELEMENT9* decl,const CCharString& name,const CCharString& layout) {
    require(this==(void*)(arena+0xa20) && (&code==(void*)(arena+0x608)||&code==(void*)(arena+0x648)));
    event(5,(unsigned)&code,(unsigned)decl);event(6,*(const unsigned*)&name,*(const unsigned*)&layout);
    Data=(CVertexShaderData*)word(0xc00);
    if(mode==2)word(8)=(unsigned)(arena+0x100);
}
CCharString CShaderBase::GetDebugName() const { printf("debugname\n");exit(23);return CCharString(); }
unsigned long CVertexShader::CountReferences(){printf("countrefs\n");exit(24);return 0;}
CVertexShaderData::~CVertexShaderData(){printf("data dtor\n");exit(25);}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
extern "C" unsigned LookupVertexVtable[];
#pragma comment(linker,"/alternatename:_LookupVertexVtable=??_7CVertexShader@@6B@")
struct CVertexShaderTestAccess {
    static void capture(CVertexShader& shader) {
        memcpy(arena+0xa40,&shader,8);
        require(*(unsigned*)&shader==(unsigned)LookupVertexVtable && (unsigned)shader.Data==word(0xc00));
        shader.Data=0;
    }
};
int main(int argc,char** argv) {
    if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
    arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
    unsigned params[3],cases=0;
    while(fread(params,4,3,input)==3) {
        if(fread(arena,1,0x1000,input)!=0x1000)return 5;
        indexValue=params[0];mode=params[1];
        CShaderResource* resource=(CShaderResource*)arena;
        if(params[2]) {
            CVertexShader result=resource->GetVertexShader(*(CCharString*)(arena+0x700));
            CVertexShaderTestAccess::capture(result);event(8);
        } else {
            CVertexShader result=resource->GetVertexShader(indexValue);
            CVertexShaderTestAccess::capture(result);event(8);
        }
        ++cases;
    }
    fclose(input);fclose(output);printf("NATIVE_SHADER_LOOKUP_PASS cases=%u\n",cases);return 0;
}
