-- Generated from the same native helper bodies as the quest draft.
local UpdateLiveEnemies, helper_DFDED0
function UpdateLiveEnemies(quest, me)
    local isActiveThreadTerminating, isActiveThreadTerminating2, isActiveThreadTerminating3
    local isActiveThreadTerminating4, isActiveThreadTerminating5, isActiveThreadTerminating6
    local isActiveThreadTerminating7, isActiveThreadTerminating8, predicateResult
    local getFollowingEntityList, scratchValue7, p0, scratchValue8, scratchValue9
    quest:StateListClear("AllCreatures")
    quest:StateListSet("AllCreatures", quest:GetAllCreaturesExcludingHero())
    scratchValue8 = 0
    while scratchValue8 ~= quest:GetStateListCount("AllCreatures") do
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return isActiveThreadTerminating
        end
        if ((quest:GetStateListAt("AllCreatures", scratchValue8):GetDefName() == "CREATURE_NEW_CHICKEN_04") and 0 or 1) ~= 0 then
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "TraderToRescue" then goto LAB_00dfc413 end
            isActiveThreadTerminating2 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating2 then
                return isActiveThreadTerminating2
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc413::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "BodyGuard" then goto LAB_00dfc45c end
            isActiveThreadTerminating3 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating3 then
                return isActiveThreadTerminating3
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc45c::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "RingFighter" then goto LAB_00dfc4a5 end
            isActiveThreadTerminating4 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating4 then
                return isActiveThreadTerminating4
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc4a5::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "FisticuffsMember" then goto LAB_00dfc4ee end
            isActiveThreadTerminating5 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating5 then
                return isActiveThreadTerminating5
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc4ee::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "Tyler" then goto LAB_00dfc537 end
            isActiveThreadTerminating6 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating6 then
                return isActiveThreadTerminating6
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc537::
            isActiveThreadTerminating7 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating7 then
                return isActiveThreadTerminating7
            end
            scratchValue8 = scratchValue8 + 1
        else
            isActiveThreadTerminating8 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating8 then
                return isActiveThreadTerminating8
            end
            quest:StateListErase("AllCreatures", scratchValue8)
        end
        ::FLOW_after_lab_00dfc3b5::
    end
    predicateResult = quest:IsActiveThreadTerminating()
    scratchValue7 = predicateResult
    if not predicateResult then
        scratchValue9 = 0
        getFollowingEntityList = quest:GetFollowingEntityList(quest:GetHero())
        if #getFollowingEntityList ~= 0 then
            scratchValue7 = 4
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00dfc618 end
                p0 = 0
                while p0 ~= quest:GetStateListCount("AllCreatures") do
                    -- TODO(native): cVar8 = (**(**(iVar3 + iStack_c) + 0x138))(quest:GetStateListAt("AllCreatures", p0))
    --[[unresolved native value]]
                    if nil ~= 0 then
                        quest:StateListErase("AllCreatures", p0)
                        break
                    end
                    p0 = p0 + 1
                end
                scratchValue9 = scratchValue9 + 1
                scratchValue7 = scratchValue7 + 12
            until scratchValue9 >= #getFollowingEntityList
        end
        ::LAB_00dfc618::
    end
    return scratchValue7
end

function helper_DFDED0(quest, me, strParam1)
    local scratchValue = resources:NewResource()
    local pScriptObject = scratchValue
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local scratchValue2 = resources:NewActorMap()
    resources:SetActor(scratchValue2, "HERO", scratchValue)
    local scratchValue3 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro(strParam1, scratchValue2, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(scratchValue3)
    resources:DestroyActorMap(scratchValue2)
    resources:ReleaseResource(scratchValue)
end

return {UpdateLiveEnemies = UpdateLiveEnemies, helper_DFDED0 = helper_DFDED0}
