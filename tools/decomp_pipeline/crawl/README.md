# Binary-wide byte-parity crawl

High-yield lane (~96% win on the small-function tail). See docs/HANDOFF.md
"Session 2026-08-01 (late)" and memory `binary-wide-parity-crawl.md`.

1. `python next_smallest.py <N> <prefix>` — emits `<prefix>.json` targets +
   `<prefix>_oracle.tsv` (smallest un-landed manifest fns, complete prototype +
   known CC, minus catalog-landed and `gen_tried.txt`).
2. Launch workflow `decomp-byte-match-batch-wf_702fd395-6d6.js` with the targets
   (merge oracle bytes inline; add one-line disasm hints per fn).
3. `python land_gen.py <taskid> <prefix>` — builds `<prefix>_land.json` from WIN
   candidates (trims over-captured oracles at first `cccccccc` run) and updates
   `gen_tried.txt` with ALL attempted addrs.
4. `python ../verify_and_land.py <prefix>_land.json <prefix>_land_oracle.tsv --land`
5. Commit; repeat.

Paths in these scripts point at the session scratchpad; adjust SCR/base if the
scratchpad dir changed. `gen_tried.txt` here is the durable copy of the attempted
ledger (52 addrs at snapshot, before gen_batch3 landed).

## Find-loop out-of-line-found-block idiom (2026-08-16)
Load-path coordinators (CPlayerManager::LoadGameState etc.) search a vector for an element
by field, using the found element if matched else a default index. Retail keeps `begin` in a
BASE register + a separate INDEX, and emits the found-case element load OUT-OF-LINE
(`je found; ...; found: mov reg,[base+idx*4]; jmp use`). Reproduce it with INDEX-BASED access
(`begin[i]`), NOT pointer-walk (`++p` folds the deref into one shared mov and misses the
out-of-line block). Pattern:
    T* n = this->begin[this->defaultIdx];
    for (int i=0;i<count;i++){ if (this->begin[i]->field==target){ n=this->begin[i]; break; } }
    Use(n);
This was the "one regalloc artifact in LoopB" that CONTINUE_GAME_PATH flagged as not-matchable.

### Find-loop coordinator landscape (2026-08-16)
Only TWO functions binary-wide carry the out-of-line find-loop idiom: AddStatUpdate 0x647457
and DestroyPlayers 0x44a070 (both CPlayerManager-family). CQuestManager::LoadGameState is NOT
a find-loop (it's a version-dispatched string-read + CStringParser + delegate — temp-scheduling
tier). Applying the idiom: the find-loops MATCH, but each function has a SECONDARY blocker:
- CPlayerManager::LoadGameState 0x449e60: local stack-slot ordering (VC vs retail slot assignment).
- DestroyPlayers 0x44a070: a std::vector::erase(begin,end) clear (cmp end,end + memmove template) —
  the STL-clone class; manual erase models get optimized away (compiler proves last==end).
So the find-loop idiom is proven+reusable, but the functions that use it are gated by slot-layout /
STL-erase, not the find-loop itself.
