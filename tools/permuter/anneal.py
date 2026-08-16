#!/usr/bin/env python3
"""Full-regalloc permuter: simulated-annealing search over the whole mutation library.

This is the driver upstream decomp-permuter calls "the permuter": rather than a greedy
best-first descent (which stalls on the first regalloc plateau), it takes a *random walk*
biased downhill by a cooling Metropolis criterion, so it can accept a lateral or slightly
worse move to reach a spelling three mutations away that finally flips the register
allocation.

State  = (source_text, flags, pragma).  Objective = permuter_score.score_source (0 = byte/
relocation exact; else same-length distance = #differing bytes, or a large length penalty).

Each step:
  * with prob `flag_move_p`, keep the source and perturb (flags, pragma);
  * otherwise pick a uniform-random single-step mutation of the current source from the
    combined library: clang_mutations.all_variants (temp-intro / reassoc / stmt-split) +
    regalloc_mutations.all_regalloc_variants (inline-temp / decl-reorder / block-insert /
    retype).
  * score (memoized), accept if dE<0 or rand() < exp(-dE/T).
  * T decays geometrically T0 -> Tend across `iters`.
  * every `restart` steps without a global-best improvement, teleport back to the best-known
    state (basin hop) so a bad walk cannot strand the search.

A score-0 result is written to <name>.match.cpp; the best non-match to <name>.best.cpp.
Byte-match is necessary but not sufficient -- re-check with the behaviour test before landing
(verify_and_land does this automatically).

Usage:
  python anneal.py <hexaddr> <plain.cpp> [--oracle t.tsv] [--name leaf]
                   [--iters 1500] [--seed 1] [--t0 4.0] [--tend 0.3]
                   [--restart 120] [--flag-move-p 0.15] [--quiet]
"""
from __future__ import annotations
import argparse, math, random, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from permuter_score import score_source, DEFAULT_ORACLE, COMPILE_FAIL, _read_tsv  # noqa: E402

FLAG_SUBSET = [["/O2", "/Oy"], ["/O1", "/Oy"], ["/Ox", "/Oy"], ["/Os", "/Oy"],
               ["/Ot", "/Oy"], ["/O2", "/Oy", "/Ob1"], ["/O2", "/Oy", "/Gy"]]
PRAGMA_SUBSET = ["", '#pragma optimize("s",on)', '#pragma optimize("t",on)',
                 '#pragma optimize("g",on)', '#pragma optimize("gs",on)',
                 '#pragma optimize("a",on)', '#pragma optimize("y",on)']


