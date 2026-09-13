-- Offline staging. Merge only after runtime/API and disposable-profile review.
RetailOverrides = {
    enabled = false,
    allowUnverifiedDisposable = false,
    disposableSaveAcknowledgement = "",
    entries = {
        {
            nativeName = "V_MazeResearch", file = "MazeResearch/MazeResearch",
            mode = "override", evidenceLevel = "reconstructed-source",
            mutatingCallsAllowed = true, saveWritesAllowed = true,
            entity_scripts = {
                {name = "EmptyGrave", file = "MazeResearch/Entities/EmptyGrave", id = 64},
                {name = "HistoryBookcase", file = "MazeResearch/Entities/HistoryBookcase", id = 65},
            },
        },
    },
}
