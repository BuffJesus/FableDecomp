#include <stdio.h>
#include "fable_retail_banks.h"

// A polymorphic allocation with the same first vtable slot as CRetailBank.
// Its destructor records the virtual deleting-destructor dispatch.
class ProbeBank
{
public:
    virtual ~ProbeBank() { ++deaths; }
    static int deaths;
};
int ProbeBank::deaths = 0;

int main()
{
    CCountedPointer<NBankFileManager::CRetailBank>::DeleteData(0);
    if (ProbeBank::deaths != 0) return 1;
    ProbeBank* first = new ProbeBank;
    CCountedPointer<NBankFileManager::CRetailBank>::DeleteData(first);
    if (ProbeBank::deaths != 1) return 2;
    ProbeBank* second = new ProbeBank;
    CCountedPointer<NBankFileManager::CRetailBank>::DeleteData(second);
    if (ProbeBank::deaths != 2) return 3;
    puts("RETAIL_BANK_DELETE_DATA_PASS");
    return 0;
}
