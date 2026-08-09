struct Inner {
    char pad[0x5c];
    unsigned char flag;
};
struct CLandscapeBackgroundTreeNode {
    char pad[0xa4];
    Inner* inner;
};
bool __fastcall CLandscapeBackgroundTreeNode_CanRelocateData(CLandscapeBackgroundTreeNode* self, int /*edx*/, long a)
{
    (void)a;
    return self->inner->flag == 0;
}