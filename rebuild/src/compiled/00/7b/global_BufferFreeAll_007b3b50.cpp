#include <cstdlib>
void __stdcall Buffer_FreeAll(void** begin,void** end){for(;begin<end;++begin){if(*begin!=0)std::free(*begin);}}