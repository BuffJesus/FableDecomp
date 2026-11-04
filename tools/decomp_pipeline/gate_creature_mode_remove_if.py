"""Verify real mode-list lifetime cleanup and composition with creature-mode removal."""
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
from gate_creature_mode_remove import SPECS as MODE,EXTRA as MODE_EXTRA,TARGETS as MODE_TARGETS,REMOVE_IF,LISTS,QUERIES,MAPS
SPECS=[dict(address='0073e790',file='CreatureModeList_remove_if_0073e790.cpp',symbol=REMOVE_IF,size=104,grade='RELOCATION_MATCH')];EXTRA=MODE+MODE_EXTRA;TARGETS=dict(MODE_TARGETS);TARGETS.update({REMOVE_IF:0x73e790,'_free':0xbfea14,'??3@YAXPAX@Z':0xbfe9bc});HEADERS=['fable_stlport_list.h','fable_stlport_list_remove_if_impl.h']

def lifetime(codes,values,query,ownership,effect,pattern):
    target,nodes,modes,infos,vtable,stack,stop=0x2000040,0x2100040,0x2200040,0x2300040,0x2400040,0x3008000,0x4000000;getType,deleteData,deleteInfo,freeNode=0x4100000,0x4100010,0xbfe9bc,0xbfea14;u=Uc(UC_ARCH_X86,UC_MODE_32);bases=[0x2000000+i*0x100000 for i in range(5)]
    for page in {a&~0xfff for a in codes}|{a&~0xfff for a in [getType,deleteData,deleteInfo,freeNode]}|{stop}|set(bases):u.mem_map(page,0x1000)
    u.mem_map(0x3000000,0x10000)
    for a,code in codes.items():u.mem_write(a,bytes(code))
    regions={a:bytearray(bytes([pattern*71])*0x300) for a in bases};expected={a:bytearray(data) for a,data in regions.items()};n=len(values);count=0 if ownership==0 else (1 if ownership==5 and n else n);refs=[(1 if ownership==1 else 2 if ownership==2 else 0xffffffff if ownership==3 else 0 if ownership==4 else n) for i in range(count)];types=list(values)+[0x55555555]*(3-n);struct.pack_into('<I',regions[0x2000000],0x40,nodes)
    for i in range(n+1):struct.pack_into('<IIII',regions[0x2100000],0x40+i*16,nodes+(i+1)*16 if i<n else nodes,nodes+n*16 if not i else nodes+(i-1)*16,modes+(i-1)*32 if i else 0xdeadbeef,(infos+(0 if ownership==5 else i-1)*32) if i and count else 0)
    for i,value in enumerate(types):struct.pack_into('<III',regions[0x2200000],0x40+i*32,vtable,value&0xffffffff,i)
    for i,ref in enumerate(refs):struct.pack_into('<III',regions[0x2300000],0x40+i*32,ref,deleteData,modes+(0 if ownership==5 else i)*32)
    for i in range(12):struct.pack_into('<I',regions[0x2400000],0x40+i*4,getType if i==1 else stop)
    expected={a:bytearray(data) for a,data in regions.items()}
    for a,data in regions.items():u.mem_write(a,bytes(data))
    u.mem_write(stack,struct.pack('<II',stop,query&0xffffffff));u.reg_write(UC_X86_REG_ESP,stack);u.reg_write(UC_X86_REG_ECX,target);saved=[UC_X86_REG_EBX,UC_X86_REG_ESI,UC_X86_REG_EDI,UC_X86_REG_EBP]
    for i,r in enumerate(saved):u.reg_write(r,0x56780000+i)
    trace=[];lastVisited=-1
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def hook(uc,pc,size,user):
        nonlocal lastVisited
        if pc not in [getType,deleteData,deleteInfo,freeNode]:return
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX);result=0xabababab
        if pc==getType:
            idx=(this-modes)//32;assert 0<=idx<n and (this-modes)%32==0;lastVisited=idx;trace.append(('type',idx));result=get(this+4)
        elif pc==deleteData:
            idx=(this-modes)//32;assert 0<=idx<n and (this-modes)%32==0;trace.append(('delete',idx));current=nodes+(lastVisited+1)*16;assert get(get(current+12))==0
            if effect:
                nextNode=get(current)
                if nextNode!=nodes:mode=get(nextNode+8);u.mem_write(mode+4,struct.pack('<I',query&0xffffffff))
                u.mem_write(current,struct.pack('<I',nodes))
        elif pc==deleteInfo:
            pointer=get(sp+4);idx=(pointer-infos)//32;assert 0<=idx<count and (pointer-infos)%32==0 and get(pointer)==0;trace.append(('info_free',idx))
        else:
            pointer=get(sp+4);idx=(pointer-nodes)//16-1;assert 0<=idx<n and (pointer-nodes)%16==0 and get(pointer+8)==0 and get(pointer+12)==0;trace.append(('node_free',idx))
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4)
    u.hook_add(UC_HOOK_CODE,hook);u.emu_start(0x73e790,stop,count=1000);wanted=[];nexts=[i+1 if i<n else 0 for i in range(n+1)];prevs=[n if i==0 else i-1 for i in range(n+1)];dataFields=[0xdeadbeef]+[modes+i*32 for i in range(n)];infoFields=[0]+[infos+(0 if ownership==5 else i)*32 if count else 0 for i in range(n)]
    for i in range(n):
        wanted.append(('type',i))
        if types[i]!=query:continue
        index=i+1;nextIndex=nexts[index];prevIndex=prevs[index];nexts[prevIndex]=nextIndex;prevs[nextIndex]=prevIndex
        if count:
            group=0 if ownership==5 else i;refs[group]=(refs[group]-1)&0xffffffff
            if not refs[group]:
                wanted +=[('delete',0 if ownership==5 else i),('info_free',group)]
                if effect:
                    if nextIndex:types[nextIndex-1]=query
                    nexts[index]=0
        wanted.append(('node_free',i));dataFields[index]=infoFields[index]=0
    for i in range(n+1):struct.pack_into('<IIII',expected[0x2100000],0x40+i*16,nodes+nexts[i]*16,nodes+prevs[i]*16,dataFields[i],infoFields[i])
    for i,value in enumerate(types):struct.pack_into('<I',expected[0x2200000],0x44+i*32,value&0xffffffff)
    for i,ref in enumerate(refs):struct.pack_into('<I',expected[0x2300000],0x40+i*32,ref)
    assert trace==wanted,(trace,wanted);assert all(bytes(u.mem_read(a,len(data)))==data for a,data in expected.items());assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));return trace

