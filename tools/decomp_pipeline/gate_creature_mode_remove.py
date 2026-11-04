"""Verify creature-mode removal with original virtual ABI, list boundary and retail conversion routine."""
from pathlib import Path
import argparse,hashlib,json,struct,subprocess,tempfile,sys
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'rebuild/include').is_dir());sys.path.insert(0,str(ROOT/'tools/decomp_pipeline'))
import check_one,pefile,verify_and_land as V
from pe_oracle import EXE
from gate_game_component_abi import Coff
from gate_inventory_events import EXTRA as PLAYER,SPECS as INVENTORY
from gate_player_lookup import SPECS as LOOKUP
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import *
SPECS=[dict(address='0073aba0',file='CTCCreatureModeManager_RemoveMode_0073aba0.cpp',symbol='?RemoveMode@CTCCreatureModeManager@@QAEXK@Z',size=170,grade='RELOCATION_MATCH')]
EXTRA=[INVENTORY[-1],dict(address='0049d870',file='CWorld_GetFrame_0049d870.cpp',symbol='?GetFrame@CWorld@@SIJXZ',size=6)];TARGETS={s['symbol']:int(s['address'],16) for s in SPECS+EXTRA};TARGETS.update({'?UpdateTurningAnims@CTCLook@@QAEXXZ':0x754050,'?ConstantFPS@CMainGameComponent@@0JA':0x1375550,'?Frame@CWorld@@1JA':0x13b89bc,'__real@41f00000':0x12673c4,'__ftol2':0xbfea70})
REMOVE_IF='??$remove_if@VCIsType@CModeBase@CTCCreatureModeManager@@@?$list@V?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@V?$allocator@V?$CCountedPointer@VCModeBase@CTCCreatureModeManager@@@@@_STL@@@_STL@@QAEXVCIsType@CModeBase@CTCCreatureModeManager@@@Z';TARGETS[REMOVE_IF]=0x73e790
HEADERS=['fable_thing_components.h','fable_stlport_list.h'];MAPS=[[],[67],[-0x80000000,-1,17,67,67,68,0x7fffffff],[66,68],[0,17,49,54,273]];LISTS=[[],[12],[12,1],[1,12,12],[12,12,12],[0x80000000,12,0x7fffffff],[0x7fffffff,0x80000000,0xffffffff],[1,0,12]];QUERIES=[0,1,12,0xffffffff,0x80000000,0x7fffffff]

