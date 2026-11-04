#include "fable_game_text_bank.h"
#include "fable_symbol_map.h"
// These local helpers preserve retail's inline accessor expansions without
// introducing a second public definition of the separately recovered methods.
static inline bool BankSymbolEntryExists(const CBankFile& bank,unsigned long index) {
 if(index==0||index>=bank.Size)return false;
 return bank.PackedValid.Size?bank.PackedValid[index-1]!=0:bank.RuntimeData[index].Valid;
}
static inline unsigned long BankSymbolEntryType(const CBankFile& bank,unsigned long index) {
 return bank.PackedDataType.Size?bank.PackedDataType[index-1]:bank.RuntimeData[index].DataType;
}
static inline CCharString BankEntrySymbol(const CBankFile& bank,unsigned long index) {
 if(bank.Symbols.empty())return CCharString("(Unknown)");
 return CCharString(bank.Symbols[index]);
}
void CBankFile::CreateSymbolMap(CSymbolMap& symbols,bool append) const {
 if(!append)symbols.Clear();
 for(unsigned long index=0;index<Size;++index){
  if(BankSymbolEntryExists(*this,index)){
   CCharString symbol=BankEntrySymbol(*this,index);
   if(symbol.FindCharPos('[')==-1){
    CCharString group=GetEnumerationType(BankSymbolEntryType(*this,index));
    symbols.AddSymbolDirty(group,symbol,index);
   }
  }
 }
 symbols.UpdateDirty();
}
