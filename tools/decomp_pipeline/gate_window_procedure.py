"""Validate the full native window callback, including all embedded switch tables."""
import argparse
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path
import capstone
import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare,retail_relocations
from pe_oracle import EXE
from window_procedure_oracle import ENTRY,TARGETS,cases,execute

ROOT=Path(__file__).resolve().parents[2]
SYMBOL='?WinProcedure@CSystemManager@@CGJPAUHWND__@@IIJ@Z'

def expected_header(results):
    text='// Expected state/trace hashes from retail execution; gate_window_procedure.py verifies every row.\nstatic const Scenario scenarios[]={\n'
    for row in results:text+='    {'+','.join('0x%08xUL'%v for v in [*row['case'],row['hash']])+'},\n'
    return text+'};\n'

def validate_object(obj):
    coff=Coff(obj);sec,start=coff.symbols[SYMBOL];built,relocs=coff.sections[sec]
    assert start==0 and len(built)==1410 and len(relocs)==63
    # Boundaries are established by the final RET and five table-base
    # references, not by disassembling embedded table bytes as instructions.
    candidate_code=1078;retail_code=1052
    with pefile.PE(str(EXE),fast_load=True) as pe:
        retail=pe.get_data(ENTRY-pe.OPTIONAL_HEADER.ImageBase,1382)
        assert pe.get_data(0x009a64df-pe.OPTIONAL_HEADER.ImageBase,4)==struct.pack('<I',ENTRY), 'InitWindow callback xref changed'
    md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);md.detail=True
    candidate_instructions=list(md.disasm(built[:candidate_code],ENTRY))
    native_instructions=list(md.disasm(retail[:retail_code],ENTRY))
    for instructions,size in [(candidate_instructions,candidate_code),(native_instructions,retail_code)]:
        assert sum(i.size for i in instructions)==size and instructions[-1].mnemonic=='ret' and instructions[-1].op_str=='0x10'
    assert built[candidate_code:1080]==b'\x8b\xff' # unreachable MOV EDI,EDI alignment
    score=compare(built[:candidate_code],retail[:retail_code],relocs,retail_relocations('009a5b60',retail_code))
    assert score.near_match,score
    starts={i.address for i in candidate_instructions}
    candidate=bytearray(built);checked=[];table_bases=[];table_targets=[]
    for site,(symbol,kind) in relocs.items():
        addend=struct.unpack_from('<I',built,site)[0]
        if symbol in TARGETS:
            assert site<candidate_code and addend==0
            target=TARGETS[symbol]
            assert kind==(20 if symbol.startswith('?') and '@@3' not in symbol else 6)
        else:
            targetsec,targetoff=coff.symbols[symbol];assert targetsec==sec and kind==6
            target=ENTRY+targetoff
            if site<candidate_code:table_bases.append(targetoff+addend)
            else:
                assert target+addend in starts
                table_targets.append(site)
        value=target+addend-(ENTRY+site+4 if kind==20 else 0)
        struct.pack_into('<I',candidate,site,value&0xffffffff)
        checked.append(dict(offset=site,symbol=symbol,target=f'{target+addend:08x}',kind=kind))
    assert sorted(table_bases)==[1080,1112,1128,1364,1392]
    assert table_targets==list(range(1080,1128,4))+list(range(1364,1392,4))
    # Both compilers emit identical lookup-byte tables, with the same selector
    # order. Every index in those tables is exercised by cases() below.
    assert built[1128:1362]==retail[1100:1334]
    assert built[1392:1410]==retail[1364:1382]
    return built,retail,candidate,score,checked

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source',type=Path,default=ROOT/'rebuild/src/compiled/00/9a/CSystemManager_WinProcedure_009a5b60.cpp')
    ap.add_argument('--test',type=Path,default=ROOT/'rebuild/tests/00/9a/CSystemManager_WinProcedure_009a5b60_test.cpp')
    ap.add_argument('--include',type=Path,default=ROOT/'rebuild/include')
    ap.add_argument('--output',type=Path,default=ROOT/'rebuild/compile-gate/window-procedure.json')
    ap.add_argument('--write-expected',action='store_true',help='write retail-derived fixture data in the supplied include tree')
    args=ap.parse_args();work=Path(tempfile.mkdtemp(prefix='window-procedure-'))
    env=check_one.env();env['INCLUDE']=str(args.include.resolve())+';'+env['INCLUDE']
    obj,test=work/'candidate.obj',work/'test.obj'
    result=check_one.cl(['/nologo','/c','/O2','/Oy','/Oa','/MT','/Fo'+str(obj),str(args.source.resolve())],env,work)
    assert result.returncode==0,result.stdout+result.stderr
    built,retail,candidate,score,checked=validate_object(obj)
    results=[]
    for case in cases():
        native=execute(retail,case);recovered=execute(candidate,case)
        assert native==recovered,dict(case=case,retail_hash=native[0],candidate_hash=recovered[0],retail_events=native[1],candidate_events=recovered[1])
        results.append({'case':case,'hash':native[0],'events':native[1]})
    expected=args.include/'test_support/window_procedure_expected.h'
    text=expected_header(results)
    if args.write_expected:
        expected.parent.mkdir(parents=True,exist_ok=True);expected.write_text(text)
    assert expected.read_text()==text,'retail-derived fixture data mismatch'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(test),str(args.test.resolve())],env,work)
    assert result.returncode==0,result.stdout+result.stderr
    exe=work/'fixture.exe';cmd=[str(check_one.VC/'bin/link.exe'),'/nologo','/out:'+str(exe)]
    standalone=subprocess.run(cmd+[str(test)],env=env,capture_output=True,text=True)
    assert standalone.returncode!=0,'fixture must require candidate'
    linked=subprocess.run(cmd+[str(obj),str(test)],env=env,capture_output=True,text=True)
    assert linked.returncode==0,linked.stdout+linked.stderr
    tested=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
    assert tested.returncode==0 and 'WIN_PROCEDURE_BEHAVIOR_PASS scenarios=3253' in tested.stdout,tested.stdout+tested.stderr
    report=dict(address='009a5b60',grade='NEAR_MATCH',retail_bytes=len(retail),built_bytes=len(built),
                retail_code_bytes=1052,built_code_bytes=1078,similarity=score.to_dict(),relocations=checked,
                source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),
                expected_sha256=hashlib.sha256(expected.read_bytes()).hexdigest(),
                behavior=tested.stdout.strip(),comparative_scenarios=results)
    args.output.parent.mkdir(parents=True,exist_ok=True);args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('WINDOW_PROCEDURE_PASS grade=NEAR_MATCH code=1078 tables=330 padding=2 compiled=3253 comparative=3253')

if __name__=='__main__':main()
