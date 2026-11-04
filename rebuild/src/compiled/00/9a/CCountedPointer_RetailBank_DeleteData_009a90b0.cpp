#include "fable_retail_banks.h"

void FABLE_FASTCALL
CCountedPointer<NBankFileManager::CRetailBank>::DeleteData(void* data)
{
    delete static_cast<NBankFileManager::CRetailBank*>(data);
}
