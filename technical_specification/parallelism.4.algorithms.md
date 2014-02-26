# Parallel algorithms {#parallel.alg}

## In general {#parallel.alg.general}

This clause describes components that C++ programs may use to perform operations on containers and other sequences in parallel.

### Effect of execution policies on parallel algorithm execution {#parallel.alg.general.exec}

1. Parallel algorithms have template parameters named `ExecutionPolicy` which describe the manner in which the execution of these algorithms may be parallelized and the manner in which they apply user-provided function objects.

2. The applications of function objects in parallel algorithms invoked with an execution policy object of type `sequential_execution_policy` execute in sequential order in the calling thread.

3. The applications of function objects in parallel algorithms invoked with an execution policy object of type `parallel_execution_policy` are permitted to execute in an unordered fashion in unspecified threads, and indeterminately sequenced within each thread. 
   [*Note:* It is the caller's responsibility to ensure correctness, for example that the invocation does not introduce data races or deadlocks. –- *end note*]
   
     [*Example:*

         using namespace std::experimental::parallel;
         int a[] = {0,1};
         std::vector<int> v;
         for_each(par, std::begin(a), std::end(a), [&](int i) {
           v.push_back(i*2+1);
         });

     The program above has a data race because of the unsynchronized access to the container `v` -– *end example*]
   
     [*Example:*

         using namespace std::experimental::parallel;
         std::atomic<int> x = 0;
         int a[] = {1,2};
         for_each(par , std::begin(a), std::end(a), [](int n) {
           x.fetch_add(1 , std::memory_order_relaxed);
           // spin wait for another iteration to change the value of x
           while(x.load(std::memory_order_relaxed) == 1)
             ;
         });

     The above example depends on the order of execution of the iterations, and is therefore undefined (may deadlock). -– *end example*]

     [*Example:*

         using namespace std::experimental::parallel;
         int x;
         std::mutex m;
         int a[] = {1,2};
         for_each(par, std::begin(a), std::end(a), [&](int) {
           m.lock();
           ++x;
           m.unlock();
         });
    
     The above example synchronizes access to object `x` ensuring that it is incremented correctly. –- *end example*]

4. The applications of function objects in parallel algorithms invoked with an execution policy of type `vector_execution_policy` are permitted to execute in an unordered fashion in unspecified threads, and unsequenced within each thread. [*Note:* as a consequence, function objects governed by the `vector_execution_policy` policy must not synchronize with each other. Specifically, they must not acquire locks. –- *end note*]

    [*Example:*

        using namespace std::experimental::parallel;
        int x;
        std::mutex m;
        int a[] = {1,2};
        for_each(vec, std::begin(a), std::end(a), [&](int) {
          m.lock();
          ++x;
          m.unlock();
        });
   

    The above program is invalid because the applications of the function object are not guaranteed to run on different threads.

    [*Note:* the application of the function object may result in two consecutive calls to `m.lock` on the same thread, which may deadlock -– *end note*]
    
    -- *end example*]

    [*Note:* The semantics of the `parallel_execution_policy` or the `vector_execution_policy` invocation allow the implementation to fall back to sequential execution if the system cannot parallelize an algorithm invocation due to lack of resources. -- *end note*.]

5. If they exist, a parallel algorithm invoked with an execution policy object of type `parallel_execution_policy` or `vector_execution_policy` may apply iterator member functions of a stronger category than its specification requires. In this case,
   the application of these member functions are subject to provisions 3. and 4. above, respectively.

    [*Note:* For example, an algorithm whose specification requires `InputIterator` but receives a concrete iterator of the category `RandomAccessIterator` may use `operator[]`. In this
    case, it is the algorithm caller's responsibility to ensure `operator[]` is race-free. -- *end note*.]

6. Algorithms invoked with an execution policy object of type `execution_policy` execute internally as if invoked with instances of type `sequential_execution_policy`, `parallel_execution_policy`, or a non-standard implementation-defined execution policy depending on the dynamic value of the `execution_policy` object.

7. The semantics of parallel algorithms invoked with an execution policy object of type other than those described by this Technical Specification are unspecified.

### `ExecutionPolicy` algorithm overloads {#parallel.alg.overloads}

1. Parallel algorithms coexist alongside their sequential counterparts as overloads distinguished by a formal template parameter named `ExecutionPolicy`.
   This template parameter corresponds to the parallel algorithm's first function parameter, whose type is `ExecutionPolicy&&`.

2. Unless otherwise specified, the semantics of `ExecutionPolicy` algorithm overloads are identical to their overloads without.

3. Parallel algorithms have the requirement `is_execution_policy<ExecutionPolicy>::value` is `true`.

4. The algorithms listed in table 1 shall have `ExecutionPolicy` overloads.

-------------------------- ---------------------- ------------------   ----------------------
`uninitialized_copy`       `uninitialized_copy_n` `uninitialized_fill` `uninitialized_fill_n`
`all_of`                   `any_of`               `none_of`            `find`
`find`                     `find_if`              `find_if_not`        `find_end`
`find_first_of`            `adjacent_find`        `count`              `count_if`
`mismatch`                 `equal`                `search`             `search_n`
`copy`                     `copy_n`               `copy_if`            `move`
`swap_ranges`              `transform`            `replace`            `replace_copy`
`replace_copy_if`          `fill`                 `fill_n`             `generate`
`generate_n`               `remove`               `remove_if`          `remove_copy`
`remove_copy_if`           `unique`               `unique_copy`        `reverse`
`reverse_copy`             `rotate`               `rotate_copy`        `is_partitioned`
`partition`                `stable_partition`     `partition_copy`     `sort`
`stable_sort`              `partial_sort`         `partial_sort_copy`  `is_sorted`
`is_sorted_until`          `nth_element`          `merge`              `inplace_merge`
`includes`                 `set_union`            `set_intersection`   `set_difference`
`set_symmetric_difference` `min_element`          `max_element`        `minmax_element`
`lexicographical_compare`  `reduce`               `inclusive_scan`     `exclusive_scan`
                           `for_each`             `for_each_n`
-------------------------- ---------------------  ------------------   ----------------------

: Table of parallel algorithms

## Novel algorithms {#parallel.alg.novel}

This subclause describes novel algorithms introduced by this technical specification.

### Header `<experimental/algorithm>` synopsis {#parallel.alg.novel.algorithm.synop}

```
namespace std {
namespace experimental {
namespace parallel {
  template<class ExecutionPolicy,
           class InputIterator, class Function>
    void for_each(ExecutionPolicy&& exec,
                  InputIterator first, InputIterator last,
                  Function f);
  template<class InputIterator, class Size, class Function>
    InputIterator for_each_n(InputIterator first, Size n,
                             Function f);
}
}
}
```

### For each {#parallel.alg.novel.foreach}

```
template<class ExecutionPolicy,
         class InputIterator, class Function>
  InputIterator for_each(ExecutionPolicy&& exec,
                         InputIterator first, InputIterator last,
                         Function f);
```

1. *Effects:* Applies `f` to the result of dereferencing every iterator in the range `[first,last)`.
   [*Note:* If the type of `first` satisfies
   the requirements of a mutable iterator, `f` may apply nonconstant functions through the dereferenced
   iterator. -- *end note*]

2. *Complexity:* `O(last - first)`.

3. *Remarks:* If `f` returns a result, the result is ignored.

4. *Note:* Unlike its sequential form, the parallel overload of `for_each` does not return a copy of its `Function` parameter, since parallelization may not permit efficient state accumulation.

```
template<class InputIterator, class Size, class Function>
  InputIterator for_each_n(InputIterator first, Size n,
                           Function f);
```

1. *Requires:* `Function` shall meet the requirements of `MoveConstructible` [*Note:* `Function need not meet the requirements of `CopyConstructible`. -- *end note*]

2. *Effects:* Applies `f` to the result of dereferencing every iterator in the range `[first,first + n)`, starting from `first` and proceeding to `first + n - 1`.
    [*Note:* If the type of `first` satisfies the requirements of a mutable iterator, `f` may apply nonconstant functions throughthe dereferenced iterator. -- *end note*]

3. *Returns:* `first` + `(last - first`).

4. *Complexity:* Applies `f` exactly `n` times.

5. *Remarks:* If `f` returns a result, the result is ignored.

### Header `<experimental/numeric>` synopsis {#parallel.alg.novel.numeric.synop}

```
namespace std {
namespace experimental {
namespace parallel {
  template<class InputIterator>
    typename iterator_traits<InputIterator>::value_type
      reduce(InputIterator first, InputIterator last);
  template<class InputIterator, class T>
    T reduce(InputIterator first, InputIterator last T init);
  template<class InputIterator, class T, class BinaryOperation>
    T reduce(InputIterator first, InputIterator last, T init,
             BinaryOperation binary_op);

  template<class InputIterator, class OutputIterator>
    OutputIterator
      exclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result);
  template<class InputIterator, class OutputIterator,
           class T>
    OutputIterator
      exclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result,
                     T init);
  template<class InputIterator, class OutputIterator,
           class T, class BinaryOperation>
    OutputIterator
      exclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result,
                     T init, BinaryOperation binary_op);

  template<class InputIterator, class OutputIterator>
    OutputIterator
      inclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result);
  template<class InputIterator, class OutputIterator,
           class BinaryOperation>
    OutputIterator
      inclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result,
                     BinaryOperation binary_op);
  template<class InputIterator, class OutputIterator,
           class T, class BinaryOperation>
    OutputIterator
      inclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result,
                     T init, BinaryOperation binary_op);
}
}
}
```

### Reduce {#parallel.alg.novel.reduce}

```
template<class InputIterator>
  typename iterator_traits<InputIterator>::value_type
    reduce(InputIterator first, InputIterator last);
