from pathlib import Path
from datetime import datetime,timezone
import sys,json,re,struct,hashlib,subprocess
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'));import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from native_retail_readonly_verify import verify
from bank_begin_worker_link_oracle import aliases
import argparse
parser=argparse.ArgumentParser();parser.add_argument('--source',type=Path,default=R/'rebuild/src/compiled/00/9a/CFontBank_Lifetime_009a0b30.cpp');parser.add_argument('--fixture',type=Path,default=R/'rebuild/tests/integration/NativeFontBaseLifetime_test.cpp');parser.add_argument('--header',type=Path,default=R/'rebuild/include/fable_font_bank.h');parser.add_argument('--output',type=Path);args=parser.parse_args();args.source=args.source.resolve();args.fixture=args.fixture.resolve();args.header=args.header.resolve();args.output=args.output.resolve() if args.output else None
b=args.output or R/'rebuild/build/native-font-base-lifetime/00/9a/009a0b30'/datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');b.mkdir(parents=True);inc=b/'include';inc.mkdir();(inc/'fable_font_bank.h').write_bytes(args.header.read_bytes());env=V.env();env['INCLUDE']=str(V.VC/'include')+';'+str(inc)+';'+str(R/'rebuild/include')
sources=[args.source]+[R/'rebuild/src/compiled'/p for p in [
'00/99/CBaseClass_CBaseClass_0099a2d0.cpp','00/99/CCharString_DefaultConstructor_0099e4b0.cpp','00/99/CCharString_Destructor_0099eae0.cpp','00/99/CCharString_UnassignString_0099e9b0.cpp','00/99/CCharString_Constructor_0099ebf0.cpp','00/99/CCharString_AllocStringData_0099ea60.cpp','00/99/CCharString_CopyAssignment_0099efb0.cpp','00/99/CCharString_ConversionOperator_0099e4c0.cpp','00/9a/CCharString_InitFromCStr_009a0590.cpp','00/9a/global_CBasicStringCharAssignFromData_009a0300.cpp','00/9a/CFontBank_Preload_009a0b20.cpp']]
fixture=args.fixture;inputs={Path(__file__),EXE,args.header};seen=set()
def closure(p):
 p=p.resolve()
 if p in seen:return
 seen.add(p);inputs.add(p)
 for name in re.findall(r'^\s*#include\s*"([^"\n]+)"',p.read_text(),re.M):closure(next(q for q in (p.parent/name,inc/name,R/'rebuild/include'/name) if q.is_file()))
for p in sources+[fixture]:closure(p)
inputs.update(p for p in (V.VC/'include').rglob('*') if p.is_file());inputs.update(p for d in (V.VC/'bin',V.VC/'lib') for p in d.iterdir() if p.is_file() and p.suffix.lower() in ('.exe','.dll','.lib'))
for module in tuple(sys.modules.values()):
 name=getattr(module,'__file__',None)
 if name:
  p=Path(name).resolve()
  if p.suffix=='.py' and p.is_relative_to(R/'tools/decomp_pipeline'):inputs.add(p)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();pins={str(p):sha(p) for p in inputs};(b/'inputs.json').write_text(json.dumps(pins,indent=2));objects=[];mapping={}
for j,source in enumerate(sources+[fixture]):
 label='Fixture' if source==fixture else 'root%d'%j;snapshot=b/(label+'.cpp');snapshot.write_bytes(source.read_bytes());obj=snapshot.with_suffix('.obj');result=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(snapshot)],env,b);(b/(label+'.compile.json')).write_text(json.dumps(dict(source=str(source),code=result.returncode,stdout=result.stdout,stderr=result.stderr)));assert not result.returncode,result.stdout+result.stderr;objects.append(obj);mapping[str(obj)]=str(source)
