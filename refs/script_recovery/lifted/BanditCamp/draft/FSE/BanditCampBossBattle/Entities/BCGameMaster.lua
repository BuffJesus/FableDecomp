-- Generated native draft: BCGameMaster. Review coverage report before use.
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
    local bVar2, bVar3, cVar4, fVar1, fret_0, fret_00, fret_01, fret_02, iVar10, iVar11, iVar8, iVar9, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, pCVar5, pCVar6, pcVar7, r1, r2, r3, xStack_10, xStack_20, x_stk_2c
    local alive = true
    bVar3 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    xStack_20 = resources:NewResource()
    if not __native_entity_state:GetStateBool("GivenPass") then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d071d7 end
        __native_entity_state:SetStateBool("GivenPass", true)
        quest:GiveThingHeroRewardItem(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", "")
    end
    quest:SetThingHasInformation(me, false, true, false)
    if not quest:GetStateBool("PubGameChatted") then
        bVar3 = true
        pCVar5 = quest:GetHero()
        bVar2 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", pCVar5)
        native_arg_sequence_1 = false
        if bVar2 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if not native_arg_sequence_1 then
            bVar2 = true
            if quest:GetStateBool("Gate2Open") then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then goto LAB_00d068cd end
    else
        goto LAB_00d068cd
    end
    goto FLOW_past_lab_00d068cd
    ::LAB_00d068cd::
    bVar2 = false
    ::FLOW_past_lab_00d068cd::
    if bVar3 then
    end
    bVar3 = false
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            resources:PrepareResource(xStack_20)
            bVar2 = resources:TryAcquire(xStack_20, me, 4)
            while not bVar2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d071d7 end
                bVar2 = resources:TryAcquire(xStack_20, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                repeat
                    bVar2 = me:IsTalkedToByHero()
                    if bVar2 then
                        goto LAB_00d069c6
                    else
                        bVar3 = true
                        pCVar5 = quest:GetHero()
                        bVar2 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", pCVar5)
                        native_arg_sequence_2 = false
                        if bVar2 then
                            native_arg_sequence_2 = true
                        else
                            native_arg_sequence_2 = false
                        end
                        if not native_arg_sequence_2 then
                            bVar2 = true
                            if quest:GetStateBool("Gate2Open") then
                                native_arg_sequence_2 = true
                            else
                                native_arg_sequence_2 = false
                            end
                        end
                        if native_arg_sequence_2 then goto LAB_00d069c6 end
                    end
                    goto FLOW_past_lab_00d069c6
                    ::LAB_00d069c6::
                    bVar2 = false
                    ::FLOW_past_lab_00d069c6::
                    if bVar3 then
                        bVar3 = false
                    end
                    if not bVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then break end
                        pCVar5 = quest:GetHero()
                        bVar3 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", pCVar5)
                        native_arg_sequence_3 = false
                        if bVar3 then
                            native_arg_sequence_3 = true
                        else
                            native_arg_sequence_3 = false
                        end
                        if not native_arg_sequence_3 then
                            bVar3 = true
                            if quest:GetStateBool("Gate2Open") then
                                native_arg_sequence_3 = true
                            else
                                native_arg_sequence_3 = false
                            end
                        end
                        if native_arg_sequence_3 then
                            bVar3 = false
                        end
                        if not bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                resources:PrepareResource(xStack_20)
                                quest:ClearThingHasInformation(me)
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                until not (not bVar3)
                                resources:ReleaseResource(xStack_20)
                                return
                            end
                            break
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then break end
                        xStack_10 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if not quest:GetStateBool("SpokenToSecondGuard") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d06bc5 end
                            x_stk_2c = resources:ScriptThing(xStack_20)
                            pCVar6 = x_stk_2c
                            fret_00 = quest:GetHealth(pCVar6)
                            fVar1 = 0.0
                            if fret_00 <= fVar1 then goto LAB_00d06c8d end
                            iVar11 = 0
                            iVar10 = 1
                            iVar9 = 0
                            iVar8 = 0
                            pcVar7 = "TEXT_QST_009_GAMES_MASTER_INTRO_GUARD_NOT_SPOKEN"
                            pCVar6 = quest:GetHero()
                            r1 = me:Speak(pCVar6, pcVar7, iVar8, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                            iVar8 = me:IsPerformingScriptTask()
                            cVar4 = iVar8
                            goto LAB_00d06c55
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d06bc5 end
                        x_stk_2c = resources:ScriptThing(xStack_20)
                        pCVar6 = x_stk_2c
                        fret_0 = quest:GetHealth(pCVar6)
                        fVar1 = 0.0
                        if fret_0 <= fVar1 then goto LAB_00d06c8d end
                        iVar11 = 0
                        iVar10 = 1
                        iVar9 = 0
                        iVar8 = 0
                        pcVar7 = "TEXT_QST_009_GAMES_MASTER_INTRO_GUARD_SPOKEN"
                        pCVar6 = quest:GetHero()
                        r2 = me:Speak(pCVar6, pcVar7, iVar8, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
                        iVar8 = me:IsPerformingScriptTask()
                        cVar4 = iVar8
                        goto LAB_00d06bbe
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        resources:ReleaseResource(xStack_20)
                        return
                    end
                until false
            end
        end
        goto LAB_00d071d7
    end
    ::LAB_00d06cd0::
    pCVar6 = quest:GetHero()
    bVar3 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", pCVar6)
    if (bVar3) or (quest:GetStateBool("Gate2Open")) then
        bVar3 = true
    end
    if bVar3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            resources:PrepareResource(xStack_20)
            quest:ClearThingHasInformation(me)
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
            until not (not bVar3)
            resources:ReleaseResource(xStack_20)
            return
        end
        goto LAB_00d071d7
    end
    quest:ClearThingHasInformation(me)
    quest:SetPrizeTavernTable(false)
    bVar3 = quest:GetSpotTheAdditionBeaten()
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d071d7 end
        pCVar6 = quest:GetHero()
        bVar3 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", pCVar6)
        if (bVar3) or (quest:GetStateBool("Gate2Open")) then
            bVar3 = true
        end
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                quest:SetPrizeTavernTable(true)
                resources:PrepareResource(xStack_20)
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                until not (not bVar3)
                resources:ReleaseResource(xStack_20)
                return
            end
            goto LAB_00d071d7
        end
        bVar3 = quest:GetSpotTheAdditionBeaten()
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d071d7 end
    quest:SetPrizeTavernTable(true)
    fret_01 = quest:SetQuitTavernGame(true)
    while (GSI->GetBestTimeGuessTheAddition(), 0.0 == fret_01 or (bVar3 = GSI->IsHeroInTavernGame(), bVar3)) do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            resources:ReleaseResource(xStack_20)
            return
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d071d7 end
    xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:PrepareResource(xStack_20)
    bVar3 = resources:TryAcquire(xStack_20, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d070e0 end
        bVar3 = resources:TryAcquire(xStack_20, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        -- LAB_00d06b0e: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        goto LAB_00d070ea
    end
    x_stk_2c = resources:ScriptThing(xStack_20)
    pCVar6 = x_stk_2c
    fret_02 = quest:GetHealth(pCVar6)
    fVar1 = 0.0
    if fVar1 < fret_02 then
        iVar11 = 0
        iVar10 = 1
        iVar9 = 0
        iVar8 = 0
        pcVar7 = "TEXT_QST_009_GAMES_MASTER_WON"
        pCVar6 = quest:GetHero()
        r3 = me:Speak(pCVar6, pcVar7, iVar8, (iVar9 ~= 0), (iVar10 ~= 0), (iVar11 ~= 0))
        iVar8 = me:IsPerformingScriptTask()
        cVar4 = iVar8
        while cVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d06bc5 end
            iVar8 = me:IsPerformingScriptTask()
            cVar4 = iVar8
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d070e0 end
    end
    quest:GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", -1, false)
    quest:RemoveItemFromContainer(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")
    quest:GiveHeroExperience(quest:ReadGlobalGameData(0x40))
    resources:PrepareResource(xStack_20)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    repeat
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    until not (not bVar3)
    ::LAB_00d071d7::
    resources:ReleaseResource(xStack_20)
    do return end
    ::LAB_00d06bbe::
    if not cVar4 then goto LAB_00d06c7b end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d06bc5 end
    iVar8 = me:IsPerformingScriptTask()
    cVar4 = iVar8
    goto LAB_00d06bbe
    ::LAB_00d06c55::
    if not cVar4 then goto LAB_00d06c7b end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d06bc5 end
    iVar8 = me:IsPerformingScriptTask()
    cVar4 = iVar8
    goto LAB_00d06c55
    ::LAB_00d06bc5::
    quest:PauseAllNonScriptedEntities(false)
    goto LAB_00d070ea
    ::LAB_00d06c7b::
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        goto LAB_00d06c8d
    end
    goto FLOW_past_lab_00d06c8d
    ::LAB_00d06c8d::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    resources:PrepareResource(xStack_20)
    quest:SetStateBool("PubGameChatted", true)
    goto LAB_00d06cd0
    ::FLOW_past_lab_00d06c8d::
    ::LAB_00d070e0::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d070ea::
    resources:DestroyMovie(xStack_10)
    resources:ReleaseResource(xStack_20)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("GivenPass", false)
end

function OnPersist(quest, me, context)
    local givenPass = quest:GetStateBool("GivenPass") or false
    givenPass = quest:PersistTransferBool(context, "GivenPass", givenPass)
    quest:SetStateBool("GivenPass", givenPass)
end

function OnPredicateFail(quest, me)
end

