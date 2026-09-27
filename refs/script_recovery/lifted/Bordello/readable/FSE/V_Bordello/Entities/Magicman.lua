-- Readable native conversion: Magicman. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local helpers = require("V_Bordello.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local doneToutIntro, mentionedPimpHat

-- Magicman.Main (retail 0x00e40e80)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isActiveThreadTerminating, predicateResult, health, timerId2, scratchValue71
    local conversationId, timerId3, scratchValue72, scratchValue73, timerId4, switch1, switch, p0
    local movie, this_01, timerId, scratchValue76, scratchValue77, scriptThing, movie2, scriptThing2
    local scratchValue78, startMovie, movie5, movie6, resource2, newResource, scratchValue81
    if not quest:NewScriptFrame(me) then return end
    newResource = resources:NewResource()
    resources:PrepareResource(0)
    while not me:AcquireControl(4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e44528 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e44528 end
    resources:AssignResource(resources:MemberResource("seh_Boss"), 0)
    timerId4 = quest:RegisterTimer()
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 40)
    scratchValue73 = 1
    while not quest:GetStateBool("PlayerOwned") do
        if not quest:NewScriptFrame(me) then goto LAB_00e4450d end
        while not quest:GetStateBool("HeroTricking") and not quest:GetStateBool("PlayerOwned") do
            if not quest:NewScriptFrame(me) then goto LAB_00e448a1 end
            if (me ~= nil and me:IsDistanceFromPositionOver(me:GetHomePos(), 2.0)) and not me:IsPerformingScriptTask() then
                scratchValue72 = 1
                conversationId = 0
                scratchValue71 = 0
                timerId2 = 1.0
                me:MoveToPosition(me:GetHomePos(), 1.0, ENTITY_MOVE_WALK, false, true)
            end
            if quest:GetStateBool("BeerSetToDPad") then goto LAB_00e410ba end
            scratchValue77 = scratchValue77 | 1
            timerId3 = quest:GetHeroTargetedThing()
            isActiveThreadTerminating = true
            if not (timerId3 ~= nil and timerId3:IsEqualTo(me)) then goto LAB_00e410ba end
            goto FLOW_past_lab_00e410ba
            ::LAB_00e410ba::
            isActiveThreadTerminating = false
            ::FLOW_past_lab_00e410ba::
            if scratchValue77 & 1 ~= 0 then
                scratchValue77 = scratchValue77 & 0xfffffffe
            end
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                quest:SetStateBool("BeerSetToDPad", true)
                quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", timerId2, scratchValue71)
                quest:CreateThread("WatchForHeroLeavingRegionWithBeer")  -- native thread body 0x00E44980: lift it as function WatchForHeroLeavingRegionWithBeer(quest)
                if scratchValue77 & 8 ~= 0 then
                    scratchValue77 = scratchValue77 & 0xfffffff7
                end
                if scratchValue77 & 4 ~= 0 then
                    scratchValue77 = scratchValue77 & 0xfffffffb
                end
                if scratchValue77 & 2 ~= 0 then
                    scratchValue77 = scratchValue77 & 0xfffffffd
                end
            else
                if not quest:GetStateBool("BeerSetToDPad") then
                    goto LAB_00e412bc
                else
                    scratchValue77 = scratchValue77 | 16
                    timerId3 = quest:GetHeroTargetedThing()
                    isActiveThreadTerminating = true
                    if timerId3 ~= nil and timerId3:IsEqualTo(me) then goto LAB_00e412bc end
                end
                goto FLOW_past_lab_00e412bc
                ::LAB_00e412bc::
                isActiveThreadTerminating = false
                ::FLOW_past_lab_00e412bc::
                if scratchValue77 & 16 ~= 0 then
                    scratchValue77 = scratchValue77 & 0xffffffef
                end
                if isActiveThreadTerminating then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                    quest:SetStateBool("BeerSetToDPad", false)
                end
            end
            timerId3 = timerId4
            if quest:GetTimer(timerId4) == 0 then
                if quest:IsDistanceBetweenThingsUnder(me, hero, 8.0) then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId4)
                        goto LAB_00e4451f
                    end
                    quest:SetTimer(timerId4, 4)
                    quest:EntitySetFacingAngleTowardsThing(hero, hero)
                end
            end
            timerId2 = quest:GetTimer(timerId)
            if timerId2 == 0 then
                if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId4)
                        goto LAB_00e4451f
                    end
                    scratchValue78 = "TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(scratchValue73)
                    if not quest:TextEntryExists() then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(timerId4)
                            goto LAB_00e4451f
                        end
                        scratchValue73 = 1
                    end
                    quest:AddNewConversation(hero, scratchValue73 ~= 0, 15.0 ~= 0)
                    quest:AddPersonToConversation(conversationId, hero)
                    -- TODO(native): xStack_308 = (undefined **)**(int **)(this + 4);
                    -- TODO(native): xStack_344 = (int *)(*(code *)xStack_308[0x46])();
                    scratchValue81 = "TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(0)
                    -- TODO(native): (*(code *)xStack_308[0x16e])();
                    scratchValue73 = 0 + 1
                    quest:SetTimer(timerId, 30)
                end
            end
            if me:IsTalkedToByHero() then
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating then goto LAB_00e448a1 end
                me:ClearCommands()
                movie6 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(scratchValue72 ~= 0)
                quest:FixMovieSequenceCamera(false)
                quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]])
                quest:Pause(0)
                timerId2 = hero
                quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]])
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], 0, nil --[[missing]], nil --[[missing]])
                -- TODO(native): bVar5 = NScript::CV_BordelloScript::IsHeroWearingBeard__ate3e320(*(CV_BordelloScript **)(this + 0x14));
                if isActiveThreadTerminating then
                    if not quest:IsActiveThreadTerminating() then
                        resources:ScriptThing(p0)
                        health = quest:GetHealth(nil --[[missing]])
                        if 0.0 < health then
                            scratchValue72 = 1
                            conversationId = 0
                            scratchValue71 = 0
                            me:Speak(hero, "TEXT_QST_B13_MAGICMAN_PIMPING", GROUP_SELECT_FIRST, false, true, false)
                            timerId3 = me:IsPerformingScriptTask()
                            while timerId3 do
                                quest:NewScriptFrame(me)
                                if not quest:IsActiveThreadTerminating() then
                                    timerId3 = me:IsPerformingScriptTask()
                                else
                                    quest:PauseAllNonScriptedEntities(false)
                                    quest:DeregisterTimer(me)
                                    quest:DeregisterTimer(hero)
                                    do return end
                                    timerId3 = me:IsPerformingScriptTask()
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                resources:DestroyMovie(movie6)
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
                    resources:ScriptThing(p0)
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_TOUTING", GROUP_SELECT_FIRST, false, true, false)
                        timerId3 = me:IsPerformingScriptTask()
                        while timerId3 do
                            if not quest:NewScriptFrame(me) then goto LAB_00e44615 end
                            timerId3 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e445a0 end
                    end
                    goto LAB_00e41b75
                end
                goto FLOW_past_lab_00e41b75
                ::LAB_00e41b75::
                quest:FixMovieSequenceCamera(nil --[[missing]])
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                resources:DestroyMovie(movie6)
                goto LAB_00e41ba8
                ::FLOW_past_lab_00e41b75::
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    resources:DestroyMovie(movie6)
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
                        resources:ScriptThing(1)
                        health = quest:GetHealth(nil --[[missing]])
                        if 0.0 < health then
                            scratchValue72 = 0
                            conversationId = 1
                            scratchValue71 = 0
                            timerId2 = 0
                            me:Speak(hero, startMovie, 0, false, true, false)
                            timerId3 = me:IsPerformingScriptTask()
                            while timerId3 do
                                quest:NewScriptFrame(me)
                                if not quest:IsActiveThreadTerminating() then
                                    timerId3 = me:IsPerformingScriptTask()
                                else
                                    quest:PauseAllNonScriptedEntities(true)
                                    goto LAB_00e44862
                                    timerId3 = me:IsPerformingScriptTask()
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(predicateResult)
                                resources:DestroyMovie(movie6)
                                goto LAB_00e448a1
                            end
                        end
                        -- TODO(native): xStack_364 = xStack_364 + 1;
                        goto LAB_00e41b75
                    end
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    resources:DestroyMovie(movie6)
                    goto LAB_00e448a1
                end
                if not quest:IsActiveThreadTerminating() then
                    resources:ScriptThing(newResource)
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_PIMP_HAT", GROUP_SELECT_FIRST, false, true, false)
                        timerId3 = me:IsPerformingScriptTask()
                        while timerId3 do
                            if not quest:NewScriptFrame(me) then goto LAB_00e44615 end
                            timerId3 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e445a0 end
                    end
                    mentionedPimpHat = true
                    goto LAB_00e41b75
                end
                ::LAB_00e445a0::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie6)
                goto LAB_00e44862
            end
            ::LAB_00e41ba8::
            scratchValue76 = scratchValue77
            scratchValue77 = scratchValue77 | 64
            if me:MsgIsHitByHero() then goto LAB_00e41c4b end
            scratchValue77 = scratchValue76 | 192
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue77 = scratchValue76 | 448
                local scratchValue = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                if not scratchValue then goto LAB_00e41c4b end
            end
            isActiveThreadTerminating = false
            goto FLOW_past_lab_00e41c4b
            ::LAB_00e41c4b::
            isActiveThreadTerminating = true
            ::FLOW_past_lab_00e41c4b::
            if scratchValue77 & 256 ~= 0 then
                scratchValue77 = scratchValue77 & 0xfffffeff
            end
            if scratchValue77 < 0 then
                scratchValue77 = scratchValue77 & 0xffffff7f
            end
            if scratchValue77 & 64 ~= 0 then
                scratchValue77 = scratchValue77 & 0xffffffbf
            end
            if isActiveThreadTerminating then
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating then goto LAB_00e448a1 end
                me:ClearCommands()
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                quest:FixMovieSequenceCamera(nil --[[missing]])
                newResource = p0
                quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]])
                quest:Pause(nil --[[missing]])
                timerId2 = hero
                quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]])
                quest:NewScriptFrame(me)
                timerId3 = -1
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], -1, nil --[[missing]], nil --[[missing]])
                -- TODO(native): bVar5 = NScript::CV_BordelloScript::IsHeroWearingBeard__ate3e320(*(CV_BordelloScript **)(this + 0x14));
                if isActiveThreadTerminating then
                    if not quest:IsActiveThreadTerminating() then
                        resources:ScriptThing(p0)
                        health = quest:GetHealth(nil --[[missing]])
                        if 0.0 >= health then
                            goto LAB_00e42003
                        end
                        goto FLOW_past_lab_00e42003
                        ::LAB_00e42003::
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        resources:DestroyMovie(movie3)
                        goto LAB_00e4203f
                        ::FLOW_past_lab_00e42003::
                        scratchValue72 = 1
                        conversationId = 0
                        scratchValue71 = 0
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_ATTACKED_FEMALE", GROUP_SELECT_FIRST, false, true, false)
                        timerId3 = me:IsPerformingScriptTask()
                        while timerId3 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                timerId3 = me:IsPerformingScriptTask()
                            else
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                resources:DestroyMovie(movie)
                                quest:DeregisterTimer(me)
                                quest:DeregisterTimer(hero)
                                do return end
                                timerId3 = me:IsPerformingScriptTask()
                            end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00e42003 end
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        resources:DestroyMovie(movie3)
                        goto LAB_00e448a1
                    end
                elseif not quest:IsActiveThreadTerminating() then
                    resources:ScriptThing(p0)
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 1
                        conversationId = 0
                        scratchValue71 = 0
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_ATTACKED_MALE", GROUP_SELECT_FIRST, false, true, false)
                        timerId2 = me:IsPerformingScriptTask()
                        while timerId2 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                timerId2 = me:IsPerformingScriptTask()
                            else
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie6)
                                quest:DeregisterTimer(-1)
                                quest:DeregisterTimer(me)
                                do return end
                                timerId2 = me:IsPerformingScriptTask()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(this_01)
                            goto LAB_00e448a1
                        end
                    end
                    quest:FixMovieSequenceCamera(nil --[[missing]])
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    resources:DestroyMovie(movie3)
                    goto LAB_00e4203f
                end
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                resources:DestroyMovie(movie2)
                goto LAB_00e44862
            end
            ::LAB_00e4203f::
            if not me:MsgIsPresentedWithItem() then goto continue_5 end
            if unaff_EBP ~= nil then
                -- TODO(native): iVar21 = CBasicString<char>::Compare((void *)*unaff_EBP,"OBJECT_BEER_TANKARD");
                if timerId3 == 0 then goto LAB_00e42094 end
            end
            goto FLOW_past_lab_00e42094
            ::LAB_00e42094::
            if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
            me:ClearCommands()
            movie5 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            quest:FixMovieSequenceCamera(nil --[[missing]])
            newResource = p0
            quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]])
            quest:Pause(nil --[[missing]])
            quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]])
            quest:NewScriptFrame(me)
            quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]])
            switch1 = quest:GetStateInt("BeersDrunk")
            repeat
                if switch1 == 0 then
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER1" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie5)
                            goto LAB_00e448a1
                        end
                    end
                    break
                elseif switch1 == 1 then
                    resources:ScriptThing(p0)
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER2" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie5)
                            goto LAB_00e448a1
                        end
                    end
                    break
                elseif switch1 == 2 then
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER3" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie5)
                            goto LAB_00e448a1
                        end
                    end
                    break
                elseif switch1 == 3 then
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER4" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie5)
                            goto LAB_00e448a1
                        end
                    end
                    break
                elseif switch1 == 4 then
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER5" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie5)
                            goto LAB_00e448a1
                        end
                    end
                    break
                elseif switch1 == 5 then
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER6" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44811 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie5)
                            goto LAB_00e448a1
                        end
                    end
                    if not quest:GetStateBool("HeroFoundDeedsLocation") then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie5)
                            goto LAB_00e448a1
                        end
                        -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                        quest:SetStateBool("HeroFoundDeedsLocation", true)
                        quest:SetStateBool("HeroPartying", true)
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie5)
                            goto LAB_00e44862
                        end
                        -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                        quest:SetStateBool("HeroPartying", true)
                    end
                    break
                else
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 >= health then
                        quest:GiveHeroObject("OBJECT_BEER_TANKARD", nil --[[missing]])
                    else
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEERTOOMUCH", GROUP_SELECT_FIRST, false, true, false)
                        if me:IsPerformingScriptTask() then
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then goto LAB_00e428c7 end
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            goto LAB_00e44862
                        end
                        -- LAB_00e428d4: (native jump target)
                        if not quest:IsActiveThreadTerminating() then
                            quest:GiveHeroObject("OBJECT_BEER_TANKARD", nil --[[missing]])
                        else
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie5)
                            goto LAB_00e448a1
                            quest:GiveHeroObject("OBJECT_BEER_TANKARD", nil --[[missing]])
                        end
                    end
                end
            until true
            quest:SetStateInt("BeersDrunk", quest:GetStateInt("BeersDrunk") + 1)
            quest:FixMovieSequenceCamera(nil --[[missing]])
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            resources:DestroyMovie(movie5)
            ::FLOW_past_lab_00e42094::
            ::continue_5::
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
        while not quest:GetStateBool("PlayerOwned") do
            if not quest:NewScriptFrame(me) then goto LAB_00e4450d end
            if (me ~= nil and me:IsDistanceFromPositionOver(me:GetHomePos(), 2.0)) and not me:IsPerformingScriptTask() then
                scratchValue72 = 1
                conversationId = 0
                scratchValue71 = 0
                timerId2 = 1.0
                me:MoveToPosition(me:GetHomePos(), 1.0, ENTITY_MOVE_WALK, false, true)
            end
            if quest:GetStateBool("BeerSetToDPad") then goto LAB_00e42a3a end
            scratchValue77 = scratchValue77 | 512
            timerId3 = quest:GetHeroTargetedThing()
            isActiveThreadTerminating = true
            if not (timerId3 ~= nil and timerId3:IsEqualTo(me)) then goto LAB_00e42a3a end
            goto FLOW_past_lab_00e42a3a
            ::LAB_00e42a3a::
            isActiveThreadTerminating = false
            ::FLOW_past_lab_00e42a3a::
            if scratchValue77 & 512 ~= 0 then
                scratchValue77 = scratchValue77 & 0xfffffdff
            end
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                quest:SetStateBool("BeerSetToDPad", true)
                quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", timerId2, scratchValue71)
                quest:CreateThread("WatchForHeroLeavingRegionWithBeer")  -- native thread body 0x00E44980: lift it as function WatchForHeroLeavingRegionWithBeer(quest)
                if scratchValue77 & 4096 ~= 0 then
                    scratchValue77 = scratchValue77 & 0xffffefff
                end
                if scratchValue77 & 2048 ~= 0 then
                    scratchValue77 = scratchValue77 & 0xfffff7ff
                end
                if scratchValue77 & 1024 ~= 0 then
                    scratchValue77 = scratchValue77 & 0xfffffbff
                end
            else
                if not quest:GetStateBool("BeerSetToDPad") then
                    goto LAB_00e42bf9
                else
                    scratchValue77 = scratchValue77 | 0x2000
                    timerId3 = quest:GetHeroTargetedThing()
                    isActiveThreadTerminating = true
                    if timerId3 ~= nil and timerId3:IsEqualTo(me) then goto LAB_00e42bf9 end
                end
                goto FLOW_past_lab_00e42bf9
                ::LAB_00e42bf9::
                isActiveThreadTerminating = false
                ::FLOW_past_lab_00e42bf9::
                if scratchValue77 & 0x2000 ~= 0 then
                    scratchValue77 = scratchValue77 & 0xffffdfff
                end
                if isActiveThreadTerminating then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                    quest:SetStateBool("BeerSetToDPad", false)
                end
            end
            timerId2 = quest:GetTimer(timerId4)
            if timerId2 == 0 then
                if quest:IsDistanceBetweenThingsUnder(me, hero, 8.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                    quest:SetTimer(timerId4, 4)
                    quest:EntitySetFacingAngleTowardsThing(hero, hero)
                end
            end
            timerId3 = quest:GetTimer(timerId)
            if timerId3 == 0 then
                if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                    scriptThing2 = "TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(scratchValue73)
                    if not quest:TextEntryExists() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                        scratchValue73 = 1
                    end
                    quest:AddNewConversation(hero, scratchValue73 ~= 0, 15.0 ~= 0)
                    quest:AddPersonToConversation(conversationId, hero)
                    -- TODO(native): xStack_194 = (undefined **)**(int **)(this + 4);
                    -- TODO(native): xStack_344 = (int *)(*(code *)xStack_194[0x46])();
                    local scratchValue75 = newResource
                    newResource = p0
                    scriptThing = "TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(p0)
                    -- TODO(native): (*(code *)xStack_194[0x16e])();
                    scratchValue73 = scratchValue75 + 1
                    quest:SetTimer(timerId, 30)
                end
            end
            if me:IsTalkedToByHero() then
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating then goto LAB_00e4450d end
                me:ClearCommands()
                resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(false)
                quest:FixMovieSequenceCamera(scratchValue72 ~= 0)
                newResource = p0
                quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]])
                quest:Pause(nil --[[missing]])
                quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]])
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]])
                -- TODO(native): bVar5 = NScript::CV_BordelloScript::IsHeroWearingBeard__ate3e320(*(CV_BordelloScript **)(this + 0x14));
                if isActiveThreadTerminating then
                    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                    if not isActiveThreadTerminating then
                        -- TODO(native): bVar5 = NScript::CV_BordelloScript::IsHeroWearingBeard__ate44a40(*(CV_BordelloScript **)(this + 0x14));
                        if isActiveThreadTerminating then
                            if not quest:IsActiveThreadTerminating() then
                                health = quest:GetHealth(nil --[[missing]])
                                if 0.0 < health then
                                    scratchValue72 = 0
                                    conversationId = 1
                                    scratchValue71 = 0
                                    timerId2 = 0
                                    me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEARD_LADY", GROUP_SELECT_FIRST, false, true, false)
                                    timerId3 = me:IsPerformingScriptTask()
                                    while timerId3 do
                                        if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                        timerId3 = me:IsPerformingScriptTask()
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                        resources:DestroyMovie(startMovie)
                                        goto LAB_00e4450d
                                    end
                                end
                                goto LAB_00e437b3
                            end
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(startMovie)
                            goto LAB_00e4450d
                        end
                        if not quest:IsActiveThreadTerminating() then
                            if helpers.IsHeroWearingTash(quest, me) then
                                if not quest:IsActiveThreadTerminating() then
                                    health = quest:GetHealth(nil --[[missing]])
                                    if 0.0 < health then
                                        scratchValue72 = 0
                                        conversationId = 1
                                        scratchValue71 = 0
                                        timerId2 = 0
                                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEARD_LADY_MOUSTACHE", GROUP_SELECT_FIRST, false, true, false)
                                        timerId3 = me:IsPerformingScriptTask()
                                        while timerId3 do
                                            if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                            timerId3 = me:IsPerformingScriptTask()
                                        end
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                            resources:DestroyMovie(startMovie)
                                            goto LAB_00e4450d
                                        end
                                    end
                                    goto LAB_00e437b3
                                end
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                resources:DestroyMovie(startMovie)
                                goto LAB_00e4450d
                            end
                            if not quest:IsActiveThreadTerminating() then
                                health = quest:GetHealth(nil --[[missing]])
                                if 0.0 < health then
                                    me:Speak(me, "TEXT_QST_B13_MAGICMAN_LOVELY_SKIN", GROUP_SELECT_FIRST, false, true, false)
                                    timerId3 = me:IsPerformingScriptTask()
                                    while timerId3 do
                                        quest:NewScriptFrame(me)
                                        if not quest:IsActiveThreadTerminating() then
                                            timerId3 = me:IsPerformingScriptTask()
                                        else
                                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                            resources:DestroyMovie(startMovie)
                                            goto LAB_00e4450d
                                            timerId3 = me:IsPerformingScriptTask()
                                        end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e448cc end
                                end
                                if quest:IsWearingClothingItem(hero, "OBJECT_HERO_HAT_WHOREWIG") then
                                    if not quest:IsActiveThreadTerminating() then
                                        health = quest:GetHealth(nil --[[missing]])
                                        if 0.0 < health then
                                            scratchValue72 = 0
                                            conversationId = 1
                                            scratchValue71 = 0
                                            timerId2 = 0
                                            if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_LOVELY_HAIR", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e448cc end
                                            if quest:IsActiveThreadTerminating() then
                                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                resources:DestroyMovie(startMovie)
                                                goto LAB_00e4450d
                                            end
                                        end
                                        quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MAGICMAN_PARTY", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                                        timerId3 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        while timerId3 < 0 do
                                            if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                            timerId3 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                                            if timerId3 == 1 then
                                                if isActiveThreadTerminating then goto LAB_00e448cc end
                                                health = quest:GetHealth(nil --[[missing]])
                                                if 0.0 < health then
                                                    scratchValue72 = 0
                                                    conversationId = 1
                                                    scratchValue71 = 0
                                                    timerId2 = 0
                                                    me:Speak(hero, "TEXT_QST_B13_MAGICMAN_HORNY", GROUP_SELECT_FIRST, false, true, false)
                                                    timerId3 = me:IsPerformingScriptTask()
                                                    while timerId3 do
                                                        quest:NewScriptFrame(me)
                                                        if not quest:IsActiveThreadTerminating() then
                                                            timerId3 = me:IsPerformingScriptTask()
                                                        else
                                                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                            resources:DestroyMovie(startMovie)
                                                            goto LAB_00e4450d
                                                            timerId3 = me:IsPerformingScriptTask()
                                                        end
                                                    end
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00e448cc end
                                                end
                                                quest:GiveHeroGold(nil --[[missing]])
                                                quest:SetNumberOfTimesHeroHasHadSex(nil --[[missing]])
                                                quest:SetCutsceneSkippable(nil --[[missing]])
                                                if not quest:GetStateBool("HeroFoundDeedsLocation") then
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00e448cc end
                                                    -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                                                    quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                else
                                                    if quest:IsActiveThreadTerminating() then
                                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                        resources:DestroyMovie(startMovie)
                                                        goto LAB_00e4450d
                                                    end
                                                    -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                                                end
                                                quest:SetCutsceneSkippable(nil --[[missing]])
                                                quest:SetStateBool("HeroPartying", true)
                                                quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                quest:SetHeroAsHavingHadGaySex(nil --[[missing]])
                                            else
                                                if isActiveThreadTerminating then
                                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                    resources:DestroyMovie(startMovie)
                                                    goto LAB_00e4450d
                                                end
                                                health = quest:GetHealth(nil --[[missing]])
                                                if 0.0 < health then
                                                    scratchValue72 = 0
                                                    conversationId = 1
                                                    scratchValue71 = 0
                                                    timerId2 = 0
                                                    me:Speak(hero, "TEXT_QST_B13_MAGICMAN_REJECTED", GROUP_SELECT_FIRST, false, true, false)
                                                    timerId3 = me:IsPerformingScriptTask()
                                                    while timerId3 do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                                        timerId3 = me:IsPerformingScriptTask()
                                                    end
                                                    if quest:IsActiveThreadTerminating() then
                                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                        resources:DestroyMovie(startMovie)
                                                        goto LAB_00e4450d
                                                    end
                                                end
                                            end
                                            goto LAB_00e437b3
                                        end
                                    end
                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                    resources:DestroyMovie(startMovie)
                                    goto LAB_00e4450d
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    health = quest:GetHealth(nil --[[missing]])
                                    if 0.0 < health then
                                        scratchValue72 = 0
                                        conversationId = 1
                                        scratchValue71 = 0
                                        timerId2 = 0
                                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BAD_HAIR", GROUP_SELECT_FIRST, false, true, false)
                                        timerId3 = me:IsPerformingScriptTask()
                                        while timerId3 do
                                            quest:NewScriptFrame(me)
                                            if not quest:IsActiveThreadTerminating() then
                                                timerId3 = me:IsPerformingScriptTask()
                                            else
                                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                resources:DestroyMovie(startMovie)
                                                goto LAB_00e4450d
                                                timerId3 = me:IsPerformingScriptTask()
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
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    resources:DestroyMovie(startMovie)
                    goto LAB_00e4450d
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    resources:DestroyMovie(startMovie)
                    goto LAB_00e4450d
                end
                health = quest:GetHealth(nil --[[missing]])
                if 0.0 < health then
                    scratchValue72 = 0
                    conversationId = 1
                    scratchValue71 = 0
                    timerId2 = 0
                    me:Speak(hero, "TEXT_QST_B13_MAGICMAN_TOUTING", GROUP_SELECT_FIRST, false, true, false)
                    timerId3 = me:IsPerformingScriptTask()
                    while timerId3 do
                        quest:NewScriptFrame(me)
                        if not quest:IsActiveThreadTerminating() then
                            timerId3 = me:IsPerformingScriptTask()
                        else
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(startMovie)
                            goto LAB_00e4450d
                            timerId3 = me:IsPerformingScriptTask()
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        resources:DestroyMovie(startMovie)
                        goto LAB_00e4450d
                    end
                end
                ::LAB_00e437b3::
                quest:FixMovieSequenceCamera(nil --[[missing]])
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                resources:DestroyMovie(startMovie)
            end
            scratchValue76 = scratchValue77
            scratchValue77 = scratchValue77 | 0x4000
            if me:MsgIsHitByHero() then goto LAB_00e43893 end
            scratchValue77 = scratchValue76 | 0xc000
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue77 = scratchValue77 | 0x10000
                local scratchValue50 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                if not scratchValue50 then goto LAB_00e43893 end
            end
            isActiveThreadTerminating = false
            goto FLOW_past_lab_00e43893
            ::LAB_00e43893::
            isActiveThreadTerminating = true
            ::FLOW_past_lab_00e43893::
            if scratchValue77 & 0x10000 ~= 0 then
                scratchValue77 = scratchValue77 & 0xfffeffff
            end
            if scratchValue77 >> 8 < 0 then
                scratchValue77 = scratchValue77 & 0xffff7fff
            end
            if scratchValue77 & 0x4000 ~= 0 then
                scratchValue77 = scratchValue77 & 0xffffbfff
            end
            if isActiveThreadTerminating then
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating then goto LAB_00e4450d end
                me:ClearCommands()
                local movie4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                quest:FixMovieSequenceCamera(nil --[[missing]])
                quest:EntitySetFacingAngleTowardsThing(nil --[[missing]], nil --[[missing]])
                quest:Pause(nil --[[missing]])
                quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]])
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]])
                -- TODO(native): bVar5 = NScript::CV_BordelloScript::IsHeroWearingBeard__ate3e320(*(CV_BordelloScript **)(this + 0x14));
                if isActiveThreadTerminating then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        resources:DestroyMovie(movie4)
                        goto LAB_00e4450d
                    end
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_ATTACKED_FEMALE", GROUP_SELECT_FIRST, false, true, false)
                        timerId3 = me:IsPerformingScriptTask()
                        while timerId3 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                timerId3 = me:IsPerformingScriptTask()
                            else
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                resources:DestroyMovie(movie4)
                                goto LAB_00e4450d
                                timerId3 = me:IsPerformingScriptTask()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie4)
                            goto LAB_00e4450d
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        resources:DestroyMovie(movie4)
                        goto LAB_00e4450d
                    end
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_ATTACKED_MALE", GROUP_SELECT_FIRST, false, true, false)
                        timerId3 = me:IsPerformingScriptTask()
                        while timerId3 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                timerId3 = me:IsPerformingScriptTask()
                            else
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                resources:DestroyMovie(movie4)
                                goto LAB_00e4450d
                                timerId3 = me:IsPerformingScriptTask()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie4)
                            goto LAB_00e4450d
                        end
                    end
                end
                quest:FixMovieSequenceCamera(nil --[[missing]])
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                resources:DestroyMovie(movie4)
            end
            if not me:MsgIsPresentedWithItem() then goto continue_13 end
            if unaff_EBP ~= nil then
                -- TODO(native): iVar21 = CBasicString<char>::Compare((void *)*unaff_EBP,"OBJECT_BEER_TANKARD");
                if timerId3 == 0 then goto LAB_00e43c54 end
            end
            goto FLOW_past_lab_00e43c54
            ::LAB_00e43c54::
            if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
            me:ClearCommands()
            quest:FixMovieSequenceCamera(nil --[[missing]])
            quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]])
            quest:Pause(nil --[[missing]])
            quest:EntitySetFacingAngleTowardsThing(nil --[[missing]], nil --[[missing]])
            quest:NewScriptFrame(me)
            quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]])
            startMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            switch = quest:GetStateInt("BeersDrunk")
            repeat
                if switch == 0 then
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER1" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie5)
                            goto LAB_00e4450d
                        end
                    end
                    break
                elseif switch == 1 then
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER2" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                    end
                    break
                elseif switch == 2 then
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER3" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                    end
                    break
                elseif switch == 3 then
                    resources:ScriptThing(resource2)
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER4" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                    end
                    break
                elseif switch == 4 then
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER5" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                    end
                    break
                elseif switch == 5 then
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 < health then
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        if not me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEER6" .. helpers.GetHeroStatusTextTag(quest, me), 0, false, true, false) then goto LAB_00e44903 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                    end
                    if not quest:GetStateBool("HeroFoundDeedsLocation") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                        -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                        quest:SetStateBool("HeroFoundDeedsLocation", true)
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(movie5)
                            goto LAB_00e4450d
                        end
                        -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                    end
                    break
                else
                    health = quest:GetHealth(nil --[[missing]])
                    if 0.0 >= health then
                        quest:GiveHeroObject("OBJECT_BEER_TANKARD", nil --[[missing]])
                    else
                        scratchValue72 = 0
                        conversationId = 1
                        scratchValue71 = 0
                        timerId2 = 0
                        me:Speak(hero, "TEXT_QST_B13_MAGICMAN_BEERTOOMUCH", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                resources:DestroyMovie(movie5)
                                goto LAB_00e4450d
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                        quest:GiveHeroObject("OBJECT_BEER_TANKARD", nil --[[missing]])
                    end
                end
            until true
            quest:SetStateInt("BeersDrunk", quest:GetStateInt("BeersDrunk") + 1)
            quest:FixMovieSequenceCamera(nil --[[missing]])
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            resources:DestroyMovie(movie5)
            ::FLOW_past_lab_00e43c54::
            ::continue_13::
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:RemoveThing(nil --[[missing]])
    end
    ::LAB_00e4450d::
    quest:DeregisterTimer(timerId4)
    quest:DeregisterTimer(timerId4)
    ::LAB_00e4451f::
    ::LAB_00e44528::
    resources:DestroyMovie(resource2)
    do return end
    ::LAB_00e44615::
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    resources:DestroyMovie(hero)
    ::LAB_00e448a1::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId4)
    goto LAB_00e4451f
    ::LAB_00e428c7::
    if not me:IsPerformingScriptTask() then return end  -- TODO(native): goto LAB_00e428d4
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then goto LAB_00e428c7 end
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    goto LAB_00e44862
    ::LAB_00e44811::
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    resources:DestroyMovie(movie5)
    ::LAB_00e44862::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId4)
    goto LAB_00e4451f
    ::LAB_00e44903::
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    resources:DestroyMovie(movie5)
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

