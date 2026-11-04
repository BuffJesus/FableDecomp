"""Bounded synthetic native text/group resource archive."""
from pathlib import Path
import struct,json,hashlib
def create_archive(destination,alignment=1):
 assert alignment in (1,4,16,2048)
 u=lambda *v:struct.pack('<'+'I'*len(v),*v);blob=lambda b:u(len(b))+b
 b=bytearray(16);entries=[]
 for index in range(1,65):
  round=(index-1)//2
  if index%2:
   text=('native_%d'%round).encode('utf-16le')+bytes(2);payload=text+blob(b'SPEECH')+blob(b'NARRATOR')+blob(b'SOUND')+u(round%5)+b''.join(u(0x80000000+round*7+i)+('TAG_%d'%i).encode()+b'\0' for i in range(round%5));kind=0
  else:payload=u(round%17)+b''.join(u(0xf0000000+round*11+i*3) for i in range(round%17));kind=1
  b+=bytes((-len(b))%alignment)
  entries.append(dict(index=index,type=kind,offset=len(b),size=len(payload)));b+=payload
 b+=bytes((-len(b))%alignment)
 at=len(b);toc=u(2,0,32,1,32)
 for e in reversed(entries):toc+=u(42,e['index'],e['type'],e['size'],e['offset'],0)+blob(('ENTRY_%d'%e['index']).encode())+u(0,0)+blob(b'')
 b+=toc;footer=len(b);b+=u(1)+b'RESOURCE_TEST\0'+u(106,64,at,len(toc),alignment);struct.pack_into('<4sIII',b,0,b'BIGB',100,footer,len(b)-footer);Path(destination).write_bytes(b);return dict(entries=entries,alignment=alignment,sha256=hashlib.sha256(b).hexdigest())
