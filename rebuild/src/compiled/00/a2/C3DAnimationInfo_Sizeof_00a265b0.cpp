// Ego_r.pdb names the counted animation payload `Data` at +0x0c.
// A present payload contributes its own size plus the full 0x18-byte retail
// record. A null counted pointer still serializes its four-byte slot followed
// by 0x14 bytes of fixed state. The donor adds later movement/rotation fields.
struct CAnimationData {
    unsigned long Sizeof() const;
};

static unsigned long SizeofCountedAnimationData(const CAnimationData* data) {
    if (data != 0) {
        return data->Sizeof() + sizeof(data);
    }
    return sizeof(data);
}

struct C3DAnimationInfoRetail {
    unsigned char _base_0x00[0x0c];
    CAnimationData* Data;
    float Duration;
    float NonLoopingDuration;

    unsigned long Sizeof() const;
};

unsigned long C3DAnimationInfoRetail::Sizeof() const {
    return SizeofCountedAnimationData(Data) + 0x14;
}