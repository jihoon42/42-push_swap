# CLAUDE.md — Push_swap Project Specification

## Project Overview

**push_swap** is a 42 C programming project that sorts a random sequence of integers in stack A into ascending order, using only two stacks (A and B) and a limited set of operations. The primary challenge is not merely sorting, but minimizing the total number of operations output.

This implementation uses **Coordinate Compression (Indexing) + 3-Way Partitioning + Cost-based Greedy Algorithm** to achieve operation counts well within the subject's benchmarks.

---

## Subject Constraints

- **Language**: C only.
- **Norm**: Must pass norminette v4.1 (details below).
- **Allowed functions**: `read`, `write`, `malloc`, `free`, `exit`, and any self-coded `ft_printf` or equivalent.
- **Libft**: Allowed.
- **Global variables**: Forbidden.
- **Error handling**: On any invalid input (non-integer, overflow beyond INT range, duplicates), print `Error\n` to **stderr** and exit. If no arguments are given, print nothing and return silently.
- **Output**: Print the sequence of operations to **stdout**, one per line, separated by `\n` only.
- **Makefile rules**: `$(NAME)`, `all`, `clean`, `fclean`, `re`. Must not relink. Compile with `-Wall -Wextra -Werror` using `cc`. Bonus rule compiles the checker program from `_bonus.{c/h}` files.

### Available Operations

| Operation | Effect |
|-----------|--------|
| `sa`  | Swap top 2 elements of stack A |
| `sb`  | Swap top 2 elements of stack B |
| `ss`  | `sa` + `sb` simultaneously |
| `pa`  | Push top of B onto top of A |
| `pb`  | Push top of A onto top of B |
| `ra`  | Rotate A upward (first element becomes last) |
| `rb`  | Rotate B upward (first element becomes last) |
| `rr`  | `ra` + `rb` simultaneously |
| `rra` | Reverse rotate A (last element becomes first) |
| `rrb` | Reverse rotate B (last element becomes first) |
| `rrr` | `rra` + `rrb` simultaneously |

### Benchmarks (for 100% / bonus eligibility)

- **100 numbers**: fewer than **700** operations.
- **500 numbers**: no more than **5500** operations.

Minimal validation (80%) allows looser limits (e.g. 100 in <1100 & 500 in <8500, etc.).

---

## Data Structure

Because rotate and reverse-rotate operations require efficient access to both ends, the project uses a **Doubly Linked List–based Deque**.

```c
typedef struct s_node
{
    int             data;   // Normalized index (0 ~ N-1)
    struct s_node   *next;
    struct s_node   *prev;
}   t_node;

typedef struct s_deque
{
    char    name;   // 'a' or 'b'
    int     size;
    t_node  *front;
    t_node  *back;
}   t_deque;
```

### Deque Primitives

```
void    push_front(t_deque *q, t_node *x);
void    push_back(t_deque *q, t_node *x);
t_node  *pop_front(t_deque *q);
t_node  *pop_back(t_deque *q);
```

### Operation Wrappers

```
void    swap(t_deque *q);           // sa or sb
void    swap_all(t_deque *a, t_deque *b);  // ss
void    push(t_deque *dst, t_deque *src);  // pa or pb
void    rotate(t_deque *q);         // ra or rb
void    rotate_all(t_deque *a, t_deque *b);  // rr
void    reverse_rotate(t_deque *q); // rra or rrb
void    reverse_all(t_deque *a, t_deque *b); // rrr
```

Each wrapper prints its corresponding instruction to stdout (e.g. `write(1, "ra\n", 3)`).

---

## Algorithm Workflow

### Phase 1: Parsing & Coordinate Compression (Indexing)

**Goal**: Convert arbitrary integer values into a contiguous index range `0 ~ N-1`.

1. Parse `argv`. Validate each argument:
   - Must be a valid integer (no letters/symbols other than optional leading sign).
   - Must be within `INT_MIN` ~ `INT_MAX`.
   - No duplicates allowed.
   - On any error → `write(2, "Error\n", 6)` and exit.

2. Push each parsed integer into stack A (via `push_back`) and simultaneously store all values into a temporary `int` array.

3. Sort the temporary array (e.g. quicksort or any O(N log N) sort).

4. Traverse stack A: for each node, perform a **binary search** on the sorted array to find the node's rank, then replace `node->data` with that rank index.

```c
void    normalize_data(t_deque *a, int *arr, int size)
{
    t_node  *curr;
    int     index;

    curr = a->front;
    while (curr)
    {
        index = binary_search(arr, curr->data, size);
        curr->data = index;
        curr = curr->next;
    }
}
```

**Why**: After indexing, the smallest value is always `0`, pivots are trivial to compute (`N/3`, `2N/3`), and target-finding in Phase 3 becomes straightforward comparison of small integers.

