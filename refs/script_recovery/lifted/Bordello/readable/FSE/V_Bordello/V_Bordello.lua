-- Readable native conversion: V_Bordello. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    BordelloHeroTavernPriceMultiplier = 1276,  -- 0.5
}

-- V_Bordello.Main (retail 0x00e39b40)
function Main(quest)
    local scratchValue
    quest:AddEntityBinding("Madame", "V_Bordello/Entities/Madame", 1)
    quest:AddEntityBinding("BordelloLady", "V_Bordello/Entities/BordelloLady", 1)
    quest:AddEntityBinding("BordelloGuard", "V_Bordello/Entities/BordelloGuard", 1)
    quest:AddEntityBinding("Magicman", "V_Bordello/Entities/Magicman", 1)
    quest:AddEntityBinding("BordelloClient", "V_Bordello/Entities/BordelloClient", 1)
    quest:AddEntityBinding("M_HeroDownstairs", "V_Bordello/Entities/M_HeroDownstairs", 1)
    quest:AddEntityBinding("BordelloEntrance", "V_Bordello/Entities/BordelloEntrance")
    quest:FinalizeEntityBindings()
    quest:CreateThread("WatchForBordelloStatus")  -- native thread body NScript::CQ_AmbushTradersScript::WatchForAllTradersDead: lift it as function WatchForBordelloStatus(quest)
    quest:CreateThread("WatchForDeedStatus")  -- native thread body NScript::CV_BordelloScript::WatchForDeedStatus: lift it as function WatchForDeedStatus(quest)
    if not scratchValue then
    end
    quest:CreateThread("CreateCustomers")  -- native thread body NScript::CV_BordelloScript::CreateCustomers: lift it as function CreateCustomers(quest)
    if not scratchValue then
    end
    quest:CreateThread("AdjustTavernPrices")  -- native thread body CScriptGameResourceObjectScriptedThingBase_HandleBordelloNPC: lift it as function AdjustTavernPrices(quest)
    if not scratchValue then
    end
end

-- V_Bordello.Init (retail 0x00e399d0)
function Init(quest)
    quest:SetStateBool("HadSex", false)
    quest:SetStateBool("HeroPartying", false)
    quest:SetStateBool("HeroTricking", false)
    quest:SetStateBool("MagicianSleeping", false)
    quest:SetStateBool("CutscenePlaying", false)
    quest:SetStateBool("PlayerOwned", false)
    quest:SetStateBool("BecomeNunnery", false)
    quest:SetStateBool("PlayerHasLeftRegion", false)
    quest:SetStateBool("HeroFoundDeeds", false)
    quest:SetStateBool("HeroFoundDeedsLocation", false)
    quest:SetStateInt("ClientsAlive", 0)
    quest:SetStateBool("WearingWhoreWig", false)
    quest:SetStateBool("ClientInUse_0", false)
    quest:SetStateBool("ClientInUse_1", false)
    quest:SetStateBool("ClientInUse_2", false)
    quest:SetStateThing("BordelloVillage", nil)
    quest:SetStateInt("BeersDrunk", 0)
    quest:SetStateBool("BeerSetToDPad", false)
end

