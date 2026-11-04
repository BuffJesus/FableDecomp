import itertools,struct,sys
from pathlib import Path
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file());sys.path.insert(0,str(ROOT))
def test_native_no_letter_state_health_speech_and_waits():
 from tools.script_recovery.lift_native_lua import RData
 from tools.script_recovery.convert_quest_unit import callee_stack_words
 from tools.script_recovery.annotate_interface_slots import load_slots
 import unicorn as u
 from unicorn import x86_const as x
 rd=RData();slots={0x118:('GetHero',0),0x420:('GetHealth',1),0x1c:('NewScriptFrame',0)}
 for slot,(name,purge) in slots.items():assert load_slots()[slot]==name and callee_stack_words(int.from_bytes(rd.bytes_at(0x1260f0c+slot,4),'little'))==purge
 cpu=u.Uc(u.UC_ARCH_X86,u.UC_MODE_32)
 for a,n in ((0xec9000,0x3000),(0x7e7000,0x1000),(0xf35000,0x1000),(0x4aa000,0x1000),(0x122d000,0x1000),(0x300000,0x10000),(0x500000,0x10000),(0x200000,0x1000)):cpu.mem_map(a,n)
 cpu.mem_write(0xec9000,rd.bytes_at(0xec9000,0x3000));cpu.mem_write(0x7e7390,rd.bytes_at(0x7e7390,16));cpu.mem_write(0x122dedc,rd.bytes_at(0x122dedc,4));assert rd.bytes_at(0x122dedc,4)==b'\0'*4
 endpoints={0x200100+i*16:slot for i,slot in enumerate(slots)}
 for a in (0x7e7490,0x7e7450,0xf35b30,0x4aa840,*endpoints,0x200140):cpu.mem_write(a,b'\xc3')
 healthTarget=next(a for a,slot in endpoints.items() if slot==0x420)
 # Controlled GetHealth callee returns ST0 through real x87 instructions.
 cpu.mem_write(healthTarget,b'\xdb\xe3\xd9\x05'+struct.pack('<I',0x507100)+b'\xc2\x04\x00')
 def put(a,v):cpu.mem_write(a,struct.pack('<I',v&0xffffffff))
 def word(a):return struct.unpack('<I',cpu.mem_read(a,4))[0]
 state={};layouts=0
 branches={0:(0x100,'TEXT_QST_B10_MANS_LOVER_INTRO_PRELETTER'),1:(0xe8,'TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10'),2:(0xd0,'TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_MAN')}
 def hook(m,a,size,user):
  if a in (0xecae05,0xecae14,0xecaf49,0xeca325):state['exit']=a;m.emu_stop();return
  if a not in (0x7e7490,0x7e7450,0xf35b30,0x4aa840,*endpoints,0x200140):return
  sp=m.reg_read(x.UC_X86_REG_ESP);ecx=m.reg_read(x.UC_X86_REG_ECX);s=state['stack'];out=s+state['offset'];purge=0
  if a==0xf35b30:
   assert ecx==state['owner'];state['terms']+=1;m.reg_write(x.UC_X86_REG_EAX,int(state['termAt']==state['terms']))
  elif a==0x7e7490:
   assert ecx==s+0x1c and word(sp+4)==out;put(out,state['cookie']);m.reg_write(x.UC_X86_REG_EAX,out);state['things']+=1;purge=4
  elif a==healthTarget:
   assert ecx==state['gsi'] and word(sp+4)==out and word(out)==state['cookie'];state['healthCalls']+=1;return
  elif a==0x4aa840:assert ecx==out and word(out)==state['cookie'];state['dtors']+=1
  elif a==0x7e7450:
   assert ecx==s+0x1c;state['tasks']+=1;m.reg_write(x.UC_X86_REG_EAX,int(state['remaining']>0));state['remaining']=max(0,state['remaining']-1)
  elif a==0x200140:
   assert ecx==0x508000 and word(sp+4)==0x509000 and rd.string_at(word(sp+8))==state['line']
   assert [word(sp+i*4) for i in range(3,7)]==[0,0,1,0];state['speeches']+=1;purge=24
  else:
   slot=endpoints[a];assert ecx==state['gsi']
   if slot==0x118:state['heroes']+=1;m.reg_write(x.UC_X86_REG_EAX,0x509000)
   else:assert slot==0x1c;state['frames']+=1
  m.reg_write(x.UC_X86_REG_EIP,word(sp));m.reg_write(x.UC_X86_REG_ESP,sp+4+purge)
 cpu.hook_add(u.UC_HOOK_CODE,hook)
 for value,bits,wait,termAt,owner,quest,gsi,sp,cookie in itertools.product((-1,0,1,2,3,4),(0,0x80000000,0xbf800000,0x3f800000,0x40400000,0x7fc00000),(0,1,3),(0,1,2,4),(0x501000,0x502000),(0x504000,0x505000),(0x500000,0x503000),(0x300800,0x301800),(713,0xdead0000)):
  offset,line=branches.get(value,(0xb8,'TEXT_QST_B10_MANS_LOVER_IN_LOVE_WITH_YOU_10'))
  state=dict(owner=owner,gsi=gsi,stack=sp,cookie=cookie,offset=offset,line=line,termAt=termAt,remaining=wait,terms=0,things=0,dtors=0,healthCalls=0,heroes=0,speeches=0,tasks=0,frames=0,exit=None)
  put(owner+4,gsi);put(owner+0x14,quest);put(gsi,gsi+0x800);put(quest+0x68,0x12345678);put(quest+0x6c,value);put(quest+0x70,0x76543210);put(0x507100,bits)
  for endpoint,slot in endpoints.items():put(gsi+0x800+slot,endpoint)
  put(sp+0x24,0x508000);put(0x508000,0x508800);put(0x508834,0x200140)
  cpu.mem_write(owner+0x1b,b'\x55\0\x66');cpu.reg_write(x.UC_X86_REG_ESI,owner);cpu.reg_write(x.UC_X86_REG_EBP,owner+8);cpu.reg_write(x.UC_X86_REG_ESP,sp)
  try:cpu.emu_start(0xecab59,0xecaf4a,count=2000)
  except u.UcError as e:raise AssertionError((state,hex(cpu.reg_read(x.UC_X86_REG_EIP)),hex(cpu.reg_read(x.UC_X86_REG_ESP)))) from e
  healthy=bits in (0x3f800000,0x40400000);entered=termAt!=1;abortedTask=entered and healthy and termAt in range(2,wait+2)
  assert state['things']==state['dtors']==state['healthCalls']==int(entered) and state['speeches']==state['heroes']==int(entered and healthy)
  expectedExit=0xecaf49 if not entered else 0xeca325 if abortedTask else 0xecae05 if healthy else 0xecae14
  assert state['exit']==expectedExit and state['frames']==(termAt-1 if abortedTask else wait if entered and healthy else 0)
  assert state['tasks']==(termAt-1 if abortedTask else wait+1 if entered and healthy else 0)
  assert word(quest+0x6c)==value&0xffffffff and word(quest+0x68)==0x12345678 and word(quest+0x70)==0x76543210 and cpu.mem_read(owner+0x1b,3)==b'\x55\0\x66'
  assert cpu.reg_read(x.UC_X86_REG_ESP)==sp;layouts+=1
 assert layouts==13824


