#!/usr/bin/env python3
"""Register-level mutations — the levers that reach the residuals source-shape and
adjacent-swap mutations can't: exact stack-slot ordering, register-choice coin-flips,
and register-pressure-driven allocation. All are byte-pure (semantics-preserving source
transformations that BIAS the VC7.1 allocator; the compiler still emits the bytes).

  permute_decl_variants  -- FULL permutations of the leading run of independent local
                            declarations (not just adjacent swaps). Local declaration
                            order is the dominant input to VC's stack-slot + first-fit
                            register assignment; the exact retail layout is often one
                            specific permutation. Exhaustive for small runs, capped
                            deterministically for large ones (the annealer samples the rest).
  pressure_variants      -- inject/remove a live dummy local. Adding a used temp raises
                            register pressure and shifts which values the allocator keeps in
                            registers vs reloads/spills; removing an existing single-use temp
                            (inline) lowers it. This is the lever for reload-vs-cache and
                            eax<->edx coin-flips that no source *shape* change reaches.

Complements clang_mutations (materialize-earlier) and regalloc_mutations (inline/adjacent-
reorder/block/retype). Wire into anneal.py's mutation library.
"""
from __future__ import annotations
import itertools
from pathlib import Path

from clang_mutations import _parse, _find_func, _off  # type: ignore
from regalloc_mutations import _top_stmts, _var_decls_in_stmt, _stmt_src, _ident_tokens  # type: ignore


def _decl_runs(top):
    """All maximal contiguous runs (>=2) of single-statement local declarations, anywhere
    in the body — not just the leading run (most functions open with a statement)."""
    runs, cur = [], []
    for st in top:
        if _var_decls_in_stmt(st):
            cur.append(st)
        else:
            if len(cur) >= 2:
                runs.append(cur)
            cur = []
    if len(cur) >= 2:
        runs.append(cur)
    return runs


def _independent(src, run):
    """True if the run's declarations can be freely reordered: no initializer references
    another declared name in the run, and no initializer looks side-effecting (a call)."""
    names = set()
    for st in run:
        for v in _var_decls_in_stmt(st):
            names.add(v.spelling)
    for st in run:
        txt = _stmt_src(src, st)
        rhs = txt.split("=", 1)[1] if "=" in txt else ""
        if "(" in rhs:               # a call in the initializer — side effects, don't reorder
            return False
        own = {v.spelling for v in _var_decls_in_stmt(st)}
        for tok in _ident_tokens(rhs):
            if tok in names and tok not in own:
                return False
    return True


def permute_decl_variants(path: Path, leaf: str, limit: int = 200):
    tu, src = _parse(path)
    func = _find_func(tu, leaf)
    if func is None:
        return []
    body, top = _top_stmts(func)
    out = []
    for run in _decl_runs(top):
        if len(run) > 8 or not _independent(src, run):
            continue
        s0 = _off(run[0].extent.start)
        s1 = _off(run[-1].extent.end)
        line_start = src.rfind("\n", 0, s0) + 1
        indent = src[line_start:s0]
        if indent.strip():
            indent = ""
        texts = [_stmt_src(src, st) for st in run]
        idxs = list(range(len(run)))
        for perm in itertools.permutations(idxs):
            if list(perm) == idxs:
                continue
            block = ("\n" + indent).join(texts[i] for i in perm)
            out.append((f"permdecl@{s0:x}{perm}", src[:s0] + block + src[s1:]))
            if len(out) >= limit:
                return out
    return out


def pressure_variants(path: Path, leaf: str, limit: int = 16):
    """Inject a live dummy local at the top of the body (raises register pressure). The
    value is derived from `this`/an arg so it stays live but is provably discardable; a
    trailing `(void)` keeps it warning-clean. Multiple offsets/ops give the allocator
    different pressure points to react to."""
    tu, src = _parse(path)
    func = _find_func(tu, leaf)
    if func is None:
        return []
    body, top = _top_stmts(func)
    if not top:
        return []
    s0 = _off(top[0].extent.start)
    line_start = src.rfind("\n", 0, s0) + 1
    indent = src[line_start:s0]
    if indent.strip():
        indent = ""
    out = []
    # A benign live int derived from a stable pointer identity — kept live to bump pressure,
    # cast to void so it can't be optimized to nothing but doesn't alter observable behavior.
    for k, expr in enumerate(("(int)(unsigned long)this",
                              "*(volatile int*)0 ? 1 : 0")):  # 2nd is a never-run guard shape
        if "volatile" in expr:
            continue  # skip the UB shape; keep only the safe pressure temp
        decl = f"volatile int __perm_pr{k} = {expr}; (void)__perm_pr{k};\n{indent}"
        mutated = src[:s0] + decl + src[s0:]
        out.append((f"pressure{k}", mutated))
        if len(out) >= limit:
            break
    return out


def all_reg_variants(path: Path, leaf: str):
    return permute_decl_variants(path, leaf) + pressure_variants(path, leaf)


if __name__ == "__main__":
    import sys
    p = Path(sys.argv[1]); leaf = sys.argv[2]
    pv = permute_decl_variants(p, leaf)
    pr = pressure_variants(p, leaf)
    print(f"permute_decl: {len(pv)} variants; pressure: {len(pr)} variants")
    for lbl, _ in pv[:10]:
        print("  ", lbl)
