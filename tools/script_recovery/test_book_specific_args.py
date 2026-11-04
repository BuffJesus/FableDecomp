"""Generated book dialogue arguments: native E57020 branches and early exits."""
import subprocess
import sys
import tempfile
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.build_readable_unit import build

ROOT=Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def generated():
    with tempfile.TemporaryDirectory(prefix='book_args_',dir=ROOT/'work') as temp:
        out=Path(temp)
        subprocess.run([sys.executable,'-m','tools.script_recovery.convert_quest_unit',
                        '--unit','book_collecting','--out',str(out/'draft')],
                       cwd=ROOT,check=True,capture_output=True,text=True)
        build('book_collecting',draft=out/'draft',out=out/'readable')
        yield [(name,(out/stage/'FSE/V_BookCollecting/Entities/BS_Teacher.lua').read_text())
               for stage,name in [('draft','helper_E57020'),('readable','GetBookSpecificArgs')]]


CASES=[]
for state,suffix in [(0,'D'),(1250,'D'),(1251,'A'),(1700,'A'),(1701,'B'),
                     (2300,'B'),(2301,'C'),(2600,'C'),(2601,'B')]:
    CASES.append((15,state,False,0,'',{'$ARG1':'TEXT_CS_B16_BOOK15_90'+suffix},0,0))
for value,suffix in [(-1,'A'),(.5,'A'),(.5001,'B')]:
    CASES.append((17,0,False,value,'',{'$ARG1':'TEXT_CS_B16_BOOK17_20'+suffix},1,0))
for state,killed,suffix in [(700,False,'A'),(700,True,'A'),(701,False,'B'),(701,True,'C')]:
    CASES.append((19,state,killed,0,'',{'$ARG1':'TEXT_CS_B16_BOOK19_80'+suffix},0,0))
for teddy in ['', 'A', 'B', 'C']:
    CASES.append((21,0,False,0,teddy,{'$ARG1':'TEXT_CS_B16_BOOK21_20'+teddy,
                                   '$ARG2':'TEXT_CS_B16_BOOK21_30'+teddy,
                                   '$ARG3':'TEXT_CS_B16_BOOK21_40'+('A' if teddy=='A' else 'B')},0,0))
for value,suffix in [(-1,'A'),(.5,'A'),(.5001,'B')]:
    CASES.append((22,0,False,value,'',{'$ARG1':'TEXT_CS_B16_BOOK22_20'+suffix},0,1))
for value,suffix,calls in [(-.5001,'A',1),(-.5,'B',2),(.5,'B',2),(.5001,'C',2)]:
    CASES.append((23,0,False,value,'',{'$ARG1':'TEXT_CS_B16_BOOK23_10'+suffix},calls,0))
for index in [0,14,16,18,20,24]:
    CASES.append((index,0,False,0,'',{},0,0))


def run(source,name,index,state,killed,stat,teddy,terminate=0):
    lua=LuaRuntime(unpack_returned_tuples=True)
    lua.execute('require=function() return {} end')
    lua.execute(source)
    lua.globals().helper=lua.globals()[name]
    return lua.execute('''return function(index,state,killed,stat,teddy,terminate)
        local output,target={},{}
        local attractive,scary,polls=0,0,0
        local resources={SetString=function(_,map,key,value)
            assert(map==target,'argument map identity changed')
            assert(output[key]==nil,'duplicate argument write')
            output[key]=value
        end}
        local q={RetailResources=function() return resources end,
            IsActiveThreadTerminating=function() polls=polls+1;return polls==terminate end,
            GetMasterGameState=function(_,key)
                if key=='PostSavePosition' then return state end
                if key=='BanditCampTwinbladeKilled' then return killed end
                assert(key=='TeddySolution');return teddy
            end,
            GetHeroAttractiveness=function() attractive=attractive+1;return stat end,
            GetHeroScariness=function() scary=scary+1;return stat end}
        helper(q,{},index,target)
        return output,attractive,scary
    end''')(index,state,killed,stat,teddy,terminate)


@pytest.mark.parametrize('case',CASES)
def test_generated_dialogue_branches(generated,case):
    index,state,killed,stat,teddy,expected,attractive,scary=case
    for name,source in generated:
        output,a,s=run(source,name,index,state,killed,stat,teddy)
        assert dict(output)==expected,(name,index)
        assert (a,s)==(attractive,scary),(name,index)


@pytest.mark.parametrize('index',[15,17,19,21,22,23])
def test_termination_before_branch_work(generated,index):
    for name,source in generated:
        output,a,s=run(source,name,index,2301,True,1,'A',terminate=1)
        assert dict(output)=={} and a==s==0,(name,index)
