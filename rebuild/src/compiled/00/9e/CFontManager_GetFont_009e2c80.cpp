#include "fable_system_manager.h"
#include "fable_symbol_map.h"
CCountedPointer<CFontBank> CFontManager::GetFont(const CCharString& name) {
 if(PSymbols->SymbolExists(name))
  return GetStaticFont(PSymbols->GetSymbolSLONG(name));
 if(PStreamingSymbols->SymbolExists(name))
  return GetStreamingFont(PStreamingSymbols->GetSymbolSLONG(name));
 return GetStaticFont(PSymbols->GetSymbolSLONG(name));
}
