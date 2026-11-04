// Retail009D0630: once, list the valid non-empty entries and sort them by data
// offset. Retail expands sort here but keeps the final insertion pass out of line
// (009D3ED0, STLBlockOrder_FinalInsertionSort_009d3ed0.cpp): VC7.1 inlines the
// shared body into this unit, so it is declared as a specialization and resolved
// by that unit's instantiation of the same symbol. This unit also emits
// __introsort_loop 009D48C0, __median 009D2E40 and CompareDataOffset 009CC640.
#include <new>
#include "fable_game_text_bank.h"
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_algo_impl.h"
#include "fable_stlport_introsort.h"
namespace _STL {
    // Landed in UlongVector_Overflow_00464aab.cpp (00464ACE).
    template<> void vector<unsigned long>::_M_insert_overflow(unsigned long*, const unsigned long&, const __true_type&, size_t, bool);
    template<> void FABLE_FASTCALL __final_insertion_sort<unsigned long*, CBankFile::CompareDataOffset>(unsigned long*, unsigned long*, CBankFile::CompareDataOffset);
}
void CBankFile::InitBlockOrder()
{
    if (BlockOrder.empty()) {
        for (unsigned long i = 0; i < Size; ++i) {
            const CRuntimeData& data = RuntimeData[i];
            if (data.Valid && data.DataSize > 0) BlockOrder.push_back(i);
        }
        unsigned long* last = BlockOrder.end();
        unsigned long* first = BlockOrder.begin();
        if (first != last) {
            _STL::__introsort_loop(first, last, (unsigned long*)0, _STL::__lg(last - first) * 2, CompareDataOffset(this));
            _STL::__final_insertion_sort(first, last, CompareDataOffset(this));
        }
    }
}
