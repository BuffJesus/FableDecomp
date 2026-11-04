# Visible frontend checkpoint - 2026-09-26

The launched VisualCheckpoint executable is a readable C++ integration build. It
executes verified recovered functions, but authored adapters still coordinate the
visible screens, renderer, input and subsystem boundaries. Recent native font
acquisition work is in the separate native dependency closure, not this window.
Do not describe the visible checkpoint as the complete reconstructed retail frontend.

Artwork/font sources are retail banks. LoadFrontendArtwork in
rebuild/integration/visual_boot_checkpoint.cpp prefers extracted BMPs under
`data/frontend`, then embedded resources. Some menu text is pre-rendered into
component atlases; other text is assembled from extracted glyph atlases. This is
not the complete native runtime container/font-loading path and does not meet the
user's current runtime-container-only standard. Retail-derived layout/actions and
focused behavior checks do not prove whole-frontend equivalence.

At the user's request, Games for Windows - LIVE is hidden from the current menu.
Input and selection already skipped that child; visual_boot_d3d9.cpp now renders
only the same six offline children, preserving their retail positions. Both normal
and BuffJesus component atlases use that same draw path. Retail reconstruction
function bodies, source assets and serialized definitions were not changed.

Validation of the removal: full gates passed (123 boot leaves, 487 header
dependents, zero regressions, full bootstrap); main-menu transition, wrap navigation
and highlight smoke passed. Its captured image confirms the six visible rows and
no LIVE entry. This does not certify all subscreens or native frontend equivalence.

The user then explicitly rejected reliance on authored adapters for the visible
program. The checkpoint was not relaunched as the requested result. Native retail
startup and runtime container loading are now the frontend priority.
