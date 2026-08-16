#!/usr/bin/env python3
"""Register-allocation / scheduling mutation levers for the full-regalloc permuter.

`clang_mutations.py` supplies the *materialize-earlier* levers (temp introduction,
operand reassociation, decl splitting). This module supplies the complementary set
that upstream decomp-permuter relies on to cross pure regalloc/scheduling plateaus --
the moves that change WHICH register holds a value or WHEN it dies, without touching
control flow or semantics:

  inline_temp_variants   -- fold a single-use local `T t = expr;` back into its one use
                            (the INVERSE of temp-intro: forces the value to be
                            re-materialized at the use site / re-read from memory).
  reorder_decl_variants  -- swap two adjacent, independent local declarations (changes
                            stack-slot assignment => graph-coloring order).
  ins_block_variants     -- wrap a run of statements in `{ }` so the locals declared
                            inside die at the brace (enables stack-slot reuse / different
                            spill choices).
  retype_local_variants  -- change a scalar local's integer type among value-equivalent
                            widths/signedness (int/unsigned/long/size_t/short/char),
                            which flips signed-vs-unsigned compares and extension codegen.

Every variant is semantics-preserving *by construction* for the guarded shapes; any
resulting byte-match is still re-checked by the behaviour test at land time. Parsing
targets MSVC (i686) so __fastcall/__cdecl/__int types resolve.
"""
from __future__ import annotations
import clang.cindex as ci
from pathlib import Path

# Reuse the parse/normalize plumbing so libclang byte offsets align with `src`.
from clang_mutations import _parse, _find_func, _body, _off  # type: ignore

_INT_TYPES = [
    "int", "unsigned int", "unsigned", "long", "unsigned long",
    "short", "unsigned short", "size_t", "ptrdiff_t",
    "__int32", "unsigned __int32",
]
# Normalized spellings libclang may report for the above.
_INT_CANON = {
    "int": "int", "unsigned int": "unsigned int", "unsigned": "unsigned int",
    "long": "long", "unsigned long": "unsigned long", "long int": "long",
    "unsigned long int": "unsigned long", "short": "short", "short int": "short",
    "unsigned short": "unsigned short", "size_t": "unsigned int",
    "unsigned __int32": "unsigned int", "__int32": "int",
}


def _top_stmts(func):
    body = _body(func)
    if body is None:
        return None, []
    return body, list(body.get_children())


def _stmt_src(src, st):
    return src[_off(st.extent.start):_off(st.extent.end)]


def _var_decls_in_stmt(st):
    """VAR_DECL cursors directly under a DECL_STMT (usually one)."""
    if st.kind != ci.CursorKind.DECL_STMT:
        return []
    return [c for c in st.get_children() if c.kind == ci.CursorKind.VAR_DECL]


def _refs_to(func, vardecl):
    """All DECL_REF_EXPR cursors in `func` that reference `vardecl`."""
    want = vardecl.hash
    out = []

    def walk(c):
        if c.kind == ci.CursorKind.DECL_REF_EXPR:
            r = c.referenced
            if r is not None and r.hash == want:
                out.append(c)
        for ch in c.get_children():
            walk(ch)

    walk(func)
    return out


def _init_expr(vardecl):
    """The initializer sub-expression of a VAR_DECL (last child that is an expr)."""
    kids = [c for c in vardecl.get_children()
            if c.kind not in (ci.CursorKind.TYPE_REF, ci.CursorKind.NAMESPACE_REF,
                              ci.CursorKind.TEMPLATE_REF)]
    return kids[-1] if kids else None


def inline_temp_variants(path: Path, leaf: str, limit: int = 40):
    """Fold single-use `T t = expr;` locals back into their one use site."""
    tu, src = _parse(path)
    func = _find_func(tu, leaf)
    if func is None:
        return []
    body, top = _top_stmts(func)
    out = []
    for st in top:
        for vd in _var_decls_in_stmt(st):
            init = _init_expr(vd)
            if init is None:
                continue
            refs = _refs_to(func, vd)
            # exactly one *use* (the DECL_REF at the decl site is not a ref);
            # require precisely one reference in the whole function body.
            if len(refs) != 1:
                continue
            use = refs[0]
            us, ue = _off(use.extent.start), _off(use.extent.end)
            is_, ie = _off(init.extent.start), _off(init.extent.end)
            ss, se = _off(st.extent.start), _off(st.extent.end)
            # the use must come after the declaration statement
            if us < se:
                continue
            expr = src[is_:ie].strip()
            if not expr or "\n" in src[ss:se]:
                continue
            # delete the decl statement (and its trailing newline/indent), then
            # replace the single use with (expr).
            del_start = src.rfind("\n", 0, ss) + 1  # line start (drop indentation)
            del_end = se
            # consume one trailing newline if the whole line was the decl
            if del_end < len(src) and src[del_end] == "\n":
                del_end += 1
            # build via two edits applied high-offset-first
            edits = [(us, ue, f"({expr})"), (del_start, del_end, "")]
            edits.sort(reverse=True)
            mutated = src
            for a, b, rep in edits:
                mutated = mutated[:a] + rep + mutated[b:]
            out.append((f"inline[{vd.spelling} = {expr[:32]}]", mutated))
            if len(out) >= limit:
                return out
    return out


