## Generalized numeric operations [alg.numerics] {#alg.numerics}

This subclause describes components that C++ programs may use to perform seminumerical operations in parallel.

### Header `<experimental/numeric>` synopsis [alg.numerics.synop] {#alg.numerics.synop}

```
namespace std {
  template<class InputIterator>
    typename iterator_traits<InputIterator>::value_type
      reduce(InputIterator first, InputIterator last);
  template<class ExecutionPolicy,
           class InputIterator>
    typename iterator_traits<InputIterator>::value_type
      reduce(ExecutionPolicy &&exec,
             InputIterator first, InputIterator last);
  template<class InputIterator, class T>
    T reduce(InputIterator first, InputIterator last T init);
  template<class ExecutionPolicy,
           class InputIterator, class T>
    T reduce(ExecutionPolicy &&exec,
             InputIterator first, InputIterator last, T init);
  template<class InputIterator, class T, class BinaryOperation>
    T reduce(InputIterator first, InputIterator last, T init,
             BinaryOperation binary_op);
  template<class ExecutionPolicy,
           class InputIterator, class T, class BinaryOperation>
    T reduce(ExecutionPolicy &&exec,
             InputIterator first, InputIterator last, T init,
             BinaryOperation binary_op);

  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2, class T>
    T inner_product(ExecutionPolicy &&exec,
                    InputIterator1 first1, InputIterator1 last1,
                    InputIterator2 first2, T init);
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2, class T,
           class BinaryOperation1, class BinaryOperation2>
    T inner_product(ExecutionPolicy &&exec,
                    InputIterator1 first1, InputIterator1 last1,
                    InputIterator2 first2, T init,
                    BinaryOperation1 binary_op1,
                    BinaryOperation2 binary_op2);

  template<class InputIterator, class OutputIterator>
    OutputIterator
      exclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator>
    OutputIterator
      exclusive_scan(ExecutionPolicy &&exec,
                     InputIterator first, InputIterator last,
                     OutputIterator result);
  template<class InputIterator, class OutputIterator,
           class T>
    OutputIterator
      exclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result,
                     T init);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator,
           class T>
    OutputIterator
      exclusive_scan(ExecutionPolicy &&exec,
                     InputIterator first, InputIterator last,
                     OutputIterator result,
                     T init);
  template<class InputIterator, class OutputIterator,
           class T, class BinaryOperation>
    OutputIterator
      exclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result,
                     T init, BinaryOperation binary_op);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator,
           class T, class BinaryOperation>
    OutputIterator
      exclusive_scan(ExecutionPolicy &&exec,
                     InputIterator first, InputIterator last,
                     OutputIterator result,
                     T init, BinaryOperation binary_op);

  template<class InputIterator, class OutputIterator>
    OutputIterator
      inclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator>
    OutputIterator
      inclusive_scan(ExecutionPolicy &&exec,
                     InputIterator first, InputIterator last,
                     OutputIterator result);
  template<class InputIterator, class OutputIterator,
           class BinaryOperation>
    OutputIterator
      inclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result,
                     BinaryOperation binary_op);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator,
           class BinaryOperation>
    OutputIterator
      inclusive_scan(ExecutionPolicy &&exec,
                     InputIterator first, InputIterator last,
                     OutputIterator result,
                     BinaryOperation binary_op);
  template<class InputIterator, class OutputIterator,
           class T, class BinaryOperation>
    OutputIterator
      inclusive_scan(InputIterator first, InputIterator last,
                     OutputIterator result,
                     T init, BinaryOperation binary_op);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator,
           class T, class BinaryOperation>
    OutputIterator
      inclusive_scan(ExecutionPolicy &&exec,
                     InputIterator first, InputIterator last,
                     OutputIterator result,
                     T init, BinaryOperation binary_op);

  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator>
    OutputIterator adjacent_difference(ExecutionPolicy &&exec,
                                       InputIterator first, InputIterator last,
                                       OutputIterator result);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator, class BinaryOperation>
    OutputIterator adjacent_difference(ExecutionPolicy &&exec,
                                       InputIterator first, InputIterator last,
                                       OutputIterator result,
                                       BinaryOperation binary_op);
}
```

### Reduce [alg.numerics.reduce] {#alg.numerics.reduce}

```
template<class InputIterator>
  typename iterator_traits<InputIterator>::value_type
    reduce(InputIterator first, InputIterator last);

template<class ExecutionPolicy,
         class InputIterator>
  typename iterator_traits<InputIterator>::value_type
    reduce(ExecutionPolicy &&exec,
           InputIterator first, InputIterator last);
```

1. *Effects:* The second algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The result of the sum of `T(0)` and the elements in the range `[first,last)`.

    The order of operands of the sum is unspecified.

