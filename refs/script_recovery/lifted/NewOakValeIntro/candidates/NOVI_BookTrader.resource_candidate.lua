-- DISABLED resource-aware BookTrader candidate; see candidate report for remaining gaps.
-- Generated native draft: NOVI_BookTrader. Review coverage report before use.
-- Not copied from the working port; registration remains disabled.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("DoneIntro", false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

local function __resource_main(quest, me, resources)
    local book_resource, book_thing, book_movie
    local function release_book()
        assert(book_movie == nil and book_thing == nil, "BookTrader cleanup order changed")
        resources:ReleaseResource(book_resource)
        book_resource = nil
    end
    local __native_condition_1, bVar4, cVar5, fVar18, fVar23, iVar11, native_arg_book_ally, native_arg_book_has_health, native_arg_book_health_actor, native_arg_book_home, native_arg_book_initial_home, native_arg_book_listener, pCVar1, pCVar13, pCVar20, pCVar21, pCVar22, pCVar24, pCVar8, pcVar19, piVar25, ppVar10, r1, r2, r3, r4, r5, r6, r7, uVar12, uVar16, uVar17, uVar6, uVar9
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        book_resource = resources:NewResource()
        alive = not quest:IsActiveThreadTerminating()
        cVar5 = not alive
        uVar16 = 0
        while true do
            if cVar5 then
                release_book()
                return
            end
            resources:PrepareResource(book_resource)
            piVar25 = 0x3
            cVar5 = resources:TryAcquire(book_resource, me, 3)
            while not cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    release_book()
                    return
                end
                cVar5 = resources:TryAcquire(book_resource, me, 3)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
            fVar23 = 0.1
            native_arg_book_initial_home = me:GetHomePos()
            bVar4 = (me ~= nil and me:IsDistanceFromPositionOver(native_arg_book_initial_home, fVar23))
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    native_arg_book_home = me:GetHomePos()
                    while true do
                        fVar23 = 2.0
                        book_thing = resources:NewThingFromResource(book_resource)
                        bVar4 = resources:ThingIsDistanceFromPositionOver(book_thing, native_arg_book_home, fVar23)
                        resources:DestroyThing(book_thing); book_thing = nil
                        if not bVar4 then break end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db4f5a end
                        resources:MoveToPosition(book_resource, native_arg_book_home, 0.0, 0, false, true)
                        bVar4 = resources:IsPerformingScriptTask(book_resource)
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db4f5a end
                                bVar4 = resources:IsPerformingScriptTask(book_resource)
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            release_book()
                            return
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        quest:FaceThingByScriptName(me, "NOVI_Theresa", false)
                        goto LAB_00db4234
                    end
                end
                break
            end
            ::LAB_00db4234::
            bVar4 = resources:IsHitByHeroExceptAbility(me, 14)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    native_arg_book_ally = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, native_arg_book_ally)
                    uVar9 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(uVar9, me)
                    require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
                    resources:PrepareResource(book_resource)
                    cVar5 = resources:TryAcquire(book_resource, me, 4)
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db4f5a end
                        cVar5 = resources:TryAcquire(book_resource, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        pCVar20 = ""
                        book_movie = resources:StartMovie("")
                        resources:Pause(true)
                        book_thing = resources:NewThingFromResource(book_resource)
                        fVar18 = resources:ThingHealth(book_thing)
                        resources:DestroyThing(book_thing); book_thing = nil
                        native_arg_book_has_health = fVar18 > 0.0
                        if native_arg_book_has_health then
                            bVar4 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_TRADER_ON_HIT"
                            pCVar8 = quest:GetHero()
                            r1 = resources:Speak(book_resource, pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                            bVar4 = resources:IsPerformingScriptTask(book_resource)
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(book_movie); book_movie = nil
                                        goto LAB_00db4f5a
                                    end
                                    bVar4 = resources:IsPerformingScriptTask(book_resource)
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                -- LAB_00db4f51: (native jump target)
                                resources:DestroyMovie(book_movie); book_movie = nil
                                break
                            end
                        end
                        quest:FaceThingByScriptName(me, "NOVI_Theresa", true)
                        resources:Pause(false)
                        -- LAB_00db4ce1: (native jump target)
                        resources:DestroyMovie(book_movie); book_movie = nil
                        goto LAB_00db4ce6
                    end
                end
                break
            end
            cVar5 = me:IsTalkedToByHero()
            if cVar5 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    resources:PrepareResource(book_resource)
                    cVar5 = resources:TryAcquire(book_resource, me, 4)
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db4f5a end
                        cVar5 = resources:TryAcquire(book_resource, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        pCVar20 = ""
                        book_movie = resources:StartMovie("")
                        uVar9 = 1
                        resources:Pause(true)
                        uVar6 = uVar9
                        if quest:GetStateBool("GivenSweets") then
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                book_thing = resources:NewThingFromResource(book_resource)
                                fVar18 = resources:ThingHealth(book_thing)
                                resources:DestroyThing(book_thing); book_thing = nil
                                native_arg_book_has_health = fVar18 > 0.0
                                if native_arg_book_has_health then
                                    bVar4 = false
                                    pCVar22 = 0x1
                                    pCVar21 = 0x0
                                    pCVar20 = 0x0
                                    pcVar19 = "TEXT_QST_048_TRADER_INTRO_10"
                                    pCVar8 = quest:GetHero()
                                    r2 = resources:Speak(book_resource, pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                    bVar4 = resources:IsPerformingScriptTask(book_resource)
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                resources:Pause(false)
                                                resources:DestroyMovie(book_movie); book_movie = nil
                                                goto LAB_00db4f5a
                                            end
                                            bVar4 = resources:IsPerformingScriptTask(book_resource)
                                        until not (bVar4)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(book_movie); book_movie = nil
                                        goto LAB_00db4f5a
                                    end
                                end
                                goto LAB_00db4c85
                            end
                            resources:Pause(false)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            goto LAB_00db4f5a
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00db4f39: (native jump target)
                            -- LAB_00db4f45: (native jump target)
                            resources:Pause(false)
                            -- LAB_00db4f4d: (native jump target)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            goto LAB_00db4f5a
                        end
                        if not __native_entity_state:GetStateBool("DoneIntro") then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(book_movie); book_movie = nil
                                goto LAB_00db4f5a
                            end
                            book_thing = resources:NewThingFromResource(book_resource)
                            fVar18 = resources:ThingHealth(book_thing)
                            resources:DestroyThing(book_thing); book_thing = nil
                            native_arg_book_has_health = fVar18 > 0.0
                            if native_arg_book_has_health then
                                bVar4 = false
                                pCVar22 = 0x1
                                pCVar21 = 0x0
                                pCVar20 = 0x0
                                pcVar19 = "TEXT_QST_048_TRADER_INTRO"
                                pCVar8 = quest:GetHero()
                                r3 = resources:Speak(book_resource, pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                bVar4 = resources:IsPerformingScriptTask(book_resource)
                                if bVar4 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            resources:Pause(false)
                                            resources:DestroyMovie(book_movie); book_movie = nil
                                            goto LAB_00db4f5a
                                        end
                                        bVar4 = resources:IsPerformingScriptTask(book_resource)
                                    until not (bVar4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    resources:Pause(false)
                                    resources:DestroyMovie(book_movie); book_movie = nil
                                    goto LAB_00db4f5a
                                end
                            end
                            __native_entity_state:SetStateBool("DoneIntro", true)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(book_movie); book_movie = nil
                                goto LAB_00db4f5a
                            end
                            book_thing = resources:NewThingFromResource(book_resource)
                            fVar18 = resources:ThingHealth(book_thing)
                            resources:DestroyThing(book_thing); book_thing = nil
                            native_arg_book_has_health = fVar18 > 0.0
                            if native_arg_book_has_health then
                                bVar4 = false
                                pCVar22 = 0x1
                                pCVar21 = 0x0
                                pCVar20 = 0x0
                                pcVar19 = "TEXT_QST_048_TRADER_STILL_GOT"
                                pCVar8 = quest:GetHero()
                                r4 = resources:Speak(book_resource, pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                bVar4 = resources:IsPerformingScriptTask(book_resource)
                                if bVar4 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            resources:Pause(false)
                                            resources:DestroyMovie(book_movie); book_movie = nil
                                            goto LAB_00db4f5a
                                        end
                                        bVar4 = resources:IsPerformingScriptTask(book_resource)
                                    until not (bVar4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    resources:Pause(false)
                                    resources:DestroyMovie(book_movie); book_movie = nil
                                    goto LAB_00db4f5a
                                end
                            end
                        end
                        pCVar20 = "TEXT_QST_048_TRADER_BUY_SWEETS"
                        quest:GiveHeroYesNoQuestion(pCVar20, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar11 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while true do
                            uVar6 = uVar9
                            if not (iVar11 < 0) then break end
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                resources:Pause(false)
                                resources:DestroyMovie(book_movie); book_movie = nil
                                goto LAB_00db4f5a
                            end
                            iVar11 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            goto LAB_00db4f5a
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if iVar11 == 1 then
                            if alive then
                                iVar11 = quest:GetHeroGold()
                                if iVar11 < 3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(book_movie); book_movie = nil
                                        goto LAB_00db4f5a
                                    end
                                    book_thing = resources:NewThingFromResource(book_resource)
                                    fVar18 = resources:ThingHealth(book_thing)
                                    resources:DestroyThing(book_thing); book_thing = nil
                                    native_arg_book_has_health = fVar18 > 0.0
                                    if native_arg_book_has_health then
                                        bVar4 = false
                                        pCVar22 = 0x1
                                        pCVar21 = 0x0
                                        pCVar20 = 0x0
                                        pcVar19 = "TEXT_QST_048_TRADER_NOT_ENOUGH_CASH"
                                        pCVar8 = quest:GetHero()
                                        r5 = resources:Speak(book_resource, pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                        bVar4 = resources:IsPerformingScriptTask(book_resource)
                                        if bVar4 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    resources:Pause(false)
                                                    resources:DestroyMovie(book_movie); book_movie = nil
                                                    goto LAB_00db4f5a
                                                end
                                                bVar4 = resources:IsPerformingScriptTask(book_resource)
                                            until not (bVar4)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            -- LAB_00db4bc5: (native jump target)
                                            resources:Pause(false)
                                            resources:DestroyMovie(book_movie); book_movie = nil
                                            goto LAB_00db4f5a
                                        end
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(book_movie); book_movie = nil
                                        goto LAB_00db4f5a
                                    end
                                    book_thing = resources:NewThingFromResource(book_resource)
                                    fVar18 = resources:ThingHealth(book_thing)
                                    resources:DestroyThing(book_thing); book_thing = nil
                                    native_arg_book_has_health = fVar18 > 0.0
                                    if native_arg_book_has_health then
                                        bVar4 = false
                                        pCVar22 = 0x1
                                        pCVar21 = 0x0
                                        pCVar20 = 0x0
                                        pcVar19 = "TEXT_QST_048_TRADER_GIVES_SWEETS"
                                        pCVar8 = quest:GetHero()
                                        r6 = resources:Speak(book_resource, pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                        bVar4 = resources:IsPerformingScriptTask(book_resource)
                                        if bVar4 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    resources:Pause(false)
                                                    resources:DestroyMovie(book_movie); book_movie = nil
                                                    goto LAB_00db4f5a
                                                end
                                                bVar4 = resources:IsPerformingScriptTask(book_resource)
                                            until not (bVar4)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            resources:Pause(false)
                                            resources:DestroyMovie(book_movie); book_movie = nil
                                            goto LAB_00db4f5a
                                        end
                                    end
                                    pCVar20 = "OBJECT_CHOCOLATE_BOX_UNGIVEABLE"
                                    quest:GiveHeroObject(pCVar20, -1)
                                    quest:GiveHeroGold(-3)
                                    ppVar10 = quest:GetActiveQuestName()
                                    quest:SetQuestCardObjective(ppVar10, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_04", "", "")
                                    quest:SetStateBool("GivenSweets", true)
                                    quest:ClearThingHasInformation(me)
                                end
                                goto LAB_00db4c85
                            end
                            -- LAB_00db46da: (native jump target)
                            resources:Pause(false)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            goto LAB_00db4f5a
                        end
                        if not alive then
                            resources:Pause(false)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            goto LAB_00db4f5a
                        end
                        book_thing = resources:NewThingFromResource(book_resource)
                        fVar18 = resources:ThingHealth(book_thing)
                        resources:DestroyThing(book_thing); book_thing = nil
                        native_arg_book_has_health = fVar18 > 0.0
                        if native_arg_book_has_health then
                            bVar4 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_TRADER_BUY_LATER"
                            pCVar8 = quest:GetHero()
                            r7 = resources:Speak(book_resource, pCVar8, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                            bVar4 = resources:IsPerformingScriptTask(book_resource)
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        resources:Pause(false)
                                        resources:DestroyMovie(book_movie); book_movie = nil
                                        goto LAB_00db4f5a
                                    end
                                    bVar4 = resources:IsPerformingScriptTask(book_resource)
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then goto LAB_00db4c85 end
                            -- LAB_00db4f3f: (native jump target)
                            resources:Pause(false)
                            resources:DestroyMovie(book_movie); book_movie = nil
                            goto LAB_00db4f5a
                        end
                        ::LAB_00db4c85::
                        quest:FaceThingByScriptName(me, "NOVI_Theresa", false)
                        resources:Pause(false)
                        resources:DestroyMovie(book_movie); book_movie = nil
                        goto LAB_00db4ce6
                    end
                end
                break
            end
            ::LAB_00db4ce6::
            iVar11 = quest:GetTimer(quest:GetStateInt("TalkIntermittentTimer"))
            __native_condition_1 = iVar11 == 0
            if __native_condition_1 then
                iVar11 = quest:RetailRandModulo(200)
                __native_condition_1 = iVar11 == 0
            end
            if __native_condition_1 then
                fVar23 = 20.0
                pCVar8 = quest:GetHero()
                bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar23)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 3)
                        uVar9 = quest:StartConversationWithHero(me, false, false)
                        resources:PrepareResource(book_resource)
                        cVar5 = resources:TryAcquire(book_resource, me, 4)
                        while not cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db4f5a end
                            cVar5 = resources:TryAcquire(book_resource, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            resources:PlayAnimation(book_resource, "ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED", false, false, false, true, resources:ReadAnimationArgument5(), false)
                            quest:AddConversationLineToHero(uVar9, "TEXT_QST_048_TRADER_ROLL_UP", me, false)
                            goto LAB_00db4e64
                        end
                    end
                    break
                end
            end
            ::LAB_00db4e64::
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            cVar5 = not alive
        end
        ::LAB_00db4f5a::
        release_book()
    end
end


function Main(quest, me)
    quest:WithRetailResources(function(resources)
        __resource_main(quest, me, resources)
    end)
end
