import copy
import hashlib
import json
import re
from pathlib import Path
from unittest.mock import patch

import pytest
from lupa.lua54 import LuaRuntime

ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())


@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    from tools.script_recovery import native_reviewed_string_copy_calls as adapter
    from tools.script_recovery.convert_quest_unit import UnitConverter
    from tools.script_recovery.build_readable_unit import readable_file
    converter=UnitConverter(ROOT/'refs/script_recovery/chicken_kicking/translation_unit_typed.json')
    unit=json.loads((ROOT/'refs/script_recovery/chicken_kicking/units/V_ChickenKicking.json').read_text())
    result={};original=adapter.restore
    class Done(Exception):pass
    def capture(source,todo,fn,unit,rd):
        generated,remaining=original(source,todo,fn,unit,rd)
        if fn['address'].lower()=='0x00e64fb0':
            result.update(source=source,todo=todo,fn=fn,unit=unit,rd=rd,draft=generated,
                          readable=readable_file(generated)[0],remaining=remaining)
            raise Done()
        return generated,remaining
    with patch.object(converter,'constructor_defaults',return_value={}),patch.object(adapter,'restore',capture):
        with pytest.raises(Done):converter.convert(unit,tmp_path_factory.mktemp('string_copy_calls'))
    return result


@pytest.mark.parametrize('fault',['native','source','lua','dependency','missing_dependency','helper','module',
                                  'frame','slot','parent','owner','copy','duplicate','bytes','cleanup',
                                  'copy_local','nonadjacent','diagnostics'])
def test_ambiguous_or_stale_copy_rejected(captured,fault):
    from tools.script_recovery.native_reviewed_string_copy_calls import restore
    c=captured;unit=copy.deepcopy(c['unit']);fn=copy.deepcopy(c['fn']);source=c['source'];todo=list(c['todo'])
    row=unit['reviewedStringCopyCalls']['0x00e64fb0']
    if fault=='native':row['nativeSha256']='0'*64
    elif fault=='source':fn['decompile']+='\n'
    elif fault=='lua':source+='\n'
    elif fault=='dependency':row['dependencies'][0]['sha256']='0'*64
    elif fault=='missing_dependency':row['dependencies'].pop()
    elif fault=='helper':unit['quest']['functions'].pop(row['helper'])
    elif fault=='module':row['module']='unreviewed.helper'
    elif fault=='frame':row['frameBytes']+=4
    elif fault=='slot':row['entrySlot']+=4
    elif fault=='parent':row['parentOffset']+=4
    elif fault=='owner':row['ownerRegister']='edi'
    elif fault=='copy':row['copyConstructor']+=4
    elif fault=='duplicate':row['calls'].append(copy.deepcopy(row['calls'][0]))
    elif fault=='bytes':row['calls'][0]['bytes']='90'
    elif fault=='cleanup':row['calleeFrameBytes']+=4
    elif fault=='copy_local':row['copyLocal']='other_copy'
    elif fault=='nonadjacent':
        source=source.replace('CCharString::CCharString(&xStack_398_2,&xStack_398);\n',
                              'CCharString::CCharString(&xStack_398_2,&xStack_398);\n    xStack_398 = "changed"\n')
        row['luaSha256']=hashlib.sha256(source.encode()).hexdigest()
    elif fault=='diagnostics':todo=[]
    with pytest.raises(ValueError):restore(source,todo,fn,unit,c['rd'])


def test_unreviewed_function_unchanged(captured):
    from tools.script_recovery.native_reviewed_string_copy_calls import restore
    c=captured;unit=copy.deepcopy(c['unit']);unit.pop('reviewedStringCopyCalls')
    assert restore(c['source'],c['todo'],c['fn'],unit,c['rd'])==(c['source'],c['todo'])


def test_exactly_two_helper_calls_recovered(captured):
    c=captured
    assert len(c['todo'])-len(c['remaining'])==4
    assert c['draft'].count('.helper_E68B20(quest, me,')==9
    assert c['source'].count('.helper_E68B20(quest, me,')==7
    assert 'GetFormattedString: not in FSE manifest' in c['remaining']
    assert 'UpdateOnlineScore_ChickenKick: not in FSE manifest' in c['remaining']


@pytest.mark.parametrize('style',['draft','readable'])
def test_actual_calls_snapshot_selected_string_through_yield(captured,style):
    lines=[line.strip() for line in captured[style].splitlines() if '.helper_E68B20(quest, me,' in line]
    assert len(lines)==9
    lua=LuaRuntime()
    values=['','CS_CHICKING_LANDED','CS_CHICKING_MISSED','CS_CHICKING_FOWL',
            'CS_CHICKING_END','CS_CHICKING_LOWPRIZE','CS_CHICKING_MIDPRIZE','CS_CHICKING_TOPPRIZE',
            'quotation"and\\slash','a'*1024]
    for line in lines[-2:]:
        value=re.search(r'\(quest, me, (\w+)\)$',line)[1]
        run=lua.eval('''function(initial)
 local quest,me={},{};local '''+value+'''=initial;local calls=0;local seen
 local function require(name)
  assert(name=="V_ChickenKicking.native_quest_helpers")
  return {helper_E68B20=function(q,m,arg)
   assert(q==quest and m==me);calls=calls+1;seen=arg
   '''+value+'''="changed during helper"
   coroutine.yield(arg);assert(arg==initial)
  end}
 end
 local helpers=require("V_ChickenKicking.native_quest_helpers")
 local co=coroutine.create(function() '''+line+''' end)
 local ok,first=coroutine.resume(co);assert(ok and first==initial)
 assert(coroutine.status(co)=="suspended")
 assert(coroutine.resume(co));assert(coroutine.status(co)=="dead")
 return seen,calls,'''+value+'''
end''')
        for text in values:
            assert run(text)==(text,1,'changed during helper')
