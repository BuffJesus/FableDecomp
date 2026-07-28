struct CExpressionDigScriptNode;
struct CExpressionDigScriptOwner;

struct CExpressionDigScriptNode {
    std::byte _pad0[0x08];
    CExpressionDigScriptNode* at_08;
    CExpressionDigScriptNode* at_0C;
};

static_assert(offsetof(CExpressionDigScriptNode, at_08) == 0x08);
static_assert(offsetof(CExpressionDigScriptNode, at_0C) == 0x0C);

struct CExpressionDigScriptOwner {
    std::byte _pad0[0x04];
    CExpressionDigScriptNode* root;
    CExpressionDigScriptNode* at_08;
    CExpressionDigScriptNode* at_0C;
};

static_assert(offsetof(CExpressionDigScriptOwner, root) == 0x04);
static_assert(offsetof(CExpressionDigScriptOwner, at_08) == 0x08);
static_assert(offsetof(CExpressionDigScriptOwner, at_0C) == 0x0C);

struct CTCCarryable {
    CExpressionDigScriptOwner* expr_owner;
    std::uint32_t at_04;

    void OnKill();
};

static_assert(offsetof(CTCCarryable, expr_owner) == 0x00);
static_assert(offsetof(CTCCarryable, at_04) == 0x04);

extern CExpressionDigScriptNode* CExpression_DigScript_CloneTree(
    CExpressionDigScriptNode* root,
    CExpressionDigScriptOwner* owner);

CTCCarryable* __thiscall CTCCarryable_assign(CTCCarryable* self, CTCCarryable* other)
{
    if (self != other) {
        self->OnKill();
        self->at_04 = 0;

        if (other->expr_owner->root == nullptr) {
            self->expr_owner->root = nullptr;
            self->expr_owner->at_08 = reinterpret_cast<CExpressionDigScriptNode*>(self->expr_owner);
            self->expr_owner->at_0C = reinterpret_cast<CExpressionDigScriptNode*>(self->expr_owner);
        } else {
            CExpressionDigScriptOwner* const owner = self->expr_owner;
            owner->root = CExpression_DigScript_CloneTree(other->expr_owner->root, owner);

            CExpressionDigScriptNode* node = self->expr_owner->root;
            CExpressionDigScriptNode* last;
            do {
                last = node;
                node = last->at_08;
            } while (node != nullptr);
            self->expr_owner->at_08 = last;

            node = self->expr_owner->root;
            do {
                last = node;
                node = last->at_0C;
            } while (node != nullptr);
            self->expr_owner->at_0C = last;

            self->at_04 = other->at_04;
        }
    }

    return self;
}