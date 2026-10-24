"""Census of Direct3D 9 interface call sites in the retail Fable.exe (sizes the modern RHI).

Evidence for docs/modern/D3D9_DEVICE_CENSUS.md (+ .json). Static only: linear disassembly
(capstone) of every non-thunk function in ghidra_out/coverage.tsv with a typed register model.

Types come from the PDB layouts (ghidra_out/struct_layouts_egor.tsv): a register that holds a
pointer to class T, loaded from `[r+d]`, gets the PDB type of T's member at offset d (descending
into embedded members, arrays and the base classes in BASES). Hand seeds are SEED_GLOBALS and
STATIC_INSTANCES; ghidra_out/rtti_live_globals.tsv and the file's RTTI vtables add the rest. Types spread:

  - into callees: a function whose callers all pass `ecx` of type T is a T method (`this`),
  - out of globals: `mov [abs], r` with r typed makes abs a typed global,
  - out of getters: a tiny `mov eax,[ecx+d] ; ret` method returns the member type.

A COM call is `call [V+off]` where V = `[P]`, P holds a D3D9 interface pointer, and P (the very same
load) was the last register pushed: the stdcall `this`. Method names come from the interface
declarations in the DirectX SDK d3d9.h. The result is a lower bound: device pointers passed as
arguments, spilled to the stack, or reloaded through untyped objects are not seen.
"""
from __future__ import annotations

import collections
import json
import re
import sys
from pathlib import Path

import capstone
from capstone import x86 as cx

ROOT = Path(__file__).resolve().parents[2]
EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
D3D9_H = Path(r"D:\Tools\dxsdk_feb10\include\d3d9.h")
COVERAGE = ROOT / "ghidra_out" / "coverage.tsv"
LAYOUTS = ROOT / "ghidra_out" / "struct_layouts_egor.tsv"
OUT_MD = ROOT / "docs" / "modern" / "D3D9_DEVICE_CENSUS.md"
OUT_JSON = ROOT / "docs" / "modern" / "D3D9_DEVICE_CENSUS.json"

# 0x1436E18 holds the CRenderManagerCore*: 96 `mov r,[0x1436E18]` loads feed `[r+0x3CF0]`
# (CRenderManagerCore::PD3DDevice in the PDB), and CRenderManagerCore::Init stores the device there.
# 0x13CB508 is the render-state-manager singleton (written by Init, cleared by ~CRenderStateManager)
# that the frontend/2D paths use; live 2026-10-16 it pointed at the render core with the device at
# both +4 and +0x3CF0, while 0x1436E18 (CEngine's copy, set in CEngine::Initialise) was still 0.
# Live RTTI (2026-10-16) names [0x13CB508]'s object CRenderManager.
SEED_GLOBALS = {0x1436E18: "CRenderManagerCore", 0x13CB508: "CRenderManager"}
# Static objects (the address is the object): `mov r, imm` and `mov eax, imm ; ret` getters.
# 0x13CA618 is CSystemManager by live RTTI; getter 0x9A4EC0 returns it, and
# [[0x13CA618+0x60]+0x58] was the live device (CSystemManager::PDisplayManager -> D3DDevice).
STATIC_INSTANCES = {0x13CA618: "CSystemManager"}
# PDB layouts print a base class as `_padding_` ints; these bases are confirmed by size
# (CRenderStateManager is 14908 bytes, exactly the padding run before CRenderManagerCore's own members;
# CRenderManager lists no members of its own before CRenderManagerCore's 15648 bytes).
BASES = {"CRenderManagerCore": "CRenderStateManager", "CRenderManager": "CRenderManagerCore"}

# Tier B: device members by retail offset, used where PDB typing cannot reach.
#  - 0x3CF0 is the only PDB member at that offset in any class (CRenderManagerCore::PD3DDevice).
#  - the others apply only inside functions whose name, in Ego_r.pdb, belongs to that class alone.
# CShaderRenderManager::D3DDevice is +0x1E4 in Ego_r.pdb but +0x1D4 in retail: retail shader-manager
# methods (ApplyVertexShader, SetUserVertexShaderConstant, UpdatePixelShader, ...) push [this+0x1D4]
# as the COM `this` of SetVertexShaderConstantF / SetPixelShader / SetStreamSource calls.
UNIQUE_DEVICE_OFFSETS = {0x3CF0}
CLASS_DEVICE_OFFSETS = {"CRenderManagerCore": {0x4, 0x3CF0}, "CRenderStateManager": {0x4},
                        "CDisplayManager": {0x58}, "CShaderRenderManager": {0x1D4}}
