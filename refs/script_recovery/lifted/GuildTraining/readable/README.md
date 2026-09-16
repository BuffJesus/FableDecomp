# GuildTraining readable review artifact

This directory contains six native reviewed entity slices from the Hero's Guild
training quests. `FSE/GuildTraining/quests.lua` intentionally registers nothing.
The artifact is disabled and is not an installable gameplay candidate.

Native ownership evidence is in `refs/script_recovery/guild_training/`; the state
access ledger must be consulted before naming fields in the remaining tutorial
entities. Focused offline tests are under `tools/script_recovery/test_guild_*.py`.
