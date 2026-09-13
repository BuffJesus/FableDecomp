-- Offline staging; requires the newer source runtime, not installed v23.
RetailOverrides = {
    enabled = false,
    allowUnverifiedDisposable = false,
    disposableSaveAcknowledgement = "",
    entries = {
        {
            nativeName = "QS_GuardianTrophyDealerInfo",
            file = "GuardianTrophyDealerInfo/GuardianTrophyDealerInfo",
            mode = "override", evidenceLevel = "reconstructed-source",
            mutatingCallsAllowed = true, saveWritesAllowed = true,
            entity_scripts = {
                {name = "GTDI_Maze", file = "GuardianTrophyDealerInfo/Entities/GTDI_Maze", id = 69},
            },
        },
    },
}
