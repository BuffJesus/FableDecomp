-- Generated native draft: AppleGirl. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local __native_condition_1, __native_condition_2, __native_condition_3, bVar2, cVar3, ctr_64, fVar1, fVar13, fret_0, fret_00, fret_01, iVar11, iVar12, iVar4, iVar7, native_arg_switch_2, pCVar5, pCVar6, pThing, pcVar10, r1, r2, r3, r4, xStack_60, xStack_70_2, xStack_84, xStack_88, x_stk_18, x_stk_c
    local alive = true
    xStack_84 = resources:NewResource()
    bVar2 = false
    if bVar2 ~= 0 then
    end
    bVar2 = resources:TryAcquire(xStack_84, me, 4)
    while not bVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            resources:ReleaseResource(xStack_84)
            return
        end
        bVar2 = resources:TryAcquire(xStack_84, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        resources:ReleaseResource(xStack_84)
        return
    end
    me:SetFriendsWithEverythingFlag(me)
    quest:EntitySetAsKillable(me, false, true)
    r1 = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:EntityAttachToVillage(me, r1)
    __native_entity_state:SetStateInt("AppleMode", 0)
    iVar4 = quest:RegisterTimer()
    xStack_88 = iVar4
    quest:SetTimer(iVar4, 0xf)
    __native_entity_state:SetStateInt("CurrentApples", 0)
    __native_entity_state:SetStateBool("ChildAppleMode", false)
    quest:SetThingHasInformation(me, false, true, false)
    iVar7 = __native_entity_state:GetStateInt("AppleMode")
    while iVar7 == 0 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d3e1b6 end
        fVar13 = 5.5
        pCVar6 = quest:GetHero()
        bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar13)
        __native_condition_1 = bVar2
        if __native_condition_1 then
            iVar7 = quest:GetTimer(iVar4)
            __native_condition_1 = iVar7 < 1
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d3e1b6 end
            iVar4 = quest:AddNewConversation(me, false, false)
            pCVar6 = quest:GetHero()
            quest:AddPersonToConversation(iVar4, pCVar6)
            pCVar6 = quest:GetHero()
            quest:AddLineToConversation(iVar4, "TEXT_QST_028_APPLEGIRL_HELP", me, pCVar6, false)
            quest:SetTimer(xStack_88, 0xf)
        end
        bVar2 = me:IsTalkedToByHero()
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d3e1b6 end
            xStack_60 = resources:StartMovie("")
            quest:StartMovieSequence()
            pCVar6 = 0x1
            quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
            if not __native_entity_state:GetStateBool("HaveChatted") then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d3e06c end
                __native_entity_state:SetStateBool("HaveChatted", true)
                x_stk_18 = resources:ScriptThing(xStack_84)
                pCVar5 = x_stk_18
                fret_0 = quest:GetHealth(pCVar5)
                fVar1 = 0.0
                if fVar1 < fret_0 then
                    iVar12 = 0
                    iVar11 = 1
                    iVar4 = 0
                    iVar7 = 0
                    pcVar10 = "TEXT_QST_028_APPLEGIRL_CHAT"
                    pCVar5 = quest:GetHero()
                    r2 = me:Speak(pCVar5, pcVar10, iVar7, (iVar4 ~= 0), (iVar11 ~= 0), (iVar12 ~= 0))
                    iVar7 = me:IsPerformingScriptTask()
                    cVar3 = iVar7
                    while cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00d3d948 end
                        iVar7 = me:IsPerformingScriptTask()
                        cVar3 = iVar7
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d3e06c end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPLEGIRL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d3e087 end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPLEGIRL_QUESTION_AGAIN", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            end
            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
            while iVar7 < 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_60)
                    quest:DeregisterTimer(iVar4)
                    xStack_70_2 = nil
                    return
                end
                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d3e087 end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if iVar7 == 1 then
                if bVar2 then goto LAB_00d3e06c end
                __native_entity_state:SetStateInt("AppleMode", 1)
                bVar2 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d3e087 end
                    __native_entity_state:SetStateBool("ChildAppleMode", true)
                end
            else
                if bVar2 then goto LAB_00d3e06c end
                x_stk_c = resources:ScriptThing(xStack_84)
                pCVar5 = x_stk_c
                fret_00 = quest:GetHealth(pCVar5)
                fVar1 = 0.0
                if fVar1 < fret_00 then
                    iVar12 = 0
                    iVar11 = 1
                    iVar4 = 0
                    iVar7 = 0
                    pcVar10 = "TEXT_QST_028_APPLEGIRL_IMPLORE"
                    pCVar6 = quest:GetHero()
                    r3 = me:Speak(pCVar6, pcVar10, iVar7, (iVar4 ~= 0), (iVar11 ~= 0), (iVar12 ~= 0))
                    iVar7 = me:IsPerformingScriptTask()
                    cVar3 = iVar7
                    while cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00d3d948 end
                        iVar7 = me:IsPerformingScriptTask()
                        cVar3 = iVar7
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d3e06c end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_60)
        end
        iVar7 = __native_entity_state:GetStateInt("AppleMode")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        iVar7 = __native_entity_state:GetStateInt("AppleMode")
        while iVar7 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d3e1b6 end
            if __native_entity_state:GetStateBool("ChildAppleMode") then
                bVar2 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
                if bVar2 then
                    bVar2 = false
                    goto FLOW_after_lab_00d3d987
                end
                bVar2 = true
            else
                -- LAB_00d3d987: (native jump target)
                bVar2 = false
            end
            ::FLOW_after_lab_00d3d987::
            if false then
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d3e1b6 end
                quest:RemoveThing(me, false, true)
            end
            fVar13 = 5.5
            pCVar6 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar13)
            __native_condition_2 = bVar2
            if __native_condition_2 then
                iVar7 = quest:GetTimer(xStack_88)
                __native_condition_2 = iVar7 < 1
            end
            if __native_condition_2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d3e1b6 end
                if __native_entity_state:GetStateInt("CurrentApples") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d3e1b6 end
                    iVar4 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar4, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar4, "TEXT_QST_028_APPLEGIRL_ANY_APPLES", me, pCVar6, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d3e1b6 end
                    iVar4 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar4, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar4, "TEXT_QST_028_APPLEGIRL_MORE_APPLES", me, pCVar6, false)
                end
                quest:SetTimer(xStack_88, 0xf)
            end
            bVar2 = me:IsTalkedToByHero()
            if not bVar2 then goto FLOW_native_label_1 end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d3e1b6 end
            ctr_64 = 0
            while true do
                pCVar6 = quest:GetHero()
                bVar2 = quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", pCVar6)
                if (bVar2) and (__native_entity_state:GetStateInt("CurrentApples") + ctr_64 < 4) then
                    bVar2 = true
                else
                    bVar2 = false
                end
                if false then
                end
                if not bVar2 then break end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d3e1b6 end
                ctr_64 = ctr_64 + 1
                quest:TakeObjectFromHero("OBJECT_APPLE_RED_01")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d3e1b6 end
            if ctr_64 == 0x1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d3e1b6 end
                iVar4 = quest:AddNewConversation(me, false, false)
                pCVar6 = quest:GetHero()
                quest:AddPersonToConversation(iVar4, pCVar6)
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(iVar4, "TEXT_QST_028_APPLEGIRL_ONE_MORE_APPLE", me, pCVar6, false)
            else
                if ctr_64 == nil then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d3e1b6 end
                    iVar4 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar4, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar4, "TEXT_QST_028_APPLEGIRL_NO_MORE_APPLES", me, pCVar6, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d3e1b6 end
                    iVar4 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar4, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar4, "TEXT_QST_028_APPLEGIRL_MANY_MORE_APPLES", me, pCVar6, false)
                end
            end
            quest:Pause(1.0)
            __native_entity_state:SetStateInt("CurrentApples", __native_entity_state:GetStateInt("CurrentApples") + ctr_64)
            if ctr_64 == nil then goto FLOW_native_label_1 end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d3e1b6 end
            iVar7 = quest:AddNewConversation(me, false, false)
            native_arg_switch_2 = __native_entity_state:GetStateInt("CurrentApples")
            repeat
                if native_arg_switch_2 == 1 then
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar7, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar7, "TEXT_QST_028_APPLEGIRL_THREE_NEEDED", me, pCVar6, false)
                    break
                else
                    if native_arg_switch_2 == 2 then
                        pCVar6 = quest:GetHero()
                        quest:AddPersonToConversation(iVar7, pCVar6)
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_APPLEGIRL_TWO_NEEDED", me, pCVar6, false)
                        break
                    else
                        if native_arg_switch_2 == 3 then
                            pCVar6 = quest:GetHero()
                            quest:AddPersonToConversation(iVar7, pCVar6)
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar7, "TEXT_QST_028_APPLEGIRL_ONE_NEEDED", me, pCVar6, false)
                            break
                        else
                            if native_arg_switch_2 == 4 then
                                xStack_60 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                pCVar5 = 0x1
                                quest:PauseAllNonScriptedEntities((pCVar5 ~= 0))
                                x_stk_c = resources:ScriptThing(xStack_84)
                                pCVar6 = x_stk_c
                                fret_01 = quest:GetHealth(pCVar6)
                                fVar1 = 0.0
                                if fVar1 < fret_01 then
                                    iVar12 = 0
                                    iVar11 = 1
                                    iVar4 = 0
                                    iVar7 = 0
                                    pcVar10 = "TEXT_QST_028_APPLEGIRL_THANKS"
                                    pCVar6 = quest:GetHero()
                                    r4 = me:Speak(pCVar6, pcVar10, iVar7, (iVar4 ~= 0), (iVar11 ~= 0), (iVar12 ~= 0))
                                    iVar7 = me:IsPerformingScriptTask()
                                    cVar3 = iVar7
                                    while cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        if bVar2 then goto LAB_00d3e06c end
                                        iVar7 = me:IsPerformingScriptTask()
                                        cVar3 = iVar7
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00d3e087 end
                                end
                                quest:GiveHeroObject("OBJECT_PIE_BLUEBERRY_01", -1)
                                __native_entity_state:SetStateInt("AppleMode", 2)
                                quest:ClearThingHasInformation(me)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_60)
                                goto FLOW_native_label_1
                            else
                                goto FLOW_native_label_1
                            end
                        end
                    end
                end
            until not (false)
            quest:Pause(1.0)
            ::FLOW_native_label_1::
            iVar7 = __native_entity_state:GetStateInt("AppleMode")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            while not bVar2 do
                if __native_entity_state:GetStateBool("ChildAppleMode") then
                    bVar2 = quest:IsQuestActive("Q_GuildTrainingPreMelee")
                    if bVar2 then
                        bVar2 = false
                        goto FLOW_after_lab_00d3e0a0
                    end
                    bVar2 = true
                else
                    -- LAB_00d3e0a0: (native jump target)
                    bVar2 = false
                end
                ::FLOW_after_lab_00d3e0a0::
                if false then
                end
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    quest:RemoveThing(me, false, true)
                end
                fVar13 = 5.5
                pCVar6 = quest:GetHero()
                bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar13)
                __native_condition_3 = bVar2
                if __native_condition_3 then
                    iVar7 = quest:GetTimer(xStack_88)
                    __native_condition_3 = iVar7 < 1
                end
                if __native_condition_3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    iVar4 = quest:AddNewConversation(me, false, false)
                    pCVar6 = quest:GetHero()
                    quest:AddPersonToConversation(iVar4, pCVar6)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar4, "TEXT_QST_028_APPLEGIRL_THANKS_AGAIN", me, pCVar6, false)
                    quest:SetTimer(xStack_88, 0xf)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
            end
        end
    end
    ::LAB_00d3e1b6::
    quest:DeregisterTimer(xStack_88)
    ::LAB_00d3e1bf::
    resources:DestroyMovie(xStack_84)
    do return end
    ::LAB_00d3d948::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_60)
    quest:DeregisterTimer(xStack_88)
    goto LAB_00d3e1bf
    ::LAB_00d3e06c::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_60)
    goto LAB_00d3e1b6
    ::LAB_00d3e087::
    quest:PauseAllNonScriptedEntities(false)
    resources:ReleaseResource(ctr_64)
    goto LAB_00d3e1b6
end

function Init(quest, me)
    __native_entity_state:SetStateBool("HaveChatted", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

