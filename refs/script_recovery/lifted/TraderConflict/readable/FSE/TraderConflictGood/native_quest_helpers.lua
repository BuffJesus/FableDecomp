-- Generated from the same native helper bodies as the quest draft.
local UpdateLiveEnemies, helper_DFDED0
function UpdateLiveEnemies(quest, me)
    local isActiveThreadTerminating2, isActiveThreadTerminating3, isActiveThreadTerminating4
    local isActiveThreadTerminating5, isActiveThreadTerminating6, scratchValue, p0
    local allCreaturesIndex, scratchValue8
    quest:StateListClear("AllCreatures")
    quest:StateListSet("AllCreatures", quest:GetAllCreaturesExcludingHero())
    allCreaturesIndex = 0
    while allCreaturesIndex ~= quest:GetStateListCount("AllCreatures") do
        local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return isActiveThreadTerminating
        end
        if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetDefName() ~= "CREATURE_NEW_CHICKEN_04" then
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() ~= "TraderToRescue" then goto LAB_00dfc413 end
            isActiveThreadTerminating2 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating2 then
                return isActiveThreadTerminating2
            end
            quest:StateListErase("AllCreatures", allCreaturesIndex)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc413::
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() ~= "BodyGuard" then goto LAB_00dfc45c end
            isActiveThreadTerminating3 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating3 then
                return isActiveThreadTerminating3
            end
            quest:StateListErase("AllCreatures", allCreaturesIndex)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc45c::
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() ~= "RingFighter" then goto LAB_00dfc4a5 end
            isActiveThreadTerminating4 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating4 then
                return isActiveThreadTerminating4
            end
            quest:StateListErase("AllCreatures", allCreaturesIndex)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc4a5::
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() ~= "FisticuffsMember" then goto LAB_00dfc4ee end
            isActiveThreadTerminating5 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating5 then
                return isActiveThreadTerminating5
            end
            quest:StateListErase("AllCreatures", allCreaturesIndex)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc4ee::
            if quest:GetStateListAt("AllCreatures", allCreaturesIndex):GetName() ~= "Tyler" then goto LAB_00dfc537 end
            isActiveThreadTerminating6 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating6 then
                return isActiveThreadTerminating6
            end
            quest:StateListErase("AllCreatures", allCreaturesIndex)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc537::
            local isActiveThreadTerminating7 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating7 then
                return isActiveThreadTerminating7
            end
            allCreaturesIndex = allCreaturesIndex + 1
        else
            local isActiveThreadTerminating8 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating8 then
                return isActiveThreadTerminating8
            end
            quest:StateListErase("AllCreatures", allCreaturesIndex)
        end
        ::FLOW_after_lab_00dfc3b5::
    end
    local predicateResult = quest:IsActiveThreadTerminating()
    scratchValue = predicateResult
    if not predicateResult then
        scratchValue8 = 0
        local followers = quest:GetFollowingEntityList(quest:GetHero())
        if #followers ~= 0 then
            scratchValue = 4
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00dfc618 end
                p0 = 0
                while p0 ~= quest:GetStateListCount("AllCreatures") do
                    -- TODO(native): cVar8 = (**(**(iVar3 + iStack_c) + 0x138))(quest:GetStateListAt("AllCreatures", p0))
    --[[unresolved native value]]
                    if nil == 0 then
                        p0 = p0 + 1
                    else
                        quest:StateListErase("AllCreatures", p0)
                        break
                        p0 = p0 + 1
                    end
                end
                scratchValue8 = scratchValue8 + 1
                scratchValue = scratchValue + 12
            until scratchValue8 >= #followers
        end
        ::LAB_00dfc618::
    end
    return scratchValue
end

function helper_DFDED0(quest, me, strParam1)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    local pScriptObject = resource
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    local movie = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro(strParam1, actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

return {UpdateLiveEnemies = UpdateLiveEnemies, helper_DFDED0 = helper_DFDED0}
