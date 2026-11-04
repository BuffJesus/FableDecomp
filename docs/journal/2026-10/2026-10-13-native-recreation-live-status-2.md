# Native recreation live status 2: scaled-graphic parent

Work remains active under the user's October 13 continuation request, excluding
Lua. Add2DTri's closed work-only evidence is unchanged.

`CRenderManager2D::Draw2DScaledGraphic 009DD210` now has a genuine work-only
840-byte VC7.1 O2/MT candidate against the 860-byte original. Its decorated
signature agrees with Ego_r and FableWin. Similarity is **95.063985%**, 271
instructions versus 276; complete decode, passing near gate. Source fitting
stopped. Argument order is box, bank, graphic index, frame index, drawing flags;
earlier declaration parameter labels were historical, while their ABI was right.

The original performs four virtual-to-actual conversions, gets a graphic frame
with mip zero and flags 0x11, copies its 32-byte UV box, selects normal/X/Y UV
ordering (X takes precedence), reads width and height separately, adds 0.01/size
directly in the UV loop, sets Z/RHW, selects blend mode and calls Add2DTri twice.
The second call rereads the owned texture pointer. The donor's unoptimized body
corroborates division inside each UV addition; named float offsets prematurely
rounded U and moved arithmetic before the second callback under VC7.1.

Native PASS: **497,664 cases / 995,328 full 64KiB arena snapshots**, twelve x87
modes, return values, callback event traces and x87 status. Twenty-six genuine
COFF bodies execute, including retained UV, Add2DTri and vertex overflow plus
actual queue, ownership, coordinate, dimension and frame-cleanup providers.
Eight exact provider omissions fail; the full control links. Frame lookup is
explicitly a fixture callback with the exact original argument tuple. This is
not a recovered asset loader or a startup/visual launch.

Cases cover blend/flip precedence and alpha flags, four dimension shapes, spare
queue capacities 0–3, false lookup with/without an owned output, successful
lookup with ordinary or last-reference cleanup, initially offscreen rendering,
and mutations to coordinate inputs, windows, queue pointers, frame selection
and COM/allocation callbacks. An offscreen mutation during the first triangle
lets that triangle finish and makes the second Add2DTri return at entry.

SpecularColour is never assigned by retail. Its values in bounded vertex
buffers, including unused capacity, are excluded. High flags/padding and the
unused geometry-record union/colour/font bytes are excluded at every observed
copy destination. All other arena bytes are compared. No invented initialization
or live lifetime/allocator/COM/GPU behavior is claimed.

All attempts are retained. First source 812B/89.385% and assignment amendment
840B/89.541% miss the gate. An accessor preparation asserted the wrong canonical
spelling and stopped before source/compilation; its dependent driver stopped at
the missing manifest. The canonical mip-level width/height accessors already
exist. Using them yields 90.275% near, but emitted arithmetic still had a material
rounded-U discrepancy. Donor loop evidence justifies the final expression
correction. The original unrun fixture's offscreen invariant was corrected in
a separate source. Two actual fixture stops remain: three incorrectly escaped
diagnostic newlines at compilation, then const UV tables with internal linkage
at linking. The separate table-linkage amendment passes without rebuilding any
production provider.

Closed packet: `work/parity_addtext/native_scaled_graphic_WORK_ONLY_validated_20261013.json`,
SHA `333f2f951c380c1975edcae52510b5a6befbeb401eccf8f821548032ed42a59c`, 1832 controls.

Next parent dependency: `CGraphicInfo::GetFrame 009FF8D0`. First source 599B/84.382%
and shared-loading-tail amendment 547B/89.97555% are compiled but below the near
gate; neither is accepted. A bounded original/compiled x86 comparison is in
progress with actual LRU helpers and frame cleanup, while construction, loading,
forced loading and allocator/virtual deletion remain explicit callbacks. No
attempt to retry capped LoadGraphicFrame or fit the remaining tiny score gap.

Canonical headers/index/grades/dashboard, previous receipts and live statuses
remain unchanged. Shared-find acceptance still ends at 980; promotion waits for
the full shared-header audit. No commit, push, game/GPU launch or Lua work.
