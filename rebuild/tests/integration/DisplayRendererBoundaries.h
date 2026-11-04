#pragma once
// Recorded nested lifetimes for the display-only fixtures. The native renderer
// lifetime is exercised separately by NativeRendererDtor_test.cpp.
#pragma comment(linker,"/alternatename:??0CShaderDataBank@@QAE@XZ=@DisplayBankCtor@4")
#pragma comment(linker,"/alternatename:??1CShaderDataBank@@UAE@XZ=@DisplayBankDtor@4")
#pragma comment(linker,"/alternatename:??0CVertexShader@@QAE@XZ=@DisplayVertexCtor@4")
#pragma comment(linker,"/alternatename:?GetDebugName@CShaderBase@@MBE?AVCCharString@@XZ=_UnusedDisplayRendererBoundary")
#pragma comment(linker,"/alternatename:?CountReferences@CVertexShader@@EAEKXZ=_UnusedDisplayRendererBoundary")
extern "C" void* __fastcall DisplayBankCtor(void* self){memset(self,0,0x128);return self;}
extern "C" void __fastcall DisplayBankDtor(void*){}
extern "C" void* __fastcall DisplayVertexCtor(void* self){memset(self,0,8);return self;}
extern "C" void UnusedDisplayRendererBoundary(){abort();}
