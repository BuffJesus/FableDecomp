"""Initial PE import identity with anexplicit external normal ABI condition."""
import struct
def prove(image,target,profile):
 def require(ok,why):
  if not ok:raise ValueError('external import contract: '+why)
 identities={0xbfe9bc:(0x1440164,'??3@YAXPAX@Z'),0xbfea1a:(0x1440150,'??2@YAPAXI@Z')}
 require(target in identities,'supported operator delete thunk')
 slot,symbol_name=identities[target]
 require(profile['imageBase']==0x400000 and profile['iatSlot']==slot,'supported operator delete thunk')
 require(type(profile['purgeBytes'])is int and profile['purgeBytes']==0 and profile['closedAllPathReturnProof'] is False and profile['implementationVerified'] is False,'external proof scope')
 expected=dict(callingConvention='cdecl',stackPurge=0,ordinaryMsvcCalleeSaved=True,callerFramePreserved=True);require(profile['normalABIRequired']==expected,'required external normal ABI')
 thunk=bytes.fromhex(profile['thunkBytes']);require(thunk==b'\xff\x25'+struct.pack('<I',profile['iatSlot']) and image.bytes_at(target,6)==thunk,'native import thunk bytes')
 descriptor=bytes.fromhex(profile['descriptorBytes']);require(len(descriptor)==20 and image.bytes_at(int(profile['descriptorAddress'],16),20)==descriptor,'PE import descriptor');original,stamp,chain,name,first=struct.unpack('<IIIII',descriptor);base=profile['imageBase'];delta=profile['iatSlot']-(base+first);require(first>0 and 0<=delta<16384 and delta%4==0,'descriptor/IAT slot link')
 lookup_address=base+(original or first)+delta;lookup=bytes.fromhex(profile['lookupBytes']);require(len(lookup)==4 and int(profile['lookupAddress'],16)==lookup_address and image.bytes_at(lookup_address,4)==lookup,'original import lookup');value=struct.unpack('<I',lookup)[0];require(not value&0x80000000 and value>0,'named import')
 module=b'MSVCR71.dll\0';symbol=(symbol_name+'\0').encode('ascii');require(profile['module']=='MSVCR71.dll' and profile['symbol']==symbol_name and int(profile['moduleNameAddress'],16)==base+name and int(profile['symbolNameAddress'],16)==base+value+2,'import name addresses')
 require(image.bytes_at(base+name,len(module))==module and image.bytes_at(base+value+2,len(symbol))==symbol,'initial imported module/symbol bytes')
 return dict(purgeBytes=0,initialImportIdentityVerified=True,module='MSVCR71.dll',symbol=symbol_name,normalABIRequired=profile['normalABIRequired'],implementationVerified=False,closedAllPathReturnProof=False,limits='Original native thunk andinitial file-backed PE import identity only. Current import binding andexternal normal cdecl/caller-frame behavior arehost conditions;no external DLL implementation,heap/reentry/exception proof.')
