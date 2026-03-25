*This project has been created as part of the 42 curriculum by jkim2.*

# push_swap

> *Because Swap_push doesn't feel as natural*

---

## Table of Contents

1. [Description](#description)
2. [Algorithm Overview](#algorithm-overview)
3. [Data Structure](#data-structure)
4. [Algorithm Phases in Detail](#algorithm-phases-in-detail)
5. [Bonus: Checker Program](#bonus-checker-program)
6. [Instructions](#instructions)
7. [Usage Examples](#usage-examples)
8. [Benchmark Results](#benchmark-results)
9. [File Organization](#file-organization)
10. [Development Log](#development-log)
11. [Known Limitations & Future Improvements](#known-limitations--future-improvements)
12. [Resources](#resources)

---

## Description

**push_swap** is a sorting project from the 42 Common Core curriculum. The goal is
to sort a random sequence of integers into ascending order in **stack A**, using only
two stacks (A and B) and a fixed set of 11 operations — while minimizing the total
number of operations printed to stdout.

The available operations are:

| Operation | Effect |
|-----------|--------|
| `sa`  | Swap the top two elements of stack A |
| `sb`  | Swap the top two elements of stack B |
| `ss`  | Perform `sa` and `sb` simultaneously |
| `pa`  | Push the top element of B onto the top of A |
| `pb`  | Push the top element of A onto the top of B |
| `ra`  | Rotate A upward — the first element becomes the last |
| `rb`  | Rotate B upward — the first element becomes the last |
| `rr`  | Perform `ra` and `rb` simultaneously |
| `rra` | Reverse rotate A — the last element becomes the first |
| `rrb` | Reverse rotate B — the last element becomes the first |
| `rrr` | Perform `rra` and `rrb` simultaneously |

The program must handle all error cases (non-integer arguments, values outside
`INT_MIN`..`INT_MAX`, duplicates) by printing `Error\n` to stderr and exiting.
When given no arguments, it prints nothing and returns silently. When the input is
already sorted, it also produces no output.

This implementation uses a **Coordinate Compression + 3-Way Partitioning +
Cost-based Greedy** strategy for large inputs (`n > 5`) and a **BFS-based optimal
solver** for small inputs (`n ≤ 5`), comfortably achieving operation counts within
the project's benchmarks.

---

## Algorithm Overview

The sorting process is divided into four main phases:

```
Phase 1: Parse & Index
  argv → validate → build stack A → coordinate compress to [0, N-1]

Phase 2: 3-Way Partition (A → B)
  Divide elements into three groups using two pivots (N/3, 2N/3)
  Push small and mid groups to B; keep large group in A
  Sort the remaining 3 elements in A with hardcoded logic

Phase 3: Greedy Insertion (B → A)
  Repeatedly find the cheapest element in B to insert into A
  Exploit combined rotations (rr, rrr) to minimize total cost
  Maintain a sorted circular invariant in A throughout

Phase 4: Final Rotation
  Rotate A so that element 0 (the global minimum) sits at the top
```

For inputs of size 2 or 3, dedicated hardcoded paths handle sorting in at most
1 or 2 operations respectively. For inputs of size 4 or 5, a bounded BFS explores
the full state space of legal push_swap operations to find and replay the
provably shortest instruction sequence.

---

## Data Structure

Since push_swap operations require efficient access to both the front and back of
a stack (rotate moves the front element to the back; reverse rotate does the
opposite), the project implements each stack as a **doubly linked list–based
deque**.

```c
typedef struct s_node
{
    int             data;   /* Normalized index (0 ~ N-1) */
    struct s_node   *next;
    struct s_node   *prev;
}   t_node;

typedef struct s_deque
{
    char    name;   /* 'a' or 'b' — determines which label to print */
    int     size;
    t_node  *front;
    t_node  *back;
}   t_deque;
```

The deque exposes four primitives — `push_front`, `push_back`, `pop_front`,
`pop_back` — each operating in O(1). Every push_swap operation (swap, push,
rotate, reverse rotate) is built on top of these primitives and emits the
corresponding instruction string to stdout upon execution.

Dual operations (`ss`, `rr`, `rrr`) perform the underlying manipulation on both
stacks silently and then print a single combined label, avoiding double-counted
output.

---

## Algorithm Phases in Detail

### Phase 1 — Parsing & Coordinate Compression (Indexing)

**Parsing** accepts arguments in two forms: multiple `argv` entries
(`./push_swap 3 1 2`) or a single quoted string (`./push_swap "3 1 2"`).
Each token is split on spaces via `ft_split`, validated for integer format, range-
checked against `INT_MIN`..`INT_MAX`, and tested for duplicates against the
already-parsed portion of the deque. Any failure immediately triggers
`Error\n` on stderr.

The `ft_atoi` function used for conversion accumulates into an `unsigned long`
and guards against overflow *before* each `nbr = nbr * 10 + digit` step,
comparing against a precomputed limit derived from `LONG_MAX` (or
`LONG_MAX + 1` for negative numbers). If the input exceeds the representable
range, it saturates to the limit value, which then reliably fails the subsequent
`INT_MIN`/`INT_MAX` check.

**Coordinate compression** replaces the raw integer values with their rank in
sorted order. A temporary array of all values is sorted (selection sort — simple
and sufficient for project scale), and each node's data is replaced by the result
of a binary search into this sorted array. After compression, the smallest
element is `0`, the largest is `N - 1`, and pivot calculations become trivial
arithmetic: `pivot1 = N / 3`, `pivot2 = 2 * N / 3`.

### Phase 2 — 3-Way Partitioning (A → B)

The goal of this phase is to move most elements from A to B in a roughly
structured manner, leaving exactly 3 elements in A.

Two pivots divide the index space into three regions:

- **Small group** (`data < N/3`): pushed to B via `pb`. These land on top of B.
- **Mid group** (`N/3 ≤ data < 2N/3`): pushed to B via `pb`, then immediately
  rotated to the bottom of B via `rb`. This separates them from the small group,
  giving B a rough two-tier structure where smaller indices sit above mid-range
  indices.
- **Large group** (`data ≥ 2N/3`): kept in A via `ra`, cycling through to the
  bottom.

After iterating through the original size of A, any remaining excess elements
(beyond 3) are flushed to B with plain `pb`. The 3 elements left in A are then
sorted with a hardcoded handler that covers all 6 permutations in at most 2
operations.

### Phase 3 — Cost-based Greedy Insertion (B → A)

This is the core optimization step. It repeats until B is empty:

**Target finding**: For each element `x` in B, find the correct insertion point in
A that maintains the sorted circular order. This is the position of the smallest
element in A that is still greater than `x`. If `x` is larger than every element
in A (no such element exists), the target wraps around to the position of the
global minimum in A.

**Cost calculation**: For each candidate element at position `b_pos` in B and its
target at position `a_pos` in A, compute the rotation cost under four strategies:

| Strategy | Formula | When optimal |
|----------|---------|--------------|
| Both forward (`rr` then leftover `ra`/`rb`) | `max(a_pos, b_pos)` | Both positions are near the top |
| Both backward (`rrr` then leftover `rra`/`rrb`) | `max(a_size − a_pos, b_size − b_pos)` | Both positions are near the bottom |
| A forward, B backward | `a_pos + (b_size − b_pos)` | A-target near top, B-element near bottom |
| A backward, B forward | `(a_size − a_pos) + b_pos` | A-target near bottom, B-element near top |

The element in B with the globally minimum cost across all four strategies is
selected.

**Execution**: The cheapest strategy is re-derived for the selected element and
executed. Combined rotations (`rr` or `rrr`) are performed first to exploit
shared movement, followed by any remaining single-stack rotations, then `pa` to
transfer the element.

**Invariant**: After every `pa`, stack A remains in sorted circular order —
ascending if you start reading from the minimum element and wrap around. This
invariant is maintained because we always insert at the correct target position.

### Phase 4 — Final Rotation (Last Sort)

After B is empty, A is sorted but the minimum element (`0`) may not be at the
front. The algorithm locates `0` and rotates A in the shorter direction (`ra` or
`rra`) to bring it to the top.

### Small Input Handling (n ≤ 5)

- **n = 0 or 1**: No output.
- **n = 2**: A single `sa` if out of order.
- **n = 3**: Hardcoded sort covering all 6 permutations in at most 2 operations.
- **n = 4 or 5**: A bounded BFS over the full state space of legal push_swap
  operations finds the provably optimal (shortest) instruction sequence for the
  given permutation and replays it on the real stacks.

The BFS solver represents each state as a compact struct holding two small
integer arrays (stack A and stack B contents), their sizes, and a back-pointer
to the parent state. It expands all 11 operations at each level, deduplicates
visited states, and terminates as soon as it reaches a goal state (A sorted
ascending with B empty). The solution path is then traced back and replayed.
With `n ≤ 5` the state space is bounded by 5! × 2^5 = 3840, so the BFS
completes in constant time.

---

## Bonus: Checker Program

The `checker` program validates whether a given sequence of push_swap
instructions actually sorts the input. It:

1. Takes the same integer arguments as `push_swap` and builds stack A.
2. Reads instructions from stdin, one per line (terminated by `\n`).
3. Executes each instruction silently on the stacks (no stdout output during
   execution).
4. After reading all instructions, prints `OK\n` if A is sorted ascending and B
   is empty; otherwise prints `KO\n`.
5. On any error (invalid input, unknown instruction), prints `Error\n` to stderr.

The checker uses "silent" operation functions that manipulate the deque without
printing anything, unlike the mandatory part's operations which always emit their
label. Stdin reading is handled by a custom `get_next_line` implementation with a
dynamically growing stash buffer.

Compiled via `make bonus`. All bonus source files follow the `*_bonus.c` /
`*_bonus.h` naming convention required by the subject.

---

## Instructions

### Prerequisites

- A Unix-like system (Linux, macOS)
- `cc` (gcc or clang)
- `make`

### Compilation

```bash
# Build the mandatory push_swap program
make

# Build the bonus checker program
make bonus

# Clean object files
make clean

# Full clean (objects + executables)
make fclean

# Rebuild from scratch
make re
```

The Makefile compiles with `-Wall -Wextra -Werror` and does not relink. It also
builds the included `libft` automatically.

### Norm Compliance

The entire codebase passes `norminette` v4.1. Key constraints adhered to:

- Maximum 25 lines per function (excluding braces), maximum 4 parameters,
  maximum 5 variable declarations per function.
- No `for`, `do...while`, `switch/case`, `goto`, or ternary `?:` operators.
- 80-column line limit, tab indentation, snake_case naming throughout.
- At most 5 function definitions per `.c` file.
- All heap allocations freed on both normal exit and error paths.
- Header files protected with include guards.

---

## Usage Examples

```bash
# Sort a small list
$> ./push_swap 2 1 3 6 5 8
sa
pb
pb
pb
sa
pa
pa
pa

# Invalid input produces an error
$> ./push_swap 0 one 2 3
Error

# No arguments produces no output
$> ./push_swap

# Count operations for 100 random numbers
$> ARG=$(shuf -i 1-10000 -n 100 | tr '\n' ' '); ./push_swap $ARG | wc -l
554

# Verify correctness with the bonus checker
$> ARG=$(shuf -i 1-10000 -n 100 | tr '\n' ' '); ./push_swap $ARG | ./checker $ARG
OK

# Count operations for 500 random numbers
$> ARG=$(shuf -i 1-10000 -n 500 | tr '\n' ' '); ./push_swap $ARG | wc -l
4187

# Verify correctness with the provided checker_OS
$> ARG=$(shuf -i 1-10000 -n 500 | tr '\n' ' '); ./push_swap $ARG | ./checker_OS $ARG
OK

# Test edge cases: overflow values must be rejected
$> ./push_swap 18446744073709551615
Error
$> ./push_swap -18446744073709551615
Error
```

---

## Benchmark Results

Measured across randomized test runs:

| Input Size | Average Operations | Worst Case | Subject Maximum (100%) |
|------------|-------------------|------------|------------------------|
| 2          | 0–1               | 1          | —                      |
| 3          | 0–2               | 2          | 3                      |
| 4          | Optimal per permutation | 5     | —                      |
| 5          | Optimal per permutation | 12    | ~12                    |
| 100        | ~550              | < 700      | 700                    |
| 500        | ~4100–4200        | < 5500     | 5500                   |

For `n = 2..5`, every permutation is solved with the provably minimum number
of operations (verified exhaustively). For `n = 100` and `n = 500`, the 3-way
partition + greedy approach consistently stays well within the subject's
thresholds for full marks (100%).

---

## File Organization

```
push_swap/
├── Makefile
├── README.md
├── includes/
│   ├── push_swap.h            # Mandatory header
│   └── checker_bonus.h        # Bonus header
├── libft/
│   ├── Makefile
│   ├── libft.h
│   ├── ft_atoi.c
│   ├── ft_bzero.c
│   ├── ft_calloc.c
│   ├── ft_isdigit.c
│   ├── ft_memset.c
│   ├── ft_putstr_fd.c
│   ├── ft_split.c
│   ├── ft_strlcpy.c
│   ├── ft_strlen.c
│   └── ft_strncmp.c
├── srcs/
│   ├── main.c                 # Entry point, dispatch by input size
│   ├── parsing.c              # argv tokenization and validation
│   ├── parsing_utils.c        # ft_atoi (overflow-safe), is_valid_number, has_duplicate
│   ├── indexing.c             # Coordinate compression (sort array + binary search)
│   ├── deque.c                # Deque primitives (push_front/back, pop_front/back, new_node)
│   ├── operations.c           # swap, push, rotate, reverse_rotate (with stdout print)
│   ├── operations_dual.c      # ss, rr, rrr (combined operations)
│   ├── utils.c                # is_sorted, free_deque
│   ├── sort_small.c           # sort_two, sort_three, sort_five (BFS entry)
│   ├── sort_small_ops.c       # BFS state transitions and operation replay
│   ├── sort_small_utils.c     # BFS queue management and state deduplication
│   ├── partition.c            # 3-way partitioning (Phase 2)
│   ├── greedy.c               # Main greedy loop and cost calculation
│   ├── greedy_exec.c          # Cheapest-move execution (rr/rrr/mixed strategies)
│   ├── greedy_utils.c         # Target position finder and min-position helper
│   └── last_sort.c            # Final rotation to bring 0 to front
└── srcs_bonus/
    ├── checker_bonus.c        # Checker entry point and instruction dispatch
    ├── deque_bonus.c          # Deque primitives (same logic, bonus compilation unit)
    ├── operations_bonus.c     # Silent operations (no stdout print)
    ├── parsing_bonus.c        # argv parsing (same logic, bonus compilation unit)
    ├── parsing_utils_bonus.c  # ft_atoi, validation (same logic, bonus compilation unit)
    ├── utils_bonus.c          # init_deque, is_sorted, free_deque
    ├── get_next_line_bonus.c  # GNL for reading instructions from stdin
    └── get_next_line_utils_bonus.c  # GNL stash buffer helpers
```

Each `.c` file contains at most 5 function definitions, as required by Norm
v4.1.

---

## Development Log

This section documents the major milestones, bugs discovered, and architectural
decisions made during development. The project evolved through iterative
implementation, testing, and refinement.

### Initial Implementation

The first working version established the core architecture: doubly linked list
deque, operation wrappers that print to stdout, `ft_split`-based argument
parsing, and the full sorting pipeline (indexing → 3-way partition → greedy →
final rotation). Small inputs (n ≤ 3) were handled with hardcoded logic, and
n = 4/5 used a simple push-min-to-B-sort-3-push-back strategy.

Initial benchmarks showed roughly **~550 average for 100 numbers** and
**~4200 average for 500 numbers**, both well within subject limits.

### Checkpoint 1 — Parse Overflow Fix & Makefile Repair

**The `ft_atoi` overflow bug** was the most critical issue found during review.
The original implementation accumulated into a `long` variable with no overflow
guard before the multiply-and-add step. On 64-bit systems where `long` is 8
bytes, extremely large numeric strings (e.g., `18446744073709551615` or its
negative counterpart) could silently wrap around and land inside the
`INT_MIN`..`INT_MAX` range, causing the program to accept invalid input without
printing `Error\n`. This was a direct subject violation.

The fix introduced an `unsigned long` accumulator with a precomputed overflow
limit. Before each `nbr = nbr * 10 + digit` step, the code now checks whether
the operation would exceed `LONG_MAX` (for positive numbers) or `LONG_MAX + 1`
(for negative numbers, to correctly handle `LONG_MIN`). On overflow, the
function saturates to the limit value, which is guaranteed to fail the subsequent
`INT_MIN`/`INT_MAX` range check. The same fix was applied to both
`srcs/parsing_utils.c` and `srcs_bonus/parsing_utils_bonus.c`.

**The Makefile bonus relink** was a minor but annoying issue: `make bonus` was
relinking the `checker` binary even when nothing had changed, because the `bonus`
target directly owned the link recipe instead of depending on `$(BONUS_NAME)`.
The fix restructured the dependency chain so `bonus` depends on `$(BONUS_NAME)`,
and `$(BONUS_NAME)` owns the real link rule with proper prerequisites. A second
`make bonus` now correctly reports that nothing needs to be done.

### Checkpoint 2 — Optimal Small Sort via BFS

**The small-sort non-optimality problem** was identified during code review.
The initial n = 4/5 handler was correct but not minimal — for example,
`./push_swap 2 1 3 4` produced 4 operations instead of the optimal 1 (`sa`).
While this did not affect the 100/500 benchmarks, it was a potential weakness
during peer evaluation where sharp evaluators might test specific small inputs.

Rather than manually hardcoding optimal sequences for every permutation of 4
and 5 elements, I implemented a **bounded BFS solver** that automatically
discovers the shortest instruction sequence for any given permutation. The solver
represents each state as a compact struct with two small integer arrays (up to 5
elements each), explores all 11 operations at each BFS level, deduplicates
visited states by linear scan (acceptable given the tiny state space of at most
~720 states for n = 5), and terminates at the first goal state found.

The implementation was split across three files — `sort_small.c` (entry point
and BFS invocation), `sort_small_ops.c` (state transition functions and operation
replay), and `sort_small_utils.c` (BFS queue management and state comparison) —
to stay within the Norm's 5-functions-per-file limit.

Exhaustive verification confirmed that every permutation of n = 2..5 now
produces the optimal (minimum) number of operations, and that every generated
sequence is validated as `OK` by the checker.

### Code Review Findings (Documented, Not All Addressed)

A deep code review identified several additional observations, documented here
for completeness. These were evaluated for risk and prioritized accordingly:

- **Unchecked `new_node` allocation in parsing**: `process_token()` was updated
  to check the return value of `new_node()` and propagate failure. In the current
  version, `new_node` returning `NULL` correctly causes `process_token` to
  return 0, which triggers error handling upstream.

- **`normalize_data` allocation failure**: The function now returns an `int`
  status (1 on success, 0 on failure), and `main` checks this return value
  before proceeding to sort. On failure, it frees the deque and prints `Error\n`.

- **Duplicated parsing logic between mandatory and bonus**: The mandatory
  (`srcs/parsing*.c`) and bonus (`srcs_bonus/parsing*_bonus.c`) parsing files
  share nearly identical logic. This is a deliberate choice — the 42 bonus rules
  require `_bonus.c` filenames, and most cadets accept the duplication rather
  than introducing a shared-source build complexity. Both copies are kept in sync
  manually.

- **Combined op functions always print even on no-op stacks**: `swap_all`,
  `rotate_all`, and `reverse_all` always emit their label even if one or both
  stacks have fewer than 2 elements. This is semantically valid per the subject
  (the operations are defined to "do nothing" on insufficient elements, not to
  be illegal), and in practice these dual operations are only called when both
  stacks have sufficient size (inside the greedy loop), so this does not inflate
  the operation count.

- **O(n²) duplicate detection**: Each new token is checked against the entire
  existing deque by linear scan. For project scale (up to ~500–1000 elements),
  this is fast enough and keeps the code simple. A hash-based approach would
  improve the asymptotic complexity but is unnecessary here.

### Verification Performed

- `make` and `make bonus` compile cleanly with `-Wall -Wextra -Werror`.
- `norminette includes srcs srcs_bonus libft` passes with no errors.
- Exhaustive optimality check: every permutation of n = 2..5 was tested and
  confirmed to use the minimum possible number of operations.
- Checker validation: the generated instruction sequence for every small
  permutation was piped through the checker and confirmed to produce `OK`.
- Overflow edge cases: inputs such as `18446744073709551615`,
  `-18446744073709551615`, `2147483648`, `-2147483649` are all correctly
  rejected with `Error\n`.
- Randomized stress testing: thousands of random inputs of sizes 100 and 500
  were tested for both correctness (`checker` returns `OK`) and benchmark
  compliance.

---

## Known Limitations & Future Improvements

- **Selection sort in indexing**: The coordinate compression phase uses O(n²)
  selection sort for the temporary array. For the project's input scale this is
  perfectly adequate, but a merge sort or quicksort would be more appropriate
  for very large inputs.

- **Linear state deduplication in BFS solver**: The small-sort BFS uses linear
  scan to detect duplicate states. This is acceptable because the state space for
  n ≤ 5 is bounded, but a hash table would improve constant factors.

- **Parsing code duplication**: The mandatory and bonus parsing modules are
  nearly identical copies. A shared source file with conditional compilation or a
  common library would reduce maintenance burden, at the cost of build
  complexity.

- **Variable naming in greedy execution**: Names like `ba`, `bb`, `best_a`,
  `best_b` are concise but could benefit from more descriptive alternatives
  (`best_a_pos`, `best_b_pos`) for maintainability.

---

## Resources

### Algorithm & Theory

- **A. Yigit Ogun — "Push_swap: The Turk Algorithm"**
  (https://medium.com/@ayogun/push-swap-c1f5d2d41e97)
  The primary reference for the cost-based greedy approach. Walks through target
  finding, cost calculation, and combined rotation optimization step by step.

- **Jamie Robert Dawson — "Push_Swap: The Least Amount of Moves with Two Stacks"**
  (https://medium.com/@jamierobertdawson/push-swap-the-least-amount-of-moves-with-two-stacks-d1e76a71789a)
  Covers small-set sorting (2, 3, 5 elements) and general push_swap strategy
  overview.

- **Jeff Erickson — "Algorithms" (Chapter on Greedy Algorithms)**
  (https://jeffe.cs.illinois.edu/teaching/algorithms/)
  Free online textbook providing formal grounding in greedy algorithm design.

### Tools

- **push_swap Visualizer (desktop, C++/SFML)**
  (https://github.com/o-reo/push_swap_visualizer)
  Step-through visualization of push_swap operation sequences.

- **push_swap Web Visualizer**
  (https://windowdong11.github.io/push_swap_visualizer/)
  Browser-based visualizer — paste commands and watch them execute.

- **Push-Swap-Tester by gemartin99**
  (https://github.com/gemartin99/Push-Swap-Tester)
  Comprehensive automated tester for both mandatory and bonus parts.

### AI Usage

AI tools (Claude by Anthropic) were used in the following limited capacities
during development:

- **Conceptual study**: Before coding began, AI was used to survey which CS
  concepts and algorithm families are relevant to push_swap (stacks, greedy
  algorithms, coordinate compression, BFS), helping to build a study plan and
  identify key reference articles.

- **Code review and bug identification**: After the initial implementation was
  complete and passing basic tests, AI was used as an additional review layer to
  identify potential edge-case bugs (such as the `ft_atoi` overflow issue) and
  to evaluate the severity and priority of each finding.
