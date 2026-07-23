#!/usr/bin/env python3
"""
match_fingerprints.py — join donor (ego_r, PDB-named) and target (Fable.exe, stripped)
function fingerprints (from ExportFuncFingerprints.java) into a rename plan.

High precision by construction:
  - donor fingerprint kept only if it maps to EXACTLY ONE real name and occurs ONCE on the donor.
  - target function renamed only if its fingerprint occurs ONCE on the target and is currently default-named.
  => a match is a 1:1:1 correspondence on a content fingerprint = very low false-positive rate.

Usage: match_fingerprints.py <fp_egor.tsv> <fp_fable.tsv> <out_plan.tsv>
"""
import sys, collections

donor_path, target_path, out_path = sys.argv[1], sys.argv[2], sys.argv[3]

donor_names = collections.defaultdict(set)   # fp -> set(real names)
donor_count = collections.Counter()          # fp -> total occurrences on donor
with open(donor_path, encoding="utf-8", errors="replace") as f:
    for ln in f:
        p = ln.rstrip("\n").split("\t")
        if len(p) < 5: continue
        fp, name, real = p[0], p[1], p[2]
        donor_count[fp] += 1
        if real == "1":
            donor_names[fp].add(name)

donor_map = {}
for fp, names in donor_names.items():
    if len(names) == 1 and donor_count[fp] == 1:
        donor_map[fp] = next(iter(names))

target_rows = []
target_count = collections.Counter()
with open(target_path, encoding="utf-8", errors="replace") as f:
    for ln in f:
        p = ln.rstrip("\n").split("\t")
        if len(p) < 5: continue
        fp, name, real, nparts, addr = p[0], p[1], p[2], p[3], p[4]
        target_count[fp] += 1
        target_rows.append((fp, real, addr))

plan = []
for fp, real, addr in target_rows:
    if real == "1": continue            # already named
    if target_count[fp] != 1: continue  # ambiguous on target
    if fp in donor_map:
        plan.append((addr, donor_map[fp]))

with open(out_path, "w", encoding="utf-8") as o:
    for addr, nm in plan:
        o.write(f"{addr}\t{nm}\n")

print(f"donor_unique_named_fps={len(donor_map)}  target_distinctive_fns={len(target_rows)}  "
      f"RENAME_PLAN={len(plan)}  -> {out_path}")
