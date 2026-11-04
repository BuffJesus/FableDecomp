"""Bank-specific partial-erase defects; other container instantiations stay intact."""
from pathlib import Path
R=Path(__file__).resolve().parents[2]
def controls(sources):
 index=next(i for i,p in enumerate(sources) if p.name=='STLAsyncPointerTree_RangeErase_009d7760.cpp')
 text=sources[index].read_text();header=(R/'rebuild/include/fable_stlport_tree_erase.h').read_text().replace('#pragma once','')
 marker='#include "fable_stlport_tree_erase.h"';assert text.count(marker)==1
 result=[]
 for name,old,new in (
  ('bank-skip-partial-erase','while (first != last)\n            erase(first++);','while (first != last)\n            ++first;'),
  ('bank-clear-all-owners','if (first == begin() && last == end())','if (first != last)'),
  ('bank-erase-other-owner','erase(first++);','if(first._M_node->_M_right) erase(iterator(first._M_node->_M_right)); else erase(first++);'),
 ):
  assert text.count(old)==1;result.append((name,index,text.replace(old,new)))
 for name,old,new in (
  ('bank-partial-no-free','if(node)free(node);',';'),
  ('bank-partial-wrong-count','--_M_node_count;',';'),
  ('bank-partial-wrong-extreme','--_M_node_count;','--_M_node_count;this->_M_header._M_data->_M_left=this->_M_header._M_data;'),
 ):
  assert header.count(old)==1;result.append((name,index,text.replace(marker,header.replace(old,new))))
 start=header.index('static_cast<_Rb_tree_node<V>*>(');end=header.index(';',start)
 detached=header[:start]+'static_cast<_Rb_tree_node<V>*>(position._M_node)'+header[end:]
 result.append(('bank-partial-not-detached',index,text.replace(marker,detached)))
 assert len(result)==7;return result
