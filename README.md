# Exhaustive verification of tree independence-sequence unimodality to n = 32

Computational artifacts for a comment on [Erdős Problem #993](https://www.erdosproblems.com/993)
(Alavi–Malde–Schwenk–Erdős 1987: the independent-set sequence of every tree/forest is unimodal).

**Result: unimodality holds for every free tree on up to 32 vertices.** This extends the
previously published exhaustive frontier (n <= 29; see arXiv:2604.18824 and
[BrettRey/erdos-problem-993](https://github.com/BrettRey/erdos-problem-993), whose n=28/29
counts these runs reproduce exactly with an independent implementation).

## Headline counts (full table in `results_per_order.txt`)
| n | free trees checked | failures |
|---|---|---|
| 28 | 2,023,443,032 | 0 |
| 29 | 5,469,566,585 | 0 |
| **30** | **14,830,871,802** | **0** |
| **31** | **40,330,829,030** | **0** |
| **32** | **109,972,410,221** | **0** |

Every per-order tree count matches OEIS [A000055](https://oeis.org/A000055) exactly,
certifying the generator + parser + DP end-to-end.

## Method
- Tree generation: `gentreeg` (nauty), streamed as edge lists via `listg -eq`.
- Per tree: exact integer independence polynomial by the standard in/out subtree DP
  (`tree_unimodal.cpp`, single file, u64 coefficients), then a rise-then-fall unimodality scan.
- Large orders split by gentreeg's res/mod (n=31 and n=32: 16 parts each).
- Hardware: one Apple M3 Max; n=31 (Jul 29, 2026) took about 8 hours wall-clock in 16 parts; n=32 (Aug 3-4, 2026) took about 23 hours in 16 parts. Every per-order count matches OEIS A000055 exactly, including A000055(32) = 109,972,410,221.

## Reproduce
```
brew install nauty            # or apt install nauty
c++ -O3 -march=native -o tree_unimodal tree_unimodal.cpp
gentreeg -q 20 | listg -eq | ./tree_unimodal      # small order, seconds
gentreeg -q 31 3/16 | listg -eq | ./tree_unimodal # one of 16 parts of n=31
```
A violation, if one existed, prints `VIOLATION` with the offending polynomial; the final
line reports the count of trees processed.

## Disclosure
Pipeline written and run with Claude (Anthropic) assistance; all numerical claims come
from the executed runs recorded here.

— Tyler Satchel Orden, Los Angeles, 2026
