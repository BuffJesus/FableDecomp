import hashlib,json,subprocess,sys,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one as V
from variable_allocator_parity import check
from pe_oracle import EXE,read_virtual_data
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
w=Path(tempfile.mkdtemp(prefix='native-font-vstring-width-'));env=V.env()
specs=[('00/43/CFontBank_GetVStringWidth_0043e4d0.cpp','font_vstring_width_candidate_20261009.cpp','?GetVStringWidth@CFontBank@@QBEMABVCWideString@@@Z',0x43e4d0,69,3),('00/99/CCharString_operatorcharconstptr2_0099af10.cpp',None,'?GetAsCharArray@CWideString@@QBEPBGXZ',0x99af10,15,1),('00/9e/Global_VirtualCoordinateHelpers_009e1d00.cpp',None,'?GFAXToVX@@YIMM@Z',0x9e1da0,40,3),('00/9e/Global_GFAYToVY_009e1dd0.cpp','gfay_to_vy_candidate_20261009.cpp','?GFAYToVY@@YIMM@Z',0x9e1dd0,49,3),('00/9b/CDisplayManager_GetRenderTargetDimensions_009bedc0.cpp',None,'?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ',0x9bedc0,24,0)]
targets={s[2]:s[3] for s in specs};targets.update({'?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,'?g_FableVirtualWidth_013961E8@@3NA':0x13961e8,'?g_FableVirtualHeight_013961F0@@3NA':0x13961f0,'?g_FableEmptyWideString_0129A8E0@@3PAGA':0x129a8e0})
offsets={s[3]:i*256 for i,s in enumerate(specs)};offsets.update({0x9a4ec0:0xffffffff,0x13961e8:0xfffffffe,0x13961f0:0xfffffffd,0x129a8e0:0xfffffffc});body=bytearray(5*256);raw=EXE.read_bytes();fixups=[];objects=[];sources=[];proofs=[];cs=Cs(CS_ARCH_X86,CS_MODE_32);cs.detail=True
for i,(rel,scratch,symbol,address,length,relcount) in enumerate(specs):
 source=ROOT/'rebuild/src/compiled'/rel
 if scratch and ('--stage' in sys.argv or not source.exists()):source=ROOT/'work/parity_addtext'/scratch
 sources.append(source);obj=w/f'{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(source)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);proofs.append(check(obj,symbol,address,length,relcount,targets));b=read_virtual_data(raw,address,length);body[i*256:i*256+length]=b
 for ins in cs.disasm(b,address):
  if ins.mnemonic in ('call','jmp') and ins.bytes[0] in (0xe8,0xe9):
   target=int(ins.op_str,16);assert target in offsets;fixups.append((i*256+ins.address-address+ins.size-4,offsets[target],20))
  if ins.disp_size==4 and ins.disp in offsets:fixups.append((i*256+ins.address-address+ins.disp_offset,offsets[ins.disp],6))
  if ins.mnemonic=='mov' and ins.bytes[0]==0xb8 and int(ins.op_str.split(',')[-1],16)==0x129a8e0:fixups.append((i*256+ins.address-address+1,offsets[0x129a8e0],6))
(w/'font_vstring_width_retail.h').write_text('struct Fixup{unsigned site,target,kind;};\nstatic const unsigned char retailCode[]={'+','.join(map(str,body))+'};\nstatic const Fixup fixups[]={'+','.join('{%u,%u,%u}'%f for f in fixups)+'};\n')
(w/'font_vstring_width_aliases.h').write_text('#pragma comment(linker,"/alternatename:@FontWidth@12='+specs[0][2]+'")\n')
settings=ROOT/'rebuild/src/compiled/01/39/VirtualDimensionsData_013961e8.cpp';fixture=ROOT/'rebuild/tests/integration/NativeFontVStringWidth_test.cpp';sources.append(settings)
for i,path in enumerate([settings,fixture]):
 obj=w/f'extra{i}.obj';r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(w),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),str(path)],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
cmd=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(w/'fixture.exe'),'kernel32.lib']
for omit in range(6):
 r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout),r.stdout+r.stderr
r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
r=subprocess.run([str(w/'fixture.exe')],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode,(w,r.returncode,r.stdout,r.stderr);assert 'cases=18432 snapshots=110592' in r.stdout
report=dict(cases=18432,snapshots=110592,width_cases=9216,vertical_cases=9216,providers=proofs,omission_checks=6,source_sha256={str(p.relative_to(ROOT)):hashlib.sha256(p.read_bytes()).hexdigest() for p in sources},fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest(),boundary='Actual virtual-font width,string chararray,coordinate X/Y,display dimensions and dimension globals;six mandatorysources.Direct vertical results are also checked with mandatory binary32numerator rounding before division.Font virtual measure and systemgetter are callbackboundaries;emptytext sentinel fixture storage.Independent full32KiB+global snapshots afterthreecallbacks/finalreturn;null/live/empty string,signed/zero dimensions,floatNaN/infinity/subnormal patterns,three x87 precisions,string/system/display/global callback mutations.')
(ROOT/'rebuild/compile-gate/native-font-vstring-width.json').write_text(json.dumps(report,indent=2)+'\n');print(r.stdout.strip());print('NATIVE_FONT_VSTRING_WIDTH_GATE_PASS')
