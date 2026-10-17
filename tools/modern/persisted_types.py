"""Step-2 serializer worklist: every type the engine's persistence layer reads or writes.

Evidence for docs/modern/PERSISTED_TYPES.md (+ .tsv). Source: Ego_r.pdb public names
(ghidra_out/egor_pdb_names.tsv), demangled with dbghelp, joined with the PDB layouts
(ghidra_out/struct_layouts_egor.tsv).

Fable persists member by member: CPersistContext::Transfer<T> (save games, defs, text) and
CPersistTraits<T>::TransferBinaryIn/Out (binary streams). So the x64/LP64 hazard is not raw struct
images but the *leaf types* those templates are instantiated with, plus the classes that own a
Transfer*/Save/Load/Persist method. For each we report the width hazards:

  pointer        4 -> 8 bytes on x64 (must never reach a stream; flagged for review)
  long / ulong   4 bytes on Windows x64 (LLP64) but 8 on Linux (LP64): fork uses int32_t/uint32_t
  wchar_t        2 bytes on Windows, 4 on Linux: fork uses char16_t
  size_t-like    container counts (vector/list/map) written as the retail 32-bit width
"""
from __future__ import annotations

import collections
import ctypes
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
NAMES = ROOT / "ghidra_out" / "egor_pdb_names.tsv"
LAYOUTS = ROOT / "ghidra_out" / "struct_layouts_egor.tsv"
OUT_MD = ROOT / "docs" / "modern" / "PERSISTED_TYPES.md"
OUT_TSV = ROOT / "docs" / "modern" / "PERSISTED_TYPES.tsv"

PERSIST_METHODS = re.compile(r"^(Transfer\w*|Persist\w*|Save|Load|SaveToFile|LoadFromFile|Serialise)$")
UNDNAME_NAME_ONLY = 0x1000  # UNDNAME_NAME_ONLY
UNDNAME_COMPLETE = 0x0


def undecorate(sym: str, flags: int = UNDNAME_COMPLETE) -> str:
    buf = ctypes.create_string_buffer(4096)
    n = ctypes.windll.dbghelp.UnDecorateSymbolName(sym.encode(), buf, len(buf), flags)
    return buf.value[:n].decode(errors="replace") if n else sym


def split_top(args: str) -> list[str]:
    out, depth, cur = [], 0, ""
    for ch in args:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def clean(t: str) -> str:
    t = re.sub(r"\b(class|struct|enum|union)\s+", "", t)
    t = t.replace(" const", "").replace("const ", "").replace("__ptr64", "")
    return re.sub(r"\s+", " ", t).strip()


def hazard(t: str) -> str:
    t = clean(t)
    out = []
    if "*" in t:
        out.append("pointer")
    if re.search(r"\bwchar_t\b|CWideString", t):
        out.append("wchar_t")
    if re.search(r"\blong\b", t):
        out.append("long")
    if re.match(r"(std::)?(vector|list|map|set|deque|multimap|basic_string|CArray|CVector)\b", t):
        out.append("container-count")
    return "+".join(out)


class Layouts:
    def __init__(self):
        self.size: dict[str, int] = {}
        self.members: dict[str, list[tuple[int, str, str]]] = collections.defaultdict(list)
        for line in LAYOUTS.read_text(errors="replace").splitlines():
            c = line.split("\t")
            if len(c) < 5:
                continue
            if c[0] == "@STRUCT":
                self.size[c[1]] = int(c[2])
            elif c[4] != "_padding_":
                self.members[c[0]].append((int(c[2]), c[3].strip(), c[4]))
                self.size.setdefault(c[0], int(c[1]))

    def counts(self, cls: str) -> collections.Counter:
        out = collections.Counter()
        for _, ty, _ in self.members.get(cls, ()):
            base = re.sub(r"\s*\[\d+\]$", "", ty)
            if base.endswith("*"):
                out["pointer"] += 1
            elif base in ("long", "ulong"):
                out["long"] += 1
            elif base in ("wchar_t", "ushort_wchar") or "CWideString" in base:
                out["wchar_t"] += 1
        return out

    def counts_deep(self, cls: str, depth: int = 0) -> collections.Counter:
        out = collections.Counter()
        for _, ty, _ in self.members.get(cls, ()):
            base = re.sub(r"\s*\[\d+\]$", "", ty).strip()
            if base.endswith("*"):
                out["pointer"] += 1
            elif base in ("long", "ulong"):
                out["long"] += 1
            elif re.match(r"(vector|list|map|set|basic_string)<", base):
                out["stl"] += 1
            elif base in self.members and base != cls and depth < 4:
                out.update(self.counts_deep(base, depth + 1))
        return out


