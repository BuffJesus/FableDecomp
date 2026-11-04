// Retail009D2AF0: the vector constructor used by ReadEntryList's CArray.
// Zero count does not allocate; each eight-byte type/count record is zeroed.
#include <new>
#include "fable_bank_file_data_type_count.h"
#include "fable_carray.h"
#include "fable_stlport_uninitialized.h"
#include "fable_stlport_vector_impl.h"
template _STL::vector<CBankFileDataTypeCount>::vector(size_t);