3. *Requires:* Let `T` be the type of `InputIterator`'s value type, then `T(0)` shall be a valid expression. The `operator+` function associated with `T` shall have associativity and commutativity.

    `operator+` shall not invalidate iterators or subranges, nor modify elements in the range `[first,last)`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The second signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class InputIterator, class T>
  T reduce(InputIterator first, InputIterator last, T init);

template<class ExecutionPolicy,
         class InputIterator, class T>
  T reduce(ExecutionPolicy &&exec,
           InputIterator first, InputIterator last, T init);

template<class InputIterator, class T, class BinaryOperation>
  T reduce(InputIterator first, InputIterator last, T init,
           BinaryOperation binary_op);

template<class ExecutionPolicy,
         class InputIterator, class T, class BinaryOperation>
  T reduce(ExecutionPolicy &&exec,
           InputIterator first, InputIterator last, T init,
           BinaryOperation binary_op);
```

1. *Effects:* The execution of the second and fourth algorithms is parallelized as determined by `exec`.

2. *Returns:* The result of the generalized sum of `init` and the elements in the range `[first,last)`.

    Sums of elements are evaluated with `operator+` or `binary_op`. The order of operands of the sum is unspecified.

3. *Requires:* The `operator+` function associated with `InputIterator`'s value type or `binary_op` shall have associativity and commutativity.

    Neither `operator+` nor `binary_op` shall invalidate iterators or subranges, nor modify elements in the range `[first,last)`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The second and fourth signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Inner product [alg.numerics.inner.product] {#alg.numerics.inner.product}


```
template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2, class T>
  T inner_product(ExecutionPolicy &&exec,
                  InputIterator1 first1, InputIterator1 last1,
                  InputIterator2 first2, T init);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The result of the sum `init + (*(first1 + i) * *(first2 + i) + ... ` for every integer `i` in the range `[0, (last1 - first1))`.

    The order of operands of the sum is unspecified.

3. *Requires:* `operator+` shall have associativity and commutativity.

    `operator+` shall not invalidate iterators or subranges, nor modify elements in the ranges `[first1,last1)` or `[first2,first2 + (last1 - first1))`.

4. *Complexity:* `O(last1 - first1)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.


```
template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2, class T,
         class BinaryOperation1, class BinaryOperation2>
  T inner_product(ExecutionPolicy &&exec,
                  InputIterator1 first1, InputIterator1 last1,
                  InputIterator2 first2, T init,
                  BinaryOperation1 binary_op1,
                  BinaryOperation2 binary_op2);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The result of the generalized sum whose operands are `init` and the result of the pairwise binary operation `binary_op2(*i,*(first2 + (i - first1)))` for all iterators `i` in the range `[first1,last1)`.

    The generalized sum's operands are combined via application of the pairwise binary operation `binary_op1`.

    The order of operands of the sum is unspecified.

3. *Requires:* `binary_op1` shall have associativity and commutativity.

    `binary_op1` and `binary_op2` shall neither invalidate iterators or subranges, nor modify elements in the ranges `[first1,last1)` or `[first2,first2 + (last1 - first1))`.

4. *Complexity:* `O(last1 - first1)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Exclusive scan [alg.numerics.exclusive.scan] {#alg.numerics.exclusive.scan}

```
template<class InputIterator, class OutputIterator,
         class T>
  OutputIterator
    exclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result,
                   T init);

template<class ExecutionPolicy,
         class InputIterator, class OutputIterator,
         class T>
  OutputIterator
    exclusive_scan(ExecutionPolicy &&exec,
                   InputIterator first, InputIterator last,
                   OutputIterator result,
                   T init);

template<class InputIterator, class OutputIterator,
         class T, class BinaryOperation>
  OutputIterator
    exclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result,
                   T init, BinaryOperation binary_op);

template<class ExecutionPolicy,
         class InputIterator, class OutputIterator,
         class T, class BinaryOperation>
  OutputIterator
    exclusive_scan(ExecutionPolicy &&exec,
                   InputIterator first, InputIterator last,
                   OutputIterator result,
                   T init, BinaryOperation binary_op);
