"""Explicit stock Arena round values for smoke fixtures, not live objects."""
import struct
from tools.decode_game_bin_field import field_tag

SHAPES={
 'CArenaRoundDef':(('NumWaves','int'),('Waves','CArenaWaveDef')),
 'CArenaWaveDef':(('NumWaveCreatures','int'),('Creatures','CArenaCreatureDef'),('ShortWave','bool')),
 'CArenaCreatureDef':(('CreatureType','string'),('NumCreatures','int'),('HUDType','string'),('DeathScore','int')),
}
LIMIT=4096

def decode_rounds(raw):
 if not isinstance(raw,bytes) or len(raw)<4 or len(raw)>4*1024*1024:raise ValueError('invalid Arena payload')
 cursor=0
 def take(size):
  nonlocal cursor
  if cursor+size>len(raw):raise ValueError('truncated Arena value')
  value=raw[cursor:cursor+size];cursor+=size;return value
 def word(signed=False):return struct.unpack('<i' if signed else '<I',take(4))[0]
 def vector(shape):
  count=word()
  if count>LIMIT:raise ValueError('Arena vector count exceeds bound')
  return [record(shape) for _ in range(count)]
 def record(shape):
  nonlocal cursor
  result={}
  for name,kind in SHAPES[shape]:
   if word()!=field_tag(name):raise ValueError('Arena field tag/order mismatch: '+name)
   if kind=='int':value=word(True)
   elif kind=='bool':
    value=take(1)[0]
    if value not in (0,1):raise ValueError('invalid serialized Arena bool')
    value=bool(value)
   elif kind=='string':
    end=raw.find(b'\0',cursor)
    if end<0 or end-cursor>16384:raise ValueError('invalid Arena string extent')
    try:value=raw[cursor:end].decode('utf-8')
    except UnicodeDecodeError as error:raise ValueError('invalid Arena UTF-8') from error
    cursor=end+1
   else:value=vector(kind)
   result[name]=value
  return result
 result=vector('CArenaRoundDef')
 if cursor!=len(raw):raise ValueError('trailing Arena bytes')
 return result

VECTOR='vector<CArenaRoundDef>'

def validate(groups,metadata):
 if not isinstance(groups,list) or len(groups)!=1:raise ValueError('Arena fixture requires one anchored ArenaRounds member')
 occupied=set();members={int(key,0) for key in metadata}
 def record(row,shape):
  if not isinstance(row,dict) or set(row)!={name for name,kind in SHAPES[shape]}:raise ValueError('Arena record fields mismatch')
  for name,kind in SHAPES[shape]:
   value=row[name]
   if kind=='int':
    if type(value) is not int or not -2147483648<=value<=2147483647:raise ValueError('invalid signed Arena value')
   elif kind=='bool':
    if type(value) is not bool:raise ValueError('invalid Arena boolean')
   elif kind=='string':
    if not isinstance(value,str) or '\0' in value or len(value.encode('utf-8'))>16384:raise ValueError('invalid Arena string')
   else:
    if not isinstance(value,list) or len(value)>LIMIT:raise ValueError('Arena nested vector exceeds bound')
    for child in value:record(child,kind)
 for group in groups:
  if not isinstance(group,dict):raise ValueError('Arena group must be an object')
  offset,rounds=group.get('offset'),group.get('rounds')
  if type(offset) is not int or offset!=0x1044:raise ValueError('invalid Arena member offset')
  field=metadata.get(hex(offset),{})
  if field.get('defType')!=VECTOR or field.get('name')!=group.get('name') or field.get('verified') is False or not field.get('retailTransferCall'):raise ValueError('Arena member does not match native metadata')
  words={offset,offset+4,offset+8}
  if words&occupied or members&{offset+4,offset+8}:raise ValueError('overlapping Arena members')
  occupied|=words
  if not isinstance(rounds,list) or len(rounds)>LIMIT:raise ValueError('Arena rounds must be bounded')
  for row in rounds:record(row,'CArenaRoundDef')
 return occupied

def decode_vectors(metadata):
 groups=[]
 for key,field in metadata.items():
  if field.get('defType')!=VECTOR:continue
  try:raw=bytes.fromhex(field['value'])
  except (KeyError,TypeError,ValueError) as error:raise ValueError('invalid Arena payload hex') from error
  groups.append(dict(offset=int(key,0),name=field.get('name'),rounds=decode_rounds(raw)))
 validate(groups,metadata);return groups

def flatten(groups):
 values={}
 for group in groups:
  rounds=group['rounds'];values['Rounds_Count']=len(rounds)
  for r,row in enumerate(rounds):
   key='Rounds_'+str(r);values[key+'_NumWaves']=row['NumWaves'];values[key+'_Waves_Count']=len(row['Waves'])
   for w,wave in enumerate(row['Waves']):
    wk=key+'_Waves_'+str(w);values[wk+'_NumWaveCreatures']=wave['NumWaveCreatures'];values[wk+'_ShortWave']=wave['ShortWave'];values[wk+'_Creatures_Count']=len(wave['Creatures'])
    for c,creature in enumerate(wave['Creatures']):
     ck=wk+'_Creatures_'+str(c)
     for name,value in creature.items():values[ck+'_'+name]=value
 return values
