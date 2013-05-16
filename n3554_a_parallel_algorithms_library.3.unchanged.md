# Existing Algorithms Left Unchanged by this Proposal

This proposal leaves some algorithms of `<memory>`, `<algorithm>`, and `<numeric>` unchanged
because they either 1. have no parallelism, 2. have explicitly sequential semantics, 3. have an uncertain parallel implementation and for which we feel parallelization would be low priority.

For example, instead of parallelizing the standard heap algorithms, a better strategy may be to provide concurrent priority queues as a data structure as some have suggested.

## Unchanged specialized algorithms from `<memory>`

  * No parallelism
    * `addressof`

## Unchanged algorithms from `<algorithm>`

  * No parallelism
    * `iter_swap`
    * `min`
    * `max`
    * `minmax`

  * Uncertain / low priority
    * `is_permutation`
    * `search`
    * `search_n`
    * `push_heap`
    * `pop_heap`
    * `make_heap`
    * `sort_heap`
    * `is_heap`
    * `is_heap_until`
    * `next_permutation`
    * `prev_permutation`

  * Explicitly sequential
    * `copy_backward`
    * `move_backward`
    * `random_shuffle`
    * `shuffle`

## Unchanged generalized numeric operations from `<numeric>` 

  * Explicitly sequential
    * `accumulate`
    * `partial_sum`
    * `iota`

