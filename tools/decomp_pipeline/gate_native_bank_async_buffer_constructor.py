"""Caller-buffer constructor parity, typed omission and independent ABI/model gate."""
import hashlib,json,re,struct,subprocess,sys,tempfile
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
SOURCE=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_CBankFileAsyncData_009d5df0.cpp'
TEST=R/'rebuild/tests/integration/NativeBankAsyncBufferConstructor_test.py'
REQUIRED=R/'rebuild/tests/integration/NativeBankAsyncBufferConstructor_required.cpp'
ROOT='??0CBankFileAsyncData@@AAE@PAXKAAVCBankFileAsync@@@Z'
TREE='?insert_unique@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@_STL@@_N@2@ABQAVCBankFileAsyncData@@@Z'
BASE='??0CBaseClassNonCopyable@@QAE@XZ'
LOCK='??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z'
UNLOCK='??1CCriticalSectionLock@@QAE@XZ'
TABLES=[('??_7CMovableResource@@6B@',23,0x129b860,['??_ECMovableResource@@UAEPAXI@Z','__purecall','?CanRelocateData@CMovableResource@@UBE_NJ@Z','?Verify@CMovableResource@@UBEKXZ'],[0x9d5110,0xbfea08,0x9d50f0,0x9d5100]),('??_7CBankFileAsyncData@@6BCThreadedFileCallback@@@',44,0x129b8b4,['??_ECBankFileAsyncData@@UAEPAXI@Z','?OnFileReadBegin@CBankFileAsyncData@@UAE_NPAPAXW4EThreadedFilePriority@@@Z','?OnFileReadCompletion@CBankFileAsyncData@@UAEX_N@Z'],[0x9d8180,0x9d5440,0x9d54a0]),('??_7CBankFileAsyncData@@6BCMovableResource@@@',51,0x129b8a0,['??_ECBankFileAsyncData@@W3AEPAXI@Z','?RelocateData@CBankFileAsyncData@@UAEXJPAX0J@Z','?CanRelocateData@CBankFileAsyncData@@UBE_NJ@Z','?Verify@CMovableResource@@UBEKXZ'],[0x9d80a0,0x9d5670,0x9d55d0,0x9d5100])]
def main():
 inputs={SOURCE,TEST,REQUIRED,Path(__file__)}
 for name in ('check_one.py','gate_game_component_abi.py','pe_oracle.py','near_match.py','verify_and_land.py','variable_allocator_parity.py','crawl/rowtrim.py','crawl/purity.py'):inputs.add(R/'tools/decomp_pipeline'/name)
 inputs.add(R/'tools/artifact_layout.py')
 def closure(p):
  if p in inputs and p.suffix=='.h':return
  inputs.add(p)
  for name in re.findall(r'^\s*#include\s*"([^"]+)"',p.read_text(encoding='utf-8'),re.M):
   q=next((q for q in (p.parent/name,R/'rebuild/include'/name) if q.is_file()),None);assert q,name
   closure(q)
 closure(SOURCE);closure(REQUIRED)
 def hashes():return {str(p.relative_to(R)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sorted(inputs)}
 def toolchain_hashes():return {str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in [V.VC/'bin'/name for name in ('cl.exe','c1xx.dll','c2.dll','link.exe','mspdb71.dll')]}
 before=hashes();toolchain_before=toolchain_hashes(); env=V.env(); build=R/'work/native-recreation/loading/bank-buffer-construction/20261019'/__import__('datetime').datetime.now(__import__('datetime').timezone.utc).strftime('gate-%H%M%S');build.mkdir(parents=True)
 obj=build/'constructor.obj';ro=build/'required.obj'
 for source,target in ((SOURCE,obj),(REQUIRED,ro)):
  result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(target),str(source)],env,build)
  assert result.returncode==0,result.stdout+result.stderr
 c=Coff(obj);sec,off=c.symbols[ROOT];assert off==0
 built,rels=c.sections[sec];assert len(built)==179
 raw=EXE.read_bytes();original=read_virtual_data(raw,0x9d5d30,179)
 expected={8:(BASE,20,0x99a2f0),125:(LOCK,20,0xa62920),153:(TREE,20,0x9d7cf0),165:(UNLOCK,20,0xa629a0)}
 tables=[]
 for name,site,address,slots,values in TABLES:
  c.table(name,slots)
  actual=list(struct.unpack('<'+'I'*len(values),read_virtual_data(raw,address,4*len(values))));assert actual==values
  assert struct.unpack_from('<I',original,site)[0]==address
  expected[site]=(name,6,address)
  tables.append(dict(symbol=name,root_site=site,address=hex(address),slot_symbols=slots,retail_slot_values=[hex(v) for v in actual],boundary='Typed table identity only; virtual methods are not executed or recovered by this gate.'))
 assert rels=={site:(name,kind) for site,(name,kind,target) in expected.items()}
 patched=bytearray(built);audit=[]
 for site,(name,kind,target) in sorted(expected.items()):
  assert struct.unpack_from('<I',built,site)[0]==0
  if kind==20:assert (0x9d5d30+site+4+struct.unpack_from('<i',original,site)[0])&0xffffffff==target
  struct.pack_into('<I',patched,site,(target-(0x9d5d30+site+4) if kind==20 else target)&0xffffffff)
  audit.append(dict(site=site,symbol=name,kind=kind,addend=0,target=hex(target)))
 assert bytes(patched)==original
 md=Cs(CS_ARCH_X86,CS_MODE_32);ins=list(md.disasm(original,0x9d5d30));assert len(ins)==52 and sum(i.size for i in ins)==179
 # VC71 weak extern auxiliary proof; defined secondary E is not a weak alias.
 sp=struct.unpack_from('<I',c.data,8)[0];weak=[]
 for name in ('??_ECMovableResource@@UAEPAXI@Z','??_ECBankFileAsyncData@@UAEPAXI@Z'):
  index=next(i for i,n in c.indices.items() if n==name);at=sp+18*index
  assert c.data[at+16:at+18]==bytes([105,1])
  default,search=struct.unpack_from('<II',c.data,at+18);g=name.replace('??_E','??_G',1)
  assert search==2 and c.indices[default]==g and g in c.symbols
  weak.append(dict(symbol=name,default=g,search=search,aux_hex=c.data[at+18:at+36].hex()))
 thunk='??_ECBankFileAsyncData@@W3AEPAXI@Z';ts,to=c.symbols[thunk];tb,tr=c.sections[ts]
 assert to==0 and tb==bytes.fromhex('83e904e900000000') and tr=={4:('??_ECBankFileAsyncData@@UAEPAXI@Z',20)}
 originalthunk=read_virtual_data(raw,0x9d80a0,8);assert originalthunk[:4]==tb[:4] and (0x9d80a8+struct.unpack_from('<i',originalthunk,4)[0])&0xffffffff==0x9d8180
 # Unused Callback base table has no root/table/helper relocation reachability.
 aliases={x['symbol']:x['default'] for x in weak};visited=set()
 def visit(name):
  name=aliases.get(name,name)
  if name in visited:return
  visited.add(name)
  if name in c.symbols:
   s,o=c.symbols[name]
   for target,kind in c.sections[s][1].values():visit(target)
 visit(ROOT);assert '??_7CThreadedFileCallback@@6B@' not in visited
 caller=Coff(ro);assert any(n==ROOT and k==20 and struct.unpack_from('<I',body,site)[0]==0 for body,rs in caller.sections.values() for site,(n,k) in rs.items())
 command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(build/'required.exe'),'kernel32.lib',str(ro)]
 missing=subprocess.run(command,env=env,capture_output=True,text=True)
 assert missing.returncode and any(ROOT in line and ('LNK2001' in line or 'LNK2019' in line) for line in (missing.stdout+missing.stderr).splitlines())
 control=subprocess.run(command+[str(obj)],env=env,capture_output=True,text=True);assert control.returncode==0,control.stdout+control.stderr
 # Never execute linkage-only aliases. Independent model executes machine bytes.
 ns=dict(__file__=str(TEST),__name__='__gate_fixture__',COMPILED_ROOT=bytes(patched))
 exec(compile(TEST.read_text(encoding='utf-8'),str(TEST),'exec'),ns)
 controls=[]
 source_text=SOURCE.read_text()
 first,second=source_text.split('// Retail 009D5DF0:',1)
 for name,old,new in (
  ('null-buffer','Data(buffer), Buffer(buffer)','Data(0), Buffer(0)'),
  ('wrong-length','Length(length)','Length(length+1)'),
  ('nonzero-skip','SkipBytes(0)','SkipBytes(1)'),
  ('wrong-initial-uid','ThreadedFileUID(0)','ThreadedFileUID(1)'),
  ('omit-registration','BankFile.AsyncDataInstances.insert(this);',';'),
  ('early-size-store','    CCriticalSectionLock lock(','    InternalAllocatedBufferSize = 0;\n    CCriticalSectionLock lock('),
 ):
  assert first.count(old)==1
  mutant=build/(name+'.cpp');mutant.write_text(first.replace(old,new)+'// Retail 009D5DF0:'+second)
  mo=build/(name+'.obj')
  result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(mo),str(mutant)],env,build)
  assert result.returncode==0,result.stdout+result.stderr
  mc=Coff(mo);msec,moff=mc.symbols[ROOT];assert moff==0
  mb,mr=mc.sections[msec];patched_mutant=bytearray(mb)
  targets={name:target for name,kind,target in expected.values()}
  for site,(name_,kind) in mr.items():
   assert kind in (6,20) and struct.unpack_from('<I',mb,site)[0]==0
   struct.pack_into('<I',patched_mutant,site,(targets[name_]-(0x9d5d30+site+4 if kind==20 else 0))&0xffffffff)
  bad=dict(__file__=str(TEST),__name__='__negative_fixture__',COMPILED_ROOT=bytes(patched_mutant))
  try:exec(compile(TEST.read_text(),str(TEST),'exec'),bad)
  except AssertionError:controls.append(name)
  else:raise AssertionError('undetected '+name)
 assert ns['cases']==600 and ns['points']==3000 and before==hashes() and toolchain_before==toolchain_hashes()
 assert ns['report']['executable_cache_invalidated'] is True
 assert ns['execution_hits']=={mode:{'0x9d5d30':600,'0x99a2f0':600} for mode in ('original','compiled')}
 report=dict(status='PASS',grade='RELOCATION_MATCH',root_bytes=179,root_instructions=52,cases=600,snapshots=3000,original_cases=600,original_snapshots=3000,compiled_cases=600,compiled_snapshots=3000,relocations=audit,tables=tables,weak_aliases=weak,unused_callback_table_reachable=False,missing_provider=dict(symbol=ROOT,returncode=missing.returncode,stdout=missing.stdout,stderr=missing.stderr),provider_present_link=dict(returncode=control.returncode,stdout=control.stdout,stderr=control.stderr),link_executable_executed=False,source_sha256=before,input_sha256_after=hashes(),object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),original_exe_sha256=hashlib.sha256(raw).hexdigest(),boundary='Existing genuine private three-argument179B caller-buffer constructor; actual original9B base, explicit mutating lock/register interfaces. Typed tree address identity established by separate recovered generic pointer-tree original/compiled/model evidence, not tree execution here. Full32KiB state including padding; delayed size store/live bank/cached lock/RET12/callee saves/original-this return. No slot/inline/destructor recovery counts, physical registration, concurrency or valid lifetime claim.')
 report.update(runtime_controls=controls,build=str(build),toolchain_sha256_before=toolchain_before,toolchain_sha256_after=toolchain_hashes(),compiler_flags=['/nologo','/c','/O2','/MT'])
 report.update(execution_hits=ns['execution_hits'],executable_cache_invalidated=True)
 out=R/'rebuild/compile-gate/native-bank-async-buffer-constructor.json';out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(report,indent=2)+'\n')
 print('NATIVE_BANK_ASYNC_BUFFER_CONSTRUCTOR_PASS cases=600 snapshots=3000 controls=6')
if __name__=='__main__':main()
