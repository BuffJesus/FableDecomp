"""Validate the event-dispatch table, its compiler-generated initializer and CRT registration."""
import argparse
import csv
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_display_feedback import TABLE_SYMBOL
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_ESI

ROOT=Path(__file__).resolve().parents[2]
ADDRESS=0x013B9288
INITIALIZER=0x0121B9B0


def execute(code,pattern):
    u=Uc(UC_ARCH_X86,UC_MODE_32)
    for base,size in [(0x121b000,0x3000),(0x13b9000,0x2000),(0x2000000,0x1000),(0x3000000,0x1000)]:u.mem_map(base,size)
    u.mem_write(INITIALIZER,bytes(code));u.mem_write(ADDRESS-16,bytes([pattern])*(5568+32))
    sp=0x2000800;u.mem_write(sp,struct.pack('<I',0x3000000));u.reg_write(UC_X86_REG_ESP,sp);u.reg_write(UC_X86_REG_ESI,0x87654321)
    u.emu_start(INITIALIZER,0x3000000,count=5000)
    assert u.reg_read(UC_X86_REG_ESP)==sp+4 and u.reg_read(UC_X86_REG_ESI)==0x87654321
    assert bytes(u.mem_read(ADDRESS-16,16))==bytes([pattern])*16 and bytes(u.mem_read(ADDRESS+5568,16))==bytes([pattern])*16
    return bytes(u.mem_read(ADDRESS,5568))


def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/01/3b/CMainGameComponent_EventDispatch_013b9288.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/integration/DispatchTable_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--evidence',type=Path,default=ROOT/'ghidra_out/game_event_dispatch_fablewin.tsv')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/dispatch-table.json')
    args=ap.parse_args();env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    work=Path(tempfile.mkdtemp(prefix='dispatch-table-'));objects=[]
    for i,source in enumerate([args.source,args.test]):
        obj=work/('source%d.obj'%i);r=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(obj),str(source.resolve())],env,work)
        assert not r.returncode,r.stdout+r.stderr;objects.append(obj)
    data,fixture=Coff(objects[0]),Coff(objects[1]);assert TABLE_SYMBOL in data.symbols and TABLE_SYMBOL not in fixture.symbols
    assert coff_data(data,TABLE_SYMBOL,5568)==bytes(5568)
    sections={}
    for index in data.sections:
        name=data.data[20+(index-1)*40:28+(index-1)*40].rstrip(b'\0')
        sections[name]=index
    built,rel=data.sections[sections[b'.text$yc']]
    crt,crtRel=data.sections[sections[b'.CRT$XCU']]
    assert len(built)==9567 and len(rel)==1477 and crt==bytes(4) and crtRel=={0:('_$E1',6)}
    evidence=list(csv.DictReader(args.evidence.open(),delimiter='\t'))
    assert len(evidence)==348
    targets={TABLE_SYMBOL:ADDRESS};expected=bytearray(5568);methods=set();fields=['ActionFunction','LocalActionFunction','FeedbackFunction','ConfirmFunction']
    for n,row in enumerate(evidence):
        assert int(row['index'])==n//4 and row['field']==fields[n%4]
        address=int(row['retail_address'],16);name=row['donor_name']
        if address:
            assert n%4<2 and name.startswith('CWorld::') and row['donor_prototype']=='public: void __thiscall '+name+'(class CGameEvent const *)'
            symbol='?'+name.split('::')[1]+'@CWorld@@QAEXPBVCGameEvent@@@Z';assert symbol not in methods
            methods.add(symbol);targets[symbol]=address
        else:assert not name and not row['donor_prototype']
        struct.pack_into('<4I',expected,n*16,address,0,0,0 if address else 0xffffffff)
    assert len(methods)==85 and methods<=set(fixture.symbols)
    patched=bytearray(built)
    for site,(name,kind) in rel.items():
        assert kind==6
        struct.pack_into('<I',patched,site,(targets[name]+struct.unpack_from('<I',built,site)[0])&0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        rva=ADDRESS-pe.OPTIONAL_HEADER.ImageBase;section=next(s for s in pe.sections if s.VirtualAddress<=rva and rva+5568<=s.VirtualAddress+s.Misc_VirtualSize)
        assert section.Name.rstrip(b'\0')==b'.data' and rva>=section.VirtualAddress+section.SizeOfRawData
        native=pe.get_data(INITIALIZER-pe.OPTIONAL_HEADER.ImageBase,len(built));assert patched==native
    for pattern in [0,1,0x5a,0xa5,0xff]:assert execute(patched,pattern)==execute(native,pattern)==expected
    exe=work/'fixture.exe';link=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    for omit in range(2):
        r=subprocess.run(link+[str(o) for i,o in enumerate(objects) if i!=omit],env=env,capture_output=True,text=True)
        assert r.returncode and any(code in r.stdout for code in (['LNK2001','LNK2019'] if omit==0 else ['LNK1561','LNK2001','LNK2019'])),r.stdout+r.stderr
    r=subprocess.run(link+list(map(str,objects)),env=env,capture_output=True,text=True);assert not r.returncode,r.stdout+r.stderr
    r=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert not r.returncode and 'DISPATCH_TABLE_BEHAVIOR_PASS scenarios=22272' in r.stdout,r.stdout+r.stderr
    report=dict(table_address='%08x'%ADDRESS,table_rows=87,data_bytes=5568,initializer_address='%08x'%INITIALIZER,
                initializer_bytes=len(built),initializer_grade='RELOCATION_MATCH',initializer_relocations=len(rel),
                comparative_initializations=5,behavior=r.stdout.strip(),missing_provider_checks=2,
                source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),
                evidence_sha256=hashlib.sha256(args.evidence.read_bytes()).hexdigest(),header_sha256=hashlib.sha256((args.include/'fable_world.h').read_bytes()).hexdigest(),
                coverage_claim='data provider and generated initialization; no new standalone function catalog entry')
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('DISPATCH_TABLE_PASS rows=87 data=5568 initializer=9567 relocations=1477 compiled=22272')


if __name__=='__main__':main()
