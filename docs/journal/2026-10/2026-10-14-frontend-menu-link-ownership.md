# Frontend/menu link and retained-body ownership

Continues [menu host compile readiness](2026-10-14-frontend-menu-host-compile.md).
The preceding turn produced the first retained draft host object. This turn links
that object without any host/provider recompilation and inspects the actual PE.

## Link and six provider omissions

`window-chain-root-continuation/link_menu_draft_readiness.py` copies the six
retained provider objects byte-for-byte under distinct local names for unambiguous
map attribution. The full link passes. Each of six provider-object omissions
fails for exactly its intended engine symbol: parent Draw, visible-area lookup,
window application, virtual-coordinate conversion, target dimensions, menu Draw.
Menu Draw and iterator share one object; no separate iterator omission is claimed.
No failed omission emits a runnable executable.

The first diagnostic parser recognized only LNK2019 referenced-in messages and
missed LNK2001 messages caused by address anchors. Its original result remains
preserved (`87238ec39e159c0a56d9ca6c7a6eb0e88332f80541f032083f680d428193726e`).
A read-only review handles both diagnostic kinds. Its first greedy regex also
mistook the referencing function's decorated name for the missing symbol; that
checker is retained as `omission_diagnostic_greedy_parser_failure.py`. The final
review splits off the referencing-function clause before parsing the missing
symbol. It proves the complete link and all six exact omissions using the saved
logs only, without link reruns or source/provider changes.

Saved omission review SHA256:
`4ac6c4dd2bbd3835c027074f6edd3ceb3f9c7fca454b7b93c5e75cf0aa79ae19`.
All artifacts are under
`work/native-recreation/frontend/0042df9e/20261014/menu-chain-compile-readiness-first/links-first/`.

## Actual executable contributions

`review_menu_linked_bodies.py` checks map ownership and complete relocated PE
bytes for seven selected roots, sized 477/38/42/215/24/59/44 bytes. Every body
matches its retained COFF contribution after resolving all code relocations.
The menu Draw call operand at offset42 reaches the 44-byte iterator selected
from the same sixth retained provider. The fixture supplies neither body.

This proves selected engine code ownership in the linked host, rather than just
absence of host definitions. CRT/service/data references are resolved against
the linker map; their whole implementations are not accepted by this inspection.

Linked-body review SHA256:
`df7950d7ab0d091cf1db86fc4ec7e8242ac801ab94aecb81e6d79fbc86b03e4d`.
Executable SHA256:
`6d333f9e78830ea6812fc39b79824ffb153b0dc05da4431ec5735bf7b9c9c6cd`.

## Remaining before execution

Reconcile the saved frontend/window native result and sealed independent reviews
into a qualified closure without rerunning its completed fixture. Prepare the
fresh menu source/oracle/runner freeze and independent source review. Existing
compile/link artifacts may be retained and bound into that packet; they do not
authorize the first menu composition run. Original oracle setup/fixup execution,
new menu behavior, borrowed map/component lifetime and integrated frontend/game
startup remain unproved.

This turn performs seven linker calls, zero compiler/provider/native calls, no
graphics/game launch and no canonical code/test/index/grade/dashboard changes.
Old fixtures/receipts stay untouched. No Lua recovery, commit or push occurs.