---

### Phase 2: 3-Way Partitioning (A → B)

**Goal**: Move most elements from A to B in a roughly structured way, leaving only 3 elements in A.

Compute two pivots:
- `pivot1 = N / 3`
- `pivot2 = (N * 2) / 3`

Iterate through the original A size:

| Condition | Action | Reasoning |
|-----------|--------|-----------|
| `data < pivot1` | `pb` | Small group → top of B |
| `pivot1 <= data < pivot2` | `pb` then `rb` | Mid group → bottom of B (separates from small group) |
| `data >= pivot2` | `ra` | Large group → stays in A for now |

After this pass, push any remaining elements beyond 3 from A to B (`pb` until `a->size == 3`).

Sort the 3 remaining elements in A with hardcoded logic (at most 2 operations).

```c
void    partition_to_b(t_deque *a, t_deque *b, int p1, int p2)
{
    int count;

    count = a->size;
    while (count-- > 0 && a->size > 3)
    {
        if (a->front->data < p1)
            push(b, a);                 // pb
        else if (a->front->data < p2)
        {
            push(b, a);                 // pb
            rotate(b);                  // rb
        }
        else
            rotate(a);                  // ra
    }
    while (a->size > 3)
        push(b, a);                     // pb
    sort_three_hardcoded(a);
}
```

---

### Phase 3: Greedy Algorithm (B → A)

**Goal**: Move every element from B back into A at the correct sorted position, always choosing the cheapest element to move.

This is the core optimization step. Repeat until B is empty:

#### 3-1. Find Target Position in A

For each element `x` in B, find where it should be inserted in A to maintain ascending order:
- Find the smallest element in A that is still greater than `x`. That element's position is the target.
- If `x` is larger than every element in A (i.e. no element in A is greater), the target is the position of the minimum element (`0`), because `x` wraps around to sit just before the smallest.

```c
int get_target_position(t_deque *a, int b_data)
{
    t_node  *curr;
    int     target_pos;
    int     closest_bigger;
    int     pos;

    curr = a->front;
    target_pos = 0;
    closest_bigger = INT_MAX;
    pos = 0;
    while (curr)
    {
        if (curr->data > b_data && curr->data < closest_bigger)
        {
            closest_bigger = curr->data;
            target_pos = pos;
        }
        curr = curr->next;
        pos++;
    }
    if (closest_bigger == INT_MAX)
        target_pos = get_min_value_position(a);
    return (target_pos);
}
```

#### 3-2. Cost Calculation

For each element in B (at position `b_pos`), compute the cost to bring both that element to the top of B and its A-target to the top of A. There are 4 rotation strategies:

| Case | Cost | When to use |
|------|------|-------------|
| Both forward (`rr`) | `max(ra, rb)` | Both positions are in the top half |
| Both backward (`rrr`) | `max(rra, rrb)` | Both positions are in the bottom half |
| A forward, B backward | `ra + rrb` | A-target near top, B-element near bottom |
| A backward, B forward | `rra + rb` | A-target near bottom, B-element near top |

Where:
- `ra = a_pos`, `rra = a->size - a_pos`
- `rb = b_pos`, `rrb = b->size - b_pos`

Pick the minimum of the 4 cases. Track the element in B with the global minimum cost.

```c
void    calc_cost_and_find_best(t_deque *a, t_deque *b,
            int *best_a, int *best_b)
{
    t_node  *b_curr;
    int     b_pos;
    int     a_pos;
    // + cost tracking variables (min_cost, ra, rra, rb, rrb, cost)

    b_curr = b->front;
    b_pos = 0;
    while (b_curr)
    {
        a_pos = get_target_position(a, b_curr->data);
        // compute ra, rra, rb, rrb
        // cost = min of 4 cases
        // update best_a, best_b if cost < min_cost
        b_curr = b_curr->next;
        b_pos++;
    }
}
```

#### 3-3. Execute the Cheapest Move

Once the best `(best_a, best_b)` pair is found, re-derive which of the 4 cases was cheapest. Execute the combined rotations (e.g. `rr` loop), then handle any remaining single-stack rotations, then `pa`.

```c
void    execute_moves(t_deque *a, t_deque *b, int best_a, int best_b)
{
    // Determine cheapest case again, then:
    // e.g. for rr case:
    while (best_a > 0 && best_b > 0)
    {
        rotate_all(a, b);   // rr
        best_a--;
        best_b--;
    }
    while (best_a-- > 0)
        rotate(a);          // ra
    while (best_b-- > 0)
        rotate(b);          // rb
    push(a, b);             // pa
}
```

#### Greedy Main Loop

