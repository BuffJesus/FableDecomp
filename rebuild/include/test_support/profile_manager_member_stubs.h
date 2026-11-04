#pragma once
// Empty profiler containers owned by system-manager boundary fixtures only.
#include "fable_system_manager.h"
namespace _STL {
template<> inline list<CProfile*>::~list(){free(_M_node);}
template<> inline list<CProfileManager::CProfileTreeEntry>::~list(){free(_M_node);}
// This boundary only owns empty trees. A nonempty tree requires the recovered
// eraser and must fail visibly instead of silently discarding fixture data.
template<> inline void _Rb_tree<CCharString, pair<const CCharString,long>,
    _Select1st<pair<const CCharString,long> >, less<CCharString>,
    allocator<pair<const CCharString,long> > >::_M_erase(
        _Rb_tree_node<pair<const CCharString,long> >*) { abort(); }
}
CProfileManager::~CProfileManager(){
    free(IDToProfileName._M_t._M_header._M_data);
    // ProfileNameToID's owning map specialization releases its own sentinel.
}
