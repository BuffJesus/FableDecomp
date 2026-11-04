"""Bounded schema106 direct BBBB bank with valid per-text narrator metadata and narrator table."""
from pathlib import Path
import struct,json,hashlib
def create_archive(destination,alignment=1):
 assert alignment in (1,4,16,2048)
 u=lambda *v:struct.pack('<'+'I'*len(v),*v);blob=lambda b:u(len(b))+b
 b=bytearray(64);entries=[]
 for index in range(1,65):
  round=(index-1)//2
  if index%2:
   text=('native_%d'%round).encode('utf-16le')+bytes(2);payload=text+blob(b'SPEECH')+blob(b'NARRATOR')+blob(b'SOUND')+u(round%5)+b''.join(u(0x80000000+round*7+i)+('TAG_%d'%i).encode()+b'\0' for i in range(round%5));kind=0
  else:payload=u(round%17)+b''.join(u(0xf0000000+round*11+i*3) for i in range(round%17));kind=1
  b+=bytes((-len(b))%alignment)
  entries.append(dict(index=index,type=kind,offset=len(b),size=len(payload)));b+=payload
 b+=bytes((-len(b))%alignment)
 at=len(b);entries.append(dict(index=65,type=2,offset=0,size=0));toc=u(3,0,32,1,32,2,1)
 for e in reversed(entries):
  info=struct.pack('<i',-5-(e['index']-1)//2) if e['type']==0 else b'' if e['type']==1 else u(3)+b'ALPHA\0\0GAMMA\0'
  toc+=u(42,e['index'],e['type'],e['size'],e['offset'],0)+blob(('ENTRY_%d'%e['index']).encode())+u(0,0)+blob(info)
 b+=toc;struct.pack_into('<8I',b,0,0x42424242,0x73,106,1,alignment,65,65,at);Path(destination).write_bytes(b);return dict(entries=entries,alignment=alignment,sha256=hashlib.sha256(b).hexdigest())
