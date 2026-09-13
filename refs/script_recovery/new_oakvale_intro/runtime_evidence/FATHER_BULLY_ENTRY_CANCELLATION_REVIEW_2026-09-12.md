# Father and Bully entry cancellation review

Two source fixes close cancellation gaps before native cutscenes. Both were
reproduced by comparing the actual Lua closures with executed native branches
before editing the port. Neither change is installed or claimed to resolve
Father's reported Escape failure or Bully's visible disappearance.

## Father

Retail checks termination at 0xDB879A before acquiring Hero. Failed acquisition
waits one frame and checks at 0xDB87DA. Successful acquisition checks again at
0xDB8806. Cancellation releases Hero, if constructed, then Father's outer
resource, without opening a movie or setting DadFinishedIntro. The native
movie is not constructed until 0xDB8889.

Previously Lua opened the movie and delegated Hero acquisition to
RunCutsceneWithSetup. That void helper can return without running a macro;
the caller then continued into DadFinishedIntro and post-macro work. Its
internal acquisition also happened after the caller's pause/camera setup.

The intro closure now checks termination, acquires Hero explicitly at priority
4, and rechecks before movie setup. The wrapper reuses those control handles.
Hero remains held through the tutorial/counter and is released after movie
cleanup, matching native release positions 0xDB8AE9 and 0xDB8A6A. Failed
AcquireControl cleans up its own failed handle. No global acquisition or
cutscene-wrapper contract was changed.

Six native entry cases cover pre-entry cancellation, failed acquisition with
cancellation, cancellation after success, ordinary success, retry success,
and cancellation immediately after retry success. All six failed against the
old closure's call ordering and pass after the fix. Eight existing post-macro
Father cases also pass; they now assert Hero stays held until movie cleanup
and is released exactly once. They still begin their termination schedule
after the macro, as their native evidence does.

## Bully

Successful acquisition does not imply that the script should continue. Native
checks at 0xDBC8ED, 0xDBC96C and 0xDBC9E1 follow acquisition of Bully, Hero and
Victim respectively. Both text-selection branches check again at 0xDBCAA5 /
0xDBCACE, before filling the substitution map and constructing the movie.

The port omitted these four checks. It now exits at each boundary and releases
the currently held resources in Victim/Hero/Bully order as applicable. Ten
native comparisons cover normal progress and cancellation at each of the four
queries, for both HeroAttackedVictim values. The initial four-case comparison
failed against the original code; the extended comparison also discriminated
the missing fourth query before that check was added.

Native map construction/destruction is doubled in these comparisons. Lua's
legacy macro helper still owns its temporary maps; this audit certifies the
explicit acquisition/query/release sequence, not identical map lifetime or
actual scheduler ownership. The six existing post-RUN1 completion comparisons
continue to pass. In particular, RUN2 cancellation still follows retail's
completion behavior, and early Escape still follows the authored skip branch.

## Reproduction and validation

Native reproducers use the installed executable bytes with a recorded SHA256;
Unicorn is loaded from work/runtime_re_tools. Engine, string, map and resource
operations are doubles. There is no rendered scene or live fiber execution.

```powershell
python work/runtime-20260912-video-comparison/prove_father_intro_acquisition.py
python work/runtime-20260912-video-comparison/prove_bully_acquired_gates.py
python -m unittest tools.script_recovery.test_father_intro_acquisition tools.script_recovery.test_father_intro_cancellation tools.script_recovery.test_bully_acquired_gates tools.script_recovery.test_bully_completion_cancellation
```

Evidence: native-father-intro-acquisition-20260912.json and
native-bully-acquired-gates-complete-20260912.json. The earlier four-case Bully
artifact is retained as a historical intermediate result.

Package validation: work/father-bully-acquired-package-final-20260912.json,
123/123 fixtures, zero warnings/errors. One new Father fixture forbids movie,
macro, state completion and tutorial work after failed Hero acquisition.
Only Father/Bully traces were refreshed. For every pre-existing Bully fixture,
removing termination-query events makes the old and new event arrays exactly
equal. The two RUN1 cancellation fixtures now schedule termination after the
four new pre-macro queries; their intended cancellation boundary is unchanged.

Clean runtime check output: work/runtime-father-bully-acquired-final-20260912/.
The older runtime proof/release review snapshots predate these Lua edits;
use the new result for current New Oakvale source provenance. No C++ source
or installed build changed for these fixes.