def reorder_decl_variants(path: Path, leaf: str, limit: int = 40):
    """Swap adjacent independent local declarations (stack-slot coloring order)."""
    tu, src = _parse(path)
    func = _find_func(tu, leaf)
    if func is None:
        return []
    body, top = _top_stmts(func)
    out = []
    for i in range(len(top) - 1):
        a, b = top[i], top[i + 1]
        avs, bvs = _var_decls_in_stmt(a), _var_decls_in_stmt(b)
        if not avs or not bvs:
            continue
        anames = {v.spelling for v in avs}
        bnames = {v.spelling for v in bvs}
        atext, btext = _stmt_src(src, a), _stmt_src(src, b)
        # independence: neither initializer mentions the other's declared names
        if any(n in btext for n in anames) or any(n in atext for n in bnames):
            continue
        as_, ae = _off(a.extent.start), _off(a.extent.end)
        bs_, be = _off(b.extent.start), _off(b.extent.end)
        if ae > bs_:
            continue
        gap = src[ae:bs_]  # whitespace/newline between the two statements
        mutated = src[:as_] + btext + gap + atext + src[be:]
        out.append((f"reorder[{'/'.join(anames)} <> {'/'.join(bnames)}]", mutated))
        if len(out) >= limit:
            break
    return out


def _writes_of(stmt_text):
    """Identifiers written by a statement: the base of any assignment/`++`/`--` LHS.
    For `*p=`, `p[i]=`, `p->m=` the *base pointer* identifier is treated as written
    (a store through it). Best-effort, textual."""
    import re
    writes = set()
    # assignment LHS (top-level '=' not '==', '!=', '<=', '>=')
    for m in re.finditer(r"([^;{}]+?)(?<![=!<>+\-*/&|^%])=(?!=)", stmt_text):
        lhs = m.group(1)
        ids = re.findall(r"[A-Za-z_]\w*", lhs)
        if ids:
            writes.add(ids[0])  # base identifier
    # ++/-- targets
    for m in re.finditer(r"(?:\+\+|--)\s*([A-Za-z_]\w*)|([A-Za-z_]\w*)\s*(?:\+\+|--)", stmt_text):
        writes.add(m.group(1) or m.group(2))
    return writes


def reorder_stmt_variants(path: Path, leaf: str, limit: int = 40):
    """Swap two adjacent independent expression statements (instruction scheduling).

    Independence heuristic: neither statement writes an identifier the other mentions.
    Only plain expression statements are eligible (no decls / control flow / calls with
    side effects across each other are still proposed -- the behaviour test guards
    semantics at land time, byte-match alone is never trusted)."""
    tu, src = _parse(path)
    func = _find_func(tu, leaf)
    if func is None:
        return []
    body, top = _top_stmts(func)
    out = []
    # assignments/compound-assignments are BINARY_OPERATOR; member calls are CALL_EXPR.
    EXPR = {ci.CursorKind.BINARY_OPERATOR, ci.CursorKind.CALL_EXPR,
            ci.CursorKind.UNARY_OPERATOR}
    for i in range(len(top) - 1):
        a, b = top[i], top[i + 1]
        if a.kind not in EXPR or b.kind not in EXPR:
            continue
        atext, btext = _stmt_src(src, a), _stmt_src(src, b)
        if "\n" in atext or "\n" in btext:
            continue
        aw, bw = _writes_of(atext), _writes_of(btext)
        aids, bids = set(_ident_tokens(atext)), set(_ident_tokens(btext))
        if (aw & bids) or (bw & aids) or (aw & bw):
            continue
        as_, ae = _off(a.extent.start), _off(a.extent.end)
        bs_, be = _off(b.extent.start), _off(b.extent.end)
        if ae > bs_:
            continue
        gap = src[ae:bs_]
        mutated = src[:as_] + btext + gap + atext + src[be:]
        out.append((f"swapstmt[{i}<>{i+1}]", mutated))
        if len(out) >= limit:
            break
    return out


