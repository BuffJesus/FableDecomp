#!/usr/bin/env python3
"""Resolve retail RTTI vtable slots to the Ego_r method that implements them.

    python tools/decomp_pipeline/vtable_slot_methods.py --classes CBankFile CACompressor --out slots.json

For each class, walks Ego_r.pdb (DIA SDK, via tools/pdb_class_methods.py) and records, per
virtual slot, the nearest class in the hierarchy that declares a function at that slot (the
implementer for that class's vtable), the undecorated signature and the Ego_r decorated public
name (matched by RVA). Retail vtables come from RTTI (complete-object locators in .rdata);
Ego_r is the same compiler, so a slot's decorated name is the retail symbol the vtable needs.
"""
import argparse,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools'))
import pdb_class_methods as P
SymTagUDT,SymTagFunction,SymTagBaseClass,SymTagPublic=11,5,18,10
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--classes',nargs='+',default=[]);ap.add_argument('--classes-file')
    ap.add_argument('--pdb',default=str(ROOT/'debug_build/Ego_r.pdb'));ap.add_argument('--out',required=True);a=ap.parse_args()
    if a.classes_file:a.classes+=[l.strip() for l in open(a.classes_file) if l.strip()]
    dia,src=P.source();src.loadDataFromPdb(a.pdb);g=src.openSession().globalScope
    udts={}
    for u in P.children(dia,g,SymTagUDT):
        if u.name not in udts:udts[u.name]=u
    publics={}
    e=g.findChildren(SymTagPublic,None,0)
    for i in range(e.count):
        s=e.Item(i)
        if s.relativeVirtualAddress:publics.setdefault(s.relativeVirtualAddress,[]).append(s.name)
    cache={}
    def declared(name):
        if name in cache:return cache[name]
        u=udts.get(name);slots={};bases=[]
        if u is not None:
            for f in P.children(dia,u,SymTagFunction):
                if f.virtual:
                    k=f.virtualBaseOffset//4
                    rva=f.relativeVirtualAddress
                    slots.setdefault(k,dict(owner=name,name=f.name,signature=f.undecoratedName or f.name,ego_rva=rva,
                        decorated=[p for p in publics.get(rva,[]) if '?'+f.name.split('::')[-1]+'@'+name.split('::')[-1]+'@' in p]))
            bases=[b.name for b in P.children(dia,u,SymTagBaseClass) if b.offset==0]
        cache[name]=(slots,bases);return cache[name]
    def implementer(name,k,depth=0):
        slots,bases=declared(name)
        if k in slots:return slots[k]
        for b in bases:
            r=implementer(b,k,depth+1)
            if r:return r
        return None
    out={}
    for c in a.classes:
        slots,_=declared(c);found={}
        for k in range(0,512):
            r=implementer(c,k)
            if r:found[k]=r
        out[c]=found
    Path(a.out).write_text(json.dumps(out,indent=1))
    print('classes',len(out),'resolved slots',sum(len(v) for v in out.values()))
if __name__=='__main__':main()