def main() -> None:
    lay = Layouts()
    leaf = collections.defaultdict(set)      # leaf type -> {template family}
    owners = collections.defaultdict(set)    # class -> {persist method}
    n_syms = 0
    for line in NAMES.read_text(errors="replace").splitlines():
        parts = line.split("\t")
        if len(parts) < 2 or not parts[1].startswith("?"):
            continue
        sym = parts[1]
        if "Transfer" not in sym and "Persist" not in sym and "Save" not in sym and "Load" not in sym \
                and "Serialise" not in sym:
            continue
        full = undecorate(sym, UNDNAME_NAME_ONLY)
        n_syms += 1
        # CPersistContext::Transfer<T>, CPersistContext::TransferWithDefault<T>, ...
        m = re.search(r"CPersistContext::(\w+)<(.+)>$", full)
        if m:
            for a in split_top(m.group(2))[:1]:
                leaf[clean(a)].add(f"CPersistContext::{m.group(1)}")
            continue
        m = re.search(r"CPersistTraits<(.+)>::(\w+)$", full)
        if m:
            leaf[clean(m.group(1))].add(f"CPersistTraits::{m.group(2)}")
            continue
        m = re.search(r"(?:^|::)(\w+)::(\w+)$", full)
        if m and PERSIST_METHODS.match(m.group(2)) and m.group(1).startswith("C"):
            owners[m.group(1)].add(m.group(2))

    # Raw struct images: CAFile::ReadVar/WriteVar<T> and CReplaceSerialise<New,Old> (versioned
    # file-format structs, packed; read whole from .lev/.wld map files).
    raw = collections.defaultdict(set)
    for line in NAMES.read_text(errors="replace").splitlines():
        parts = line.split("\t")
        if len(parts) < 2 or not parts[1].startswith("?") or not (
                "ReadVar" in parts[1] or "WriteVar" in parts[1] or "CReplaceSerialise" in parts[1]):
            continue
        full = undecorate(parts[1], UNDNAME_NAME_ONLY)
        m = re.search(r"CAFile::(ReadVar|WriteVar)<(.+)>$", full)
        if m:
            raw[clean(m.group(2))].add(f"CAFile::{m.group(1)}")
            continue
        m = re.search(r"CReplaceSerialise<(.+)>::\w+$", full)
        if m:
            for a in split_top(m.group(1)):
                raw[clean(a)].add("CReplaceSerialise")

    rows = []
    for t in sorted(leaf):
        rows.append(("leaf", t, ",".join(sorted(leaf[t])), hazard(t), lay.size.get(t, ""), "", "", ""))
    for cls in sorted(owners):
        c = lay.counts(cls)
        rows.append(("class", cls, ",".join(sorted(owners[cls])), "", lay.size.get(cls, ""),
                     c["pointer"], c["long"], c["wchar_t"]))
    with OUT_TSV.open("w", encoding="utf-8", newline="\n") as f:
        f.write("kind\ttype\tpersist_entry\thazard\tpdb_size\tpointer_members\tlong_members\twchar_members\n")
        for r in rows:
            f.write("\t".join(str(x) for x in r) + "\n")

    leaves = [r for r in rows if r[0] == "leaf"]
    classes = [r for r in rows if r[0] == "class"]
    hz = collections.Counter(h for r in leaves for h in r[3].split("+") if h)
    L = [
        "# Persisted types (step-2 serializer worklist)",
        "",
        "Generated by `tools/modern/persisted_types.py` from Ego_r.pdb names and layouts. Do not edit by",
        "hand; the full table is `PERSISTED_TYPES.tsv`. Feeds [MODERNIZATION](MODERNIZATION.md) step 2",
        "(\"Persisted layouts\" hazard).",
        "",
        "**Finding.** Saves and defs go member by member through `CPersistContext::Transfer<T>` (and its",
        "relatives) or `CPersistTraits<T>::TransferBinaryIn/Out`, one template instance per leaf type. So",
        "for that path the x64 job is (1) pin the width of every leaf type below to the retail 32-bit",
        "encoding and (2) resolve the pointer leaves (`COpinionDeed *`, `COpinionPersonalityTraits *`),",
        "whose traits must write something other than the address. Class layouts can change freely in the",
        "fork as long as their Transfer methods write the same leaves in the same order.",
        "",
        "**Pointer leaves are resolved (2026-10-16, read from `debug_build/ego_r.exe`): none writes an",
        "address.** `CPersistTraits<COpinionPersonalityTraits *>` serializes the pointee, a 5-element array",
        "of 0x24-byte `COpinionPersonalityTraits` (loop to 0xB4). `CPersistTraits<COpinionDeed *>` writes",
        "the deed's fields (SLONG, SLONG, 3D vector, frame, 2 EBOOL, SLONG) and its target object as a",
        "64-bit UID (`WriteUHUGE` of the intelligent pointer's object +0x18). `std::list<CReactionMatch *>`",
        "writes a count, then per element a type id (vcall +8) followed by the element's own polymorphic",
        "transfer (vcall +0x10). `std::pair<long,COpinionDeed *>` composes the two. The fork keeps these",
        "encodings as written. The only width rule is that counts, SLONGs and UIDs stay 32/32/64-bit.",
        "",
        "Not covered by this table: file loaders outside CPersistContext (`CMap::LoadFromFile`,",
        "`CLandscapeLayerMesh::Save`, `CThingManager::LoadFromFile`, `CWorld::Load`, bank headers).",
        "Their raw struct images are listed in the last section. Each loader's remaining body still needs",
        "a read when its fork serializer is written.",
        "",
        f"{n_syms} persistence symbols read; **{len(leaves)} leaf types** "
        f"({', '.join(f'{v} {k}' for k, v in hz.most_common())} hazards) and "
        f"**{len(classes)} classes** with their own persist methods.",
        "",
        "## Leaf types with a width hazard",
        "",
        "| Type | Hazard | Entry points |",
        "| --- | --- | --- |",
    ]
    for r in leaves:
        if r[3]:
            L.append(f"| `{r[1]}` | {r[3]} | {r[2]} |")
    L += ["", "## Leaf types without a width hazard", "",
          ", ".join(f"`{r[1]}`" for r in leaves if not r[3]), "",
          "## Persisting classes with pointer, long or wchar_t members", "",
          "Pointer members are fine unless a Transfer method writes them; each needs one look at its",
          "Transfer body when the fork's serializer for that class is written.", "",
          "| Class | PDB size | Persist methods | Pointers | long | wchar_t |",
          "| --- | ---: | --- | ---: | ---: | ---: |"]
    for r in sorted(classes, key=lambda r: (-int(r[5] or 0), -int(r[7] or 0), -int(r[6] or 0), r[1])):
        if (r[5] or r[6] or r[7]):
            L.append(f"| `{r[1]}` | {r[4]} | {r[2]} | {r[5]} | {r[6]} | {r[7]} |")
    clean_classes = [r[1] for r in classes if not (r[5] or r[6] or r[7])]
    L += ["", f"## Persisting classes with none of those members ({len(clean_classes)})", "",
          ", ".join(f"`{c}`" for c in clean_classes), ""]
    L += ["## Raw struct images (map/level files)", "",
          "Read and written whole by `CAFile::ReadVar<T>` / `WriteVar<T>`, or by",
          "`CReplaceSerialise<New, Old>`, which reads an old file-format version and upgrades it. Odd sizes",
          "(25, 73 bytes) show these are packed. The fork must keep them byte-identical: `#pragma pack(1)`,",
          "`int32_t` for every `long`, and no pointers (none has a pointer or STL member). Members are",
          "counted through nested structs. PDB sizes are Ego_r's, so check them against retail reads",
          "before relying on them.", "",
          "| Type | PDB size | Via | long | pointer | STL |", "| --- | ---: | --- | ---: | ---: | ---: |"]
    for t in sorted(raw):
        c = lay.counts_deep(t)
        L.append(f"| `{t}` | {lay.size.get(t, '')} | {', '.join(sorted(raw[t]))} | {c['long']} | "
                 f"{c['pointer']} | {c['stl']} |")
    L.append("")
    OUT_MD.write_text("\n".join(L), encoding="utf-8")
    print(f"{len(leaves)} leaf types ({dict(hz)}), {len(classes)} classes")


if __name__ == "__main__":
    main()