EGOR_NAMES = ROOT / "ghidra_out" / "egor_pdb_names.tsv"
RTTI_GLOBALS = ROOT / "ghidra_out" / "rtti_live_globals.tsv"

REGS32 = ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp")
VOLATILE = ("eax", "ecx", "edx")
ITERATIONS = 10


def d3d9_interfaces() -> dict[str, list[str]]:
    text = D3D9_H.read_text(errors="replace")
    out = {}
    for m in re.finditer(r"DECLARE_INTERFACE_\((IDirect3D\w+),\s*\w+\)", text):
        body = text[m.end():text.index("};", m.end())]
        out[m.group(1)] = re.findall(r"STDMETHOD(?:_\([^,]+,\s*|\()\s*(\w+)\)", body)
    if len(out.get("IDirect3DDevice9", [])) != 119:
        sys.exit("d3d9.h parse failed: IDirect3DDevice9 should have 119 methods")
    return out


class Layouts:
    def __init__(self, path: Path):
        self.size: dict[str, int] = {}
        self.fields: dict[str, list[tuple[int, str]]] = collections.defaultdict(list)
        for line in path.read_text(errors="replace").splitlines():
            c = line.split("\t")
            if len(c) < 5:
                continue
            if c[0] == "@STRUCT":
                self.size[c[1]] = int(c[2])
            elif c[4] != "_padding_":
                self.fields[c[0]].append((int(c[2]), c[3].strip()))
                self.size.setdefault(c[0], int(c[1]))

    def member(self, t: str, off: int, depth: int = 0) -> str | None:
        """Type of the member of `t` that starts exactly at `off` (through embedded members)."""
        if depth > 6:
            return None
        base = BASES.get(t)
        if base and off < self.size.get(base, 0) and not any(o == off for o, _ in self.fields.get(t, ())):
            return self.member(base, off, depth + 1)
        best = None
        for o, ty in self.fields.get(t, ()):
            if o == off and not ty.endswith("]"):
                return ty
            if o <= off and (best is None or o > best[0]):
                best = (o, ty)
        if best is None:
            return None
        o, ty = best
        arr = re.fullmatch(r"(.+?)\s*\[(\d+)\]", ty)
        if arr:
            elem = arr.group(1)
            esz = 4 if elem.endswith("*") else self.size.get(elem, 0)
            if esz and off - o < esz * int(arr.group(2)):
                rel = (off - o) % esz
                return elem if rel == 0 else self.member(elem, rel, depth + 1)
            return None
        if ty in self.size and off - o < self.size[ty]:
            return self.member(ty, off - o, depth + 1)
        return None

    def load(self, t: str, off: int) -> str | None:
        """Type of the dword loaded from `[T* + off]`: an embedded struct yields its first field
        (e.g. CCountedPointer<T>::Data)."""
        ty = self.member(t, off)
        for _ in range(4):
            if ty is None or ty.endswith("*") or not (ty in self.fields or ty in self.size):
                break
            ty = self.member(ty, 0)
        return ty


def functions() -> list[tuple[int, str, int]]:
    out = []
    for line in COVERAGE.read_text(errors="replace").splitlines()[1:]:
        c = line.split("\t")
        if len(c) >= 5 and c[4] != "1" and int(c[3] or 0) > 0:
            out.append((int(c[0], 16), c[1], int(c[3])))
    return out


def load_image():
    import pefile
    pe = pefile.PE(str(EXE), fast_load=True)
    return pe.OPTIONAL_HEADER.ImageBase, pe.get_memory_mapped_image()


def class_only_names() -> dict[str, str]:
    """Bare method name -> device class, for names that no other class in Ego_r.pdb uses."""
    owners = collections.defaultdict(set)
    for line in EGOR_NAMES.read_text(errors="replace").splitlines():
        m = re.match(r"[0-9a-fA-F]+	\?(\w+)@(\w+)@@", line)
        if m and not m.group(1).startswith("?"):
            owners[m.group(1)].add(m.group(2))
    return {n: next(iter(cs)) for n, cs in owners.items()
            if len(cs) == 1 and next(iter(cs)) in CLASS_DEVICE_OFFSETS}


