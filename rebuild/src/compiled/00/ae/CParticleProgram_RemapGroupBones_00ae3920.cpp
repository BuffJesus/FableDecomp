// CParticleProgram::RemapBoneIndices(map&, CArray<C3DGroup2>&) (private static; retail 0x00AE3920, Ego_r 0x00BF38A0):
// rebuild a group list keeping only the groups whose bone survives the remap, each moved to its new bone index.
#include "fable_particle_program.h"

namespace _STL
{
// STLport's tree search, expanded inline here as in retail.
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
__forceinline typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_Base_ptr
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_find(const _Key& __k)
{
    _Base_ptr __header = this->_M_header._M_data;
    _Base_ptr __y = this->_M_header._M_data;
    _Base_ptr __x = this->_M_header._M_data->_M_parent;
    while (__x != 0)
        if (!_M_key_compare(static_cast<_Rb_tree_node<_Value>*>(__x)->_M_value_field.first, __k))
            __y = __x, __x = __x->_M_left;
        else
            __x = __x->_M_right;
    if (__y == __header || _M_key_compare(__k, static_cast<_Rb_tree_node<_Value>*>(__y)->_M_value_field.first))
        __y = __header;
    return __y;
}
}

void FABLE_FASTCALL CParticleProgram::RemapBoneIndices(_STL::map<unsigned long, unsigned long>& boneRemap, CArray<C3DGroup2>& groups)
{
    CArray<C3DGroup2> remapped;
    remapped.reserve(groups.size());
    for (unsigned long i = 0; i < groups.size(); ++i)
    {
        C3DGroup2& group = groups[i];
        unsigned long bone = group.GetBoneIndex();
        if (boneRemap.find(bone) != boneRemap.end())
        {
            group.SetBoneIndex(boneRemap[bone]);
            remapped.push_back(group);
        }
    }
    groups = remapped;
}
