#pragma once
#include "fable_stlport_vector.h"
// Retail STLport list: one allocator-proxy pointer, circular sentinel and no
// size member. CConsole ctor 0x009ECD80 allocates a 24-byte command-line node.
namespace _STL {
    struct _List_node_base {
        _List_node_base* _M_next; _List_node_base* _M_prev;
        void _M_unhook(){
            _List_node_base* next=_M_next;_List_node_base* prev=_M_prev;
            prev->_M_next=next;next->_M_prev=prev;
        }
    };
    template<class T> struct _List_node : _List_node_base { T _M_data; };
    struct _List_iterator_base {
        _List_node_base* _M_node;
        void _M_incr(){_M_node=_M_node->_M_next;}
        void _M_decr(){_M_node=_M_node->_M_prev;}
    };
    template<class T> struct _List_iterator : _List_iterator_base {
        _List_iterator() {}
        _List_iterator(_List_node_base* p) { _M_node=p; }
        _List_iterator(const _List_iterator& other) { _M_node=other._M_node; }
        T& operator*() const { return static_cast<_List_node<T>*>(_M_node)->_M_data; }
        T* operator->() const { return &operator*(); }
        _List_iterator& operator++(){_M_incr();return *this;}
        _List_iterator& operator--(){_M_decr();return *this;}
    };
    template<class T> inline bool operator==(const _List_iterator<T>& a,const _List_iterator<T>& b){return a._M_node==b._M_node;}
    template<class T> inline bool operator!=(const _List_iterator<T>& a,const _List_iterator<T>& b){return a._M_node!=b._M_node;}
    template<class I> inline ptrdiff_t FABLE_FASTCALL __list_distance(I first,I last,const input_iterator_tag&) {
        ptrdiff_t n=0;while(first!=last){++first;++n;}return n;
    }
    template<class T,class D> inline void FABLE_FASTCALL distance(_List_iterator<T> first,_List_iterator<T> last,D& n) {
        while(first!=last){++first;++n;}
    }
    template<class T> inline ptrdiff_t FABLE_FASTCALL distance(_List_iterator<T> first,_List_iterator<T> last) {
        return __list_distance(first,last,input_iterator_tag());
    }
    // STLport advance for bidirectional iterators, as retail's console expands
    // it (ScrollToHistoryBeginning 0x009EAC80, ScrollToHistoryEnd 0x009EAD10).
    template<class T,class D> inline void FABLE_FASTCALL advance(_List_iterator<T>& it,D n){
        if(n>0){while(n--)++it;}else{while(n++)--it;}
    }
    template<class T, class A = allocator<T> > class list {
    public:
        typedef _List_node<T> _Node;
        typedef _List_iterator<T> iterator;
        list() : _M_node(0) {
            _Node* p = static_cast<_Node*>(malloc(sizeof(_Node)));
            p->_M_next = p; p->_M_prev = p; _M_node = p;
        }
        explicit list(const A& allocator);
        ~list();
        void clear();
        bool empty() const { return _M_node->_M_next == _M_node; }
        template<class P> void remove_if(P predicate);
        void remove(const T& value);
        iterator erase(iterator position);
        size_t size() const {iterator last=end();iterator first=begin();size_t n=0;_STL::distance(first,last,n);return n;}
        iterator insert(iterator position,const T& value);
        iterator insert(iterator position);
        T& front() { return static_cast<_Node*>(_M_node->_M_next)->_M_data; }
        void pop_front() {
            _Node* p=static_cast<_Node*>(_M_node->_M_next);
            p->_M_unhook();
            _Destroy(&p->_M_data);free(p);
        }
        T& back() { return static_cast<_Node*>(_M_node->_M_prev)->_M_data; }
        const T& back() const { return static_cast<const _Node*>(_M_node->_M_prev)->_M_data; }
        void push_back(const T& value) { insert(end(),value); }
        void push_front(const T& value) { insert(begin(),value); }
        void pop_back() {
            _Node* p=static_cast<_Node*>(_M_node->_M_prev);
            p->_M_unhook();
            _Destroy(&p->_M_data);free(p);
        }
        iterator begin() const { return iterator(_M_node->_M_next); }
        iterator end() const { return iterator(_M_node); }
        _Node* _M_node;
    protected:
        _Node* _M_create_node(const T& value);
    };
}
