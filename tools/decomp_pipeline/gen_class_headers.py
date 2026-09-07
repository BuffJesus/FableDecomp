#!/usr/bin/env python3
"""Generate compilable, PDB-named class headers for the reconstruction.

    python gen_class_headers.py --classes CGameScriptInterface CPlayerGui
    python gen_class_headers.py --all-trusted          # every class with >=1 trusted landed fn
    python gen_class_headers.py --classes X --compile-check   # also compile the header under VC7.1

Inputs
  ghidra_out/struct_layouts_egor.tsv   donor layouts (Ego_r.pdb via DumpStructLayouts.java):
                                        `@STRUCT name size ncomp` then `name size off type member`.
                                        Base-class subobjects are flattened into `_padding_` rows.
  landed sources                        retail evidence: class_struct.facts_for() over files whose
                                        module label is address-unique (label_trust) and genuine.
Outputs
  rebuild/include/engine/<Class>.h              reconciled, no conflicts
  rebuild/include/engine/_quarantine/<Class>.h  donor/retail disagreement, NOT on the rewriter path
  rebuild/include/engine/RECONCILE.tsv          per-field verdicts with evidence
  rebuild/include/engine/INDEX.tsv              class, size, named members, status

Header contract (see docs/pipeline/... and CONTRIBUTING.md)
  * self-contained: forward decls only, no engine-to-engine includes
  * `#pragma pack(push,1)` POD struct; every byte accounted for; real PDB member names
  * composites/templates are opaque `unsigned char Name[N]` with the real type in a comment
  * compile-time size/offset checks via FABLE_STATIC_ASSERT (rebuild_abi.h); never spell
    lowercase `static_assert` (the verify harness strips those lines from candidates)
"""
from __future__ import annotations

import argparse
import collections
import re
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
DONOR = ROOT / "ghidra_out" / "struct_layouts_egor.tsv"
INCLUDE = ROOT / "rebuild" / "include"
OUT = INCLUDE / "engine"
QUAR = OUT / "_quarantine"
sys.path.insert(0, str(HERE))
import class_struct  # noqa: E402
import label_trust  # noqa: E402

SCALARS = {
    "int": ("int", 4, "int"), "uint": ("unsigned int", 4, "int"),
    "long": ("long", 4, "int"), "ulong": ("unsigned long", 4, "int"),
    "short": ("short", 2, "int"), "ushort": ("unsigned short", 2, "int"),
    "char": ("char", 1, "int"), "uchar": ("unsigned char", 1, "int"),
    "schar": ("signed char", 1, "int"), "byte": ("unsigned char", 1, "int"),
    "bool": ("bool", 1, "int"), "float": ("float", 4, "float"),
    "double": ("double", 8, "float"), "ulong64": ("unsigned __int64", 8, "int"),
    "long64": ("__int64", 8, "int"), "longlong": ("__int64", 8, "int"),
    "ulonglong": ("unsigned __int64", 8, "int"), "DWORD": ("unsigned long", 4, "int"),
    "WORD": ("unsigned short", 2, "int"), "BYTE": ("unsigned char", 1, "int"),
    "wchar_t": ("wchar_t", 2, "int"), "undefined4": ("unsigned long", 4, "int"),
    "undefined2": ("unsigned short", 2, "int"), "undefined1": ("unsigned char", 1, "int"),
    "undefined": ("unsigned char", 1, "int"), "pointer": ("void*", 4, "ptr"),
    "void *": ("void*", 4, "ptr"), "HWND": ("void*", 4, "ptr"), "HANDLE": ("void*", 4, "ptr"),
}
CXX_KEYWORDS = {"class", "default", "new", "delete", "operator", "template", "this", "union",
                "enum", "struct", "register", "auto", "virtual", "explicit", "export", "friend",
                "mutable", "namespace", "typename", "using", "signed", "unsigned", "int", "long",
                "short", "char", "bool", "float", "double", "void", "const", "static", "case",
                "switch", "for", "while", "do", "if", "else", "return", "break", "continue",
                "goto", "try", "catch", "throw", "true", "false", "inline", "typedef", "sizeof"}
IDENT = re.compile(r"^[A-Za-z_]\w*$")
ARRAY_RE = re.compile(r"^(.*?)\[(\d+)\]$")


@dataclass
class DonorMember:
    off: int
    typ: str
    name: str
    span: int = 0


@dataclass
class DonorStruct:
    name: str
    size: int
    members: list[DonorMember] = field(default_factory=list)


