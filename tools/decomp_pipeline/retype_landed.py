#!/usr/bin/env python3
"""Retype landed functions onto the shared engine headers WITHOUT changing their bytes.

    python retype_landed.py --class CPlayerGui                 # dry run, report only
    python retype_landed.py --class CPlayerGui --apply         # overwrite files that stay byte-exact
    python retype_landed.py --class A --class B --limit 20
    python retype_landed.py --addr 0088e920 --apply

For every trusted, genuine landed function of a class whose `rebuild/include/engine/<Class>.h`
exists (and is not quarantined), replace the file's throwaway local struct
(`struct T { char pad[0x1a8]; Sub* sub; ... }`) with `#include "engine/<Class>.h"` and the real
PDB member names, then prove the rewritten TU still compiles to the SAME retail bytes
(MATCH stays MATCH, RELOCATION_MATCH stays RELOCATION_MATCH) and its behaviour test still
passes. Only then is the file overwritten (with --apply). Every decision is logged to
rebuild/backlog/retype_log.tsv with a reason code, so the misses are a worklist, not noise.

Reason codes: NO_HEADER, QUARANTINED, SUSPECT_FILE, NOT_GENUINE, NO_THIS_STRUCT, VIRTUAL_THIS,
AMBIG_ALIGN, NO_EXACT_MAP(<field>), PARTIAL_SUB(<field>), AMBIGUOUS_MEMBER(<name>),
ALREADY_TYPED, BASELINE_FAIL, PARITY_CHANGED(<old>-><new>), BEHAV_FAIL(<status>),
RETYPED (dry run) / APPLIED.
"""
from __future__ import annotations

import argparse
import csv
import re
import sys
from dataclasses import dataclass
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import class_struct  # noqa: E402
import label_trust  # noqa: E402
import verify_and_land as v  # noqa: E402

ROOT = v.ROOT
ENGINE = ROOT / "rebuild" / "include" / "engine"
CATALOG = ROOT / "rebuild" / "build_candidates.ps1"
ORACLE = ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv"
LOG = ROOT / "rebuild" / "backlog" / "retype_log.tsv"
WORK = v.WORK_ROOT / "retype"

HDR_MEMBER_RE = re.compile(r"^\s+(.+?)\s+([A-Za-z_]\w*)((?:\[[^\]]+\])*);\s*// \+0x([0-9a-f]+)(.*)$")
FLAG_LADDER = [["/O2", "/Oy", "/W3"], ["/O2", "/Oy-", "/W3"], ["/O2", "/Oy-", "/G7", "/W3"],
               ["/O2", "/Os", "/W3"], ["/O1", "/Oy", "/W3"], ["/O1", "/Oy-", "/W3"]]
EXTRA_LADDER = [[], ["/GS"], ["/Oa"], ["/GS", "/Oa"]]
SCALAR_POINTEES = {"void", "char", "unsigned char", "wchar_t", "int", "long", "unsigned long",
                   "float", "short", "unsigned short", "bool", "unsigned", "unsigned int", "double"}


@dataclass
class HdrMember:
    off: int
    size: int
    ctype: str
    name: str
    suffix: str
    kind: str      # ptr / int / float / opaque / vptr / pad


def parse_header(cls: str) -> tuple[list[HdrMember], int] | None:
    p = ENGINE / f"{cls}.h"
    if not p.exists():
        return None
    text = p.read_text(encoding="utf-8")
    m = re.search(r"FABLE_STATIC_ASSERT\(sizeof\(" + re.escape(cls) + r"\) == 0x([0-9a-f]+)\)", text)
    size = int(m.group(1), 16) if m else 0
    members: list[HdrMember] = []
    body = text[text.index("{") + 1: text.index("\n};")]
    for line in body.splitlines():
        mm = HDR_MEMBER_RE.match(line)
        if not mm:
            continue
        ctype, name, suffix = mm.group(1).strip(), mm.group(2), mm.group(3)
        off, rest = int(mm.group(4), 16), mm.group(5)
        n = 1
        for a in re.findall(r"\[([^\]]+)\]", suffix):
            n *= int(a, 0)
        if name.startswith("_pad_") or name.startswith("_base_"):
            kind, sz = "pad", n
        elif name == "__vftable":
            kind, sz = "vptr", 4
        elif ctype == "unsigned char" and suffix and rest.strip() and not rest.strip().startswith("retail-only"):
            kind, sz = "opaque", n
        elif ctype.endswith("*"):
            kind, sz = "ptr", 4 * n
        elif ctype in ("float", "double"):
            kind, sz = "float", (8 if ctype == "double" else 4) * n
        else:
            kind, sz = "int", class_struct.type_size(ctype) * n
        members.append(HdrMember(off, sz, ctype, name, suffix, kind))
    return members, size


