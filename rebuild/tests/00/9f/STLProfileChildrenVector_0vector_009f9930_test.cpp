#include "fable_system_manager.h"
typedef CProfileManager::CProfileTreeEntry* Child;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>

typedef _STL::vector<Child> Vector;

int main()
{
    Child values[5] = { (Child)3, (Child)1, (Child)4, 0, (Child)5 };
    unsigned char srcStore[sizeof(Vector)], dstStore[sizeof(Vector)];
    Vector* src = reinterpret_cast<Vector*>(srcStore);
    src->_M_start = values;
    src->_M_finish = values + 4;
    src->_M_end_of_storage._M_data = values + 5;
    Vector* copy = new (dstStore) Vector(*src);
    if (copy->_M_start == 0 || copy->_M_start == values || copy->_M_finish != copy->_M_start + 4 ||
        copy->_M_end_of_storage._M_data != copy->_M_start + 4 || memcmp(copy->_M_start, values, 4 * sizeof(long)) != 0)
    { puts("copy"); return 1; }
    free(copy->_M_start);

    src->_M_finish = values;
    memset(dstStore, 0xCC, sizeof(dstStore));
    copy = new (dstStore) Vector(*src);
    if (copy->_M_start != 0 || copy->_M_finish != 0 || copy->_M_end_of_storage._M_data != 0) { puts("empty"); return 1; }
    puts("PROFILE_CHILDREN_COPY_PASS");
    return 0;
}