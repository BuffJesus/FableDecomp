"""Losing-teddy complaint flag, control acquisition and health/speech lifetime."""
import json
from tools.script_recovery.victim_talk import recover as proof
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import ROOT,RData
SOURCE='''function VictimComplaint(quest, me, resources, control)
    if not quest:GetStateBool("VictimComplainsAboutLosingTeddy") then return true end
    if quest:IsActiveThreadTerminating() then return false end
    if not VictimAcquire(quest, resources, control, function() return me end) then return false end
    if not VictimSpeak(quest, resources, control, "TEXT_QST_048_VICTIM_EVIL_BROS_10") then return false end
    quest:SetStateBool("VictimComplainsAboutLosingTeddy", false)
    return true
end
'''
def recover(data=None):
    data=data or RData();w=proof(data)[1];window=read_call_window(data,0xdbcd60,4309,0xdbdced,argument_count=1)
    if window is None or window.ecx!=('stack',16) or window.stack_arguments[0]!=('stack',268):raise ValueError('Victim complaint health output changed')
    if data.string_at(0x12d9b84)!='TEXT_QST_048_VICTIM_EVIL_BROS_10':raise ValueError('Victim complaint literal changed')
    return SOURCE,{'mainSha256':w['mainSha256'],'start':0xdbdc58,'end':0xdbdd8d,'cancel':0xdbde18,'parentFlag':150,'healthOutput':268,
        'review':'Flag clears after successful phase including health-suppressed speech; cancellation leaves it set. Caller retains control and original Bully Thing.'}
def generate():
    source,w=recover();out=ROOT/'work/victim_converter';(out/'COMPLAINT_PHASE.lua').write_text('-- Disabled Victim complaint phase.\n'+source);(out/'COMPLAINT_EVIDENCE.json').write_text(json.dumps(w,indent=2)+'\n');return source,w
