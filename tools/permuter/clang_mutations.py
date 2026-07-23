#!/usr/bin/env python3
"""AST-driven, semantics-preserving source mutations for the permuter (libclang).

The regalloc/scheduling tail is cracked by changing WHEN/WHERE the compiler
materializes values, not by changing control flow. These mutations do exactly that:

  temp_intro_variants  -- hoist a subexpression into a named temporary declared just
                          before its statement (forces the value into a register early).
  reassoc_variants     -- swap operands of a commutative binary operator (+ * & | ^).

Both are guaranteed semantics-preserving; any resulting byte-match is still re-checked
by the behaviour test at land time. Parsing targets MSVC (i686) so __fastcall/__cdecl
and __int types resolve; parse errors from missing engine headers are tolerated (we only
need the target function body's structure).
"""
from __future__ import annotations
import clang.cindex as ci
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
VC_INCLUDE = Path(r"D:\Tools\vc71\include")
_ARGS = ["-x", "c++", "-std=c++03", "--target=i686-pc-windows-msvc",
         "-fms-extensions", "-fms-compatibility", "-fms-compatibility-version=1310",
         f"-isystem{VC_INCLUDE}", f"-I{ROOT/'rebuild'/'include'}"]

_HOISTABLE = {
    ci.CursorKind.BINARY_OPERATOR,
    ci.CursorKind.ARRAY_SUBSCRIPT_EXPR,
    ci.CursorKind.CALL_EXPR,
    ci.CursorKind.CSTYLE_CAST_EXPR,
    ci.CursorKind.MEMBER_REF_EXPR,
}
_COMMUTATIVE = {"+", "*", "&", "|", "^"}


def _parse(path: Path):
    """Parse LF-normalized content via unsaved_files so libclang byte offsets align
    with the `src` string we index into (avoids CRLF/LF offset drift on Windows)."""
    src = path.read_bytes().decode("utf-8", "replace").replace("\r\n", "\n")
    idx = ci.Index.create()
    tu = idx.parse(str(path), args=_ARGS, unsaved_files=[(str(path), src)],
                   options=ci.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)
    return tu, src


def _tu(path: Path):
    return _parse(path)[0]


def _find_func(tu, leaf: str):
    want = leaf.rsplit("::", 1)[-1]
    hits = []
    def walk(c):
        if c.kind in (ci.CursorKind.FUNCTION_DECL, ci.CursorKind.CXX_METHOD) and c.is_definition():
            if c.spelling == want:
                hits.append(c)
        for ch in c.get_children():
            walk(ch)
    walk(tu.cursor)
    return hits[0] if hits else None


def _body(func):
    for ch in func.get_children():
        if ch.kind == ci.CursorKind.COMPOUND_STMT:
            return ch
    return None


def _off(loc):
    return loc.offset


def _binop_operator(text: str) -> str | None:
    """Best-effort: the operator token of a top-level binary op (from its source text)."""
    depth = 0
    for i, ch in enumerate(text):
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        elif depth == 0 and ch in "+*&|^" and 0 < i < len(text) - 1:
            # avoid ++, **, &&, ||, unary
            if text[i-1] not in "+*&|^ \t" or text[i+1] in "+*&|^=":
                continue
            return ch
    return None


def _enclosing_stmt(body_children, start, end):
    """The direct child statement of the function body that fully contains [start,end)."""
    for st in body_children:
        s, e = _off(st.extent.start), _off(st.extent.end)
        if s <= start and end <= e:
            return st
    return None


def _collect(func, src: str):
    body = _body(func)
    if body is None:
        return [], []
    top = list(body.get_children())
    cands = []  # (kind, start, end, type_spelling)
    def walk(c):
        try:
            s, e = _off(c.extent.start), _off(c.extent.end)
        except Exception:
            s = e = None
        if s is not None and e is not None and e > s and c.kind in _HOISTABLE:
            ty = c.type.spelling if c.type else ""
            if ty and ty not in ("void", "") and "<" not in ty and "(" not in ty:
                cands.append((c.kind, s, e, ty))
        for ch in c.get_children():
            walk(ch)
    walk(body)
    return top, cands


