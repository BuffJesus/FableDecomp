"""Fresh canonical-source root/actual-child/body-model gate; no scratch proof seeds."""
import hashlib,json,struct,sys,re,subprocess,tempfile
from pathlib import Path
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'tools/decomp_pipeline'))
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from near_match import compare,retail_relocations
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
w=R/'rebuild/tests/integration'
parent=w/'NativeBankAsyncAutoAlloc_model.py'
required=w/'NativeBankAsyncAutoAlloc_required.cpp'
source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncEntry_BeginReadDataAutoAlloc_009d6100.cpp'
alignment_source=R/'rebuild/src/compiled/00/9c/CBankFile_GetAlignment_009cbb80.cpp'
symbol='?BeginReadDataAutoAlloc@CBankFileAsyncEntry@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@KKW4EThreadedFilePriority@@@Z'
alignment_symbol='?GetAlignment@CBankFile@@QBEKXZ'
flags=['/nologo','/c','/O2','/MT']
inputs={source,alignment_source,parent,required,Path(__file__).resolve(),R/'tools/decomp_pipeline/gate_native_bank_async_autoalloc.py'}
helpers=['check_one.py','gate_game_component_abi.py','pe_oracle.py','variable_allocator_parity.py','near_match.py','verify_and_land.py','crawl/rowtrim.py','crawl/purity.py']
inputs.update(R/'tools/decomp_pipeline'/n for n in helpers);inputs.add(R/'tools/artifact_layout.py')
visited=set()
def headers(path):
 if path in visited:return
 visited.add(path);inputs.add(path)
 for name in re.findall(r'^\s*#\s*include\s*"([^"]+)"',path.read_text(errors='replace'),re.M):
  choices=(path.parent/name,R/'rebuild/include'/name)
  found=next((q.resolve() for q in choices if q.is_file()),None)
  assert found is not None,(path,name)
  headers(found)
for path in (source,alignment_source,required):headers(path)
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def current_inputs():return {str(q.relative_to(R)):sha(q) for q in sorted(inputs)}
toolchain_paths=[V.VC/'bin'/n for n in ('cl.exe','link.exe','c1.dll','c1xx.dll','c2.dll')]
def current_toolchain():return {str(q):sha(q) for q in toolchain_paths}
BEFORE=current_inputs();TOOLCHAIN=current_toolchain()
build=Path(tempfile.mkdtemp(prefix='native-bank-autoalloc-'));obj=build/'root.obj';alignment_obj=build/'alignment.obj';ro=build/'required.obj'
for src,dst in ((source,obj),(alignment_source,alignment_obj),(required,ro)):
 result=V.cl(flags+['/I'+str(R/'rebuild/include'),'/Fo'+str(dst),str(src)],V.env(),build)
 assert result.returncode==0,result.stdout+result.stderr
c=Coff(obj);s,o=c.symbols[symbol];built,relocs=c.sections[s]
assert o==0 and len(built)==235 and len(relocs)==8
md=Cs(CS_ARCH_X86,CS_MODE_32);assert len(list(md.disasm(built,0)))==93
original=read_virtual_data(EXE.read_bytes(),0x9d6100,235)
grade=compare(built,original,tuple(relocs),retail_relocations('009d6100',235)).to_dict()
assert grade['near_match'] and grade['retail_instructions']==grade['built_instructions']==93
assert grade['opcode_similarity']==0.967741935483871 and grade['instruction_delta']==0
targets={'?GetAlignment@CBankFile@@QBEKXZ':0x9cbb80,'?Align@CBankFile@@QBEKK@Z':0x9cbb90,'??0CBankFileAsyncData@@AAE@KKPAVCMovableResourceMemoryPool@@AAVCBankFileAsync@@@Z':0x9d5df0,'?SetupRead@CBankFileAsyncEntry@@AAEXKAAVCBankFileAsyncData@@KKKW4EThreadedFilePriority@@@Z':0x9d5970,'??2@YAPAXI@Z':0xbfea1a,'??3@YAXPAX@Z':0xbfe9bc,'?DeleteData@?$CCountedPointer@VCBankFileAsyncData@@@@CIXPAX@Z':0x9a42b0}
assert set(relocs)=={25,72,83,110,123,143,181,218}
patched=bytearray(built);audit=[]
for site,(name,kind) in relocs.items():
 assert name in targets and kind==(6 if site==143 else 20)
 assert struct.unpack_from('<I',built,site)[0]==0
 target=targets[name];struct.pack_into('<I',patched,site,(target-(0x9d6100+site+4) if kind==20 else target)&0xffffffff)
 audit.append(dict(offset=site,symbol=name,type=kind,addend=0,target=hex(target)))