```

1. *Returns:* `reduce(first, last, typename iterator_traits<InputIterator>::value_type{})`

2. *Requires:* `typename iterator_traits<InputIterator>::value_type{}` shall be a valid expression. The `operator+` function associated with `iterator_traits<InputIterator>::value_type` shall have associativity and commutativity.

    `operator+` shall not invalidate iterators or subranges, nor modify elements in the range `[first,last)`.

3. *Complexity:* `O(last - first)`.

4. *Note:* The primary difference between `reduce` and `accumulate` is that the behavior of `reduce` may be non-deterministic for non-associative or non-commutative `operator+`.

```
template<class InputIterator, class T>
  T reduce(InputIterator first, InputIterator last, T init);
```

1. *Returns:* `reduce(first, last, init, plus<>())`

2. *Requires:* The `operator+` function associated with `T` shall not invalidate iterators or subranges, nor modify elements in the range `[first,last)`.

3. *Complexity:* `O(last - first)`.

4. *Note:* The primary difference between `reduce` and `accumulate` is that the behavior of `reduce` may be non-deterministic for non-associative or non-commutative `operator+`.

```
template<class InputIterator, class T, class BinaryOperation>
  T reduce(InputIterator first, InputIterator last, T init,
           BinaryOperation binary_op);
```

1. *Returns:* The result of the generalized sum of `init` and the elements in the range `[first,last)`.

    Sums of elements are evaluated with `binary_op`. The order of operands of the sum is unspecified.

2. *Requires:* `binary_op` shall have associativity and commutativity.

    `binary_op` shall not invalidate iterators or subranges, nor modify elements in the range `[first,last)`.

3. *Complexity:* `O(last - first)`.

4. *Note:* The primary difference between `reduce` and `accumulate` is that the behavior of `reduce` may be non-deterministic for non-associative or non-commutative `operator+`.


### Exclusive scan {#parallel.alg.novel.exclusive.scan}

```
template<class InputIterator, class OutputIterator,
         class T>
  OutputIterator
    exclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result,
                   T init);
