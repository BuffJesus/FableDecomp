struct S; extern int __fastcall lr_helper(S*);
struct CThingAICreature; struct S { char pad[0xC]; int fC; void LogReaction(const CThingAICreature&, bool); };
void S::LogReaction(const CThingAICreature&, bool){ this->fC = lr_helper(this); }