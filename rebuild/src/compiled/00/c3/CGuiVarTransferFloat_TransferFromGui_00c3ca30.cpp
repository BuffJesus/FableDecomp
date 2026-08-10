struct Node {
    int f0;
    int f4;
    Node* f8;
};
struct CGuiVarTransferFloat {
    int f0;
    int f4;
    Node* f8;
    int fC;
};

// thiscall target at 0xc4cef0, ecx = node
void __fastcall NodeReturn(Node* n);

void __fastcall TransferFromGui(CGuiVarTransferFloat* self)
{
    Node* n = self->f8;
    self->f8 = n->f8;
    NodeReturn(n);
    self->fC--;
}