-- Readable native conversion: AppleGirl. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- AppleGirl.Main (retail 0x00d3d150)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult6, predicateResult7, predicateResult10, ctr_64, scratchValue7
    local conversationId, conversationId2, conversationId3, conversationId4, conversationId5
    local conversationId6, questionAnswer, conversationId7, switch2, u_stk_74_1, scratchValue11
    local scratchValue12, scratchValue13
    u_stk_74_1 = 0
    scratchValue12 = resources:NewResource()
    while not resources:TryAcquire(scratchValue12, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue12); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(scratchValue12); return end
    me:SetFriendsWithEverythingFlag(me)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntityAttachToVillage(me, quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE"))
    state:SetInt("AppleMode", 0)
    scratchValue7 = quest:RegisterTimer()
    scratchValue13 = scratchValue7
    quest:SetTimer(scratchValue7, 15)
    state:SetInt("CurrentApples", 0)
    state:SetBool("ChildAppleMode", false)
    quest:SetThingHasInformation(me, false, true, false)
    while state:GetInt("AppleMode") == 0 do
        if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
        if quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(scratchValue7) < 1 then
            scratchValue7 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(scratchValue7, quest:GetHero())
            quest:AddLineToConversation(scratchValue7, "TEXT_QST_028_APPLEGIRL_HELP", me, quest:GetHero(), false)
            quest:SetTimer(scratchValue13, 15)
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            scratchValue11 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            if not state:GetBool("HaveChatted") then
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e06c end
                state:SetBool("HaveChatted", true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue12)) then
                    scratchValue7 = 0
                    me:Speak(quest:GetHero(), "TEXT_QST_028_APPLEGIRL_CHAT", 0, false, true, false)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d3d948 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e06c end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPLEGIRL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPLEGIRL_QUESTION_AGAIN", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            end
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer < 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue11)
                    quest:DeregisterTimer(scratchValue7)
                    return
                end
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
            if questionAnswer == 1 then
                state:SetInt("AppleMode", 1)
                if quest:IsQuestActive("Q_GuildTrainingPreMelee") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
                    state:SetBool("ChildAppleMode", true)
                end
            elseif 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue12)) then
                scratchValue7 = 0
                me:Speak(quest:GetHero(), "TEXT_QST_028_APPLEGIRL_IMPLORE", 0, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d3d948 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e06c end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue11)
        end
    end
    if not quest:IsActiveThreadTerminating() then
        while state:GetInt("AppleMode") == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
            if state:GetBool("ChildAppleMode") then
                u_stk_74_1 = u_stk_74_1 | 1
                if quest:IsQuestActive("Q_GuildTrainingPreMelee") then
                    predicateResult6 = false
                    goto FLOW_after_lab_00d3d987
                end
                predicateResult6 = true
            else
                predicateResult6 = false
            end
            ::FLOW_after_lab_00d3d987::
            if u_stk_74_1 & 1 ~= 0 then
                u_stk_74_1 = u_stk_74_1 & 0xfffffffe
            end
            if predicateResult6 then
                quest:RemoveThing(me, false, true)
            end
            if quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(scratchValue13) < 1 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                if state:GetInt("CurrentApples") == 0 then
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, quest:GetHero())
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPLEGIRL_ANY_APPLES", me, quest:GetHero(), false)
                else
                    conversationId2 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId2, quest:GetHero())
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_APPLEGIRL_MORE_APPLES", me, quest:GetHero(), false)
                end
                quest:SetTimer(scratchValue13, 15)
            end
            if not me:IsTalkedToByHero() then goto FLOW_native_label_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            ctr_64 = 0
            while true do
                u_stk_74_1 = u_stk_74_1 | 2
                predicateResult7 = quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", quest:GetHero()) and (state:GetInt("CurrentApples") + ctr_64 < 4)
                if u_stk_74_1 & 2 ~= 0 then
                    u_stk_74_1 = u_stk_74_1 & 0xfffffffd
                end
                if not predicateResult7 then break end
                if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
                ctr_64 = ctr_64 + 1
                quest:TakeObjectFromHero("OBJECT_APPLE_RED_01")
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            if ctr_64 == 1 then
                conversationId3 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId3, quest:GetHero())
                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_APPLEGIRL_ONE_MORE_APPLE", me, quest:GetHero(), false)
            elseif ctr_64 == nil then
                conversationId4 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId4, quest:GetHero())
                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPLEGIRL_NO_MORE_APPLES", me, quest:GetHero(), false)
            else
                conversationId5 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId5, quest:GetHero())
                quest:AddLineToConversation(conversationId5, "TEXT_QST_028_APPLEGIRL_MANY_MORE_APPLES", me, quest:GetHero(), false)
            end
            quest:Pause(1.0)
            state:SetInt("CurrentApples", state:GetInt("CurrentApples") + ctr_64)
            if ctr_64 == nil then goto FLOW_native_label_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            conversationId7 = quest:AddNewConversation(me, false, false)
            switch2 = state:GetInt("CurrentApples")
            repeat
                if switch2 == 1 then
                    quest:AddPersonToConversation(conversationId7, quest:GetHero())
                    quest:AddLineToConversation(conversationId7, "TEXT_QST_028_APPLEGIRL_THREE_NEEDED", me, quest:GetHero(), false)
                    break
                elseif switch2 == 2 then
                    quest:AddPersonToConversation(conversationId7, quest:GetHero())
                    quest:AddLineToConversation(conversationId7, "TEXT_QST_028_APPLEGIRL_TWO_NEEDED", me, quest:GetHero(), false)
                    break
                elseif switch2 == 3 then
                    quest:AddPersonToConversation(conversationId7, quest:GetHero())
                    quest:AddLineToConversation(conversationId7, "TEXT_QST_028_APPLEGIRL_ONE_NEEDED", me, quest:GetHero(), false)
                    break
                elseif switch2 == 4 then
                    scratchValue11 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue12)) then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_APPLEGIRL_THANKS", 0, false, true, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d3e06c end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
                    end
                    quest:GiveHeroObject("OBJECT_PIE_BLUEBERRY_01", -1)
                    state:SetInt("AppleMode", 2)
                    quest:ClearThingHasInformation(me)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue11)
                    goto FLOW_native_label_1
                else
                    goto FLOW_native_label_1
                end
            until true
            quest:Pause(1.0)
            ::FLOW_native_label_1::
        end
        while not quest:IsActiveThreadTerminating() do
            if state:GetBool("ChildAppleMode") then
                u_stk_74_1 = u_stk_74_1 | 4
                if quest:IsQuestActive("Q_GuildTrainingPreMelee") then
                    predicateResult10 = false
                    goto FLOW_after_lab_00d3e0a0
                end
                predicateResult10 = true
            else
                predicateResult10 = false
            end
            ::FLOW_after_lab_00d3e0a0::
            if u_stk_74_1 & 4 ~= 0 then
                u_stk_74_1 = u_stk_74_1 & 0xfffffffb
            end
            if predicateResult10 then
                quest:RemoveThing(me, false, true)
            end
            if quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(scratchValue13) < 1 then
                if quest:IsActiveThreadTerminating() then break end
                conversationId6 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId6, quest:GetHero())
                quest:AddLineToConversation(conversationId6, "TEXT_QST_028_APPLEGIRL_THANKS_AGAIN", me, quest:GetHero(), false)
                quest:SetTimer(scratchValue13, 15)
            end
            quest:NewScriptFrame(me)
        end
    end
    ::LAB_00d3e1b6::
    quest:DeregisterTimer(scratchValue13)
    ::LAB_00d3e1bf::
    resources:DestroyMovie(scratchValue12)
    do return end
    ::LAB_00d3d948::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(scratchValue11)
    quest:DeregisterTimer(scratchValue13)
    goto LAB_00d3e1bf
    ::LAB_00d3e06c::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(scratchValue11)
    goto LAB_00d3e1b6
    ::LAB_00d3e087::
    quest:PauseAllNonScriptedEntities(false)
    resources:ReleaseResource(ctr_64)
    goto LAB_00d3e1b6
end

-- AppleGirl.Init (retail 0x00d3d110)
function Init(quest, me)
    state:SetBool("HaveChatted", false)
end

-- AppleGirl.OnPersist (retail 0x00d44650)
function OnPersist(quest, context)
end

-- AppleGirl.OnPredicateFail (retail 0x00d3d120)
function OnPredicateFail(quest, me)
end