```

1. *Effects:* For each iterator `i` in `[result,result + (last - first))`, produces a result such that upon
    completion of the algorithm, `*i` yields the generalized sum of `init` and the elements in the range
    `[first, first + (i - result))`.

    During execution of the algorithm, every evaluation of the above sum is either of the corresponding form

    `(init + A) + B)` or `A + B` or

    `binary_op(binary_op(init,A), B)` or `binary_op(A, B)`

    where there exists some iterator `j` in `[first, last)` such that:

    1. `A` is the partial sum of elements in the range `[j, j + n)`.

    2. `B` is the partial sum of elements in the range `[j + n, j + m)`.

    3. `n` and `m` are positive integers and `j + m < last`.

    The execution of the second and fourth algorithms is parallelized as determined by `exec`.

2. *Returns:* The end of the resulting range beginning at `result`.

3. *Requires:* The `operator+` function associated with `InputIterator`'s value type or `binary_op` shall have associativity.

    Neither `operator+` nor `binary_op` shall invalidate iterators or subranges, nor modify elements in the ranges `[first,last)` or `[result,result + (last - first))`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The second and fourth signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

6. *Notes:* The primary difference between `exclusive_scan` and `inclusive_scan` is that `exclusive_scan` excludes the `i`th input element from the `i`th sum.

### Inclusive scan [alg.numerics.inclusive.scan] {#alg.numerics.inclusive.scan}

```
template<class InputIterator, class OutputIterator>
  OutputIterator
    inclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result);

template<class ExecutionPolicy,
         class InputIterator, class OutputIterator>
  OutputIterator
    inclusive_scan(ExecutionPolicy &&exec,
                   InputIterator first, InputIterator last,
                   OutputIterator result);

template<class InputIterator, class OutputIterator,
         class BinaryOperation>
  OutputIterator
    inclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result,
                   BinaryOperation binary_op);

template<class ExecutionPolicy,
         class InputIterator, class OutputIterator,
         class BinaryOperation>
  OutputIterator
    inclusive_scan(ExecutionPolicy &&exec,
                   InputIterator first, InputIterator last,
                   OutputIterator result,
                   BinaryOperation binary_op);

template<class InputIterator, class OutputIterator,
         class T, class BinaryOperation>
  OutputIterator
    inclusive_scan(InputIterator first, InputIterator last,
                   OutputIterator result,
                   T init, BinaryOperation binary_op);

template<class ExecutionPolicy,
         class InputIterator, class OutputIterator,
         class T, class BinaryOperation>
  OutputIterator
    inclusive_scan(ExecutionPolicy &&exec,
                   InputIterator first, InputIterator last,
                   OutputIterator result,
                   T init, BinaryOperation binary_op);
```

1. *Effects:* For each iterator `i` in `[result,result + (last - first))`, produces a result such that upon
    completion of the algorithm, `*i` yields the generalized sum of `init` and the elements in the range
    `[first, first + (i - result)]`.

    During execution of the algorithm, every evaluation of the above sum is either of the corresponding form

    `(init + A) + B)` or `A + B` or

    `binary_op(binary_op(init,A), B)` or `binary_op(A, B)`

    where there exists some iterator `j` in `[first, last)` such that:

    1. `A` is the partial sum of elements in the range `[j, j + n)`.

    2. `B` is the partial sum of elements in the range `[j + n, j + m)`.

    3. `n` and `m` are positive integers and `j + m < last`.

    The execution of the second and fourth algorithms is parallelized as determined by `exec`.

2. *Returns:* The end of the resulting range beginning at `result`.

3. *Requires:* The `operator+` function associated with `InputIterator`'s value type or `binary_op` shall have associativity.

    Neither `operator+` nor `binary_op` shall invalidate iterators or subranges, nor modify elements in the ranges `[first,last)` or `[result,result + (last - first))`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The second, fourth, and sixth signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

6. *Notes:* The primary difference between `exclusive_scan` and `inclusive_scan` is that `inclusive_scan` includes the `i`th input element in the `i`th sum.

### Adjacent difference [alg.numerics.adjacent.difference] {#alg.numerics.adjacent.difference}

```
template<class ExecutionPolicy,
         class InputIterator, class OutputIterator>
  OutputIterator adjacent_difference(ExecutionPolicy &&exec,
                                     InputIterator first, InputIterator last,
                                     OutputIterator result);

template<class ExecutionPolicy,
         class InputIterator, class OutputIterator, class BinaryOperation>
  OutputIterator adjacent_difference(ExecutionPolicy &&exec,
                                     InputIterator first, InputIterator last,
                                     OutputIterator result,
                                     BinaryOperation binary_op);
```

1. *Effects:* Performs `*result = *first` and for each iterator `i` in the range `[first + 1, last)`, performs `*result = *i - *(i - 1)`, or `*result = binary_op(*i, *(i - 1))`, respectively.

    The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `result + (last - first)`.

3. *Requires:* The result of the expression `*i - *(i - 1)` or `binary_op(*i, *(i - 1))` shall be writable to the `result` output iterator. 

    Neither `operator-` nor `binary_op` shall invalidate iterators or subranges, nor modify elements in the range `[first,last)` or `[result,result + (last - first))`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* `result` may be equal to `first`.

    The signatures shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.

