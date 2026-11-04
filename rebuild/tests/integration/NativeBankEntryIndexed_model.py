"""Cross all eight bank indexing modes with every accepted entry-list case."""
import sys,struct
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'rebuild/tests/integration'))
import NativeBankEntryList_model as B
from NativeBankBlockOrder_model import stl_sort
cstr=B.cstr;words=B.words;stream=B.stream
def crc(text):
 value=0
 for byte in text:
  value^=byte
  for bit in range(8):value=(value>>1)^(0xedb88320 if value&1 else 0)
 return value
def scenario(seed):
 c=B.scenario(seed%480);mask=seed//480;c['flags']|=(64 if mask&1 else 0)|(128 if mask&2 else 0)|(256 if mask&4 else 0)
 if mask&2:
  for i,e in enumerate(c['entries']):
   if (seed+i)&1:e['names']=[name.upper() for name in e['names']]
 return c
def cases():return list(range(3840))
def write_cases(path):
 modes=set();out=[len(cases())];stats=dict(entries=0,updates=0,read_only=0,symbol_nodes=0,filename_nodes=0,crc_pairs=0)
 for seed in cases():
  c=scenario(seed);data,ends=stream(c);e=c['entries'];flags=c['flags'];modes.add(flags)
  out+=[flags,c['read_only'],c['chunk'],c['max_index'],len(e)]+words(data)
  out+=[len(c['types'])]+[w for t in c['types'] for w in t]
  out+=[len(c['crc']),int(c['crc_dirty']),c['crc_extra']]+[w for p in c['crc'] for w in p]
  for x,end in zip(e,ends):
   out+=[x['index'],x['type'],x['size'],x['offset'],x['crc'],x['timestamp']]+words(cstr(x['symbol']))+[len(x['names'])]
   for name in x['names']:out+=words(cstr(name))
   out+=words(x['info'])+[end]
  pairs=list(c['crc'])+([(crc(cstr(x['symbol'])),x['index']) for x in e] if flags&256 else [])
  if c['crc_dirty'] or flags&256 and e:pairs,_=stl_sort(pairs,lambda a,b:a[0]<b[0])
  out+=[len(pairs)]+[w for pair in pairs for w in pair]
  symbols={cstr(x['symbol']):x['index'] for x in e} if flags&64 else {}
  filenames=[]
  if flags&128:
   for x in e:
    if x['names'] and cstr(x['names'][0]):
     key=cstr(x['names'][0]).replace(b'\\',b'/').rsplit(b'/',1)[-1].lower();filenames.append((key,x['index']))
  for records in (sorted(symbols.items()),sorted(filenames,key=lambda item:item[0])):
   out+=[len(records)]
   for key,value in records:out+=words(key)+[value]
  stats['entries']+=len(e);stats['updates']+=len(e) if flags&32 else 0;stats['read_only']+=c['read_only'];stats['symbol_nodes']+=len(symbols);stats['filename_nodes']+=len(filenames);stats['crc_pairs']+=len(pairs)
 assert modes=={basic|index for basic in range(0,64,8) for index in range(0,512,64)};stats['flag_modes']=len(modes)
 Path(path).write_bytes(struct.pack('<%dI'%len(out),*out));return stats
