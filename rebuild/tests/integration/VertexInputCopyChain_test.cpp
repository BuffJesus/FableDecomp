
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_shader_resource.h"
struct CShaderResourceTestAccess { static CVertexShaderInput& Input(CShaderResource& r){return r.VertexShaderInput;} static D3DVERTEXELEMENT9* Declaration(CShaderResource& r){return r.VertexShaderDeclaration;} };
typedef CVertexShaderInput::CStreamInfo Stream;
typedef CVertexStreamFormat::CDataElement Element;
typedef _STL::vector<Stream> Streams;
typedef _STL::vector<Element> Elements;
extern "C" void* FormatVtable[];
#pragma comment(linker,"/alternatename:_FormatVtable=??_7CVertexStreamFormat@@6B@")
extern "C" void NeverCalledFormatDelete(){abort();}
#pragma comment(linker,"/alternatename:??_ECVertexStreamFormat@@UAEPAXI@Z=_NeverCalledFormatDelete")
static unsigned failed,copies,assigns,destroys;
static void check(bool v){if(!v)++failed;}
CBaseClass::~CBaseClass(){++destroys;}
static void initStream(Stream& s,unsigned id,unsigned n){
 s.StreamIndex=id;*reinterpret_cast<unsigned long*>(&s.StreamFormat)=0x12345678;
 Elements& e=s.StreamFormat.DataElements;e._M_start=n?static_cast<Element*>(malloc(n*8)):0;e._M_finish=e._M_start+n;e._M_end_of_storage._M_data=e._M_finish;
 for(unsigned i=0;i<n;++i){e[i].Register=id*10+i;e[i].DataType=static_cast<EVertexStreamDataType>((id+i)%14);}
}
static void dispose(Streams& v){for(Stream* s=v._M_start;s!=v._M_finish;++s)free(s->StreamFormat.DataElements._M_start);free(v._M_start);}
int main(){unsigned cases=0;
 for(unsigned self=0;self<2;++self)for(unsigned used=0;used<4;++used)for(unsigned cap=used;cap<6;++cap)for(unsigned n=0;n<5;++n)for(unsigned nested=0;nested<3;++nested)for(unsigned declMode=0;declMode<4;++declMode){
  unsigned long ownerWords[15],inputWords[7];memset(ownerWords,0xa5,sizeof(ownerWords));memset(inputWords,0x66,sizeof(inputWords));CShaderResource& owner=*reinterpret_cast<CShaderResource*>(ownerWords);CVertexShaderInput& output=CShaderResourceTestAccess::Input(owner);CVertexShaderInput& input=*reinterpret_cast<CVertexShaderInput*>(inputWords);Streams& dest=output.DataStreams;Streams& source=input.DataStreams;
  const unsigned declSizes[4]={0,0,2,2},declCaps[4]={0,3,2,4},sourceSizes[4]={0,1,3,2};unsigned declUsed=declSizes[declMode],declCap=declCaps[declMode],declCount=sourceSizes[declMode];
  output.D3DDeclaration._M_start=declCap?static_cast<D3DVERTEXELEMENT9*>(malloc(declCap*8)):0;output.D3DDeclaration._M_finish=output.D3DDeclaration._M_start+declUsed;output.D3DDeclaration._M_end_of_storage._M_data=output.D3DDeclaration._M_start+declCap;
  input.D3DDeclaration._M_start=declCount?static_cast<D3DVERTEXELEMENT9*>(malloc(declCount*8)):0;input.D3DDeclaration._M_finish=input.D3DDeclaration._M_start+declCount;input.D3DDeclaration._M_end_of_storage._M_data=input.D3DDeclaration._M_finish;
  if(declCap)memset(output.D3DDeclaration._M_start,0x33,declCap*8);if(declCount)memset(input.D3DDeclaration._M_start,0x55,declCount*8);
  dest._M_start=cap?static_cast<Stream*>(malloc(cap*20)):0;dest._M_finish=dest._M_start+used;dest._M_end_of_storage._M_data=dest._M_start+cap;
  source._M_start=n?static_cast<Stream*>(malloc(n*20)):0;source._M_finish=source._M_start+n;source._M_end_of_storage._M_data=source._M_finish;
  for(unsigned i=0;i<used;++i)initStream(dest[i],100+i,(i+nested)%3);
  for(i=0;i<n;++i)initStream(source[i],200+i,(i+nested)%3);
  Stream* old=dest._M_start;copies=assigns=destroys=0;owner.SetVertexShaderInput(self?output:input);
  unsigned finalSize=self?used:n;check(dest.size()==finalSize && dest.capacity()==(self||n<=cap?cap:n));
  check(self?dest._M_start==old:((dest._M_start!=old)==(n>cap)));
  check(destroys==(self?0:n>cap?used:used>n?used-n:0));
  for(i=0;i<finalSize;++i){unsigned id=(self?100:200)+i;check(dest[i].StreamIndex==id);Elements& e=dest[i].StreamFormat.DataElements;unsigned nn=(i+nested)%3;check(e.size()==nn);
   for(unsigned j=0;j<nn;++j)check(e[j].Register==id*10+j && e[j].DataType==static_cast<EVertexStreamDataType>((id+j)%14));
   void* expectedVtable=!self && (n>cap || i>=used)?FormatVtable:reinterpret_cast<void*>(0x12345678);check(*reinterpret_cast<void**>(&dest[i].StreamFormat)==expectedVtable);
   if(!self && nn)check(e._M_start!=source[i].StreamFormat.DataElements._M_start);
  }
  for(i=0;i<n;++i)check(source[i].StreamIndex==200+i);
  unsigned expectedDecl=self?declUsed:declCount;check(output.D3DDeclaration.size()==expectedDecl);
  check(CShaderResourceTestAccess::Declaration(owner)==(expectedDecl?output.D3DDeclaration._M_start:reinterpret_cast<D3DVERTEXELEMENT9*>(&output.D3DDeclaration)));
  for(i=0;i<expectedDecl*8;++i)check(reinterpret_cast<unsigned char*>(output.D3DDeclaration._M_start)[i]==(self?0x33:0x55));
  for(i=0;i<7;++i)check(ownerWords[i]==0xa5a5a5a5);check(ownerWords[14]==0xa5a5a5a5);
  dispose(dest);dispose(source);free(output.D3DDeclaration._M_start);free(input.D3DDeclaration._M_start);++cases;
 }
 if(failed){printf("FAIL %u\n",failed);return 1;}printf("VERTEX_INPUT_COPY_CHAIN_BEHAVIOR_PASS scenarios=%u\n",cases);return 0;
}
