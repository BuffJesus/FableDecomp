"""Killed helper with explicit, still-pending native by-value Thing adapter."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT,RData

BODY='''    quest:WithCopiedThreadThing(troll, function(actor)
        actor:RegisterParentAliveCondition()
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
        while not actor:MsgIsKilledBy("") do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:RemoveQuestInfoElement(quest:GetStateInt("RockTrollHealthBarID"))
        quest:DisplayQuestInfo(false)
        quest:SetStateBool("MissionOver", true)
    end)
'''


def recover(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('rock_killed_witness.json').read_text())
    for region in w['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:raise ValueError('Rock killed native operand changed')
    for source in w['sources']:
        if hashlib.sha256((ROOT/source['path']).read_bytes()).hexdigest()!=source['sha256']:raise ValueError('Rock killed source changed')
    if data.bytes_at(0x122d70e,1)!=b'\0':raise ValueError('Killed filter changed')
    if data.string_at(0x12f17c4)!='WatchForRockTrollKilled':raise ValueError('Killed helper name changed')
    return BODY,w
