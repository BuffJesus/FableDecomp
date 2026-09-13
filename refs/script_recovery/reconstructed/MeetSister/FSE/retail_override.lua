-- Offline staging only. Do not enable on the New Oakvale test profile.
RetailOverrides = {
    enabled = false,
    allowUnverifiedDisposable = false,
    disposableSaveAcknowledgement = "",
    entries = {
        {
            nativeName = "QS_MeetSister",
            file = "MeetSister/MeetSister",
            mode = "override",
            evidenceLevel = "reconstructed-source",
            mutatingCallsAllowed = true,
            saveWritesAllowed = true,
            entity_scripts = {
                { name = "MeetSisterMessenger", file = "MeetSister/Entities/MeetSisterMessenger", id = 220 },
                { name = "MeetSisterSister", file = "MeetSister/Entities/MeetSisterSister", id = 221 },
            },
        },
    },
}
