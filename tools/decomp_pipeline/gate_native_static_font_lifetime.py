from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,hashlib,struct,subprocess
R=Path(__file__).resolve().parents[2];
import argparse
parser=argparse.ArgumentParser();parser.add_argument('--staging',type=Path);parser.add_argument('--output',type=Path);args=parser.parse_args();stage=args.staging.resolve() if args.staging else None;b=args.output.resolve() if args.output else R/'rebuild/build/native-static-font-lifetime/00/ab/00ab7a70'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir();sys.path.insert(0,str(R/'tools/decomp_pipeline'));import check_one as V
from pe_oracle import EXE,read_virtual_data
from gate_game_component_abi import Coff
from bank_begin_worker_link_oracle import aliases
import pefile
helpers=['00/99/CBaseClass_CBaseClass_0099a2d0.cpp','00/99/CInitBaseClass_IsValid_0099a330.cpp','00/99/CCharString_DefaultConstructor_0099e4b0.cpp','00/99/CCharString_Destructor_0099eae0.cpp','00/99/CCharString_UnassignString_0099e9b0.cpp','00/99/CCharString_Constructor_0099ebf0.cpp','00/99/CCharString_AllocStringData_0099ea60.cpp','00/99/CCharString_CopyAssignment_0099efb0.cpp','00/99/CCharString_ConversionOperator_0099e4c0.cpp','00/9a/CCharString_InitFromCStr_009a0590.cpp','00/9a/global_CBasicStringCharAssignFromData_009a0300.cpp','00/9a/CFontBank_Preload_009a0b20.cpp','00/ab/CStaticFontBank_GetFontMaxHeight_00ab7a50.cpp','00/ab/CStaticFontBank_GetStringExtents_00ab7b00.cpp','00/9f/CTexture_Uninitialise_009f9f70.cpp'];source=stage/'Candidate.cpp' if stage else R/'rebuild/src/compiled/00/ab/CStaticFontBank_Lifetime_00ab7a70.cpp';fixture=stage/'CatalogFixture.cpp' if stage else R/'rebuild/tests/integration/NativeStaticFontLifetime_test.cpp';sources=[source]+[R/'rebuild/src/compiled'/p for p in helpers]+[fixture];inputs={EXE,Path(__file__)};seen=set()
def closure(p):
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()))
for p in sources:closure(p)
for p in seen:
 if p.is_relative_to(R/'rebuild/include'):
  target=inc/p.relative_to(R/'rebuild/include');target.parent.mkdir(parents=True,exist_ok=True);target.write_bytes(p.read_bytes())
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file())
inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  p=Path(name).resolve()
  if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)

sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2));objects=[];mapping={};env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)
for i,p in enumerate(sources):
 snapshot=b/('unit'+str(i)+'.cpp');snapshot.write_bytes(p.read_bytes());obj=snapshot.with_suffix('.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(snapshot)],env,b);(b/('unit'+str(i)+'.compile.json')).write_text(json.dumps(dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr;objects.append(obj);mapping[str(obj)]=str(p)
targets={'??_7CStaticFontBank@@6B@':0x129ead4,'??_V@YAXPAX@Z':0xbfeb1c,'?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,'?IsValid@CInitBaseClass@@QBE_NXZ':0x99a330,'?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,'??3@YAXPAX@Z':0xbfe9bc,'??_7CFontBank@@6B@':0x129ab38,'??1CCharString@@QAE@XZ':0x99eae0,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??1CStaticFontBank@@UAE@XZ':0xab7a70};c=Coff(objects[0]);originals=[];proof=[]
for symbol,address,size in [('??1CStaticFontBank@@UAE@XZ',0xab7a70,143),('??_GCStaticFontBank@@UAEPAXI@Z',0xab9990,30)]:
 sec,offset=c.symbols[symbol];raw,rels=c.sections[sec];fixed=bytearray(raw)
 for at,(name,kind) in rels.items():struct.pack_into('<I',fixed,at,(targets[name]+struct.unpack_from('<i',raw,at)[0]-(address+at+4 if kind==20 else 0))&0xffffffff)
 original=read_virtual_data(EXE.read_bytes(),address,size);assert offset==0 and len(raw)==size and bytes(fixed)==original;path=b/(hex(address)+'.bin');path.write_bytes(original);originals.append(path);proof.append(dict(symbol=symbol,address=hex(address),bytes=size,exact=True,relocations={str(k):v for k,v in rels.items()}))
exe=b/'probe.exe';command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),'/map:'+str(b/'probe.map'),*map(str,objects)];x=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(command=command,code=x.returncode,stdout=x.stdout,stderr=x.stderr)));assert not x.returncode,x.stdout+x.stderr
x=subprocess.run([str(exe),*map(str,originals)],capture_output=True,text=True,timeout=30);record=dict(code=x.returncode,stdout=x.stdout,stderr=x.stderr);(b/'run.json').write_text(json.dumps(record,indent=2));print(record,flush=True);assert not x.returncode,record
again=subprocess.run([str(exe),*map(str,originals)],capture_output=True,text=True,timeout=30);assert not again.returncode and again.stdout==x.stdout
assert all(sha(Path(p))==h for p,h in pins.items()),'input drift'
standalone=subprocess.run([str(exe)],capture_output=True,text=True,timeout=30)
assert not standalone.returncode and 'cases=1080 original=0 paths=1080' in standalone.stdout
assert 'cases=1080 original=1080 paths=2160' in record['stdout']
# Independently verify the complete static-font vtable, including MSVC's E->G alias.
section,offset=c.symbols['??_7CStaticFontBank@@6B@'];table,table_rels=c.sections[section];assert offset==0 and len(table)==24 and len(table_rels)==6
alias=aliases([objects[0]],b/('unit'+str(len(objects)-1)+'.cpp'));assert alias['??_ECStaticFontBank@@UAEPAXI@Z']=='??_GCStaticFontBank@@UAEPAXI@Z'
slot_targets={'??_GCStaticFontBank@@UAEPAXI@Z':0xab9990,'?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PBG_N@Z':0xab7b00,'?GetFontMaxHeight@CStaticFontBank@@UBEMXZ':0xab7a50,'?PrepareToRender@CStaticFontBank@@UBEX_N@Z':0xab83a0,'?Preload@CFontBank@@UBE_NABVCWideString@@_N@Z':0x9a0b20,'?Render@CStaticFontBank@@UBEXABVCWideString@@ABVC3DVector@@MABVCRGBColour@@KPBVCTexture@@@Z':0xab7c20}
fixed=bytearray(table)
for site,(name,kind) in table_rels.items():
 assert kind==6;struct.pack_into('<I',fixed,site,slot_targets[alias.get(name,name)]+struct.unpack_from('<i',table,site)[0])
assert bytes(fixed)==read_virtual_data(EXE.read_bytes(),0x129ead4,24)
# Final PE contributions must retain both destructor bodies and all six vtable pointers.
mapped={}
for line in (b/'probe.map').read_text().splitlines():
 match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if match:mapped[match[1]]=int(match[2],16)
pe=pefile.PE(str(exe))
for symbol,size in [('??1CStaticFontBank@@UAE@XZ',143),('??_GCStaticFontBank@@UAEPAXI@Z',30),('??_7CStaticFontBank@@6B@',24)]:
 section,offset=c.symbols[symbol];payload,relocations=c.sections[section];expected=bytearray(payload)
 for site,(target,kind) in relocations.items():
  target=alias.get(target,target);addend=struct.unpack_from('<i',payload,site)[0];struct.pack_into('<I',expected,site,(mapped[target]+addend-(mapped[symbol]+site+4 if kind==20 else 0))&0xffffffff)
 assert pe.get_data(mapped[symbol]-pe.OPTIONAL_HEADER.ImageBase,size)==bytes(expected)

out=b/'controls';out.mkdir();base=b;r={'object_sources':mapping};text=source.read_text();records=[]
def reject(path,items,label):
 exe=out/(label+'.exe');x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),*map(str,items)],env=env,capture_output=True,text=True);assert not x.returncode,x.stdout+x.stderr;x=subprocess.run([str(exe),str(path or base/'0xab7a70.bin'),str(base/'0xab9990.bin')],capture_output=True,text=True,timeout=30);rec=dict(name=label,code=x.returncode,stdout=x.stdout,stderr=x.stderr);records.append(rec);(out/'progress.json').write_text(json.dumps(records,indent=2));assert x.returncode==80 and 'STATIC_FONT_LIFETIME_FAILURE' in x.stderr,rec;print(label,'rejected',flush=True)
