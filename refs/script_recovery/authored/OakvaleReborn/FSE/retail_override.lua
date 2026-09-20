-- Single-authority New Oakvale Intro runtime profile.
-- DEFAULT OFF in source control. Deployment may enable this only for the explicit
-- disposable playtest after preserving rollback copies of the installed profile.
RetailOverrides = {
    enabled = true,
    disposableSaveAcknowledgement = "I UNDERSTAND THIS OVERRIDE MAY CORRUPT DISPOSABLE SAVES",
    allowUnverifiedDisposable = true,
    entries = {
        {
            nativeName = "Q_NewOakValeIntro",
            file = "OakvaleReborn/OakvaleReborn",
            mode = "override",
            evidenceLevel = "reconstructed-source",
            mutatingCallsAllowed = true,
            saveWritesAllowed = true,
            entity_scripts = {
                { name = "NOVI_LiveFather",    file = "OakvaleReborn/Entities/OVR_LiveFather",    id = 200 },
                { name = "NOVI_Theresa",       file = "OakvaleReborn/Entities/OVR_Theresa",       id = 201 },
                { name = "NOVI_Guard",         file = "OakvaleReborn/Entities/OVR_Guard",         id = 202 },
                { name = "NOVI_Villager",      file = "OakvaleReborn/Entities/OVR_Villager",      id = 203 },
                { name = "NOVI_Bully",         file = "OakvaleReborn/Entities/OVR_Bully",         id = 204 },
                { name = "NOVI_Victim",        file = "OakvaleReborn/Entities/OVR_Victim",        id = 205 },
                { name = "NOVI_TeddyGirl",     file = "OakvaleReborn/Entities/OVR_TeddyGirl",     id = 206 },
                { name = "NOVI_AffairMan",     file = "OakvaleReborn/Entities/OVR_AffairMan",     id = 207 },
                { name = "NOVI_AffairWoman",   file = "OakvaleReborn/Entities/OVR_AffairWoman",   id = 208 },
                { name = "NOVI_AffairWife",    file = "OakvaleReborn/Entities/OVR_AffairWife",    id = 209 },
                { name = "NOVI_BookTrader",    file = "OakvaleReborn/Entities/OVR_BookTrader",    id = 210 },
                { name = "NOVI_BarrelMan",     file = "OakvaleReborn/Entities/OVR_BarrelMan",     id = 211 },
                { name = "NOVI_BarrelThug",    file = "OakvaleReborn/Entities/OVR_BarrelThug",    id = 212 },
                { name = "NOVI_Barrel",        file = "OakvaleReborn/Entities/OVR_Barrel",        id = 213 },
                { name = "NOVI_CreatedBeetle", file = "OakvaleReborn/Entities/OVR_CreatedBeetle", id = 214 },
                { name = "OVI_DeadFather",     file = "OakvaleReborn/Entities/OVR_DeadFather",     id = 215 },
            },
        },
    },
}
