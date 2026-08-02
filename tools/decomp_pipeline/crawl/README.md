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
