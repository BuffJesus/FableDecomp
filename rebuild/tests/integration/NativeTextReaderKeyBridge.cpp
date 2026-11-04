#include "fable_threaded_file_object.h"
extern "C" unsigned long ObservedReaderKey(const CThreadedFile* file){return file->physicalSortKey_;}
