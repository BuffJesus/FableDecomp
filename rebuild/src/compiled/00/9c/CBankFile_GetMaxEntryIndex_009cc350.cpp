#include "fable_game_text_bank.h"

long CBankFile::GetMaxEntryIndex() const
{
    long index = static_cast<long>(Size - 1);
    while (index >= 0)
    {
        if (index != 0 && static_cast<unsigned long>(index) < Size)
        {
            const bool valid = PackedValid.Size
                ? PackedValid[static_cast<unsigned long>(index) - 1] != 0
                : RuntimeData[index].Valid;
            if (valid)
                return index;
        }
        --index;
    }
    return index;
}
