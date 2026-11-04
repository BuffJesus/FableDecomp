import sys,json,tempfile,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one
from variable_allocator_parity import check
from gate_game_component_abi import Coff
from pe_oracle import EXE
import pefile
work=Path(tempfile.mkdtemp(prefix='allocator-support-'));obj=work/'support.obj'
r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(ROOT/'rebuild/src/compiled/00/9c/FileInstallerDestroyBufferRange_009c6d60.cpp')],check_one.env(),work)
assert r.returncode==0,r.stdout+r.stderr
targets={'__imp__VirtualFree@12':0x143fd40,'__imp__EnterCriticalSection@4':0x143fe0c,'__imp__LeaveCriticalSection@4':0x143fe10,'?g_CMemoryAllocatorCriticalSection_013CAA70@@3U_RTL_CRITICAL_SECTION@@A':0x13caa70}
reports=[]
for symbol,address,size,count in [('??3CMemoryAllocator@@KIXPAX@Z',0x9f1f90,15,1),('?EnterSynchronisation@CMemoryAllocator@@KIXXZ',0x9f2000,12,2),('?LeaveSynchronisation@CMemoryAllocator@@KIXXZ',0x9f2010,12,2)]:
 c=Coff(obj);section,offset=c.symbols[symbol];body,relocs=c.sections[section];assert offset==0 and len(relocs)==count
 patched=bytearray(body)
 for site,(target,kind)in relocs.items():
  assert kind==6
  struct.pack_into('<I',patched,site,targets[target]+struct.unpack_from('<I',body,site)[0])
 with pefile.PE(str(EXE))as pe:retail=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,size)
 assert patched==retail,(symbol,patched.hex(),retail.hex())
 reports.append(dict(address=hex(address),symbol=symbol,grade='MATCH',bytes=len(body),relocations=len(relocs),verification='all relocations resolved to retail targets; complete body byte equality'))
(ROOT/'rebuild/compile-gate/installer-cleanup-allocator-support-parity.json').write_text(json.dumps(reports,indent=2)+'\n')
print(json.dumps(reports,indent=2))