def ins_block_variants(path: Path, leaf: str, limit: int = 24):
    """Wrap a run of consecutive statements in a `{ }` scope. Guarded so any locals
    declared inside the run are not referenced after it (else the brace would break
    the program)."""
    tu, src = _parse(path)
    func = _find_func(tu, leaf)
    if func is None:
        return []
    body, top = _top_stmts(func)
    if len(top) < 2:
        return []
    # names declared by each top statement, and names read anywhere after index j
    decl_names = []
    for st in top:
        decl_names.append({v.spelling for v in _var_decls_in_stmt(st)})

    def names_used_after(j):
        used = set()
        for k in range(j + 1, len(top)):
            t = _stmt_src(src, top[k])
            used |= set(_ident_tokens(t))
        return used

    out = []
    n = len(top)
    for i in range(n):
        for j in range(i + 1, n):
            # locals declared within window [i..j]
            win_decls = set()
            for k in range(i, j + 1):
                win_decls |= decl_names[k]
            if not win_decls:
                continue  # nothing to scope -> pointless (skip to keep search tight)
            if win_decls & names_used_after(j):
                continue  # a scoped local escapes -> would not compile
            si = _off(top[i].extent.start)
            sj = _off(top[j].extent.end)
            line_start = src.rfind("\n", 0, si) + 1
            indent = src[line_start:si]
            if indent.strip():
                indent = ""
            window = src[si:sj]
            block = "{\n" + indent + window + "\n" + indent + "}"
            mutated = src[:si] + block + src[sj:]
            out.append((f"block[{i}..{j}]", mutated))
            if len(out) >= limit:
                return out
    return out


def _ident_tokens(text):
    import re
    return re.findall(r"[A-Za-z_]\w*", text)


def retype_local_variants(path: Path, leaf: str, limit: int = 40):
    """Retype a scalar integer local to each value-equivalent integer type."""
    tu, src = _parse(path)
    func = _find_func(tu, leaf)
    if func is None:
        return []
    out = []

    def walk(c):
        if c.kind == ci.CursorKind.VAR_DECL:
            ty = (c.type.spelling or "").strip()
            canon = _INT_CANON.get(ty)
            if canon is not None:
                # locate the type spelling at the start of the decl's source
                ds, de = _off(c.extent.start), _off(c.extent.end)
                dtext = src[ds:de]
                # the declared type is the leading run before the identifier
                name = c.spelling
                idx = dtext.find(name)
                if idx > 0:
                    head = dtext[:idx]
                    for nt in _INT_TYPES:
                        if _INT_CANON.get(nt) is None:
                            continue
                        if nt == ty:
                            continue
                        # value-equivalence guard: only swap among types of the same
                        # canonical signedness+width class OR wider, to avoid changing
                        # results; behaviour test still guards.
                        if _INT_CANON[nt] != canon:
                            continue
                        newhead = head[:head.find(ty)] + nt + head[head.find(ty) + len(ty):] \
                            if ty in head else nt + " "
                        mutated = src[:ds] + newhead + dtext[idx:] + src[de:]
                        out.append((f"retype[{name}: {ty}->{nt}]", mutated))
                        if len(out) >= limit:
                            return
        for ch in c.get_children():
            if len(out) >= limit:
                return
            walk(ch)

    walk(func)
    return out


def all_regalloc_variants(path: Path, leaf: str):
    return (inline_temp_variants(path, leaf)
            + reorder_decl_variants(path, leaf)
            + reorder_stmt_variants(path, leaf)
            + ins_block_variants(path, leaf)
            + retype_local_variants(path, leaf))


if __name__ == "__main__":
    import sys
    p = Path(sys.argv[1]); leaf = sys.argv[2]
    for tag, fn in [("inline", inline_temp_variants), ("reorder", reorder_decl_variants),
                    ("block", ins_block_variants), ("retype", retype_local_variants)]:
        vs = fn(p, leaf)
        print(f"{tag}: {len(vs)} variants")
        for lbl, _ in vs[:12]:
            print("   ", lbl)