def temp_intro_variants(path: Path, leaf: str, limit: int = 40):
    """Yield (label, mutated_source) hoisting one subexpression each into a temp."""
    tu, src = _parse(path)
    func = _find_func(tu, leaf)
    if func is None:
        return []
    top, cands = _collect(func, src)
    out, n = [], 0
    seen = set()
    for kind, s, e, ty in cands:
        st = _enclosing_stmt(top, s, e)
        if st is None:
            continue
        ss = _off(st.extent.start)
        key = (s, e)
        if key in seen:
            continue
        seen.add(key)
        sub = src[s:e]
        if len(sub) < 4 or "\n" in sub:
            continue
        # indentation of the statement line
        line_start = src.rfind("\n", 0, ss) + 1
        indent = src[line_start:ss]
        if indent.strip():
            indent = ""
        tmp = f"__perm_t{n}"
        decl = f"{ty} {tmp} = {sub};\n{indent}"
        mutated = src[:ss] + decl + src[ss:s] + tmp + src[e:]
        out.append((f"temp[{ty} = {sub[:36]}]", mutated))
        n += 1
        if n >= limit:
            break
    return out


def reassoc_variants(path: Path, leaf: str, limit: int = 40):
    tu, src = _parse(path)
    func = _find_func(tu, leaf)
    if func is None:
        return []
    body = _body(func)
    if body is None:
        return []
    out, n = [], 0
    def walk(c):
        nonlocal n
        if n >= limit:
            return
        if c.kind == ci.CursorKind.BINARY_OPERATOR:
            s, e = _off(c.extent.start), _off(c.extent.end)
            text = src[s:e]
            op = _binop_operator(text)
            if op in _COMMUTATIVE:
                kids = list(c.get_children())
                if len(kids) == 2:
                    ls, le = _off(kids[0].extent.start), _off(kids[0].extent.end)
                    rs, re_ = _off(kids[1].extent.start), _off(kids[1].extent.end)
                    if le <= rs:
                        left, right = src[ls:le], src[rs:re_]
                        newexpr = src[ls:le].__class__  # noop
                        mutated = src[:ls] + right + src[le:rs] + left + src[re_:]
                        out.append((f"reassoc[{left[:20]} {op} {right[:20]}]", mutated))
                        n += 1
        for ch in c.get_children():
            walk(ch)
    walk(body)
    return out


def stmt_split_variants(path: Path, leaf: str, limit: int = 20):
    """Split `T x = a OP b;` declarations into `T x = a; x OP= b;` (OP in + - * & | ^).
    Shifts when the RHS operands are materialized -- a distinct regalloc lever from
    temp-introduction. Semantics-preserving for scalar types."""
    tu, src = _parse(path)
    func = _find_func(tu, leaf)
    if func is None:
        return []
    body = _body(func)
    if body is None:
        return []
    out = []
    decl_re = None
    import re as _re
    decl_re = _re.compile(
        r"(?P<indent>[ \t]*)(?P<ty>[A-Za-z_][\w:<>\* ]*?)\s+(?P<var>\w+)\s*=\s*"
        r"(?P<a>[^;=]+?)\s*(?P<op>[-+*&|^])\s*(?P<b>[^;=]+?);")
    for m in decl_re.finditer(src):
        a, b, op = m.group("a").strip(), m.group("b").strip(), m.group("op")
        if any(t in a + b for t in ("++", "--", "?")):
            continue
        indent, ty, var = m.group("indent"), m.group("ty").strip(), m.group("var")
        repl = f"{indent}{ty} {var} = {a};\n{indent}{var} {op}= {b};"
        mutated = src[:m.start()] + repl + src[m.end():]
        out.append((f"split[{var} = {a[:20]} {op} {b[:20]}]", mutated))
        if len(out) >= limit:
            break
    return out


def all_variants(path: Path, leaf: str):
    """Every single-step mutation this module knows."""
    return (temp_intro_variants(path, leaf)
            + reassoc_variants(path, leaf)
            + stmt_split_variants(path, leaf))


if __name__ == "__main__":
    import sys
    p = Path(sys.argv[1]); leaf = sys.argv[2]
    ti = temp_intro_variants(p, leaf); ra = reassoc_variants(p, leaf)
    print(f"temp_intro: {len(ti)} variants; reassoc: {len(ra)} variants")
    for lbl, _ in ti[:20]:
        print("  TI", lbl)
    for lbl, _ in ra[:20]:
        print("  RA", lbl)
