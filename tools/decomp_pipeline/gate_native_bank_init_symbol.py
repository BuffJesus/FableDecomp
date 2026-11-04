"""Native bank filename path conversion with wide/narrow ownership."""
from pathlib import Path
from datetime import datetime,timezone
import sys,json,hashlib,re,struct,subprocess,itertools
import pefile
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/"rebuild/tests/integration"))
import NativeBankInitSymbol_model as M
SOURCES=[R/"rebuild/src/compiled"/n for n in ['00/9d/BankFilenameTree_InsertEqual_009d34e0.cpp',
 '00/42/BankFilenameTree_InsertNode_0042bcb5.cpp',
 '00/42/CBasicStringchar_operator_00429950.cpp',
 '00/42/STL_Rb_global_Rebalance_0042971d.cpp',
 '00/9c/CBankFile_InitEntry_009ce050.cpp',
 '00/99/CCharString_CCharString_0099ec30.cpp',
 '00/99/CCharString_Destructor_0099eae0.cpp',
 '00/99/CCharString_UnassignString_0099e9b0.cpp',
 '00/42/BankSymbolMap_Index_0042d131.cpp',
 '00/42/BankSymbolMap_LowerBound_0042ad34.cpp',
 '00/42/BankSymbolMap_InsertHint_0042c940.cpp',
 '00/42/BankSymbolMap_InsertUnique_0042bd4a.cpp']]
