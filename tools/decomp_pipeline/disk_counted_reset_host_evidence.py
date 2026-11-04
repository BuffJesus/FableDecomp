"""Final linked typed ownership audit and six executed semantic controls."""
import subprocess,re,struct,json
import pefile
import check_one as V
from gate_game_component_abi import Coff
RESET='?Reset@?$CCountedPointer@VCDiskFileWin32@@@@QAEXPAVCDiskFileWin32@@@Z'
DELETE='?DeleteData@?$CCountedPointer@VCDiskFileWin32@@@@CIXPAX@Z'

def validate(build,root,env,save):
 def compile(name,source):
  obj=build/(name+'.obj');r=V.cl(['/nologo','/c','/O2','/MT','/I'+str(build/'include'),'/Fo'+str(obj),str(source)],env,build)
  save(name+'.compile.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,r.stdout+r.stderr;return obj
 fixture=compile('fixture',build/'fixture.cpp')
 def link(name,objects):
  exe=build/(name+'.exe');r=subprocess.run([str(V.VC/'bin/link.exe'),'/nologo','/subsystem:console','/map:'+str(exe.with_suffix('.map')),'/out:'+str(exe),*map(str,objects),'kernel32.lib'],cwd=build,env=env,capture_output=True,text=True,timeout=30)
  save(name+'.link.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return exe,r
 exe,r=link('probe',[root,fixture]);assert not r.returncode,r.stdout+r.stderr
 mapping={}
 for line in exe.with_suffix('.map').read_text().splitlines():
  m=re.match(r'^\s+[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b',line)
  if m:mapping[m[1]]=int(m[2],16)
 c=Coff(root);fixups=0
 with pefile.PE(str(exe)) as pe:
  for symbol,size in ((RESET,103),(DELETE,11)):
   section,offset=c.symbols[symbol];body,rels=c.sections[section];assert not offset and len(body)==size;expected=bytearray(body);address=mapping[symbol]
   for site,(name,kind) in rels.items():
    assert kind in (6,20);value=mapping[name]+struct.unpack_from('<i',body,site)[0]-(address+site+4 if kind==20 else 0);struct.pack_into('<I',expected,site,value&0xffffffff);fixups+=1
   assert pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,size)==bytes(expected)
 def run(name,exe):
  r=subprocess.run([str(exe)],cwd=build,capture_output=True,text=True,timeout=10);save(name+'.run.json',dict(returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));return r
 r=run('positive',exe);assert not r.returncode and 'DISK_RESET_HOST_PASS cases=36' in r.stdout,r.stdout+r.stderr
 _,r=link('missing-root',[fixture]);assert r.returncode and RESET in r.stdout+r.stderr
 text=(build/'root.cpp').read_text();controls=[]
 for name,old,new in [('wrong-refcount','--Info->RefCount;','++Info->RefCount;'),('missing-object-delete','Info->DeleteFunc(Info->Data);',';'),('missing-info-delete','delete Info;',';'),('retained-info',' Info=0;',' ;'),('wrong-new-data',' Data=data;',' Data=0;'),('missing-callback','&DeleteData','0')]:
  assert text.count(old)==1;source=build/(name+'.cpp');source.write_text(text.replace(old,new));obj=compile(name,source);bad,r=link(name,[obj,fixture])
  # Missing-callback removes the only address-taking use; fixture's direct call
  # still roots DeleteData through the unmodified provider, so retain that COMDAT.
  if name=='missing-callback' and r.returncode:
   source.write_text(text.replace(old,new)+'\ntemplate void CCountedPointer<CDiskFileWin32>::DeleteData(void*);\n');obj=compile(name+'-rooted',source);bad,r=link(name+'-rooted',[obj,fixture])
  assert not r.returncode,r.stdout+r.stderr
  r=run(name,bad);assert r.returncode==80 and 'DISK_RESET_FAILURE' in r.stderr,(name,r.stdout,r.stderr);controls.append(name)
 return dict(host_cases=36,linked_bodies=2,linked_fixups=fixups,missing_root_rejected=True,host_controls=controls)
