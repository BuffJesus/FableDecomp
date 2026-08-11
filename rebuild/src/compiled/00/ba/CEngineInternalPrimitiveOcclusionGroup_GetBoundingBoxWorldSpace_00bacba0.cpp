// CEngineInternalPrimitiveOcclusionGroup::GetBoundingBoxWorldSpace @ 00bacba0
struct C3DBoundingBox { float mn[3]; float mx[3]; };

// helper @ 0xb92d10 : native thiscall member
// bool COcclusionInner::occ(C3DBoundingBox& bbox, int f58, int f5c)
struct COcclusionInner {
    bool occ(C3DBoundingBox& bbox, int f58, int f5c);
};

struct CEngineInternalPrimitiveOcclusionGroup {
    char pad[0x54];
    COcclusionInner* f54; // +0x54  -> callee ecx
    int f58;              // +0x58
    int f5c;              // +0x5c
};

bool __fastcall CEngineInternalPrimitiveOcclusionGroup_GetBoundingBoxWorldSpace(
        CEngineInternalPrimitiveOcclusionGroup* self, int /*edx*/, C3DBoundingBox& bbox)
{
    return self->f54->occ(bbox, self->f58, self->f5c);
}