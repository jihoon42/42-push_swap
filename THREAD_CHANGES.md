# Thread Changes

This file records the code changes made together in this thread.

## Summary

Two checkpoints were created:

- `96f0f95` - `Fix parse overflow and bonus relinking`
- `403c55f` - `Optimize small sorts and restore norm`

## 1. Parse Overflow Fix

Files:

- `srcs/parsing_utils.c`
- `srcs_bonus/parsing_utils_bonus.c`

Reason:

- Very large numeric inputs could overflow during conversion before the later
  `INT_MIN` / `INT_MAX` check.
- That could let invalid input be accepted instead of printing `Error\n`.

What changed:

- `ft_atol` was changed to guard overflow before `nbr * 10 + digit`.
- The same checked conversion logic was applied to both mandatory and bonus
  parsing.

Result:

- Huge positive and negative values are now rejected correctly in both
  `push_swap` and `checker`.

## 2. Makefile Bonus Relink Fix

File:

- `Makefile`

Reason:

- `make bonus` was relinking `checker` even when nothing changed.

What changed:

- `bonus` now depends on `$(BONUS_NAME)`.
- `$(BONUS_NAME)` owns the real link rule.

Result:

- A second `make bonus` now reports that nothing needs to be done.

## 3. Small Input Optimality Improvement

Files:

- `includes/push_swap.h`
- `srcs/sort_small.c`
- `srcs/sort_small_ops.c`
- `srcs/sort_small_utils.c`

Reason:

- The previous `n <= 5` path was correct but not minimal for some `4` and `5`
  element cases.
- Example: `./push_swap 2 1 3 4` produced 4 moves instead of 1.

What changed:

- The small-sort path now solves normalized states for `n <= 5` with a bounded
  BFS over legal push_swap operations.
- The shortest sequence found is replayed on the real stacks.
- The helper code was split into small files so the project still passes
  `norminette`.

Result:

- `n = 2..5` now uses optimal move counts for every permutation.

## Verification Performed

- `make`
- `make bonus`
- `norminette includes srcs srcs_bonus libft`
- Exhaustive optimality check for every permutation of `n = 2..5`
- Checker validation that generated sequences produce `OK`
- Overflow reproducers such as:
  - `18446744073709551615`
  - `-18446744073709551615`

## Notes

- The local build binaries `push_swap` and `checker` were left untracked.
- These thread changes were pushed to `origin/main`.
