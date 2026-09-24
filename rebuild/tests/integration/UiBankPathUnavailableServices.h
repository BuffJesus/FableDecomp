// Connected opening/startup fixtures contain registered paths. Allocating-wide
// branches and missing-name throws are exercised by independent path/string gates.
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
const char FableUiEmptyString[]={0};
const unsigned char FableUiGenericExceptionThrowInfo[]={0};
void* __cdecl FableUiAllocateWideBuffer(unsigned) { abort(); return 0; }
void __cdecl FableUiFreeWideBuffer(void*) { abort(); }
__declspec(noreturn) void __fastcall FableUiWideLengthError(CWideStringData*,void*) { abort(); }
__declspec(noreturn) void __stdcall FableUiThrowException(void*,const void*) { abort(); }