FIXTURE=R/"rebuild/tests/integration/NativeBankInitSymbol_test.cpp"
SPECS=[{'bytes': 87,
  'entry': '0x99e9b0',
  'fixups': 2,
  'owner': 'root7.obj',
  'symbol': '?UnassignString@CCharString@@AAEXXZ',
  'targets': {'??3@YAXPAX@Z': '0xbfe9bc', '??_V@YAXPAX@Z': '0xbfeb1c'}},
 {'bytes': 12,
  'entry': '0x99eae0',
  'fixups': 2,
  'owner': 'root6.obj',
  'symbol': '??1CCharString@@QAE@XZ',
  'targets': {'?UnassignString@CCharString@@AAEXXZ': '0x99e9b0',
              '?g_CCharStringInstanceCount_013BD800@@3HA': '0x13bd800'}},
 {'bytes': 93,
  'entry': '0x9d34e0',
  'fixups': 2,
  'owner': 'root0.obj',
  'symbol': '?insert_equal@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@2@ABU?$pair@$$CBVCCharString@@K@2@@Z',
  'targets': {'??M?$CBasicString@D@@QBE_NABV0@@Z': '0x429950',
              '?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@K@2@0@Z': '0x42bcb5'}},
 {'bytes': 149,
  'entry': '0x42bcb5',
  'fixups': 4,
  'owner': 'root1.obj',
  'symbol': '?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@K@2@0@Z',
  'targets': {'??R?$less@VCCharString@@@_STL@@QBE_NABVCCharString@@0@Z': '0x42aab3',
              '?_M_create_node@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@K@_STL@@@2@ABU?$pair@$$CBVCCharString@@K@2@@Z': '0x42b587',
              '?_Rebalance@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z': '0x42971d'}},
 {'bytes': 33,
  'entry': '0x42b587',
  'fixups': 2,
  'owner': 'root1.obj',
  'symbol': '?_M_create_node@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@K@_STL@@@2@ABU?$pair@$$CBVCCharString@@K@2@@Z',
  'targets': {'??0?$pair@$$CBVCCharString@@K@_STL@@QAE@ABU01@@Z': '0x4297db', '_MultimapMalloc': '0xbfea0e'}},
 {'bytes': 27,
  'entry': '0x4297db',
  'fixups': 1,
  'owner': 'root1.obj',
  'symbol': '??0?$pair@$$CBVCCharString@@K@_STL@@QAE@ABU01@@Z',
  'targets': {'??0CCharString@@QAE@ABV0@@Z': '0x99ec30'}},
 {'bytes': 41,
  'entry': '0x42aab3',
  'fixups': 1,
  'owner': 'root1.obj',
  'symbol': '??R?$less@VCCharString@@@_STL@@QBE_NABVCCharString@@0@Z',
  'targets': {'??M?$CBasicString@D@@QBE_NABV0@@Z': '0x429950'}},
 {'bytes': 88,
  'entry': '0x429950',
  'fixups': 0,
  'owner': 'root2.obj',
  'symbol': '??M?$CBasicString@D@@QBE_NABV0@@Z',
  'targets': {}},
 {'bytes': 190,
  'entry': '0x42971d',
  'fixups': 4,
  'owner': 'root3.obj',
  'symbol': '?_Rebalance@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z',
  'targets': {'?_Rotate_left@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z': '0x42951b',
              '?_Rotate_right@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z': '0x42955b'}},
 {'bytes': 64,
  'entry': '0x42951b',
  'fixups': 0,
  'owner': 'root3.obj',
  'symbol': '?_Rotate_left@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z',
  'targets': {}},
 {'bytes': 64,
  'entry': '0x42955b',
  'fixups': 0,
  'owner': 'root3.obj',
  'symbol': '?_Rotate_right@?$_Rb_global@_N@_STL@@SAXPAU_Rb_tree_node_base@2@AAPAU32@@Z',
  'targets': {}},
 {'bytes': 302,
  'entry': '0x9ce050',
  'fixups': 13,
  'owner': 'root4.obj',
  'symbol': '?InitEntry@CBankFile@@AAEXKABVCCharString@@0@Z',
  'targets': {'??0CCharString@@QAE@ABV0@@Z': '0x99ec30',
              '??1CCharString@@QAE@XZ': '0x99eae0',
              '??A?$map@VCCharString@@KU?$less@VCCharString@@@_STL@@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAEAAKABVCCharString@@@Z': '0x42d131',
              '?ConvertPathToFilename@CBankFile@@ABE?AVCCharString@@ABV2@@Z': '0x9cbe10',
              '?FableCRC_Calc_004014A0@@YIIIPBXI@Z': '0x4014a0',
              '?GetAsLowerCase@CCharString@@QBE?AV1@XZ': '0x99ed10',
              '?_M_insert_overflow@?$vector@U?$pair@KK@_STL@@V?$allocator@U?$pair@KK@_STL@@@2@@_STL@@QAEXPAU?$pair@KK@2@ABU32@ABU__false_type@2@I_N@Z': '0x9b6300',
              '?g_FableEmptyNarrowString_0129AAF4@@3PADA': '0x129aaf4',
              '?insert_equal@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@2@ABU?$pair@$$CBVCCharString@@K@2@@Z': '0x9d34e0'}},
 {'bytes': 61,
  'entry': '0x99ec30',
  'fixups': 3,
  'owner': 'root5.obj',
  'symbol': '??0CCharString@@QAE@ABV0@@Z',
  'targets': {'?UnassignString@CCharString@@AAEXXZ': '0x99e9b0',
              '?g_CCharStringInstanceCount_013BD800@@3HA': '0x13bd800'}},
 {'bytes': 96,
  'entry': '0x42d131',
  'fixups': 5,
  'owner': 'root8.obj',
  'symbol': '??A?$map@VCCharString@@KU?$less@VCCharString@@@_STL@@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAEAAKABVCCharString@@@Z',
  'targets': {'??0CCharString@@QAE@ABV0@@Z': '0x99ec30',
              '??1CCharString@@QAE@XZ': '0x99eae0',
              '??R?$less@VCCharString@@@_STL@@QBE_NABVCCharString@@0@Z': '0x42aab3',
              '?_M_lower_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QBEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@K@_STL@@@2@ABVCCharString@@@Z': '0x42ad34',
              '?insert@?$map@VCCharString@@KU?$less@VCCharString@@@_STL@@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@2@U32@ABU?$pair@$$CBVCCharString@@K@2@@Z': '0x42cda6'}},
 {'bytes': 56,
  'entry': '0x42ad34',
  'fixups': 1,
  'owner': 'root9.obj',
  'symbol': '?_M_lower_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QBEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@K@_STL@@@2@ABVCCharString@@@Z',
  'targets': {'??R?$less@VCCharString@@@_STL@@QBE_NABVCCharString@@0@Z': '0x42aab3'}},
 {'bytes': 29,
  'entry': '0x42cda6',
  'fixups': 1,
  'owner': 'root8.obj',
  'symbol': '?insert@?$map@VCCharString@@KU?$less@VCCharString@@@_STL@@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@2@U32@ABU?$pair@$$CBVCCharString@@K@2@@Z',
  'targets': {'?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@2@U32@ABU?$pair@$$CBVCCharString@@K@2@@Z': '0x42c940'}},
 {'bytes': 456,
  'entry': '0x42c940',
  'fixups': 13,
  'owner': 'root10.obj',
  'symbol': '?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@2@U32@ABU?$pair@$$CBVCCharString@@K@2@@Z',
  'targets': {'??R?$less@VCCharString@@@_STL@@QBE_NABVCCharString@@0@Z': '0x42aab3',
              '?_M_decrement@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@@Z': '0x4294e2',
              '?_M_increment@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@@Z': '0x4292c0',
              '?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@K@2@0@Z': '0x42bcb5',
              '?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@_STL@@_N@2@ABU?$pair@$$CBVCCharString@@K@2@@Z': '0x42bd4a'}},
 {'bytes': 44,
  'entry': '0x4292c0',
  'fixups': 0,
  'owner': 'root10.obj',
  'symbol': '?_M_increment@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@@Z',
  'targets': {}},
 {'bytes': 57,
  'entry': '0x4294e2',
  'fixups': 0,
  'owner': 'root10.obj',
  'symbol': '?_M_decrement@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@@Z',
  'targets': {}},
 {'bytes': 166,
  'entry': '0x42bd4a',
  'fixups': 4,
  'owner': 'root11.obj',
  'symbol': '?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@_STL@@_N@2@ABU?$pair@$$CBVCCharString@@K@2@@Z',
  'targets': {'??R?$less@VCCharString@@@_STL@@QBE_NABVCCharString@@0@Z': '0x42aab3',
              '?_M_decrement@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@@Z': '0x4294e2',
              '?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@K@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@K@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@K@_STL@@@3@@_STL@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@K@_STL@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@K@2@0@Z': '0x42bcb5'}}]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 