-- V_Bordello.OnPersist (retail 0x00e3b8f0)
function OnPersist(quest, context)
    local hadSex = quest:GetStateBool("HadSex") or false
    hadSex = quest:PersistTransferBool(context, "HadSex", hadSex)
    quest:SetStateBool("HadSex", hadSex)
    local heroPartying = quest:GetStateBool("HeroPartying") or false
    heroPartying = quest:PersistTransferBool(context, "HeroPartying", heroPartying)
    quest:SetStateBool("HeroPartying", heroPartying)
    local heroTricking = quest:GetStateBool("HeroTricking") or false
    heroTricking = quest:PersistTransferBool(context, "HeroTricking", heroTricking)
    quest:SetStateBool("HeroTricking", heroTricking)
    local magicianSleeping = quest:GetStateBool("MagicianSleeping") or false
    magicianSleeping = quest:PersistTransferBool(context, "MagicianSleeping", magicianSleeping)
    quest:SetStateBool("MagicianSleeping", magicianSleeping)
    local playerOwned = quest:GetStateBool("PlayerOwned") or false
    playerOwned = quest:PersistTransferBool(context, "PlayerOwned", playerOwned)
    quest:SetStateBool("PlayerOwned", playerOwned)
    local becomeNunnery = quest:GetStateBool("BecomeNunnery") or false
    becomeNunnery = quest:PersistTransferBool(context, "BecomeNunnery", becomeNunnery)
    quest:SetStateBool("BecomeNunnery", becomeNunnery)
    local playerHasLeftRegion = quest:GetStateBool("PlayerHasLeftRegion") or false
    playerHasLeftRegion = quest:PersistTransferBool(context, "PlayerHasLeftRegion", playerHasLeftRegion)
    quest:SetStateBool("PlayerHasLeftRegion", playerHasLeftRegion)
    local heroFoundDeeds = quest:GetStateBool("HeroFoundDeeds") or false
    heroFoundDeeds = quest:PersistTransferBool(context, "HeroFoundDeeds", heroFoundDeeds)
    quest:SetStateBool("HeroFoundDeeds", heroFoundDeeds)
    local heroFoundDeedsLocation = quest:GetStateBool("HeroFoundDeedsLocation") or false
    heroFoundDeedsLocation = quest:PersistTransferBool(context, "HeroFoundDeedsLocation", heroFoundDeedsLocation)
    quest:SetStateBool("HeroFoundDeedsLocation", heroFoundDeedsLocation)
    local clientInUse0 = quest:GetStateBool("ClientInUse_0") or false
    clientInUse0 = quest:PersistTransferBool(context, "ClientInUse[0]", clientInUse0)
    quest:SetStateBool("ClientInUse_0", clientInUse0)
    local clientInUse1 = quest:GetStateBool("ClientInUse_1") or false
    clientInUse1 = quest:PersistTransferBool(context, "ClientInUse[1]", clientInUse1)
    quest:SetStateBool("ClientInUse_1", clientInUse1)
    local clientInUse = quest:GetStateBool("ClientInUse_2") or false
    clientInUse = quest:PersistTransferBool(context, "ClientInUse[2]", clientInUse)
    quest:SetStateBool("ClientInUse_2", clientInUse)
    local beersDrunk = quest:GetStateInt("BeersDrunk") or 0
    beersDrunk = quest:PersistTransferInt(context, "BeersDrunk", beersDrunk)
    quest:SetStateInt("BeersDrunk", beersDrunk)
end

-- V_Bordello.WatchForBordelloStatus (retail 0x00e3a030)
function WatchForBordelloStatus(quest)
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if quest:IsRegionLoaded("Bordello") then
            quest:NewScriptFrame()
        else
            if quest:IsActiveThreadTerminating() then return end
            while not quest:IsRegionLoaded("Bordello") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("PlayerHasLeftRegion", true)
            quest:SetStateBool("HeroPartying", false)
            quest:SetStateBool("MagicianSleeping", false)
            quest:NewScriptFrame()
        end
    until false
end

-- V_Bordello.WatchForDeedStatus (retail 0x00e3a150)
function WatchForDeedStatus(quest)
    local isObjectInThingsPossession
    while not quest:IsRegionLoaded("Bordello") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if quest:GetStateBool("HeroFoundDeeds") then
        quest:IsActiveThreadTerminating()
        return
    end
    local hiddenDeeds = quest:GetThingWithScriptName("M_HiddenDeeds")
    quest:EntitySetInLimbo(hiddenDeeds, true, true)
    while not quest:GetStateBool("HeroFoundDeedsLocation") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntitySetInLimbo(hiddenDeeds, false, true)
    local hero = quest:GetHero()
    isObjectInThingsPossession = quest:IsObjectInThingsPossession("OBJECT_DEEDS_BORDELLO", hero)
    while not isObjectInThingsPossession do
        if not quest:NewScriptFrame() then return end
        local hero2 = quest:GetHero()
        isObjectInThingsPossession = quest:IsObjectInThingsPossession("OBJECT_DEEDS_BORDELLO", hero2)
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("HeroFoundDeeds", true)
end

