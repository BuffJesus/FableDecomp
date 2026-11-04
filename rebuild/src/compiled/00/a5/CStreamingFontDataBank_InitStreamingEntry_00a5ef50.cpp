#include "fable_font_data_banks.h"
template<class T> CCountedPointer<T>& CCountedPointer<T>::operator=(const CCountedPointer<T>& other) {
 T* data=other.Data;CCPPointerInfo* info=other.Info;
 if(Info!=info){Release();Data=data;Info=info;if(info)++info->RefCount;}
 return *this;
}
void CStreamingFontDataBank::InitStreamingEntry(CSymbolMap&) {
 unsigned long index=FindIndexBySymbol("GlyphData");
 streamingEntry=GetAsyncEntry(index);
}