def rtti_vtable_methods(base: int, img: bytes, lay: "Layouts") -> dict[int, str]:
    """Function -> `this` class from the retail file's own MSVC RTTI. Every vtable is preceded by a
    complete-object locator (signature 0, offset 0 = primary vtable) naming its class; each slot that
    points into .text is a method of that class. A function shared by several classes' vtables (an
    inherited, non-overridden method) gets the smallest class with a PDB layout: its offsets are
    valid in every derived class."""
    import pefile
    pe = pefile.PE(str(EXE), fast_load=True)
    sec = {s.Name.rstrip(b"\0").decode(): (base + s.VirtualAddress, base + s.VirtualAddress + s.Misc_VirtualSize)
           for s in pe.sections}
    text, rdata = sec[".text"], sec[".rdata"]
    u32 = lambda a: int.from_bytes(img[a - base:a - base + 4], "little")
    owners: dict[int, set[str]] = collections.defaultdict(set)
    for a in range(rdata[0], rdata[1] - 8, 4):
        col = u32(a)
        if not rdata[0] <= col < rdata[1] - 16 or u32(col) != 0 or u32(col + 4) != 0:
            continue
        td = u32(col + 12)
        if not sec[".data"][0] <= td < sec[".data"][1]:
            continue
        raw = img[td + 8 - base:td + 8 - base + 128].split(b"\0")[0].decode("latin1")
        if not raw.startswith((".?AV", ".?AU")) or not raw.endswith("@@") or "@" in raw[4:-2]:
            continue
        cls = raw[4:-2]
        if cls not in lay.size and cls not in lay.fields:
            continue
        v = a + 4
        while v < rdata[1] and text[0] <= u32(v) < text[1]:
            owners[u32(v)].add(cls)
            v += 4
    return {f: min(cs, key=lambda c: lay.size.get(c, 1 << 30)) for f, cs in owners.items()}


def pointee(ty: str | None) -> str | None:
    if ty and ty.endswith("*") and not ty.endswith("**"):
        return ty[:-1].strip()
    return None


