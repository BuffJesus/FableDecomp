-- Readable native conversion: IngredientOwner. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local boughtSpecialStuff, stoleSpecialStuff

-- IngredientOwner.Main (retail 0x00ecb0e0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local questionAnswer, sequence, movie, movie2, movie4
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local ingredient = quest:GetThingWithScriptName("Ingredient")
    if not boughtSpecialStuff then
        if not quest:IsActiveThreadTerminating() then quest:SetThingHasInformation(me, false, true, false); goto LAB_00ecb222 end
    elseif not quest:IsActiveThreadTerminating() then
        quest:RemoveThing(ingredient, (0 + 4) ~= 0, false)
        goto LAB_00ecb222
    end
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00ecb222::
    quest:SetIsPushableByHero(me, false)
    quest:SetIsThingForcePushable(me, false)
    local timerId = quest:RegisterTimer()
    while not quest:IsActiveThreadTerminating() do
        if me:IsTalkedToByHero() then
            if not quest:IsActiveThreadTerminating() then
                if not boughtSpecialStuff then
                    local movie6 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                    if fret_0 <= 0.0 then
                        goto LAB_00ecb3b6
                    else
                        if not me:Speak(hero, "TEXT_QST_B10_TRADER_INTRO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00ecbbc9 end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00ecb3b6 end
                    end
                    goto FLOW_past_lab_00ecb3b6
                    ::LAB_00ecb3b6::
                    quest:GiveHeroYesNoQuestion("TEXT_QST_B10_TRADER_BUY_MUSHROOM_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00ecbbc9 end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        if questionAnswer ~= 1 then goto LAB_00ecb6d9 end
                        if quest:GetHeroGold() < 1500 then
                            if 1499 < quest:GetHeroGold() then goto LAB_00ecb6d9 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00ecb6c3 end
                            local fret_01 = quest:GetHealth(resources:ScriptThing(resource))
                            if 0.0 < fret_01 then
                                if not me:Speak(hero, "TEXT_QST_B10_TRADER_NOT_ENOUGH_MONEY_10", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00ecbbc9 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00ecb6c3 end
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00ecbbc9 end
                            quest:GiveHeroObject(quest:ReadGlobalGameDataString(1848), -1, false)
                            quest:RemoveThing(1, false, false)
                            quest:GiveHeroGold(-1500)
                            quest:EntityGiveGold(me, 1500)
                            local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
                            if 0.0 < fret_00 then
                                if not me:Speak(hero, "TEXT_QST_B10_TRADER_BOUGHT_MUSHROOM_10", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00ecb6c3 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00ecbbc9 end
                            end
                            boughtSpecialStuff = true
                            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(1904))
                            quest:ClearThingHasInformation(me)
                        end
                        goto FLOW_past_lab_00ecb6d9
                        ::LAB_00ecb6d9::
                        if quest:IsActiveThreadTerminating() then goto LAB_00ecbbc9 end
                        goto FLOW_hoist_lab_00ecbbc9_1
                        ::FLOW_past_lab_00ecb6d9::
                        goto FLOW_hoist_lab_00ecbbc9_2
                    end
                    goto FLOW_past_lab_00ecbbc9
                    ::LAB_00ecbbc9::
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie6
                    goto LAB_00ecbc48
                    ::FLOW_hoist_lab_00ecbbc9_1::
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_B10_TRADER_NOT_BOUGHT_MUSHROOM_10", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00ecb6c3 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00ecbbc9 end
                    end
                    ::FLOW_hoist_lab_00ecbbc9_2::
                    quest:PauseAllNonScriptedEntities(false)
                    movie2 = movie6
                    goto LAB_00ecb9e6
                    ::FLOW_past_lab_00ecbbc9::
                    ::FLOW_past_lab_00ecb3b6::
                    ::LAB_00ecb6c3::
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie6
                else
                    if stoleSpecialStuff then
                        local movie3 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local fret_04 = quest:GetHealth(resources:ScriptThing(resource))
                        if 0.0 < fret_04 then
                            me:Speak(hero, "TEXT_QST_B10_TRADER_POST_STOLE_MUSHROOM_10", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    movie = movie3
                                    goto LAB_00ecbc48
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie3
                                goto LAB_00ecbc48
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        movie2 = movie3
                        goto LAB_00ecb9e6
                    end
                    goto FLOW_hoist_lab_00ecb9e6_1
                end
                goto FLOW_past_lab_00ecb9e6
                ::LAB_00ecb9e6::
                resources:DestroyMovie(movie2)
                goto LAB_00ecb9eb
                ::FLOW_hoist_lab_00ecb9e6_1::
                movie4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then
                    goto LAB_00ecb8bb
                end
                goto FLOW_past_lab_00ecb8bb
                ::LAB_00ecb8bb::
                quest:PauseAllNonScriptedEntities(false)
                movie2 = movie4
                goto LAB_00ecb9e6
                ::FLOW_past_lab_00ecb8bb::
                me:Speak(hero, "TEXT_QST_B10_TRADER_POST_BOUGHT_MUSHROOM_10", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie4
                        goto LAB_00ecbc48
                    end
                end
                if not quest:IsActiveThreadTerminating() then goto LAB_00ecb8bb end
                quest:PauseAllNonScriptedEntities(false)
                movie = movie4
                ::FLOW_past_lab_00ecb9e6::
                goto LAB_00ecbc48
            end
            goto FLOW_hoist_lab_00ecbc48_1
        end
        goto FLOW_past_lab_00ecbc48
        ::LAB_00ecbc48::
        resources:DestroyMovie(movie)
        goto LAB_00ecbc4d
        ::FLOW_hoist_lab_00ecbc48_1::
        goto FLOW_hoist_lab_00ecbc4d_1
        ::FLOW_past_lab_00ecbc48::
        goto FLOW_past_lab_00ecbc4d
        ::LAB_00ecbc4d::
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
        do return end
        ::FLOW_hoist_lab_00ecbc4d_1::
        break
        ::FLOW_past_lab_00ecbc4d::
        ::LAB_00ecb9eb::
        sequence = not boughtSpecialStuff
        if sequence then
            -- TODO(native): cVar4 = (**(CStack_d8._0_4_ + 0x12c))()
    --[[unresolved native value]]
            sequence = not nil
        end
        if sequence then
            if quest:IsActiveThreadTerminating() then goto LAB_00ecbc4d end
            local movie5 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local fret_05 = quest:GetHealth(resources:ScriptThing(resource))
            if 0.0 < fret_05 then
                me:Speak(hero, "TEXT_QST_B10_TRADER_STOLE_MUSHROOM_10", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecbc44 end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecbc44 end
                goto FLOW_past_lab_00ecbc44
                ::LAB_00ecbc44::
                movie = movie5
                goto LAB_00ecbc48
                ::FLOW_past_lab_00ecbc44::
            end
            boughtSpecialStuff = true
            stoleSpecialStuff = true
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(1900))
            quest:ClearThingHasInformation(me)
            quest:AddCrimeCommitted(quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS"), 6, true, nil --[[missing]], nil --[[missing]], 0)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie5)
        end
        quest:NewScriptFrame(me)
    end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
end

-- IngredientOwner.Init (retail 0x00ecb050)
function Init(quest, me)
    boughtSpecialStuff = false
    stoleSpecialStuff = false
    if not quest:GetStateBool("OwnerAlive") then
        quest:RemoveThing(me, false, true)
    end
end

-- IngredientOwner.OnPersist (retail 0x00ecd960)
function OnPersist(quest, me, context)
    quest:SetStateBool("BoughtSpecialStuff", quest:PersistTransferBool(context, "BoughtSpecialStuff", quest:GetStateBool("BoughtSpecialStuff")))
    quest:SetStateBool("StoleSpecialStuff", quest:PersistTransferBool(context, "StoleSpecialStuff", quest:GetStateBool("StoleSpecialStuff")))
end

-- IngredientOwner.OnPredicateFail (retail 0x00ecb080)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") then
        quest:SetStateBool("OwnerAlive", false)
    end
end

