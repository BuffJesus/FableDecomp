# Bandit Camp conversion bootstrap ? 2026-09-25

Offline recovery; no Bandit Camp override has been deployed or activated.

The retail lifecycle anchors bound the family at 0x00D006D0 through
0x00D12D00 (the next Bounty Hunt Init). The three scripts are Q_BanditCamp,
Q_BanditCampBossBattle and Q_BanditCampHoldingScript. Read-only Ghidra
export converged after three passes: 131 function bodies, all entity
lifecycle exports present, and no inventory errors. The family contains
16 entity bindings and 12 registered workers. Evidence currently lives in
`work/bandit_camp_bootstrap`; export logs in `work/bandit_camp_export`.
The x86 DIA tool extracted 31,708 bytes of PDB locals. Typed export applied
1,355 overrides across the 131 functions.

## Worker-name scope

The evidence builder rejected two legitimate WatchForTermination workers:
0x00D01290 registered by Q_BanditCamp Main, and 0x00D04260 registered by
Q_BanditCampBossBattle Main. It had validated unique names across the whole
translation unit before determining which functions a quest reaches.
Name uniqueness is now checked within the reached quest closure. Conflicting
names for one address still fail globally; different bodies with the same
name still fail if both are reached by the same quest.

Five focused tests pass, including two sibling quests retaining their own
worker and a transitive registration collision still being rejected.
An in-memory before/after evidence build across all 17 existing units found
identical results wherever the old builder succeeded. Previously blocked
Guild Training (nine scripts), Wasp and Trader Conflict (two scripts) now
build successfully. Evidence: `work/thread_scope_ab.json`. No existing
unit evidence was rewritten by this comparison.
