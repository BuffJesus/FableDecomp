#!/usr/bin/env python3
"""Full Lua 5.1 bytecode disassembler (big-endian Xbox 360 chunks), recursive
over nested protos, with RK operands resolved to constants. Complements
luadis.py/script_index.py whose proto handling is partial.

Usage:
  python tools/lua_mod/luadis51.py <bnk> <entry-substring> [out.txt]
"""
import struct, sys, os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import bnk_repack, script_index

OPS = [
    "MOVE","LOADK","LOADBOOL","LOADNIL","GETUPVAL","GETGLOBAL","GETTABLE",
    "SETGLOBAL","SETUPVAL","SETTABLE","NEWTABLE","SELF","ADD","SUB","MUL",
    "DIV","MOD","POW","UNM","NOT","LEN","CONCAT","JMP","EQ","LT","LE","TEST",
    "TESTSET","CALL","TAILCALL","RETURN","FORLOOP","FORPREP","TFORLOOP",
    "SETLIST","CLOSE","CLOSURE","VARARG",
]

class R:
    def __init__(self, data):
        self.d = data
        self.p = 0
        # header: [6]=endianness (1=little — yes, even on the 360 build),
        # [10]=sizeof(lua_Number) (4 = float32 here).
        self.en = "<" if data[6] == 1 else ">"
        self.numfmt = (self.en + "f", 4) if data[10] == 4 else (self.en + "d", 8)
    def u8(self):
        v = self.d[self.p]; self.p += 1; return v
    def u32(self):
        v = struct.unpack_from(self.en + "I", self.d, self.p)[0]; self.p += 4; return v
    def dbl(self):
        fmt, sz = self.numfmt
        v = struct.unpack_from(fmt, self.d, self.p)[0]; self.p += sz; return v
    def s(self):
        n = self.u32()
        if n == 0: return None
        v = self.d[self.p:self.p+n-1].decode("latin-1"); self.p += n
        return v

def proto(r):
    p = {}
    p["source"] = r.s()
    p["line"], p["lastline"] = r.u32(), r.u32()
    p["nups"], p["nparams"], p["vararg"], p["stack"] = r.u8(), r.u8(), r.u8(), r.u8()
    p["code"] = [r.u32() for _ in range(r.u32())]
    consts = []
    for _ in range(r.u32()):
        t = r.u8()
        if t == 0: consts.append(None)
        elif t == 1: consts.append(bool(r.u8()))
        elif t == 3:
            v = r.dbl()
            consts.append(int(v) if v == int(v) else v)
        elif t == 4: consts.append(r.s())
        else: raise ValueError(f"const type {t}")
    p["consts"] = consts
    p["protos"] = [proto(r) for _ in range(r.u32())]
    # debug sections. NB: `r.p += 4 * r.u32()` would be wrong — Python loads
    # the old r.p before u32() advances it, silently rewinding 4 bytes.
    nli = r.u32()
    r.p += 4 * nli                          # lineinfo
    for _ in range(r.u32()):                # locvars
        r.s(); r.u32(); r.u32()
    upvals = [r.s() for _ in range(r.u32())]
    p["upvals"] = upvals
    return p

def k(p, i):
    c = p["consts"][i]
    return repr(c) if not isinstance(c, str) else "'" + c + "'"

def rk(p, x):
    return k(p, x - 256) if x >= 256 else f"R{x}"