class Census:
    def __init__(self):
        self.ifaces = d3d9_interfaces()
        self.lay = Layouts(LAYOUTS)
        self.funcs = functions()
        self.base, self.img = load_image()
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        self.md.detail = True
        self.globals: dict[int, str] = dict(SEED_GLOBALS)
        # Live RTTI sweep (tools/modern/rtti_live_globals.py): typed statics and pointer globals.
        self.statics: dict[int, str] = dict(STATIC_INSTANCES)
        if RTTI_GLOBALS.exists():
            for line in RTTI_GLOBALS.read_text().splitlines()[1:]:
                kind, addr, cls = line.split("\t")[:3]
                if cls in self.lay.size or cls in self.lay.fields:
                    (self.statics if kind == "static" else self.globals).setdefault(int(addr, 16), cls)
        self.this_type: dict[int, str] = rtti_vtable_methods(self.base, self.img, self.lay)
        self.rtti_methods = set(self.this_type)  # seeds that caller agreement must not override
        self.returns: dict[int, str] = {}
        self.owned = class_only_names()
        self.tier_b: set[int] = set()  # identities of tier-B device loads

    def scan(self):
        calls = []
        callee_this = collections.defaultdict(collections.Counter)
        new_globals = collections.defaultdict(collections.Counter)
        new_returns = {}
        lay, ifaces = self.lay, self.ifaces
        for fva, fname, size in self.funcs:
            code = self.img[fva - self.base: fva - self.base + size]
            # reg -> (type, identity); vt: reg -> (type, identity) of the object whose vtable it holds
            val: dict[str, tuple[str, int]] = {}
            vt: dict[str, tuple[str, int]] = {}
            if fva in self.this_type:
                val["ecx"] = (self.this_type[fva], fva)
            last_push = None
            esp4 = None
            dev_offs = set(UNIQUE_DEVICE_OFFSETS) | CLASS_DEVICE_OFFSETS.get(self.owned.get(fname, ""), set())
            ins = list(self.md.disasm(code, fva))
            if (len(ins) == 2 and ins[1].mnemonic == "ret" and ins[0].mnemonic == "mov"
                    and ins[0].operands[1].type == cx.X86_OP_IMM
                    and ins[0].operands[1].imm in self.statics):
                new_returns[fva] = self.statics[ins[0].operands[1].imm]
            if (len(ins) == 2 and ins[1].mnemonic == "ret" and ins[0].mnemonic == "mov"
                    and fva in self.this_type):
                o = ins[0].operands
                if (o[0].type == cx.X86_OP_REG and ins[0].reg_name(o[0].reg) == "eax"
                        and o[1].type == cx.X86_OP_MEM and o[1].mem.index == 0
                        and ins[0].reg_name(o[1].mem.base) == "ecx"):
                    ty = pointee(lay.load(self.this_type[fva], o[1].mem.disp))
                    if ty:
                        new_returns[fva] = ty
            for insn in ins:
                mn, ops = insn.mnemonic, insn.operands
                if mn in ("mov", "lea") and len(ops) == 2 and ops[0].type == cx.X86_OP_MEM and mn == "mov":
                    m = ops[0].mem
                    if m.base == 0 and m.index == 0 and ops[1].type == cx.X86_OP_REG:
                        s = insn.reg_name(ops[1].reg)
                        if s in val and not val[s][0].startswith("@"):
                            new_globals[m.disp & 0xFFFFFFFF][val[s][0]] += 1
                    elif (m.index == 0 and m.disp == 4 and ops[1].type == cx.X86_OP_REG
                          and insn.reg_name(m.base) == "esp"):
                        esp4 = val.get(insn.reg_name(ops[1].reg))  # `this` of a COM tail jump
                    continue
                if (mn == "add" and len(ops) == 2 and ops[0].type == cx.X86_OP_REG
                        and ops[1].type == cx.X86_OP_IMM and insn.reg_name(ops[0].reg) in val):
                    # pointer to a member: `add edi, 0x3cf0` then `mov eax, [edi]`
                    r = insn.reg_name(ops[0].reg)
                    t, ident = val[r]
                    vt.pop(r, None)
                    if t in ifaces:
                        val.pop(r)
                    else:
                        base_t, off0 = (t[1:].rsplit("@", 1)[0], int(t.rsplit("@", 1)[1])) if t.startswith("@") else (t, 0)
                        val[r] = (f"@{base_t}@{off0 + ops[1].imm}", ident)
                    continue
                if mn in ("mov", "lea") and len(ops) == 2 and ops[0].type == cx.X86_OP_REG:
                    dst = insn.reg_name(ops[0].reg)
                    if dst not in REGS32:
                        continue
                    src = ops[1]
                    nv = nvt = None
                    if src.type == cx.X86_OP_REG and mn == "mov":
                        s = insn.reg_name(src.reg)
                        nv, nvt = val.get(s), vt.get(s)
                    elif src.type == cx.X86_OP_IMM and mn == "mov" and src.imm in self.statics:
                        nv = (self.statics[src.imm], insn.address)
                    elif src.type == cx.X86_OP_MEM and src.mem.index == 0:
                        b = insn.reg_name(src.mem.base) if src.mem.base else None
                        d = src.mem.disp
                        if mn == "lea":
                            if b in val and val[b][0] not in ifaces:
                                ty = val[b][0] if d == 0 else lay.member(val[b][0], d)
                                if ty in lay.size or ty in lay.fields:
                                    nv = (ty, insn.address)
                        elif b is None:
                            g = self.globals.get(d & 0xFFFFFFFF)
                            if g:
                                nv = (g, insn.address)
                        elif b in val:
                            t = val[b][0]
                            if d == 0 and t in ifaces:
                                nvt = val[b]
                            elif t.startswith("@"):
                                if mn == "mov":
                                    bt, o0 = t[1:].rsplit("@", 1)
                                    ty = pointee(lay.load(bt, int(o0) + d))
                                    if ty:
                                        nv = (ty, insn.address)
                            else:
                                ty = pointee(lay.load(t, d))
                                if ty:
                                    nv = (ty, insn.address)
                        if nv is None and nvt is None and mn == "mov" and b and d in dev_offs:
                            nv = ("IDirect3DDevice9", insn.address)
                            self.tier_b.add(insn.address)
                    val.pop(dst, None)
                    vt.pop(dst, None)
                    if nv:
                        val[dst] = nv
                    if nvt:
                        vt[dst] = nvt
                    continue
                if mn == "push" and len(ops) == 1:
                    last_push = insn.reg_name(ops[0].reg) if ops[0].type == cx.X86_OP_REG else None
                    continue
                if mn == "call" and len(ops) == 1:
                    op = ops[0]
                    if op.type == cx.X86_OP_MEM and op.mem.base and op.mem.index == 0:
                        vreg = insn.reg_name(op.mem.base)
                        obj = vt.get(vreg)
                        if obj and last_push and val.get(last_push) == obj:
                            calls.append((fva, fname, insn.address, obj[0], op.mem.disp,
                                          "B" if obj[1] in self.tier_b else "A"))
                    if (op.type == cx.X86_OP_IMM and "ecx" in val and val["ecx"][0] not in ifaces
                            and not val["ecx"][0].startswith("@")):
                        callee_this[op.imm][val["ecx"][0]] += 1
                    esp4 = None
                    for r in VOLATILE:
                        val.pop(r, None)
                        vt.pop(r, None)
                    if op.type == cx.X86_OP_IMM and op.imm in self.returns:
                        val["eax"] = (self.returns[op.imm], insn.address)
                    last_push = None
                    continue
                if mn == "jmp" and len(ops) == 1 and ops[0].type == cx.X86_OP_MEM and ops[0].mem.base:
                    # COM tail call: `mov [esp+4], this ; jmp [vtbl+off]`
                    obj = vt.get(insn.reg_name(ops[0].mem.base))
                    if obj and ops[0].mem.index == 0 and esp4 == obj:
                        calls.append((fva, fname, insn.address, obj[0], ops[0].mem.disp,
                                      "B" if obj[1] in self.tier_b else "A"))
                    continue
                if mn == "ret":
                    val.clear(); vt.clear()
                    continue
                if ops and ops[0].type == cx.X86_OP_REG and mn not in ("cmp", "test", "push"):
                    dst = insn.reg_name(ops[0].reg)
                    if dst in REGS32:
                        val.pop(dst, None)
                        vt.pop(dst, None)
        self.callee_this = callee_this  # kept for diagnostics
        # Learn: `this` types agreed by every typed caller; globals written with one type only.
        changed = 0
        for callee, cnt in callee_this.items():
            # Unanimous, or dominant (>= 3 typed callers, >= 80%): one caller mistyped by PDB offset
            # drift must not hide a method (ClearRenderTarget 0x9BE420: 5 CDisplayManager, 1 CStateInfo).
            t, top = cnt.most_common(1)[0]
            total = sum(cnt.values())
            if callee not in self.rtti_methods and (len(cnt) == 1 or (total >= 3 and top >= 0.8 * total)):
                if self.this_type.get(callee) != t:
                    self.this_type[callee] = t
                    changed += 1
        for g, cnt in new_globals.items():
            if len(cnt) == 1 and g not in self.globals:
                self.globals[g] = next(iter(cnt))
                changed += 1
        for f, t in new_returns.items():
            if self.returns.get(f) != t:
                self.returns[f] = t
                changed += 1
        return calls, changed

    def run(self):
        for i in range(ITERATIONS):
            calls, changed = self.scan()
            print(f"pass {i + 1}: {len(calls)} D3D9 calls, {len(self.this_type)} typed methods, "
                  f"{len(self.globals)} typed globals, {len(self.returns)} getters, {changed} new facts")
            if not changed:
                break
        return calls


