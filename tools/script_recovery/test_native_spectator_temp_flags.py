import copy,itertools,json,re
from pathlib import Path
from unittest.mock import patch
import pytest
from lupa.lua54 import LuaRuntime
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/script_recovery/script_units.py').is_file())

@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    from tools.script_recovery.convert_quest_unit import UnitConverter
    from tools.script_recovery import native_reviewed_stack_aliases as adapter
    from tools.script_recovery.build_readable_unit import readable_file
    unit=json.loads((ROOT/'refs/script_recovery/chicken_kicking/units/V_ChickenKicking.json').read_text());baseline=copy.deepcopy(unit['reviewedStackScalarAliases']['0x00e63890'])
    # A root run already includes the candidate metadata: retain the established greeting-only baseline.
    baseline['edits']=baseline['edits'][:3];baseline.pop('additionalAliases',None);baseline.pop('editCounts',None)
    unit['entities']={'Spectator':unit['entities']['Spectator']};unit['quest']['functions']={};unit['quest']['helpers']={}
    original=adapter.restore;result={}
    for mode in ['before','after']:
        converter=UnitConverter(ROOT/'refs/script_recovery/chicken_kicking/translation_unit_typed.json')
        def capture(source,fn,row,rd):
            if fn['address'].lower()=='0x00e63890':
                selected=baseline if mode=='before' else row
                out=original(source,fn,selected,rd)
                result.update(fn=fn,rd=rd,source=source,row=row)
                result[mode+'_native']=out
                return out
            return original(source,fn,row,rd)
        out=tmp_path_factory.mktemp('spectator_'+mode)
        with patch.object(adapter,'restore',capture),patch.object(converter,'constructor_defaults',return_value={}):converter.convert(unit,out)
        text=(out/'FSE/V_ChickenKicking/Entities/Spectator.lua').read_text(encoding='utf-8')
        result[mode]=text
    result['draft']=result['after'];result['readable']=readable_file(result['after'])[0]
    return result

@pytest.mark.parametrize('site',[0xe63898,0xe6389f,0xe63e00,0xe63e0a,0xe63e10,0xe63e31,0xe63e37,0xe63e5c,0xe63e63,0xe63e82,0xe63e89,0xe63e97,0xe63e9e,0xe63eac,0xe63eb3])
def test_changed_native_flag_instruction_rejected(captured,site):
    from tools.script_recovery.native_reviewed_stack_aliases import restore
    c=captured;row=copy.deepcopy(c['row']);row['nativeInstructions'][hex(site)]='90'
    with pytest.raises(ValueError,match='native instruction changed'):restore(c['source'],c['fn'],row,c['rd'])

@pytest.mark.parametrize('fault',['native','source','missing_init','missing_snapshot','missing_set','missing_relay','missing_clear','duplicate_relay','alias_collision'])
def test_flag_evidence_or_source_ambiguity_rejected(captured,fault):
    from tools.script_recovery.native_reviewed_stack_aliases import restore
    c=captured;row=copy.deepcopy(c['row']);source=c['source'];fn=copy.deepcopy(c['fn'])
    if fault=='native':row['nativeSha256']='0'*64
    elif fault=='source':fn['decompile']+='\n'
    elif fault=='alias_collision':source+='\nnative_hit_cleanup_flags = 0;'
    elif fault=='duplicate_relay':source+='\n'+row['edits'][6][0]
    else:
        index=dict(missing_init=3,missing_snapshot=4,missing_set=5,missing_relay=6,missing_clear=7)[fault]
        source=source.replace(row['edits'][index][0],'missing = 0;',1)
    with pytest.raises(ValueError):restore(source,fn,row,c['rd'])


def test_only_flag_spellings_are_separated(captured):
    c=captured;row=c['row'];assert row['editCounts'][3:]==[1,1,1,5,1]
    before=c['before_native'];after=c['after_native']
    for old,new in row['edits'][3:]:before=before.replace(old,new)
    assert before==after
    # Every original string constructor/address/conversion remains byte-for-byte present.
    strings=[line for line in c['before_native'].splitlines() if 'xStack_64' in line and ('&xStack_64' in line or 'CCharString::' in line)]
    assert strings and all(after.count(line)==c['before_native'].count(line) for line in strings)


def test_emitted_executable_operations_are_unchanged(captured):
    def clean(text):
        return [line for line in text.splitlines() if not re.fullmatch(r'\s*local [\w, ]+',line) and '-- TODO(native):' not in line]
    c=captured;assert clean(c['before'])==clean(c['after'])
    assert c['before'].count('-- TODO(native):')==2 and '-- TODO(native):' not in c['after']
    assert len(re.findall(r'\bnative_hit_cleanup_flags\b',c['after']))==1 # unused declaration only


def hit_block(text):
    lines=text.splitlines();a=next(i for i,s in enumerate(lines) if 'me:MsgIsHitByHero()' in s)
    b=next(i for i in range(a,len(lines)) if 'quest:SetStateBool("SpectatorsUnderAttack", true)' in lines[i])+2
    block='\n'.join(lines[a:b]);names=sorted(set(re.findall(r'(?m)^\s*(?:local )?(\w+) = ',block)))
    return LuaRuntime(unpack_returned_tuples=True).eval('''function(hit,special,heal,terminating)
      local '''+','.join(names)+'''
      local HERO_ABILITY_HEAL_LIFE_SPELL=14;local trace={};local state=false
      local function message(name,value) trace[#trace+1]=name;return value end
      local me={MsgIsHitByHero=function() return message('hit',hit) end,
       MsgIsHitByAnySpecialAbilityFromHero=function() return message('special',special) end,
       MsgIsHitByHeroSpecialAbility=function(self,ability) assert(ability==14);return message('heal',heal) end}
      local quest={IsActiveThreadTerminating=function() return message('terminating',terminating) end,
       SetStateBool=function(self,name,value) assert(name=='SpectatorsUnderAttack' and value);state=true end}
      local resources={ReleaseResource=function() end}
      local function body()
      '''+block+'''
      ::LAB_00e63f59::
      end
      body();return table.concat(trace,','),state
    end''')

@pytest.mark.parametrize('style',['draft','readable'])
def test_hit_queries_keep_native_short_circuit_and_termination(captured,style):
    run=hit_block(captured[style])
    for hit,special,heal,terminating in itertools.product([False,True],repeat=4):
        reaction=hit or (special and not heal)
        trace=['hit']+([] if hit else ['special']+(['heal'] if special else []))+(['terminating'] if reaction else [])
        assert run(hit,special,heal,terminating)==(','.join(trace),reaction and not terminating)
