#include <cstdlib>
void __stdcall FreeBufferArray(void** begin,void** end){for(;begin<end;++begin){if(*begin!=0)std::free(*begin);}}