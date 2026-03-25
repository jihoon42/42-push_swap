# Push_swap Deep Code Review (Logic/Architecture Focus)

## Critical Risks

1. **Unchecked node allocation in parsing can silently corrupt state.**
   - `process_token()` calls `push_back(a, new_node((int)value));` but does not check whether `new_node()` returned `NULL`.
   - `push_back()` simply returns when given `NULL`, and `process_token()` still returns success.
   - Impact: under memory pressure, arguments may be dropped silently, producing incorrect sorting behavior or false validation outcomes.

2. **Normalization allocation failure is ignored, but algorithm continues as if indexing succeeded.**
   - `normalize_data()` returns early if `malloc` fails, with no error propagation.
   - Callers proceed to sorting strategies that rely on normalized indices (`sort_five()` explicitly searches for values `0` and `1`).
   - Impact: potential infinite rotation in `sort_five()` when expected values are absent; undefined project behavior instead of explicit error handling.

3. **Combined op functions always print even when no underlying operation occurred.**
   - `swap_all()`, `rotate_all()`, and `reverse_all()` always emit `ss`, `rr`, `rrr` regardless of effective stack sizes.
   - This is legal in checker semantics but can inflate operation count and harm benchmark margins in corner strategy paths.

## Requirement Compliance Snapshot

- Mandatory executable and bonus checker build through `Makefile` with expected rules and flags.
- Argument validation covers non-integers, overflow, duplicates, and emits `Error\n` on stderr with non-zero exit.
- Core operation set (`sa/sb/ss/pa/pb/ra/rb/rr/rra/rrb/rrr`) is implemented in both mandatory and bonus pathways.
- Checker reads stdin operations and outputs `OK` / `KO`, with invalid operation handling via `Error`.
- Randomized integration checks passed for correctness and benchmark ranges (sampled runs).

## Refactoring Suggestions (Readability / Logic)

1. **Unify duplicated parsing logic between mandatory and bonus.**
   - `srcs/parsing*.c` and `srcs_bonus/parsing*_bonus.c` duplicate almost identical logic.
   - Consider a shared parser module with function pointers/hooks for program-specific behavior.

2. **Make memory-failure paths explicit and deterministic.**
   - Change parsing helpers to return tri-state results (ok / invalid input / alloc failure).
   - Bubble alloc failures up to one centralized error path.

3. **Replace O(n^2) duplicate checks with amortized O(1) or O(log n) strategy.**
   - Current duplicate detection linearly scans deque for each token.
   - For large quoted input strings, this is still acceptable for project scale, but a sorted temporary array or hash set would clarify intent and improve asymptotic behavior.

4. **Improve intent with tighter naming in greedy execution.**
   - Names like `ba`, `bb` are terse. Prefer `best_a_pos`, `best_b_pos` for maintainability.
   - `get_best_case()` should return an enum for readability (`CASE_RR`, `CASE_RRR`, etc.).

5. **Remove dead/unused utility or wire it consistently.**
   - `error_exit()` exists but normal flow uses ad-hoc error handling in `main`/`parse_args`.
   - Consolidate into one approach to reduce divergence and mistakes.

6. **Document data-type choices for maintainers.**
   - `int size`/positions are practical here, but adding a brief comment that project constraints keep sizes safely in `int` range would make choices look intentional.

## Suggested Priority Fix Order

1. Fix allocation-handling bugs (`new_node` + `normalize_data` propagation).
2. Add a small test harness for forced alloc-failure simulation.
3. Reduce parser duplication.
4. Polish naming and control-flow clarity in greedy executor.