root=Coff(objects[0]);symbol='??_GCFontBank@@UAEPAXI@Z';sec,off=root.symbols[symbol];body,relocations=root.sections[sec];assert not off and len(body)==46 and {site:kind for site,(_,kind) in relocations.items()}=={8:6,13:20,20:20,33:20};targets={'??_7CFontBank@@6B@':0x129ab38,'??1CCharString@@QAE@XZ':0x99eae0,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??3@YAXPAX@Z':0xbfe9bc};fixed=bytearray(body)
for site,(name,kind) in relocations.items():struct.pack_into('<I',fixed,site,(targets[name]+struct.unpack_from('<i',body,site)[0]-(0x9a0b30+site+4 if kind==20 else 0))&0xffffffff)
original=read_virtual_data(EXE.read_bytes(),0x9a0b30,46);assert bytes(fixed)==original;(b/'original.bin').write_bytes(original)
command=[str(V.VC/'bin/link.exe'),'/nologo','/OPT:REF','/out:'+str(b/'probe.exe'),'/map:'+str(b/'probe.map'),*map(str,objects)];result=subprocess.run(command,cwd=b,env=env,capture_output=True,text=True);(b/'link.json').write_text(json.dumps(dict(command=command,code=result.returncode,stdout=result.stdout,stderr=result.stderr)));assert not result.returncode,result.stdout+result.stderr
result=subprocess.run([str(b/'probe.exe'),str(b/'original.bin')],cwd=b,capture_output=True,text=True,timeout=20);record=dict(code=result.returncode,stdout=result.stdout,stderr=result.stderr);(b/'run.json').write_text(json.dumps(record,indent=2));print(record,flush=True);assert not result.returncode and 'NATIVE_FONT_BASE_LIFETIME_PASS cases=384 original=384 paths=768' in result.stdout,record
linked=verify(b,objects,{p.name for p in objects[:-1]},b/'Fixture.cpp');again=subprocess.run([str(b/'probe.exe'),str(b/'original.bin')],cwd=b,capture_output=True,text=True,timeout=20);assert not again.returncode and again.stdout==result.stdout
for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
report=dict(status='PASS',build=str(b),cases=384,paths=768,repeat=True,original_bytes=46,original_relocations={hex(k):v for k,v in relocations.items()},linked=linked,input_sha256=pins,objects={str(p):sha(p) for p in objects},object_sources=mapping,run=record,boundary='Typed CFontBank base hierarchy and owned Name, with native base/string helpers. Empty/exclusive/shared names, 32 height patterns and four deleting flags compare against original instructions with guarded host allocation. Checks cleanup, counters, references, return pointer, final base vptr and balanced allocation. No renderer/glyph readiness, OOM or exception-unwind certification.')


r=report;out=b/'controls';out.mkdir();records=[]
original=read_virtual_data(EXE.read_bytes(),0x9a0b30,46)
def audit(obj):
 c=Coff(obj);sec,off=c.symbols[symbol];body,rels=c.sections[sec];assert not off and len(body)==46 and len(rels)==4;targets={'??_7CFontBank@@6B@':0x129ab38,'??1CCharString@@QAE@XZ':0x99eae0,'??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,'??3@YAXPAX@Z':0xbfe9bc};fixed=bytearray(body)
 for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(targets[name]+struct.unpack_from('<i',body,site)[0]-(0x9a0b30+site+4 if kind==20 else 0))&0xffffffff)
 assert bytes(fixed)==original
for name,old,new in [('raw-name','CCharString Name;','CCharStringData* Name;'),('wrong-base','public CBaseClassNonCopyable','public CBaseClass')]:
 inc=out/name;inc.mkdir();text=(args.header).read_text();assert text.count(old)==1;(inc/'fable_font_bank.h').write_text(text.replace(old,new));obj=out/(name+'.obj');result=V.cl(['/nologo','/c','/O2','/MT','/Gy','/I'+str(inc),'/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(args.source)],env,out);assert not result.returncode,result.stdout+result.stderr;rejected=False
 try:audit(obj)
 except (AssertionError,KeyError):rejected=True
 assert rejected;records.append(dict(name=name,original_byte_proof_rejected=True))
mutant=bytearray(original);assert mutant[0x1c]==1 and mutant[0x18:0x1c]==bytes.fromhex('f6442408');mutant[0x1c]=0;path=out/'skip-delete-flag.bin';path.write_bytes(mutant);result=subprocess.run([str(b/'probe.exe'),str(path)],cwd=out,capture_output=True,text=True,timeout=20);line=next(i for i,text in enumerate((args.fixture).read_text().splitlines(),1) if 'CHECK(fontDeletes==1 && deleteChars==1)' in text);assert result.returncode==80 and ('FONT_LIFETIME_FAILURE line=%d'%line) in result.stderr,(result.returncode,result.stderr);records.append(dict(name='skip-delete-flag',code=result.returncode,stderr=result.stderr,expected_line=line))
command=json.loads((b/'link.json').read_text())['command']
for filename in ('CBaseClass_CBaseClass_0099a2d0.cpp','CCharString_Destructor_0099eae0.cpp'):
 obj=next(obj for obj,source in r['object_sources'].items() if Path(source).name==filename);linkargs=[arg for arg in command if arg!=obj and not arg.startswith('/map:')];linkargs=['/out:'+str(out/(filename+'.exe')) if arg.startswith('/out:') else arg for arg in linkargs];result=subprocess.run(linkargs,cwd=out,env=env,capture_output=True,text=True);assert result.returncode and ('CCharString' if 'CCharString' in filename else 'CBaseClassNonCopyable') in result.stdout+result.stderr;records.append(dict(name='omit-'+filename,link_rejected=True,stdout=result.stdout,stderr=result.stderr))
(out/'progress.json').write_text(json.dumps(records,indent=2))
# Independently resolve every font vtable function pointer in the compiled data section.
c=Coff(next(Path(obj) for obj,source in r['object_sources'].items() if Path(source)==args.source));sec,off=c.symbols['??_7CFontBank@@6B@'];data,rels=c.sections[sec];assert off==0 and len(data)==24 and len(rels)==6;fixed=bytearray(data);targets={'??_GCFontBank@@UAEPAXI@Z':0x9a0b30,'__purecall':0xbfea08,'?Preload@CFontBank@@UBE_NABVCWideString@@_N@Z':0x9a0b20}
alias=aliases([next(Path(obj) for obj,source in r['object_sources'].items() if Path(source)==args.source)],args.fixture);assert alias['??_ECFontBank@@UAEPAXI@Z']=='??_GCFontBank@@UAEPAXI@Z'
for site,(name,kind) in rels.items():
 if off<=site<off+24:assert kind==6;name=alias.get(name,name);struct.pack_into('<I',fixed,site-off,targets[name]+struct.unpack_from('<i',data,site)[0])
assert bytes(fixed)==read_virtual_data(EXE.read_bytes(),0x129ab38,24)

# Verify both relocated contributions in the final linked PE, including linker weak aliases.
import pefile
mapped={}
for line in (b/'probe.map').read_text().splitlines():
 match=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if match:mapped[match[1]]=int(match[2],16)
pe=pefile.PE(str(b/'probe.exe'))
for name,size in [(symbol,46),('??_7CFontBank@@6B@',24)]:
 section,offset=root.symbols[name];payload,relocs=root.sections[section];expected=bytearray(payload)
 for site,(target,kind) in relocs.items():
  target=alias.get(target,target);addend=struct.unpack_from('<i',payload,site)[0]
  struct.pack_into('<I',expected,site,(mapped[target]+addend-(mapped[name]+site+4 if kind==20 else 0))&0xffffffff)
 assert pe.get_data(mapped[name]-pe.OPTIONAL_HEADER.ImageBase,size)==bytes(expected)

# Independent retail RTTI root/hierarchy check; compiler-generated RTTI is not used as evidence.
retail=EXE.read_bytes()
def u32(address):return struct.unpack('<I',read_virtual_data(retail,address,4))[0]
def type_name(address):return read_virtual_data(retail,address+8,80).split(b'\0',1)[0].decode('ascii')
locator=u32(0x129ab38-4);assert u32(locator+4)==u32(locator+8)==0
hierarchy=u32(locator+16);count=u32(hierarchy+8);array=u32(hierarchy+12);assert count==3
names=[]
for i in range(count):
 descriptor=u32(array+4*i);names.append(type_name(u32(descriptor)))
assert names==['.?AVCFontBank@@','.?AVCBaseClassNonCopyable@@','.?AVCBaseClass@@']
assert type_name(u32(locator+12))==names[0]
standalone=subprocess.run([str(b/'probe.exe')],cwd=b,capture_output=True,text=True,timeout=20)
assert not standalone.returncode and 'cases=384 original=0 paths=384' in standalone.stdout
for p,h in pins.items():assert sha(Path(p))==h,('input drift',p)
report.update(controls=records,vtable_bytes_exact=24,final_pe_bytes_exact=70,rtti_hierarchy=names,standalone=dict(code=standalone.returncode,stdout=standalone.stdout),status='PASS')
(b/'receipt.json').write_text(json.dumps(report,indent=2))
if args.output is None:(R/'rebuild/compile-gate/native-font-base-lifetime.json').write_text(json.dumps(report,indent=2))
print('NATIVE_FONT_BASE_LIFETIME_GATE_PASS cases=384 original=384 paths=768 controls=5 vtable_bytes=24 exact_bytes=46',b,flush=True)
