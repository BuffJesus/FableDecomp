"""Native initial plea, yes/no question and answer speech into structured Lua."""
from tools.script_recovery.teddy_girl_health import prove
SOURCE='''function TeddyGirlSpeak(quest, resources, control, key)
    if TeddyGirlControlledHealthPositive(resources, control) then
        resources:Speak(control, quest:GetHero(), key, 0, false, true, false)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
    end
    return true
end

function TeddyGirlQuestion(quest, resources, control, state, given)
    if not state:GetStateBool("DoneIntro") then
        local hit = state:GetStateBool("HeroHitMe")
        if quest:IsActiveThreadTerminating() then return false end
        local key = hit and "TEXT_QST_048_TEDDYGIRL_PLEA_POST_BEATEN" or "TEXT_QST_048_TEDDYGIRL_LOST_TEDDY"
        if not TeddyGirlSpeak(quest, resources, control, key) then return false end
        state:SetStateBool("DoneIntro", true)
    end
    resources:GiveTeddyGirlQuestion()
    local answer = quest:MsgIsQuestionAnsweredYesOrNo()
    while answer < 0 do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
        answer = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    if quest:IsActiveThreadTerminating() then return false end
    if quest:IsActiveThreadTerminating() then return false end
    if answer == 1 then
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_FOUND_TEDDY") then return false end
        given()
    else
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_REPEAT_PLEA") then return false end
    end
    return true
end
'''

def recover(data=None):
    from tools.script_recovery.lift_native_lua import RData
    data=data or RData();w=prove(data)
    keys=[data.string_at(at) for at in (0x122d70e,0x12c216c,0x12c2188,0x12d88ec)]
    if data.bytes_at(0x122d70e,1)!=b'\0':raise ValueError('TeddyGirl empty question text changed')
    keys[0]=''
    if keys!=['','TEXT_OBJECT_HERO_ANSWER_NO','TEXT_OBJECT_HERO_ANSWER_YES','TEXT_QST_048_GIVE_TEDDY_TO_GIRL']:raise ValueError('TeddyGirl question text changed')
    return SOURCE,{'nativeMainSha256':w['mainSha256'],'start':0xdaf2bf,'ends':[0xdaf60b,0xdaf6d3],
        'cancelDestinations':[0xdaf38a,0xdb04ea,0xdb04fe,0xdb0513],
        'questionConstructionOrder':keys,'questionFlag':True,
        'limits':['Caller owns control, presented output and movie; callback supplies verified GivenTeddy effects.']}
