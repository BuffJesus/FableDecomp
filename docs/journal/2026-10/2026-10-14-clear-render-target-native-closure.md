# Clear-render-target qualified native closure

The clear graph passed once: 49,152 cases, 98,304 compared side snapshots, 12 masked x87 modes, exit 0 and empty stderr. Independent full-source review passed all 13 checks, and independent saved-result review passed. No fixture was rerun.

Qualified closure: `work/native-recreation/rendering/009d8cf0/20261014/qualified-closure/WORK_ONLY_validated_NO_NEW_COVERAGE.json`, SHA256 `63ec9adf4c4bc296d90b8c6b992da1adce3e804a0b493ad3d42ac048b9255e77`.

The native result SHA256 is `a89e9255fe94dbb456d233730e4488cb7d36002108a513d22b3c014b84122a8a`; saved-result review SHA256 is `076e3be14c71c99f213f2a5e202c17ab0a6e0a36258ff5e1c02189ee000e386e`; frozen manifest SHA256 is `d154f760897999b8e59675e182de9936f9e5d687ef203d9afe1c4be00aa7a17f`.

Fifteen genuine engine bodies, the actual 14-byte vendor CRT cookie checker, 1,692-byte immutable format table and 4-byte initial cookie contribution were checked against the linked PE. The clear parent is 3,363 bytes versus 3,385 original, with 94.8655% opcode similarity. All 15 original bodies and 82 operand sites were independently checked. The first 80-site inventory missed two table+8 immediate pointers; its failure is preserved. The first caller link omitted unreferenced pixel helpers; the amended caller retains them with typed volatile anchors. Those helpers are not called in this corpus and receive no new behavior credit.

The corpus covers 16 flag values, four shader states, both clear modes, four valid depth-format indices, four colour seeds and two callback mutation settings across 12 FP modes. Some state axes remain coupled. Final arena, globals, event trace and FP endpoints match, with independent per-side guards outside permitted write spans. Native branch checks distinguish one COM clear from one indexed draw plus two viewport calls. These are reviewed finite-domain checks, not saved per-case memory or live FPU evidence.

Indexed payload comparison excludes precisely four source-uninitialized SpecularColour DWORDs at offsets 20, 52, 84 and 116. Stack pointer identity, whole-stack guards and constructed object lifetimes remain unproved. Integer viewport, COM and shader services remain typed substitutes; no legacy naked viewport body is accepted. Current CRT cookie remapping does not establish original boot, cookie initialization or failure-path behavior.

Post-run pin integrity is supported by the retained runner's successful terminal result, without a separate AFTER snapshot. Saved normalized stdout hashes do not establish raw pipe bytes. Historical compiler/header/PATH, near-match, raw COFF metadata, borrowed-storage and backend limitations remain. No canonical promotion, new coverage, game/GPU execution, save/playthrough or x64 acceptance occurred.

Continuation: compose the frontend with the verified rendering graphs. Read-only preparation now identifies 40 original engine roots, 432 relocation operands and actual current code provider candidates. See `2026-10-14-frontend-render-union-preparation.md`.