```c
void    greedy(t_deque *a, t_deque *b)
{
    int best_a;
    int best_b;

    while (b->size)
    {
        calc_cost_and_find_best(a, b, &best_a, &best_b);
        execute_moves(a, b, best_a, best_b);
    }
    last_sort(a);
}
```

**Invariant**: Stack A is always in sorted (circular) order — i.e. ascending if you read from the minimum element onward wrapping around. Every `pa` preserves this invariant because we insert at the correct target position.

---

### Phase 4: Final Rotation (Last Sort)

After B is empty, A is sorted but the minimum element (`0`) may not be at the front. Find `0`'s position and rotate A by the shorter direction.

```c
void    last_sort(t_deque *a)
{
    int zero_pos;

    zero_pos = get_min_value_position(a);
    if (zero_pos <= a->size / 2)
    {
        while (zero_pos-- > 0)
            rotate(a);          // ra
    }
    else
    {
        zero_pos = a->size - zero_pos;
        while (zero_pos-- > 0)
            reverse_rotate(a);  // rra
    }
}
```

---

## Edge Cases & Special Sorts

- **0 or 1 elements**: Do nothing.
- **2 elements**: `sa` if out of order.
- **3 elements**: Hardcoded sort with at most 2 operations (6 permutations → each mapped to a fixed instruction sequence).
- **Already sorted input**: Detect and exit with no output.

---

## Norm v4.1 Compliance Checklist

- **Functions**: Max 25 lines (excluding braces). Max 4 parameters. Max 5 variable declarations per function.
- **Forbidden**: `for`, `do...while`, `switch/case`, `goto`, ternary `?:`, VLAs.
- **Formatting**: 80-column max. Indent with real tabs (4-char width). Braces on own lines. One variable declaration per line. No init at declaration (except `static`/`const`/global). Empty line between declarations and code body, no other empty lines within a function.
- **Naming**: snake_case only. Structs prefixed `s_`, typedefs `t_`, unions `u_`, enums `e_`. Identifiers must be readable English.
- **Files**: Max 5 function definitions per `.c` file. Structs declared in `.h` only. Headers protected with include guards (`#ifndef`/`#define`/`#endif`).
- **Comments**: Not inside function bodies. English, useful.
- **Memory**: Free all heap allocations. `free_deque(a)` and `free_deque(b)` on both normal exit and error paths.

---

## Bonus: Checker Program

A separate `checker` program that:
1. Takes the same integer arguments as `push_swap`.
2. Reads operations from stdin (one per line, terminated by `\n`).
3. Executes them on the stacks.
4. Prints `OK\n` to stdout if A is sorted ascending and B is empty; otherwise `KO\n`.
5. Prints `Error\n` to stderr on invalid input or unknown instruction.

Compiled via `make bonus`. Source files named `*_bonus.c` / `*_bonus.h`.

---

## Compilation & Testing

```bash
# Build
make

# Run with space-separated arguments
./push_swap 5 2 3 1 4

# Run with a single quoted string
./push_swap "5 2 3 1 4"

# Count operations for 500 random numbers
ARG=$(shuf -i 1-10000 -n 500 | tr '\n' ' '); ./push_swap $ARG | wc -l

# Verify correctness with checker
ARG=$(shuf -i 1-10000 -n 500 | tr '\n' ' '); ./push_swap $ARG | ./checker_OS $ARG
```

### Target Operation Counts

| Input Size | Target Range | Maximum (for 100%) |
|------------|-------------|---------------------|
| 3          | 0–2         | 3                   |
| 5          | ~8–12       | ~12                 |
| 100        | ~550–700    | 700                 |
| 500        | ~3500–5500  | 5500                |

---

## File Organization (Suggested)

```
push_swap/
├── Makefile
├── includes/
│   └── push_swap.h
├── libft/            # if using libft
│   ├── Makefile
│   └── ...
├── srcs/
│   ├── main.c
│   ├── parsing.c        # argv parsing, validation
│   ├── indexing.c        # coordinate compression (sort array, binary search)
│   ├── deque.c           # push_front, push_back, pop_front, pop_back
│   ├── operations.c      # swap, push, rotate, reverse_rotate (+ print)
│   ├── operations_dual.c # ss, rr, rrr
│   ├── sort_small.c      # hardcoded sort for 2–3 elements
│   ├── partition.c       # 3-way partitioning (Phase 2)
│   ├── greedy.c          # target finding, cost calc, execute moves
│   ├── greedy_utils.c    # helper functions for cost/rotation logic
│   └── last_sort.c       # final rotation to bring 0 to front
├── srcs_bonus/
│   ├── checker_bonus.c
│   └── ...
└── README.md
```

Keep each `.c` file to at most 5 functions per Norm rules.