@dataclass
class Emitted:
    off: int
    size: int
    ctype: str          # C type text without the name (arrays handled via suffix)
    name: str
    suffix: str = ""    # e.g. "[4]"
    kind: str = "opaque"  # int / float / ptr / opaque / pad / vptr
    comment: str = ""
    is_pad: bool = False


def load_donor(path: Path = DONOR) -> dict[str, DonorStruct]:
    structs: dict[str, DonorStruct] = {}
    dups = collections.Counter()
    cur: DonorStruct | None = None
    with open(path, encoding="utf-8", errors="ignore") as fh:
        for line in fh:
            parts = line.rstrip("\n").split("\t")
            if parts[0] == "@STRUCT":
                name, size = parts[1], int(parts[2])
                cur = DonorStruct(name, size)
                dups[name] += 1
                prev = structs.get(name)
                # duplicate names: keep the definition with the most components later
                if prev is None:
                    structs[name] = cur
                else:
                    cur._pending = True  # type: ignore[attr-defined]
                continue
            if cur is None or len(parts) < 5:
                continue
            cur.members.append(DonorMember(int(parts[2]), parts[3], parts[4]))
            if getattr(cur, "_pending", False):
                prev = structs[cur.name]
                if len(cur.members) > len(prev.members):
                    structs[cur.name] = cur
    for s in structs.values():
        s.members.sort(key=lambda m: m.off)
        for i, m in enumerate(s.members):
            nxt = s.members[i + 1].off if i + 1 < len(s.members) else s.size
            m.span = max(0, nxt - m.off)
    load_donor.dups = {n for n, c in dups.items() if c > 1}  # type: ignore[attr-defined]
    return structs


def sanitize_ident(name: str, used: set[str]) -> str:
    n = re.sub(r"\W", "_", name) or "_unnamed"
    if n[0].isdigit():
        n = "_" + n
    if n in CXX_KEYWORDS:
        n += "_"
    base, i = n, 2
    while n in used:
        n = f"{base}_{i}"; i += 1
    used.add(n)
    return n


def pointee_forward(typ: str) -> str | None:
    """`CWorld *` -> 'CWorld'; `CThing_const_ *` -> 'CThing'; templates/namespaces -> None."""
    t = typ.strip()
    if not t.endswith("*"):
        return None
    base = t[:-1].strip()
    if base.endswith("*"):
        return None  # pointer-to-pointer: handled as void**
    const = base.endswith("_const_")
    if const:
        base = base[:-len("_const_")]
    base = base.strip()
    if base in SCALARS or base == "void":
        return None
    if IDENT.match(base):
        return ("const " if const else "") + base
    return None


def map_member(m: DonorMember, used: set[str], fwd: set[str]) -> list[Emitted]:
    """Map one donor member (with its span) to one or more emitted C members."""
    out: list[Emitted] = []
    typ, span = m.typ.strip(), m.span
    if span <= 0:
        return out
    if m.name == "_padding_" or not m.name:
        out.append(Emitted(m.off, span, "unsigned char", f"_pad_0x{m.off:02x}", f"[0x{span:x}]",
                           "pad", "", True))
        return out
    name = sanitize_ident(m.name, used)
    # bitfields (`bool:1`, `ulong:3`) -> opaque byte group. `::` in template args is not one.
    if re.search(r":\d+$", typ):
        out.append(Emitted(m.off, span, "unsigned char", name, f"[0x{span:x}]", "opaque",
                           f"bitfield group: {typ}"))
        return out
    arr = ARRAY_RE.match(typ)
    count = None
    if arr:
        typ, count = arr.group(1).strip(), int(arr.group(2))
    elt_c = None; elt_size = 0; kind = "opaque"; comment = ""
    if typ in SCALARS:
        elt_c, elt_size, kind = SCALARS[typ]
    elif typ.endswith("*"):
        p = pointee_forward(typ)
        if p:
            elt_c = p + "*"; fwd.add(p.replace("const ", ""))
        else:
            elt_c = "void*"; comment = typ
        elt_size, kind = 4, "ptr"
    if elt_c is None:
        # composite / template / enum-like: opaque bytes, real type in the comment
        out.append(Emitted(m.off, span, "unsigned char", name, f"[0x{span:x}]", "opaque",
                           typ + (f"[{count}]" if count else "")))
        return out
    total = elt_size * (count or 1)
    if total > span:
        # donor member larger than its slot (should not happen) -> opaque
        out.append(Emitted(m.off, span, "unsigned char", name, f"[0x{span:x}]", "opaque",
                           f"{typ} (size mismatch)"))
        return out
    out.append(Emitted(m.off, total, elt_c, name, f"[{count}]" if count else "", kind, comment))
    if total < span:
        out.append(Emitted(m.off + total, span - total, "unsigned char",
                           f"_pad_0x{m.off + total:02x}", f"[0x{span - total:x}]", "pad", "", True))
    return out


