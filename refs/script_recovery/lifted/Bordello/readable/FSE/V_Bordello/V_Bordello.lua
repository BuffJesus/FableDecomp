-- Readable native conversion: V_Bordello. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- V_Bordello.Main (retail 0x00e39b40)
function Main(quest)
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
    quest:CreateThread("CreateCustomers")  -- native thread body NScript::CV_BordelloScript::CreateCustomers: lift it as function CreateCustomers(quest)
    quest:CreateThread("AdjustTavernPrices")  -- native thread body CScriptGameResourceObjectScriptedThingBase_HandleBordelloNPC: lift it as function AdjustTavernPrices(quest)
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
    quest:SetStateBool("HadSex", quest:PersistTransferBool(context, "HadSex", quest:GetStateBool("HadSex")))
    quest:SetStateBool("HeroPartying", quest:PersistTransferBool(context, "HeroPartying", quest:GetStateBool("HeroPartying")))
    quest:SetStateBool("HeroTricking", quest:PersistTransferBool(context, "HeroTricking", quest:GetStateBool("HeroTricking")))
    quest:SetStateBool("MagicianSleeping", quest:PersistTransferBool(context, "MagicianSleeping", quest:GetStateBool("MagicianSleeping")))
    quest:SetStateBool("PlayerOwned", quest:PersistTransferBool(context, "PlayerOwned", quest:GetStateBool("PlayerOwned")))
    quest:SetStateBool("BecomeNunnery", quest:PersistTransferBool(context, "BecomeNunnery", quest:GetStateBool("BecomeNunnery")))
    quest:SetStateBool("PlayerHasLeftRegion", quest:PersistTransferBool(context, "PlayerHasLeftRegion", quest:GetStateBool("PlayerHasLeftRegion")))
    quest:SetStateBool("HeroFoundDeeds", quest:PersistTransferBool(context, "HeroFoundDeeds", quest:GetStateBool("HeroFoundDeeds")))
    quest:SetStateBool("HeroFoundDeedsLocation", quest:PersistTransferBool(context, "HeroFoundDeedsLocation", quest:GetStateBool("HeroFoundDeedsLocation")))
    quest:SetStateBool("ClientInUse_0", quest:PersistTransferBool(context, "ClientInUse[0]", quest:GetStateBool("ClientInUse_0")))
    quest:SetStateBool("ClientInUse_1", quest:PersistTransferBool(context, "ClientInUse[1]", quest:GetStateBool("ClientInUse_1")))
    quest:SetStateBool("ClientInUse_2", quest:PersistTransferBool(context, "ClientInUse[2]", quest:GetStateBool("ClientInUse_2")))
    quest:SetStateInt("BeersDrunk", quest:PersistTransferInt(context, "BeersDrunk", quest:GetStateInt("BeersDrunk") or 0))
end

-- V_Bordello.WatchForBordelloStatus (retail 0x00e3a030)
function WatchForBordelloStatus(quest)
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if quest:IsRegionLoaded("Bordello") then
            quest:NewScriptFrame()
        else
            while not quest:IsRegionLoaded("Bordello") do
                if not quest:NewScriptFrame() then return end
            end
            quest:SetStateBool("PlayerHasLeftRegion", true)
            quest:SetStateBool("HeroPartying", false)
            quest:SetStateBool("MagicianSleeping", false)
            quest:NewScriptFrame()
        end
    until false
end

-- V_Bordello.WatchForDeedStatus (retail 0x00e3a150)
function WatchForDeedStatus(quest)
    local hero = quest:GetHero()
    while not quest:IsRegionLoaded("Bordello") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if quest:GetStateBool("HeroFoundDeeds") then
        return
    end
    local hiddenDeeds = quest:GetThingWithScriptName("M_HiddenDeeds")
    quest:EntitySetInLimbo(hiddenDeeds, true, true)
    while not quest:GetStateBool("HeroFoundDeedsLocation") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntitySetInLimbo(hiddenDeeds, false, true)
    while not quest:IsObjectInThingsPossession("OBJECT_DEEDS_BORDELLO", hero) do
        if not quest:NewScriptFrame() then return end
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
                quest:SetTimer(timerId, 15)
                if not quest:GetStateBool("BecomeNunnery") then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    local clientSpawn = quest:GetThingWithScriptName("M_ClientSpawn")
                    quest:CreateCreature("CREATURE_BS_VILLAGER_MALE", clientSpawn:GetPos(), "BordelloClient")
                else
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                    local clientSpawn2 = quest:GetThingWithScriptName("M_ClientSpawn")
                    quest:CreateCreature("CREATURE_BS_VILLAGER_FEMALE", clientSpawn2:GetPos(), "BordelloClient")
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
    local hero = quest:GetHero()
    if quest:IsActiveThreadTerminating() then return end
    while true do
        while not quest:IsRegionLoaded("Bordello") do
            if not quest:NewScriptFrame() then return end
        end
        quest:SetStateThing("BordelloVillage", quest:GetNearestWithDefName(hero, "VILLAGE_BORDELLO"))
        quest:EntitySetOpinionReactionMask(quest:GetNearestWithDefName(hero, "CREATURE_OAKVALE_VILLAGER_FEMALE_BARMAID"), "OPINION_REACTION_MASK_DONT_SCREAM")
        quest:EntitySetOpinionReactionMask(quest:GetNearestWithDefName(hero, "CREATURE_OAKVALE_VILLAGER_MALE_BARMAN"), "OPINION_REACTION_MASK_DONT_SCREAM")
        while quest:IsRegionLoaded("Bordello") do
            if not quest:NewScriptFrame() then return end
            if quest:GetStateBool("PlayerOwned") then
                quest:SetTradingPriceMult(quest:ReadGlobalGameDataFloat(1276))
                break
            end
        end
        if quest:IsActiveThreadTerminating() then return end
        while quest:IsRegionLoaded("Bordello") do
            if not quest:NewScriptFrame() then return end
        end
        quest:SetTradingPriceMult(1.0)
        quest:SetStateInt("BeersDrunk", 0)
        if not quest:NewScriptFrame() then return end
    end
