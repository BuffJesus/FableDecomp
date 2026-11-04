"""Candidate-required native fixtures and retail differential checks for pool access."""
import hashlib
import struct
import subprocess
import tempfile
from pathlib import Path
import check_one as V
from gate_game_component_abi import Coff
from movable_pool_construct_oracle import compile_codes
from movable_pool_policy_oracle import run, POOL, HEADERS, INFOS
from pe_oracle import EXE, read_virtual_data

R=Path(__file__).resolve().parents[2]


def verify(roots, addresses, method, address, pattern):
    root, = roots
    test=R/'rebuild/tests/00/a0'/('CHandleBasedMemoryPool_%s_%08x_test.cpp'%(method,address))
    pins={str(p):hashlib.sha256(p.read_bytes()).hexdigest() for p in (Path(__file__),root['source'],test)}
    with tempfile.TemporaryDirectory(prefix='pool-access-behavior-') as tmp:
        build=Path(tmp)
        candidate=compile_codes(roots,addresses,build)
        env=V.env()
        obj=build/'test.obj'
        result=V.cl(['/nologo','/c','/O2','/MT','/I'+str(R/'rebuild/include'),'/Fo'+str(obj),str(test)],env,build)
        assert result.returncode==0,result.stdout+result.stderr
        assert root['symbol'] not in Coff(obj).symbols, 'fixture defines its own implementation'
        exe=build/'fixture.exe'
        command=[str(V.VC/'bin/link.exe'),'/nologo','/out:'+str(exe),str(obj)]
        result=subprocess.run(command,env=env,capture_output=True,text=True)
        assert result.returncode!=0 and root['symbol'] in result.stdout+result.stderr, 'provider omission must fail'
        result=subprocess.run(command+[str(build/'provider0.obj')],env=env,capture_output=True,text=True)
        assert result.returncode==0,result.stdout+result.stderr
        result=subprocess.run([str(exe)],env=env,capture_output=True,text=True,timeout=30)
        assert result.returncode==0 and pattern in result.stdout,(result.returncode,result.stdout,result.stderr)
        native_fixture=result.stdout.strip()
    raw=EXE.read_bytes()
    native={address:read_virtual_data(raw,address,root['size'])}
    cases=0
    for selected in range(3):
        for lock in (0,1,2,255):
            for count in (0xfffffff6,0,4,0x7fffffff):
                def setup(u,put):
                    u.mem_write(HEADERS,b'\x6b'*0x60)
                    put(POOL+0x438,INFOS)
                    put(POOL+0x18,count)
                    for i in range(3):put(INFOS+4*i,HEADERS+(2-i)*0x20)
                    u.mem_write(HEADERS+(2-selected)*0x20+15,bytes([lock]))
                    if method=='GetSizeFromHandle':put(HEADERS+(2-selected)*0x20+8,count)
                outcomes=[]
                for codes in (candidate,native):
                    trace,eax,_,pool,headers=run(codes,address,POOL,[selected],setup=setup)
                    assert not trace
                    expected_pool=bytearray(b'\xa5'*0x464)
                    struct.pack_into('<I',expected_pool,0x438,INFOS)
                    expected_count=(count-(1 if lock and method=='Unlock' else 0))&0xffffffff
                    struct.pack_into('<I',expected_pool,0x18,expected_count)
                    expected_headers=bytearray(b'\x6b'*0x60)
                    expected_headers[(2-selected)*0x20+15]=0 if method=='Unlock' else lock
                    if method=='GetSizeFromHandle':struct.pack_into('<I',expected_headers,(2-selected)*0x20+8,count)
                    assert pool==bytes(expected_pool) and headers==bytes(expected_headers)
                    if method=='GetPointerFromHandle':assert eax==HEADERS+(2-selected)*0x20+0x20
                    if method=='GetSizeFromHandle':assert eax==count
                    outcomes.append((pool,headers))
                assert outcomes[0]==outcomes[1]
                cases+=1
    assert all(hashlib.sha256(Path(p).read_bytes()).hexdigest()==digest for p,digest in pins.items())
    return dict(status='PASS',comparative_cases=cases,native_fixture=native_fixture,
                missing_provider_checks=1,input_sha256=pins,boundary='No called service substitutes; valid handle tables only.')