def _load_mutlibs():
    libs = []
    try:
        import clang_mutations as cm
        libs.append(("cm", cm.all_variants))
    except Exception as e:
        print(f"  (clang_mutations unavailable: {e})")
    try:
        import regalloc_mutations as rm
        libs.append(("rm", rm.all_regalloc_variants))
    except Exception as e:
        print(f"  (regalloc_mutations unavailable: {e})")
    return libs


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("addr"); ap.add_argument("cpp", type=Path)
    ap.add_argument("--oracle", type=Path, default=DEFAULT_ORACLE)
    ap.add_argument("--name", default=None)
    ap.add_argument("--iters", type=int, default=1500)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--t0", type=float, default=4.0)
    ap.add_argument("--tend", type=float, default=0.3)
    ap.add_argument("--restart", type=int, default=120,
                    help="steps without global improvement before hopping back to best")
    ap.add_argument("--flag-move-p", type=float, default=0.15)
    ap.add_argument("--qfe", action="store_true", help="compile with the 13.10.4035 QFE toolset")
    ap.add_argument("--quiet", action="store_true")
    a = ap.parse_args()

    addr = a.addr.lower().replace("0x", "")
    orc = {r["address"].lower(): r for r in _read_tsv(a.oracle)}
    if addr not in orc:
        print(f"NO_ORACLE for {addr}"); return 3
    leaf = (a.name or orc[addr]["name"]).rsplit("::", 1)[-1].lstrip("~")

    work = a.cpp.parent / "_anneal"; work.mkdir(exist_ok=True)
    vp = work / "v.cpp"
    walkf = work / "walk.cpp"
    rng = random.Random(a.seed)
    libs = _load_mutlibs()

    cache: dict = {}
    evals = 0

    def score(text, flags, pragma):
        nonlocal evals
        key = (hash(text), tuple(flags), pragma)
        if key in cache:
            return cache[key]
        vp.write_text(text, encoding="utf-8")
        r = score_source(vp, addr, a.name, a.oracle, workdir=work, flags=flags,
                         prepend=pragma, qfe=a.qfe)
        cache[key] = r
        evals += 1
        return r

    def neighbors(text):
        walkf.write_text(text, encoding="utf-8")
        out = []
        for _tag, fn in libs:
            try:
                out += fn(walkf, leaf)
            except Exception:
                pass
        return out

    # --- initial state: quick flag/pragma sweep on the untouched source for a good seed
    base = a.cpp.read_text(encoding="utf-8")
    cur_text = base
    cur_flags, cur_pragma = ["/O2", "/Oy"], ""
    cur = score(cur_text, cur_flags, cur_pragma)
    best = (cur, cur_text, cur_flags, cur_pragma, "seed")
    for flags in FLAG_SUBSET:
        for pragma in PRAGMA_SUBSET:
            r = score(base, flags, pragma)
            if r["score"] < best[0]["score"]:
                best = (r, base, flags, pragma, f"seed {' '.join(flags)} {pragma}")
            if r["score"] == 0:
                return _finish(a, best, evals)
    cur, cur_text, cur_flags, cur_pragma = best[0], best[1], best[2], best[3]
    if not a.quiet:
        print(f"seed best score={best[0]['score']} status={best[0]['status']} "
              f"({best[4]}); {evals} compiles")

    # --- deterministic greedy 1-hop sweep: every single mutation of the base, across a
    # small flag/pragma subset. Guarantees a match that is one mutation away is found
    # regardless of the SA random seed (the SA loop below handles multi-hop combos).
    GREEDY_FLAGS = [["/O2", "/Oy"], ["/O1", "/Oy"], ["/Os", "/Oy"]]
    GREEDY_PRAGMAS = ["", '#pragma optimize("s",on)', '#pragma optimize("t",on)']
    for _label, mtext in neighbors(base):
        for flags in GREEDY_FLAGS:
            for pragma in GREEDY_PRAGMAS:
                r = score(mtext, flags, pragma)
                if r["score"] < best[0]["score"]:
                    best = (r, mtext, flags, pragma, f"greedy {_label}")
                    if not a.quiet:
                        print(f"  greedy NEW BEST score={r['score']:>8} {r['status']:16} {_label}")
                if r["score"] == 0:
                    return _finish(a, best, evals)
    cur, cur_text, cur_flags, cur_pragma = best[0], best[1], best[2], best[3]

    last_improve = 0
    for step in range(a.iters):
        T = a.t0 * (a.tend / a.t0) ** (step / max(1, a.iters - 1))

        if rng.random() < a.flag_move_p or not libs:
            cand_text = cur_text
            cand_flags = rng.choice(FLAG_SUBSET)
            cand_pragma = rng.choice(PRAGMA_SUBSET)
            label = f"flag {' '.join(cand_flags)} {cand_pragma or '-'}"
        else:
            nb = neighbors(cur_text)
            if not nb:
                cand_text = cur_text
                cand_flags = rng.choice(FLAG_SUBSET)
                cand_pragma = rng.choice(PRAGMA_SUBSET)
                label = f"flag(no-mut) {' '.join(cand_flags)}"
            else:
                label, cand_text = rng.choice(nb)
                cand_flags, cand_pragma = cur_flags, cur_pragma

        r = score(cand_text, cand_flags, cand_pragma)
        dE = r["score"] - cur["score"]
        if dE < 0 or rng.random() < math.exp(-dE / max(1e-6, T)):
            cur, cur_text, cur_flags, cur_pragma = r, cand_text, cand_flags, cand_pragma

        if r["score"] < best[0]["score"]:
            best = (r, cand_text, cand_flags, cand_pragma, label)
            last_improve = step
            if not a.quiet:
                extra = f"prefix={r.get('prefix')}/{r.get('retail')}" if r["status"] == "DIFFER" else ""
                print(f"  step {step:5} T={T:5.2f} NEW BEST score={r['score']:>8} "
                      f"{r['status']:16} {label} {extra}")
            if r["score"] == 0:
                print(f"MATCH after {evals} compiles ({step} steps)")
                return _finish(a, best, evals)

        if step - last_improve >= a.restart:
            cur, cur_text, cur_flags, cur_pragma = best[0], best[1], best[2], best[3]
            last_improve = step
            if not a.quiet:
                print(f"  step {step:5} restart -> best score={best[0]['score']}")

    return _finish(a, best, evals)


def _finish(a, best, evals):
    r, text, flags, pragma, label = best
    outp = a.cpp.with_suffix(".match.cpp" if r["score"] == 0 else ".best.cpp")
    body = (pragma + "\n" + text) if pragma else text
    outp.write_text(body, encoding="utf-8")
    print(f"\nBEST score={r['score']} status={r['status']}  ({evals} compiles)")
    print(f"  flags: {' '.join(flags)}   pragma: {pragma or '(none)'}   via: {label}")
    print(f"  wrote {outp}")
    return 0 if r["score"] == 0 else 2


if __name__ == "__main__":
    raise SystemExit(main())