end

-- V_Bordello.null (retail 0x00e44980)
function null(quest)
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
    local predicateResult, predicateResult2, predicateResult3, scratchValue
local hero = quest:GetHero()
    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_DRESS") then
        goto LAB_00e3e3d9
    else
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_DRESS_GOOD") then goto LAB_00e3e3d9 end
        scratchValue = 0
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_DRESS_EVIL") then goto LAB_00e3e3d9 end
    end
    goto FLOW_past_lab_00e3e3d9
    ::LAB_00e3e3d9::
    scratchValue = 1
    ::FLOW_past_lab_00e3e3d9::
    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_DRESS") then
        goto LAB_00e3e4c2
    else
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_DRESS_GOOD") then goto LAB_00e3e4c2 end
        predicateResult = false
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_DRESS_EVIL") then goto LAB_00e3e4c2 end
    end
    goto FLOW_past_lab_00e3e4c2
    ::LAB_00e3e4c2::
    predicateResult = true
    ::FLOW_past_lab_00e3e4c2::
    if predicateResult then
        scratchValue = scratchValue + 1
    end
    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_DRESS") then
        goto LAB_00e3e5ad
    else
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_DRESS_GOOD") then goto LAB_00e3e5ad end
        predicateResult2 = false
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_DRESS_EVIL") then goto LAB_00e3e5ad end
    end
    goto FLOW_past_lab_00e3e5ad
    ::LAB_00e3e5ad::
    predicateResult2 = true
    ::FLOW_past_lab_00e3e5ad::
    if predicateResult2 then
        scratchValue = scratchValue + 1
    end
    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_HAT_WHOREWIG") then
        predicateResult3 = true
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_NO_BOOTS") then goto LAB_00e3e66d end
    end
    predicateResult3 = false
    ::LAB_00e3e66d::
    if predicateResult3 then
        scratchValue = scratchValue + 1
    end
    return scratchValue == '\x04'
end

-- V_Bordello.PlayCutscene (retail 0x00e3e720)
-- E3E720: bsim names this body NScript::CV_BordelloScript::PlayCutscene (a homologous script member); no PDB name
function PlayCutscene(quest, param2, param3)
    local resources = quest:RetailResources()
    quest:SetStateBool("CutscenePlaying", true)
    local resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "BOSS", resources:MemberResource("seh_Boss"))
    resources:SetActor(actorMap, "GUARD", resources:MemberResource("seh_Guard"))
    resources:SetActor(actorMap, "MADAM", resources:MemberResource("seh_Madam"))
    resources:SetActor(actorMap, "WHORE", resources:MemberResource("seh_Whore"))
    quest:FixMovieSequenceCamera(true)
    if param3 then
        if quest:IsActiveThreadTerminating() then
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            return
        end
        -- TODO(native): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding
        quest:SetCutsceneSkippableWhilePaused(true)
    end
    resources:RunMacroWithStrings(param2, actorMap, resources:MemberStringMap("csargs"), false, true)
    if param3 then
        if quest:IsActiveThreadTerminating() then
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            return
        end
        -- TODO(native): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding
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
    local flags, predicateResult
    local hero = quest:GetHero()
    flags = 1
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_01") then
        flags = 3
        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_02") then
            flags = 7
            if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_03") then
                flags = 15
                if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_MUTTON_01") then
                    flags = 31
                    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_LONG_01") then
                        flags = 63
                        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_CHIN_01") then
                            flags = 127
                            if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_TRAMP_01") then
                                flags = 255
                                predicateResult = false
                                if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_WATSON_01") then goto LAB_00e44c26 end
                            end
                        end
                    end
                end
            end
        end
    end
    predicateResult = true
    ::LAB_00e44c26::
    if flags < 0 then
        flags = flags & 127
    end
    if flags & 64 ~= 0 then
        flags = flags & 191
    end
    if flags & 32 ~= 0 then
        flags = flags & 223
    end
    if flags & 16 ~= 0 then
        flags = flags & 239
    end
    if flags & 8 ~= 0 then
        flags = flags & 247
    end
    if flags & 4 ~= 0 then
        flags = flags & 251
    end
    return predicateResult
end

-- V_Bordello.IsHeroWearingTash (retail 0x00e44cc0)
-- E44CC0: bsim names this body NScript::CV_BordelloScript::IsHeroWearingTash (a homologous script member); no PDB name
function IsHeroWearingTash(quest)
    local predicateResult
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMITH_01") then
        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHTRADER_01") then
            if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHKHG_01") then
                if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSHERIFF_01") then
                    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHCHINESE_01") then
                        predicateResult = false
                        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMALL_01") then goto LAB_00e44e30 end
                    end
                end
            end
        end
    end
    predicateResult = true
    ::LAB_00e44e30::
    return predicateResult
end

