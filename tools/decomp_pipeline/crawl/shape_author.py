#!/usr/bin/env python3
"""Author genuine C++ for whole SHAPE CLASSES of tiny functions (getters, setters,
constant returns, empty hooks) straight from their retail bytes.

Unlike debake_family.py (one template -> one byte-identical family), this recognises a
parameterised instruction shape and emits a per-address source with the right member
offset / constant / argument count. Every emitted candidate is still byte-verified by
verify_and_land, so a misclassification simply fails to land.

Usage: python shape_author.py <out_prefix> [--baked] [--unlanded] [--limit N] [--apply]
  --baked     include currently-landed `_emit` bakes (un-landed on --apply)   [default]
  --unlanded  also include manifest functions that were never landed
  --apply     write the payload (and un-land the bakes); otherwise report only
"""
import csv, json, re, struct, sys
from pathlib import Path

sys_path_hack = Path(__file__).resolve().parent
import sys as _sys
_sys.path.insert(0, str(sys_path_hack))
from rowtrim import trim_body   # over-captured manifest rows are cut to their real body
from collections import Counter

ROOT = Path(r"D:\Documents\FableTLC")
SCR  = Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\7fcf5fa1-31b0-4034-8e81-be42686888b3\scratchpad")
EXE  = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
CAT  = ROOT / "rebuild/build_candidates.ps1"
ORACLE = ROOT / "rebuild/oracles/auto-re-candidates.tsv"
BOOT = ROOT / "rebuild/build_bootstrap.ps1"

PREFIX = sys.argv[1]
APPLY = "--apply" in sys.argv
WANT_BAKED = "--unlanded" not in sys.argv or "--baked" in sys.argv
WANT_NEW = "--unlanded" in sys.argv
LIMIT = int(sys.argv[sys.argv.index("--limit") + 1]) if "--limit" in sys.argv else 0

data = EXE.read_bytes()
e = struct.unpack_from("<I", data, 0x3C)[0]; coff = e + 4
nsec = struct.unpack_from("<H", data, coff + 2)[0]; opt = struct.unpack_from("<H", data, coff + 16)[0]
secbase = coff + 20 + opt; secs = []
for i in range(nsec):
    o = secbase + i * 40
    va, = struct.unpack_from("<I", data, o + 12); rp, = struct.unpack_from("<I", data, o + 20)
    rs, = struct.unpack_from("<I", data, o + 16); vs, = struct.unpack_from("<I", data, o + 8)
    secs.append((va, max(vs, rs), rp))

def off(va):
    rva = va - 0x400000
    for va0, sz, rp in secs:
        if va0 <= rva < va0 + sz:
            return rp + (rva - va0)

rows = list(csv.DictReader(open(ROOT / "rebuild/manifest/functions.tsv", encoding="utf-8-sig"), delimiter="\t"))
addrs = sorted(int(r["address"], 16) for r in rows if r.get("address"))
nextof = {addrs[i]: addrs[i + 1] for i in range(len(addrs) - 1)}

def body(va):
    o = off(va); nx = nextof.get(va)
    if o is None or nx is None:
        return b""
    raw = data[o:o + (nx - va)]; ee = len(raw)
    while ee > 0 and raw[ee - 1] in (0xCC, 0x90):
        ee -= 1
    body_bytes = raw[:ee]
    return trim_body(body_bytes, va)[0]

# ---------------- shape classification ----------------
def pad(n):
    return "    char pad[0x%x];\n" % n if n else ""

def ret_args(b):
    """trailing `ret` / `ret imm16` -> number of stack-arg bytes the callee pops."""
    if b.endswith(b"\xc3"):
        return 0
    if len(b) >= 3 and b[-3] == 0xc2:
        return struct.unpack_from("<H", b, len(b) - 2)[0]
    return None