def write_report(c: Census, calls) -> None:
    ifaces = c.ifaces
    per = collections.defaultdict(collections.Counter)
    per_funcs = collections.defaultdict(lambda: collections.defaultdict(set))
    dev_funcs = collections.Counter()
    sites, bad = [], 0
    tiers = collections.Counter()
    for fva, fname, cva, iface, off, tier in sorted(calls, key=lambda t: t[2]):
        methods = ifaces[iface]
        if off % 4 or off // 4 >= len(methods):
            bad += 1
            continue
        name = methods[off // 4]
        per[iface][name] += 1
        tiers[(iface == "IDirect3DDevice9", tier)] += 1
        per_funcs[iface][name].add(fva)
        if iface == "IDirect3DDevice9":
            dev_funcs[(fva, fname)] += 1
        sites.append({"function": f"{fva:08X}", "name": fname, "call": f"{cva:08X}",
                      "interface": iface, "method": name, "tier": tier})
    dev = ifaces["IDirect3DDevice9"]
    data = {
        "source": "tools/modern/d3d9_device_census.py",
        "functions_scanned": len(c.funcs),
        "call_sites": len(sites),
        "out_of_range_slots": bad,
        "seed_globals": {f"{k:08X}": v for k, v in SEED_GLOBALS.items()},
        "static_instances": len(c.statics),
        "rtti_typed_methods": len(c.rtti_methods),
        "typed_globals": len(c.globals),
        "typed_methods": len(c.this_type),
        "interfaces": {i: {m: {"call_sites": n, "functions": len(per_funcs[i][m])}
                           for m, n in per[i].most_common()} for i in sorted(per)},
        "sites": sites,
    }
    OUT_JSON.write_text(json.dumps(data, indent=1) + "\n")

    used = [m for m in dev if per["IDirect3DDevice9"][m]]
    nd = sum(per["IDirect3DDevice9"].values())
    L = [
        "# Direct3D 9 call-site census",
        "",
        "Generated by `tools/modern/d3d9_device_census.py` from the retail `Fable.exe`. Do not edit by",
        "hand; every site is in `D3D9_DEVICE_CENSUS.json`. Sizes the RHI for",
        "[MODERNIZATION](MODERNIZATION.md) step 4.",
        "",
        "Method: capstone over every non-thunk function, with registers typed from the PDB layouts.",
        "Seeds: the render-manager pointers `[0x1436E18]` / `[0x13CB508]`, the static `CSystemManager`",
        "`0x13CA618`, the live RTTI sweep of globals (`ghidra_out/rtti_live_globals.tsv`) and `this` of",
        "every method in a retail RTTI vtable. Types spread into callees (unanimous or >= 80% of >= 3 typed",
        "callers), globals, getters, embedded structs and member pointers. A site counts when the same",
        "load supplies the vtable and the COM `this` (pushed, or stored to `[esp+4]` for a tail jump).",
        "Still a lower bound (stack-spilled pointers are not followed), but against the live frontend",
        "trace it finds 54 of 55 call sites ([D3D9_LIVE_CENSUS](D3D9_LIVE_CENSUS.md)). Method order is",
        "the DirectX SDK `d3d9.h`.",
        "",
        f"Scanned {len(c.funcs)} functions; typed {len(c.this_type)} methods and {len(c.globals)} globals.",
        f"**IDirect3DDevice9: {nd} call sites in {len(dev_funcs)} functions, {len(used)} of 119 methods.**",
        f"Tier A (PDB-typed): {tiers[(True, 'A')]} device sites; tier B (retail device offset: the unique",
        f"+0x3CF0, or a class-only method name from Ego_r.pdb): {tiers[(True, 'B')]}. Tier B covers",
        "`CShaderRenderManager`, whose retail device member is +0x1D4 although Ego_r.pdb says +0x1E4.",
        "",
        "## IDirect3DDevice9 methods in use",
        "",
        "| Slot | Method | Call sites | Functions |",
        "| ---: | --- | ---: | ---: |",
    ]
    for m in sorted(used, key=lambda m: (-per["IDirect3DDevice9"][m], dev.index(m))):
        L.append(f"| {dev.index(m)} | `{m}` | {per['IDirect3DDevice9'][m]} | "
                 f"{len(per_funcs['IDirect3DDevice9'][m])} |")
    L += ["", "## IDirect3DDevice9 methods not seen", "",
          ", ".join(f"`{m}`" for m in dev if not per["IDirect3DDevice9"][m]), "",
          "Not seen is not proof of unused: see the lower-bound note above.", "",
          "## Other Direct3D 9 interfaces", "", "| Interface | Methods (call sites) |", "| --- | --- |"]
    for i in sorted(per):
        if i != "IDirect3DDevice9":
            L.append(f"| `{i}` | " + ", ".join(f"`{m}` ({n})" for m, n in per[i].most_common()) + " |")
    L += ["", "## Functions with the most device calls", "", "| Function | Name | Device calls |",
          "| --- | --- | ---: |"]
    for (fva, fname), n in dev_funcs.most_common(40):
        L.append(f"| `{fva:08X}` | `{fname}` | {n} |")
    OUT_MD.write_text("\n".join(L) + "\n", encoding="utf-8")
    print(f"device: {nd} sites, {len(used)} methods, {len(dev_funcs)} functions; all D3D9: {len(sites)}")


def main() -> None:
    c = Census()
    calls = c.run()
    write_report(c, calls)


if __name__ == "__main__":
    main()
