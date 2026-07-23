# Decompilation accelerator assessment

Updated: 2026-07-22. This assessment is specific to the 32-bit VC7.1 Fable TLC reconstruction,
the symbolized `Ego_r.pdb` donor, the existing Ghidra/BSim database, and the curated compile,
behavior, and retail-parity gates.

## Adopt now

### Direct PDB oracle ([`llvm-pdbutil`](https://llvm.org/docs/CommandGuide/llvm-pdbutil.html) + DIA)

Visual Studio already supplies `llvm-pdbutil` and the 64-bit DIA runtime locally. Targeted
`pretty --with-name` queries return authoritative donor signatures, addresses, sizes, parameter
names, and return types in well under a second without opening Ghidra. Targeted class queries also
return complete member layouts.

This is now integrated into `auto-re-agent`: every future target queries `Ego_r.pdb` by qualified
name and adds any exact donor signature to the reversal prompt. The existing TSV class-layout
context remains in place. Manual queries use:

```powershell
tools/query_pdb_oracle.ps1 -Name 'CNavQuadTree::Initialise'
tools/query_pdb_oracle.ps1 -Mode Type -Name 'CNavQuadTree'
```

Expected payoff: fewer invented parameters, incorrect return types, wrong calling-convention
assumptions, and signature-audit repairs. This lane does not contend for the Ghidra project lock.

### [Frida](https://frida.re/docs/stalker/) runtime oracles

Frida and `frida-trace` are already installed. Its Interceptor and IA-32 Stalker support can capture
arguments, returns, call edges, basic blocks, and selected memory writes in the retail executable.
Use it selectively for branch-heavy terrain, navigation, quest-card, save, and renderer functions
where static fixtures cannot settle behavior. It should become a separate runtime-trace lane, not
part of every compile test.

## Pilot next

### [Google BinDiff](https://github.com/google/bindiff)

BinDiff is open source, supports Ghidra and x86, identifies similar functions, and ports function
names, comments, and local names between related binaries. Pilot it on `ego_r.exe` versus retail
`Fable.exe`, concentrating on the small remaining semantic-name tail and ambiguous donor mappings.
It complements BSim's decompiler-feature matching with graph/call-context matching; importing all
matches automatically would be unsafe.

### [reccmp](https://github.com/isledecomp/reccmp)

`reccmp` is designed for old 32-bit MSVC reconstruction projects and provides whole-function match
percentages, stack-layout comparison, global-data comparison, vtable validation, HTML reports, and
progress diffs. It is an excellent fit once reconstructed translation units link into a real PE.
Its documentation explicitly calls newer-than-MSVC-4.20 support work in progress, so VC7.1 must be
validated with a small pilot before making it authoritative.

### [objdiff](https://github.com/encounter/objdiff)

`objdiff` supports x86 and MSVC symbol demangling, automatic rebuild-on-edit, register/value
highlighting, CLI reports, and whole-object progress. It expects an original relocatable object,
while this project currently extracts retail function bodies from the linked executable. A pilot
therefore depends on generating synthetic COFF target objects (including relocation metadata), or
waiting until original/donor objects are available. The present relocation-aware comparator remains
the lower-friction batch oracle.

## Selective or deferred

- **[angr](https://docs.angr.io/en/latest/):** useful for generating path conditions and testing small dependency-light functions with
  hooked imports. State explosion and the size of the Fable engine make it unsuitable as the main
  49,553-function lifting loop.
- **[ghidriff](https://github.com/clearbluejar/ghidriff):** offers headless Ghidra Version Tracking/BSim diffing with JSON and Markdown output.
  Most matching capability overlaps the existing BSim and donor-port pipeline, and it consumes the
  same Ghidra resource. Consider it for reproducible audit reports, not as another concurrent loop.
- **Diaphora:** strong diffing and type/comment transfer, but centered on IDA/Hex-Rays. BinDiff and
  existing Ghidra infrastructure offer a cheaper integration path here.
- **Alternative decompilers (RetDec, rev.ng, Reko):** useful as disagreement oracles on the tiny
  decompiler-failure tail, but Ghidra already produces clean pseudo-C for 49,081/49,082 measured
  functions. They do not address the current bottleneck: buildable, ABI-correct source promotion.

## Recommended execution order

1. Measure the PDB-signature injection on the next Wave3 batch: donor hits, checker retries, and
   signature-audit pass rate.
2. Add a Frida trace fixture for one terrain/nav function and one quest-card path.
3. Pilot BinDiff only on currently ambiguous/unmatched target functions.
4. Test `reccmp` against a small linked VC7.1 proof module before adopting its annotations.
5. Reconsider objdiff after a COFF retail-slice exporter exists.
