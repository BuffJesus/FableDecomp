from pathlib import Path
from datetime import datetime, timezone
import sys, json, hashlib, re, subprocess, struct
import argparse,pefile,shutil
R=Path(__file__).resolve().parents[2]
parser=argparse.ArgumentParser();parser.add_argument('--staging',type=Path);parser.add_argument('--output',type=Path);args=parser.parse_args()
w=args.staging.resolve() if args.staging else None;overlay=w/'include' if w else R/'rebuild/include'
b=args.output.resolve() if args.output else R/'rebuild/build/native-static-font-prepare'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True)
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from pe_oracle import EXE, read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from crawl import purity
helpers=['01/39/CRenderStateManager_StaticData_01396da8.cpp','00/a0/CRenderManagerCore_SetVertexFormat_00a0aa30.cpp','01/39/CRenderStateManager_D3DAlphaFuncModeTable_01396dd8.cpp','01/39/CRenderStateManager_D3DTextureOperationModeTable_01396e70.cpp']
sources=[w/'Candidate.cpp' if w else R/'rebuild/src/compiled/00/ab/CStaticFontBank_PrepareToRender_00ab83a0.cpp']+[R/'rebuild/src/compiled'/p for p in helpers]+[w/'Fixture.cpp' if w else R/'rebuild/tests/integration/NativeStaticFontPrepare_test.cpp']
assert purity.is_genuine(sources[0].read_text())
inc=b/'include';inc.mkdir();inputs={EXE,Path(__file__)};seen=set()
def closure(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):
  closure(next(q for q in (p.parent/name,overlay/name,R/'rebuild/include'/name) if q.is_file()))
for p in sources:closure(p)
for p in seen:
 if p.is_relative_to(overlay):q=inc/p.relative_to(overlay)
 elif p.is_relative_to(R/'rebuild/include'):q=inc/p.relative_to(R/'rebuild/include')
 else:continue
 q.parent.mkdir(parents=True,exist_ok=True);q.write_bytes(p.read_bytes())
inputs.update(inc.rglob('*.h'))
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  p=Path(name).resolve()
  if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2))
