"""Repeated-hit conversation and its distinct retained Bully/movie scopes."""
import hashlib,json
from tools.script_recovery.victim_talk import recover as talk_proof
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import ROOT,RData
SOURCE='''function VictimRepeatHit(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    local conversation = resources:NewConversation(me, false, false)
    local bully = resources:NewThingFromScriptName("NOVI_Bully")
    local movie
    local pauseAttempted = false
    local ok, complete = xpcall(function()
        if resources:ThingAlive(bully) then
            if quest:IsActiveThreadTerminating() then return false end
            resources:AddConversationPerson(conversation, bully)
            resources:AddVictimRepeatConversationLines(conversation, me, bully)
        else
            if quest:IsActiveThreadTerminating() then return false end
            movie = resources:NewMovie()
            resources:StartOwnedMovie(movie, "")
            pauseAttempted = true
            quest:PauseAllNonScriptedEntities(true)
            if not VictimAcquire(quest, resources, control, function() return me end) then return false end
            if not VictimSpeak(quest, resources, control, "TEXT_QST_048_VICTIM_EVIL_BROS", 2) then return false end
            resources:PrepareResource(control)
        end
        return true
    end, function(err) return err end)
    local cleanupError
    local function close(callback)
        local closed, err = pcall(callback)
        if not closed and cleanupError == nil then cleanupError = err end
    end
    if pauseAttempted then close(function() quest:PauseAllNonScriptedEntities(false) end) end
    if movie then close(function() resources:DestroyMovie(movie) end) end
    close(function() resources:DestroyThing(bully) end)
    if not ok then error(complete, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return complete
end
'''
def recover(data=None):
    data=data or RData();w=talk_proof(data)[1]
    window=read_call_window(data,0xdbcd60,4309,0xdbdb8b,argument_count=1)
    if window is None or window.ecx!=('stack',16) or window.stack_arguments[0]!=('stack',256):raise ValueError('Victim repeat health output changed')
    for address,value in ((0x12d8338,'NOVI_Bully'),(0x12d9bd0,'TEXT_QST_048_VICTIM_EVIL_BROS'),(0x12d9ba8,'TEXT_QST_048_BULLY_HERO_ATTACKS_VICTIM')):
        if data.string_at(address)!=value:raise ValueError('Victim repeat literal changed')
    if data.bytes_at(0x1238c8c+0x12c,4)!=bytes.fromhex('30b14a00'):raise ValueError('Victim ThingAlive dispatch changed')
    if data.bytes_at(0x4ab130,28)!=bytes.fromhex('8b490485c974128b01ff902c01000084c07406b801000000c333c0c3'):raise ValueError('Victim IsAlive implementation changed')
    return SOURCE,{'mainSha256':w['mainSha256'],'start':0xdbd9d0,'end':0xdbdc58,'cancel':0xdbde18,'retainedBully':56,'movie':80,'healthOutput':256,
        'lines':'Both lines use native speaker=self and listener=the newly retained Bully, despite the second key naming Bully.',
        'limits':['AddVictimRepeatConversationLines is staged/compiled with scoped CString tests; merged owner validation remains pending.',
                  'Conversation API lifetime is engine-managed; no native local conversation destructor is present.',
                  'Caller retains its earlier Bully Thing independently.']}
def generate():
    source,w=recover();out=ROOT/'work/victim_converter';(out/'REPEAT_PHASE.lua').write_text('-- Disabled repeated-hit Victim phase.\n'+source);(out/'REPEAT_EVIDENCE.json').write_text(json.dumps(w,indent=2)+'\n');return source,w