def dis(p, name="main", depth=0, out=None):
    ind = "  " * depth
    out.append(f"{ind}function {name} (params={p['nparams']} vararg={p['vararg']} "
               f"stack={p['stack']} consts={len(p['consts'])} protos={len(p['protos'])})")
    for i, w in enumerate(p["code"]):
        op = w & 0x3F
        A = (w >> 6) & 0xFF
        C = (w >> 14) & 0x1FF
        B = (w >> 23) & 0x1FF
        Bx = (w >> 14) & 0x3FFFF
        sBx = Bx - 131071
        n = OPS[op] if op < len(OPS) else f"OP{op}"
        t = ""
        if n == "LOADK": t = f"R{A} := {k(p,Bx)}"
        elif n == "GETGLOBAL": t = f"R{A} := _G[{k(p,Bx)}]"
        elif n == "SETGLOBAL": t = f"_G[{k(p,Bx)}] := R{A}"
        elif n == "GETTABLE": t = f"R{A} := R{B}[{rk(p,C)}]"
        elif n == "SETTABLE": t = f"R{A}[{rk(p,B)}] := {rk(p,C)}"
        elif n == "SELF": t = f"R{A+1}:=R{B}; R{A}:=R{B}[{rk(p,C)}]"
        elif n == "MOVE": t = f"R{A} := R{B}"
        elif n == "LOADBOOL": t = f"R{A} := {bool(B)}{'; pc++' if C else ''}"
        elif n == "LOADNIL": t = f"R{A}..R{B} := nil"
        elif n == "NEWTABLE": t = f"R{A} := {{}} (arr~{B} hash~{C})"
        elif n == "CALL": t = f"R{A}(args={B-1 if B else 'var'}, ret={C-1 if C else 'var'})"
        elif n == "TAILCALL": t = f"return R{A}(args={B-1 if B else 'var'})"
        elif n == "RETURN": t = f"return R{A}..({B-1 if B else 'var'})"
        elif n == "JMP": t = f"-> {i+1+sBx}"
        elif n in ("EQ","LT","LE"): t = f"if ({rk(p,B)} {n} {rk(p,C)}) != {A} then pc++ ; else -> jmp"
        elif n == "TEST": t = f"if bool(R{A}) != {C} then pc++ (skip jmp)"
        elif n == "TESTSET": t = f"if bool(R{B}) == {C} then R{A}:=R{B} else pc++"
        elif n == "CONCAT": t = f"R{A} := R{B}..R{C}"
        elif n == "CLOSURE": t = f"R{A} := closure(proto[{Bx}])"
        elif n == "GETUPVAL": t = f"R{A} := up[{B}]"
        elif n == "SETUPVAL": t = f"up[{B}] := R{A}"
        elif n == "SETLIST": t = f"R{A}[..] := R{A+1}..R{A+B} (block {C})"
        elif n == "FORPREP": t = f"-> {i+1+sBx}"
        elif n == "FORLOOP": t = f"loop -> {i+1+sBx}"
        elif n == "TFORLOOP": t = f"iter R{A} (nres={C})"
        elif n == "VARARG": t = f"R{A}..R{A+B-2} := ..."
        elif n in ("ADD","SUB","MUL","DIV","MOD","POW"):
            t = f"R{A} := {rk(p,B)} {n} {rk(p,C)}"
        elif n in ("UNM","NOT","LEN"): t = f"R{A} := {n} R{B}"
        out.append(f"{ind}  {i:4d} {n:<10} A={A:<3} B={B:<3} C={C:<3}  {t}")
    for j, sub in enumerate(p["protos"]):
        dis(sub, f"{name}.proto[{j}]", depth + 1, out)

def main():
    bnk, needle = sys.argv[1], sys.argv[2]
    base, ents = bnk_repack.read_bnk(bnk)
    e = next(x for x in ents if needle.lower() in x["name"].lower())
    d = script_index.entry_bytes(e)
    assert d[:4] == b"\x1bLua", d[:8]
    r = R(d); r.p = 12
    p = proto(r)
    out = [f"; {e['name']} ({e['dsz']} bytes)"]
    dis(p, "main", 0, out)
    text = "\n".join(out)
    if len(sys.argv) > 3:
        open(sys.argv[3], "w", encoding="utf-8").write(text)
        print(f"wrote {sys.argv[3]} ({len(out)} lines)")
    else:
        print(text)

if __name__ == "__main__":
    main()
