"""List a class's PDB methods, data members and vtable offsets via the DIA SDK.

usage: python tools/pdb_class_methods.py CLASS [--pdb debug_build/Ego_r.pdb]
Uses the Visual Studio BuildTools msdia140.dll without registry registration.
Addresses printed are the PDB build's RVAs (Ego_r/FableWin), never retail addresses.
"""
import argparse,ctypes,os,sys
from pathlib import Path
import comtypes,comtypes.client
from comtypes.server import IClassFactory
DIA=Path(r'C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\DIA SDK\bin')/('amd64' if sys.maxsize>2**32 else '')/'msdia140.dll'
ROOT=Path(__file__).resolve().parents[1]
def source():
    dia=comtypes.client.GetModule(str(DIA))
    lib=ctypes.OleDLL(str(DIA));factory=ctypes.POINTER(comtypes.IUnknown)()
    clsid=dia.DiaSource._reg_clsid_;iid=IClassFactory._iid_
    lib.DllGetClassObject(ctypes.byref(clsid),ctypes.byref(iid),ctypes.byref(factory))
    return dia,factory.QueryInterface(IClassFactory).CreateInstance(interface=dia.IDiaDataSource)
def children(dia,sym,tag):
    out=[];e=sym.findChildren(tag,None,0)
    for i in range(e.count):out.append(e.Item(i))
    return out
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('cls');ap.add_argument('--pdb',default=str(ROOT/'debug_build/Ego_r.pdb'));a=ap.parse_args()
    dia,src=source();src.loadDataFromPdb(a.pdb);session=src.openSession();g=session.globalScope
    SymTagUDT,SymTagFunction,SymTagData,SymTagBaseClass=11,5,7,18
    for udt in children(dia,g,SymTagUDT):
        if udt.name!=a.cls:continue
        print('class',udt.name,'size',hex(udt.length))
        for b in children(dia,udt,SymTagBaseClass):print('  base',b.name,'+'+hex(b.offset))
        for d in children(dia,udt,SymTagData):
            try:print('  data +%#x %s %s'%(d.offset,d.type.name or '',d.name))
            except Exception:print('  data (static)',d.name)
        for f in children(dia,udt,SymTagFunction):
            virt=' virtual slot=%d'%(f.virtualBaseOffset//4) if f.virtual else ''
            print('  func rva=%#x len=%d %s%s'%(f.relativeVirtualAddress,f.length,f.undecoratedName or f.name,virt))
        break
if __name__=='__main__':main()

def publics(pattern, pdb):
    """Decorated public symbols containing `pattern` (the build's own mangling)."""
    dia,src=source();src.loadDataFromPdb(pdb);g=src.openSession().globalScope
    e=g.findChildren(10,None,0);out=[]  # SymTagPublicSymbol
    for i in range(e.count):
        s=e.Item(i);n=s.name or ''
        if pattern in n:out.append((s.relativeVirtualAddress,n))
    return out