def fill_gaps(emitted: list[Emitted], size: int) -> list[Emitted]:
    """Make the member list cover [0, size) exactly once.

    The donor dump omits inherited storage when the class declares no `_padding_` rows for it
    (CDefClassBase's members start at +0x1c), and unions/bitfields put several members at one
    offset. Pads fill every gap; a member that starts inside the previous member is a union
    alternative and is dropped (its name survives in the previous member's comment)."""
    out: list[Emitted] = []
    cur = 0
    for e in sorted(emitted, key=lambda x: (x.off, -x.size)):
        if e.size <= 0:
            continue
        if e.off < cur:
            if out:
                out[-1].comment = (out[-1].comment + f" | union alt: {e.ctype} {e.name}").strip(" |")
            continue
        if e.off > cur:
            out.append(Emitted(cur, e.off - cur, "unsigned char", f"_pad_0x{cur:02x}",
                               f"[0x{e.off - cur:x}]", "pad", "unlisted/inherited storage", True))
        if e.off + e.size > size:
            e = Emitted(e.off, size - e.off, "unsigned char", e.name, f"[0x{size - e.off:x}]",
                        "opaque", (e.comment + " (truncated to sizeof)").strip())
        out.append(e)
        cur = e.off + e.size
    if cur < size:
        out.append(Emitted(cur, size - cur, "unsigned char", f"_pad_0x{cur:02x}", f"[0x{size - cur:x}]",
                           "pad", "unlisted/inherited storage", True))
    return out


def kind_of_retail(typ: str) -> tuple[str, int]:
    t = typ.strip()
    if "*" in t:
        return "ptr", 4
    if t in ("float",):
        return "float", 4
    if t == "double":
        return "float", 8
    return "int", class_struct.type_size(t)


@dataclass
class Reconcile:
    cls: str
    off: int
    donor_type: str
    donor_name: str
    donor_size: int
    retail_type: str
    retail_name: str
    retail_size: int
    verdict: str
    evidence: str


