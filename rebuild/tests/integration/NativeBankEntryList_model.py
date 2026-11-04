"""CBankFile::ReadEntryList cases: entry-list streams (BIG.md section 7 record layout) plus the
independently derived observations the native host checks."""
import random,struct
def lpstr(b):return struct.pack('<I',len(b))+b
def cstr(b):return b.split(b'\0',1)[0]
def scenario(seed):
 rng=random.Random(seed);flags=[0,8,16,32,8|16|32,8|32][seed%6];read_only=(seed//6)%2;chunk=[4096,7,3,1,64][(seed//12)%5]
 if seed>=360:flags=[8|16,16|32][seed%2]
 count=[0,1,2,5,12][(seed//60)%5] if seed<300 else rng.randrange(0,24)
 duplicate=count>2 and not flags&32 and seed%7==0
 indices=rng.sample(range(1,60),count) if count else []
 if duplicate:indices[-1]=indices[0]
 max_index=max(indices+[0])+rng.randrange(0,4)
 types=[(rng.randrange(0,9),rng.randrange(0,1<<20)) for _ in range(rng.randrange(0,4))]
 entries=[]
 for index in indices:
  symbol=bytes(rng.choice(b'ABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789') for _ in range(rng.randrange(0,14)))
  if rng.random()<0.15:symbol=symbol[:3]+b'\0'+symbol[3:]
  names=[bytes(rng.choice(b'abcdefghij\\./_') for _ in range(rng.randrange(0,10))) for _ in range(rng.choice((0,0,1,2,3)))]
  info=bytes(rng.randrange(256) for _ in range(rng.choice((0,1,4,9,20))))
  entries.append(dict(magic=(0x2a2a0000|index) if seed<360 else rng.randrange(1<<32),index=index,type=rng.choice((0,1,7,0x1ff,0xabcd12)),size=rng.randrange(0,1<<24),offset=rng.randrange(0,1<<30),crc=rng.randrange(1<<32),timestamp=rng.randrange(1<<32),symbol=symbol,names=names,info=info))
 crc=[(rng.randrange(1<<32),rng.randrange(1000)) for _ in range(rng.choice((0,1,3,17,40)))];crc=list({k:v for k,v in crc}.items())
 return dict(flags=flags,read_only=read_only,chunk=chunk,max_index=max_index,types=types,entries=entries,crc=crc,crc_dirty=seed%3!=0,crc_extra=rng.choice((0,1,5)))
def stream(case):
 out=struct.pack('<I',len(case['types']))+b''.join(struct.pack('<II',*t) for t in case['types']);ends=[]
 for e in case['entries']:
  out+=struct.pack('<6I',e['magic'],e['index'],e['type'],e['size'],e['offset'],e['crc'])+lpstr(e['symbol'])+struct.pack('<II',e['timestamp'],len(e['names']))
  out+=b''.join(lpstr(n) for n in e['names'])+lpstr(e['info']);ends.append(len(out))
 return out,ends
def words(b):return [len(b)]+list(struct.unpack('<%dI'%((len(b)+3)//4),b+b'\0'*(-len(b)%4)))
def cases():return list(range(480))
def write_cases(path):
 out=[len(cases())];stats=dict(entries=0,updates=0,overflow=0,dirty=0,read_only=0,duplicates=0)
 for seed in cases():
  c=scenario(seed);data,ends=stream(c);e=c['entries']
  out+=[c['flags'],c['read_only'],c['chunk'],c['max_index'],len(e)]+words(data)
  out+=[len(c['types'])]+[w for t in c['types'] for w in t]
  out+=[len(c['crc']),1 if c['crc_dirty'] else 0,c['crc_extra']]+[w for p in c['crc'] for w in p]
  for x,end in zip(e,ends):
   out+=[x['index'],x['type'],x['size'],x['offset'],x['crc'],x['timestamp']]+words(cstr(x['symbol']))+[len(x['names'])]
   for n in x['names']:out+=words(cstr(n))
   out+=words(x['info'])+[end]
  want=sorted(c['crc']) if c['crc_dirty'] else c['crc'];out+=[len(want)]+[w for p in want for w in p]
  stats['entries']+=len(e);stats['updates']+=len(e) if c['flags']&32 else 0;stats['overflow']+=c['chunk']<16 and len(e)>0;stats['dirty']+=c['crc_dirty'] and len(c['crc'])>1;stats['read_only']+=c['read_only'];stats['duplicates']+=len({x['index'] for x in e})<len(e)
 open(path,'wb').write(struct.pack('<%dI'%len(out),*out));return stats
