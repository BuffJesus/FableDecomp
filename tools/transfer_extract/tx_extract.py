import struct, capstone, re, bisect, json, sys
BIN="debug_build/ego_r.exe"
f=open(BIN,"rb").read()
e=struct.unpack_from("<I",f,0x3C)[0]; coff=e+4
nsec=struct.unpack_from("<H",f,coff+2)[0]; opt=coff+20
imagebase=struct.unpack_from("<I",f,opt+28)[0]
secoff=opt+struct.unpack_from("<H",f,coff+16)[0]
secs=[]
for i in range(nsec):
    b=secoff+i*40
    va=struct.unpack_from("<I",f,b+12)[0]; vsz=struct.unpack_from("<I",f,b+8)[0]
    rsz=struct.unpack_from("<I",f,b+16)[0]; ptr=struct.unpack_from("<I",f,b+20)[0]
    secs.append((va,vsz,rsz,ptr))
def va2off(va):
    r=va-imagebase
    for va0,vsz,rsz,ptr in secs:
        if va0<=r<va0+max(vsz,rsz): return ptr+(r-va0)
    return None
def read_cstr(va):
    o=va2off(va)
    if o is None: return None
    end=f.find(b"\x00",o,o+128)
    if end<0: return None
    s=f[o:end]
    try: t=s.decode("ascii")
    except: return None
    if len(t)>=1 and all(32<=c<127 for c in s): return t
    return None
names={}
for line in open("ghidra_out/egor_pdb_names.tsv"):
    p=line.rstrip("\n").split("\t")
    if len(p)>=2 and re.match(r"^[0-9a-fA-F]{6,8}$",p[0]):
        names[int(p[0],16)]=p[1]
addrs=sorted(names)
def next_addr(a):
    i=bisect.bisect_right(addrs,a)
    return addrs[i] if i<len(addrs) else a+0x800
md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32); md.detail=True

# --- wire type from a CPersistContext::Transfer<...> mangled name ---
def wire_type(sym):
    # ??$Transfer@<TARG>@CPersistContext@@... or ??$TransferVector@<TARG>@... or ??$TransferEnum...
    # any CPersistContext::Transfer<...> family: Transfer, TransferVector,
    # TransferVectorOfSubComponents/OfComponents, TransferComponent, TransferEnum/Flags/Guid, ...
    m=re.match(r"\?\?\$(Transfer\w*)@(.+?)@CPersistContext@@",sym)
    if not m: return None,None
    kind=m.group(1); targ=m.group(2)
    return kind,targ
_TCODE={"J":"i32(long)","H":"i32(int)","_N":"bool","M":"f32","N":"f64",
        "K":"u32(ulong)","I":"u32(uint)","F":"i16(short)","G":"u16(ushort)",
        "E":"u8(uchar)","D":"i8(char)","_J":"i64","_K":"u64"}
def decode_targ(kind,targ):
    if targ in _TCODE: base=_TCODE[targ]
    elif targ.startswith("VCCharString"): base="String(CCharString)"
    elif targ.startswith("VCDefIndex"): base="DefIndex"
    elif targ.startswith("W4"):
        en=re.match(r"W4([A-Za-z0-9_]+)@",targ+"@"); base="enum "+(en.group(1) if en else targ)
    elif targ.startswith("V"):
        cn=re.match(r"V([A-Za-z0-9_]+)@",targ+"@"); base="struct "+(cn.group(1) if cn else targ)
    else: base=targ
    if kind=="Transfer": return base
    if kind=="TransferEnum": return "enum(%s)"%base
    if kind=="TransferFlags": return "flags(%s)"%base
    if "VectorOfSubComponents" in kind or "VectorOfComponents" in kind:
        return "vector<subdef %s>"%base
    if "Vector" in kind: return "vector<%s>"%base
    if "Component" in kind: return "subdef %s"%base
    return "%s<%s>"%(kind,base)

def extract(addr):
    end=next_addr(addr); off=va2off(addr)
    if off is None: return None
    code=f[off:off+(end-addr)]
    fields=[]; pending_name=None; last_field=None
    for ins in md.disasm(code,addr):
        m=ins.mnemonic; op=ins.op_str
        if m=="push":
            if ins.operands and ins.operands[0].type==capstone.x86.X86_OP_IMM:
                s=read_cstr(ins.operands[0].imm)
                if s is not None and re.match(r"^[A-Za-z_][A-Za-z0-9_ ]*$",s):
                    pending_name=s
        if m in ("mov","lea") and (op.startswith("ecx,") or op.startswith("eax,")):
            mm=re.search(r"\[(e[a-ds][ipx]) \+ (0x[0-9a-f]+)\]",op)
            if mm: last_field=int(mm.group(2),16)
        if m=="call":
            if ins.operands and ins.operands[0].type==capstone.x86.X86_OP_IMM:
                tgt=ins.operands[0].imm; sym=names.get(tgt,"")
                kind,targ=wire_type(sym)
                if kind:  # a CPersistContext::Transfer<...> field control
                    fields.append({"name":pending_name,"type":decode_targ(kind,targ),
                                   "kind":kind,"targ":targ,"off":last_field})
                elif sym.startswith("?Transfer@") and "Def@@" in sym:
                    dm=re.match(r"\?Transfer@(C[A-Za-z0-9_]+Def)@@",sym)
                    fields.append({"name":pending_name,"type":"base:%s"%(dm.group(1) if dm else sym),
                                   "kind":"base","targ":None,"off":last_field})
            else:
                fields.append({"name":pending_name,"type":"<indirect/subdef>","kind":"indirect","targ":None,"off":last_field})
            pending_name=None; last_field=None
    return fields

# Class name + optional namespace from a ?Transfer@<Class>[@<NS>]@@... symbol.
# Global classes are @@ right after the Def; namespaced ones (e.g. NUISystem UI
# defs, NSpeechGainManager CDialogueLayerDef) carry @N<Namespace>@@ in between.
_CLS_RE=re.compile(r"\?Transfer@(C[A-Za-z0-9_]+Def)(?:@(N[A-Za-z0-9_]+))?@@UAEXAAVCPersistContext@@@Z$")
def demangled_class(sym):
    m=_CLS_RE.match(sym); return m.group(1) if m else None
def demangled_ns(sym):
    m=_CLS_RE.match(sym); return m.group(2) if m else None

if __name__=="__main__":
    if len(sys.argv)>1 and sys.argv[1]=="--all":
        out={}
        for a,sym in names.items():
            if _CLS_RE.match(sym):
                cls=demangled_class(sym); ns=demangled_ns(sym)
                e={"addr":"%#x"%a,"fields":extract(a)}
                if ns: e["namespace"]=ns
                out[cls]=e
        json.dump(out,open("scratchpad_out.json","w"),indent=1)
        nf=sum(len(v["fields"]) for v in out.values())
        print("classes",len(out),"total field-controls",nf)
    else:
        for label,a in [("CChestDef",0x004f8afd),("CAbilityDef",0x00457f5b)]:
            print("\n==",label,"==")
            for i,fl in enumerate(extract(a),1):
                print("  %2d  off=%-6s %-22s name=%s"%(i,fl["off"],fl["type"],fl["name"]))