-- V_Bordello.CreateCustomers (retail 0x00e3a350)
function CreateCustomers(quest)
    local predicateResult
    local timerId = quest:RegisterTimer()
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            quest:DeregisterTimer(timerId)
            return
        end
        while quest:IsRegionLoaded("Bordello") do
            if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
            if quest:GetTimer(timerId) == 0 and quest:GetStateInt("ClientsAlive") < 3 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                quest:SetTimer(timerId, 15)
                if not quest:GetStateBool("BecomeNunnery") then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    local position = quest:GetThingWithScriptName("M_ClientSpawn"):GetPos()
                    quest:CreateCreature("CREATURE_BS_VILLAGER_MALE", position, "BordelloClient", false)
                else
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    local position2 = quest:GetThingWithScriptName("M_ClientSpawn"):GetPos()
                    quest:CreateCreature("CREATURE_BS_VILLAGER_FEMALE", position2, "BordelloClient", false)
                end
            end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        quest:NewScriptFrame()
        predicateResult = quest:IsActiveThreadTerminating()
    until false
end

-- V_Bordello.AdjustTavernPrices (retail 0x00e3a6b0)
function AdjustTavernPrices(quest)
    local getNearestWithDefName
    if quest:IsActiveThreadTerminating() then return end
    while true do
        while not quest:IsRegionLoaded("Bordello") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then break end
        local hero = quest:GetHero()
        getNearestWithDefName = quest:GetNearestWithDefName(hero, "VILLAGE_BORDELLO")
        quest:SetStateThing("BordelloVillage", getNearestWithDefName)
        getNearestWithDefName = nil
        local hero2 = quest:GetHero()
        getNearestWithDefName = quest:GetNearestWithDefName(hero2, "CREATURE_OAKVALE_VILLAGER_FEMALE_BARMAID")
        quest:EntitySetOpinionReactionMask(getNearestWithDefName, "OPINION_REACTION_MASK_DONT_SCREAM")
        getNearestWithDefName = nil
        local hero3 = quest:GetHero()
        getNearestWithDefName = quest:GetNearestWithDefName(hero3, "CREATURE_OAKVALE_VILLAGER_MALE_BARMAN")
        quest:EntitySetOpinionReactionMask(getNearestWithDefName, "OPINION_REACTION_MASK_DONT_SCREAM")
        getNearestWithDefName = nil
        while quest:IsRegionLoaded("Bordello") do
            if not quest:NewScriptFrame() then return end
            if quest:GetStateBool("PlayerOwned") then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetTradingPriceMult(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BordelloHeroTavernPriceMultiplier))
                break
            end
        end
        if quest:IsActiveThreadTerminating() then return end
        while quest:IsRegionLoaded("Bordello") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetTradingPriceMult(1.0)
        quest:SetStateInt("BeersDrunk", 0)
        if not quest:NewScriptFrame() then return end
    end
end

-- V_Bordello.WatchForHeroLeavingRegionWithBeer (retail 0x00e44980)
function WatchForHeroLeavingRegionWithBeer(quest)
    local isRegionLoaded
    while true do
        isRegionLoaded = quest:IsRegionLoaded("Bordello")
        if not isRegionLoaded or not quest:GetStateBool("BeerSetToDPad") then
            isRegionLoaded = false
        end
        if not isRegionLoaded then break end
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("BeerSetToDPad", false)
    quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", 2, -1)
end

