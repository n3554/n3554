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
         for_each(par, std::begin(a), std::end(a), [](int n) {
           x.fetch_add(1, std::memory_order_relaxed);
           // spin wait for another iteration to change the value of x
           while (x.load(std::memory_order_relaxed) == 1) { }
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
    
    -- *end example*]

    [*Note:* the application of the function object may result in two consecutive calls to `m.lock` on the same thread, which may deadlock -– *end note*]

    [*Note:* The semantics of the `parallel_execution_policy` or the `vector_execution_policy` invocation allow the implementation to fall back to sequential execution if the system cannot parallelize an algorithm invocation due to lack of resources. -- *end note*.]

5. If they exist, a parallel algorithm invoked with an execution policy object of type `parallel_execution_policy` or `vector_execution_policy` may apply iterator member functions of a stronger category than its specification requires. In this case,
   the application of these member functions are subject to provisions 3. and 4. above, respectively.

    [*Note:* For example, an algorithm whose specification requires `InputIterator` but receives a concrete iterator of the category `RandomAccessIterator` may use `operator[]`. In this
    case, it is the algorithm caller's responsibility to ensure `operator[]` is race-free. -- *end note*.]

6. Algorithms invoked with an execution policy object of type `execution_policy` execute internally as if invoked with instances of type `sequential_execution_policy`, `parallel_execution_policy`, or an implementation-defined execution policy type depending on the dynamic value of the `execution_policy` object.

7. The semantics of parallel algorithms invoked with an execution policy object of implementation-defined type are unspecified.

### `ExecutionPolicy` algorithm overloads {#parallel.alg.overloads}

1. Parallel algorithms coexist alongside their sequential counterparts as overloads distinguished by a formal template parameter named `ExecutionPolicy`.
   This template parameter corresponds to the parallel algorithm's first function parameter, whose type is `ExecutionPolicy&&`.

2. Unless otherwise specified, the semantics of `ExecutionPolicy` algorithm overloads are identical to their overloads without.

3. Parallel algorithms have the requirement `is_execution_policy<ExecutionPolicy>::value` is `true`.

4. The algorithms listed in table 1 shall have `ExecutionPolicy` overloads.

-------------------------- ------------------------- ------------------- --------------------------
`adjacent_find`            `for_each`                `none_of`           `search`                  
`all_of`                   `for_each_n`              `nth_element`       `search_n`                
`any_of`                   `generate`                `partial_sort`      `set_difference`          
`copy`                     `generate_n`              `partial_sort_copy` `set_intersection`        
`copy_if`                  `includes`                `partition`         `set_symmetric_difference`
`copy_n`                   `inclusive_scan`          `partition_copy`    `set_union`               
`count`                    `inplace_merge`           `reduce`            `sort`                    
`count_if`                 `is_heap`                 `remove`            `stable_partition`        
`equal`                    `is_partitioned`          `remove_copy`       `stable_sort`             
`exclusive_scan`           `is_sorted`               `remove_copy_if`    `swap_ranges`             
`fill`                     `is_sorted_until`         `remove_if`         `transform`               
`fill_n`                   `lexicographical_compare` `replace`           `uninitialized_copy`      
`find`                     `max_element`             `replace_copy`      `uninitialized_copy_n`    
`find`                     `merge`                   `replace_copy_if`   `uninitialized_fill`      
`find_end`                 `min_element`             `reverse`           `uninitialized_fill_n`    
`find_first_of`            `minmax_element`          `reverse_copy`      `unique`                  
`find_if`                  `mismatch`                `rotate`            `unique_copy`             
`find_if_not`              `move`                    `rotate_copy`        
-------------------------- ------------------------- ------------------- --------------------------

: Table of parallel algorithms

## Definitions {#parallel.alg.defns}

\color{addclr}
1. Define \texttt{\textit{GENERALIZED\_SUM}(op, a1, ..., aN)} as follows:

   * `a1` when `N` is `1`

   * \texttt{op(\textit{GENERALIZED\_SUM}(op, b1, ..., bM), \textit{GENERALIZED\_SUM}(op, bM, ..., bN))} where
     
      * `b1, ..., bN` may be any permutation of `a1, ..., aN` and
      * `0 < M < N`.

2. Define \texttt{\textit{GENERALIZED\_NONCOMMUTATIVE\_SUM}(op, a1, ..., aN)} as follows:

   * `a1` when `N` is `1`

   * \texttt{op(\textit{GENERALIZED\_NONCOMMUTATIVE\_SUM}(op, a1, ..., aM), \textit{GENERALIZED\_NONCOMMUTATIVE\_SUM}(op, aM, ..., aN)} where `0 < M < N`.

\color{black}

## Novel algorithms {#parallel.alg.novel}

This subclause describes novel algorithms introduced by this technical specification.

### Header `<experimental/algorithm>` synopsis {#parallel.alg.novel.algorithm.synop}

\begin{Verbatim}[commandchars=\\\[\]]
namespace std {
namespace experimental {
namespace parallel {
  template<class ExecutionPolicy,
           class InputIterator, class Function>
\remvrbline[    InputIterator for_each(ExecutionPolicy&& exec,]
\remvrbline[                           InputIterator first, InputIterator last,]
\remvrbline[                           Function f);]
\addvrbline[    void for_each(ExecutionPolicy&& exec,]
\addvrbline[                  InputIterator first, InputIterator last,]
\addvrbline[                  Function f);]
  template<class InputIterator, class Size, class Function>
\remvrbline[    Function for_each_n(InputIterator first, Size n,]
\remvrbline[                        Function f);]
\addvrbline[    InputIterator for_each_n(InputIterator first, Size n,]
\addvrbline[                             Function f);]
\remvrbline[  template<class ExecutionPolicy,]
\remvrbline[           class InputIterator, class Size, class Function>]
\remvrbline[    InputIterator for_each_n(ExecutionPolicy&& exec, InputIterator first, Size n,]
\remvrbline[                             Function f);]
}
}
}
\end{Verbatim}

### For each {#parallel.alg.novel.foreach}

\begin{Verbatim}[commandchars=\\\[\]]
template<class ExecutionPolicy,
         class InputIterator, class Function>
\remvrbline[  InputIterator for_each(ExecutionPolicy&& exec,]
\remvrbline[                         InputIterator first, InputIterator last,]
\remvrbline[                         Function f);]
\addvrbline[  void for_each(ExecutionPolicy&& exec,]
\addvrbline[                InputIterator first, InputIterator last,]
\addvrbline[                Function f);]
\end{Verbatim}

1. *Effects:* Applies `f` to the result of dereferencing every iterator in the range `[first,last)`.
   [*Note:* If the type of `first` satisfies
   the requirements of a mutable iterator, `f` may apply nonconstant functions through the dereferenced
   iterator. -- *end note*]

2. \color{remclr}~~*Returns:* `first + (last - first)`.~~\color{black}

3. *Complexity:* Applies `f` exactly `last - first` times.

4. *Remarks:* If `f` returns a result, the result is ignored.

5. *Note:* Unlike its sequential form, the parallel overload of `for_each` does not return a copy of its `Function` parameter, since parallelization may not permit efficient state accumulation.

\begin{Verbatim}[commandchars=\\\[\]]
template<class InputIterator, class Size, class Function>
\remvrbline[  Function for_each_n(InputIterator first, Size n,]
\remvrbline[                      Function f);]
\addvrbline[  InputIterator for_each_n(InputIterator first, Size n,]
\addvrbline[                           Function f);]
\end{Verbatim}

1. *Requires:* `Function` shall meet the requirements of `MoveConstructible` [*Note:* `Function need not meet the requirements of `CopyConstructible`. -- *end note*]

2. *Effects:* Applies `f` to the result of dereferencing every iterator in the range `[first,first + n)`, starting from `first` and proceeding to `first + n - 1`.
    [*Note:* If the type of `first` satisfies the requirements of a mutable iterator, `f` may apply nonconstant functions through the dereferenced iterator. -- *end note*]

3. *Returns:* \color{remclr}~~`std::move(f)`~~\color{addclr}`first + n`\color{black}.

4. *Complexity:* Applies `f` exactly `n` times.

5. *Remarks:* If `f` returns a result, the result is ignored.


\begin{Verbatim}[commandchars=\\\[\]]
\remvrbline[template<class ExecutionPolicy,]
\remvrbline[         class InputIterator, class Size, class Function>]
\remvrbline[  InputIterator for_each_n(ExecutionPolicy&& exec,]
\remvrbline[                           InputIterator first, Size n,]
\remvrbline[                           Function f);]
\end{Verbatim}

\color{remclr}
~~1. *Requires:* `Function` shall meet the requirements of `MoveConstructible` [*Note:* `Function` need not meet the requirements of `CopyConstructible`. -- *end note*]~~

~~2. *Effects:* Applies `f` to the result of dereferencing every iterator in the range `[first,first + n)`, starting from `first` and proceeding to `first + n - 1`.
    [*Note:* If the type of `first` satisfies the requirements of a mutable iterator, `f` may apply nonconstant functions through the dereferenced iterator. -- *end note*]~~

~~3. *Returns:* `first + n`.~~

~~4. *Complexity:* Applies `f` exactly `n` times.~~

~~5. *Remarks:* If `f` returns a result, the result is ignored.~~
\color{black}

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

1. *Returns:* `reduce(first, last, typename iterator_traits<InputIterator>::value_type`\color{remclr}~~`(0)`~~\color{addclr}`{}`\color{black}`)`

2. *Requires:* `typename iterator_traits<InputIterator>::value_type`\color{remclr}~~`(0)`~~\color{addclr}`{}` \color{black} shall be a valid expression. The `operator+` function associated with `iterator_traits<InputIterator>::value_type` shall not invalidate iterators or subranges, nor modify elements in the range `[first,last)`.

3. *Complexity:* $\bigoh{\texttt{last - first}}$ applications of `operator+`.

4. *Note:* The primary difference between `reduce` and `accumulate` is that the behavior of `reduce` may be non-deterministic for non-associative or non-commutative `operator+`.

```
template<class InputIterator, class T>
  T reduce(InputIterator first, InputIterator last, T init);
```

1. *Returns:* `reduce(first, last, init, plus<>())`

2. *Requires:* The `operator+` function associated with `T` shall not invalidate iterators or subranges, nor modify elements in the range `[first,last)`.

3. *Complexity:* $\bigoh{\texttt{last - first}}$ applications of `operator+`.

4. *Note:* The primary difference between `reduce` and `accumulate` is that the behavior of `reduce` may be non-deterministic for non-associative or non-commutative `operator+`.

```
template<class InputIterator, class T, class BinaryOperation>
  T reduce(InputIterator first, InputIterator last, T init,
           BinaryOperation binary_op);
```

1. *Returns:* \texttt{\textit{GENERALIZED\_SUM}(binary\_op, init, *first, ..., *(first + last - first - 1))}.

2. *Requires:* `binary_op` shall not invalidate iterators or subranges, nor modify elements in the range `[first,last)`.

3. *Complexity:* $\bigoh{\texttt{last - first}}$ applications of `binary_op`.

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

3. *Complexity:* $\bigoh{\texttt{last - first}}$ applications of `operator+`.

4. *Notes:* The primary difference between `exclusive_scan` and `inclusive_scan` is that `exclusive_scan` excludes the `i`th input element from the `i`th sum.
    If the `operator+` function is not mathematically associative, the behavior of `exclusive_scan` may be non-deterministic.

```
template<class InputIterator, class OutputIterator,
         class T, class BinaryOperation>
  OutputIterator
    exclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result,
                   T init, BinaryOperation binary_op);
```

1. *Effects:* Assigns through each iterator `i` in `[result,result + (last - first))` the value of
    \texttt{\textit{GENERALIZED\_NONCOMMUTATIVE\_SUM}(binary\_op, init, *first, ..., (*first + i - result - 1))}.

2. *Returns:* The end of the resulting range beginning at `result`.

3. *Requires:* `binary_op` shall not invalidate iterators or subranges, nor modify elements in the ranges `[first,last)` or `[result,result + (last - first))`.

4. *Complexity:* $\bigoh{\texttt{last - first}}$ applications of `binary_op`.

5. *Notes:* The primary difference between `exclusive_scan` and `inclusive_scan` is that `exclusive_scan` excludes the `i`th input element from the `i`th sum.
    If `binary_op` is not mathematically associative, the behavior of `exclusive_scan` may be non-deterministic.


### Inclusive scan {#parallel.alg.novel.inclusive.scan}

```
template<class InputIterator, class OutputIterator>
  OutputIterator
    inclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result);
```

1. *Returns:* `inclusive_scan(first, last, result, plus<>())`

2. *Requires:* The `operator+` function associated with `iterator_traits<InputIterator>::value_type` shall not invalidate iterators or subranges, nor modify elements in the ranges `[first,last)` or `[result,result + (last - first))`.

3. *Complexity:* $\bigoh{\texttt{last - first}}$ applications of `operator+`.

4. *Notes:* The primary difference between `exclusive_scan` and `inclusive_scan` is that `exclusive_scan` excludes the `i`th input element from the `i`th sum.
    If the `operator+` function is not mathematically associative, the behavior of `inclusive_scan` may be non-deterministic.

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

1. *Effects:* Assigns through each iterator `i` in `[result,result + (last - first))` the value of
    \texttt{\textit{GENERALIZED\_NONCOMMUTATIVE\_SUM}(binary\_op, *first, ..., (*first + i - result))} or
    \texttt{\textit{GENERALIZED\_NONCOMMUTATIVE\_SUM}(binary\_op, init, *first, ..., (*first + i - result))} if `init` is provided.

2. *Returns:* The end of the resulting range beginning at `result`.

3. *Requires:* binary_op` shall not invalidate iterators or subranges, nor modify elements in the ranges `[first,last)` or `[result,result + (last - first))`.

4. *Complexity:* $\bigoh{\texttt{last - first}}$ applications of `binary_op`.

5. *Notes:* The primary difference between `exclusive_scan` and `inclusive_scan` is that `inclusive_scan` includes the `i`th input element in the `i`th sum.
    If `binary_op` is not mathematically associative, the behavior of `inclusive_scan` may be non-deterministic.

