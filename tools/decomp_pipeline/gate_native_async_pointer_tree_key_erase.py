"""Fresh seven-source key erase acceptance and receipt validation."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=next(p for p in Path(__file__).resolve().parents if (p/'rebuild').is_dir())
KEY='?erase@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAEIABQAVCBankFileAsyncData@@@Z'
EQUAL='?equal_range@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@_STL@@U12@@2@ABQAVCBankFileAsyncData@@@Z'
def index_symbols(source,obj):
 """Verify indexer's own fresh object and admit only its recovered root."""
 import struct
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 from gate_game_component_abi import Coff
 from pe_oracle import EXE,read_virtual_data
 from near_match import compare
 role={'00/9d/STLAsyncPointerTree_KeyErase_009d7f40.cpp':(KEY,0x9d7f40),'00/9d/STLAsyncPointerTree_EqualRange_009d7c80.cpp':(EQUAL,0x9d7c80)}
 symbol,address=role[source];c=Coff(obj);section,offset=c.symbols[symbol];assert offset==0
 body,rels=c.sections[section];assert len(body)==83
 instructions=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(body,address));assert sum(i.size for i in instructions)==83
 assert instructions[-1].mnemonic=='ret' and instructions[-1].op_str==('4' if symbol==KEY else '8')
 retail=read_virtual_data(EXE.read_bytes(),address,83)
 if symbol==EQUAL:assert not rels and len(instructions)==38 and body==retail
 else:
  distance='??$__distance@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@_STL@@@_STL@@YAHABU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@0@0ABUinput_iterator_tag@0@@Z'
  range_erase='?erase@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAEXU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@2@0@Z'
  assert rels=={20:(EQUAL,20),40:(distance,20),69:(range_erase,20)}
  patched=bytearray(body)
  for site,target in ((20,0x9d7c80),(40,0x9d7550),(69,0x9d7760)):
   assert struct.unpack_from('<I',body,site)[0]==0
   struct.pack_into('<I',patched,site,(target-address-site-4)&0xffffffff)
  assert bytes(patched)!=retail
  score=compare(bytes(patched),retail);assert score.complete_decode and score.near_match and score.built_instructions==score.retail_instructions==33 and score.instruction_delta==0
 return {symbol}
def validate_report(path):
 report=json.loads(Path(path).read_text());assert report['status']=='NATIVE_ASYNC_POINTER_TREE_KEY_ERASE_PASS'
 assert report['key_erase_grade']=='NEAR_MATCH' and report['equal_range_grade']=='MATCH'
 providers=report['providers'];assert len(providers)==10 and len({r['symbol'] for r in providers})==10
 assert len({r['source'] for r in providers})==7
 key=next(r for r in providers if r['symbol']==KEY);equal=next(r for r in providers if r['symbol']==EQUAL)
 assert not key['exact'] and key['grade']=='NEAR_MATCH' and key['bytes']==83 and key['instructions']==33 and len(key['fixups'])==3
 assert key['similarity']['near_match'] and key['similarity']['complete_decode'] and key['similarity']['instruction_delta']==0
 assert equal['exact'] and equal['bytes']==83 and equal['instructions']==38 and not equal['fixups']
 assert all(r['exact'] and r['grade']=='MATCH' for r in providers if r['symbol']!=KEY)
 domain=report['domain'];assert (domain['cases_per_mode'],domain['snapshots_per_mode'],domain['matched'],domain['absent'])==(6000,15000,3000,3000)
 assert domain['mutation_partitions']=={'0':2000,'1':2000,'3':2000}
 assert domain['provider_hits']['original']==domain['provider_hits']['compiled']
 assert all(domain['provider_hits']['compiled'][address]==6000 for address in ('0x9d7f40','0x9d7c80','0x9d7550','0x9d7760'))
 assert all(n>0 for n in domain['provider_hits']['compiled'].values())
 aliases=report['equal_range_domain'];assert aliases['cases_per_mode']==aliases['full_arena_snapshots_per_mode']==1440
 assert aliases['alias_partitions']=={'separate':360,'key':360,'tree':360,'header':360}
 assert aliases['provider_hits']=={'original':{'0x9d7c80':1440},'compiled':{'0x9d7c80':1440}}
 omissions=report['source_omissions'];assert len(omissions)==7 and len({row['source'] for row in omissions})==7
 for row in omissions:assert row['returncode'] and any(row['symbol'] in line and ('LNK2001' in line or 'LNK2019' in line) for line in row['diagnostic'].splitlines())
 assert report['link_control_passed'] and not report['link_executable_executed']
 assert len(report['caller_abi'])==2 and report['caller_abi'][0]['callee_cleanup']==4 and report['caller_abi'][1]['callee_cleanup']==8
 assert report['input_sha256_before']==report['input_sha256_after']
 for input_path,digest in {**report['input_sha256_after'],**report['object_sha256']}.items():assert hashlib.sha256(Path(input_path).read_bytes()).hexdigest()==digest,input_path
 assert report['flags'][:4]==['/nologo','/c','/O2','/MT']
 return str(path)
def validate(test_arguments=()):
 canonical=R/'rebuild/tests/integration/NativeAsyncPointerTreeKeyErase_test.py'
 test=canonical if Path(__file__).resolve().parent==R/'tools/decomp_pipeline' else Path(__file__).with_name('NativeAsyncPointerTreeKeyErase_test.py')
 result=subprocess.run([sys.executable,str(test),*test_arguments],capture_output=True,text=True,timeout=180)
 assert result.returncode==0,result.stdout+result.stderr
 paths=[line[len('RECEIPT '):] for line in result.stdout.splitlines() if line.startswith('RECEIPT ')];assert len(paths)==1,result.stdout
 return validate_report(paths[0])
if __name__=='__main__':print('NATIVE_ASYNC_POINTER_TREE_KEY_ERASE_ACCEPTANCE_PASS',validate(sys.argv[1:]))
