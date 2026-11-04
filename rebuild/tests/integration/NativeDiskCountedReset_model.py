from pathlib import Path
import sys,struct,json,itertools,types
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from NativeDiskCountedReset_state import collect
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
import threaded_file_counted_delete_oracle as D
def compare_disk_reset(obj):
 c=Coff(obj);s,o=c.symbols['?Reset@?$CCountedPointer@VCDiskFileWin32@@@@QAEXPAVCDiskFileWin32@@@Z'];body,rels=c.sections[s]
 addresses={'??3@YAXPAX@Z':0xbfe9bc,'??2@YAPAXI@Z':0xbfea1a,'?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@CIXPAX@Z':0x9a9030};patched=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',patched,site,(addresses[name]+struct.unpack_from('<i',body,site)[0]-(0x9d1ee0+site+4 if kind==20 else 0))&0xffffffff)
 original=read_virtual_data(EXE.read_bytes(),0x9d1ee0,103);assert bytes(patched)==original
 before=collect();after=collect(bytes(patched));assert before==after
 detected=[]
 for name,bad in [('missing-reset',b'\xc2\x04\x00'),('wrong-delete-callback',bytes(patched).replace(struct.pack('<I',0x9a9030),struct.pack('<I',0x9a9040)))]:
  assert bad!=bytes(patched)
  try:collect(bad)
  except AssertionError:detected.append(name)
  else:raise AssertionError(name)
 s,o=c.symbols['?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@CIXPAX@Z'];body,rels=c.sections[s];assert not o and not rels and body==read_virtual_data(EXE.read_bytes(),0x9a9030,11)
 execute=types.FunctionType(D.execute.__code__,dict(D.execute.__globals__,ENTRY=0x9a9030));cases=0
 for args in itertools.product((False,True),range(4),range(2),range(4),range(4)):
  assert execute(body,args)==execute(read_virtual_data(EXE.read_bytes(),0x9a9030,11),args);cases+=1
 try:execute(b'\xc3',(True,1,1,3,3))
 except AssertionError:detected.append('missing-delete')
 else:raise AssertionError('Undetected delete')
 return dict(status='PASS',grade='MATCH',cases=len(before)+cases,reset_cases=len(before),delete_cases=cases,controls=detected,boundary='Reset32KiB full-state/ABI/trace includes callback mutation and failed allocation; typed delete4KiB full-state/ABI includes slot mutation. Helpers controlled.')
