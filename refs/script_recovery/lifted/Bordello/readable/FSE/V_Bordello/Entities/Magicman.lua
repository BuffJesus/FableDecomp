-- Readable native conversion: Magicman. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("V_Bordello.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local doneToutIntro, mentionedPimpHat

-- Magicman.Main (retail 0x00e40e80)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local __push19, __push5, isActiveThreadTerminating, predicateResult, scratchValue19, timerId2
    local i_stk_350_1, timerId3, switch1, switch, p0, getHero, this_01, timerId, scratchValue25
    local scratchValue26, movie, movie4, startMovie, movie5, movie6, resource, scriptThing
    if not quest:NewScriptFrame(me) then return end
    local resource6 = resources:NewResource()
    resources:PrepareResource(resource6)
    while not resources:TryAcquire(resource6, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e44528 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e44528 end
    resources:AssignResource(resources:MemberResource("seh_Boss"), resource6)
    timerId3 = quest:RegisterTimer()
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 40)
    i_stk_350_1 = 1
    while not quest:GetStateBool("PlayerOwned") do
        if not quest:NewScriptFrame(me) then goto LAB_00e4450d end
        while not quest:GetStateBool("HeroTricking") and not quest:GetStateBool("PlayerOwned") do
            if not quest:NewScriptFrame(me) then goto LAB_00e448a1 end
            if (me ~= nil and me:IsDistanceFromPositionOver(me:GetHomePos(), 2.0)) and not me:IsPerformingScriptTask() then
                me:MoveToPosition(me:GetHomePos(), 1.0, ENTITY_MOVE_WALK, false, true)
            end
            if quest:GetStateBool("BeerSetToDPad") then goto LAB_00e410ba end
            scratchValue26 = scratchValue26 | 1
            timerId2 = quest:GetHeroTargetedThing()
            isActiveThreadTerminating = true
            if not (timerId2 ~= nil and timerId2:IsEqualTo(me)) then goto LAB_00e410ba end
            goto FLOW_past_lab_00e410ba
            ::LAB_00e410ba::
            isActiveThreadTerminating = false
            ::FLOW_past_lab_00e410ba::
            if scratchValue26 & 1 ~= 0 then
                scratchValue26 = scratchValue26 & 0xfffffffe
            end
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                quest:SetStateBool("BeerSetToDPad", true)
                quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", 2, 0xf4240)
                quest:CreateThread("WatchForHeroLeavingRegionWithBeer")  -- native thread body 0x00E44980: lift it as function WatchForHeroLeavingRegionWithBeer(quest)
                if scratchValue26 & 8 ~= 0 then
                    scratchValue26 = scratchValue26 & 0xfffffff7
                end
                if scratchValue26 & 4 ~= 0 then
                    scratchValue26 = scratchValue26 & 0xfffffffb
                end
                if scratchValue26 & 2 ~= 0 then
                    scratchValue26 = scratchValue26 & 0xfffffffd
                end
            else
                if not quest:GetStateBool("BeerSetToDPad") then
                    goto LAB_00e412bc
                else
                    scratchValue26 = scratchValue26 | 16
                    timerId2 = quest:GetHeroTargetedThing()
                    isActiveThreadTerminating = true
                    if timerId2 ~= nil and timerId2:IsEqualTo(me) then goto LAB_00e412bc end
                end
                goto FLOW_past_lab_00e412bc
                ::LAB_00e412bc::
                isActiveThreadTerminating = false
                ::FLOW_past_lab_00e412bc::
                if scratchValue26 & 16 ~= 0 then
                    scratchValue26 = scratchValue26 & 0xffffffef
                end
                if isActiveThreadTerminating then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                    quest:SetStateBool("BeerSetToDPad", false)
                end
            end
            timerId2 = timerId3
            if quest:GetTimer(timerId3) == 0 then
                scratchValue19 = 8.0
                getHero = hero
                if quest:IsDistanceBetweenThingsUnder(me, hero, 8.0) then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId3)
                        goto LAB_00e4451f
                    end
                    quest:SetTimer(timerId3, 4)
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                end
            end
            if quest:GetTimer(timerId) == 0 then
                scratchValue19 = 15.0
                getHero = hero
                if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId3)
                        goto LAB_00e4451f
                    end
                    if not quest:TextEntryExists("TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(i_stk_350_1)) then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(timerId3)
                            goto LAB_00e4451f
                        end
                    end
                    quest:AddPersonToConversation(quest:AddNewConversation(me, false, false), hero)
                    -- TODO(native): xStack_308 = (undefined **)**(int **)(this + 4);
                    -- TODO(native): xStack_344 = (int *)(*(code *)xStack_308[0x46])();
                    __push5 = "TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(resource6)
                    -- TODO(native): (*(code *)xStack_308[0x16e])(__push3,__push5,0,(this + 8),__unknown_push);
                    i_stk_350_1 = resource6 + 1
                    quest:SetTimer(timerId, 30)
                end
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                me:ClearCommands()
                local movie7 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:Pause(1.0)
                quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(me, getHero, -1.0, 0, -1)
                if helpers.IsHeroWearingBeard(quest, me) then
                    if not quest:IsActiveThreadTerminating() then
                        scriptThing = nil
                        if 0.0 < quest:GetHealth(resources:ScriptThing(p0)) then
                            me:Speak(hero, "TEXT_QST_B13_MAGICMAN_PIMPING", GROUP_SELECT_FIRST, false, true, false)
                            timerId2 = me:IsPerformingScriptTask()
                            while timerId2 do
                                quest:NewScriptFrame(me)
                                if not quest:IsActiveThreadTerminating() then
                                    timerId2 = me:IsPerformingScriptTask()
                                else
                                    quest:PauseAllNonScriptedEntities(false)
                                    quest:DeregisterTimer(me)
                                    quest:DeregisterTimer(hero)
                                    do return end
                                    timerId2 = me:IsPerformingScriptTask()
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie7)
                                goto LAB_00e448a1
                            end
                        end
                        goto LAB_00e41b75
                    end
                    goto LAB_00e445a0
                end
                if not doneToutIntro then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e445a0 end
                    doneToutIntro = true
                    if 0.0 < quest:GetHealth(resources:ScriptThing(p0)) then
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_TOUTING", GROUP_SELECT_FIRST, false, true, false)
                        timerId2 = me:IsPerformingScriptTask()
                        while timerId2 do
                            if not quest:NewScriptFrame(me) then goto LAB_00e44615 end
                            timerId2 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e445a0 end
                    end
                    goto LAB_00e41b75
                end
                goto FLOW_past_lab_00e41b75
                ::LAB_00e41b75::
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie7)
                goto LAB_00e41ba8
                ::FLOW_past_lab_00e41b75::
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie7)
                    goto LAB_00e448a1
                end
                if mentionedPimpHat then goto LAB_00e418e3 end
                -- TODO(native): xStack_350 = (CScriptThing *)((uint)xStack_350 | 0x20);
                predicateResult = true
                if not quest:IsWearingClothingItem(hero, "OBJECT_HERO_HAT_PIMP") then goto LAB_00e418e3 end
                goto FLOW_past_lab_00e418e3
                ::LAB_00e418e3::
                predicateResult = false
                ::FLOW_past_lab_00e418e3::
                if p0 & 32 ~= 0 then
                    -- TODO(native): xStack_350 = (CScriptThing *)((uint)xStack_350 & 0xffffffdf);
                end
                if not predicateResult then
                    if not quest:IsActiveThreadTerminating() then
                        startMovie = "TEXT_QST_B13_MAGICMAN_CHATTER_0" .. tostring(1)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(1)) then
                            me:Speak(hero, startMovie, 0, false, true, false)
                            timerId2 = me:IsPerformingScriptTask()
                            while timerId2 do
                                quest:NewScriptFrame(me)
                                if not quest:IsActiveThreadTerminating() then
                                    timerId2 = me:IsPerformingScriptTask()
                                else
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00e44862
                                    timerId2 = me:IsPerformingScriptTask()
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie7)
                                goto LAB_00e448a1
                            end
                        end
                        -- TODO(native): xStack_364 = xStack_364 + 1;
                        goto LAB_00e41b75
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie7)
                    goto LAB_00e448a1
                end
                if not quest:IsActiveThreadTerminating() then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource6)) then
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_PIMP_HAT", GROUP_SELECT_FIRST, false, true, false)
                        timerId2 = me:IsPerformingScriptTask()
                        while timerId2 do
                            if not quest:NewScriptFrame(me) then goto LAB_00e44615 end
                            timerId2 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e445a0 end
                    end
                    mentionedPimpHat = true
                    goto LAB_00e41b75
                end
                ::LAB_00e445a0::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie7)
                goto LAB_00e44862
            end
            ::LAB_00e41ba8::
            scratchValue25 = scratchValue26
            scratchValue26 = scratchValue26 | 64
            if me:MsgIsHitByHero() then goto LAB_00e41c4b end
            scratchValue26 = scratchValue25 | 192
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue26 = scratchValue25 | 448
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e41c4b end
            end
            isActiveThreadTerminating = false
            goto FLOW_past_lab_00e41c4b
            ::LAB_00e41c4b::
            isActiveThreadTerminating = true
            ::FLOW_past_lab_00e41c4b::
            if scratchValue26 & 256 ~= 0 then
                scratchValue26 = scratchValue26 & 0xfffffeff
            end
            if scratchValue26 < 0 then
                scratchValue26 = scratchValue26 & 0xffffff7f
            end
            if scratchValue26 & 64 ~= 0 then
                scratchValue26 = scratchValue26 & 0xffffffbf
            end
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                me:ClearCommands()
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:Pause(1.0)
                quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                quest:NewScriptFrame(me)
                timerId2 = -1
                quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0, -1)
                if helpers.IsHeroWearingBeard(quest, me) then
                    if not quest:IsActiveThreadTerminating() then
                        scriptThing = 0
                        if 0.0 >= quest:GetHealth(resources:ScriptThing(p0)) then
                            goto LAB_00e42003
                        end
                        goto FLOW_past_lab_00e42003
                        ::LAB_00e42003::
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e4203f
                        ::FLOW_past_lab_00e42003::
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_ATTACKED_FEMALE", GROUP_SELECT_FIRST, false, true, false)
                        timerId2 = me:IsPerformingScriptTask()
                        while timerId2 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                timerId2 = me:IsPerformingScriptTask()
                            else
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                quest:DeregisterTimer(me)
                                quest:DeregisterTimer(hero)
                                do return end
                                timerId2 = me:IsPerformingScriptTask()
                            end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00e42003 end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(this_01)
                        goto LAB_00e448a1
                    end
                elseif not quest:IsActiveThreadTerminating() then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(p0)) then
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_ATTACKED_MALE", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                quest:DeregisterTimer(-1)
                                quest:DeregisterTimer(me)
                                do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            goto LAB_00e448a1
                        end
                    end
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00e4203f
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                goto LAB_00e44862
            end
            ::LAB_00e4203f::
            if not me:MsgIsPresentedWithItem() then goto continue_5 end
            if unaff_EBP ~= nil then
                -- TODO(native): iVar21 = CBasicString<char>::Compare((void *)*unaff_EBP,"OBJECT_BEER_TANKARD");
                if timerId2 == 0 then goto LAB_00e42094 end
            end
            goto FLOW_past_lab_00e42094
            ::LAB_00e42094::
            if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
            me:ClearCommands()
            movie6 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            quest:Pause(1.0)
            quest:EntitySetFacingAngleTowardsThing(me, hero, true)
            quest:NewScriptFrame(me)
            quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0, -1)
            switch1 = quest:GetStateInt("BeersDrunk")
            repeat
                if switch1 == 0 then
                    getHero = resources:ScriptThing(p0)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER1" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(0x126008c)
                            resources:DestroyMovie(movie6)
                            goto LAB_00e448a1
                        end
                    end
                    break
                elseif switch1 == 1 then
                    getHero = resources:ScriptThing(p0)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER2" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(0x126008c)
                            resources:DestroyMovie(movie6)
                            goto LAB_00e448a1
                        end
                    end
                    break
                elseif switch1 == 2 then
                    getHero = resources:ScriptThing(p0)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER3" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(0x126008c)
                            resources:DestroyMovie(movie6)
                            goto LAB_00e448a1
                        end
                    end
                    break
                elseif switch1 == 3 then
                    getHero = resources:ScriptThing(p0)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER4" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(0x126008c)
                            resources:DestroyMovie(movie6)
                            goto LAB_00e448a1
                        end
                    end
                    break
                elseif switch1 == 4 then
                    getHero = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER5" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(0x126008c)
                            resources:DestroyMovie(movie6)
                            goto LAB_00e448a1
                        end
                    end
                    break
                elseif switch1 == 5 then
                    getHero = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER6" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(0x126008c)
                            resources:DestroyMovie(movie6)
                            goto LAB_00e448a1
                        end
                    end
                    if not quest:GetStateBool("HeroFoundDeedsLocation") then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(0x126008c)
                            resources:DestroyMovie(movie6)
                            goto LAB_00e448a1
                        end
                        helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANDRUNK", false)
                        quest:SetStateBool("HeroFoundDeedsLocation", true)
                        quest:SetStateBool("HeroPartying", true)
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie6)
                            goto LAB_00e44862
                        end
                        helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANDRUNK_QUICKIE", false)
                        quest:SetStateBool("HeroPartying", true)
                    end
                    break
                elseif 0.0 >= quest:GetHealth(resources:ScriptThing(resource)) then
                    quest:GiveHeroObject("OBJECT_BEER_TANKARD", 0)
                else
                    me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEERTOOMUCH", GROUP_SELECT_FIRST, false, true, false)
                    if me:IsPerformingScriptTask() then
                        quest:NewScriptFrame(me)
                        if not quest:IsActiveThreadTerminating() then goto LAB_00e428c7 end
                        quest:PauseAllNonScriptedEntities(0x126008c)
                        goto LAB_00e44862
                    end
                    -- LAB_00e428d4: (native jump target)
                    if not quest:IsActiveThreadTerminating() then
                        quest:GiveHeroObject("OBJECT_BEER_TANKARD", 0)
                    else
                        quest:PauseAllNonScriptedEntities(0x126008c)
                        resources:DestroyMovie(movie6)
                        goto LAB_00e448a1
                        quest:GiveHeroObject("OBJECT_BEER_TANKARD", 0)
                    end
                end
            until true
            quest:SetStateInt("BeersDrunk", quest:GetStateInt("BeersDrunk") + 1)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie5)
            ::FLOW_past_lab_00e42094::
            ::continue_5::
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
        while not quest:GetStateBool("PlayerOwned") do
            if not quest:NewScriptFrame(me) then goto LAB_00e4450d end
            if (me ~= nil and me:IsDistanceFromPositionOver(me:GetHomePos(), 2.0)) and not me:IsPerformingScriptTask() then
                me:MoveToPosition(me:GetHomePos(), 1.0, ENTITY_MOVE_WALK, false, true)
            end
            if quest:GetStateBool("BeerSetToDPad") then goto LAB_00e42a3a end
            scratchValue26 = scratchValue26 | 512
            timerId2 = quest:GetHeroTargetedThing()
            isActiveThreadTerminating = true
            if not (timerId2 ~= nil and timerId2:IsEqualTo(me)) then goto LAB_00e42a3a end
            goto FLOW_past_lab_00e42a3a
            ::LAB_00e42a3a::
            isActiveThreadTerminating = false
            ::FLOW_past_lab_00e42a3a::
            if scratchValue26 & 512 ~= 0 then
                scratchValue26 = scratchValue26 & 0xfffffdff
            end
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                quest:SetStateBool("BeerSetToDPad", true)
                quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", 2, 0xf4240)
                quest:CreateThread("WatchForHeroLeavingRegionWithBeer")  -- native thread body 0x00E44980: lift it as function WatchForHeroLeavingRegionWithBeer(quest)
                if scratchValue26 & 4096 ~= 0 then
                    scratchValue26 = scratchValue26 & 0xffffefff
                end
                if scratchValue26 & 2048 ~= 0 then
                    scratchValue26 = scratchValue26 & 0xfffff7ff
                end
                if scratchValue26 & 1024 ~= 0 then
                    scratchValue26 = scratchValue26 & 0xfffffbff
                end
            else
                if not quest:GetStateBool("BeerSetToDPad") then
                    goto LAB_00e42bf9
                else
                    scratchValue26 = scratchValue26 | 0x2000
                    timerId2 = quest:GetHeroTargetedThing()
                    isActiveThreadTerminating = true
                    if timerId2 ~= nil and timerId2:IsEqualTo(me) then goto LAB_00e42bf9 end
                end
                goto FLOW_past_lab_00e42bf9
                ::LAB_00e42bf9::
                isActiveThreadTerminating = false
                ::FLOW_past_lab_00e42bf9::
                if scratchValue26 & 0x2000 ~= 0 then
                    scratchValue26 = scratchValue26 & 0xffffdfff
                end
                if isActiveThreadTerminating then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                    quest:SetStateBool("BeerSetToDPad", false)
                end
            end
            if quest:GetTimer(timerId3) == 0 then
                scratchValue19 = 8.0
                getHero = hero
                if quest:IsDistanceBetweenThingsUnder(me, hero, 8.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                    quest:SetTimer(timerId3, 4)
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                end
            end
            timerId2 = quest:GetTimer(timerId)
            if timerId2 == 0 then
                scratchValue19 = 15.0
                getHero = hero
                if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                    if not quest:TextEntryExists("TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(i_stk_350_1)) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                    end
                    quest:AddPersonToConversation(quest:AddNewConversation(me, false, false), hero)
                    -- TODO(native): xStack_2bc = (undefined **)**(int **)(this + 4);
                    -- TODO(native): xStack_344 = (int *)(*(code *)xStack_2bc[0x46])();
                    resource = p0
                    __push19 = "TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(resource6)
                    -- TODO(native): (*(code *)xStack_2bc[0x16e])(__push17,__push19,0,(this + 8),__unknown_push);
                    i_stk_350_1 = resource6 + 1
                    quest:SetTimer(timerId, 30)
                end
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                me:ClearCommands()
                resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resource = p0
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:Pause(1.0)
                quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                quest:NewScriptFrame(me)
                getHero = "CAM_B_OWNER"
                quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0, -1)
                if helpers.IsHeroWearingBeard(quest, me) then
                    if not quest:IsActiveThreadTerminating() then
                        if helpers.helper_E44A40(quest, me) then
                            if not quest:IsActiveThreadTerminating() then
                                if 0.0 < quest:GetHealth(resources:ScriptThing(p0)) then
                                    me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEARD_LADY", GROUP_SELECT_FIRST, false, true, false)
                                    timerId2 = me:IsPerformingScriptTask()
                                    while timerId2 do
                                        if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                        timerId2 = me:IsPerformingScriptTask()
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie6)
                                        goto LAB_00e4450d
                                    end
                                end
                                goto LAB_00e437b3
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie6)
                            goto LAB_00e4450d
                        end
                        if not quest:IsActiveThreadTerminating() then
                            if helpers.IsHeroWearingTash(quest, me) then
                                if not quest:IsActiveThreadTerminating() then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(hero)) then
                                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEARD_LADY_MOUSTACHE", GROUP_SELECT_FIRST, false, true, false)
                                        timerId2 = me:IsPerformingScriptTask()
                                        while timerId2 do
                                            if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                            timerId2 = me:IsPerformingScriptTask()
                                        end
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(movie6)
                                            goto LAB_00e4450d
                                        end
                                    end
                                    goto LAB_00e437b3
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie6)
                                goto LAB_00e4450d
                            end
                            if not quest:IsActiveThreadTerminating() then
                                if 0.0 < quest:GetHealth(resources:ScriptThing(p0)) then
                                    me:Speak(me, "TEXT_QST_B13_MAGICMAN_LOVELY_SKIN", GROUP_SELECT_FIRST, false, true, false)
                                    timerId2 = me:IsPerformingScriptTask()
                                    while timerId2 do
                                        quest:NewScriptFrame(me)
                                        if not quest:IsActiveThreadTerminating() then
                                            timerId2 = me:IsPerformingScriptTask()
                                        else
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(movie6)
                                            goto LAB_00e4450d
                                            timerId2 = me:IsPerformingScriptTask()
                                        end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e448cc end
                                end
                                if quest:IsWearingClothingItem(hero, "OBJECT_HERO_HAT_WHOREWIG") then
                                    if not quest:IsActiveThreadTerminating() then
                                        if 0.0 < quest:GetHealth(resources:ScriptThing(hero)) then
                                            if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_LOVELY_HAIR", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e448cc end
                                            if quest:IsActiveThreadTerminating() then
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyMovie(movie6)
                                                goto LAB_00e4450d
                                            end
                                        end
                                        quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MAGICMAN_PARTY", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                                        timerId2 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        while timerId2 < 0 do
                                            if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                            timerId2 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                                            if timerId2 == 1 then
                                                if isActiveThreadTerminating then goto LAB_00e448cc end
                                                if 0.0 < quest:GetHealth(resources:ScriptThing(movie6)) then
                                                    me:Speak(hero, "TEXT_QST_B13_MAGICMAN_HORNY", GROUP_SELECT_FIRST, false, true, false)
                                                    timerId2 = me:IsPerformingScriptTask()
                                                    while timerId2 do
                                                        quest:NewScriptFrame(me)
                                                        if not quest:IsActiveThreadTerminating() then
                                                            timerId2 = me:IsPerformingScriptTask()
                                                        else
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            resources:DestroyMovie(movie6)
                                                            goto LAB_00e4450d
                                                            timerId2 = me:IsPerformingScriptTask()
                                                        end
                                                    end
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00e448cc end
                                                end
                                                quest:GiveHeroGold(1000)
                                                quest:SetNumberOfTimesHeroHasHadSex(quest:GetNumberOfTimesHeroHasHadSex())
                                                quest:SetCutsceneSkippable(false)
                                                if not quest:GetStateBool("HeroFoundDeedsLocation") then
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00e448cc end
                                                    helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANSEX", true)
                                                    quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                else
                                                    if quest:IsActiveThreadTerminating() then
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        resources:DestroyMovie(movie6)
                                                        goto LAB_00e4450d
                                                    end
                                                    helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANSEX_QUICKIE", true)
                                                end
                                                quest:SetCutsceneSkippable(true)
                                                quest:SetStateBool("HeroPartying", true)
                                                quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                quest:SetHeroAsHavingHadGaySex(true)
                                            else
                                                if isActiveThreadTerminating then
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    resources:DestroyMovie(movie6)
                                                    goto LAB_00e4450d
                                                end
                                                if 0.0 < quest:GetHealth(resources:ScriptThing(hero)) then
                                                    me:Speak(hero, "TEXT_QST_B13_MAGICMAN_REJECTED", GROUP_SELECT_FIRST, false, true, false)
                                                    timerId2 = me:IsPerformingScriptTask()
                                                    while timerId2 do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                                        timerId2 = me:IsPerformingScriptTask()
                                                    end
                                                    if quest:IsActiveThreadTerminating() then
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        resources:DestroyMovie(movie6)
                                                        goto LAB_00e4450d
                                                    end
                                                end
                                            end
                                            goto LAB_00e437b3
                                        end
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie6)
                                    goto LAB_00e4450d
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(movie6)) then
                                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BAD_HAIR", GROUP_SELECT_FIRST, false, true, false)
                                        timerId2 = me:IsPerformingScriptTask()
                                        while timerId2 do
                                            quest:NewScriptFrame(me)
                                            if not quest:IsActiveThreadTerminating() then
                                                timerId2 = me:IsPerformingScriptTask()
                                            else
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyMovie(movie6)
                                                goto LAB_00e4450d
                                                timerId2 = me:IsPerformingScriptTask()
                                            end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00e448cc end
                                    end
                                    goto LAB_00e437b3
                                end
                            end
                        end
                    end
                    ::LAB_00e448cc::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie6)
                    goto LAB_00e4450d
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie6)
                    goto LAB_00e4450d
                end
                if 0.0 < quest:GetHealth(resources:ScriptThing(hero)) then
                    me:Speak(hero, "TEXT_QST_B13_MAGICMAN_TOUTING", GROUP_SELECT_FIRST, false, true, false)
                    timerId2 = me:IsPerformingScriptTask()
                    while timerId2 do
                        quest:NewScriptFrame(me)
                        if not quest:IsActiveThreadTerminating() then
                            timerId2 = me:IsPerformingScriptTask()
                        else
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie6)
                            goto LAB_00e4450d
                            timerId2 = me:IsPerformingScriptTask()
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie6)
                        goto LAB_00e4450d
                    end
                end
                ::LAB_00e437b3::
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(hero)
            end
            scratchValue25 = scratchValue26
            scratchValue26 = scratchValue26 | 0x4000
            if me:MsgIsHitByHero() then goto LAB_00e43893 end
            scratchValue26 = scratchValue25 | 0xc000
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue26 = scratchValue26 | 0x10000
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e43893 end
            end
            isActiveThreadTerminating = false
            goto FLOW_past_lab_00e43893
            ::LAB_00e43893::
            isActiveThreadTerminating = true
            ::FLOW_past_lab_00e43893::
            if scratchValue26 & 0x10000 ~= 0 then
                scratchValue26 = scratchValue26 & 0xfffeffff
            end
            if scratchValue26 >> 8 < 0 then
                scratchValue26 = scratchValue26 & 0xffff7fff
            end
            if scratchValue26 & 0x4000 ~= 0 then
                scratchValue26 = scratchValue26 & 0xffffbfff
            end
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                me:ClearCommands()
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                quest:EntitySetFacingAngleTowardsThing(p0, nil --[[missing]], __unknown_push)
                quest:Pause(1.0)
                quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]], __unknown_push)
                quest:NewScriptFrame(me)
                getHero = "CAM_B_OWNER"
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], __unknown_push, -1.0, 0)
                if helpers.IsHeroWearingBeard(quest, me) then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e4450d
                    end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(p0)) then
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_ATTACKED_FEMALE", GROUP_SELECT_FIRST, false, true, false)
                        timerId2 = me:IsPerformingScriptTask()
                        while timerId2 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                timerId2 = me:IsPerformingScriptTask()
                            else
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00e4450d
                                timerId2 = me:IsPerformingScriptTask()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00e4450d
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e4450d
                    end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_ATTACKED_MALE", GROUP_SELECT_FIRST, false, true, false)
                        timerId2 = me:IsPerformingScriptTask()
                        while timerId2 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                timerId2 = me:IsPerformingScriptTask()
                            else
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00e4450d
                                timerId2 = me:IsPerformingScriptTask()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00e4450d
                        end
                    end
                end
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
            end
            if not me:MsgIsPresentedWithItem() then goto continue_13 end
            if unaff_EBP ~= nil then
                -- TODO(native): iVar21 = CBasicString<char>::Compare((void *)*unaff_EBP,"OBJECT_BEER_TANKARD");
                if timerId2 == 0 then goto LAB_00e43c54 end
            end
            goto FLOW_past_lab_00e43c54
            ::LAB_00e43c54::
            if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
            me:ClearCommands()
            quest:FixMovieSequenceCamera(true)
            quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]], __unknown_push)
            quest:Pause(1.0)
            quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]], __unknown_push)
            quest:NewScriptFrame(me)
            quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], __unknown_push, -1.0, 0)
            startMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            switch = quest:GetStateInt("BeersDrunk")
            repeat
                if switch == 0 then
                    getHero = resources:ScriptThing(hero)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER1" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(startMovie)
                            goto LAB_00e4450d
                        end
                    end
                    break
                elseif switch == 1 then
                    getHero = resources:ScriptThing(hero)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER2" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                    end
                    break
                elseif switch == 2 then
                    getHero = resources:ScriptThing(hero)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER3" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                    end
                    break
                elseif switch == 3 then
                    getHero = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER4" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                    end
                    break
                elseif switch == 4 then
                    getHero = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER5" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                    end
                    break
                elseif switch == 5 then
                    getHero = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(getHero) then
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER6" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                    end
                    if not quest:GetStateBool("HeroFoundDeedsLocation") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                        helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANDRUNK", false)
                        quest:SetStateBool("HeroFoundDeedsLocation", true)
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(scratchValue19 ~= 0)
                            resources:DestroyMovie(startMovie)
                            goto LAB_00e4450d
                        end
                        helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANDRUNK_QUICKIE", false)
                    end
                    break
                else
                    getHero = resources:ScriptThing(resource)
                    if 0.0 >= quest:GetHealth(getHero) then
                        quest:GiveHeroObject("OBJECT_BEER_TANKARD", scriptThing)
                    else
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEERTOOMUCH", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00e4450d
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                        quest:GiveHeroObject("OBJECT_BEER_TANKARD", scriptThing)
                    end
                end
            until true
            quest:SetStateInt("BeersDrunk", quest:GetStateInt("BeersDrunk") + 1)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie4)
            ::FLOW_past_lab_00e43c54::
            ::continue_13::
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:RemoveThing(nil --[[missing]], __unknown_push, false)
    end
    ::LAB_00e4450d::
    quest:DeregisterTimer(timerId3)
    quest:DeregisterTimer(timerId3)
    ::LAB_00e4451f::
    ::LAB_00e44528::
    resources:ReleaseResource(resource6)
    do return end
    ::LAB_00e44615::
    quest:PauseAllNonScriptedEntities(0x126008c)
    resources:DestroyMovie(movie6)
    ::LAB_00e448a1::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId3)
    goto LAB_00e4451f
    ::LAB_00e428c7::
    if not me:IsPerformingScriptTask() then return end  -- TODO(native): goto LAB_00e428d4
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then goto LAB_00e428c7 end
    quest:PauseAllNonScriptedEntities(0x126008c)
    goto LAB_00e44862
    ::LAB_00e44811::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie6)
    ::LAB_00e44862::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId3)
    goto LAB_00e4451f
    ::LAB_00e44903::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(startMovie)
    goto LAB_00e4450d
end

-- Magicman.Init (retail 0x00e3b060)
function Init(quest, me)
    quest:SetThingHasInformation(me, true, false, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:SetIsThingForcePushable(me, false)
    doneToutIntro = false
    mentionedPimpHat = false
end

-- Magicman.OnPersist (retail 0x00e3bad0)
function OnPersist(quest, me, context)
    quest:SetStateBool("DoneToutIntro", quest:PersistTransferBool(context, "DoneToutIntro", quest:GetStateBool("DoneToutIntro")))
end

-- Magicman.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

