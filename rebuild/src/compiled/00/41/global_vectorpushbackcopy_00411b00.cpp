#pragma optimize("s",on)
struct CActionInputControl;

struct VectorPushBackCopyOverlay
{
    unsigned char unused[4];
    CActionInputControl* end;
};

CActionInputControl* __fastcall Vector_CopyImpl(
    CActionInputControl* first,
    CActionInputControl* last,
    CActionInputControl* destination,
    CActionInputControl** destination_copy,
    void* tag);

CActionInputControl* __fastcall vector_push_back_copy(
    VectorPushBackCopyOverlay* self,
    void*,
    CActionInputControl* destination,
    CActionInputControl* first)
{
    register CActionInputControl* result = destination;
    CActionInputControl* new_end = Vector_CopyImpl(
        first, self->end, destination, &destination, 0);
    self->end = new_end;
    return result;
}