# Seven baseline fixture contexts: recipes and syntax readiness

The [transitive audit](2026-10-14-gameplay-header-transitive-impact.md) preserved
seven original-header failures. This continuation traces their actual causes and
prepares local corrections/inputs without changing canonical tests or rerunning
passing providers/native fixtures.

## Three source-level fixture defects

Both main-game destructor tests include `test_support/base_class_stubs.h`, which
already defines the empty `CBaseClass` destructor. Each also defines the same empty
body locally. New local copies remove only that duplicate definition. The shared
empty behavior and the separately logged noncopyable-base destructor remain; no
assertion or event logger is removed.

The frontend Run fixture declares `template<>` on
`CTBaseSingleton<CFileInstaller>::Release`. The shared installer-base header
already explicitly specializes the class, so its member is an ordinary static
member definition. A local copy removes only `template<>`; the installer-release
logging body remains byte-identical.

All six original/staged syntax checks pass for those three corrected copies.
Result SHA256:
`c74a3be25b00bcbd7e3b0615b396deaa166b4bc3137bcec9911ea94bfedbd469`.
No behavioral assertion, boundary logging or canonical test is weakened or edited.
Linking and actual behavior remain unverified.

## Three generated-header recipe dependencies

The primitive FrameEnd, composed frame-scene and primitive Render gates generate
their retail/alias headers into private temporary lookup directories before
compiling their fixtures. The earlier common-profile census did not invoke those
entrypoints or provide their generated directories, so the missing-header
diagnostics were genuine lookup failures, not new seam problems.

Fresh local headers are prepared from the original gate literal specifications
and complete original retail instruction spans. The scene span list is checked
independently against the original gate AST. Its seven actual operand fixups and
the primitive Render's sixty-three fixups are decoded from the original bytes.
No provider trial, original-code execution, native gate entrypoint or fixture run
occurs. The dynamically discovered scene erase symbol is not needed for these
address/byte tables and is not recovered by recompiling a passing provider.

Four local headers prepared; result SHA256:
`6cded957eaedd1d2c369dd4eace6d2f8d61b490d2943087b0371e751842b26ff`.
They contain retail-derived local oracle spans and remain under `work/`; they
must not be committed.

With that explicit generated-header directory, all six original/staged fixture
syntax checks pass using the original gate `/O2 /MT` flags with `/Zs` replacing
`/c`. This records current readiness, not full historical environment/profile
identity, link closure or runtime acceptance.
Result SHA256:
`4d6684a1ddf58bf6832ab0489cae7640ccb82c3b58e0ea63f43b691f4eba5078`.

## Text-initialization fixture remains qualified

The unchanged text-initialization fixture already parsed with the staged header
in its first comparison (original exit 2, staged exit 0). The original display
include reaches the shader-data-bank header, which reaches the real
`NGameText::CDataBank` definition and conflicts with the fixture's explicitly
documented local two-slot bank substitute. Removing that unrelated display
dependency removes the collision; no fresh syntax rerun is needed.

The local bank substitute has a deliberately limited vtable/buffer scope and does
not prove real text-bank construction, inheritance/lifetime or loading. Its
original-header failure is retained as a baseline qualification, not silently
upgraded to an original/staged comparison pass.

## Current status

All 136 uncapped test/runtime sources have staged syntax readiness under their
recorded common or restored-gate setup: 129 earlier comparable passes, three
corrected source contexts, three restored-header contexts and the one unchanged
staged-only text fixture. This is mixed-profile syntax evidence, not 136 native
tests or full operational fixture-profile acceptance. InitLUA remains excluded.

A concrete local eleven-file unapplied patch now includes the public-header seam,
four implementation display includes, three test display includes and the three
source-level fixture corrections. The generated oracle headers are not part of
that patch. Earlier five-file patch, original baseline failures, source copies
and receipts remain preserved.

Canonical source/header/tests/index/grades, dashboards and old native receipts
remain unchanged. This turn made twelve syntax-only calls, no provider compiler
call, no link/native/original-code/GPU/game invocation, no Lua recovery and no
commit or push. Actual link/runtime and complete native-promotion gates remain.
