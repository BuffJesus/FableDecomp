"""Reviewed temporary-resource lifetime and out-of-hours speech/wait behavior."""
import copy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_evidence_lowering import LoweringSpec
from tools.script_recovery.native_member_resource_calls import resource_call_evidence
from tools.script_recovery.reviewed_resource_operands import reviewed_resource_operands
from tools.script_recovery.test_book_specific_args import generated

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def inputs():
    base = ROOT / 'refs/script_recovery/book_collecting'
    unit = json.loads((base / 'units/V_BookCollecting.json').read_text())
    converter = UnitConverter(base / 'translation_unit_typed.json')
    fn = converter.native('0x00e54e90')
    spec = LoweringSpec(unit, 'BS_Teacher', entity=True, thing_slots={})
    return fn, unit['reviewedResourceOperands']['0x00e54e90'], converter.rdata, resource_call_evidence(converter.rdata, fn, spec)


def test_reviewed_receiver_does_not_weaken_unknown_write_analysis(inputs):
    fn, row, image, inferred = inputs
    assert 0xe557c2 in inferred
    assert all(site not in inferred for site in (0xe55851, 0xe55858, 0xe5587b))
    recovered = reviewed_resource_operands(fn, row, image, inferred)
    assert recovered[0xe55851] == dict(family='self', offset=52, slot=0x34,
                                      pushes=[('hero',0xe55844),('imm',0x12e606c),('imm',2),('imm',0),('imm',1),('imm',0)])
    assert all(recovered[site] == dict(family='self', offset=52, slot=0x68, pushes=[])
               for site in (0xe55858, 0xe5587b))


@pytest.mark.parametrize('fault', ['body','source','anchor','family','lifetime','cleanup','anchor_receiver',
                                  'speech_receiver','table','frame','zero','literal','helper','poll','poll_receiver'])
def test_stale_or_mismatched_lifetime_is_rejected(inputs, fault):
    fn,row,image,inferred=inputs
    fn,row=copy.deepcopy((fn,row));call=row['calls'][0];life=call['receiverLifetime']
    if fault=='body': row['nativeSha256']='0'*64
    if fault=='source': fn['decompile']+='\n'
    if fault=='anchor': call['resourceAnchor']='0x00e557c3'
    if fault=='family': call['family']='parent'
    if fault=='lifetime': life['sha256']='0'*64
    if fault=='cleanup': life['cleanup']['sha256']='0'*64
    if fault=='anchor_receiver': life['anchorReceiver']['bytes']='90'
    if fault=='speech_receiver': life['speechReceiver']['bytes']='90'
    if fault=='table': life['tableLoad']['bytes']='90'
    if fault=='frame': call['vtableStack'][1]['entryEspDelta']+=4
    if fault=='zero': call['flagPushes'][1]['zeroDefinition']['bytes']='90'
    if fault=='literal': call['textPush']['literal']='UNRELATED'
    if fault=='helper': call['helpers'][0]['sha256']='0'*64
    if fault=='poll': call['taskPolls'][1]['dispatch']['bytes']='ff5034'
    if fault=='poll_receiver': call['taskPolls'][0]['receiver']['bytes']='90'
    with pytest.raises(ValueError,match='resource operand evidence'):
        reviewed_resource_operands(fn,row,image,inferred)


def run_wrong_time(source, time, health, abort):
    lua=LuaRuntime()
    lua.execute("require=function() return {} end")
    lua.execute(source)
    lua.globals().hour=time;lua.globals().health=health;lua.globals().abort=abort
    lua.execute('''
        me,hero,quest,resources,member,movie={},{},{},{},{},{}
        local state={BooksDonated=3,GoodBooksDonated=2,GossipState=2,DoneIntro=true,HasInformation=true}
        spoken,polls,frames,created,destroyed=0,0,0,0,0
        local stop={}
        function quest:RetailResources() return resources end
        function quest:GetHero() return hero end
        function quest:ReadGlobalGameData() return 25 end
        function quest:ReadGlobalGameDataFloat() return 0 end
        function quest:GetStateInt(k) return state[k] or 0 end
        function quest:GetStateBool(k) return state[k] or false end
        function quest:SetStateBool(k,v) assert(k=='HasInformation');state[k]=v end
        function quest:SetThingPersistent(v,b) assert(v==me and b) end
        function quest:GetMasterGameState() return false end
        function quest:GetTimeOfDay() return hour end
        function quest:IsActiveThreadTerminating() return abort and frames>0 end
        function quest:NewScriptFrame(v)
            assert(v==me);if spoken>0 then frames=frames+1 end
            assert(frames<4);return not self:IsActiveThreadTerminating()
        end
        function quest:ClearThingHasInformation(v) assert(v==me) end
        function me:IsTalkedToByHero() return true end
        function resources:StartMovie(k) assert(k=='');created=created+1;return movie end
        function quest:PauseAllNonScriptedEntities(v) paused=v end
        function resources:MemberResource(k,v) assert(k=='seh_me' and v==me);return member end
        function resources:PrepareResource(v) assert(v==member) end
        function resources:TryAcquire(v,actor,priority) assert(v==member and actor==me and priority==4);return true end
        function resources:ScriptThing(v) assert(v==member);return me end
        function quest:GetHealth(v) assert(v==me);return health end
        function me:Speak(actor,key,method,a,b,c)
            assert(actor==hero and key=='TEXT_QST_B16_BOOK_WRONG_TIME')
            assert(method==2 and a==false and b==true and c==false);spoken=spoken+1
            return true
        end
        function resources:IsPerformingScriptTask(v) assert(v==member);polls=polls+1;return polls<3 end
        function resources:DestroyMovie(v)
            assert(v==movie and paused==false);destroyed=destroyed+1;error(stop)
        end
        Init(quest,me)
        local ok,err=pcall(Main,quest,me)
        assert(not ok and err==stop,tostring(err))
        assert(created==1 and destroyed==1)
        assert(state.BooksDonated==3 and state.GoodBooksDonated==2 and state.DoneIntro)
        assert(not state.HasInformation)
    ''')
    return tuple(lua.globals()[k] for k in ['spoken','polls','frames'])


@pytest.mark.parametrize('time',[0,844,1616,2359])
@pytest.mark.parametrize('health',[10,0,-1])
@pytest.mark.parametrize('abort',[False,True])
def test_out_of_hours_speech_wait_and_cleanup(generated,time,health,abort):
    expected = ((1,1,1) if abort else (1,3,2)) if health>0 else (0,0,0)
    for _,source in generated:
        assert run_wrong_time(source,time,health,abort)==expected
