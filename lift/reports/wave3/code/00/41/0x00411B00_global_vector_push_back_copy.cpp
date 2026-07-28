struct CActionInputControl;

struct VectorPushBackCopyOverlay {
    std::byte pad_00[0x4];
    CActionInputControl* end;
};

static_assert(offsetof(VectorPushBackCopyOverlay, end) == 0x4);

namespace std {
    template <class InIt, class OutIt>
    OutIt _Copy_impl(InIt _First, InIt _Last, OutIt _Dest);
}

CActionInputControl* __thiscall
vector_push_back_copy(
    VectorPushBackCopyOverlay* self,
    CActionInputControl* param_2,
    CActionInputControl* param_3)
{
    CActionInputControl* const new_end =
        std::_Copy_impl<CActionInputControl*, CActionInputControl*>(param_3, self->end, param_2);

    self->end = new_end;
    return param_2;
}