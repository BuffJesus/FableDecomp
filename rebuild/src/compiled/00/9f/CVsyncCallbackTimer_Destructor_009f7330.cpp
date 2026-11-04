#include "fable_display_viewport.h"
namespace _STL {
 template<> __forceinline list<CVsyncCallbackTimer::CPendingQuery>::~list() {
  clear(); _Node* node=_M_node; if(node) free(node);
 }
}
CVsyncCallbackTimer::~CVsyncCallbackTimer(){ClearAllQueries();}