def reconcile(cls: str, donor: DonorStruct, emitted: list[Emitted],
              retail_prov: dict[int, list[tuple[str, str, int, str]]],
              retail_size: int | None) -> tuple[list[Reconcile], list[Emitted]]:
    """Compare retail-landed field facts to the donor layout; return verdicts and the emitted
    list with RETAIL_ONLY fields carved out of padding."""
    rows: list[Reconcile] = []
    if retail_size is not None and retail_size != donor.size:
        rows.append(Reconcile(cls, -1, "", "", donor.size, "sizeof", "GetSizeofClass",
                              retail_size, "SIZE_CONFLICT", "GetSizeofClass"))
    by_off = {e.off: e for e in emitted}
    for off, facts in sorted(retail_prov.items()):
        for rtyp, rname, rsize, rfile in facts:
            rkind, rsz = kind_of_retail(rtyp)
            rsz = rsize or rsz
            if off >= donor.size:
                rows.append(Reconcile(cls, off, "", "", donor.size, rtyp, rname, rsz,
                                      "SIZE_CONFLICT", rfile))
                continue
            host = None
            for e in emitted:
                if e.off <= off < e.off + e.size:
                    host = e; break
            if host is None:
                rows.append(Reconcile(cls, off, "", "", 0, rtyp, rname, rsz, "CONFLICT", rfile))
                continue
            if host.kind == "vptr":
                # Any 4-byte read of slot 0 is the vtable pointer under a local name; agree.
                v = "AGREE" if (rkind == "ptr" and rsz == 4) else ("AGREE_KIND_SOFT" if rsz == 4 else "CONFLICT")
                rows.append(Reconcile(cls, off, host.ctype, host.name, host.size, rtyp, rname, rsz, v, rfile))
                continue
            if host.is_pad:
                if off + rsz <= host.off + host.size:
                    rows.append(Reconcile(cls, off, host.ctype, host.name, host.size, rtyp, rname,
                                          rsz, "RETAIL_ONLY", rfile))
                else:
                    rows.append(Reconcile(cls, off, host.ctype, host.name, host.size, rtyp, rname,
                                          rsz, "CONFLICT", rfile))
                continue
            if host.kind == "opaque":
                v = "CONTAINED" if off + rsz <= host.off + host.size else "CONFLICT"
                rows.append(Reconcile(cls, off, host.comment or host.ctype, host.name, host.size,
                                      rtyp, rname, rsz, v, rfile))
                continue
            # scalar/pointer leaf
            if host.off == off and host.size == rsz:
                if host.kind == rkind:
                    v = "AGREE"
                elif {host.kind, rkind} == {"ptr", "int"}:
                    v = "AGREE_KIND_SOFT"
                else:
                    v = "CONFLICT"
            elif host.suffix and off + rsz <= host.off + host.size and (off - host.off) % max(1, rsz) == 0:
                v = "CONTAINED"  # element of a donor array
            else:
                v = "CONFLICT"
            rows.append(Reconcile(cls, off, host.ctype + host.suffix, host.name, host.size,
                                  rtyp, rname, rsz, v, rfile))
    # carve RETAIL_ONLY fields out of padding (first evidence per offset wins)
    carved: dict[int, Reconcile] = {}
    for r in rows:
        if r.verdict == "RETAIL_ONLY" and r.off not in carved:
            carved[r.off] = r
    if carved:
        new: list[Emitted] = []
        used = {e.name for e in emitted}
        for e in emitted:
            hits = sorted((r for o, r in carved.items() if e.off <= o < e.off + e.size), key=lambda r: r.off)
            if not (e.is_pad or e.kind == "vptr") or not hits:
                new.append(e); continue
            cur = e.off
            for r in hits:
                if r.off < cur:
                    continue
                if r.off > cur:
                    new.append(Emitted(cur, r.off - cur, "unsigned char", f"_pad_0x{cur:02x}",
                                       f"[0x{r.off - cur:x}]", "pad", "", True))
                rk, _ = kind_of_retail(r.retail_type)
                ctype = {"ptr": "void*", "float": "float"}.get(rk, {1: "unsigned char", 2: "unsigned short", 4: "long", 8: "unsigned __int64"}.get(r.retail_size, "unsigned char"))
                if rk == "int" and r.retail_type.strip() in ("bool", "char", "unsigned char", "short", "unsigned short", "int", "unsigned", "unsigned long", "long", "unsigned int"):
                    ctype = r.retail_type.strip()
                nm = sanitize_ident(r.retail_name, used)
                if class_struct.type_size(ctype) != r.retail_size:
                    # retail declared an array (`char sub8[4]`) or an odd width: keep the bytes
                    # exact and the name, but as an opaque run
                    new.append(Emitted(r.off, r.retail_size, "unsigned char", nm,
                                       f"[0x{r.retail_size:x}]", "opaque",
                                       f"retail-only {r.retail_type} ({r.evidence})"))
                else:
                    new.append(Emitted(r.off, r.retail_size, ctype, nm, "", rk,
                                       f"retail-only ({r.evidence})"))
                cur = r.off + r.retail_size
            if cur < e.off + e.size:
                new.append(Emitted(cur, e.off + e.size - cur, "unsigned char", f"_pad_0x{cur:02x}",
                                   f"[0x{e.off + e.size - cur:x}]", "pad", "", True))
        emitted = new
    return rows, emitted


def emit_header(cls: str, donor: DonorStruct, emitted: list[Emitted], fwd: set[str],
                n_retail: int, status: str) -> str:
    guard = "FABLE_ENGINE_" + re.sub(r"\W", "_", cls).upper() + "_H"
    lines = [f"#ifndef {guard}", f"#define {guard}",
             "// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.",
             f"// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof {cls} = 0x{donor.size:x}.",
             f"// Retail evidence: {n_retail} trusted landed function(s); reconciliation {status}.",
             "// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque",
             "// `unsigned char X[N]` members carry their real (composite/template) type in a comment.",
             "#include <stddef.h>", '#include "rebuild_abi.h"', ""]
    for f in sorted(fwd - {cls}):
        lines.append(f"struct {f};")
    if fwd - {cls}:
        lines.append("")
    lines += ["#pragma pack(push, 1)", f"struct {cls} {{"]
    w = max((len(e.ctype) for e in emitted), default=10)
    for e in emitted:
        decl = f"    {e.ctype:<{w}} {e.name}{e.suffix};"
        cm = f"// +0x{e.off:02x}"
        if e.comment:
            cm += f" {e.comment}"
        lines.append(f"{decl:<60} {cm}")
    lines += ["};", "#pragma pack(pop)", "",
              f"FABLE_STATIC_ASSERT(sizeof({cls}) == 0x{donor.size:x});"]
    for e in emitted:
        if not e.is_pad:
            lines.append(f"FABLE_STATIC_ASSERT(offsetof({cls}, {e.name}) == 0x{e.off:x});")
    lines += ["", f"#endif // {guard}", ""]
    return "\n".join(lines)