```

1. *Returns:* `exclusive_scan(first, last, result, init, plus<>())`

2. *Requires:* The `operator+` function associated with `iterator_traits<InputIterator>::value_type` shall not invalidate iterators or subranges, nor modify elements in the ranges `[first,last)` or `[result,result + (last - first))`.

3. *Complexity:* `O(last - first)`.

4. *Notes:* The primary difference between `exclusive_scan` and `inclusive_scan` is that `exclusive_scan` excludes the `i`th input element from the `i`th sum.

```
template<class InputIterator, class OutputIterator,
         class T, class BinaryOperation>
  OutputIterator
    exclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result,
                   T init, BinaryOperation binary_op);
```

1. *Effects:* For each iterator `i` in `[result,result + (last - first))`, produces a result such that upon
    completion of the algorithm, `*i` yields the generalized sum of `init` and the elements in the range
    `[first, first + (i - result))`.

    During execution of the algorithm, every evaluation of the above sum is 

    `binary_op(binary_op(init,A), B)` or `binary_op(A, B)`

    where there exists some iterator `j` in `[first, last)` such that:

    1. `A` is the partial sum of elements in the range `[j, j + n)`.

    2. `B` is the partial sum of elements in the range `[j + n, j + m)`.

    3. `n` and `m` are positive integers and `j + m < last`.

2. *Returns:* The end of the resulting range beginning at `result`.

3. *Requires:* `binary_op` shall have associativity.

    `binary_op` shall not invalidate iterators or subranges, nor modify elements in the ranges `[first,last)` or `[result,result + (last - first))`.

4. *Complexity:* `O(last - first)`.

5. *Notes:* The primary difference between `exclusive_scan` and `inclusive_scan` is that `exclusive_scan` excludes the `i`th input element from the `i`th sum.


### Inclusive scan {#parallel.alg.novel.inclusive.scan}

```
template<class InputIterator, class OutputIterator>
  OutputIterator
    inclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result);