def test_reviewed_dialogue_state_is_scoped_in_registered_output():
 import json
 from pathlib import Path
 from tools.script_recovery.lift_native_lua import RData
 root=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())
 unit=json.loads((root/'refs/script_recovery/sick_child/units/V_SickChild.json').read_text())
 row=unit['reviewedStackScalarAliases']['0x00ec9d30']
 name='mansLoverDialogueState'
 assert name in row['additionalAliases']
 assert row['edits'][-4:]==[
  ['iVar5 = *(int *)(*(int *)((int)this + 0x14) + 0x6c);',name+' = *(int *)(*(int *)((int)this + 0x14) + 0x6c);'],
  ['          if (iVar5 == 0) {','          if ('+name+' == 0) {'],
  ['else if (iVar5 == 1)','else if ('+name+' == 1)'],
  ['else if (iVar5 == 2)','else if ('+name+' == 2)'],
 ] and row['editCounts'][-4:]==[1]*4
 rd=RData()
 for addr in (0xecab68,0xecab6b,0xecab71,0xecab77,0xecab7e,0xecabd9,0xecac7d,0xecad20,0xecadbf):
  raw=bytes.fromhex(row['nativeInstructions'][hex(addr)])
  assert rd.bytes_at(addr,len(raw))==raw
 for style in ('draft','readable'):
  source=(root/'refs/script_recovery/lifted/SickChild'/style/'FSE/V_SickChild/Entities/MansLover.lua').read_text()
  assert source.count(name+' = quest:GetStateInt("MansLoverState")')==1
  assert all(source.count(name+' == '+str(state))==1 for state in (0,1,2))
  assert 'iVar5 = quest:GetStateInt("MansLoverState")' not in source
