"""Record each type's class key (class vs struct) from FableWin.pdb's decorated publics.

The class key is part of every decoration that names the type (`AAV` vs `AAU`),
so a header that says `struct` for a retail `class` links against names retail
never defines. Writes ghidra_out/class_keys_fablewin.tsv: name, key, V-count,
U-count. Types that never appear in a decoration are absent (no evidence).

usage: python tools/decomp_pipeline/class_keys.py [--pdb debug_build/FableWin.pdb]
"""
import argparse,collections,re,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
OUT=ROOT/'ghidra_out/class_keys_fablewin.tsv'
PATTERN=re.compile(r'[ABPQ]([VU])([A-Za-z_][A-Za-z0-9_]*)@@')

def load(path=OUT):
    """name -> 'class' | 'struct' for types with unambiguous evidence."""
    keys={}
    if path.exists():
        for line in path.read_text().splitlines()[1:]:
            name,key,_,_=line.split('\t');keys[name]=key
    return keys

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--pdb',default=str(ROOT/'debug_build/FableWin.pdb'));a=ap.parse_args()
    sys.path.insert(0,str(ROOT/'tools'))
    import pdb_class_methods as P
    dia,src=P.source();src.loadDataFromPdb(a.pdb);e=src.openSession().globalScope.findChildren(10,None,0)
    counts=collections.defaultdict(lambda:[0,0])
    for i in range(e.count):
        for key,name in PATTERN.findall(e.Item(i).name or ''):counts[name][key=='U']+=1
    rows=['name\tkey\tV\tU']
    for name in sorted(counts):
        v,u=counts[name]
        if v and u:continue  # ambiguous: leave without evidence
        rows.append('%s\t%s\t%d\t%d'%(name,'class' if v else 'struct',v,u))
    OUT.write_text('\n'.join(rows)+'\n');print('CLASS_KEYS',len(rows)-1,'types ->',OUT)
if __name__=='__main__':main()
