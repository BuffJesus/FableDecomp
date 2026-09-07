#include <cstdlib>
void __stdcall Vector_FreeElementPointers(void** begin,void** end){for(;begin<end;++begin){if(*begin!=0)std::free(*begin);}}