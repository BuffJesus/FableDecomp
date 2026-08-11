import json,re,glob
SC="C:/Users/Cornelio/AppData/Local/Temp/claude/D--Documents-FableTLC/c1506a19-6449-4e31-9da7-38998d0c1b33/scratchpad"
ours=json.load(open(SC+"/transfer_orders.json"))
DEFDIR="C:/Users/Cornelio/Documents/EgoCoreInspect/fable-defs/packages/defs/src/def"
fd={}
for p in glob.glob(DEFDIR+"/*.rs"):
    txt=open(p,encoding="utf-8").read()
    sm=re.search(r"pub struct (\w+Def)\b",txt)
    if not sm: continue
    fd["C"+sm.group(1)]=re.findall(r'#\[def\("([^"]+)"',txt)
def subseq(short,long):
    it=iter(long); return all(any(x==y for y in it) for x in short)
out={
 "_meta":{
  "artifact":"Fable: The Lost Chapters — CDefBase::Transfer field orders",
  "source_binary":"debug_build/ego_r.exe (Anniversary 2013-02-25 debug build, ImageBase 0x400000)",
  "symbols":"ghidra_out/egor_pdb_names.tsv (FableWin/ego_r PDB, demangled)",
  "method":"linear disasm of each ?Transfer@C*Def@@UAEXAAVCPersistContext@@@Z; each CPersistContext::Transfer<T>(const char* name, T& field, ...) call = one game.bin field control, in call order. name=1st arg string literal, wire type T from the template arg of the call target symbol.",
  "wire_type_legend":{"J":"i32 (long)","H":"i32 (int)","_N":"bool","M":"f32","N":"f64","K":"u32 (ulong)","I":"u32 (uint)","F":"i16","G":"u16","E":"u8","D":"i8","VCCharString":"String/DefString","VCDefIndex":"DefIndex","W4X":"enum X (i32 wire)","TransferVector<T>":"Vec<T>"},
  "validation":"CChestDef 6/6 vs fable-defs chest.rs incl. OpenerObject off=0x34 (independently RE-verified). 251/262 classes are an exact order-preserving supersequence of fable-defs #[def] order.",
  "known_gap":"Sub-component COLLECTION fields (Vec of sub-def: Abilities/Waves/Creatures/Meshes/...) ARE captured (inline TransferVectorOfSubComponents<T>, name+order+element-type recovered). Residual 11 'partial' classes are NOT a data gap: they are base-class FLATTENING differences - fable-defs inlines base fields (e.g. CThingBaseDef's Components/PersistenceFlags/Health) into each concrete C*Def, while our extraction chains via a 'base:<Class>' marker entry, so the fields live under that base class's own record. 6 UI/frontend defs (CUiDef/CUiIconsDef/CUiMiscThingsDef/CUiStateDef/CUILocaleGraphicsDef/CDialogueLayerDef) have no matching *Def::Transfer symbol (different transfer signature) - TODO if wanted.",
  "license":"Derived from our own RE of a freely-distributed debug build; shareable. fable-defs is Zlib."
 }
}
per={}
for cls,v in ours.items():
    fields=[{k:fl[k] for k in ("name","type","off") if fl.get(k) is not None} for fl in v["fields"]]
    entry={"addr":v["addr"],"fields":fields}
    if cls in fd:
        ourn=[fl["name"] for fl in v["fields"] if fl["name"]]
        entry["fable_defs_order_match"]= "exact" if subseq(fd[cls],ourn) else "partial(base-class fields under base:* marker / minor order)"
    per[cls]=entry
out["defs"]={k:per[k] for k in sorted(per)}
json.dump(out,open("refs/transfer_field_orders.json","w"),indent=1)
print("wrote refs/transfer_field_orders.json")
print("classes",len(per),"fields",sum(len(v["fields"]) for v in per.values()))