def main():
 inputs={Path(__file__),Path(M.__file__),EXE};visited=set()
 def closure(p):
  if p in visited:return
  visited.add(p);inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
 for p in SOURCES+[FIXTURE]:closure(p)
 inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
 for module in tuple(sys.modules.values()):
  name=getattr(module,'__file__',None)
  if name:
   p=Path(name).resolve()
   if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
 pins={str(p.resolve()):sha(p) for p in sorted(inputs)}
 b=R/'rebuild/build/native-bank-init-symbol/00/9c/009ce050'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir()
 def save(name,data):
  p=b/name;assert not p.exists();p.write_text(json.dumps(data,indent=2)+'\n')
 save('inputs.json',pins)
 for p in visited:
  q=b/('root%d.cpp'%SOURCES.index(p) if p in SOURCES else 'Fixture.cpp') if p in SOURCES+[FIXTURE] else inc/p.relative_to(R/'rebuild/include');q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[];coffs=[]
 for name in ['root%d'%i for i in range(len(SOURCES))]+['Fixture']:
  obj=b/(name+'.obj');result=V.cl(['/nologo','/c','/O2','/MT','/Gy',*(['/Dmalloc=MultimapMalloc'] if name=='root1' else []),'/I'+str(inc),'/Fo'+str(obj),str(b/(name+'.cpp'))],env,b);save(name+'.compile.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr;objects.append(obj);coffs.append(Coff(obj))

 candidate={};original={};image=EXE.read_bytes();proof=[]
 for row in SPECS:
  n=int(row['owner'][4:-4]);c=coffs[n];section,offset=c.symbols[row['symbol']];body,rels=c.sections[section];assert offset==0 and len(body)==row['bytes'] and len(rels)==row['fixups']
  entry=int(row['entry'],16);fixed=bytearray(body)
  for site,(name,kind) in rels.items():
   assert kind in (6,20);target=int(row['targets'][{'_PathMalloc':'_malloc','_PathFree':'_free'}.get(name,name)],16);struct.pack_into('<I',fixed,site,(target+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
  candidate[entry]=bytes(fixed);original[entry]=read_virtual_data(image,entry,len(body));assert candidate[entry]==original[entry],row['symbol'];proof.append(row)
 command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*[str(objects[i]) for i in (2,1,3,0,*range(4,len(objects)))]]
 result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);save('link.json',dict(command=command,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode,result.stdout+result.stderr
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if match:mapping[match[1]]=int(match[2],16)
 raw=(b/'probe.exe').read_bytes();pe=pefile.PE(data=raw);file_offsets={}
 for row in SPECS:
  n=int(row['owner'][4:-4]);symbol=row['symbol'];section,offset=coffs[n].symbols[symbol];body,rels=coffs[n].sections[section];fixed=bytearray(body);entry=mapping[symbol]
  for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
  assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body)),symbol;file_offsets[int(row['entry'],16)]=pe.get_offset_from_rva(entry-pe.OPTIONAL_HEADER.ImageBase)
 result=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=30);save('run.json',dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and 'cases=1600' in result.stdout
 cases=M.compare(candidate,original);assert cases==1600

 faults=[('symbol-flag',0x9ce050,0x13,b'\x20'),('payload-store',0x9ce050,0x22,b'\x90'*2),('temporary-destroy',0x42d131,0x52,b'\x90'*5),('lower-bound-branch',0x42ad34,0x20,b'\x74'),('return-field',0x42d131,0x5a,b'\x10'),('node-count',0x42bcb5,0x84,b'\x90'*3),('leftmost-link',0x42bcb5,0x6a,b'\x90'*3),('rebalance',0x42bcb5,0x7f,b'\x90'*5),('allocation-size',0x42b587,2,b'\x20'),('copy-instance-count',0x99ec30,0x14,b'\x90')]
 controls=[]
 for address,prefixes in ((0x99e9b0,('??_V@YAXPAX@Z','??3@YAXPAX@Z')),(0x42971d,('?_Rotate_left','?_Rotate_right'))):
  row=next(r for r in SPECS if int(r['entry'],16)==address);c=coffs[int(row['owner'][4:-4])];sec,_=c.symbols[row['symbol']];_,rels=c.sections[sec]
  for prefix in prefixes:
   site=next(site for site,(name,kind) in rels.items() if name.startswith(prefix));faults.append((prefix,address,site-1,b'\x90'*5))
 for i,(name,entry,offset,replacement) in enumerate(faults):
  changed=dict(original);body=bytearray(changed[entry]);body[offset:offset+len(replacement)]=replacement;changed[entry]=bytes(body);detected=False
  for args in itertools.product((1,4,9,17),(0,1,2,3),(0,3),(0,1)):
   expected=M.execute(original,*args)
   try:actual=M.execute(changed,*args)
   except AssertionError:detected=True;break
   if actual!=expected:detected=True;break
  assert detected,(name,'oracle neutral')
  altered=bytearray(raw);start=file_offsets[entry]+offset;altered[start:start+len(replacement)]=replacement;exe=b/('control-%d.exe'%i);exe.write_bytes(altered)
  result=subprocess.run([str(exe)],cwd=b,capture_output=True,text=True,timeout=30);save('control-%d.run.json'%i,dict(name=name,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==80 and 'INIT_SYMBOL_FAILURE' in result.stderr,(name,result.returncode);controls.append(dict(name=name,oracle_detected=True,native_detected=True))
 for n in (1,3,4,5,6,7,8,9,10,11):
  cmd=['/out:'+str(b/('missing-%d.exe'%n)) if arg.startswith('/out:') else arg for arg in command if arg!=str(objects[n]) and not arg.startswith('/map:')];result=subprocess.run(cmd,cwd=b,env=env,capture_output=True,text=True);save('missing-%d.link.json'%n,dict(returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout)
 for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
 for p in inputs:
  if p.suffix=='.py' and p.is_relative_to(R):
   q=b/'python-provenance'/p.relative_to(R);q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
 report=dict(status='PASS',build=str(b),model_cases=cases,host_cases=1600,proof=proof,controls=controls,required_provider_omissions=10,input_sha256=pins,object_sha256={str(p):sha(p) for p in objects},executable_sha256=sha(b/'probe.exe'),boundary='Native InitEntry symbol branch through 20 exact original/final bodies, including seven new map lookup/insertion helpers and existing node/ownership providers. Original/rebuilt full128KiB arena digests and complete allocation/free event traces compared. 1600 native guarded scenarios cover null, empty, high-byte keys, insertion orders, aliases and repeated-symbol overwrite without duplicate nodes. Red-black invariants, header extrema, input key ownership and instance count checked; all guarded allocations released. Filename/CRC branches controlled and unused in baseline. Fixture anchors and manual tree teardown explicit; no allocation-failure/unwind claim.')
 save('receipt.json',report);(R/'rebuild/compile-gate/native-bank-init-symbol.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_BANK_INIT_SYMBOL_PASS model=1600 host=1600 controls=28 bodies=20')
if __name__=='__main__':main()
