"""Memory compressed reader: full-state native comparison at explicit LZO boundaries."""
import hashlib,itertools,json,struct,subprocess,tempfile
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
ROOT=Path(__file__).resolve().parents[2];w=Path(tempfile.mkdtemp(prefix='native-memory-read-compressed-'));env=V.env()
source=ROOT/'rebuild/src/compiled/00/99/CMemoryDataInputStream_ReadCompressed_00995b10.cpp';fixture=ROOT/'rebuild/tests/integration/NativeMemoryReadCompressed_test.cpp'
symbol='?ReadCompressed@CMemoryDataInputStream@@UAEXPAXJ@Z'
targets={'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,'_lzo1x_decompress_asm_fast':0xc069d0,'?m_errorCallback@CAFile@@2P6IXXZA':0x13bc9fc}
aliases={'@ReadCompressed@16':symbol,'?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':'@Overflow@16','_lzo1x_decompress_asm_fast':'_Decompress','?m_errorCallback@CAFile@@2P6IXXZA':'_ErrorCallback'}
(w/'memory_compressed_aliases.h').write_text('\n'.join('#pragma comment(linker,"/alternatename:'+a+'='+b+'")' for a,b in aliases.items())+'\n')
body=read_virtual_data(EXE.read_bytes(),0x995b10,540);md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;fixups=[]
for i in md.disasm(body,0x995b10):
 if i.mnemonic=='call' and i.operands[0].type==2:
  assert i.operands[0].imm in targets.values();fixups.append((i.address-0x995b10+1,i.operands[0].imm))
 elif any(o.type==3 and o.mem.disp==0x13bc9fc for o in i.operands):
  assert i.bytes==b'\xa1\xfc\xc9\x3b\x01';fixups.append((i.address-0x995b10+1,0x13bc9fc))
assert len(fixups)==6
(w/'memory_compressed_retail.h').write_text('static const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const unsigned retailFixups[][2]={'+','.join('{%d,%d}'%f for f in fixups)+'};\n')
objects=[]
for n,p in enumerate((source,fixture)):
 o=w/(str(n)+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/Fo'+str(o),str(p)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(o)
proof=check(objects[0],symbol,0x995b10,540,6,targets)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
r=subprocess.run(cmd+[str(objects[1])],env=env,capture_output=True,text=True);assert r.returncode
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
cases=0;base=0x24000000
with (w/'input.bin').open('wb') as f:
 for fill,mode,size,prefix,remaining,position in itertools.product((0,0x95),range(8),(-1,0,1,3,16),((0,0),(-2,0),(4,0),(-1,0),(-1,-2),(-1,4)),(0,1,2,6,64),(0,4,0xfffffffe)):
  a=bytearray([fill])*2048;struct.pack_into('<6I',a,4,position,64,base+0x400,0,remaining,base+0x400)
  struct.pack_into('<hi',a,0x400,*prefix);a[0x700:0x710]=bytes(range(16))
  f.write(struct.pack('<2I',mode,size&0xffffffff));f.write(a);cases+=1
for kind in ('retail','compiled'):
 r=subprocess.run([str(w/'fixture.exe'),str(w/'input.bin'),str(w/(kind+'.bin')),kind],env=env,capture_output=True,text=True,timeout=60);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);print(r.stdout.strip(),kind,flush=True)
with (w/'retail.bin').open('rb') as e,(w/'compiled.bin').open('rb') as f:
 while True:
  x=e.read(1<<20);y=f.read(1<<20);assert x==y,(w,'native mismatch')
  if not x:break
size=(w/'compiled.bin').stat().st_size;assert size%2060==0;snapshots=size//2060
report=dict(cases=cases,snapshots=snapshots,omission_checks=1,provider=proof,source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual540byte reader/six audited fixups, full2KiB snapshots. Short/extended sizes, raw/compressed/nonpositive paths, chunk overflow, wrapped and rejected const-data positioning, tail copying and error callback/null callback. LZO, virtual position and chunk overflow are explicit callback boundaries; LZO callback fills bounded output and resets tail window after synthetic positions. Initial prefix guards always permit a read, excluding original uninitialized-local behavior. No real decompression claim.')
(ROOT/'rebuild/compile-gate/native-memory-read-compressed.json').write_text(json.dumps(report,indent=2)+'\n');print('NATIVE_MEMORY_READ_COMPRESSED_GATE_PASS',snapshots,'snapshots')
