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
from permuter_score import score_source, DEFAULT_ORACLE, COMPILE_FAIL  # noqa: E402

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
    ap.add_argument("--mutate", action="store_true", help="greedy AST mutation search (temp-intro + reassoc)")
    ap.add_argument("--depth", type=int, default=3, help="greedy stacking depth for AST mutations")
    ap.add_argument("--random", type=int, default=0, help="random multi-mutation restarts (composes several mutations)")
    ap.add_argument("--walk", type=int, default=6, help="max mutations composed per random restart")
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

    def _update_best(r, text, flags, pragma, label):
        nonlocal best
        if best is None or r["score"] < best[0]["score"]:
            best = (r, text, flags, pragma, label)
            extra = f"prefix={r.get('prefix')}/{r.get('retail')}" if r["status"] == "DIFFER" else ""
            print(f"  NEW BEST score={r['score']:>8} {r['status']:16} {label} {extra}")

    def tried_inc():
        nonlocal tried
        tried += 1

    def consider(text, flags, pragma, label):
        tried_inc()
        r = sc(text, flags, pragma)
        _update_best(r, text, flags, pragma, label)
        return r["score"] == 0

    # Phase 1: exhaustive flag x pragma grid on the untouched source
    print(f"Phase 1: flag x pragma grid ({len(FLAG_SETS)*len(PRAGMAS)} compiles)")
    for flags in FLAG_SETS:
        for pragma in PRAGMAS:
            if consider(base, flags, pragma, f"{' '.join(flags)} | {pragma or '(no pragma)'}"):
                print("MATCH via flag/pragma sweep"); return _finish(a, best)

    # Shared AST-mutation setup (used by greedy Phase 2 and random Phase 3).
    cm = None
    leaf = ""
    if a.mutate or a.random:
        try:
            import clang_mutations as cm  # type: ignore
        except Exception as e:
            print(f"  (clang mutations unavailable: {e})"); cm = None
        from permuter_score import _read_tsv  # type: ignore
        orc = {r["address"].lower(): r for r in _read_tsv(a.oracle)}
        leaf = (a.name or orc.get(a.addr.lower().replace("0x", ""), {}).get("name", "")).rsplit("::", 1)[-1]
    FLAG_SUBSET = [["/O2", "/Oy"], ["/O1", "/Oy"], ["/Ox", "/Oy"], ["/Os", "/Oy"], ["/Ot", "/Oy"]]
    PRAGMA_SUBSET = ["", '#pragma optimize("s",on)', '#pragma optimize("t",on)', '#pragma optimize("g",on)']

    def mutations_of(text):
        """All single-step AST mutations of `text` (re-parsed)."""
        wf = work / "walk.cpp"
        wf.write_text(text, encoding="utf-8")
        return cm.temp_intro_variants(wf, leaf) + cm.reassoc_variants(wf, leaf)

    # Phase 2: greedy stacking (deterministic best-first).
    if a.mutate and cm:
        cur = base
        curfile = work / "cur.cpp"
        for depth in range(a.depth):
            curfile.write_text(cur, encoding="utf-8")
            variants = []
            if cm:
                variants += cm.temp_intro_variants(curfile, leaf)
                variants += cm.reassoc_variants(curfile, leaf)
            if not variants:
                print(f"Phase 2 depth {depth}: no AST variants"); break
            print(f"Phase 2 depth {depth}: {len(variants)} mutation variants x {len(FLAG_SUBSET)*len(PRAGMA_SUBSET)} flag/pragma")
            round_best = None
            for lbl, mtext in variants:
                for flags in FLAG_SUBSET:
                    for pragma in PRAGMA_SUBSET:
                        r = sc(mtext, flags, pragma)
                        tried_inc()
                        if round_best is None or r["score"] < round_best[0]["score"]:
                            round_best = (r, mtext, flags, pragma, lbl)
                        if r["score"] == 0:
                            _update_best(r, mtext, flags, pragma, lbl)
                            print(f"MATCH via AST mutation: {lbl}"); return _finish(a, best)
            # accept round best if it improves; else stop
            rb = round_best[0]["score"]
            if rb < best[0]["score"]:
                _update_best(*round_best)
                print(f"  depth {depth} accepted: score={rb} via {round_best[4]}")
                cur = round_best[1]
            else:
                print(f"  depth {depth} no improvement (best round score={rb}); stopping"); break

    # Phase 3: random multi-mutation combination. A random walk that COMPOSES several
    # mutations (accepting lateral/worse moves) so it can cross a plateau -- e.g. a pair
    # of temp-introductions that together flip a register allocation no single one does.
    if a.random and cm:
        print(f"Phase 3: random multi-mutation ({a.random} restarts x walk<={a.walk})")
        for restart in range(a.random):
            # 30% of restarts intensify from the best-known source, else diversify from base
            if best is not None and best[0]["score"] < COMPILE_FAIL and rng.random() < 0.3:
                cur = best[1]
            else:
                cur = base
            for step in range(a.walk):
                variants = mutations_of(cur)
                if not variants:
                    break
                _, cur = rng.choice(variants)          # random move (composes over steps)
                flags = rng.choice(FLAG_SUBSET)
                pragma = rng.choice(PRAGMA_SUBSET)
                tried_inc()
                r = sc(cur, flags, pragma)
                _update_best(r, cur, flags, pragma, f"rand r{restart}s{step+1} {' '.join(flags)} {pragma or ''}")
                if r["score"] == 0:
                    print(f"MATCH via random multi-mutation (restart {restart}, {step+1} mutations)")
                    return _finish(a, best)
        print(f"  random search done ({tried} total evaluations)")

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