def compile(path,label):
 obj=out/(label+'.obj');x=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/Fo'+str(obj),str(path)],env,out);assert not x.returncode,x.stdout+x.stderr;return obj
specs=[('no-glyph-free','delete[] GlyphBanks[i].Glyphs;',';'),('no-glyph-clear','GlyphBanks[i].Glyphs=0;',';'),('no-context-guard','GFGetSystemManager() && GFGetSystemManager()->IsValid() && GFGetSystemManager()->GetDisplayManager()','true'),('no-validity-guard','GFGetSystemManager()->IsValid()','true'),('no-texture-delete','delete PTexture;',';'),('no-texture-clear','        PTexture=0;','        ;'),('clear-retained-texture','\n        PTexture=0;\n    }\n}','\n        PTexture=0;\n    }\n    PTexture=0;\n}'),('array-declaration-missing','#include <new>\n','')]
for label,old,new in specs:
 assert text.count(old)==1,(label,text.count(old));path=out/(label+'.cpp');path.write_text(text.replace(old,new));obj=compile(path,label);items=list(objects);items[0]=obj;reject(None,items,label)
helper=next(Path(p) for p,s in r['object_sources'].items() if Path(s).name=='CCharString_Destructor_0099eae0.cpp');original_helper=Path(r['object_sources'][str(helper)]);path=out/'name-unassign-missing.cpp';body=original_helper.read_text();assert body.count('UnassignString();')==1;path.write_text(body.replace('UnassignString();',';'));obj=compile(path,'name-unassign-missing');reject(None,[obj if p==helper else p for p in objects],'name-unassign-missing')
for name in (source.name,'CInitBaseClass_IsValid_0099a330.cpp','CTexture_Uninitialise_009f9f70.cpp'):
 obj=next(Path(p) for p,s in r['object_sources'].items() if Path(s).name==name);x=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(out/('missing-'+name+'.exe')),*map(str,[p for p in objects if p!=obj])],env=env,capture_output=True,text=True);assert x.returncode;records.append(dict(name='missing-'+name,link_rejected=True,stdout=x.stdout,stderr=x.stderr))
scalar=bytearray((base/'0xab9990.bin').read_bytes());assert scalar[12]==1;scalar[12]=0;path=out/'skip-object-free.bin';path.write_bytes(scalar);x=subprocess.run([str(base/'probe.exe'),str(base/'0xab7a70.bin'),str(path)],capture_output=True,text=True,timeout=30);assert x.returncode==80 and 'STATIC_FONT_LIFETIME_FAILURE' in x.stderr;records.append(dict(name='original-skip-object-free',code=x.returncode,stderr=x.stderr))

assert len(records)==13
for path,digest in pins.items():assert sha(Path(path))==digest,('input drift',path)
report=dict(status='PASS',build=str(b),cases=1080,paths=2160,run=record,proof=proof,repeat=True,standalone=dict(code=standalone.returncode,stdout=standalone.stdout),vtable_bytes_exact=24,final_pe_bytes_exact=197,controls=records,input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources=mapping,boundary='Actual static-font destructor/scalar roots, base/name/validity/texture helpers, guarded host allocation and synthetic COM Release. Five singleton/validity/display contexts,4 owned-glyph patterns,3 names/texture states and6 flags. Retained texture pointer is preserved when display is unavailable; fixture cleanup afterward is outside native observations. Concrete constructor, render/prepare services, real device/heap implementation and OOM/unwind remain boundaries.')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None and stage is None:(R/'rebuild/compile-gate/native-static-font-lifetime.json').write_text(json.dumps(report,indent=2))
print('NATIVE_STATIC_FONT_LIFETIME_GATE_PASS cases=1080 original=1080 paths=2160 controls=13 exact_bytes=173 vtable_bytes=24',b,flush=True)
