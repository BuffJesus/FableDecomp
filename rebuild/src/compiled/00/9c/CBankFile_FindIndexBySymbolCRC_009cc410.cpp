#include "fable_game_text_bank.h"
unsigned long CBankFile::FindIndexBySymbolCRC(unsigned long crc) const
{
    typedef CVectorMap<unsigned long,unsigned long,CKeyPairCompareLess<unsigned long,unsigned long> > Map;
    const Map* storage = &SymbolCRCIndices;
    const _STL::pair<unsigned long,unsigned long>* position = storage->LowerBound(crc);
    const _STL::pair<unsigned long,unsigned long>* end = storage->end();
    if (position == end || crc < position->first) position = end;
    if (position != SymbolCRCIndices.end()) return position->second;
    return 0;
}
