"""Packed metadata decoding in the native bank factory and Windows read chain."""
import struct
import bank_factory_worker_evidence as factory
from gate_game_component_abi import Coff
from pe_oracle import EXE,read_virtual_data
R=factory.R
PACKED=R/'rebuild/src/compiled/00/a6/CPackedUIntArray_Get_00a629f0.cpp'
EXTRAS=factory.EXTRAS
SOURCES=factory.SOURCES+[PACKED]
REQUIRED_SOURCES=factory.SOURCES[:-1]+[PACKED]

def audit(objects,sources,selected):
 records=factory.audit(objects,sources,selected)
 c=Coff(objects[sources.index(PACKED)])
 table=next(s for s in c.symbols if 'g_PackedUIntMasks_013979E8' in s)
 for symbol,address,size in [('??ACPackedUIntArray@@QBEKK@Z',0xa629f0,101),(table,0x13979e8,132)]:
  section,offset=c.symbols[symbol];assert offset==0
  body,rels=c.sections[section];assert len(body)==size,(symbol,len(body),size)
  patched=bytearray(body)
  for site,(target,kind) in rels.items():
   assert target==table and kind==6
   struct.pack_into('<I',patched,site,(0x13979e8+struct.unpack_from('<i',body,site)[0])&0xffffffff)
  assert bytes(patched)==read_virtual_data(EXE.read_bytes(),address,size),symbol
  record=dict(symbol=symbol,address=address,body=body.hex(),relocations=[dict(site=site,symbol=name,kind=kind) for site,(name,kind) in rels.items()])
  assert not any(r['symbol']==symbol for r in selected)
  selected.append(record);records.append(record)
 return records

def controls(sources):
 cold={'factory-wrong-runtime-size','factory-size-ignores-index','factory-offset-ignores-index'}
 result=[r for r in factory.controls(sources) if r[0] not in cold]
 def add(name,source,old,new):
  i=sources.index(source);text=source.read_text();assert text.count(old)==1,(name,text.count(old));result.append((name,i,text.replace(old,new)))
 add('packed-no-cross-word',PACKED,'value |= data[1] << (32 - shift);',';')
 add('packed-no-bias',PACKED,'+ Bias;','+ 0;')
 add('packed-ignores-index',PACKED,'Bits * index','Bits * 0')
 add('packed-size-runtime-fallback',EXTRAS[0],'if (PackedDataSize.Size)','if (false)')
 add('packed-size-wrong-index',EXTRAS[0],'PackedDataSize[index - 1]','PackedDataSize[0]')
 offset=R/'rebuild/src/compiled/00/9c/CBankFile_GetEntryDataOffset_009cc270.cpp'
 add('packed-offset-runtime-fallback',offset,'if (PackedDataOffset.Size)','if (false)')
 add('packed-offset-wrong-index',offset,'PackedDataOffset[index - 1]','PackedDataOffset[0]')
 add('packed-no-width-mask',PACKED,'value & g_PackedUIntMasks_013979E8[Bits]','value')
 return result
