#!/usr/bin/env python3
"""decomp-permuter, VC7.1/x86 flavour (prototype).

Manual-mode permutation: annotate a candidate .cpp with choice points

    PERM(<alt0>|<alt1>|...)          # inline: tries each alternative
    PERMPRAGMA(<p0>|<p1>|)           # convenience for a whole line (empty = omit)

The driver expands the cross-product of all choice points (or random-samples when
the space is large), compiles each with the real VC7.1 cl.exe, scores the .text
against retail bytes (relocation-masked) via permuter_score, and reports the best.
Score 0 == exact/relocation byte match; it stops early on a match and writes the
winning source next to the input as <name>.match.cpp.

This targets the register-allocation / instruction-scheduling tail: wrap the 2-3
spellings you are unsure about in PERM(...) and let the search find the one whose
codegen matches. Mirrors simonlindholm/decomp-permuter's manual mode; the scorer is
our own relocation-masked objdump diff.

Usage:
  python permuter.py <hexaddr> <annotated.cpp> [--oracle t.tsv] [--name leaf]
                     [--max-iters N] [--seed S]
"""
from __future__ import annotations
import argparse, itertools, random, re, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from permuter_score import score_source, DEFAULT_ORACLE  # noqa: E402


def parse_choices(text: str):
    """Return (segments, choices): segments has len(choices)+1 literal parts;
    reconstruct a variant as segments[0]+choice[0]+segments[1]+choice[1]+..."""
    segments, choices = [], []
    i, n, last = 0, len(text), 0
    while i < n:
        m = re.compile(r"PERM(PRAGMA)?\(").match(text, i)
        if not m:
            i += 1; continue
        # balanced-paren scan from after '('
        depth, j = 1, m.end()
        while j < n and depth:
            if text[j] == "(":
                depth += 1
            elif text[j] == ")":
                depth -= 1
            j += 1
        inner = text[m.end():j - 1]
        alts = split_top(inner)
        segments.append(text[last:m.start()])
        choices.append(alts)
        last = j
        i = j
    segments.append(text[last:])
    return segments, choices


def split_top(s: str):
    """Split on top-level '|' (ignoring | inside nested parens/brackets/braces)."""
    out, depth, cur = [], 0, []
    for ch in s:
        if ch in "([{":
            depth += 1; cur.append(ch)
        elif ch in ")]}":
            depth -= 1; cur.append(ch)
        elif ch == "|" and depth == 0:
            out.append("".join(cur)); cur = []
        else:
            cur.append(ch)
    out.append("".join(cur))
    return out


def build(segments, combo):
    parts = [segments[0]]
    for k, choice in enumerate(combo):
        parts.append(choice)
        parts.append(segments[k + 1])
    return "".join(parts)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("addr")
    ap.add_argument("cpp", type=Path)
    ap.add_argument("--oracle", type=Path, default=DEFAULT_ORACLE)
    ap.add_argument("--name", default=None)
    ap.add_argument("--max-iters", type=int, default=2000)
    ap.add_argument("--seed", type=int, default=1)
    a = ap.parse_args()

    text = a.cpp.read_text(encoding="utf-8")
    segments, choices = parse_choices(text)
    total = 1
    for c in choices:
        total *= len(c)
    print(f"choice points: {len(choices)}  combinations: {total}")
    if not choices:
        print("no PERM(...) directives found; scoring as-is")

    work = a.cpp.parent / "_perm"
    work.mkdir(exist_ok=True)
    rng = random.Random(a.seed)

    if total <= a.max_iters:
        combos = itertools.product(*choices) if choices else [()]
        space = total
    else:
        def sample():
            return tuple(rng.choice(c) for c in choices)
        seen = set(); combos = []
        while len(combos) < a.max_iters:
            t = sample()
            if t not in seen:
                seen.add(t); combos.append(t)
        space = a.max_iters
        print(f"space too large; random-sampling {space}")

    best = None
    for idx, combo in enumerate(combos):
        src = build(segments, combo)
        vp = work / "variant.cpp"
        vp.write_text(src, encoding="utf-8")
        r = score_source(vp, a.addr, a.name, a.oracle, workdir=work)
        if best is None or r["score"] < best[0]["score"]:
            best = (r, combo, src)
            tag = r["status"]
            print(f"[{idx+1}/{space}] NEW BEST score={r['score']} {tag} "
                  + (f"prefix={r.get('prefix')}/{r.get('retail')}" if tag == "DIFFER" else ""))
        if r["score"] == 0:
            print(f"MATCH FOUND at combo {idx+1}: {r['status']}")
            break

    if best is None:
        print("no variants scored"); return 1
    r, combo, src = best
    outp = a.cpp.with_suffix(".match.cpp" if r["score"] == 0 else ".best.cpp")
    outp.write_text(src, encoding="utf-8")
    print(f"\nBEST: score={r['score']} status={r['status']} -> {outp}")
    if choices:
        print("winning choices:")
        for k, ch in enumerate(combo):
            print(f"  [{k}] {ch.strip()[:70]}")
    return 0 if r["score"] == 0 else 2


if __name__ == "__main__":
    raise SystemExit(main())