ds,do=c.symbols['?DeleteData@?$CCountedPointer@VCBankFileAsyncData@@@@CIXPAX@Z'];deleter,dr=c.sections[ds]
assert do==0 and len(deleter)==11 and not dr and deleter==read_virtual_data(EXE.read_bytes(),0x9a42b0,11)
ac=Coff(alignment_obj);asec,aoff=ac.symbols[alignment_symbol];ab,ar=ac.sections[asec]
assert aoff==0 and len(ab)==7 and not ar and ab==read_virtual_data(EXE.read_bytes(),0x9cbb80,7)
rc=Coff(ro);assert symbol not in rc.symbols and alignment_symbol not in rc.symbols
command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/'required.exe'),'kernel32.lib',str(ro)]
def link(objects):return subprocess.run(command+[str(q) for q in objects],env=V.env(),capture_output=True,text=True)
missing=link([alignment_obj]);diagnostic=missing.stdout+missing.stderr
assert missing.returncode and any(symbol in line and ('LNK2001' in line or 'LNK2019' in line) for line in diagnostic.splitlines()),diagnostic
missing_alignment=link([obj]);diagnostic=missing_alignment.stdout+missing_alignment.stderr
assert missing_alignment.returncode and any(alignment_symbol in line and ('LNK2001' in line or 'LNK2019' in line) for line in diagnostic.splitlines()),diagnostic
control=link([obj,alignment_obj]);assert control.returncode==0,control.stdout+control.stderr
CLOSURE=current_inputs();assert CLOSURE==BEFORE and current_toolchain()==TOOLCHAIN
text=parent.read_text()
def replace(old,new):
 global text
 assert text.count(old)==1,(old,text.count(old));text=text.replace(old,new)
text=text.replace('GETDATA','SETUPBUFFER').replace('GetData9D52D0','SetupBuffer9D52D0')
replace('ALTDELETER=0x5000100','ALTDELETER=0x5000100;VIRTUALDELETE=0x5000200;VT=0x5000800\nroot=bytes(COMPILED_ROOT)')
replace("returned=ctx.get('ctor_return',selfp);m.put(returned+12", "returned=ctx.get('ctor_return',selfp);m.put(returned,VT);m.put(returned+12")
replace("if not count:call('counted_delete',m.w(control+8));call('delete',0,(control,))", "if not count:\n   receiver=m.w(control+8)\n   if m.w(control+4)==ALTDELETER:call('counted_delete',receiver)\n   elif receiver:call('counted_delete',receiver,(1,))\n   call('delete',0,(control,))")
replace('u.mem_map(A,SIZE);','u.mem_write(DELETER,ACTUAL_DELETER);u.mem_write(VT,word(VIRTUALDELETE))\nu.mem_map(A,SIZE);')
replace(",DELETER:('counted_delete',0,0)",",VIRTUALDELETE:('counted_delete',1,4)")
replace("out=Path(__file__).with_name('NativeBankAsyncAutoAlloc_model_result.json')", "report.update(input_sha256=CLOSURE,toolchain_sha256=TOOLCHAIN,compiler_flags=FLAGS,compiled_object_sha256=hashlib.sha256(OBJECT.read_bytes()).hexdigest(),relocation_audit=AUDIT,actual_deleter_sha256=hashlib.sha256(ACTUAL_DELETER).hexdigest(),source_omission={'symbol':ROOT_SYMBOL,'missing_failed':True,'provider_link_passed':True},prerequisite_omission={'symbol':ALIGNMENT_SYMBOL,'missing_failed':True,'provider_link_passed':True,'grade':'MATCH','bytes':7,'extra_recovery_credit':False},original_root_sha256=hashlib.sha256(ORIGINAL).hexdigest(),executed_compiled_root_sha256=hashlib.sha256(root).hexdigest(),compiled_grade=GRADE,status='COMPILED_ROOT_ACTUAL_SETUP_DELETER_MODEL_PASS')\nreport['original_child_sha256']={k:v for k,v in report.pop('body_sha256').items() if k!=hex(ENTRY)}\nreport['boundary']='Fresh canonical235B/93instruction near root; actual retail191B SetupRead and19B SetupBuffer null-fault prefix; exact emitted11B DeleteData; virtual deleting destructor/Alignment/Align/heap/constructor/entryoffset/queue/alternate deleter remain disclosed boundaries. Full32KiB snapshots; stack and syncnonnull excluded. Genuine7B GetAlignment prerequisite exact/strictomission, not executed as callback in root model and no extra recovery credit. No pool/constructor/queue/LZO implementation claim.'\nout=R/'rebuild/compile-gate/native-bank-async-autoalloc.json'")
replace("'ASYNC_AUTOALLOC_ORIGINAL_MODEL_PASS'", "'NATIVE_BANK_ASYNC_AUTOALLOC_PASS'")
scope=dict(__file__=str(parent),__name__='__main__',COMPILED_ROOT=patched,ACTUAL_DELETER=deleter,OBJECT=obj,AUDIT=audit,CLOSURE=CLOSURE,TOOLCHAIN=TOOLCHAIN,FLAGS=flags,ROOT_SYMBOL=symbol,ALIGNMENT_SYMBOL=alignment_symbol,ORIGINAL=original,GRADE=grade)
exec(compile(text,str(parent),'exec'),scope)
assert current_inputs()==BEFORE and current_toolchain()==TOOLCHAIN,'Inputs/toolchain changed during gate'
report_path=R/'rebuild/compile-gate/native-bank-async-autoalloc.json'
final_report=json.loads(report_path.read_text())
assert set(final_report['original_child_sha256'])=={'0x9d5970','0x9d52d0'}
report_path.write_text(json.dumps(final_report,indent=2)+'\n')
