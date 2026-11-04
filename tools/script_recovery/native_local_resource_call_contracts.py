"""Static GSI/embedded-Thing call profiles for an initial checked entity layout."""
import hashlib,struct
from capstone import Cs,CS_ARCH_X86,CS_MODE_32,x86 as x
from tools.script_recovery.annotate_interface_slots import VTABLE_BASE
from tools.script_recovery.native_register_snapshot_provenance import analyze
from tools.script_recovery.native_bounded_return_contracts import inspect
from tools.script_recovery.native_entity_factory_layout import prove as prove_layout

def resolve(image,raw,start,row,return_words,*,entity,global_interface_addresses=()):
 def require(ok,why):
  if not ok:raise ValueError('local resource call contracts: '+why)
 layout=prove_layout(image,entity,row['entityLayout']);require(layout['interfaceOffset']==row['cacheProof']['interfaceOffset']==4 and layout['embeddedThingOffset']==8,'initial interface/member fields')
 md=Cs(CS_ARCH_X86,CS_MODE_32);md.detail=True;by={i.address:i for i in md.disasm(raw,start)};states=analyze(raw,start,interface_offset=4,member_offsets=(8,),member_table_loads=(8,),global_interface_addresses=global_interface_addresses)['states'];records=row['callContracts'];require(isinstance(records,list) and records and len({r['site'] for r in records})==len(records),'reviewed call inventory');summaries={};reports=[]
 for record in records:
  a=int(record['site'],16);require(a in by and by[a].mnemonic=='call','native call identity');i=by[a];op=i.operands[0];state=states.get(a,{});require(i.bytes.hex()==record['instructionBytes'],'native call bytes');target=int(record['target'],16)
  if op.type==x.X86_OP_IMM:require(record['family']=='direct' and op.imm==target and record['receiverValueEquivalenceProved'] is False,'direct call target')
  else:
   require(op.type==x.X86_OP_MEM and not op.mem.index and not op.mem.segment,'indirect operand');origin=state.get(i.reg_name(op.mem.base));require(origin is not None,'table bit provenance')
   if record['family']=='GSI_profile':
    require(origin[0]=='table_snapshot','GSI table provenance');table=VTABLE_BASE;equivalent=state.get('ecx')==('interface_snapshot',origin[1])
   else:
    require(record['family']=='constructor_member_profile' and origin[0]=='member_table_snapshot' and origin[1]==8 and state.get('ecx')==('member_address',8),'embedded Thing table/receiver');table=int(layout['embeddedThingVtable'],16);equivalent=True
   require(record['vtable']==hex(table) and record['slot']==hex(op.mem.disp) and image.bytes_at(table+op.mem.disp,4)==struct.pack('<I',target),'static table target');require(record['receiverValueEquivalenceProved'] is equivalent,'receiver equality claim')
  prefix=bytes.fromhex(record['calleePrefix']);require(len(prefix)==16 and image.bytes_at(target,16)==prefix,'callee prefix');bounded=row['boundedReturnRegions'].get(hex(target));proof=None
  if bounded:
   regions=[]
   for part in bounded:
    first,last=int(part['start'],16),int(part['endExclusive'],16);require(first<last and last-first<=65536,'return region extent');data=image.bytes_at(first,last-first);require(data is not None and hashlib.sha256(data).hexdigest()==part['sha256'],'return region bytes');regions.append((first,last))
   proof=inspect(image,target,regions);purge=proof['purgeBytes']
  else:purge=return_words(target)*4
  require(type(record['purgeBytes'])is int and purge==record['purgeBytes'],'normal purge');summaries[a]=dict(purgeBytes=purge,source='bounded_native_RET' if proof else 'reviewed_external_ABI_condition' if hex(target) in row['externalNormalABIs'] else 'nullable_implementation_ABI_condition');reports.append(dict(site=hex(a),target=hex(target),purgeBytes=purge,profile=record['family'],receiverValueEquivalenceProved=record['receiverValueEquivalenceProved'],opaqueChildStackBalanceAssumed=True,currentVtableLifetimeVerified=False))
 targets={r['target'] for r in records};require(set(row['boundedReturnRegions'])==targets-set(row['conditionalNonNullWrappers'])-set(row['externalNormalABIs']),'bounded target inventory');require(set(row['conditionalNonNullWrappers'])<=targets and set(row['externalNormalABIs'])<=targets,'unused ABI profile');require(set(row['conditionalNonNullWrappers']).isdisjoint(row['externalNormalABIs']),'overlapping ABI profiles')
 return summaries,reports,layout