def catalog_block(addr: str) -> dict:
    text = CATALOG.read_text(encoding="utf-8")
    m = re.search(r"\[pscustomobject\]@\{\s*Address = '" + addr + r"'(.*?)\n\s*\}", text, re.S)
    out = {"flags": None, "qfe": False, "pattern": "PASS"}
    if not m:
        return out
    blk = m.group(1)
    f = re.search(r"CompilerFlags = '([^']*)'", blk)
    if f:
        out["flags"] = f.group(1).split()
    out["qfe"] = "Compiler = 'qfe4035'" in blk
    pp = re.search(r"PassPattern = '([^']*)'", blk)
    if pp:
        out["pattern"] = pp.group(1)
    return out


def oracle_bytes() -> dict[str, bytes]:
    rows = csv.DictReader(open(ORACLE, encoding="utf-8-sig"), delimiter="\t")
    return {r["address"].lower(): bytes.fromhex(r["bytes"]) for r in rows}


def local_kind(typ: str) -> str:
    t = typ.strip()
    if "*" in t:
        return "ptr"
    if t in ("float", "double"):
        return "float"
    return "int"


def word_sub(text: str, old: str, new: str) -> str:
    return re.sub(r"\b" + re.escape(old) + r"\b", new, text)


class Transform:
    def __init__(self, cls: str, hdr: list[HdrMember], hdr_size: int):
        self.cls, self.hdr, self.hdr_size = cls, hdr, hdr_size
        self.by_off = {m.off: m for m in hdr if m.kind != "pad"}
        self.reason = ""

    def fail(self, why: str):
        self.reason = why
        return None

    def run(self, src: str) -> tuple[str, dict] | None:
        """Return (new_source, info) or None with self.reason set."""
        if '#include "engine/' + self.cls + '.h"' in src:
            return self.fail("ALREADY_TYPED")
        pick = class_struct.choose_this_struct(src, self.cls)
        if pick is None:
            # no struct at all = the function never touches `this` fields (constant-return
            # stubs, pure forwarders): nothing to retype, not a defect.
            return self.fail("NO_STRUCT" if not class_struct.STRUCT_RE.search(src) else "NO_THIS_STRUCT")
        tname, body, start = pick
        if "virtual" in body:
            return self.fail("VIRTUAL_THIS")
        packed = class_struct.is_packed(src, start)
        fields, total = class_struct.parse_struct(body, packed=True)
        if not packed:
            nat, _ = class_struct.parse_struct(body, packed=False)
            if nat != fields:
                return self.fail("AMBIG_ALIGN")
        if any(f[0] >= self.hdr_size for f in fields):
            return self.fail("NO_EXACT_MAP(beyond sizeof 0x%x)" % self.hdr_size)
        # pad/vtable-named local members are dropped by parse_struct; if the code still
        # references one (`self->vtbl[...]`, `self->pad0`) the header cannot express it
        # (its vptr is a plain void*), so leave the file for the vtable-typing pass.
        for mm in class_struct.MEMBER_RE.finditer(body):
            pname = mm.group(2)
            if class_struct.is_pad(pname) and re.search(r"(->|\.)\s*" + re.escape(pname) + r"\b", src):
                return self.fail("VTABLE_TYPED(%s)" % pname if pname.lower().lstrip("m_").startswith(("vt", "vf", "_vf", "vptr", "vtable"))
                                 else "PAD_REFERENCED(%s)" % pname)
        # every named local field must map exactly onto a header leaf
        renames: dict[str, str] = {}
        sub_renames: dict[str, str] = {}
        other_structs = {m.group(1): m.group(2) for m in class_struct.STRUCT_RE.finditer(src)
                         if m.group(1) != tname}
        other_members: dict[str, set] = {}
        for sname, sbody in other_structs.items():
            for mm in class_struct.MEMBER_RE.finditer(sbody):
                other_members.setdefault(mm.group(2), set()).add(sname)
        for off, typ, name, size in fields:
            h = self.by_off.get(off)
            if h is None or h.kind == "opaque":
                return self.fail("NO_EXACT_MAP(+0x%x %s)" % (off, name))
            lk = local_kind(typ)
            if h.kind == "vptr":
                if lk != "ptr" or size != 4:
                    return self.fail("NO_EXACT_MAP(+0x%x %s vptr)" % (off, name))
                renames[name] = h.name
                continue
            if h.size != size or lk != h.kind:
                return self.fail("NO_EXACT_MAP(+0x%x %s %s vs %s)" % (off, name, typ, h.ctype))
            # signedness differences (char vs bool) are deliberately NOT rejected: a store-only
            # accessor compiles identically either way and the parity gate is the judge.
            if lk == "ptr":
                pointee = typ.replace("*", "").replace("const", "").strip()
                hp = h.ctype.replace("*", "").replace("const", "").strip()
                if pointee in other_structs or pointee not in SCALAR_POINTEES:
                    if hp == "void":
                        return self.fail("PARTIAL_SUB(+0x%x %s -> void*)" % (off, name))
                    if hp != pointee:
                        if hp in other_structs or hp == tname:
                            return self.fail("PARTIAL_SUB(+0x%x %s->%s name clash)" % (off, pointee, hp))
                        sub_renames[pointee] = hp
            if name != h.name and name in other_members:
                return self.fail("AMBIGUOUS_MEMBER(%s)" % name)
            renames[name] = h.name
        # method idiom?
        stmts = [s.strip() for s in body.split(";") if s.strip()]
        methods = [s for s in stmts if "(" in s and not class_struct.MEMBER_RE.fullmatch(s + ";")]
        # method idiom = the struct declares methods AND the code (not a comment) defines
        # `T::Method(`. Header comments like `// CGameScriptInterface::Foo @ 0x...` must not count.
        code_only = re.sub(r"//[^\n]*", "", re.sub(r"/\*.*?\*/", "", src, flags=re.S))
        method_idiom = bool(methods) and bool(
            re.search(r"\b" + re.escape(tname) + r"::~?\w+\s*\(", code_only))
        new = src
        # 1. remove the this-struct declaration
        decl_re = re.compile(r"struct\s+" + re.escape(tname) + r"\s*\{.*?\}\s*;[ \t]*\n?", re.S)
        m = decl_re.search(new, max(0, start - 1))
        if not m:
            return self.fail("NO_THIS_STRUCT")
        insert_at = m.start()
        new = new[:m.start()] + new[m.end():]
        # 2. shim or type rename. Rename the qualifier FIRST, then insert the shim (whose base
        #    must stay the real class name), then any leftover `T` -> class.
        shim_name = self.cls + "_Methods"
        if method_idiom:
            if not methods:
                return self.fail("NO_THIS_STRUCT")
            new = re.sub(r"\b" + re.escape(tname) + r"::", shim_name + "::", new)
            if tname != self.cls:
                new = word_sub(new, tname, self.cls)
            shim = ("struct %s : %s {\n" % (shim_name, self.cls)
                    + "".join("    %s;\n" % s for s in methods) + "};\n")
            new = new[:insert_at] + shim + new[insert_at:]
        elif tname != self.cls:
            new = word_sub(new, tname, self.cls)
        # 3. nested pointee renames
        for old, nw in sub_renames.items():
            new = word_sub(new, old, nw)
        # 4. member renames: `->x` / `.x` everywhere; bare `x` only in the method idiom (implicit
        #    this), and only when `x` is not also declared as a local/parameter/global.
        for old, nw in renames.items():
            if old == nw:
                continue
            new = re.sub(r"(->|\.)\s*" + re.escape(old) + r"\b", r"\g<1>" + nw, new)
            if method_idiom:
                bare = re.compile(r"(?<![\w>.])\b" + re.escape(old) + r"\b")
                if bare.search(new):
                    if re.search(r"[\w\*&>\]]\s+" + re.escape(old) + r"\s*[;,)=\[]", new):
                        return self.fail("AMBIGUOUS_MEMBER(%s)" % old)
                    new = bare.sub(nw, new)
        # 5. include line after leading comments / pragma optimize
        lines = new.splitlines()
        i = 0
        while i < len(lines) and (lines[i].startswith("//") or lines[i].startswith("#pragma optimize")
                                  or not lines[i].strip()):
            i += 1
        lines.insert(i, '#include "engine/%s.h"  // retyped onto the PDB layout; byte parity re-verified' % self.cls)
        new = "\n".join(lines) + ("\n" if src.endswith("\n") else "")
        return new, {"this": tname, "idiom": "method" if method_idiom else "free",
                     "renames": renames, "subs": sub_renames,
                     "shim": shim_name if method_idiom else ""}

    def run_test(self, tst: str, info: dict) -> str:
        """Mirror the transform in the paired test when it repeats the struct."""
        tname = info["this"]
        decl_re = re.compile(r"struct\s+" + re.escape(tname) + r"\s*\{(.*?)\}\s*;[ \t]*\n?", re.S)
        m = decl_re.search(tst)
        if not m:
            return tst
        body = m.group(1)
        new = tst[:m.start()] + tst[m.end():]
        insert_at = m.start()
        if info["idiom"] == "method":
            stmts = [s.strip() for s in body.split(";") if s.strip()]
            methods = [s for s in stmts if "(" in s and not class_struct.MEMBER_RE.fullmatch(s + ";")]
            new = re.sub(r"\b" + re.escape(tname) + r"::", info["shim"] + "::", new)
            if tname != self.cls:
                new = word_sub(new, tname, self.cls)
            shim = ("struct %s : %s {\n" % (info["shim"], self.cls)
                    + "".join("    %s;\n" % s for s in methods) + "};\n")
            new = new[:insert_at] + shim + new[insert_at:]
        elif tname != self.cls:
            new = word_sub(new, tname, self.cls)
        for old, nw in info["subs"].items():
            new = word_sub(new, old, nw)
        for old, nw in info["renames"].items():
            if old != nw:
                new = re.sub(r"(->|\.)\s*" + re.escape(old) + r"\b", r"\g<1>" + nw, new)
        lines = new.splitlines()
        i = 0
        while i < len(lines) and (lines[i].startswith("//") or not lines[i].strip()):
            i += 1
        lines.insert(i, '#include "engine/%s.h"' % self.cls)
        return "\n".join(lines) + ("\n" if tst.endswith("\n") else "")