-- V_Bordello.IsHeroWearingBeard (retail 0x00e3e320)
-- E3E320: bsim names this body NScript::CV_BordelloScript::IsHeroWearingBeard (a homologous script member); no PDB name
function IsHeroWearingBeard(quest)
    local predicateResult, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    local predicateResult6, predicateResult7, predicateResult8, scratchValue
    predicateResult2 = false
    predicateResult5 = false
    local hero = quest:GetHero()
    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_DRESS") then
        goto LAB_00e3e3d9
    else
        predicateResult2 = true
        predicateResult5 = false
        local hero2 = quest:GetHero()
        if quest:IsWearingClothingItem(hero2, "OBJECT_HERO_TROUSERS_DRESS_GOOD") then goto LAB_00e3e3d9 end
        predicateResult2 = true
        predicateResult5 = true
        local hero3 = quest:GetHero()
        scratchValue = 0
        if quest:IsWearingClothingItem(hero3, "OBJECT_HERO_TROUSERS_DRESS_EVIL") then goto LAB_00e3e3d9 end
    end
    goto FLOW_past_lab_00e3e3d9
    ::LAB_00e3e3d9::
    scratchValue = 1
    ::FLOW_past_lab_00e3e3d9::
    if predicateResult5 then
    end
    if predicateResult2 then
    end
    predicateResult3 = false
    predicateResult6 = false
    local hero4 = quest:GetHero()
    if quest:IsWearingClothingItem(hero4, "OBJECT_HERO_SHIRT_DRESS") then
        goto LAB_00e3e4c2
    else
        predicateResult3 = true
        local hero5 = quest:GetHero()
        if quest:IsWearingClothingItem(hero5, "OBJECT_HERO_SHIRT_DRESS_GOOD") then goto LAB_00e3e4c2 end
        predicateResult3 = true
        predicateResult6 = true
        local hero6 = quest:GetHero()
        predicateResult = false
        if quest:IsWearingClothingItem(hero6, "OBJECT_HERO_SHIRT_DRESS_EVIL") then goto LAB_00e3e4c2 end
    end
    goto FLOW_past_lab_00e3e4c2
    ::LAB_00e3e4c2::
    predicateResult = true
    ::FLOW_past_lab_00e3e4c2::
    if predicateResult6 then
    end
    if predicateResult3 then
    end
    predicateResult7 = false
    if predicateResult then
        scratchValue = scratchValue + 1
    end
    local hero7 = quest:GetHero()
    if quest:IsWearingClothingItem(hero7, "OBJECT_HERO_GLOVES_DRESS") then
        goto LAB_00e3e5ad
    else
        local hero8 = quest:GetHero()
        if quest:IsWearingClothingItem(hero8, "OBJECT_HERO_GLOVES_DRESS_GOOD") then goto LAB_00e3e5ad end
        predicateResult7 = true
        local hero9 = quest:GetHero()
        predicateResult4 = false
        if quest:IsWearingClothingItem(hero9, "OBJECT_HERO_GLOVES_DRESS_EVIL") then goto LAB_00e3e5ad end
    end
    goto FLOW_past_lab_00e3e5ad
    ::LAB_00e3e5ad::
    predicateResult4 = true
    ::FLOW_past_lab_00e3e5ad::
    if predicateResult7 then
    end
    if predicateResult4 then
        scratchValue = scratchValue + 1
    end
    local hero10 = quest:GetHero()
    if quest:IsWearingClothingItem(hero10, "OBJECT_HERO_HAT_WHOREWIG") then
        local hero11 = quest:GetHero()
        predicateResult8 = true
        if quest:IsWearingClothingItem(hero11, "OBJECT_HERO_NO_BOOTS") then goto LAB_00e3e66d end
    end
    predicateResult8 = false
    ::LAB_00e3e66d::
    if predicateResult8 then
        scratchValue = scratchValue + 1
    end
    return scratchValue == 4
end

