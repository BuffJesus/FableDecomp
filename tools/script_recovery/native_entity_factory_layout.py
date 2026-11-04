"""Initial reference-counted entity factory layout for the supported retail ABI.

Checks the complete native factory template, embedded Thing copy and registered
parent getter. Allocation/current heap/type/lifetime remain caller conditions.
"""
import hashlib
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
TEMPLATE=['push ebx',
 'push esi',
 'push edi',
 'push ALLOCATION',
 'mov ebx, edx',
 'mov edi, ecx',
 'call 0xbfea1a',
 'mov esi, eax',
 'add esp, 4',
 'test esi, esi',
 'je @0x42',
 'mov eax, dword ptr [ebx + 0x40]',
 'mov dword ptr [esi + 4], eax',
 'mov eax, dword ptr [esp + 0x14]',
 'push eax',
 'lea ecx, [esi + 8]',
 'mov dword ptr [esi], 0x12c3224',
 'call 0x4abe90',
 'mov ecx, dword ptr [esp + 0x10]',
 'mov dword ptr [esi], ENTITY_VTABLE',
 'mov dword ptr [esi + 0x14], ebx',
 'mov dword ptr [esi + 0x18], ecx',
 'jmp @0x44',
 'xor esi, esi',
 'test esi, esi',
 'mov dword ptr [edi], esi',
 'je @0x82',
 'push 0xc',
 'call 0xbfea1a',
 'add esp, 4',
 'test eax, eax',
 'je @0x75',
 'mov ecx, dword ptr [edi]',
 'mov dword ptr [eax], 1',
 'mov dword ptr [eax + 4], 0xcdee00',
 'mov dword ptr [eax + 8], ecx',
 'mov dword ptr [edi + 4], eax',
 'mov eax, edi',
 'pop edi',
 'pop esi',
 'pop ebx',
 'ret 8',
 'xor eax, eax',
 'mov dword ptr [edi + 4], eax',
 'mov eax, edi',
 'pop edi',
 'pop esi',
 'pop ebx',
 'ret 8',
 'mov dword ptr [edi + 4], 0',
 'mov eax, edi',
 'pop edi',
 'pop esi',
 'pop ebx',
 'ret 8']
THING_COPY_SHA256='3c497d08782ebf50f8d8813805f1e16fc8c68869028ff7033e4ecbfed3ef1da9'

def prove(image,entity,profile):
 def require(ok,why):
  if not ok:raise ValueError('entity factory layout: '+why)
 start=int(profile['factoryAddress'],16);data=bytes.fromhex(profile['factoryBytes']);allocation=profile['allocationBytes'];require(type(allocation)is int and 28<=allocation<=256 and allocation%4==0,'allocation extent')
 require(len(data)==145 and image.bytes_at(start,len(data))==data,'factory bytes');md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;code=list(md.disasm(data,start));require(sum(i.size for i in code)==145,'complete factory decode');actual=[]
 for i in code:
  text=i.mnemonic+' '+i.op_str
  if i.mnemonic.startswith('j'):text=i.mnemonic+' @'+hex(i.operands[0].imm-start)
  actual.append(text.rstrip())
 expected=[s.replace('ALLOCATION',hex(allocation)).replace('ENTITY_VTABLE',hex(int(entity['vtable'],16))) for s in TEMPLATE];require(actual==expected,'complete initial layout template')
 getter=int(entity['functions']['GetParentScript']['address'],16);require(getter==start+160 and profile['parentGetterAddress']==hex(getter),'registered parent getter link');getter_bytes=bytes.fromhex(profile['parentGetterBytes']);require(getter_bytes==bytes.fromhex('8b4114c3') and image.bytes_at(getter,4)==getter_bytes,'parent getter field')
 copied=bytes.fromhex(profile['thingCopyBytes']);require(len(copied)==33 and hashlib.sha256(copied).hexdigest()==THING_COPY_SHA256 and image.bytes_at(0x4abe90,33)==copied,'embedded Thing copy implementation')
 require(profile['entityThingOffset']==8 and profile['entityThingVtable']=='0x1238c8c' and profile['interfaceOffset']==4 and profile['parentPointerOffset']==20 and profile['parentInterfaceOffset']==64,'registered initial field profile')
 return dict(factory=hex(start),allocationBytes=allocation,checkedInitialBytes=28,embeddedThingOffset=8,embeddedThingVtable='0x1238c8c',interfaceOffset=4,parentPointerOffset=20,parentInterfaceOffset=64,initialLayoutVerified=True,currentTypeAndLifetimeVerified=False,limits='Complete original supported entity factory/Thing-copy/parent-getter template only; initial first28 bytes. Allocation normal cdecl interface and valid source/parent objects are conditions. Trailing state, allocation failure cleanup, current dynamic types/lifetime, heap, reentry and exceptions remain unproved.')
