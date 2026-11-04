from pathlib import Path
import sys,re,struct,json,hashlib,subprocess
import pefile
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import check_one as V
from gate_game_component_abi import Coff
def validate(b,env,save):
 obj=b/'fixture.obj';r=V.cl(['/nologo','/c','/O2','/MT','/EHsc','/I'+str(b/'include'),'/Fo'+str(obj),str(b/'fixture.cpp')],env,b)
 save('fixture.compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,r.stdout+r.stderr
 exe=b/'probe.exe';r=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/map:'+str(b/'probe.map'),'/out:'+str(exe),str(b/'root.obj'),str(obj),'kernel32.lib'],cwd=b,env=env,capture_output=True,text=True)
 save('link.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,r.stdout+r.stderr
 data=b/'owned-file.bin';assert not data.exists();r=subprocess.run([str(exe),str(data)],cwd=b,capture_output=True,text=True,timeout=15)
 save('positive.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode and 'FABLETLC_CDISKFILEWIN32_OPEN_0099A6A0_PASS' in r.stdout,r.stdout+r.stderr
 missing=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(b/'missing.exe'),str(obj),'kernel32.lib'],cwd=b,env=env,capture_output=True,text=True)
 save('missing.link.json',dict(returncode=missing.returncode,stdout=missing.stdout,stderr=missing.stderr));assert missing.returncode and '?Open@CDiskFileWin32@@' in missing.stdout+missing.stderr
 c=Coff(b/'root.obj');symbol='?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z';section,offset=c.symbols[symbol];body,rels=c.sections[section]
 assert offset==0 and len(body)==640 and len(rels)==51
 mapping={}
 for line in (b/'probe.map').read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 with pefile.PE(str(b/'probe.exe')) as pe:
  address=mapping[symbol];expected=bytearray(body)
  for site,(name,kind) in rels.items():
   if name.startswith('$L'):
    sec,off=c.symbols[name];assert sec==section;target=address+off
   else:target=mapping[name]
   value=target+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0);struct.pack_into('<I',expected,site,value&0xffffffff)
  assert pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,640)==bytes(expected)
 text=(b/'root.cpp').read_text();controls=[]
 for name,old,new in [('wrong-access-state','AccessType = accessType;','AccessType = FILE_ACCESS_NULL;'),('wrong-length','Length = GetFileSize(handle, 0);','Length = 0;'),('missing-open-flag','Opened = true;','Opened = false;'),('bad-parameter-type','CBBBFileException::BAD_PARAMETERS','CBBBFileException::GENERIC'),('missing-file-type','CBBBFileException::FILE_NOT_FOUND','CBBBFileException::GENERIC'),('missing-directory','CreateDirectoryForFile(fullPathname);',';')]:
  assert old in text;source=b/(name+'.cpp');source.write_text(text.replace(old,new));obj=b/(name+'.obj')
  r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(b/'include'),'/Fo'+str(obj),str(source)],env,b);(b/(name+'.compile.json')).write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr
  exe=b/(name+'.exe');r=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/out:'+str(exe),str(obj),str(b/'fixture.obj'),'kernel32.lib'],cwd=b,env=env,capture_output=True,text=True);(b/(name+'.link.json')).write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert not r.returncode,r.stdout+r.stderr
  data=b/(name+'.owned.bin');assert not data.exists();r=subprocess.run([str(exe),str(data)],cwd=b,capture_output=True,text=True,timeout=15);(b/(name+'.run.json')).write_text(json.dumps(dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr),indent=2));assert r.returncode==80 and 'FAIL' in r.stdout,(name,r.returncode,r.stdout,r.stderr);controls.append(name)
 report=dict(status='PASS_SCRATCH',linked_bytes=640,linked_fixups=51,host_cases=7,controls=controls,exe_sha256=hashlib.sha256((b/'probe.exe').read_bytes()).hexdigest(),boundary='Actual Windows file open/size/seek and caller-caught native throws; string/path/error-message helpers controlled. No full exception-unwinding/cleanup claim. Build-owned files retained.')
 return report