def hand_owned() -> set[str]:
    names = set()
    for h in INCLUDE.glob("*.h"):
        for m in re.finditer(r"^\s*(?:struct|class)\s+([A-Za-z_]\w*)\s*[{:]", h.read_text(encoding="utf-8", errors="ignore"), re.M):
            names.add(m.group(1))
    return names


def generate(classes: list[str], donor: dict[str, DonorStruct], mf: label_trust.Manifest,
             owned: set[str]):
    OUT.mkdir(parents=True, exist_ok=True); QUAR.mkdir(parents=True, exist_ok=True)
    index_rows = []; rec_rows: list[Reconcile] = []
    written = quarantined = skipped = 0
    for cls in classes:
        if cls in owned:
            index_rows.append((cls, "", 0, 0, "HAND_OWNED", "")); skipped += 1; continue
        d = donor.get(cls)
        if d is None or d.size == 0:
            index_rows.append((cls, "", 0, 0, "NO_DONOR", "")); skipped += 1; continue
        used: set[str] = set(); fwd: set[str] = set(); emitted: list[Emitted] = []
        for m in d.members:
            emitted.extend(map_member(m, used, fwd))
        emitted = fill_gaps(emitted, d.size)
        # leading unnamed dword: vptr or flattened base -> name it
        if emitted and emitted[0].is_pad and emitted[0].off == 0:
            e0 = emitted[0]
            emitted[0] = Emitted(0, 4, "void*", "__vftable", "", "vptr",
                                 "vptr, or first dword of a flattened base subobject")
            if e0.size > 4:
                emitted.insert(1, Emitted(4, e0.size - 4, "unsigned char", "_base_0x04",
                                          f"[0x{e0.size - 4:x}]", "pad",
                                          "flattened base-class subobject(s)", True))
        # retail evidence (trusted + genuine files only)
        files = [r["compiled_source"] for r in mf.trusted_landed_for(cls)]
        files = [f for f in files if Path(f).exists() and label_trust.is_genuine(f)]
        prov: dict[int, list] = {}
        _, rsize, used_n = class_struct.facts_for(cls, files=files, provenance=prov)
        rows, emitted = reconcile(cls, d, emitted, prov, rsize)
        rec_rows.extend(rows)
        bad = [r for r in rows if r.verdict in ("CONFLICT", "SIZE_CONFLICT")]
        bad_files = {r.evidence for r in bad}
        good_files = {r.evidence for r in rows if r.verdict.startswith(("AGREE", "CONTAINED"))} - bad_files
        size_bad = any(r.verdict == "SIZE_CONFLICT" and r.evidence == "GetSizeofClass" for r in bad)
        # Quarantine policy: a class-size disagreement from GetSizeofClass, or conflicting
        # files outnumbering agreeing files, means donor != retail for this class. A minority
        # of conflicting files is far more often a mislabelled function than a layout change;
        # the header is emitted and those files are listed so the rewriter skips them.
        quarantine = size_bad or (bad_files and len(bad_files) >= max(1, len(good_files)))
        status = ("clean" if not bad else
                  f"{len(bad)} conflict(s) in {len(bad_files)} file(s) vs {len(good_files)} agreeing")
        text = emit_header(cls, d, emitted, fwd, used_n, status)
        named = sum(1 for e in emitted if not e.is_pad and e.kind != "vptr")
        if quarantine:
            (QUAR / f"{cls}.h").write_text(text, encoding="utf-8"); quarantined += 1
            index_rows.append((cls, f"_quarantine/{cls}.h", d.size, named, "QUARANTINED", status))
            (OUT / f"{cls}.h").unlink(missing_ok=True)
        else:
            (OUT / f"{cls}.h").write_text(text, encoding="utf-8"); written += 1
            note = f"{used_n} retail files"
            if cls in load_donor.dups:  # type: ignore[attr-defined]
                note += "; dup_donor"
            if bad_files:
                note += "; suspect=" + ",".join(sorted(bad_files))
            index_rows.append((cls, f"{cls}.h", d.size, named, "OK", note))
            (QUAR / f"{cls}.h").unlink(missing_ok=True)
    # merge into existing TSVs (regeneration is per-class)
    def merge_tsv(path: Path, header: list[str], rows: list[tuple], key):
        old = {}
        if path.exists():
            for line in path.read_text(encoding="utf-8").splitlines()[1:]:
                p = line.split("\t")
                old[key(p)] = p
        for r in rows:
            r = [str(x) for x in r]
            old[key(r)] = r
        touched = {key([str(x) for x in r]) for r in rows}
        # drop stale rows for regenerated classes
        regen = {str(r[0]) for r in rows}
        keep = [v for k, v in old.items() if k in touched or v[0] not in regen]
        keep.sort()
        path.write_text("\t".join(header) + "\n" + "\n".join("\t".join(v) for v in keep) + "\n",
                        encoding="utf-8")
    merge_tsv(OUT / "INDEX.tsv", ["class", "header", "size", "named_members", "status", "note"],
              index_rows, key=lambda p: p[0])
    merge_tsv(OUT / "RECONCILE.tsv",
              ["class", "offset", "donor_type", "donor_name", "donor_size", "retail_type",
               "retail_name", "retail_size", "verdict", "evidence"],
              [(r.cls, f"0x{r.off:x}" if r.off >= 0 else "size", r.donor_type, r.donor_name,
                r.donor_size, r.retail_type, r.retail_name, r.retail_size, r.verdict, r.evidence)
               for r in rec_rows],
              key=lambda p: (p[0], p[1], p[6], p[9]))
    verdicts = collections.Counter(r.verdict for r in rec_rows)
    print(f"headers written={written} quarantined={quarantined} skipped={skipped}; "
          f"reconcile verdicts={dict(verdicts)}")
    return written, quarantined