```

1. *Returns:* `inclusive_scan(first, last, result, plus<>())`

2. *Requires:* The `operator+` function associated with `iterator_traits<InputIterator>::value_type` shall have associativity.

    `operator+` shall not invalidate iterators or subranges, nor modify elements in the ranges `[first,last)` or `[result,result + (last - first))`.

3. *Complexity:* `O(last - first)`.

4. *Notes:* The primary difference between `exclusive_scan` and `inclusive_scan` is that `exclusive_scan` excludes the `i`th input element from the `i`th sum.

```
template<class InputIterator, class OutputIterator,
         class BinaryOperation>
  OutputIterator
    inclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result,
                   BinaryOperation binary_op);

template<class InputIterator, class OutputIterator,
         class T, class BinaryOperation>
  OutputIterator
    inclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result,
                   T init, BinaryOperation binary_op);
```

1. *Effects:* For each iterator `i` in `[result,result + (last - first))`, produces a result such that upon
    completion of the algorithm, `*i` yields the generalized sum of `init`, if it is provided as a parameter,
    and the elements in the range `[first, first + (i - result)]`.

    During execution of the algorithm, every evaluation of the above sum is either of the corresponding form

    `binary_op(binary_op(init,A), B)` or `binary_op(A, B)`

    where there exists some iterator `j` in `[first, last)` such that:

    1. `A` is the partial sum of elements in the range `[j, j + n)`.

    2. `B` is the partial sum of elements in the range `[j + n, j + m)`.

    3. `n` and `m` are positive integers and `j + m < last`.

2. *Returns:* The end of the resulting range beginning at `result`.

3. *Requires:* `binary_op` shall have associativity.

    `binary_op` shall not invalidate iterators or subranges, nor modify elements in the ranges `[first,last)` or `[result,result + (last - first))`.

4. *Complexity:* `O(last - first)`.

5. *Notes:* The primary difference between `exclusive_scan` and `inclusive_scan` is that `inclusive_scan` includes the `i`th input element in the `i`th sum.