env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc);objects=[]
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj')
 x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(snapshot)],env,b)
 (b/('unit'+str(i)+'.compile.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)))
 assert not x.returncode,x.stdout+x.stderr;objects.append(obj)
exe=b/'probe.exe';x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/map:'+str(b/'probe.map'),'/out:'+str(exe),*map(str,objects)],env=env,cwd=b,capture_output=True,text=True)
(b/'link.json').write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr
mapped={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapped[m[1]]=int(m[2],16)
targets={0x9a4ec0:'?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0xa0aa30:'?SetVertexFormat@CRenderManagerCore@@QAEXK@Z'}
constants={address:('?FableRenderDefaultValue_'+format(address,'08x')+'@@3KA',0) for address in (0x1396f78,0x1396f7c,0x1396df0,0x1396fa4,0x1396fb0,0x1396e5c,0x1396dc8,0x1396fcc,0x1396e7c,0x1396ef0,0x1396eec,0x1396e70)}
constants[0x1396e64]=('?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA',0)
constants[0x1396df0]=('?D3DAlphaFuncModeTable@CRenderStateManager@@0PAKA',24)
constants[0x1396e7c]=('?D3DTextureOperationModeTable@CRenderStateManager@@0PAKA',12)
constants[0x1396e70]=('?D3DTextureOperationModeTable@CRenderStateManager@@0PAKA',0)
data=EXE.read_bytes();original=read_virtual_data(data,0xab83a0,2672);assert original[-3:]==b'\xc2\x04\0';(b/'original.bin').write_bytes(original)
md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;patches=[]
for i in md.disasm(original,0xab83a0):
 if i.mnemonic=='call' and i.operands[0].type==2:
  address=i.operands[0].imm;patches.append((i.address-0xab83a0+i.imm_offset,mapped[targets[address]],1))
 for address,(name,addend) in constants.items():
  value=struct.pack('<I',address)
  if value in i.bytes:patches.append((i.address-0xab83a0+bytes(i.bytes).index(value),mapped[name]+addend,0))
(b/'patches.bin').write_bytes(struct.pack('<I',len(patches))+b''.join(struct.pack('<III',*p) for p in patches));(b/'patches.json').write_text(json.dumps(patches,indent=2))
standalone=subprocess.run([str(exe)],capture_output=True,text=True,timeout=60)
standalone_record=dict(code=standalone.returncode,stdout=standalone.stdout,stderr=standalone.stderr);assert not standalone.returncode,standalone_record
(b/'standalone.json').write_text(json.dumps(standalone_record,indent=2))
x=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=60)
result=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);(b/'run.json').write_text(json.dumps(result,indent=2));print(result,flush=True)
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
assert not x.returncode,result
r=dict(build=str(b),input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources={str(o):str(p) for o,p in zip(objects,sources)})
positive=result
root_symbol='?PrepareToRender@CStaticFontBank@@UBEX_N@Z'
coff=Coff(objects[0]);section,offset=coff.symbols[root_symbol];raw,relocations=coff.sections[section];assert offset==0
original=read_virtual_data(EXE.read_bytes(),0xab83a0,2672)
grade=compare(raw,original,tuple(relocations),retail_relocations('00ab83a0',2672));assert grade.near_match,grade
helper='?SetVertexFormat@CRenderManagerCore@@QAEXK@Z';c=Coff(objects[2]);section,offset=c.symbols[helper];body,rels=c.sections[section];assert offset==0 and len(body)==23 and not rels and body==read_virtual_data(EXE.read_bytes(),0xa0aa30,23)
mapped={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapped[m[1]]=int(m[2],16)
pe=pefile.PE(str(b/'probe.exe'));image=pe.OPTIONAL_HEADER.ImageBase
final_bytes=0
for index,symbol in ((0,root_symbol),(2,helper)):
 c=Coff(objects[index]);section,offset=c.symbols[symbol];body,rels=c.sections[section];assert offset==0;fixed=bytearray(body)
 for site,(name,kind) in rels.items():
  assert kind in (6,20)
  struct.pack_into('<I',fixed,site,(mapped[name]+struct.unpack_from('<i',body,site)[0]-(mapped[symbol]+site+4 if kind==20 else 0))&0xffffffff)
 assert pe.get_data(mapped[symbol]-image,len(body))==bytes(fixed),symbol
 final_bytes+=len(body)
data_specs=[(3,'?D3DAlphaFuncModeTable@CRenderStateManager@@0PAKA',0x1396dd8,32),(4,'?D3DTextureOperationModeTable@CRenderStateManager@@0PAKA',0x1396e70,96),(1,'?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA',0x1396e64,12)]
data_specs += [(0 if address==0x1396dc8 else 1,'?FableRenderDefaultValue_'+format(address,'08x')+'@@3KA',address,4) for address in (0x1396f78,0x1396f7c,0x1396fa4,0x1396fb0,0x1396e5c,0x1396dc8,0x1396fcc,0x1396ef0,0x1396eec)]
tables=[]
for index,symbol,address,size in data_specs:
 c=Coff(objects[index]);section,offset=c.symbols[symbol];body,rels=c.sections[section];assert not c.section_metadata[section]['characteristics']&0x20;assert not any(offset<=site<offset+size for site in rels)
 meta=c.section_metadata[section]
 if meta['characteristics']&0x80:
  assert not meta['raw_pointer'] and offset+size<=meta['raw_size']
  raw=bytes(size) # COFF uninitialized storage becomes loader-zeroed data.
 else:raw=body[offset:offset+size]
 assert len(raw)==size and raw==read_virtual_data(EXE.read_bytes(),address,size),symbol
 assert raw==pe.get_data(mapped[symbol]-image,size),symbol
 tables.append(dict(symbol=symbol,address=hex(address),bytes=size,original_exact=True,final_pe_exact=True))
assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in r['input_sha256'].items())
qualification=dict(status='PASS',grade=grade.to_dict(),root_bytes=2672,helper_original_bytes_exact=23,final_pe_function_bytes_exact=final_bytes,table_original_final_bytes_exact=sum(row['bytes'] for row in tables),table_proof=tables)
out=b/'controls';out.mkdir();records=[]
targets={0x9a4ec0:'?GFGetSystemManager@@YIPAVCSystemManager@@XZ',0xa0aa30:'?SetVertexFormat@CRenderManagerCore@@QAEXK@Z'}
constants={address:('?FableRenderDefaultValue_'+format(address,'08x')+'@@3KA',0) for address in (0x1396f78,0x1396f7c,0x1396df0,0x1396fa4,0x1396fb0,0x1396e5c,0x1396dc8,0x1396fcc,0x1396e7c,0x1396ef0,0x1396eec,0x1396e70)}
constants[0x1396e64]=('?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA',0)
constants[0x1396df0]=('?D3DAlphaFuncModeTable@CRenderStateManager@@0PAKA',24)
constants[0x1396e7c]=('?D3DTextureOperationModeTable@CRenderStateManager@@0PAKA',12)
constants[0x1396e70]=('?D3DTextureOperationModeTable@CRenderStateManager@@0PAKA',0)
md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;specs=[]
for i in md.disasm((b/'original.bin').read_bytes(),0xab83a0):
 if i.mnemonic=='call' and i.operands[0].type==2:specs.append((i.address-0xab83a0+i.imm_offset,targets[i.operands[0].imm],1,0))
 for address,(name,addend) in constants.items():
  value=struct.pack('<I',address)
  if value in i.bytes:specs.append((i.address-0xab83a0+bytes(i.bytes).index(value),name,0,addend))
def patches(mapping,path):
 names={}
 for line in mapping.read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:names[m[1]]=int(m[2],16)
 rows=[(site,names[name]+addend,kind) for site,name,kind,addend in specs];path.write_bytes(struct.pack('<I',len(rows))+b''.join(struct.pack('<III',*p) for p in rows))
def reject(name,items,missing=False):
 exe=out/('fault'+str(len(records))+'.exe');mapping=exe.with_suffix('.map')
 x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/map:'+str(mapping),'/out:'+str(exe),*['/include:'+name for name in targets.values()],*map(str,items)],env=env,capture_output=True,text=True)
 if missing:assert x.returncode;record=dict(name=name,link_rejected=True,stdout=x.stdout,stderr=x.stderr)
 else:
  assert not x.returncode,x.stdout+x.stderr
  p=exe.with_suffix('.patches.bin');patches(mapping,p)
  x=subprocess.run([str(exe),str(b/'original.bin'),str(p)],capture_output=True,text=True,timeout=60)
  record=dict(name=name,code=x.returncode,stdout=x.stdout,stderr=x.stderr);assert x.returncode==80 and 'FONT_PREPARE_FAILURE' in x.stderr,record
 records.append(record);(out/'progress.json').write_text(json.dumps(records,indent=2));print(name,'rejected',flush=True)
def compile(source,label,include=inc,expect_failure=False):
 obj=out/(label+'.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(include),'/Fo'+str(obj),str(source)],env,out)
 (out/(label+'.compile.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)))
 if expect_failure:assert x.returncode;return
 assert not x.returncode,x.stdout+x.stderr;return obj
text=Path(r['object_sources'][str(objects[0])]).read_text()
mutants=[
 ('blend-disabled','SetRenderState(1,render->DeviceState.AlphaBlendEnable)','SetRenderState(0,render->DeviceState.AlphaBlendEnable)'),
 ('alpha-reference','SetRenderState(8,render->DeviceState.AlphaRef)','SetRenderState(7,render->DeviceState.AlphaRef)'),
 ('wrong-alpha-test','SetRenderState(CRenderStateManager::D3DAlphaFuncModeTable[ALPHA_TEST_GREATER_EQUAL],','SetRenderState(FableRenderDefaultValue_01396de8,'),
 ('z-enable','SetRenderState(1,render->DeviceState.ZBufferEnable)','SetRenderState(0,render->DeviceState.ZBufferEnable)'),
 ('fog-enable','SetRenderState(0,render->DeviceState.FogEnable)','SetRenderState(1,render->DeviceState.FogEnable)'),
 ('wrong-filter','SetRenderState(FableRenderDefaultValue_01396e5c,','SetRenderState(FableRenderDefaultValue_01396e58,'),
 ('wrong-uv-mode','SetRenderState(FableRenderDefaultValue_01396dc8,','SetRenderState(FableRenderDefaultValue_01396dd0,'),
 ('always-format','if(setupVertexFormat){','if(true){'),
 ('never-format','if(setupVertexFormat){','if(false){'),
 ('format-value','SetVertexFormat(0x144)','SetVertexFormat(0x142)'),
 ('format-call-omitted','render->SetVertexFormat(0x144);',';'),
 ('live-modulation-table','render->SetVertexFormat(0x144);','unsigned long cachedMod=CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_MODULATE];render->SetVertexFormat(0x144);'),
 ('stage-one-disable','SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_DISABLE],','SetRenderState(FableRenderDefaultValue_01396e74,'),
]
for name,old,new in mutants:
 assert old in text,name;body=text.replace(old,new)
 if name=='live-modulation-table':body=body.replace('SetRenderState(CRenderStateManager::D3DTextureOperationModeTable[TEXTURE_OP_MODULATE],','SetRenderState(cachedMod,')
 source=out/(name+'.cpp');source.write_text(body);reject(name,[compile(source,name),*objects[1:]])
header=(inc/'fable_render_state_manager.h').read_text()
header_faults=[
 ('bookmark-condition','(CurrentBookmarkMask & info.BookmarkMask) == 0','(CurrentBookmarkMask & info.BookmarkMask) != 0'),
 ('bookmark-mask','info.BookmarkMask |= CurrentBookmarkMask;',';'),
 ('bookmark-old-value','StateStack[StateStackSize].Value = info.DesiredState;','StateStack[StateStackSize].Value = value;'),
 ('stack-count','++StateStackSize;',';'),
 ('desired-state','info.DesiredState = value;',';'),
 ('dirty-state','if (!info.DirtyListFlag)','if (false)'),
 ('dirty-duplicate','if (!info.DirtyListFlag)','if (true)'),
 ('unchanged-state','if (info.DesiredState != value)','if (true)'),
]
for name,old,new in header_faults:
 assert old in header,name;overlay=out/(name+'-include');shutil.copytree(inc,overlay);(overlay/'fable_render_state_manager.h').write_text(header.replace(old,new))
 reject(name,[compile(Path(r['object_sources'][str(objects[0])]),name,overlay),*objects[1:]])
overlay=out/'no-friend-include';shutil.copytree(inc,overlay);needle='    friend class CStaticFontBank; // Narrow access for typed font-state reconstruction.\n';assert needle in header;(overlay/'fable_render_state_manager.h').write_text(header.replace(needle,''));compile(Path(r['object_sources'][str(objects[0])]),'no-font-friend',overlay,True);records.append(dict(name='no-font-friend',compile_rejected=True))
for index in (0,1,2,3,4):reject('missing-provider-'+str(index),[p for i,p in enumerate(objects) if i!=index],True)
body=bytearray((b/'original.bin').read_bytes());site=0xab8a69-0xab83a0;assert body[site:site+2]==b'\x0f\x84';body[site+1]=0x85;mutant=out/'original-boolean-branch.bin';mutant.write_bytes(body)
x=subprocess.run([str(b/'probe.exe'),str(mutant),str(b/'patches.bin')],capture_output=True,text=True,timeout=60);assert x.returncode==80 and 'FONT_PREPARE_FAILURE' in x.stderr;records.append(dict(name='original-boolean-branch',code=x.returncode,stderr=x.stderr))
assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==h for p,h in r['input_sha256'].items()),'input drift'
again=subprocess.run([str(exe),str(b/'original.bin'),str(b/'patches.bin')],capture_output=True,text=True,timeout=60)
assert not again.returncode and again.stdout==positive['stdout'],again
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
report=dict(status='PASS',cases=2304,paths=4608,repeat=True,qualification=qualification,controls=records,run=positive,standalone=standalone_record,**r,boundary='Typed static-font state setup, actual private inline bookmark/dirty setters and SetVertexFormat. Independent whole-manager model plus original instructions. Borrowed system/display/render/font storage and controlled SetFVF, with live table/state mutation during callback and ignored failure HRESULT. Explicit existing shared alpha/op/mipmap tables. No device-state realization, GPU/raster or game certification.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None and w is None:(R/'rebuild/compile-gate/native-static-font-prepare.json').write_text(json.dumps(report,indent=2))
print('NATIVE_STATIC_FONT_PREPARE_GATE_PASS cases=2304 paths=4608 controls='+str(len(records)),b,flush=True)
