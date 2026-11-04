#pragma once
// These display-only fixtures own zeroed shader-manager storage. Real manager
// construction/destruction is a separately tested native dependency.
#pragma comment(linker,"/alternatename:??0CShaderRenderManager@@QAE@XZ=@DisplayShaderManagerCtor@4")
#pragma comment(linker,"/alternatename:??1CShaderRenderManager@@UAE@XZ=@DisplayShaderManagerDtor@4")
extern "C" void* __fastcall DisplayShaderManagerCtor(void* self){return self;}
extern "C" void __fastcall DisplayShaderManagerDtor(void*){}
