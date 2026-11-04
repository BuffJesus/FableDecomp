// lib_console2.cpp, retail 0x009ECF30 (Ego_r ??1CConsole@@UAE@XZ): empty the
// binding maps, drop every registered input, then clear the input map and the
// history before the members' own destructors run.
#include "fable_console.h"

namespace _STL
{
    // STLport's _List_base clear and destructor, expanded here.
    template<> inline void list<CConsoleCommandLine>::clear()
    {
        _Node* cur = static_cast<_Node*>(_M_node->_M_next);
        while (cur != _M_node)
        {
            _Node* tmp = cur;
            cur = static_cast<_Node*>(cur->_M_next);
            _Destroy(&tmp->_M_data);
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    template<> inline list<CConsoleCommandLine>::~list()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
}


namespace _STL
{
    // The console's maps own their nodes (STLport ~_Rb_tree), expanded here.
    template<> inline map<char, CCharString>::~map()
    {
        _M_t.clear();
        if (_M_t._M_header._M_data != 0)
            _M_t._M_header.deallocate(_M_t._M_header._M_data, 1);
    }
    template<> inline map<CCharString, CCharString>::~map()
    {
        _M_t.clear();
        if (_M_t._M_header._M_data != 0)
            _M_t._M_header.deallocate(_M_t._M_header._M_data, 1);
    }
    template<> inline map<CCharString, CCountedPointer<CConsoleInputBase> >::~map()
    {
        _M_t.clear();
        if (_M_t._M_header._M_data != 0)
            _M_t._M_header.deallocate(_M_t._M_header._M_data, 1);
    }
}

template<class T> void CCountedPointer<T>::Reset(T* data)
{
    Release();
    Info = 0;
    Data = data;
    if (Data)
        Info = new CCPPointerInfo(&DeleteData, (void*)Data);
}

CConsole::~CConsole()
{
    KeyBindingMap.clear();
    StringBindingMap.clear();
    for (_STL::map<CCharString, CCountedPointer<CConsoleInputBase> >::iterator it = RegisteredConsoleInputs.begin();
         it != RegisteredConsoleInputs.end(); ++it)
        it->second.Reset(0);
    RegisteredConsoleInputs.clear();
    CommandLineHistory.clear();
}
