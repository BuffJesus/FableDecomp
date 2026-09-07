#include <cstdlib>
void __stdcall Array_Free_Elements(void** begin,void** end){for(;begin<end;++begin){if(*begin!=0)std::free(*begin);}}