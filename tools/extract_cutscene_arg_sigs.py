#!/usr/bin/env python3
"""Extract per-verb argument signatures + dispatch order from the decompiled
RunCutsceneMacro_Func pseudo-C.

Method
------
The interpreter tokenizes a command's arguments into a fixed array of CCharString
slots on the stack. In the decompilation these appear as `local_44` (arg1),
`local_40` (arg2), ... down to `local_18` (arg12), stride 4 bytes. The verb is in
`local_c`. Dispatch is a linear chain of
    CCharString(tmp, "<VERB>", -1); ... strncmp(local_c, tmp, len) == 0
tests. Each verb's handler block runs from its dispatch anchor to the next
anchor in file order, which also recovers the true chain ORDER (for prefix
collisions like Remove vs RemoveAll*).

Within a handler block we classify each arg slot by how it is consumed:
    GF(Char)StringToFloat(&slot)  -> float
    GF(Char)StringToInt(&slot)    -> int
    IsFalse(&slot)                -> bool   (TRUE/FALSE flag)
    GetLength(&slot)              -> (presence/required check only)
    otherwise (operator char*, passed by name) -> str

Output: TSV `verb, order, argCount, sig` where sig is arg1..argN joined, each
`argK:type` (type = float|int|bool|str|opt).
"""
import re
import sys
from pathlib import Path

C_FILE = Path(r"D:\Documents\FableTLC\ghidra_out\runcutscenemacro_full.c")
VERB_FILE = Path(r"D:\Documents\FableTLC\ghidra_out\cutscene_native_verbs.txt")
OUT_FILE = Path(r"D:\Documents\FableTLC\ghidra_out\cutscene_verb_args.tsv")

# stack slot offset (hex) -> 1-based arg index
SLOT_ARG = {}
off = 0x44
for i in range(1, 13):
    SLOT_ARG[f"local_{off:x}"] = i
    off -= 4

ARG_SLOT_RE = re.compile(r"&(local_(?:44|40|3c|38|34|30|2c|28|24|20|1c|18))\b")


def load_verbs():
    verbs = []
    for line in VERB_FILE.read_text(encoding="utf-8-sig").splitlines():
        v = line.strip().lstrip("﻿")
        if v:
            verbs.append(v)
    return verbs


def find_anchors(text, line_starts, verbs):
    """Return list of (lineIdx, verb) for each verb-dispatch site, in file order.

    A dispatch site is a CCharString ctor of the exact verb literal whose next
    stretch of text contains a strncmp against the command string local_c. The
    decompiler wraps long lines, so the ctor call itself may straddle a newline
    (`CCharString::CCharString\\n(local_x,"Verb",-1)`); match on the whole text
    with whitespace-tolerant patterns rather than per line.
    """
    verbset = set(verbs)
    anchors = []
    ctor_re = re.compile(
        r'CCharString::CCharString\s*\([^,]+,\s*"([^"\\]+)"\s*,\s*-1\)')
    for m in ctor_re.finditer(text):
        lit = m.group(1)
        if lit not in verbset:
            continue
        window = text[m.start():m.start() + 1400]
        if "strncmp" in window and "local_c)" in window:
            # char offset -> 0-based line index
            import bisect
            line_idx = bisect.bisect_right(line_starts, m.start()) - 1
            anchors.append((line_idx, lit))
    return anchors


def classify_block(block_text):
    """Return {argIndex: type} for the arg slots referenced in a handler block."""
    args = {}

    def note(idx, typ):
        # Priority: a concrete conversion beats a presence-only check.
        rank = {"opt": 0, "str": 1, "bool": 2, "int": 3, "float": 3}
        cur = args.get(idx)
        if cur is None or rank[typ] > rank[cur]:
            args[idx] = typ

    # Typed conversions.
    for m in re.finditer(r"GF(?:Char)?StringToFloat\(\(CCharString \*\)&(local_\w+)\)", block_text):
        if m.group(1) in SLOT_ARG:
            note(SLOT_ARG[m.group(1)], "float")
    for m in re.finditer(r"GF(?:Char)?StringToInt\(\(CCharString \*\)&(local_\w+)\)", block_text):
        if m.group(1) in SLOT_ARG:
            note(SLOT_ARG[m.group(1)], "int")
    for m in re.finditer(r"IsFalse\(\(CCharString \*\)&(local_\w+)\)", block_text):
        if m.group(1) in SLOT_ARG:
            note(SLOT_ARG[m.group(1)], "bool")
    # Any other address-of-arg use (name/string/presence).
    for m in ARG_SLOT_RE.finditer(block_text):
        idx = SLOT_ARG[m.group(1)]
        note(idx, "str")
    return args


def main():
    text = C_FILE.read_text(encoding="utf-8", errors="replace")
    lines = text.splitlines()
    # Char offset of the start of each line, for offset->line mapping.
    line_starts = [0]
    for ln in lines:
        line_starts.append(line_starts[-1] + len(ln) + 1)
    verbs = load_verbs()
    anchors = find_anchors(text, line_starts, verbs)

    # Deduplicate: a verb can have multiple ctor sites (e.g. re-used literal);
    # keep the first dispatch occurrence per verb, but preserve file order for
    # block boundaries using ALL anchors.
    anchor_lines = sorted(set(a[0] for a in anchors))
    line_to_verb = {}
    for idx, v in anchors:
        line_to_verb.setdefault(idx, v)

    # Build ordered unique-verb dispatch list (first occurrence per verb).
    seen = set()
    ordered = []
    for idx in anchor_lines:
        v = line_to_verb[idx]
        if v in seen:
            continue
        seen.add(v)
        ordered.append((idx, v))

    results = []
    for order, (idx, verb) in enumerate(ordered, 1):
        # Block end = next anchor line strictly greater than idx.
        nxt = None
        for al in anchor_lines:
            if al > idx:
                nxt = al
                break
        end = nxt if nxt is not None else len(lines)
        block = "\n".join(lines[idx:end])
        args = classify_block(block)
        arg_count = max(args.keys()) if args else 0
        sig_parts = []
        for k in range(1, arg_count + 1):
            sig_parts.append(f"arg{k}:{args.get(k, 'opt')}")
        results.append((verb, order, arg_count, " ".join(sig_parts)))

    missing = sorted(set(verbs) - seen)

    with OUT_FILE.open("w", encoding="utf-8", newline="") as f:
        f.write("verb\torder\targcount\tsignature\n")
        for verb, order, ac, sig in results:
            f.write(f"{verb}\t{order}\t{ac}\t{sig}\n")

    sys.stdout.reconfigure(encoding="utf-8")
    print(f"verbs: {len(verbs)}  dispatched: {len(ordered)}  "
          f"missing: {len(missing)}")
    if missing:
        print("MISSING (no dispatch anchor found):", ", ".join(missing))
    # Quick stats.
    with_args = sum(1 for r in results if r[2] > 0)
    print(f"handlers with >=1 detected arg: {with_args}")
    print(f"wrote {OUT_FILE}")


if __name__ == "__main__":
    sys.exit(main())
