"""PDB method signatures and retail bank vtable slots, independent of probe output."""
import struct
BASE_NAMES=[
'??_ECBankFile@@UAEPAXI@Z',
'?OpenReadOnly@CBankFile@@UAE_NABVCCharString@@K@Z',
'?OpenReadWrite@CBankFile@@UAEXABVCCharString@@KK@Z',
'?OpenFileReadOnly@CBankFile@@UAE_NABVCWideString@@K@Z',
'?OpenFileReadWrite@CBankFile@@UAEXABVCWideString@@KK@Z',
'?Close@CBankFile@@UAEXXZ',
'?AddEntryMultipleFiles@CBankFile@@UAEXABVCCharString@@KABV?$CArray@VCCharString@@@@V?$CCountedPointer@VCBankStateBlock@@@@@Z',
'?Update@CBankFile@@UAEXXZ','__purecall',
'?AdjustUpdateEntries@CBankFile@@MAEKXZ',
'?NotifyBankSize@CBankFile@@MAEXKABV?$CArray@VCBankFileDataTypeCount@@@@@Z',
'?NotifyBankOpened@CBankFile@@MAEXXZ',
'?NotifyEntryAdded@CBankFile@@MAEXKKABV?$CArray@D@@@Z',
'?NotifyEntryUpdated@CBankFile@@MAEXKKABV?$CArray@D@@@Z',
'?NotifyEntryRemoved@CBankFile@@MAEXK@Z',
'?NotifyPostEntriesLoaded@CBankFile@@MAEXXZ',
'?PrepareDataForInclusion@CBankFile@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z',
'?IsDiskFile@CBankFile@@MAE_NKK@Z',
'?GetLastModifiedTime@CBankFile@@MAE?AVCDateAndTime@@ABVCCharString@@@Z',
'?GetEnumerationType@CBankFile@@MBE?AVCCharString@@K@Z']
BASE_TARGETS=[0x9d47a0,0x9d06f0,0x9cb960,0x9d1090,0x9d1160,0x9ce360,0x9ce5e0,0x9cec60,0xbfea08,0x9cba70,0x9cbbf0,0x9cbc00,0x9cbc20,0x9cbc10,0x9cbc30,0x50b440,0x9cc9f0,0x9cbc40,0x9cbc50,0x9cbd10]
ASYNC_TARGETS={0:0x9d81a0,1:0x9d56c0,2:0x9d51d0,3:0x9d5820,4:0x9d5200,5:0x9d58a0}
# Overrides match the retail tables, not editor-only donor methods.
TABLES=[('CBankFile',0x129b54c,False,{}),
('CBankFileAsync',0x129b8d4,True,{}),
('CShaderDataBank',0x129d644,False,{0:0xa62860,5:0xa62110,8:0xa61f90,12:0xa623d0,14:0xa61fc0,16:0xa62000}),
('CFontDataBank',0x129da84,False,{0:0xa63b30,5:0xa635c0,8:0xa63340,12:0xa636c0,14:0xa635f0,16:0xa63420,17:0xa63ac0,18:0xa63ad0,19:0xa63380}),
('CStreamingFontDataBank',0x129d5cc,True,{0:0xa5fa30,5:0xa5f420,8:0xa5ee90,9:0xa5eea0,12:0xa5f430,14:0xa5f0b0,16:0xa5f0f0,17:0xa5f9c0,18:0xa5f9d0,19:0xa5ef10}),
('CDataBank@NGameText',0x129b4d4,True,{0:0x9cb840,5:0x9c8bf0,8:0x9c79d0,10:0x9c82c0,12:0x9c8c00,18:0x9cb710,19:0x9c79e0}),
('CParticleDataBank@NParticleEngine',0x129f38c,True,{0:0xaf21e0,5:0xaf15b0,8:0xaf11e0,10:0xaf16e0,17:0xaf11f0})]

def table_contract(owner):
 _,address,asynchronous,overrides=next(r for r in TABLES if r[0]==owner)
 names=list(BASE_NAMES);targets=list(BASE_TARGETS)
 if asynchronous:
  for slot,target in ASYNC_TARGETS.items():names[slot]=BASE_NAMES[slot].replace('CBankFile@@','CBankFileAsync@@');targets[slot]=target
 for slot,target in overrides.items():
  names[slot]=('?GetDataFormatVersion@'+owner+'@@MAEKXZ') if slot==8 else BASE_NAMES[slot].replace('CBankFile@@',owner+'@@')
  targets[slot]=target
 return address,names,targets

def verify(coff,pe,owners):
 records=[]
 for owner in owners:
  address,names,targets=table_contract(owner)
  # Multiple inheritance needs the base-qualified primary vtable symbol.
  table='??_7'+owner+'@@6B'+('CBankFileAsync@@@' if '@' in owner else '@')
  coff.table(table,names)
  actual=struct.unpack('<20I',pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,80));assert list(actual)==targets,(owner,actual)
  records.append(dict(owner=owner,address=address,slots=names,targets=targets))
 return records