def compile_check(classes: list[str]) -> int:
    """Compile every generated header on its own under VC7.1. A header whose size/offset asserts
    fail is moved to _quarantine (status COMPILE_FAIL) so the rewriter can never use it."""
    import verify_and_land as v
    work = v.WORK_ROOT / "header_smoke"; work.mkdir(parents=True, exist_ok=True)
    e = v.env()
    ok = bad = 0
    failed: dict[str, str] = {}
    for c in classes:
        h = OUT / f"{c}.h"
        if not h.exists():
            continue
        tu = work / "one.cpp"
        tu.write_text(f'#include "engine/{c}.h"\nint main(void){{return 0;}}\n', encoding="utf-8")
        cp = v.cl(["/nologo", "/c", "/W3", f"/Fo{work / 'one.obj'}", str(tu)], e)
        if cp.returncode == 0:
            ok += 1
            continue
        bad += 1
        first = next((l for l in (cp.stdout + cp.stderr).splitlines() if "error" in l), "").strip()
        failed[c] = first[-160:]
        (QUAR / f"{c}.h").write_text(h.read_text(encoding="utf-8"), encoding="utf-8")
        h.unlink()
    if failed:
        idx = OUT / "INDEX.tsv"
        rows = idx.read_text(encoding="utf-8").splitlines()
        out_rows = [rows[0]]
        for line in rows[1:]:
            p = line.split("\t")
            if p[0] in failed:
                p[1] = f"_quarantine/{p[0]}.h"; p[4] = "COMPILE_FAIL"; p[5] = failed[p[0]]
            out_rows.append("\t".join(p))
        idx.write_text("\n".join(out_rows) + "\n", encoding="utf-8")
        for c, err in list(failed.items())[:10]:
            print(f"  COMPILE_FAIL {c}: {err}")
    print(f"compile-check headers ok={ok} quarantined_for_compile_fail={bad}")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--classes", nargs="*", default=[])
    ap.add_argument("--all-trusted", action="store_true",
                    help="every class with at least one trusted landed function")
    ap.add_argument("--min-landed", type=int, default=1)
    ap.add_argument("--compile-check", action="store_true")
    a = ap.parse_args()
    donor = load_donor()
    mf = label_trust.Manifest()
    classes = list(a.classes)
    if a.all_trusted:
        cnt = collections.Counter()
        for r in mf.landed():
            m = mf.trusted_module(r["address"])
            if m:
                cnt[m] += 1
        classes += [c for c, n in cnt.most_common() if n >= a.min_landed and c not in classes]
    if not classes:
        ap.error("give --classes or --all-trusted")
    generate(classes, donor, mf, hand_owned())
    if a.compile_check:
        return compile_check(classes)
    return 0


if __name__ == "__main__":
    sys.exit(main())
