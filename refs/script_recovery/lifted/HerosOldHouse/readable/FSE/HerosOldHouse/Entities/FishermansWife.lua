-- Readable native conversion: FishermansWife. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    HOH_MoralityGain = 3936,  -- 0.029999999329447746
    HOH_MoralityLoss = 3940,  -- -0.029999999329447746
}

-- per-entity fields (native class members; one Lua state per entity instance)
local leavingHappy_, talkedTo_, initialAngle

-- FishermansWife.Main (retail 0x00d8bcd0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, predicateResult22, getStateBool, questionAnswer, line, getPos
    local hiddenBooty, speechResult, this_00, movie, resource3, timerId, movie3, scriptThing
    if not quest:NewScriptFrame(me) then return end
    local resource4 = resources:NewResource()
    resources:PrepareResource(resource4)
    while not resources:TryAcquire(resource4, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d8d57a end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d8d57a end
    quest:EntitySetAsKillable(me, false, true)
    quest:SetIsPushableByHero(me, false)
    hiddenBooty = quest:GetThingWithScriptName("HiddenBooty")
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 5)
    getStateBool = leavingHappy_
    ::FLOW_loop_head_lab_00d8cc55::
    if getStateBool then goto FLOW_loop_exit_lab_00d8cc55 end
    if not quest:NewScriptFrame(me) then goto LAB_00d8d571 end
    if quest:GetTimer(timerId) < 1 then
        local hero = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d8d571 end
            local conversationId = quest:AddNewConversation(me, false, false)
            local hero2 = quest:GetHero()
            quest:AddPersonToConversation(conversationId, hero2)
            local hero3 = quest:GetHero()
            quest:AddLineToConversation(conversationId, "TEXT_QST_032_FISHERWIFE_CRY", me, hero3, false)
            quest:SetTimer(timerId, 15)
        end
    end
    if me:IsTalkedToByHero() then
        if quest:IsActiveThreadTerminating() then goto LAB_00d8d571 end
        if not quest:GetStateBool("Helped") then
            if (not quest:GetStateBool("Helping") or quest:IsDiggingSpotEnabled(hiddenBooty)) or quest:GetHeroGold() < 500 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d8d571 end
                local movie4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if not talkedTo_ then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        goto LAB_00d8d571
                    end
                    resource3 = resources:NewResource()
                    local hero4 = quest:GetHero()
                    hero4:AcquireControl(4)
                    local actorMap3 = resources:NewActorMap()
                    resources:SetActor(actorMap3, "HERO", resource3)
                    resources:SetActor(actorMap3, "WIFE", resource4)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GHOSTFISH_WIFE_INTRO", actorMap3, false, true)
                    quest:FixMovieSequenceCamera(false)
                    talkedTo_ = true
                    resources:DestroyActorMap(actorMap3)
                    resources:ReleaseResource(resource3)
                else
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        goto LAB_00d8d571
                    end
                    local scratchValue15 = resources:ScriptThing(resource4)
                    if 0.0 < quest:GetHealth(scratchValue15) then
                        line = "TEXT_QST_032_FISHERWIFE_REPEAT_GREET"
                        local hero5 = quest:GetHero()
                        if not me:Speak(hero5, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            goto LAB_00d8d571
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00d8d571
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            goto LAB_00d8d571
                        end
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                goto LAB_00d8ca0f
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d8d571 end
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:GiveHeroYesNoQuestion("TEXT_QST_032_FISHERWIFE_GOLD_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer < 0 do
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                else
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00d8d571
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                goto LAB_00d8d571
            end
            local predicateResult7 = quest:IsActiveThreadTerminating()
            if questionAnswer == 1 then
                if not predicateResult7 then
                    quest:SetStateBool("Helped", true)
                    local resource5 = resources:NewResource()
                    local resource = resource5
                    local hero6 = quest:GetHero()
                    resources:TryAcquire(resource, hero6, 4)
                    local actorMap2 = resources:NewActorMap()
                    resources:SetActor(actorMap2, "HERO", resource5)
                    resources:SetActor(actorMap2, "WIFE", resource4)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GHOSTFISH_WIFE_SUCCESS", actorMap2, false, true)
                    quest:FixMovieSequenceCamera(false)
                    quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.HOH_MoralityGain))
                    quest:AddItemToContainer(me, "OBJECT_GOLDBAG_MEDIUM_WITH_COINS_500")
                    quest:GiveHeroGold(-500)
                    local pQuestName = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_03", "", "OakBay")
                    quest:ClearThingHasInformation(me)
                    quest:MiniMapRemoveMarker(me)
                    local ghostFisherman = quest:GetThingWithScriptName("GhostFisherman")
                    quest:MiniMapAddMarker(ghostFisherman, "HUD_ORB_QUEST_VIGNETTE")
                    quest:FadeScreenIn()
                    leavingHappy_ = true
                    resources:DestroyActorMap(actorMap2)
                    this_00 = resource5
                    goto LAB_00d8c74b
                end
                goto FLOW_hoist_lab_00d8c74b_1
            end
            goto FLOW_past_lab_00d8c74b
            ::LAB_00d8c74b::
            resources:ReleaseResource(this_00)
            goto LAB_00d8c750
            ::FLOW_hoist_lab_00d8c74b_1::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            goto LAB_00d8d571
            ::FLOW_past_lab_00d8c74b::
            if predicateResult7 then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                goto LAB_00d8d571
            end
            if not talkedTo_ then
                if not quest:IsActiveThreadTerminating() then
                    local resource6 = resources:NewResource()
                    local resource2 = resource6
                    local hero7 = quest:GetHero()
                    resources:TryAcquire(resource2, hero7, 4)
                    local actorMap = resources:NewActorMap()
                    resources:SetActor(actorMap, "HERO", resource6)
                    resources:SetActor(actorMap, "WIFE", resource4)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GHOSTFISH_WIFE_INTRO", actorMap, false, true)
                    quest:FixMovieSequenceCamera(false)
                    talkedTo_ = true
                    resources:DestroyActorMap(actorMap)
                    this_00 = resource6
                    goto LAB_00d8c74b
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                goto LAB_00d8d571
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                goto LAB_00d8d571
            end
            scriptThing = resources:ScriptThing(resource4)
            if 0.0 < quest:GetHealth(scriptThing) then
                line = "TEXT_QST_032_FISHERWIFE_REPEAT_GREET"
                local hero8 = quest:GetHero()
                if not me:Speak(hero8, line, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00d8d571
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d8d571
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00d8d571
                end
            end
            ::LAB_00d8c750::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00d8d571 end
            local movie5 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local scratchValue16 = resources:ScriptThing(resource4)
            if 0.0 < quest:GetHealth(scratchValue16) then
                line = "TEXT_QST_032_FISHERWIFE_THANKS"
                local hero9 = quest:GetHero()
                if not me:Speak(hero9, line, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    quest:DeregisterTimer(timerId)
                    goto LAB_00d8d57a
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        quest:DeregisterTimer(timerId)
                        goto LAB_00d8d57a
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    goto LAB_00d8d571
                end
            end
            quest:PauseAllNonScriptedEntities(false)
        end
        ::LAB_00d8ca0f::
        quest:EntitySetFacingAngle(me, initialAngle, true)
    end
    if me:MsgIsHitByHero() then
        goto LAB_00d8cab0
    else
        if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d8cab0 end
        predicateResult = false
    end
    goto FLOW_past_lab_00d8cab0
    ::LAB_00d8cab0::
    predicateResult = true
    ::FLOW_past_lab_00d8cab0::
    if predicateResult then
        if quest:IsActiveThreadTerminating() then goto LAB_00d8d571 end
        movie3 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        local scratchValue = resources:ScriptThing(resource4)
        if 0.0 < quest:GetHealth(scratchValue) then
            line = "TEXT_QST_032_FISHERWIFE_ATTACKED"
            local hero10 = quest:GetHero()
            speechResult = me:Speak(hero10, line, 0, false, true, false)
            if me:IsPerformingScriptTask() then
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then goto LAB_00d8cc48 end
                quest:PauseAllNonScriptedEntities(false)
                quest:DeregisterTimer(timerId)
                goto LAB_00d8d57a
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                goto LAB_00d8d571
            end
        end
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
        quest:EntitySetAsKillable(me, true, true)
        local fisherWifeLeaveMarker4 = quest:GetThingWithScriptName("FisherWifeLeaveMarker")
        quest:SetStateBool("WifeAttacked", true)
        if not (fisherWifeLeaveMarker4 ~= nil and not fisherWifeLeaveMarker4:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = fisherWifeLeaveMarker4:GetPos()
        end
        me:MoveToPosition(getPos, 3.0, ENTITY_MOVE_RUN, false, true)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); goto LAB_00d8d57a end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00d8d568 end
        quest:SetStateBool("WifeAttackedAndLeft", true)
        quest:FadeOutAndKillEntity(me, true, 1.0, true)
    end
    ::FLOW_after_lab_00d8cc55::
    getStateBool = leavingHappy_
    goto FLOW_loop_head_lab_00d8cc55
    ::FLOW_loop_exit_lab_00d8cc55::
    if not quest:IsActiveThreadTerminating() then
        local p0_00 = quest:GetThingWithScriptName("FisherWifeLeaveMarker"):GetPos()
        me:MoveToPosition(p0_00, 3.0, ENTITY_MOVE_WALK, false, true)
        while not quest:IsActiveThreadTerminating() do
            if quest:GetTimer(timerId) < 1 then
                local hero11 = quest:GetHero()
                if quest:IsDistanceBetweenThingsUnder(hero11, me, 5.5) then
                    if quest:IsActiveThreadTerminating() then break end
                    local conversationId2 = quest:AddNewConversation(me, false, false)
                    local hero12 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId2, hero12)
                    local hero13 = quest:GetHero()
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_032_FISHERWIFE_SHOPPING_ASIDE", me, hero13, false)
                    quest:SetTimer(timerId, 15)
                end
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then break end
                me:ClearCommands()
                resource3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue13 = resources:ScriptThing(resource4)
                if 0.0 < quest:GetHealth(scratchValue13) then
                    line = "TEXT_QST_032_FISHERWIFE_SHOPPING"
                    local hero14 = quest:GetHero()
                    if not me:Speak(hero14, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(resource3)
                        goto LAB_00d8d571
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(resource3)
                            goto LAB_00d8d571
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(resource3)
                        break
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(resource3)
                local fisherWifeLeaveMarker5 = quest:GetThingWithScriptName("FisherWifeLeaveMarker")
                if fisherWifeLeaveMarker5 == nil then
                    getPos = {x = 0, y = 0, z = 0}
                else
                    getPos = fisherWifeLeaveMarker5:GetPos()
                end
                me:MoveToPosition(getPos, 3.0, ENTITY_MOVE_WALK, false, true)
            end
            if me:MsgIsHitByHero() then
                goto LAB_00d8d21b
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d8d21b end
                predicateResult22 = false
            end
            goto FLOW_past_lab_00d8d21b
            ::LAB_00d8d21b::
            predicateResult22 = true
            ::FLOW_past_lab_00d8d21b::
            if predicateResult22 then
                if quest:IsActiveThreadTerminating() then break end
                me:ClearCommands()
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                scriptThing = resources:ScriptThing(resource4)
                if 0.0 < quest:GetHealth(scriptThing) then
                    line = "TEXT_QST_032_FISHERWIFE_ATTACKED_AFTER"
                    local hero15 = quest:GetHero()
                    if not me:Speak(hero15, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d8d571
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00d8d571
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        break
                    end
                end
                quest:EntitySetAsKillable(me, true, true)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:PrepareResource(resource4)
                while not resources:TryAcquire(resource4, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00d8d571 end
                end
                if quest:IsActiveThreadTerminating() then break end
                quest:SetStateBool("WifeAttacked", true)
                local fisherWifeLeaveMarker = quest:GetThingWithScriptName("FisherWifeLeaveMarker")
                if fisherWifeLeaveMarker == nil then
                    getPos = {x = 0, y = 0, z = 0}
                else
                    getPos = fisherWifeLeaveMarker:GetPos()
                end
                me:MoveToPosition(getPos, 3.0, ENTITY_MOVE_RUN, false, true)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d8d568 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d8d568 end
                quest:SetStateBool("WifeAttackedAndLeft", true)
                quest:FadeOutAndKillEntity(me, true, 1.0, true)
            end
            if me:IsPerformingScriptTask() then
                quest:NewScriptFrame(me)
            else
                if quest:IsActiveThreadTerminating() then break end
                quest:FadeOutAndKillEntity(me, true, 1.0, true)
                quest:NewScriptFrame(me)
            end
        end
    end
    ::LAB_00d8d571::
    quest:DeregisterTimer(timerId)
    ::LAB_00d8d57a::
    resources:ReleaseResource(resource4)
    do return end
    ::LAB_00d8cc48::
    if not me:IsPerformingScriptTask() then
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            goto LAB_00d8d571
        end
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
        quest:EntitySetAsKillable(me, true, true)
        local fisherWifeLeaveMarker3 = quest:GetThingWithScriptName("FisherWifeLeaveMarker")
        quest:SetStateBool("WifeAttacked", true)
        if not (fisherWifeLeaveMarker3 ~= nil and not fisherWifeLeaveMarker3:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = fisherWifeLeaveMarker3:GetPos()
        end
        me:MoveToPosition(getPos, 3.0, ENTITY_MOVE_RUN, false, true)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); goto LAB_00d8d57a end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00d8d568 end
        quest:SetStateBool("WifeAttackedAndLeft", true)
        quest:FadeOutAndKillEntity(me, true, 1.0, true)
        goto FLOW_after_lab_00d8cc55
    end
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then goto LAB_00d8cc48 end
    quest:PauseAllNonScriptedEntities(false)
    quest:DeregisterTimer(timerId)
    goto LAB_00d8d57a
    ::LAB_00d8d568::
    goto LAB_00d8d571
end

-- FishermansWife.Init (retail 0x00d8a700)
function Init(quest, me)
    talkedTo_ = false
    leavingHappy_ = false
    initialAngle = me:GetAngleXY()
    quest:SetThingHasInformation(me, false, false, false)
    if quest:GetStateBool("Helping") and not quest:GetStateBool("Helped") then
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_VIGNETTE")
    end
end

-- FishermansWife.OnPersist (retail 0x00d8aba0)
function OnPersist(quest, me, context)
    local talkedTo = talkedTo_ or false
    talkedTo_ = quest:PersistTransferBool(context, "TalkedTo", talkedTo)
    local leavingHappy = leavingHappy_ or false
    leavingHappy_ = quest:PersistTransferBool(context, "LeavingHappy", leavingHappy)
end

-- FishermansWife.OnPredicateFail (retail 0x00d8a770)
function OnPredicateFail(quest, me)
    if not me:MsgIsKilledBy("SCRIPT_NAME_HERO") then return end
    quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.HOH_MoralityLoss))
    quest:SetStateBool("WifeAttackedAndLeft", true)
end

