-- Single-authority New Oakvale Intro runtime profile.
-- DEFAULT OFF in source control. Deployment may enable this only for the explicit
-- disposable playtest after preserving rollback copies of the installed profile.
RetailOverrides = {
    enabled = false,
    disposableSaveAcknowledgement = "I UNDERSTAND THIS OVERRIDE MAY CORRUPT DISPOSABLE SAVES",
    allowUnverifiedDisposable = true,
    entries = {
        {
            nativeName = "Q_NewOakValeIntro",
            file = "NewOakValeIntro/NewOakValeIntro",
            mode = "override",
            evidenceLevel = "reconstructed-source",
            mutatingCallsAllowed = true,
            saveWritesAllowed = true,
            entity_scripts = {
                { name = "NOVI_LiveFather",    file = "NewOakValeIntro/Entities/NOVI_LiveFather",    id = 200 },
                { name = "NOVI_Theresa",       file = "NewOakValeIntro/Entities/NOVI_Theresa",       id = 201 },
                { name = "NOVI_Guard",         file = "NewOakValeIntro/Entities/NOVI_Guard",         id = 202 },
                { name = "NOVI_Villager",      file = "NewOakValeIntro/Entities/NOVI_Villager",      id = 203 },
                { name = "NOVI_Bully",         file = "NewOakValeIntro/Entities/NOVI_Bully",         id = 204 },
                { name = "NOVI_Victim",        file = "NewOakValeIntro/Entities/NOVI_Victim",        id = 205 },
                { name = "NOVI_TeddyGirl",     file = "NewOakValeIntro/Entities/NOVI_TeddyGirl",     id = 206 },
                { name = "NOVI_AffairMan",     file = "NewOakValeIntro/Entities/NOVI_AffairMan",     id = 207 },
                { name = "NOVI_AffairWoman",   file = "NewOakValeIntro/Entities/NOVI_AffairWoman",   id = 208 },
                { name = "NOVI_AffairWife",    file = "NewOakValeIntro/Entities/NOVI_AffairWife",    id = 209 },
                { name = "NOVI_BookTrader",    file = "NewOakValeIntro/Entities/NOVI_BookTrader",    id = 210 },
                { name = "NOVI_BarrelMan",     file = "NewOakValeIntro/Entities/NOVI_BarrelMan",     id = 211 },
                { name = "NOVI_BarrelThug",    file = "NewOakValeIntro/Entities/NOVI_BarrelThug",    id = 212 },
                { name = "NOVI_Barrel",        file = "NewOakValeIntro/Entities/NOVI_Barrel",        id = 213 },
                { name = "NOVI_CreatedBeetle", file = "NewOakValeIntro/Entities/NOVI_CreatedBeetle", id = 214 },
                { name = "OVI_DeadFather",     file = "NewOakValeIntro/Entities/OVI_DeadFather",     id = 215 },
            },
        },
    },
}
