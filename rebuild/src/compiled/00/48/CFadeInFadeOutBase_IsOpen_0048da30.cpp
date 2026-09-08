// Member names and offsets are identical in retail and Ego_r.pdb.
struct CFadeInFadeOutBase {
    bool Active;
    unsigned char _pad_0x01[0x0b];
    bool Closing;
    bool Opening;

    bool IsOpen() const;
};

bool CFadeInFadeOutBase::IsOpen() const {
    return Active && !Opening && !Closing;
}