# Native recreation live status 5: cropped graphic drawing

Work is active, native only. Previous packets and canonical files remain frozen.

`CRenderManager2D::Draw2DScaledGraphicWithUVs 009DD570` now has work-only validated
genuine C++: **875B / 276 instructions** versus884B/279, **94.774775% near**.
Both donors corroborate the six-argument public signature and original crop
interpolation. Source fitting stopped after passing near and native behavior.

It converts the rectangle, calls GetFrame with mip0/flags1 (not the other
drawer's flags0x11), copies the actual UV box, interpolates four U/four V edges
without rounded delta temporaries, applies X-priority optional UV flips,
selects the original blend and calls Add2DTri twice with late texture rereads.
There is no 0.01/pixel inset or two additional dimension getters in this sibling.

First candidate798B/248 instructions is retained: SetRHWFromZ became a separate
call despite being inline in the complete original009DD79A..7F7. Its instruction
delta11.11% and similarity87.666% fail. A separate isolated header amendment
forces only that already recovered template method inline; parent source,
algorithm and O2/MT profile are unchanged. This yields the passing875B candidate.
No other source fitting or passing-provider recompilation occurred.

Native comparison passes **497,664 cases / 995,328 full 64KiB snapshots** across
twelve x87 modes, complete event traces, return values and x87 status. All
twenty-six child bodies are reused from the validated uncropped composition.
Eight exact provider omissions fail and the full control links.

Cases cover full/subrange/outside0–1/inverted crop rectangles, distinct crop vs
alias with the caller rectangle, descriptor callbacks mutating crop input after
coordinate conversion, four texture dimension shapes, alpha/boolean-alpha and
nine flags including combined X/Y flip precedence, queue capacities0/3 spanning
both growth branches, lookup false/owned false/success/last-reference cleanup,
offscreen state and mutable frame/window/queue/COM/allocator callbacks. Lookup
is an explicit exact-argument fixture boundary; this is not asset loading.

Undefined SpecularColour in bounded vertex buffers (including unused capacity)
and unused geometry-record high flags/padding/union/colour/font bytes remain
excluded from comparison. No invented initialization, live object construction,
actual allocator/COM/GPU ownership or game/frontend-frame claim.

Closed packet: `work/parity_addtext/native_scaled_graphic_uvs_WORK_ONLY_validated_20261013.json`,
SHA `ada9f648019401c95efb45e9e0ce72f07a723fdda60270186ff5723a2dd49c69`, 1822 controls.

Draw2DLine's separate native functional-only packet is
`work/parity_addtext/native_draw2dline_WORK_ONLY_functional_NOT_ACCEPTED_20261013.json`,
SHA `507caa19495838eb3d622916a76dc496ab9021a5853d5c7c6a96d86da3e17e81`, 1760 controls.
Its231B/88.312% near gate stays false. GetFrame's547B/89.97555% gate also stays
false; no broad exception or canonical coverage gain.

Next: Draw2DText009DD8F0, the thin public wrapper over existing AddText. Full
retail/PDB evidence shows coordinates cached before an additional wide-string
copy, then AddText at scale1, local-string and by-value-parameter destruction,
and true return. Capped font/glyph/extents and LoadGraphicFrame remain excluded.
Both rebuilding background tasks remain disabled. Shared-find acceptance980;
promotion waits for the full audit. No commit/push/game/GPU launch or Lua.
