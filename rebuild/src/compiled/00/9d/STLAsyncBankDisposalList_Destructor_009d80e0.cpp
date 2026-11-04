// Retail 009D80E0 (byte-identical twin at 009D7FA0): the out-of-line STLport
// list<CCountedPointer<CBankFileAsyncData> > destructor. It calls the list's
// clear (retail 009D7DB0, whose only other caller is ~CBankFileAsync 009D6490
// destroying CBankFileAsync::DisposalList) and frees the sentinel node.
#include "fable_game_text_bank.h"
namespace _STL {
template<> __declspec(noinline) list<CCountedPointer<CBankFileAsyncData> >::~list() {
    clear();
    if(_M_node) free(_M_node);
}
}
