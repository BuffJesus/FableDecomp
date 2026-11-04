"""Build a bounded synthetic BIGB and derive expected rows with parse_bigb/zlib."""
from pathlib import Path
import struct,zlib,sys,hashlib,collections
R=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(R/'tools'))
import parse_bigb as M
def create_archive(destination):
 data=bytearray(16);banks=[];samples=[]
 u32=lambda *values:struct.pack('<'+'I'*len(values),*values)
 blob=lambda value:u32(len(value))+value
 for bank,(count,alignment) in enumerate(((1,1),(3,4),(32,16),(257,2048))):
  entries=[];name='SYNTHETIC_%u'%bank
  for index in range(1,count+1):
   data.extend(bytes((-len(data))%alignment));offset=len(data);size=0 if index==2 else (17+index*29+bank*53)%193+1;payload=bytes((i*37+index*11+bank*19)&255 for i in range(size));data.extend(payload);entries.append(dict(index=index,type=(index*17+bank)%256,size=size,offset=offset,symbol=('ENTRY_%u_%04u'%(bank,index)).encode(),payload=payload))
  banks.append(dict(name=name,version=100+bank,count=count,alignment=alignment,entries=entries))
 for bank in banks:
  toc=bytearray();counts=collections.Counter(e['type'] for e in bank['entries']);toc+=u32(len(counts))
  for kind,count in sorted(counts.items()):toc+=u32(kind,count)
  for e in reversed(bank['entries']):
   deps=[('dependency_%u'%i).encode() for i in range(e['index']%11)];info=bytes((i+e['index'])&255 for i in range(e['index']%18));toc+=u32(42,e['index'],e['type'],e['size'],e['offset'],0x12345678)+blob(e['symbol'])+u32(0x87654321,len(deps))+b''.join(blob(x) for x in deps)+blob(info)
  bank['offset']=len(data);bank['toc_size']=len(toc);data+=toc
 footer=len(data);data+=u32(len(banks))
 for bank in banks:data+=bank['name'].encode()+b'\0'+u32(bank['version'],bank['count'],bank['offset'],bank['toc_size'],bank['alignment'])
 struct.pack_into('<4sIII',data,0,b'BIGB',100,footer,len(data)-footer);asset=Path(destination);asset.write_bytes(data);sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();before=sha(asset)
 magic,version,at,length=M.parse_header(data);assert magic==b'BIGB' and version==100 and at+length==len(data);subs,end=M.parse_footer(data,at);assert end==len(data);expected=[];expected_sub=[];expected_async=[];expected_batch=[];expected_shared=[]
 for sub in subs:
  entries,stats,end,error=M.parse_toc(data,sub,at);assert not error and end==sub['offset']+sub['size'];expected_sub.append(f"SUB\t{sub['name']}\t{sub['version']}\t{sub['entry_count']}\t{sub['offset']}\t{sub['size']}\t{sub['align']}")
  for e in entries:
   payload=data[e['offset']:e['offset']+e['size']];crc=zlib.crc32(payload,0xffffffff)^0xffffffff;expected.append(f"ENTRY\t{sub['name']}\t{e['id']}\t{e['type']}\t{e['size']}\t{e['offset']}\t{e['name']}\t{payload[:17].hex()}\t{crc:08x}")
   if e['id'] in (1,sub['entry_count']//2,sub['entry_count']):
    relative=e['size']//2;n=min(37,e['size']-relative);expected_async.append(f"ASYNC\t{sub['name']}\t{e['id']}\t{relative}\t{n}\t{payload[relative:relative+n].hex()}")
   for slot,selected in enumerate((1,sub['entry_count']//2 or 1,sub['entry_count'])):
    if e['id']==selected:
     relative=e['size']//3;n=min(37,e['size']-relative);expected_batch.append(f"BATCH\t{sub['name']}\t{slot}\t{e['id']}\t{relative}\t{n}\t{payload[relative:relative+n].hex()}")
   if e['id']==1:
    relative=e['size']//4;n=min(37,e['size']-relative)
    for slot in range(3):expected_shared.append(f"SHARED\t{sub['name']}\t{slot}\t1\t{relative}\t{n}\t{payload[relative:relative+n].hex()}")

 return dict(asset=str(asset),asset_sha256=before,banks=len(banks),entries=len(expected),expected=dict(SUB=expected_sub,ENTRY=expected,ASYNC=expected_async,BATCH=expected_batch,SHARED=expected_shared))