def composed(codes,listValues,query,keys,has,fps,frame,effect,pattern):
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
    trace=[];finalFPS=fps;finalFrame=frame&0xffffffff;freed=0
    def get(a):return struct.unpack('<I',u.mem_read(a,4))[0]
    def putNode(a,value):u.mem_write(a,struct.pack('<I',value));struct.pack_into('<I',regions[0x2400000],a-0x2400000,value)
    def hook(uc,pc,size,user):
        nonlocal finalFPS,finalFrame,freed
        sp=uc.reg_read(UC_X86_REG_ESP);this=uc.reg_read(UC_X86_REG_ECX)
        if pc==0xbfea70:trace.append((6,0,0));return
        if pc==0x49d870:trace.append((7,0,0));return
        if pc==removeIf:assert this==manager+12;trace.append((3,0,get(sp+4)));return
        if pc==0xbfea14:
            pointer=get(sp+4);assert nodes<pointer<=nodes+n*16 and (pointer-nodes)%16==0 and get(pointer+8)==0 and get(pointer+12)==0
            def expectedPut(a,value):struct.pack_into('<I',regions[0x2400000],a-0x2400000,value)
            offset=pointer-0x2400000;nextNode,prevNode=struct.unpack_from('<II',regions[0x2400000],offset);expectedPut(prevNode,nextNode);expectedPut(nextNode+4,prevNode);expectedPut(pointer+8,0);expectedPut(pointer+12,0);freed+=1
            uc.reg_write(UC_X86_REG_EAX,0xabababab);uc.reg_write(UC_X86_REG_ECX,0xcdcdcdcd);uc.reg_write(UC_X86_REG_EDX,0xefefefef);uc.reg_write(UC_X86_REG_EIP,get(sp));uc.reg_write(UC_X86_REG_ESP,sp+4);return
        if pc not in [virtualType,activate,deactivate,turning]:return
        result=0xabababab;pop=0
        if pc in [virtualType,activate,deactivate]:
            assert (this-modes)%32==0 and 0<=this-modes<96;idx=(this-modes)//32;trace.append(({virtualType:1,activate:4,deactivate:2}[pc],idx,0))
            if pc==virtualType:result=get(this+4)
            if pc==deactivate and effect==1:values[idx]^=0x80000000;struct.pack_into('<I',regions[0x2500000],0x44+idx*32,values[idx]);uc.mem_write(this+4,struct.pack('<I',values[idx]))
            if pc==activate and effect==2:struct.pack_into('<I',regions[0x2000000],0x44,parent2);uc.mem_write(manager+4,struct.pack('<I',parent2))
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
    expected +=[(6,0,0),(7,0,0)];assert trace==expected,(trace,expected);assert freed==n-len(live);deadline=(finalFrame+finalFPS*30)&0xffffffff;struct.pack_into('<I',regions[0x2000000],0x54,deadline);assert get(manager+0x14)==deadline;assert get(0x1375550)==finalFPS&0xffffffff and get(0x13b89bc)==finalFrame;assert all(bytes(u.mem_read(a,len(data)))==data for a,data in regions.items());assert u.reg_read(UC_X86_REG_ESP)==stack+8 and all(u.reg_read(r)==0x56780000+i for i,r in enumerate(saved));assert u.reg_read(UC_X86_REG_FPCW)==0x037f and not(u.reg_read(UC_X86_REG_FPSW)&0x3800);return trace