def execute(codes,listValues,query,keys,has,fps,frame,effect,pattern):
    manager,parent,parent2,table,nodes,modes,vtable,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x2500040,0x2600040,0x3008000,0x4000000;virtualType,activate,deactivate,removeIf,turning=0x4100000,0x4100010,0x4100020,0x73e790,0x754050;u=Uc(UC_ARCH_X86,UC_MODE_32);bases=[0x2000000+i*0x100000 for i in range(7)]
    for page in {a&~0xfff for a in codes}|{a&~0xfff for a in [virtualType,activate,deactivate,removeIf,turning]}|{0x1375000,0x13b8000,0x1267000,stop}|set(bases):u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,code in codes.items():u.mem_write(a,bytes(code))
    regions={a:bytearray(bytes([pattern*71])*0x300) for a in bases};struct.pack_into('<I',regions[0x2000000],0x44,parent);struct.pack_into('<I',regions[0x2000000],0x4c,nodes);n=len(listValues)
    for i in range(n+1):struct.pack_into('<IIII',regions[0x2400000],0x40+i*16,nodes+(i+1)*16 if i<n else nodes,nodes+n*16 if i==0 else nodes+(i-1)*16,modes+(i-1)*32 if i else 0xdeadbeef,0)
    values=list(listValues)+[0x55555555]*(3-n)
    for i,value in enumerate(values):struct.pack_into('<III',regions[0x2500000],0x40+i*32,vtable,value&0xffffffff,i)
    for i in range(12):struct.pack_into('<I',regions[0x2600000],0x40+i*4,{1:virtualType,5:activate,6:deactivate}.get(i,stop))
    for base,present in [(0x2100000,has),(0x2200000,not has)]:struct.pack_into('<I',regions[base],0x68,(0xfffffff7 if pattern else 0)|(8 if present else 0));struct.pack_into('<III',regions[base],0x84,table,table+len(keys)*8,table+(len(keys)+1)*8)
    for i,key in enumerate(keys+[0x7fffffff]):struct.pack_into('<II',regions[0x2300000],0x40+i*8,key&0xffffffff,0x2700040+i*32)
    for a,data in regions.items():u.mem_write(a,bytes(data))
    u.mem_write(0x1375550,struct.pack('<I',fps&0xffffffff));u.mem_write(0x13b89bc,struct.pack('<I',frame&0xffffffff));u.mem_write(0x12673c4,struct.pack('<f',30.0));u.mem_write(stack,struct.pack('<II',stop,query&0xffffffff));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,manager);u.reg_write(UC_X86_REG_FPCW,0x037f);u.reg_write(UC_X86_REG_FPTAG,0xffff);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[];finalFPS=fps;finalFrame=frame&0xffffffff
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def putNode(a,value):u.mem_write(a,struct.pack('<I',value));struct.pack_into('<I',regions[0x2400000],a-0x2400000,value)
    def hook(uc,pc,size,user):
        nonlocal finalFPS,finalFrame
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
        if pc==0xbfea70:trace.append((6,0,0));return
        if pc==0x49d870:trace.append((7,0,0));return
        if pc not in [virtualType,activate,deactivate,removeIf,turning]:return
        result=0xabababab;pop=0
        if pc in [virtualType,activate,deactivate]:
            assert (this-modes)%32==0 and 0<=this-modes<96;idx=(this-modes)//32;trace.append(({virtualType:1,activate:4,deactivate:2}[pc],idx,0))
            if pc==virtualType:result=get(this+4)
            if pc==deactivate and effect==1:values[idx]^=0x80000000;struct.pack_into('<I',regions[0x2500000],0x44+idx*32,values[idx]);uc.mem_write(this+4,struct.pack('<I',values[idx]))
            if pc==activate and effect==2:struct.pack_into('<I',regions[0x2000000],0x44,parent2);uc.mem_write(manager+4,struct.pack('<I',parent2))
        elif pc==removeIf:
            assert this==manager+12;value=get(sp+4);assert value==query&0xffffffff;pop=4;trace.append((3,0,value));node=get(nodes)
            while node!=nodes:
                nextNode=get(node);mode=get(node+8);idx=(mode-modes)//32;trace.append((1,idx,0))
                if get(mode+4)==value:prev=get(node+4);putNode(prev,nextNode);putNode(nextNode+4,prev)
                node=nextNode
        else:
            idx=next((i for i,key in enumerate(keys) if key==67),len(keys));assert this==0x2700040+idx*32;trace.append((5,0,0))
            if effect==3:finalFPS+=1;finalFrame=(finalFrame+17)&0xffffffff;uc.mem_write(0x1375550,struct.pack('<I',finalFPS&0xffffffff));uc.mem_write(0x13b89bc,struct.pack('<I',finalFrame))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4+pop)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x73aba0,stop,count=1000);expected=[];current=n>0 and listValues[0]==query
    if n:expected.append((1,0,0))
    if current:expected.append((2,0,0))
    expected.append((3,0,query&0xffffffff));live=[]
    for i in range(n):expected.append((1,i,0));live +=[i] if values[i]!=query else []
    if current and live:
        expected.append((4,live[0],0))
        if (not has if effect==2 else has):expected.append((5,0,0))
    expected +=[(6,0,0),(7,0,0)];assert trace==expected,(trace,expected);deadline=(finalFrame+finalFPS*30)&0xffffffff;struct.pack_into('<I',regions[0x2000000],0x54,deadline);assert get(manager+0x14)==deadline;assert get(0x1375550)==finalFPS&0xffffffff and get(0x13b89bc)==finalFrame;assert all(bytes(u.mem_read(a,len(data)))==data for a,data in regions.items());assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));assert u.reg_read(UC_X86_REG_FPCW)==0x037f and not(u.reg_read(UC_X86_REG_FPSW)&0x3800);return trace

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CreatureModeRemove_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/creature-mode-remove.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='creature-mode-remove-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+EXTRA):
            source=a.stage/s['file'] if a.stage and i<1 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0;address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size'])
            assert patched==retail,s
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
        candidate[0xbfea70]=native[0xbfea70]=pe.get_data(0xbfea70-0x400000,117)
    count=0
    for idx,values in enumerate(LISTS):
        for q,query in enumerate(QUERIES):
            for keys in MAPS:
                for has in [False,True]:
                    for fps in [15,-1]:
                        for effect in range(4):
                            for pattern in range(2):frame=[0,1,-1,-0x80000000,0x7fffffff][(idx+q+pattern)%5];assert execute(candidate,values,query,keys,has,fps,frame,effect,pattern)==execute(native,values,query,keys,has,fps,frame,effect,pattern);count+=1
    test=work/'test.obj';r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(a.test.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS+EXTRA);objects.append(test);exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(3):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and 'CREATURE_MODE_REMOVE_BEHAVIOR_PASS scenarios=134400' in r.stdout,r.stdout+r.stderr
    report=dict(providers=providers,comparative_scenarios=count,compiled_scenarios=134400,missing_provider_checks=3,retail_ftol2_length=117,remove_if_boundary=True,behavior=r.stdout.strip(),fixture_sha256=hashlib.sha256(a.test.read_bytes()).hexdigest(),headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CREATURE_MODE_REMOVE_PASS compiled=134400 comparative=%d'%count)
if __name__=='__main__':main()