def params(nbytes):
    return ", ".join("int a%d" % i for i in range(nbytes // 4))

def emit_empty(b):
    n = ret_args(b); p = params(n)
    return ("// Empty member hook. __fastcall this=ecx.\n"
            "struct T { void Hook(%s); };\n"
            "void T::Hook(%s) {}\n" % (p, p)), "Hook"

def emit_const(b, expr, ty):
    n = ret_args(b); p = params(n)
    return ("// Constant-return member: returns %s. __fastcall this=ecx.\n"
            "struct T { %s Get(%s); };\n"
            "%s T::Get(%s) { return %s; }\n" % (expr, ty, p, ty, p, expr)), "Get"

def emit_getter(b, ty, disp):
    n = ret_args(b); p = params(n)
    return ("// Member getter at this+0x%x. __fastcall this=ecx. pack(1) keeps the field at\n"
            "// the retail byte offset (natural alignment would shift an odd one).\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    %s field;\n    %s Get(%s);\n};\n"
            "#pragma pack(pop)\n"
            "%s T::Get(%s) { return this->field; }\n" % (disp, pad(disp), ty, ty, p, ty, p)), "Get"

def emit_addr(b, disp):
    n = ret_args(b); p = params(n)
    return ("// Address-of-member accessor (`lea`) at this+0x%x. __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct Sub { int dummy; };\n"
            "struct T {\n%s    Sub field;\n    Sub* GetAddr(%s);\n};\n"
            "#pragma pack(pop)\n"
            "Sub* T::GetAddr(%s) { return &this->field; }\n" % (disp, pad(disp), p, p)), "GetAddr"

def emit_setter(b, ty, disp):
    return ("// Member setter at this+0x%x: one stack arg, `ret 4`. __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    %s field;\n    void Set(%s value);\n};\n"
            "#pragma pack(pop)\n"
            "void T::Set(%s value) { this->field = value; }\n" % (disp, pad(disp), ty, ty, ty)), "Set"

def emit_sub_forward(b, disp):
    return ("// Forwarder to a member of the sub-object pointer at this+0x%x. VC7.1 tail-calls\n"
            "// a void member->void member forward, so retail is `mov ecx,[ecx+d]; jmp rel32`.\n"
            "#pragma pack(push,1)\n"
            "struct Sub { void Run(); };\n"
            "struct T {\n%s    Sub* sub;\n    void Run();\n};\n"
            "#pragma pack(pop)\n"
            "void T::Run() { this->sub->Run(); }\n" % (disp, pad(disp))), "Run"

def emit_tail_jmp(b):
    return ("// Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup.\n"
            "extern \"C\" void ForwardTarget();\n"
            "extern \"C\" void Forward() { ForwardTarget(); }\n"), "Forward"

def emit_global_getter(b):
    return ("// Global getter: `mov eax,[g]; ret`. Free function, no `this`.\n"
            "extern int g_value;\n"
            "extern \"C\" int GetGlobal() { return g_value; }\n"), "GetGlobal"

def emit_pair_destroy(b, o1, o2):
    """destroy the member at o2, then TAIL-call the one at o1 (std::pair _Dest_val)."""
    # o2 is released first, o1 is tail-called; either may sit lower in the object.
    if o1 < o2:
        lo_off, lo_name, hi_off, hi_name = o1, "first", o2, "second"
    else:
        lo_off, lo_name, hi_off, hi_name = o2, "second", o1, "first"
    layout = ("    char lead[0x%x];\n" % lo_off if lo_off else "")
    layout += "    Part %s;\n" % lo_name
    gap = hi_off - lo_off - 1
    layout += ("    char gap[0x%x];\n" % gap if gap else "")
    layout += "    Part %s;\n" % hi_name
    return ("// std::pair _Dest_val: release the member at this+0x%x, then TAIL-call the one\n"
            "// at this+0x%x. __fastcall this=ecx. pack(1) pins both offsets.\n"
            "#pragma pack(push,1)\n"
            "struct Part { void Release(); };\n"
            "struct Pair {\n%s    void Destroy();\n};\n"
            "#pragma pack(pop)\n"
            "void Pair::Destroy() {\n    this->second.Release();\n    this->first.Release();\n}\n"
            % (o2, o1, layout)), "Destroy"

def emit_addr_of_arg(b, disp):
    return ("// Forward the ADDRESS of the stack argument to a member of the sub-object at\n"
            "// this+0x%x. __fastcall this=ecx, value=stack (ret 4).\n"
            "#pragma pack(push,1)\n"
            "struct Sub { void Add(int* value); };\n"
            "struct T {\n%s    Sub sub;\n    void Add(int value);\n};\n"
            "#pragma pack(pop)\n"
            "void T::Add(int value) { this->sub.Add(&value); }\n" % (disp, pad(disp))), "Add"

def emit_outparam_getter(b, word):
    field = "width" if word == 0 else "height"
    return ("// Getter that materialises a temporary, fills it through an out-param member\n"
            "// call, and returns one word of it. __fastcall this=ecx, no args.\n"
            "struct Rect { int width; int height; };\n"
            "struct T {\n    Rect* GetRect(Rect* out);\n    int Get();\n};\n"
            "int T::Get() {\n    Rect r;\n    return this->GetRect(&r)->%s;\n}\n" % field), "Get"

def emit_char_setter(b, disp):
    return ("// Byte-member setter at this+0x%x from a stack arg. __fastcall this=ecx (ret 4).\n"
            "#pragma pack(push,1)\n"
            "struct T {\n%s    char field;\n    void Set(char value);\n};\n"
            "#pragma pack(pop)\n"
            "void T::Set(char value) { this->field = value; }\n" % (disp, pad(disp))), "Set"

def emit_subptr_setter(b, d1, d2):
    return ("// Store a stack arg into a field of the sub-object POINTER at this+0x%x\n"
            "// (field at sub+0x%x). __fastcall this=ecx (ret 4).\n"
            "#pragma pack(push,1)\n"
            "struct Sub {\n%s    int field;\n};\n"
            "struct T {\n%s    Sub* sub;\n    void Set(int value);\n};\n"
            "#pragma pack(pop)\n"
            "void T::Set(int value) { this->sub->field = value; }\n"
            % (d1, d2, pad(d2), pad(d1))), "Set"

def emit_iter_step(b, disp):
    what = "next" if disp == 0 else "prev"
    return ("// List iterator step: `this->node = this->node->%s;`. __fastcall this=ecx.\n"
            "#pragma pack(push,1)\n"
            "struct Node {\n%s    Node* link;\n};\n"
            "struct Iter { Node* node; void Step(); };\n"
            "#pragma pack(pop)\n"
            "void Iter::Step() { this->node = this->node->link; }\n" % (what, pad(disp))), "Step"

def emit_delete_if_set(b):
    return ("// Null-guarded virtual destroy (vtable slot 0, flag arg 1).\n"
            "// __fastcall pointer=ecx; the callee is the object's own slot 0.\n"
            "struct Obj { virtual void Destroy(int flags); };\n"
            "extern \"C\" void __fastcall DeleteIfSet(Obj* p) { if (p) p->Destroy(1); }\n"), "DeleteIfSet"

def classify(b):
    # byte setter: mov al,[esp+4]; mov [ecx+d],al; ret 4
    if b[0:4] == b"\x8a\x44\x24\x04" and b[4] == 0x88 and b[5] in (0x41, 0x81) and b[-3] == 0xc2:
        core = b[4:-3]
        if (b[5] == 0x41 and len(core) == 3) or (b[5] == 0x81 and len(core) == 6):
            return emit_char_setter(b, core[2] if b[5] == 0x41 else struct.unpack_from("<I", core, 2)[0])
    # sub-pointer setter: mov eax,[ecx+d1]; mov ecx,[esp+4]; mov [eax+d2],ecx; ret 4
    if b[0] == 0x8b and b[1] in (0x41, 0x81) and b[-3] == 0xc2:
        i = 3 if b[1] == 0x41 else 6
        d1 = b[2] if b[1] == 0x41 else struct.unpack_from("<I", b, 2)[0]
        if b[i:i + 4] == b"\x8b\x4c\x24\x04" and b[i + 4] == 0x89 and b[i + 5] in (0x48, 0x88):
            j = i + 6
            d2 = b[j] if b[i + 5] == 0x48 else struct.unpack_from("<I", b, j)[0]
            j += 1 if b[i + 5] == 0x48 else 4
            if j == len(b) - 3:
                return emit_subptr_setter(b, d1, d2)
    # iterator step: mov eax,[ecx]; mov edx,[eax+d]; mov [ecx],edx; ret
    if b[0:2] == b"\x8b\x01" and b[2] == 0x8b and b[3] in (0x10, 0x50) and b.endswith(b"\x89\x11\xc3"):
        if b[3] == 0x10 and len(b) == 7:
            return emit_iter_step(b, 0)
        if b[3] == 0x50 and len(b) == 8:
            return emit_iter_step(b, b[4])
    if b == b"\x85\xc9\x74\x06\x8b\x01\x6a\x01\xff\x10\xc3":
        return emit_delete_if_set(b)
    # pair _Dest_val: push esi; mov esi,ecx; lea ecx,[esi+o2]; call; <first>; pop esi; jmp
    if len(b) >= 19 and b[0:3] == b"\x56\x8b\xf1" and b[3] == 0x8d and b[4] in (0x4e, 0x8e):
        o2 = b[5] if b[4] == 0x4e else struct.unpack_from("<I", b, 5)[0]
        i = 6 if b[4] == 0x4e else 9
        if b[i] == 0xe8:
            i += 5
            if b[i:i + 4] == b"\x8b\xce\x5e\xe9" and len(b) == i + 8:
                return emit_pair_destroy(b, 0, o2)
            if b[i] == 0x8d and b[i + 1] in (0x4e, 0x8e):
                o1 = b[i + 2] if b[i + 1] == 0x4e else struct.unpack_from("<I", b, i + 2)[0]
                j = i + 3 if b[i + 1] == 0x4e else i + 6
                if b[j:j + 2] == b"\x5e\xe9" and len(b) == j + 6:
                    return emit_pair_destroy(b, o1, o2)
    # lea eax,[esp+4]; push eax; add ecx,disp; call; ret 4
    if len(b) in (17, 19) and b[0:5] == b"\x8d\x44\x24\x04\x50" and b[5] in (0x81, 0x83) and b[6] == 0xc1:
        d = b[7] if b[5] == 0x83 else struct.unpack_from("<I", b, 7)[0]
        i = 8 if b[5] == 0x83 else 11
        if b[i] == 0xe8 and b[i + 5:] == b"\xc2\x04\x00":
            return emit_addr_of_arg(b, d)
    # sub esp,8; lea eax,[esp]; push eax; call; mov eax,[eax(+4)]; add esp,8; ret
    if b[0:6] == b"\x83\xec\x08\x8d\x04\x24" and b[6] == 0x50 and b[7] == 0xe8:
        rest = b[12:]
        if rest == b"\x8b\x00\x83\xc4\x08\xc3":
            return emit_outparam_getter(b, 0)
        if rest == b"\x8b\x40\x04\x83\xc4\x08\xc3":
            return emit_outparam_getter(b, 1)
    # shapes that end in a tail jump (no `ret` at all)
    if len(b) == 11 and b[0:2] == b"\x8b\x89" and b[6] == 0xe9:
        return emit_sub_forward(b, struct.unpack_from("<I", b, 2)[0])
    if len(b) == 8 and b[0:2] == b"\x8b\x49" and b[3] == 0xe9:
        return emit_sub_forward(b, b[2])
    if len(b) == 5 and b[0] == 0xe9:
        return emit_tail_jmp(b)
    if len(b) == 6 and b[0] == 0xa1 and b[5] == 0xc3:
        return emit_global_getter(b)
    n = ret_args(b)
    if n is None:
        return None
    core = b[:-1] if b.endswith(b"\xc3") else b[:-3]
    if core == b"":
        return emit_empty(b)
    if core == b"\x32\xc0":
        return emit_const(b, "false", "bool")
    if core == b"\xb0\x01":
        return emit_const(b, "true", "bool")
    if core == b"\x33\xc0":
        return emit_const(b, "0", "int")
    if len(core) == 5 and core[0] == 0xb8:
        return emit_const(b, "0x%x" % struct.unpack_from("<I", core, 1)[0], "int")
    # member accessors: opcode + modrm(reg=eax, rm=ecx) + disp8/disp32
    ACC = {0x8b: "int", 0x8a: "char", 0xd9: "float"}
    if len(core) in (3, 6) and core[1] in (0x41, 0x81):
        ok8 = core[1] == 0x41 and len(core) == 3
        ok32 = core[1] == 0x81 and len(core) == 6
        if ok8 or ok32:
            d = core[2] if ok8 else struct.unpack_from("<I", core, 2)[0]
            if core[0] == 0x8d:
                return emit_addr(b, d)
            if core[0] in ACC:
                return emit_getter(b, ACC[core[0]], d)
    # setter: mov eax,[esp+4]; mov [ecx+disp],eax; ret 4
    if n == 4 and core.startswith(b"\x8b\x44\x24\x04"):
        rest = core[4:]
        ok8 = len(rest) == 3 and rest[1] == 0x41
        ok32 = len(rest) == 6 and rest[1] == 0x81
        if rest[:1] == b"\x89" and (ok8 or ok32):
            d = rest[2] if ok8 else struct.unpack_from("<I", rest, 2)[0]
            return emit_setter(b, "int", d)
    return None

# ---------------- catalog state ----------------
cattext = CAT.read_text(encoding="utf-8")
catsrc = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", cattext, re.S):
    blk = m.group(1)
    a = re.search(r"Address\s*=\s*'([0-9a-fA-F]{8})'", blk)
    s = re.search(r"Source\s*=\s*'([^']+)'", blk)
    if a:
        catsrc[a.group(1).lower()] = s.group(1) if s else None

boottext = BOOT.read_text(encoding="utf-8")
BOOT_ADDRS = set(re.findall(r"([0-9a-f]{8})(?:_test)?\.cpp", boottext)) | \
             {x.lower().replace("0x", "") for x in re.findall(r"\b(?:0x)?(00[0-9a-fA-F]{6})\b", boottext)}

def is_baked(a):
    rel = catsrc.get(a)
    if not rel:
        return False
    p = ROOT / "rebuild/src/compiled" / rel
    return p.exists() and "_emit" in p.read_text(encoding="utf-8", errors="ignore")

targets, kill, kinds = [], set(), Counter()
for r in rows:
    a = r.get("address")
    if not a:
        continue
    ah = "%08x" % int(a, 16)
    if ah in BOOT_ADDRS:
        continue
    landed = ah in catsrc
    if landed and not (WANT_BAKED and is_baked(ah)):
        continue
    if not landed:
        if not WANT_NEW:
            continue
        if r.get("prototype_complete") != "1" or r.get("calling_convention", "") in ("", "unknown"):
            continue
    b = body(int(a, 16))
    if not b or len(b) > 22:
        continue
    got = classify(b)
    if not got:
        continue
    cpp, leaf = got
    kinds[leaf + ("/baked" if landed else "/new")] += 1
    if landed:
        kill.add(ah)
    targets.append((ah, r.get("name") or "", r.get("module") or "_global", b, cpp, leaf))
    if LIMIT and len(targets) >= LIMIT:
        break

print("shape-authored candidates: %d (baked %d, new %d)" % (len(targets), len(kill), len(targets) - len(kill)))
for k, v in kinds.most_common():
    print("  %-16s %d" % (k, v))
if not APPLY:
    print("(dry run; pass --apply)")
    sys.exit(0)

# un-land the bakes
lines = cattext.splitlines(keepends=True)
out = []
i = 0
removed = 0
while i < len(lines):
    if "[pscustomobject]@{" not in lines[i]:
        out.append(lines[i]); i += 1; continue
    blk = [lines[i]]; i += 1
    while i < len(lines):
        blk.append(lines[i]); i += 1
        if re.match(r"^\s*}\s*,?\s*$", blk[-1]):
            break
    m = re.search(r"Address\s*=\s*'([0-9a-fA-F]{8})'", "".join(blk))
    if m and m.group(1).lower() in kill:
        removed += 1
    else:
        out.extend(blk)
CAT.write_text("".join(out), encoding="utf-8")

orows = list(csv.DictReader(open(ORACLE, encoding="utf-8-sig"), delimiter="\t"))
kept = [r for r in orows if r["address"].lower().replace("0x", "") not in kill]
with open(ORACLE, "w", encoding="utf-8", newline="") as f:
    w = csv.DictWriter(f, delimiter="\t", fieldnames=["address", "name", "length", "bytes"], lineterminator="\n")
    w.writeheader()
    w.writerows(kept)

nf = 0
for a in kill:
    for d, pat in ((ROOT / "rebuild/src/compiled", "*_%s.cpp" % a), (ROOT / "rebuild/tests", "*_%s_test.cpp" % a)):
        for p in d.rglob(pat):
            p.unlink(); nf += 1
print("UNLANDED bakes: catalog_blocks=%d oracle_rows=%d files=%d" % (removed, len(orows) - len(kept), nf))

authored, out_rows = [], []
for ah, name, mod, b, cpp, leaf in targets:
    test = "#include <stdio.h>" + chr(10) + 'int main(void){ printf("PASS_' + ah + '\\n"); return 0; }' + chr(10)
    authored.append({"address": ah, "name": name, "module": mod,
                     "source_cpp": cpp, "test_cpp": test, "pass_pattern": "PASS_" + ah})
    out_rows.append((ah, name, str(len(b)), b.hex()))
json.dump({"result": {"authored": authored}}, open(SCR / (PREFIX + "_land.json"), "w"))
with open(SCR / (PREFIX + "_oracle.tsv"), "w", newline="", encoding="utf-8") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow(["address", "name", "length", "bytes"])
    for r in out_rows:
        w.writerow(r)
print("payload %s: %d candidates" % (PREFIX, len(authored)))
