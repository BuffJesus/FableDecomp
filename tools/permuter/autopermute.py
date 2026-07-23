#!/usr/bin/env python3
"""Automatic permuter (no annotation). Two automatic levers, both scored by the real
VC7.1 cl.exe against retail bytes (relocation-masked):

  1. FLAG/PRAGMA SWEEP  -- always semantics-preserving. Retail translation units were
     built with varying optimization settings (this is why `optimize("s")` matters), so
     brute-force the flag x pragma grid with the source untouched.
  2. AUTO SOURCE MUTATION (random search, optional --mutate) -- auto-detects commutative
     operators (+ * & | ^) and randomly swaps operands (commutative => same result) to
     shuffle the compiler's register allocation / scheduling, layered on the grid.

Any score-0 result should still be re-checked with its behaviour test before landing
(verify_and_land does this) -- byte-match is necessary, the behaviour test guards semantics.

Usage:
  python autopermute.py <hexaddr> <plain.cpp> [--oracle t.tsv] [--name leaf]
                        [--mutate] [--iters N] [--seed S]
"""
from __future__ import annotations
import argparse, random, re, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from permuter_score import score_source, DEFAULT_ORACLE  # noqa: E402

FLAG_SETS = [
    ["/O2", "/Oy"], ["/O2", "/Oy-"],
    ["/O1", "/Oy"], ["/O1", "/Oy-"],
    ["/Ox", "/Oy"], ["/Ox", "/Oy-"],
    ["/Os", "/Oy"], ["/Ot", "/Oy"],
    ["/O2", "/Oy", "/Ob0"], ["/O2", "/Oy", "/Ob1"], ["/O2", "/Oy", "/Ob2"],
    ["/Og", "/Oy"], ["/O2", "/Oy", "/Gy"], ["/O2", "/Oy", "/Gy-"],
    ["/O1", "/Os"], ["/O2", "/Os"], ["/O2", "/Ot"],
]
PRAGMAS = [
    "",
    '#pragma optimize("s",on)',
    '#pragma optimize("t",on)',
    '#pragma optimize("g",on)',
    '#pragma optimize("a",on)',
    '#pragma optimize("y",on)',
    '#pragma optimize("gs",on)',
    '#pragma optimize("",on)',
    '#pragma auto_inline(off)',
    '#pragma inline_depth(0)',
]

# ATOM op ATOM where op is commutative. ATOM = identifier / member / index / simple call.
_ATOM = r"[A-Za-z_]\w*(?:(?:->|\.)\w+|\[[^\]\[]*\]|\([^()]*\))*"
_COMMUT = re.compile(rf"(?P<a>{_ATOM})\s*(?P<op>[+*&|^])\s*(?P<b>{_ATOM})")


def find_swaps(text: str):
    """Non-overlapping commutative-op sites; each is a (start,end,swapped_text)."""
    sites, last = [], -1
    for m in _COMMUT.finditer(text):
        if m.start() < last:
            continue
        # skip ++ / && / || / *ptr-deref-looking and compound assigns
        seg = m.group(0)
        if any(t in seg for t in ("++", "--", "&&", "||", "+=", "*=", "&=", "|=", "^=")):
            continue
        swapped = f"{m.group('b')} {m.group('op')} {m.group('a')}"
        sites.append((m.start(), m.end(), swapped))
        last = m.end()
    return sites


def apply_swaps(text: str, sites, chosen):
    for i in sorted(chosen, reverse=True):
        s, e, sw = sites[i]
        text = text[:s] + sw + text[e:]
    return text


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("addr"); ap.add_argument("cpp", type=Path)
    ap.add_argument("--oracle", type=Path, default=DEFAULT_ORACLE)
    ap.add_argument("--name", default=None)
    ap.add_argument("--mutate", action="store_true", help="also random-search commutative swaps")
    ap.add_argument("--iters", type=int, default=400)
    ap.add_argument("--seed", type=int, default=1)
    a = ap.parse_args()

    base = a.cpp.read_text(encoding="utf-8")
    work = a.cpp.parent / "_auto"; work.mkdir(exist_ok=True)
    rng = random.Random(a.seed)
    vp = work / "v.cpp"

    def sc(text, flags, pragma):
        vp.write_text(text, encoding="utf-8")
        return score_source(vp, a.addr, a.name, a.oracle, workdir=work, flags=flags, prepend=pragma)

    best = None
    tried = 0

    def consider(text, flags, pragma, label):
        nonlocal best, tried
        tried += 1
        r = sc(text, flags, pragma)
        if best is None or r["score"] < best[0]["score"]:
            best = (r, text, flags, pragma, label)
            extra = f"prefix={r.get('prefix')}/{r.get('retail')}" if r["status"] == "DIFFER" else ""
            print(f"  NEW BEST score={r['score']:>8} {r['status']:16} {label} {extra}")
        return r["score"] == 0

    # Phase 1: exhaustive flag x pragma grid on the untouched source
    print(f"Phase 1: flag x pragma grid ({len(FLAG_SETS)*len(PRAGMAS)} compiles)")
    for flags in FLAG_SETS:
        for pragma in PRAGMAS:
            if consider(base, flags, pragma, f"{' '.join(flags)} | {pragma or '(no pragma)'}"):
                print("MATCH via flag/pragma sweep"); return _finish(a, best)

    # Phase 2: optional random commutative-swap search, each with a random flag/pragma
    if a.mutate:
        sites = find_swaps(base)
        print(f"Phase 2: random commutative-swap search over {len(sites)} sites x {a.iters} iters")
        if sites:
            for _ in range(a.iters):
                k = rng.randint(1, len(sites))
                chosen = rng.sample(range(len(sites)), k)
                text = apply_swaps(base, sites, chosen)
                flags = rng.choice(FLAG_SETS); pragma = rng.choice(PRAGMAS)
                if consider(text, flags, pragma, f"swap{sorted(chosen)} {' '.join(flags)} {pragma or ''}"):
                    print("MATCH via mutation search"); return _finish(a, best)
        else:
            print("  (no commutative sites detected)")

    return _finish(a, best)


def _finish(a, best):
    r, text, flags, pragma, label = best
    outp = a.cpp.with_suffix(".match.cpp" if r["score"] == 0 else ".best.cpp")
    body = (pragma + "\n" + text) if pragma else text
    outp.write_text(body, encoding="utf-8")
    print(f"\nBEST score={r['score']} status={r['status']}")
    print(f"  flags: {' '.join(flags)}   pragma: {pragma or '(none)'}")
    print(f"  wrote {outp}")
    return 0 if r["score"] == 0 else 2


if __name__ == "__main__":
    raise SystemExit(main())
