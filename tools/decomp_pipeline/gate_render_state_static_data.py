"""Validate render-state data against retail, SDK identities, and linked defaults."""
import argparse,hashlib,json,re,struct,subprocess,tempfile
from pathlib import Path
import check_one,pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
ROOT=Path(__file__).resolve().parents[2]
DEFAULTS=[('01396dd0',3,'D3DTADDRESS_CLAMP'),('01396ddc',2,'D3DCMP_LESS'),('01396e04',4,'D3DCMP_LESSEQUAL'),('01396e34',8,'D3DCMP_ALWAYS'),('01396e5c',2,'D3DTEXF_LINEAR'),('01396e70',1,'D3DTOP_DISABLE'),('01396e7c',4,'D3DTOP_MODULATE'),('01396ed0',1,'D3DBLENDOP_ADD'),('01396ee8',1,'D3DTA_CURRENT'),('01396eec',0,'D3DTA_DIFFUSE'),('01396ef0',2,'D3DTA_TEXTURE'),('01396f38',1,'D3DSTENCILOP_KEEP'),('01396f58',0,'D3DFOG_NONE'),('01396f68',1,'D3DBLEND_ZERO'),('01396f6c',2,'D3DBLEND_ONE'),('01396fa0',2,'D3DSHADE_GOURAUD'),('01396fa4',3,'D3DFILL_SOLID'),('01396fb0',3,'D3DCULL_CCW'),('01396fbc',0,'D3DVBF_DISABLE'),('01396fcc',0,'D3DTTFF_DISABLE'),('01396ff4',0,'D3DMCS_MATERIAL')]
DEFAULTS += [('01396e14',8,'D3DCMP_ALWAYS'),('01396e74',2,'D3DTOP_SELECTARG1'),('01396f40',3,'D3DSTENCILOP_REPLACE')]
DEFAULTS += [('01396e58',1,'D3DTEXF_POINT')]
DEFAULTS += [('01396de8',5,'D3DCMP_GREATER'),('01396f78',5,'D3DBLEND_SRCALPHA'),('01396f7c',6,'D3DBLEND_INVSRCALPHA')]
SPECS=[(int(a,16),'?FableRenderDefaultValue_'+a+'@@3KA',[value],[name]) for a,value,name in DEFAULTS]
SPECS += [(0x1396e64,'?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA',[1,2,0],['D3DTEXF_POINT','D3DTEXF_LINEAR','D3DTEXF_NONE']),(0x1396da8,'?FableRenderWrapStateTypes_01396da8@@3PAW4_D3DRENDERSTATETYPE@@A',list(range(128,136)),['D3DRS_WRAP'+str(i) for i in range(8)]),(0x13cb508,'?FableCurrentRenderStateManager_013cb508@@3PAVCRenderStateManager@@A',[0],[])]
CALLER=ROOT/'rebuild/src/compiled/00/a0/CRenderStateManager_SetDefaultRenderStates_00a059e0.cpp'

def coff_data(coff,symbol,size):
 section,offset=coff.symbols[symbol];header=20+(section-1)*40;length,raw=struct.unpack_from('<II',coff.data,header+16);flags=struct.unpack_from('<I',coff.data,header+36)[0]
 assert offset+size<=length and not flags&0x20 and not coff.sections[section][1]
 if raw:return coff.data[raw+offset:raw+offset+size]
 assert flags&0x80;return bytes(size)

def main():
 ap=argparse.ArgumentParser(description=__doc__)
 ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/01/39/CRenderStateManager_StaticData_01396da8.cpp')
 ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/RenderStateStaticData_test.cpp')
 ap.add_argument('--sdk',type=Path,default=Path('C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/shared/d3d9types.h'))
 ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/render-state-static-data.json');a=ap.parse_args()
 sdk=a.sdk.read_text();env=check_one.env();env['INCLUDE']=str(ROOT/'rebuild/include')+';'+env['INCLUDE'];w=Path(tempfile.mkdtemp(prefix='render-state-static-data-'));objects=[]
 for i,source in enumerate([a.source,CALLER,a.test]):
  obj=w/('source'+str(i)+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,w);assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
 data=Coff(objects[0]);fixture=Coff(objects[2]);assert all(symbol not in fixture.symbols for _,symbol,_,_ in SPECS);checked=[]
 with pefile.PE(str(EXE)) as pe:
  for address,symbol,values,names in SPECS:
   size=4*len(values);expected=struct.pack('<'+'I'*len(values),*values);rva=address-pe.OPTIONAL_HEADER.ImageBase
   for name,value in zip(names,values):
    m=re.search(r'\b'+name+r'\b\s*(?:=\s*|\s+)(0x[0-9a-fA-F]+|[0-9]+)\b',sdk);assert m and int(m.group(1),0)==value,(name,value)
   native=pe.get_data(rva,size)
   if len(native)!=size:
    assert address==0x13cb508
    section=next(s for s in pe.sections if s.VirtualAddress<=rva and rva+size<=s.VirtualAddress+s.Misc_VirtualSize)
    assert rva>=section.VirtualAddress+section.SizeOfRawData;native=bytes(size)
   assert coff_data(data,symbol,size)==native==expected
   checked.append(dict(address='%08x'%address,symbol=symbol,values=values,sdk_names=names))
 exe=w/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
 for omit in [0,1]:
  r=subprocess.run(cmd+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode,'data provider and real caller must both be required'
 r=subprocess.run(cmd+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
 r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'RENDER_STATE_STATIC_DATA_BEHAVIOR_PASS scenarios=486' in r.stdout,r.stdout+r.stderr
 report=dict(data_symbols=checked,data_bytes=sum(len(v)*4 for _,_,v,_ in SPECS),behavior=r.stdout.strip(),source_sha256=hashlib.sha256(a.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),caller_sha256=hashlib.sha256(CALLER.read_bytes()).hexdigest(),sdk_sha256=hashlib.sha256(a.sdk.read_bytes()).hexdigest(),missing_provider_checks=2,coverage_claim='data only; no new reconstructed functions')
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('RENDER_STATE_STATIC_DATA_PASS symbols=%d bytes=%d compiled=486'%(len(SPECS),sum(len(v)*4 for _,_,v,_ in SPECS)))
if __name__=='__main__':main()
