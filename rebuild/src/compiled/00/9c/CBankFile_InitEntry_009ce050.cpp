#include <new>
#include "fable_game_text_bank.h"
#include "fable_stlport_vector_impl.h"
void CBankFile::InitEntry(unsigned long index,const CCharString& symbol,const CCharString& filename) {
if(OpenFlags&64)SymbolIndices[symbol]=index;
if(OpenFlags&256)SymbolCRCIndices.Insert(symbol.GetCRC(),index);
if((OpenFlags&128)&&!filename.IsEmpty()) {
CCharString lowercase_filename=ConvertPathToFilename(filename).GetAsLowerCase();
FilenameIndices.insert(_STL::pair<CCharString,unsigned long>(lowercase_filename,index));
}
}
