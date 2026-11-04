"""Fresh UID iterator erase gate; no cached objects and no linked EXE execution."""
import hashlib,json,subprocess,sys
from pathlib import Path
R=next(p for p in Path(__file__).resolve().parents if (p/'rebuild').is_dir())
def validate_report(path):
 report=json.loads(Path(path).read_text());assert report['status']=='NATIVE_THREADED_UID_ITERATOR_ERASE_PASS'
 providers=report['providers'];assert len(providers)==6 and {int(p['address'],16):p['bytes'] for p in providers}=={0x990430:50,0x42a34e:572,0x41c7e0:26,0x41c800:26,0x42951b:64,0x42955b:64}
 assert all(p['grade']=='MATCH' and p['relocated_exact'] and p['compiled_hex']==p['original_hex'] for p in providers)
 root=next(p for p in providers if int(p['address'],16)==0x990430);assert root['retail_sha256']=='8d69723a76abb818dd2edc506c13672199979e656f7d1dcd9b615c4b79f7709c'
 assert len(root['relocations'])==2 and root['disassembly'][-1]['operands']=='4'
 assert {p['role'] for p in report['caller_abi']}=={'outlined_map','generic_call'}
 for p in report['caller_abi']:assert p['root_relocation']['symbol']==root['symbol'] and p['root_relocation']['kind']==20 and p['root_relocation']['addend']==0
 expected={'0x990430':1576,'0x42a34e':1576,'0x41c7e0':56,'0x41c800':56,'0x42951b':208,'0x42955b':416}
 assert report['domain']['original']==report['domain']['compiled']
 for mode in ('original','compiled'):
  domain=report['domain'][mode];assert domain['cases']==1576 and domain['snapshots']==4728 and domain['provider_hits']==expected
  assert domain['partitions']=={'none':1064,'count_add7':128,'count_zero':128,'count_max':128,'detached_links_poison':128}
 assert len(report['source_omissions'])==3 and len({p['omitted_object'] for p in report['source_omissions']})==3
 for p in report['source_omissions']:assert p['returncode'] and any(p['required_symbol'] in line and ('LNK2001' in line or 'LNK2019' in line) for line in (p['stdout']+p['stderr']).splitlines())
 assert report['link_control']['returncode']==0 and not report['link_executable_executed']
 assert report['input_sha256_before']==report['input_sha256_after'] and report['flags'][:5]==['/nologo','/c','/O2','/MT','/showIncludes']
 for p,h in {**report['input_sha256_after'],**report['object_sha256']}.items():assert hashlib.sha256(Path(p).read_bytes()).hexdigest()==h,p
 assert report['identity_qualification']
 return str(path)
def validate(arguments=()):
 test=R/'rebuild/tests/integration/NativeThreadedUIDIteratorErase_test.py' if Path(__file__).resolve().parent==R/'tools/decomp_pipeline' else Path(__file__).with_name('NativeThreadedUIDIteratorErase_test.py')
 result=subprocess.run([sys.executable,str(test),*arguments],capture_output=True,text=True,timeout=180);assert result.returncode==0,result.stdout+result.stderr
 receipts=[line[8:] for line in result.stdout.splitlines() if line.startswith('RECEIPT ')];assert len(receipts)==1,result.stdout
 return validate_report(receipts[0])

ROOT_SYMBOL = '?erase@?$_Rb_tree@KU?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@U?$_Select1st@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@_STL@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBKPAVCFileObject@CThreadedFileManager@@@_STL@@@2@@Z'

def index_symbols(source,obj):
 """Audit indexer's own fresh complete root; admit one qualified clone row."""
 import struct
 from capstone import Cs,CS_ARCH_X86,CS_MODE_32
 from gate_game_component_abi import Coff
 from pe_oracle import EXE,read_virtual_data
 assert source=='00/99/STLThreadedUIDTree_IteratorErase_00990430.cpp'
 obj=Path(obj);raw=obj.read_bytes();object_hash=hashlib.sha256(raw).hexdigest()
 machine,count,stamp,symptr,symcount,opt,flags=struct.unpack_from('<HHIIIHH',raw);assert machine==0x14c and opt==0
 stringptr=symptr+18*symcount;definitions=[];i=0
 while i<symcount:
  name,value,section,typ,storage,aux=struct.unpack_from('<8sIhHBB',raw,symptr+18*i)
  if name[:4]==bytes(4):
   at=stringptr+struct.unpack_from('<I',name,4)[0];name=raw[at:raw.index(0,at)].decode('ascii')
  else:name=name.rstrip(b'\0').decode('ascii')
  if name==ROOT_SYMBOL and section>0:definitions.append((value,section,storage))
  i+=1+aux
 assert len(definitions)==1 and definitions[0][0]==0 and definitions[0][2]==2
 c=Coff(obj);section,offset=c.symbols[ROOT_SYMBOL];assert section==definitions[0][1] and offset==0
 assert struct.unpack_from('<I',raw,20+(section-1)*40+36)[0]&0x1000
 body,fixups=c.sections[section];rebalance='?_Rebalance_for_erase@?$_Rb_global@_N@_STL@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z'
 assert len(body)==50 and fixups=={23:(rebalance,20),36:('_free',20)}
 patched=bytearray(body)
 for site,target in ((23,0x42a34e),(36,0xbfea14)):
  assert struct.unpack_from('<I',body,site)[0]==0;struct.pack_into('<I',patched,site,(target-0x990430-site-4)&0xffffffff)
 retail=read_virtual_data(EXE.read_bytes(),0x990430,50)
 assert hashlib.sha256(retail).hexdigest()=='8d69723a76abb818dd2edc506c13672199979e656f7d1dcd9b615c4b79f7709c' and bytes(patched)==retail
 ins=list(Cs(CS_ARCH_X86,CS_MODE_32).disasm(bytes(patched),0x990430));assert len(ins)==21 and sum(i.size for i in ins)==50 and ins[-1].mnemonic=='ret' and ins[-1].op_str=='4'
 assert [(i.address,i.op_str) for i in ins if i.mnemonic=='call']==[(0x990446,'0x42a34e'),(0x990453,'0xbfea14')]
 assert hashlib.sha256(obj.read_bytes()).hexdigest()==object_hash
 return {ROOT_SYMBOL}

if __name__=='__main__':print('NATIVE_THREADED_UID_ITERATOR_ERASE_ACCEPTANCE_PASS',validate(sys.argv[1:]))
