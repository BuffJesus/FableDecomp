#!/usr/bin/env python3
"""Minimal Lua 5.1 bytecode disassembler (reads Fable II .luac). Enough to trace API
calls: resolves GETGLOBAL / GETTABLE / SELF / LOADK / CALL so we can read what a script
actually does. Handles the header's declared endianness / int / size_t / number sizes."""
import struct, sys

OPNAMES = ["MOVE","LOADK","LOADBOOL","LOADNIL","GETUPVAL","GETGLOBAL","GETTABLE","SETGLOBAL",
"SETUPVAL","SETTABLE","NEWTABLE","SELF","ADD","SUB","MUL","DIV","MOD","POW","UNM","NOT","LEN",
"CONCAT","JMP","EQ","LT","LE","TEST","TESTSET","CALL","TAILCALL","RETURN","FORLOOP","FORPREP",
"TFORLOOP","SETLIST","CLOSE","CLOSURE","VARARG"]
# iABC / iABx / iAsBx per opcode
IABX = {1,5,7,36}          # LOADK, GETGLOBAL, SETGLOBAL, CLOSURE use Bx
IASBX = {22,31,32}         # JMP, FORLOOP, FORPREP use sBx

class R:
    def __init__(s, d):
        s.d = d; s.p = 0
    def u8(s):
        v = s.d[s.p]; s.p += 1; return v
    def raw(s, n):
        v = s.d[s.p:s.p+n]; s.p += n; return v

class Proto: pass

def parse(path):
    d = open(path, "rb").read()
    assert d[:4] == b"\x1bLua", "not lua bytecode"
    ver = d[4]; fmt = d[5]; endian = d[6]
    ints = d[7]; szt = d[8]; instr = d[9]; numsz = d[10]; integral = d[11]
    E = "<" if endian == 1 else ">"
    r = R(d); r.p = 12
    def rint():  return struct.unpack(E+"i", r.raw(ints))[0]
    def rszt():  return struct.unpack(E+("I" if szt==4 else "Q"), r.raw(szt))[0]
    def rinstr(): return struct.unpack(E+"I", r.raw(instr))[0]
    def rnum():
        b = r.raw(numsz)
        if integral: return struct.unpack(E+("i" if numsz==4 else "q"), b)[0]
        return struct.unpack(E+("f" if numsz==4 else "d"), b)[0]
    def rstr():
        n = rszt()
        if n == 0: return None
        s = r.raw(n)
        return s[:-1].decode("utf-8","replace")  # strip trailing \0

    def rproto():
        p = Proto()
        p.source = rstr(); p.linedef = rint(); p.lastline = rint()
        p.nups = r.u8(); p.nparams = r.u8(); p.isvararg = r.u8(); p.maxstack = r.u8()
        n = rint(); p.code = [rinstr() for _ in range(n)]
        n = rint(); p.k = []
        for _ in range(n):
            t = r.u8()
            if t == 0: p.k.append(None)
            elif t == 1: p.k.append(bool(r.u8()))
            elif t == 3: p.k.append(rnum())
            elif t == 4: p.k.append(rstr())
            else: raise ValueError("bad const type %d" % t)
        n = rint(); p.protos = [rproto() for _ in range(n)]
        # debug: lineinfo
        n = rint(); r.raw(n*ints)
        n = rint()
        for _ in range(n): rstr(); rint(); rint()   # locals
        n = rint()
        for _ in range(n): rstr()                    # upvalues
        return p
    return rproto()

def decode(ins):
    op = ins & 0x3F; A = (ins >> 6) & 0xFF
    C = (ins >> 14) & 0x1FF; B = (ins >> 23) & 0x1FF
    Bx = (ins >> 14) & 0x3FFFF; sBx = Bx - 131071
    return op, A, B, C, Bx, sBx

def kstr(p, idx):
    v = p.k[idx] if 0 <= idx < len(p.k) else "?"
    return repr(v)

def disasm(p, depth=0, name="main"):
    ind = "  "*depth
    print(f"{ind}function {name} (params={p.nparams} vararg={p.isvararg} stack={p.maxstack} consts={len(p.k)} protos={len(p.protos)})")
    for i, ins in enumerate(p.code):
        op, A, B, C, Bx, sBx = decode(ins)
        nm = OPNAMES[op] if op < len(OPNAMES) else f"OP{op}"
        note = ""
        if op == 5:   note = f"R{A} := _G[{kstr(p,Bx)}]"            # GETGLOBAL
        elif op == 7: note = f"_G[{kstr(p,Bx)}] := R{A}"            # SETGLOBAL
        elif op == 1: note = f"R{A} := {kstr(p,Bx)}"               # LOADK
        elif op == 6: note = f"R{A} := R{B}[{ 'K'+str(C-256)+'='+kstr(p,C-256) if C>=256 else 'R'+str(C)}]"  # GETTABLE
        elif op == 11:note = f"R{A+1}:=R{B}; R{A}:=R{B}[{kstr(p,C-256) if C>=256 else 'R'+str(C)}] (method)"    # SELF
        elif op == 28:note = f"call R{A} args={B-1 if B>0 else 'var'} ret={C-1 if C>0 else 'var'}"  # CALL
        elif op == 29:note = f"tailcall R{A}"
        elif op == 9: note = f"R{A}[{kstr(p,B-256) if B>=256 else 'R'+str(B)}] := {kstr(p,C-256) if C>=256 else 'R'+str(C)}"  # SETTABLE
        print(f"{ind}  {i:3} {nm:10} A={A} B={B} C={C} Bx={Bx}   {note}")
    for j, sp in enumerate(p.protos):
        disasm(sp, depth+1, f"proto[{j}]")

if __name__ == "__main__":
    p = parse(sys.argv[1])
    if len(sys.argv) > 2 and sys.argv[2] == "--strings":
        def walk(pr):
            for k in pr.k:
                if isinstance(k,str): print(" K:", repr(k))
            for sp in pr.protos: walk(sp)
        walk(p)
    else:
        disasm(p)
