"""Replace the remaining proximity operand gaps with native ordered operations."""
from tools.script_recovery.bully_initial_phases import recover
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.signed_int32_lua import SOURCE as SIGNED_INT32

SOURCE=SIGNED_INT32+'''
function BullyProximity(quest, resources, me, victim, control, state)
    resources:FaceRetainedThing(me, victim, true)
    if quest:GetTimer(quest:GetStateInt("TalkIntermittentTimer")) ~= 0 then return true end
    if state:GetStateBool("SpokenOnFirstProximity") then
        local roll = quest:RetailRandModulo(resources:ReadBullyRandomModulus())
        if roll ~= 0 then return true end
    end
    local range = resources:ReadBullyProximityRange()
    local hero = quest:GetHero()
    if not resources:IsDistanceBetweenThingsUnder(me, hero, range) then return true end
    if state:GetStateInt("HitsTaken") ~= 0 then return true end
    if quest:IsActiveThreadTerminating() then return false end
    state:SetStateBool("SpokenOnFirstProximity", true)
    quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 3)
    quest:SetStateBool("VictimShake", true)
    local conversation = resources:NewConversation(me, false, false)
    resources:AddConversationPerson(conversation, victim)
    resources:AddConversationLine(conversation, "TEXT_QST_048_BULLY_BADGERING", me, victim, false)
    local nextLine = wrapSignedInt32(state:GetStateInt("IntimidateSpeechLoop") + 10)
    state:SetStateInt("IntimidateSpeechLoop", nextLine)
    if nextLine > 40 then
        if quest:IsActiveThreadTerminating() then
            return false
        end
        state:SetStateInt("IntimidateSpeechLoop", 10)
    end
    local even = quest:RetailRandModulo(2) == 0
    if quest:IsActiveThreadTerminating() then
        return false
    end
    resources:PlayAnimationWithNativeArgument5(control,
        even and "ST_OPINION_DISAPPROVAL_SHAKE_FIST" or "ST_OPINION_DISAPPROVAL_POINT_AT",
        false, false, false, true, false, false)
    return true
end
'''

def lower(source,data=None):
    data=data or RData();recover(data)
    expected={0x12d99f0:'TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_%d',0x128f2ac:'ST_OPINION_DISAPPROVAL_SHAKE_FIST',0x1299988:'ST_OPINION_DISAPPROVAL_POINT_AT'}
    if any(data.string_at(p)!=v for p,v in expected.items()):raise ValueError('Bully proximity literals changed')
    start=source.index('        quest:EntitySetFacingAngleTowardsThing(nil --[[missing]], nil --[[missing]])')
    end=source.index('        alive = quest:NewScriptFrame(me)\n        alive = not quest:IsActiveThreadTerminating()\n    end\n    goto LAB_00dbcce2',start)
    old=source[start:end]
    if old.count('resources:PlayAnimationWithNativeArgument5')!=2:raise ValueError('Bully proximity source correspondence changed')
    source=source[:start]+'''        if not BullyProximity(quest, resources, me, r1, bully_control, __native_entity_state) then
            goto LAB_00dbcce2
        end
'''+source[end:]
    return SOURCE+'\n'+source,{'region':[0xdbc588,0xdbc76f],'facing':[0xdbc595,44,True],
        'timerField':0x104,'timerName':'TalkIntermittentTimer','getTimer':0xdbc5ae,'setTimer':[0xdbc62c,3],
        'dynamicModulus':0x13ac860,'dynamicRange':0x13ac85c,'distance':0xcbe2ff,
        'textCtor':0xdbc660,'textFormat':0xdbc673,'textDestructor':[0xdbc76a,0xdbccdd],
        'limits':'Text retained through counter rollover query and animation; error fallback is owned-resource close policy. Dynamic tuning globals are reloaded, not frozen.'}
