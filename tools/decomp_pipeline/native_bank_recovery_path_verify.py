from pathlib import Path
import sys,json,struct,re,hashlib
import pefile
sys.path.insert(0,'tools/decomp_pipeline')
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
b=Path(sys.argv[1]).resolve();symbol='?GetRecoveryFilePath@CBankFile@@AAE?AVCWideString@@XZ';c=Coff(b/'new0.obj');s,o=c.symbols[symbol];body,rels=c.sections[s];assert not o and len(body)==207 and len(rels)==16
targets={'??0CWideString@@QAE@XZ':0x99aed0,'??1CWideString@@QAE@XZ':0x99b510,'??0CWideString@@QAE@ABV0@@Z':0x99b720,'?SplitPath@CAFile@@SIXABVCWideString@@PAV2@111@Z':0x997620,'??H@YI?AVCWideString@@ABV0@0@Z':0x99be70,'??H@YI?AVCWideString@@ABV0@PBG@Z':0x99bf30,'??_C@_19NCDOPEAD@?$AA?4?$AAr?$AAe?$AAc?$AA?$AA@':0x129b52c}
fixed=bytearray(body)
for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(targets[name]+struct.unpack_from('<i',body,site)[0]-(0x9cbfe0+site+4 if kind==20 else 0))&0xffffffff)
assert bytes(fixed)==read_virtual_data(EXE.read_bytes(),0x9cbfe0,207)
mapping={}
for line in (b/'probe.map').read_text().splitlines():
 m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
 if m:mapping[m[1]]=int(m[2],16)
assert '?DeleteRecoveryFile@CBankFile@@AAEXXZ' not in mapping
entry=mapping[symbol];fixed=bytearray(body)
for site,(name,kind) in rels.items():struct.pack_into('<I',fixed,site,(mapping[name]+struct.unpack_from('<i',body,site)[0]-(entry+site+4 if kind==20 else 0))&0xffffffff)
with pefile.PE(str(b/'probe.exe')) as pe:assert bytes(fixed)==pe.get_data(entry-pe.OPTIONAL_HEADER.ImageBase,len(body))
p=b/'recovery-path-verification.json';assert not p.exists();p.write_text(json.dumps(dict(status='PASS',grade='MATCH',bytes=207,fixups=16,executable_sha256=hashlib.sha256((b/'probe.exe').read_bytes()).hexdigest(),boundary='Recovery path construction only; DeleteRecoveryFile discarded and not executed. Parent169 bodies retain their original classifications.'),indent=2)+'\n');print('NATIVE_RECOVERY_PATH_VERIFY_PASS bytes=207 fixups=16')
