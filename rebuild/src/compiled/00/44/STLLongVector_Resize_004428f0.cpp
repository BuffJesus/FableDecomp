#include <new>
#define FABLE_STLP_FILL_CALL __fastcall
#include "fable_game_text_bank.h"
#include "fable_stlport_algobase.h"
namespace _STL {
template<> __declspec(noinline) void __fastcall fill<long*,long>(long* first,long* last,const long& value){for(;first!=last;++first)*first=value;}
}
#include "fable_stlport_vector_impl.h"
namespace _STL { template void vector<long>::resize(size_t,const long&); }
