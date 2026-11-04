#include <new>
#define FABLE_STLP_FILL_CALL __fastcall
#include "fable_game_text_bank.h"
#include "fable_stlport_char_string_vector.h"
#include "fable_stlport_algobase.h"
namespace _STL {
inline NGameText::CTextTag* __fastcall __uninitialized_fill_n(NGameText::CTextTag* first,size_t count,const NGameText::CTextTag& value)
{
 for(;count>0;--count,++first) _Construct(first,value);
 return first;
}
template<> NGameText::CTextTag* vector<NGameText::CTextTag>::erase(NGameText::CTextTag*,NGameText::CTextTag*);
template<> void vector<NGameText::CTextTag>::_M_fill_insert(NGameText::CTextTag*,size_t,const NGameText::CTextTag&);
}
#include "fable_stlport_vector_impl.h"
namespace _STL {
template<> NGameText::CTextTag* vector<NGameText::CTextTag>::erase(NGameText::CTextTag* first,NGameText::CTextTag* last)
{
 NGameText::CTextTag* finish=copy(last,_M_finish,first);_Destroy(finish,_M_finish);_M_finish=finish;return first;
}
template<> void vector<NGameText::CTextTag>::_M_fill_insert(NGameText::CTextTag* position,size_t count,const NGameText::CTextTag& value)
{
 if(count){
  if(size_t(this->_M_end_of_storage._M_data-this->_M_finish)>=count){
   NGameText::CTextTag copyValue=value;size_t after=this->_M_finish-position;NGameText::CTextTag* oldFinish=this->_M_finish;
   if(after>count){__uninitialized_copy(this->_M_finish-count,this->_M_finish,this->_M_finish,__false_type());this->_M_finish+=count;copy_backward(position,oldFinish-count,oldFinish);fill(position,position+count,copyValue);}
   else{__uninitialized_fill_n(this->_M_finish,count-after,copyValue);this->_M_finish+=count-after;__uninitialized_copy(position,oldFinish,this->_M_finish,__false_type());this->_M_finish+=after;fill(position,oldFinish,copyValue);}
  }else _M_insert_overflow(position,value,__false_type(),count);
 }
}
template void vector<NGameText::CTextTag>::resize(size_t);
template void vector<NGameText::CTextTag>::resize(size_t,const NGameText::CTextTag&);
}