def find_baseline(src: str, addr: str, leaf: str, retail: bytes, work: Path, e, cat: dict):
    """Reproduce the landed file's parity. Returns (status, flags, extra, extractor) or None."""
    ladders = ([cat["flags"]] if cat["flags"] else []) + FLAG_LADDER
    extractors = [None]
    try:
        import verify_land_jumptable as jt
        extractors.append(jt.raw_obj_text)
    except Exception:
        pass
    for ext in extractors:
        for flags in ladders:
            for extra in EXTRA_LADDER:
                st, _, _ = v.parity_of(src, addr, leaf, retail, work, e, extra=extra,
                                       base_flags=flags, text_extractor=ext)
                if st.startswith(("MATCH", "RELOCATION_MATCH")):
                    return st, flags, extra, ext
    return None


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--class", dest="classes", action="append", default=[])
    ap.add_argument("--addr", action="append", default=[])
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--limit", type=int, default=0)
    ap.add_argument("--all", action="store_true",
                    help="every class with an OK header in rebuild/include/engine/INDEX.tsv")
    a = ap.parse_args()
    if a.all:
        idx_path = ENGINE / "INDEX.tsv"
        for line in idx_path.read_text(encoding="utf-8").splitlines()[1:]:
            p = line.split("\t")
            if len(p) > 4 and p[4] == "OK" and p[0] not in a.classes:
                a.classes.append(p[0])
    mf = label_trust.Manifest()
    orc = oracle_bytes()
    WORK.mkdir(parents=True, exist_ok=True)
    LOG.parent.mkdir(parents=True, exist_ok=True)
    new_log = not LOG.exists()
    logf = open(LOG, "a", encoding="utf-8", newline="")
    if new_log:
        logf.write("address\tclass\tfile\taction\tbaseline\tnew_status\tbehaviour\treason\n")
    index = {}
    idx = ENGINE / "INDEX.tsv"
    if idx.exists():
        for line in idx.read_text(encoding="utf-8").splitlines()[1:]:
            p = line.split("\t")
            index[p[0]] = p
    rows = []
    for ad in a.addr:
        r = mf.by_addr.get(ad.lower().zfill(8))
        if r:
            rows.append(r)
    for cls in a.classes:
        rows.extend(mf.trusted_landed_for(cls))
    if a.limit:
        rows = rows[:a.limit]
    counts: dict[str, int] = {}
    orig_cl, orig_bin = v.CL_EXE, v.CC_BIN
    hdr_cache: dict = {}
    for r in rows:
        addr = r["address"].lower().zfill(8)
        cls = mf.trusted_module(addr) or r["module"]
        src_path = Path(r["compiled_source"])
        baseline = new_status = beh = ""

        def log(action, reason):
            counts[reason.split("(")[0]] = counts.get(reason.split("(")[0], 0) + 1
            logf.write("\t".join([addr, cls, src_path.name, action, baseline, new_status, beh, reason]) + "\n")
            print("%s %-8s %-44s %s" % (addr, action, reason, src_path.name))

        if not src_path.exists():
            log("skip", "MISSING_FILE"); continue
        if (ENGINE / "_quarantine" / (cls + ".h")).exists():
            log("skip", "QUARANTINED"); continue
        if not (ENGINE / (cls + ".h")).exists():
            log("skip", "NO_HEADER"); continue
        note = index.get(cls, ["", "", "", "", "", ""])[5]
        if src_path.name in note:
            log("skip", "SUSPECT_FILE"); continue
        src = src_path.read_text(encoding="utf-8")
        if not label_trust.is_genuine(src_path):
            log("skip", "NOT_GENUINE"); continue
        if cls not in hdr_cache:
            hdr_cache[cls] = parse_header(cls)
        hdr, hsize = hdr_cache[cls]
        tr = Transform(cls, hdr, hsize)
        res = tr.run(src)
        if res is None:
            log("skip", tr.reason); continue
        new_src, info = res
        rel = src_path.relative_to(ROOT / "rebuild" / "src" / "compiled")
        test_path = (ROOT / "rebuild" / "tests" / rel).with_name(src_path.stem + "_test.cpp")
        tst = test_path.read_text(encoding="utf-8") if test_path.exists() else None
        new_tst = tr.run_test(tst, info) if tst is not None else None
        retail = orc.get(addr)
        if retail is None:
            log("skip", "NO_ORACLE"); continue
        cat = catalog_block(addr)
        if cat["qfe"]:
            try:
                v.use_qfe()
            except FileNotFoundError:
                log("skip", "QFE_TOOLSET_MISSING"); continue
        else:
            v.CL_EXE, v.CC_BIN = orig_cl, orig_bin
        e = v.env()
        leaf = label_trust.Manifest.leaf(r["name"])
        work = WORK / addr
        work.mkdir(parents=True, exist_ok=True)
        base = find_baseline(v.vc71(src), addr, leaf, retail, work, e, cat)
        if base is None:
            log("skip", "BASELINE_FAIL"); continue
        baseline, flags, extra, ext = base
        st, _, _ = v.parity_of(v.vc71(new_src), addr, leaf, retail, work, e, extra=extra,
                               base_flags=flags, text_extractor=ext)
        new_status = st
        if st != baseline:
            log("skip", "PARITY_CHANGED(%s->%s)" % (baseline, st)); continue
        test_kept = False
        if new_tst is not None:
            beh = v.behaviour_of(v.vc71(new_tst), addr, cat["pattern"], work, e, work / (addr + ".obj"))
            if beh != "PASS" and new_tst != tst:
                # the mirrored test did not survive (its helper types differ from the source's);
                # the original self-contained test is still a valid oracle for the retyped source.
                beh2 = v.behaviour_of(v.vc71(tst), addr, cat["pattern"], work, e, work / (addr + ".obj"))
                if beh2 == "PASS":
                    beh, new_tst, test_kept = "PASS", tst, True
            if beh != "PASS":
                log("skip", "BEHAV_FAIL(%s)" % beh); continue
        else:
            beh = "n/a"
        suffix = "(test kept)" if test_kept else ""
        if a.apply:
            src_path.write_text(new_src, encoding="utf-8")
            if new_tst is not None and new_tst != tst:
                test_path.write_text(new_tst, encoding="utf-8")
            log("applied", "APPLIED" + suffix)
        else:
            log("retype", "RETYPED" + suffix)
    v.CL_EXE, v.CC_BIN = orig_cl, orig_bin
    logf.close()
    print("\nsummary:", dict(sorted(counts.items(), key=lambda kv: -kv[1])))
    return 0


if __name__ == "__main__":
    sys.exit(main())
