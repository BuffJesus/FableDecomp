// Native bank symbol-index map; recovered STLport unsigned-long instantiation.
#pragma optimize("s",on)
#include "fable_stlport_tree.h"
#include "fable_string.h"

namespace _STL
{
    template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
    typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator __position, const _Value& __v)
    {
        if (__position._M_node == this->_M_header._M_data->_M_left) // begin()
        {
            if (size() <= 0)
                return insert_unique(__v).first;

            if (_M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
                return _M_insert(__position._M_node, __position._M_node, __v);
            else
            {
                bool __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));

                if (__comp_pos_v == false) // Neither is less: they are equal.
                    return __position;

                iterator __after = __position;
                ++__after;

                if (__after._M_node == this->_M_header._M_data)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);

                if (_M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node)))
                {
                    if (__position._M_node->_M_right == 0)
                        return _M_insert(0, __position._M_node, __v, __position._M_node);
                    else
                        return _M_insert(__after._M_node, __after._M_node, __v);
                }
                else
                    return insert_unique(__v).first;
            }
        }
        else if (__position._M_node == this->_M_header._M_data) // end()
        {
            if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
                return _M_insert(0, _M_rightmost(), __v, __position._M_node);
            else
                return insert_unique(__v).first;
        }
        else
        {
            iterator __before = __position;
            --__before;

            bool __comp_v_pos = _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node));

            if (__comp_v_pos && _M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v)))
            {
                if (__before._M_node->_M_right == 0)
                    return _M_insert(0, __before._M_node, __v, __before._M_node);
                else
                    return _M_insert(__position._M_node, __position._M_node, __v);
            }
            else
            {
                iterator __after = __position;
                ++__after;

                bool __comp_pos_v = !__comp_v_pos;
                if (!__comp_v_pos)
                    __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));

                if ((!__comp_v_pos) && __comp_pos_v &&
                    (__after._M_node == this->_M_header._M_data ||
                     _M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node))))
                {
                    if (__position._M_node->_M_right == 0)
                        return _M_insert(0, __position._M_node, __v, __position._M_node);
                    else
                        return _M_insert(__after._M_node, __after._M_node, __v);
                }
                else
                {
                    if (__comp_v_pos == __comp_pos_v)
                        return __position;
                    else
                        return insert_unique(__v).first;
                }
            }
        }
    }

    template _Rb_tree<CCharString, pair<const CCharString, unsigned long>, _Select1st<pair<const CCharString, unsigned long> >,
                      less<CCharString>, allocator<pair<const CCharString, unsigned long> > >::iterator
    _Rb_tree<CCharString, pair<const CCharString, unsigned long>, _Select1st<pair<const CCharString, unsigned long> >,
             less<CCharString>, allocator<pair<const CCharString, unsigned long> > >::insert_unique(iterator __position,
                                                                                         const pair<const CCharString, unsigned long>& __v);
}
