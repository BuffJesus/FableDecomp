# Native recreation live status 15 ? bank metadata and graphic caller

Three original bank metadata providers are work-only validated: GetNoFramesInGraphic 00A013E0, GetGraphicFrameWidth 00A01430 and GetGraphicFrameHeight 00A01480. Each first source matches its complete 68-byte / 22-instruction body, with no relocations and both return paths. Retail RTTI type descriptor 01396D5C identifies CGraphicDataBank; its primary vtable 0129C8CC places these roots in slots 20?22. Both PDB donor names agree. A separately retained RTTI receipt pins the original complete-object locator and exact slot addresses.

Native comparison passes 13,824 cases / 27,648 unchanged 64KiB snapshots: six array sizes, six uint16 patterns, eight index classes, eight frame values and two arena placements. Valid header words are independently checked. Invalid indices cause 9,792 caught access violations, with exact read addresses +0A/+06/+08. The recovered source preserves the unchecked PeekGraphic dereference. Width and height ignore the frame argument. One combined-object omission requires all three exact decorated roots. This does not establish bank construction, lifetime or its complete virtual table.

Closed packet: `work/parity_addtext/native_graphic_bank_metadata_WORK_ONLY_validated_20261013.json`, SHA256 `d936964535361efa814fe3817b0177853c73d2ce67eae0f02eb23761b013136a`, 1776 pinned files.

Current task: Draw2DGraphic 009DCA80, first source 955B versus 971B, 309 versus 312 instructions, 96.940% near. The parent caches actual XY through two coordinate conversions before width/width/height/height virtual calls. Its six arguments are position, bank, graphic index, frame index, drawing flags and packed colour. Frame lookup uses flags 1, then the existing UV flip/inset and ownership/queue sequence. Specular is undefined.

The first native fixture is running. It reuses the passing UV, Add2DTri, queue, ownership, coordinate, dimensions, frame-uninitialise and metadata objects. Width/height interposers call the actual recovered bodies; list switches between two valid records. Separate fixture virtual overrides exercise unsigned 32-bit dimension extremes. GetFrame remains an exact-argument fixture boundary, not loading recovery. Check FIRST_native_behavior/result.json before any rerun. Passing providers were not recompiled.

Native work remains active, without Lua, canonical promotion, header/index/grade/dashboard/coverage changes, GPU/game launch or asset-loading integration. Shared-find acceptance remains through 980; capped loader and font exclusions are unchanged.