-- V_Bordello.PlayCutscene (retail 0x00e3e720)
-- E3E720: bsim names this body NScript::CV_BordelloScript::PlayCutscene (a homologous script member); no PDB name
function PlayCutscene(quest, param2, param3)
    local resources = quest:RetailResources()
    quest:SetStateBool("CutscenePlaying", true)
    local resource = resources:NewResource()
    local pThing = quest:GetHero()
    resources:TryAcquire(resource, pThing, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "BOSS", resources:MemberResource("seh_Boss"))
    resources:SetActor(actorMap, "GUARD", resources:MemberResource("seh_Guard"))
    resources:SetActor(actorMap, "MADAM", resources:MemberResource("seh_Madam"))
    resources:SetActor(actorMap, "WHORE", resources:MemberResource("seh_Whore"))
    quest:FixMovieSequenceCamera(true)
    local scratchValue = param3
    if param3 then
        if quest:IsActiveThreadTerminating() then
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            return
        end
        quest:SetCutsceneSkippableWhilePaused(true)
    end
    resources:RunMacroWithStrings(param2, actorMap, resources:MemberStringMap("csargs"), false, true)
    if scratchValue then
        if quest:IsActiveThreadTerminating() then
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            return
        end
        quest:SetCutsceneSkippableWhilePaused(false)
    end
    quest:FixMovieSequenceCamera(false)
    quest:SetStateBool("CutscenePlaying", false)
    resources:ClearStringMap(resources:MemberStringMap("csargs"))
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

-- V_Bordello.GetHeroStatusTextTag (retail 0x00e3e6b0)
-- E3E6B0: bsim names this body NScript::CV_BordelloScript::GetHeroStatusTextTag (a homologous script member); no PDB name
function GetHeroStatusTextTag(quest)
    if quest:GetStateBool("HeroTricking") then
        if IsHeroWearingBeard(quest) then
            return "_HEROWHORE"
        end
    end
    if IsHeroWearingBeard(quest) then
        return "_HEROLADY"
    end
    return "_HEROMAN"
end

-- V_Bordello.helper_E44A40 (retail 0x00e44a40)
function helper_E44A40(quest)
    local flags, scratchValue, predicateResult
    predicateResult = true
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_01") then
        local hero2 = quest:GetHero()
        if not quest:IsWearingHairstyle(hero2, "OBJECT_HERO_BEARD_02") then
            local hero3 = quest:GetHero()
            if not quest:IsWearingHairstyle(hero3, "OBJECT_HERO_BEARD_03") then
                local hero4 = quest:GetHero()
                if not quest:IsWearingHairstyle(hero4, "OBJECT_HERO_BEARD_MUTTON_01") then
                    local hero5 = quest:GetHero()
                    if not quest:IsWearingHairstyle(hero5, "OBJECT_HERO_BEARD_LONG_01") then
                        local hero6 = quest:GetHero()
                        if not quest:IsWearingHairstyle(hero6, "OBJECT_HERO_BEARD_CHIN_01") then
                            local hero7 = quest:GetHero()
                            if not quest:IsWearingHairstyle(hero7, "OBJECT_HERO_BEARD_TRAMP_01") then
                                local hero8 = quest:GetHero()
                                predicateResult = not not quest:IsWearingHairstyle(hero8, "OBJECT_HERO_BEARD_WATSON_01")
                            end
                        end
                    end
                end
            end
        end
    end
    return predicateResult
end

-- V_Bordello.IsHeroWearingTash (retail 0x00e44cc0)
-- E44CC0: bsim names this body NScript::CV_BordelloScript::IsHeroWearingTash (a homologous script member); no PDB name
function IsHeroWearingTash(quest)
    local predicateResult, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    local predicateResult6
    predicateResult6 = true
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMITH_01") then
        local hero2 = quest:GetHero()
        if not quest:IsWearingHairstyle(hero2, "OBJECT_HERO_TASHTRADER_01") then
            local hero3 = quest:GetHero()
            if not quest:IsWearingHairstyle(hero3, "OBJECT_HERO_TASHKHG_01") then
                local hero4 = quest:GetHero()
                if not quest:IsWearingHairstyle(hero4, "OBJECT_HERO_TASHSHERIFF_01") then
                    local hero5 = quest:GetHero()
                    if not quest:IsWearingHairstyle(hero5, "OBJECT_HERO_TASHCHINESE_01") then
                        local hero6 = quest:GetHero()
                        predicateResult6 = not not quest:IsWearingHairstyle(hero6, "OBJECT_HERO_TASHSMALL_01")
                    end
                end
            end
        end
    end
    return predicateResult6
end

