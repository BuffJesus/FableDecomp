import copy
import hashlib
import json
import re
import struct
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime

ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())

@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    from tools.script_recovery import native_reviewed_online_score_calls as adapter
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
        with pytest.raises(Done):converter.convert(unit,tmp_path_factory.mktemp('online_score'))
    return result

@pytest.mark.parametrize('fault',['native','source','lua','field','owner','counter','float_local','float_slot',
    'frame','float_assignment','vtable','slot','target','dependency','missing_dependency','window',
    'termination','abort','guard','call','remaining_owner','diagnostics'])
def test_stale_or_ambiguous_evidence_rejected(captured,fault):
    from tools.script_recovery.native_reviewed_online_score_calls import restore
    c=captured;unit=copy.deepcopy(c['unit']);fn=copy.deepcopy(c['fn']);source=c['source'];todo=list(c['todo'])
    row=unit['reviewedOnlineScoreCalls']['0x00e64fb0']
    if fault=='native':row['nativeSha256']='0'*64
    elif fault=='source':fn['decompile']+='\n'
    elif fault=='lua':source+='\n'
    elif fault=='field':unit['master']['fields']['0xf8'][1]='String'
    elif fault=='owner':row['ownerOffset']+=4
    elif fault=='counter':row['valueLocal']='other'
    elif fault=='float_local':row['floatLocal']='other'
    elif fault=='float_slot':row['floatEntrySlot']+=4
    elif fault=='frame':row['frameBytes']+=4
    elif fault=='float_assignment':row['floatAssignment']+=' '
    elif fault=='vtable':row['vtable']+=4
    elif fault=='slot':row['slot']+=4
    elif fault=='target':row['target']+=4
    elif fault=='dependency':row['dependencies'][0]['sha256']='0'*64
    elif fault=='missing_dependency':row['dependencies'].pop()
    elif fault=='window':row['window']['bytes']='90'
    elif fault=='termination':row['terminationTarget']+=4
    elif fault=='abort':row['abortTarget']+=4
    elif fault=='guard':row['guardLines'][1]='if true then'
    elif fault=='call':row['callLines'][1]='quest:UpdateOnlineScore_ChickenKick(other)'
    elif fault=='remaining_owner':
        source+='\nlocal owner=__native_entity_state:GetStateInt("self_0x18")\n'
        row['luaSha256']=hashlib.sha256(source.encode()).hexdigest()
    elif fault=='diagnostics':todo=[]
    with pytest.raises(ValueError):restore(source,todo,fn,unit,c['rd'])

def test_unreviewed_function_unchanged(captured):
    from tools.script_recovery.native_reviewed_online_score_calls import restore
    c=captured;unit=copy.deepcopy(c['unit']);unit.pop('reviewedOnlineScoreCalls')
    assert restore(c['source'],c['todo'],c['fn'],unit,c['rd'])==(c['source'],c['todo'])

def test_only_guard_and_call_changed(captured):
    c=captured;row=c['unit']['reviewedOnlineScoreCalls']['0x00e64fb0']
    assert len(c['todo'])-len(c['remaining'])==3
    before=[s for s in c['source'].splitlines() if s.strip() not in row['guardLines']+row['callLines']]
    after=[s for s in c['draft'].splitlines() if not (s.strip().startswith('if ((') and '== quest:GetMasterGameState' in s) and ':UpdateOnlineScore(' not in s]
    assert before==after
    assert c['draft'].count(':ClearGlobalText(')==1 and c['draft'].count('native_wide_arguments:Close()')==2

@pytest.mark.parametrize('style',['draft','readable'])
def test_actual_emitted_equality_termination_and_float_argument(captured,style):
    lines=[s.strip() for s in captured[style].splitlines()]
    begin=next(i for i,s in enumerate(lines) if s.startswith('if ((') and '== quest:GetMasterGameState("MaxChickenKickingScore")' in s)
    end=next(i for i in range(begin+1,len(lines)) if '= resources:StartMovie(' in lines[i])
    block='\n'.join(lines[begin:end])
    value=re.search(r'^if \(+([A-Za-z_]\w*) \+',lines[begin])[1]
    call=re.search(r'resources:UpdateOnlineScore\((0x[0-9a-f]+|[0-9]+), (\w+)\)',block)
    assert int(call[1],0)==0xb58
    float_local=call[2]
    assert block.count(':IsActiveThreadTerminating()')==1
    lua=LuaRuntime()
    run=lua.eval('''function(score,maximum,stopping)
      local '''+value+'''=score
      local '''+float_local+'''=string.unpack("<f",string.pack("<f",(score+2147483648)%4294967296-2147483648))
      local alive,bVar6
      local trace={};local sent=-1
      local quest={
       GetMasterGameState=function(self,name) assert(name=="MaxChickenKickingScore");trace[#trace+1]="read";return maximum end,
       IsActiveThreadTerminating=function() trace[#trace+1]="term";return stopping end
      }
      local resources={UpdateOnlineScore=function(self,slot,n)
       assert(slot==0xb58);trace[#trace+1]="update";sent=string.unpack("<I4",string.pack("<f",n))
      end}
      '''+block+'''
      do return "normal",table.concat(trace,","),sent end
      ::LAB_00e68ad9::
      return "abort",table.concat(trace,","),sent
    end''')
    values=[0,1,500,-1,-2147483648,2147483647,16777217,4294967295,2147483648]
    for score in values:
        signed=(score+2147483648)%4294967296-2147483648
        float_bits=struct.unpack('<I',struct.pack('<f',signed))[0]
        for maximum in [signed,(signed+1+2147483648)%4294967296-2147483648,(signed-1+2147483648)%4294967296-2147483648]:
            for stopping in [False,True]:
                equal=signed==maximum
                expected=('abort' if equal and stopping else 'normal',
                          'read'+(',term' if equal else '')+(',update' if equal and not stopping else ''),
                          float_bits if equal and not stopping else -1)
                assert run(score,maximum,stopping)==expected
