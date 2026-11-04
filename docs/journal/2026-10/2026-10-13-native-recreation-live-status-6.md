# Native recreation live status 6: original 2D text wrapper

`CRenderManager2D::Draw2DText 009DD8F0` first work-only C++ source emits all
**109 bytes /34 instructions** after four typed call relocations. Both donors
agree on the exact public signature, including the4B by-value CWideString slot.
It caches both coordinate words before constructing an additional string copy,
passes Z0/scale1 to the existing genuine AddText, destroys the local copy and
incoming parameter, and returns true. No source/profile fitting occurred.

Native comparison passes **98,304 cases /196,608 full64KiB snapshots** across
twelve x87 modes, event traces, bool return, all three cached coordinate words
and x87 status. Seventeen genuine code/data contributions execute, including
actual AddText, wide-string ctor/copy/assignment/Unassign/destruction, quick-info
copy/growth and coordinate/window/dimension code. The existing AddText and
string-assignment sources were compiled once for this new composition; every
other child was reused. No additional child recovery grade or count.

Nine exact original-root omission links fail; full control links. Cases cover
all32 combinations of coordinate mutation after copy, incoming/local string
ownership reassignment through the genuine assignment provider, late colour
mutation and changing coordinate/window inputs. Empty and owned strings, both
queue-capacity branches, flags/high bits, offscreen bytes0/1/128/255 and signed
zero/subnormal/quiet-NaN/signalling-NaN coordinate bits pass. Independent checks
cover cached position, exact argument tuple, both destructions, final string
instance/reference counts and every defined text-record field. Only record
high flag bits/padding are excluded; no union/colour/font/vertex masking.

Allocators, system/global inputs and observable string-copy/assignment/queue
callbacks remain bounded interfaces. The raw4B parameter slot owns its input
reference, and the actual callee performs genuine destruction. This does not
prove actual heap/lifetime construction, UTF/glyph rendering, real font-bank
ownership, GPU or an integrated native frontend frame. No capped font/glyph/
extents work was retried. First source and native fixture pass without failures.

Closed packet: `work/parity_addtext/native_draw2dtext_WORK_ONLY_validated_20261013.json`,
SHA `992cbcba0fd0454b2e6631f65e21467f932f4780a93bb44f7e228a0b94b58fb1`, 1738 controls.

Next: Draw2DBoxColours009DDA50, a515B four-corner-colour rectangle parent (not a
thin wrapper). Both donors identify the array-qualified colour argument. Full
retail evidence packs all four colours before its four coordinate callbacks,
sets Z0/RHW100/UV0 while leaving SpecularColour undefined, selects blend3 for
flag1, otherwise2 for flag2 or1, and queues two Add2DTri calls with null texture.
There is no Draw2DLine-style late-alpha special case here.

Work remains active, native only. Canonical headers/index/grades/dashboard and
previous receipts/journals remain unchanged. Shared-find acceptance980;
promotion waits for the remaining audit. GetFrame and Draw2DLine remain
functional-only below the near gate; capped loading/font targets stay excluded.
No commit, push or game/GPU launch.