def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('--stage',type=Path);ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include');ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/CreatureModeListLifetime_test.cpp');ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/creature-mode-remove-if.json');a=ap.parse_args();env=check_one.env();env['INCLUDE']=str(a.include.resolve())+';'+env['INCLUDE'];work=Path(tempfile.mkdtemp(prefix='creature-mode-remove-if-gate-'));objects=[];providers=[];candidate={};native={}
    with pefile.PE(str(EXE)) as pe:
        for i,s in enumerate(SPECS+EXTRA):
            source=a.stage/s['file'] if a.stage and i<1 else ROOT/'rebuild/src/compiled'/s['address'][:2]/s['address'][2:4]/s['file'];obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;objects.append(obj);c=Coff(obj);sec,off=c.symbols[s['symbol']];built,rel=c.sections[sec];assert off==0;address=int(s['address'],16);patched=bytearray(built)
            for site,(name,kind) in rel.items():assert kind in [6,20];struct.pack_into('<I',patched,site,(TARGETS[name]+struct.unpack_from('<I',built,site)[0]-(address+site+4 if kind==20 else 0))&0xffffffff)
            retail=pe.get_data(address-0x400000,s['size'])
            assert patched==retail,s
            candidate[address]=patched;native[address]=retail;providers.append(dict(source=str(source),sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
        candidate[0xbfea70]=native[0xbfea70]=pe.get_data(0xbfea70-0x400000,117)
    count=0;composedCount=0
    for values in LISTS:
        for query in QUERIES:
            for ownership in range(6):
                for effect in range(2):
                    for pattern in range(4):assert lifetime(candidate,values,query,ownership,effect,pattern)==lifetime(native,values,query,ownership,effect,pattern);count+=1
    for idx,values in enumerate(LISTS):
        for q,query in enumerate(QUERIES):
            for keys in MAPS:
                for has in [False,True]:
                    for fps in [15,-1]:
                        for effect in range(4):
                            for pattern in range(2):frame=[0,1,-1,-0x80000000,0x7fffffff][(idx+q+pattern)%5];assert composed(candidate,values,query,keys,has,fps,frame,effect,pattern)==composed(native,values,query,keys,has,fps,frame,effect,pattern);composedCount+=1
    compiled=[]
    for testPath,pattern,countExpected,required in [(a.test,'CREATURE_MODE_LIST_LIFETIME_PASS',2304,1),(a.test.with_name('CreatureModeListComposed_test.cpp'),'CREATURE_MODE_LIST_COMPOSED_PASS',134400,4)]:
        test=work/(testPath.stem+'.obj');r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(testPath.resolve())],env,work);assert not r.returncode,r.stdout+r.stderr;assert all(s['symbol'] not in Coff(test).symbols for s in SPECS+EXTRA);selected=objects[:1]+[test] if required==1 else objects+[test];exe=work/(testPath.stem+'.exe');link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
        for omit in range(required):
            r=subprocess.run(link+[str(obj) for i,obj in enumerate(selected) if i!=omit],env=env,capture_output=True,text=True);assert r.returncode and ('LNK2001' in r.stdout or 'LNK2019' in r.stdout)
        r=subprocess.run(link+list(map(str,selected)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr;r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30);assert not r.returncode and pattern+' scenarios='+str(countExpected) in r.stdout,r.stdout+r.stderr;compiled.append(dict(path=str(testPath),sha256=hashlib.sha256(testPath.read_bytes()).hexdigest(),scenarios=countExpected,omission_checks=required,behavior=r.stdout.strip()))
    report=dict(providers=providers,comparative_scenarios=count,composed_comparative_scenarios=composedCount,compiled=compiled,retail_ftol2_length=117,headers={h:hashlib.sha256((a.include/h).read_bytes()).hexdigest() for h in HEADERS});a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print('CREATURE_MODE_REMOVE_IF_PASS lifetime=2304 composed=134400 native=%d+%d'%(count,composedCount))
if __name__=='__main__':main()
