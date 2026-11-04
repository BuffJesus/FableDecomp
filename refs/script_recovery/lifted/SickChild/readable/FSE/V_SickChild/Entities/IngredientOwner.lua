-- Readable native conversion: IngredientOwner. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    IngredientObject = 1848,  -- 'OBJECT_MUSHROOM_BLUE_01'
    StoleMushroomInBarrowFields = 1900,  -- -0.019999999552965164
    BoughtMushroomInBarrowFields = 1904,  -- 0.019999999552965164
}

-- per-entity fields (native class members; one Lua state per entity instance)
local boughtSpecialStuff_, stoleSpecialStuff_

-- IngredientOwner.Main (retail 0x00ecb0e0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local questionAnswer, scratchValue, getHero, movie, movie2, movie4, movie5
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local ingredient = quest:GetThingWithScriptName("Ingredient")
    if not boughtSpecialStuff_ then
        if not quest:IsActiveThreadTerminating() then
            getHero = 0
            quest:SetThingHasInformation(me, false, true, false)
            goto LAB_00ecb222
        end
    elseif not quest:IsActiveThreadTerminating() then
        getHero = 0 + 4
        quest:RemoveThing(ingredient, false, true)
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
                if not boughtSpecialStuff_ then
                    if quest:IsActiveThreadTerminating() then goto LAB_00ecbc4d end
                    local movie6 = resources:StartMovie("")
                    scratchValue = 1
                    quest:PauseAllNonScriptedEntities(true)
                    getHero = nil
                    if not resources:ResourceThingHealthAbove(resource, 0.0) then
                        goto LAB_00ecb3b6
                    else
                        getHero = quest:GetHero()
                        if not me:Speak(getHero, "TEXT_QST_B10_TRADER_INTRO", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00ecbbc9
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00ecbbc9 end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00ecb3b6 end
                    end
                    goto FLOW_past_lab_00ecbbc9
                    ::LAB_00ecb3b6::
                    quest:GiveHeroYesNoQuestion("TEXT_QST_B10_TRADER_BUY_MUSHROOM_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00ecbbc9 end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        if not (questionAnswer ~= 1) then
                            if quest:GetHeroGold() < 1500 then
                                if 1499 < quest:GetHeroGold() then goto LAB_00ecb6d9 end
                                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00ecbbc9 end
                                getHero = nil
                                if resources:ResourceThingHealthAbove(resource, 0.0) then
                                    scratchValue = quest:GetHero()
                                    if not me:Speak(scratchValue, "TEXT_QST_B10_TRADER_NOT_ENOUGH_MONEY_10", GROUP_SELECT_FIRST, false, true, false) then
                                        goto LAB_00ecbbc9
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00ecbbc9 end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00ecbbc9 end
                                end
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00ecbbc9 end
                                local scratchValue3 = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                                quest:GiveHeroObject(scratchValue3, -1, false)
                                quest:RemoveThing(ingredient, false, true)
                                quest:GiveHeroGold(-1500)
                                quest:EntityGiveGold(me, 1500)
                                getHero = nil
                                if resources:ResourceThingHealthAbove(resource, 0.0) then
                                    scratchValue = quest:GetHero()
                                    if not me:Speak(scratchValue, "TEXT_QST_B10_TRADER_BOUGHT_MUSHROOM_10", GROUP_SELECT_FIRST, false, true, false) then
                                        goto FLOW_past_lab_00ecbbc9
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00ecbbc9 end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00ecbbc9 end
                                end
                                boughtSpecialStuff_ = true
                                quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BoughtMushroomInBarrowFields))
                                quest:ClearThingHasInformation(me)
                            end
                            goto FLOW_past_lab_00ecb6d9
                        end
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
                    getHero = nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        scratchValue = quest:GetHero()
                        if not me:Speak(scratchValue, "TEXT_QST_B10_TRADER_NOT_BOUGHT_MUSHROOM_10", GROUP_SELECT_FIRST, false, true, false) then
                            goto FLOW_past_lab_00ecbbc9
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00ecbbc9 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00ecbbc9 end
                    end
                    ::FLOW_hoist_lab_00ecbbc9_2::
                    quest:PauseAllNonScriptedEntities(false)
                    movie2 = movie6
                    goto LAB_00ecb9e6
                    ::FLOW_past_lab_00ecbbc9::
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie6
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00ecbc4d end
                    if stoleSpecialStuff_ then
                        if quest:IsActiveThreadTerminating() then goto LAB_00ecbc4d end
                        local movie3 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        scratchValue = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            scratchValue = quest:GetHero()
                            if not me:Speak(scratchValue, "TEXT_QST_B10_TRADER_POST_STOLE_MUSHROOM_10", GROUP_SELECT_FIRST, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie3
                                goto LAB_00ecbc48
                            end
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
                goto FLOW_past_lab_00ecbc4d
                ::FLOW_hoist_lab_00ecb9e6_1::
                if quest:IsActiveThreadTerminating() then goto LAB_00ecbc4d end
                movie4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                scratchValue = nil
                if not (not resources:ResourceThingHealthAbove(resource, 0.0)) then
                    goto FLOW_past_lab_00ecb8bb
                end
                ::LAB_00ecb8bb::
                quest:PauseAllNonScriptedEntities(false)
                movie2 = movie4
                goto LAB_00ecb9e6
                ::FLOW_past_lab_00ecb8bb::
                scratchValue = quest:GetHero()
                if not me:Speak(scratchValue, "TEXT_QST_B10_TRADER_POST_BOUGHT_MUSHROOM_10", GROUP_SELECT_FIRST, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie4
                    goto LAB_00ecbc48
                end
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
        if boughtSpecialStuff_ or (ingredient ~= nil and ingredient:IsAlive()) then quest:NewScriptFrame(me); goto continue_3 end
        if quest:IsActiveThreadTerminating() then goto LAB_00ecbc4d end
        movie5 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        scratchValue = nil
        if resources:ResourceThingHealthAbove(resource, 0.0) then
            local hero = quest:GetHero()
            if not me:Speak(hero, "TEXT_QST_B10_TRADER_STOLE_MUSHROOM_10", GROUP_SELECT_FIRST, false, true, false) then
                quest:PauseAllNonScriptedEntities(false)
            else
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecbc44 end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecbc44 end
                goto FLOW_past_lab_00ecbc44
            end
            ::LAB_00ecbc44::
            movie = movie5
            goto LAB_00ecbc48
            ::FLOW_past_lab_00ecbc44::
        end
        boughtSpecialStuff_ = true
        stoleSpecialStuff_ = true
        quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.StoleMushroomInBarrowFields))
        quest:ClearThingHasInformation(me)
        getHero = 0
        scratchValue = quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS")
        quest:AddCrimeCommitted(scratchValue, 6, true, nil, nil, 7)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie5)
        quest:NewScriptFrame(me)
        ::continue_3::
    end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
end

-- IngredientOwner.Init (retail 0x00ecb050)
function Init(quest, me)
    boughtSpecialStuff_ = false
    stoleSpecialStuff_ = false
    if not quest:GetStateBool("OwnerAlive") then
        quest:RemoveThing(me, false, true)
    end
end

-- IngredientOwner.OnPersist (retail 0x00ecd960)
function OnPersist(quest, me, context)
    local boughtSpecialStuff = boughtSpecialStuff_ or false
    boughtSpecialStuff_ = quest:PersistTransferBool(context, "BoughtSpecialStuff", boughtSpecialStuff)
    local stoleSpecialStuff = stoleSpecialStuff_ or false
    stoleSpecialStuff_ = quest:PersistTransferBool(context, "StoleSpecialStuff", stoleSpecialStuff)
end

-- IngredientOwner.OnPredicateFail (retail 0x00ecb080)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") then
        quest:SetStateBool("OwnerAlive", false)
    end
end

