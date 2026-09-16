"""Five native ordinary-talk branches, including state writes with zero health."""
from tools.script_recovery.teddy_girl_health import prove
SOURCE='''function TeddyGirlTalk(quest, me, resources, control, state)
    local found = state:GetStateBool("FoundTeddy")
    if not found and state:GetStateBool("HeroHitMe") then
        if quest:IsActiveThreadTerminating() then return false end
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_PLEA_POST_BEATEN") then return false end
        state:SetStateBool("DoneIntro", true)
    elseif not state:GetStateBool("DoneIntro") then
        if quest:IsActiveThreadTerminating() then return false end
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_LOST_TEDDY") then return false end
        state:SetStateBool("DoneIntro", true)
    elseif found then
        if quest:IsActiveThreadTerminating() then return false end
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_REPEAT_FOUND") then return false end
    elseif quest:GetStateBool("TeddyRuined") then
        if quest:IsActiveThreadTerminating() then return false end
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_BAD_FEELING") then return false end
        quest:ClearThingHasInformation(me)
    else
        if quest:IsActiveThreadTerminating() then return false end
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_REPEAT_PLEA") then return false end
    end
    return true
end
'''

def recover(data=None):
    w=prove(data)
    return SOURCE,{'nativeMainSha256':w['mainSha256'],'start':0xdafd91,'end':0xdb0179,
        'cancelDestinations':[0xdb0590,0xdb05a2],
        'limits':['Caller retains original movie/control; no state persistence or scheduler claim.']}
