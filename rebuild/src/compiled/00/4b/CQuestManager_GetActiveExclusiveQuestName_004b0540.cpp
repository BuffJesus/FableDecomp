// CQuestManager::GetActiveExclusiveQuestName  @0x004b0540
// module: CQuestManager  __fastcall  returns CCharString*

struct CCharString;

struct SubNode {
    int              key;    // +0x00
    SubNode*         next;   // +0x04
    char             pad[0x28 - 8];
    CCharString*     name;   // +0x28
};

struct Sub {                 // located at QData+0x44
    char             base0;  // +0x00 (Find 'this')
    char             pad0[3];
    SubNode*         end;     // +0x04
    SubNode* Find(int* pkey); // implicit __fastcall: this=ecx, pkey pushed
};

struct QData {
    char             pad0[0x2c];
    unsigned int     flags;  // +0x2c
    char             pad1[0x44 - 0x2c - 4];
    Sub              sub;     // +0x44
};

struct TNode {
    TNode*           left;   // +0x00
    char             pad0[4];
    char             keybuf[1]; // +0x08
};

struct QManager {
    char             pad0[0x5c];
    TNode*           treehdr; // +0x5c
};

extern QData* __fastcall QLookup(TNode* keynode);   // 0x551610
extern bool __fastcall SubCheck(SubNode* self);      // 0x251d60
extern CCharString g_defaultQuestName;               // object @ 0x13bd804

CCharString* __fastcall CQuestManager_GetActiveExclusiveQuestName(QManager* self)
{
    union { int scratch; SubNode* found; } u; // single local (the push ecx slot)
    TNode* node = *(TNode**)self->treehdr;
    if (node != self->treehdr) {
        SubNode* found = u.found; // preheader read (once); loop preserves esi
        do {
            QData* q = QLookup((TNode*)((char*)node + 8));
            if (q) {
                if (q->flags & 0x1000) {
                    Sub* sub = &q->sub;
                    u.scratch = 0x6c;
                    SubNode* r = sub->Find(&u.scratch);
                    SubNode* end = sub->end;
                    if (r == end || r->key > 0x6c)
                        r = end;
                    found = r->next;
                }
                if (found) {
                    if (SubCheck(found))
                        return (CCharString*)&found->name;
                }
            }
            node = node->left;
        } while (node != self->treehdr);
    }
    return &g_defaultQuestName;
}