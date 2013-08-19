# Overloads of Existing Algorithms Introduced by this Proposal

## Existing specialized algorithms from `<memory>`

### Header `<memory>` synopsis

```
namespace std {
  // specialized algorithms
  template<class ExecutionPolicy,
           class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(ExecutionPolicy &&exec,
                                       InputIterator first, InputIterator last,
                                       ForwardIterator result);
  template<class ExecutionPolicy,
           class ForwardIterator, class T>
    void uninitialized_fill(ExecutionPolicy &&exec,
                            ForwardIterator first, ForwardIterator last
                            const T& x);
  template<class ExecutionPolicy,
           class ForwardIterator, class Size>
    ForwardIterator uninitialized_fill_n(ExecutionPolicy &&exec,
                                         ForwardIterator first, Size n,
                                         const T& x);
}
```

### `uninitialized_copy` `[uninitialized.copy]`

```
template<class ExecutionPolicy,
         class InputIterator, class ForwardIterator>
  ForwardIterator uninitialized_copy(ExecutionPolicy &&exec,
                                     InputIterator first, InputIterator last,
                                     ForwardIterator result);
template<class ExecutionPolicy,
         class InputIterator, class Size, class ForwardIterator>
  ForwardIterator uninitialized_copy_n(ExecutionPolicy &&exec,
                                       InputIterator first, Size n,
                                       ForwardIterator result);
```

1. *Effects:* Copy constructs the element referenced by every iterator `i` in the range `[result,result + (last - first))` as if by the expression

    ```
    ::new (static_cast<void*>(&*i))
        typename iterator_traits<ForwardIterator>::value_type(*(first + (i - result)))
    ```
    
    The execution of the algorithm is parallelized as determined by `exec`.

2. *Returns:* `result + (last - first)`.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* Neither signature shall participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class InputIterator, class Size, class ForwardIterator>
  ForwardIterator uninitialized_copy_n(ExecutionPolicy &&exec,
                                       InputIterator first, Size n,
                                       ForwardIterator result);
```

1. *Effects:* Copy constructs the element referenced by every iterator `i` in the range `[result,result + n)` as if by the expression

    ```
    ::new (static_cast<void*>(&*i))
        typename iterator_traits<ForwardIterator>::value_type(*(first + (i - result)))
    ```
   
   The execution of the algorithm is parallelized as determined by `exec`.

2. *Returns:* `result + n`.

3. *Complexity:* `O(n)`.

4. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### `uninitialized_fill` `[uninitialized.fill]`

```
template<class ExecutionPolicy,
         class ForwardIterator, class T>
  void uninitialized_fill(ExecutionPolicy &&exec,
                          ForwardIterator first, ForwardIterator last
                          const T& x);
```

1. *Effects:* Copy constructs the element referenced by every iterator `i` in the range `[first,last)` as if by the expression

    ```
    ::new (static_cast<void*>(&*i))
        typename iterator_traits<ForwardIterator>::value_type(x)
    ```

   The execution of the algorithm is parallelized as determined by `exec`.

2. *Complexity:* `O(last - first)`.

3. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### `uninitialized_fill_n` `[uninitialized.fill.n]`

```
template<class ExecutionPolicy,
         class ForwardIterator, class Size>
  ForwardIterator uninitialized_fill_n(ExecutionPolicy &&exec,
                                       ForwardIterator first, Size n,
                                       const T& x);
```

1. *Effects:* Copy constructs the element referenced by every iterator `i` in the range `[first,first + n)` as if by the expression

    ```
    ::new (static_cast<void*>(&*i))
        typename iterator_traits<ForwardIterator>::value_type(x)
    ```

    The execution of the algorithm is parallelized as determined by `exec`.

2. *Returns:* `first + n`.

3. *Complexity:* `O(n)`.

4. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

## Existing function Templates from `<algorithm>`

### Header `<algorithm>` synopsis

```
namespace std {
  // non-modifying sequence operations:
  template<class ExecutionPolicy,
           class InputIterator, class Predicate>
    bool all_of(ExecutionPolicy &&exec,
                InputIterator first, InputIterator last, Predicate pred);
  template<class ExecutionPolicy,
           class InputIterator, class Predicate>
    bool any_of(ExecutionPolicy &&exec,
                InputIterator first, InputIterator last, Predicate pred);
  template<class ExecutionPolicy,
           class InputIterator, class Predicate>
    bool none_of(ExecutionPolicy &&exec,
                 InputIterator first, InputIterator last, Predicate pred);

  template<class ExecutionPolicy,
           class InputIterator, class T>
    InputIterator find(ExecutionPolicy &&exec,
                       InputIterator first, InputIterator last,
                       const T& value);
  template<class ExecutionPolicy,
           class InputIterator, class Predicate>
    InputIterator find_if(ExecutionPolicy &&exec,
                          InputIterator first, InputIterator last,
                          Predicate pred);
  template<class ExecutionPolicy,
           class InputIterator, class Predicate>
    InputIterator find_if_not(ExecutionPolicy &&exec,
                              InputIterator first, InputIterator last,
                              Predicate pred);

  template<class ExecutionPolicy,
           class ForwardIterator1, class ForwardIterator2>
    ForwardIterator1
      find_end(ExecutionPolicy &exec,
               ForwardIterator1 first1, ForwardIterator1 last1,
               ForwardIterator2 first2, ForwardIterator2 last2);
  template<class ExecutionPolicy,
           class ForwardIterator1, class ForwardIterator2,
           class BinaryPredicate>
    ForwardIterator1
      find_end(ExecutionPolicy &&exec,
               ForwardIterator1 first1, ForwardIterator1 last1,
               ForwardIterator2 first2, ForwardIterator2 last2,
               BinaryPredicate pred);

  template<class ExecutionPolicy,
           class InputIterator, class ForwardIterator>
    InputIterator
      find_first_of(ExecutionPolicy &&exec,
                    InputIterator first1, InputIterator last1,
                    ForwardIterator first2, ForwardIterator last2);
  template<class ExecutionPolicy,
           class InputIterator, class ForwardIterator,
           class BinaryPredicate>
    InputIterator
      find_first_of(ExecutionPolicy &&exec,
                    InputIterator first1, InputIterator last1,
                    ForwardIterator first2, ForwardIterator last2,
                    BinaryPredicate pred);

  template<class ExecutionPolicy,
           class ForwardIterator>
    ForwardIterator adjacent_find(ExecutionPolicy &&exec, ForwardIterator first, ForwardIterator last);
  template<class ExecutionPolicy,
           class ForwardIterator, class BinaryPredicate>
    ForwardIterator adjacent_find(ExecutionPolicy &&exec, ForwardIterator first, ForwardIterator last,
                                  BinaryPredicate pred);

  template<class ExecutionPolicy,
           class InputIterator, class EqualityComparable>
    typename iterator_traits<InputIterator>::difference_type
      count(ExecutionPolicy &&exec,
            InputIterator first, InputIterator last, const EqualityComparable &value);
  template<class ExecutionPolicy,
           class InputIterator, class Predicate>
    typename iterator_traits<InputIterator>::difference_type
      count_if(ExecutionPolicy &&exec,
               InputIterator first, InputIterator last, Predicate pred);

  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2>
    pair<InputIterator1,InputIterator2>
      mismatch(ExecutionPolicy &&exec,
               InputIterator1 first1, InputIterator1 last1, 
               InputIterator2 first2);
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2, class BinaryPredicate>
    pair<InputIterator1,InputIterator2>
      mismatch(ExecutionPolicy &&exec,
               InputIterator1 first1, InputIterator1 last1,
               InputIterator2 first2, BinaryPredicate pred);

  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2>
    bool equal(ExecutionPolicy &&exec,
               InputIterator1 first1, InputIterator1 last1,
               InputIterator2 first2);
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2, class BinaryPredicate>
    bool equal(ExecutionPolicy &&exec,
               InputIterator1 first1, InputIterator1 last1,
               InputIterator2 first2, BinaryPredicate pred);

  template<class ExecutionPolicy,
           class ForwardIterator1, class ForwardIterator2>
    ForwardIterator1 search(ExecutionPolicy &&exec,
                            ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2);
  template<class ExecutionPolicy,
           class ForwardIterator1, class ForwardIterator2,
           class BinaryPredicate>
    ForwardIterator1 search(ExecutionPolicy &&exec,
                            ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            BinaryPredicate pred);
  template<class ExecutionPolicy,
           class ForwardIterator, class Size, class T>
    ForwardIterator search_n(ExecutionPolicy &&exec,
                             ForwardIterator first, ForwardIterator last, Size count,
                             const T& value);
  template<class ExecutionPolicy,
           class ForwardIterator, class Size, class T, class BinaryPredicate>
    ForwardIterator search_n(ExecutionPolicy &&exec,
                             ForwardIterator first, ForwardIterator last, Size count,
                             const T& value, BinaryPredicate pred);

  // modifying sequence operations:
  // copy:
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator>
    OutputIterator copy(ExecutionPolicy &&exec,
                        InputIterator first, InputIterator last,
                        OutputIterator result);

  template<class ExecutionPolicy,
           class InputIterator, class Size, class OutputIterator>
    OutputIterator copy_n(ExecutionPolicy &&exec,
                          InputIterator first, Size n,
                          OutputIterator result);

  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator, class Predicate>
    OutputIterator
      copy_if(ExecutionPolicy &&exec,
              InputIterator first, InputIterator last,
              OutputIterator result, Predicate pred);

  // move:
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator>
    OutputIterator
      move(ExecutionPolicy &&exec,
           InputIterator first, InputIterator last,
           OutputIterator result);

  // swap:
  template<class ExecutionPolicy,
           class ForwardIterator1, class ForwardIterator2>
    ForwardIterator2
      swap_ranges(ExecutionPolicy &&exec,
                  ForwardIterator1 first1, ForwardIterator1 last1,
                  ForwardIterator1 first2);

  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator,
           class UnaryOperation>
    OutputIterator transform(ExecutionPolicy &&exec,
                             InputIterator first, InputIterator last,
                             OutputIterator result, UnaryOperation op);
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2, class OutputIterator,
           class BinaryOperation>
    OutputIterator
      transform(ExecutionPolicy &&exec,
                InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, OutputIterator result,
                BinaryOperation binary_op);

  template<class ExecutionPolicy,
           class ForwardIterator, class T>
    void replace(ExecutionPolicy &&exec,
                 ForwardIterator first, ForwardIterator last,
                 const T& old_value, const T& new_value);
  template<class ExecutionPolicy,
           class ForwardIterator, class Predicate, class T>
    void replace_if(ExecutionPolicy &&exec,
                    ForwardIterator first, ForwardIterator last,
                    Predicate pred, const T& new_value);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator, class T>
    OutputIterator
      replace_copy(ExecutionPolicy &&exec,
                   ForwardIterator first, ForwardIterator last,
                   OutputIterator result,
                   const T& old_value, const T& new_value);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator, class Predicate, class T>
    OutputIterator
      replace_copy_if(ExecutionPolicy &&exec,
                      InputIterator first, InputIterator last,
                      OutputIterator result,

  template<class ExecutionPolicy,
           class ForwardIterator, class T>
    void fill(ExecutionPolicy &&exec,
              ForwardIterator first, ForwardIterator last, const T& value);
  template<class ExecutionPolicy,
           class OutputIterator, class Size, class T>
    void fill_n(ExecutionPolicy &&exec,
                OutputIterator first, Size n, const T& value);

  template<class ExecutionPolicy,
           class ForwardIterator, class Generator>
    void generate(ExecutionPolicy &&exec,
                  ForwardIterator first, ForwardIterator last, Generator gen);
  template<class ExecutionPolicy,
           class OutputIterator, class Size, class Generator>
    OutputIterator generate_n(ExecutionPolicy &&exec,
                              OutputIterator first, Size n, Generator gen);

  template<class ExecutionPolicy,
           class ForwardIterator, class T>
    ForwardIterator remove(ExecutionPolicy &&exec,
                           ForwardIterator first, ForwardIterator last, const T& value);
  template<class ExecutionPolicy,
           class ForwardIterator, class Predicate>
    ForwardIterator remove_if(ExecutionPolicy &&exec,
                              ForwardIterator first, ForwardIterator last, Predicate pred);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator, class T>
    OutputIterator
      remove_copy(ExecutionPolicy &&exec,
                  InputIterator first, InputIterator last,
                  OutputIterator result, const T& value);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator, class Predicate>
    OutputIterator
      remove_copy_if(ExecutionPolicy &&exec,
                     InputIterator first, InputIterator last,
                     OutputIterator result, Predicate pred);

  template<class ExecutionPolicy,
           class ForwardIterator>
    ForwardIterator unique(ExecutionPolicy &&exec,
                           ForwardIterator first, ForwardIterator last);
  template<class ExecutionPolicy,
           class ForwardIterator, typename BinaryPredicate>
    ForwardIterator unique(ExecutionPolicy &&exec,
                           ForwardIterator first, ForwardIterator last
                           BinaryPredicate pred);

  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator>
    OutputIterator
      unique_copy(ExecutionPolicy &&exec,
                  InputIterator first, InputIterator last,
                  OutputIterator result);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator, class BinaryPredicate>
    OutputIterator
      unique_copy(ExecutionPolicy &&exec,
                  InputIterator first, InputIterator last,
                  OutputIterator result, BinaryPredicate pred);

  template<class ExecutionPolicy,
           class BidirectionalIterator>
    void reverse(ExecutionPolicy &&exec,
                 BidirectionalIterator first, BidirectionalIterator last);

  template<class ExecutionPolicy,
           class BidirectionalIterator, class OutputIterator>
    OutputIterator
      reverse_copy(ExecutionPolicy &&exec,
                   BidirectionalIterator first,
                   BidirectionalIterator last, OutputIterator result);

  template<class ExecutionPolicy,
           class ForwardIterator>
    ForwardIterator rotate(ExecutionPolicy &&exec,
                           ForwardIterator first, ForwardIterator middle,
                           ForwardIterator last);
  template<class ExecutionPolicy,
           class ForwardIterator, class OutputIterator>
    OutputIterator
      rotate_copy(ExecutionPolicy &&exec,
                  ForwardIterator first, ForwardIterator middle,
                  ForwardIterator last, OutputIterator result);

  // partitions:
  template<class ExecutionPolicy,
           class InputIterator, class Predicate>
    bool is_partitioned(ExecutionPolicy &&exec,
                        InputIterator first, InputIterator last, Predicate pred);
  template<class ExecutionPolicy,
           class ForwardIterator, class Predicate>
    ForwardIterator
      partition(ExecutionPolicy &&exec,
                ForwardIterator first,
                ForwardIterator last, Predicate pred);
  template<class ExecutionPolicy,
           class BidirectionalIterator, class Predicate>
    BidirectionalIterator
      stable_partition(ExecutionPolicy &&exec,
                       BidirectionalIterator first,
                       BidirectionalIterator last, Predicate pred);
  template<class ExecutionPolicy,
           class InputIterator, class OutputIterator1,
           class OutputIterator2, class Predicate>
  pair<OutputIterator1, OutputIterator2>
  partition_copy(ExecutionPolicy &&exec,
                 InputIterator first, InputIterator last,
                 OutputIterator1 out_true, OutputIterator2 out_false,
                 Predicate pred);
  template<class ExecutionPolicy,
           class ForwardIterator, class Predicate>
    ForwardIterator partition_point(ExecutionPolicy &&exec,
                                    ForwardIterator first,
                                    ForwardIterator last,
                                    Predicate pred);

  // sorting and related operations:
  // sorting:
  template<class ExecutionPolicy,
           class RandomAccessIterator>
    void sort(ExecutionPolicy &&exec,
              RandomAccessIterator first, RandomAccessIterator last);
  template<class ExecutionPolicy,
           class RandomAccessIterator, class Compare>
    void sort(ExecutionPolicy &&exec,
              RandomAccessIterator first, RandomAccessIterator last, Compare comp);

  template<class ExecutionPolicy,
           class RandomAccessIterator>
    void stable_sort(ExecutionPolicy &&exec,
                     RandomAccessIterator first, RandomAccessIterator last);
  template<class ExecutionPolicy,
           class RandomAccessIterator, class Compare>
    void stable_sort(ExecutionPolicy &&exec,
                     RandomAccessIterator first, RandomAccessIterator last,
                     Compare comp);

  template<class ExecutionPolicy,
           class RandomAccessIterator>
    void partial_sort(ExecutionPolicy &&exec,
                      RandomAccessIterator first,
                      RandomAccessIterator middle,
                      RandomAccessIterator last);
  template<class ExecutionPolicy,
           class RandomAccessIterator, class Compare>
    void partial_sort(ExecutionPolicy &&exec,
                      RandomAccessIterator first,
                      RandomAccessIterator middle,
                      RandomAccessIterator last,
                      Compare comp);
  template<class ExecutionPolicy,
           class InputIterator, class RandomAccessIterator>
    RandomAccessIterator
      partial_sort_copy(ExecutionPolicy &&exec,
                        InputIterator first, InputIterator last,
                        RandomAccessIterator result_first,
                        RandomAccessIterator result_last);
  template<class ExecutionPolicy,
           class InputIterator, class RandomAccessIterator,
           class Compare>
    RandomAccessIterator
      partial_sort_copy(ExecutionPolicy &&exec,
                        InputIterator first, InputIterator last,
                        RandomAccessIterator result_first,
                        RandomAccessIterator result_last,
                        Compare comp);

  template<class ExecutionPolicy,
           class ForwardIterator>
    bool is_sorted(ExecutionPolicy &&exec,
                   ForwardIterator first, ForwardIterator last);
  template<class ExecutionPolicy,
           class ForwardIterator, class Compare>
    bool is_sorted(ExecutionPolicy &&exec,
                   ForwardIterator first, ForwardIterator last,
                   Compare comp);
  template<class ExecutionPolicy,
           class ForwardIterator>
    ForwardIterator is_sorted_until(ExecutionPolicy &&exec,
                                    ForwardIterator first, ForwardIterator last);
  template<class ExecutionPolicy,
           class ForwardIterator, class Compare>
    ForwardIterator is_sorted_until(ExecutionPolicy &&exec,
                                    ForwardIterator first, ForwardIterator last,
                                    Compare comp);

  template<class ExecutionPolicy,
           class RandomAccessIterator>
    void nth_element(ExecutionPolicy &&exec,
                     RandomAccessIterator first, RandomAccessIterator nth,
                     RandomAccessIterator last);
  template<class ExecutionPolicy,
           class RandomAccessIterator, class Compare>
    void nth_element(ExecutionPolicy &&exec,
                     RandomAccessIterator first, RandomAccessIterator nth,
                     RandomAccessIterator last, Compare comp);

  // merge:
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2,
           class OutputIterator>
    OutputIterator
      merge(ExecutionPolicy &&exec,
            InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2, InputIterator2 last2,
            OutputIterator result);
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2,
           class OutputIterator, class Compare>
    OutputIterator
      merge(ExecutionPolicy &&exec,
            InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2, InputIterator2 last2,
            OutputIterator result, Compare comp);

  template<class ExecutionPolicy,
           class BidirectionalIterator>
    void inplace_merge(ExecutionPolicy &&exec,
                       BidirectionalIterator first,
                       BidirectionalIterator middle,
                       BidirectionalIterator last);
  template<class ExecutionPolicy,
           class BidirectionalIterator,
           class Compare>
    void inplace_merge(ExecutionPolicy &&exec,
                       BidirectionalIterator first,
                       BidirectionalIterator middle,
                       BidirectionalIterator last, Compare comp);

  // set operations:
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2>
    bool includes(ExecutionPolicy &&exec,
                  InputIterator1 first1, InputIterator1 last1,
                  InputIterator2 first2, InputIterator2 last2);
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2, class Compare>
    bool includes(ExecutionPolicy &&exec,
                  InputIterator1 first1, InputIterator1 last1,
                  InputIterator2 first2, InputIterator2 last2,
                  Compare comp);

  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2,
           class OutputIterator>
    OutputIterator
      set_union(ExecutionPolicy &&exec,
                InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, InputIterator2 last2,
                OutputIterator result);
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2,
           class OutputIterator, class Compare>
    OutputIterator
      set_union(ExecutionPolicy &&exec,
                InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, InputIterator2 last2,
                OutputIterator result, Compare comp);

  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2,
           class OutputIterator>
    OutputIterator
      set_intersection(ExecutionPolicy &&exec,
                       InputIterator1 first1, InputIterator1 last1,
                       InputIterator2 first2, InputIterator2 last2,
                       OutputIterator result);
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2,
           class OutputIterator, class Compare>
    OutputIterator
      set_intersection(ExecutionPolicy &&exec,
                       InputIterator1 first1, InputIterator1 last1,
                       InputIterator2 first2, InputIterator2 last2,
                       OutputIterator result, Compare comp);

  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2,
           class OutputIterator>
    OutputIterator
      set_difference(ExecutionPolicy &&exec,
                     InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result);
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2,
           class OutputIterator, class Compare>
    OutputIterator
      set_difference(ExecutionPolicy &&exec,
                     InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result, Compare comp);

  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2,
           class OutputIterator>
    OutputIterator
      set_symmetric_difference(ExecutionPolicy &&exec,
                               InputIterator1 first1, InputIterator1 last1,
                               InputIterator2 first2, InputIterator2 last2,
                               OutputIterator result);
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2,
           class OutputIterator, class Compare>
    OutputIterator
      set_symmetric_difference(ExecutionPolicy &&exec,
                               InputIterator1 first1, InputIterator1 last1,
                               InputIterator2 first2, InputIterator2 last2,
                               OutputIterator result, Compare comp);

  // minimum and maximum:
  template<class ExecutionPolicy,
           class ForwardIterator>
    ForwardIterator min_element(ExecutionPolicy &&exec,
                                ForwardIterator first, ForwardIterator last);
                                Compare comp);
  template<class ExecutionPolicy,
           class ForwardIterator, class Compare>
    ForwardIterator min_element(ExecutionPolicy &&exec,
                                ForwardIterator first, ForwardIterator last,
                                Compare comp);
  template<class ExecutionPolicy,
           class ForwardIterator>
    ForwardIterator max_element(ExecutionPolicy &&exec,
                                ForwardIterator first, ForwardIterator last);
  template<class ExecutionPolicy,
           class ForwardIterator, class Compare>
    ForwardIterator max_element(ExecutionPolicy &&exec,
                                ForwardIterator first, ForwardIterator last,
  template<class ExecutionPolicy,
           class ForwardIterator>
    pair<ForwardIterator, ForwardIterator>
      minmax_element(ExecutionPolicy &&exec,
                     ForwardIterator first, ForwardIterator last);
  template<class ExecutionPolicy,
           class ForwardIterator, class Compare>
    pair<ForwardIterator, ForwardIterator>
      minmax_element(ExecutionPolicy &&exec,
                     ForwardIterator first, ForwardIterator last, Compare comp);
                                  Compare comp);

  
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2>
    bool
      lexicographical_compare(ExecutionPolicy &&exec,
                              InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2);
  
  template<class ExecutionPolicy,
           class InputIterator1, class InputIterator2, class Compare>
    bool
      lexicographical_compare(ExecutionPolicy &&exec,
                              InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              Compare comp);
} // namespace parallel
}
```

### All of `[alg.all__of]`

```
template<class ExecutionPolicy,
         class InputIterator, class Predicate>
  bool all_of(ExecutionPolicy &&exec,
              InputIterator first, InputIterator last, Predicate pred);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `true` if `[first,last)` is empty or `pred(*i)` is true for every iterator `i` in the range `[first,last)` and `false` otherwise.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Any of `[alg.any__of]`

```
template<class ExecutionPolicy,
         class InputIterator, class Predicate>
  bool any_of(ExecutionPolicy &&exec,
              InputIterator first, InputIterator last, Predicate pred);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `false` if `[first,last)` is empty or if there is no iterator `i` in the range `[first,last)` such that `pred(*i)` is `true`, and `true` otherwise.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### None of `[alg.none__of]`

```
template<class ExecutionPolicy,
         class InputIterator, class Predicate>
  bool none_of(ExecutionPolicy &&exec,
               InputIterator first, InputIterator last, Predicate pred);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `true` if `[first,last)`is empty or if `pred(*i)` is `false` for every iterator `i` in the range `[first,last)`, and `false` otherwise.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Find `[alg.find]`

```
template<class ExecutionPolicy,
         class InputIterator, class T>
  InputIterator find(ExecutionPolicy &&exec,
                     InputIterator first, InputIterator last,
                     const T& value);

template<class ExecutionPolicy,
         class InputIterator, class Predicate>
  InputIterator find_if(ExecutionPolicy &&exec,
                        InputIterator first, InputIterator last,
                        Predicate pred);

template<class ExecutionPolicy,
         class InputIterator, class Predicate>
  InputIterator find_if_not(ExecutionPolicy &&exec,
                            InputIterator first, InputIterator last,
                            Predicate pred);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The first iterator `i` in the range `[first,last)` for which the following corresponding expression holds: `*i == value`, `pred(*i) != false`, `pred(*i) == false`.
   Returns `last` if no such iterator is found.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Find end `[alg.find.end]`

```
template<class ExecutionPolicy,
         class ForwardIterator1, class ForwardIterator2>
  ForwardIterator1
    find_end(ExecutionPolicy &exec,
             ForwardIterator1 first1, ForwardIterator1 last1,
             ForwardIterator2 first2, ForwardIterator2 last2);

template<class ExecutionPolicy,
         class ForwardIterator1, class ForwardIterator2,
         class BinaryPredicate>
  ForwardIterator1
    find_end(ExecutionPolicy &&exec,
             ForwardIterator1 first1, ForwardIterator1 last1,
             ForwardIterator2 first2, ForwardIterator2 last2,
             BinaryPredicate pred);
```

1. *Effects: The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The last iterator `i` in the range `[first1,last1 - (last2 - first2))` such that for any
    non-negative integer `n < (last2 - first2)`, the following corresponding conditions hold:
    `*(i + n) == *(first2 + n)`, `pred(*(i + n), *(first2 + n)) != false`. Returns `last1` if `[first2,last2)`
    is empty or if no such iterator is found.

3. *Requires:* Neither `operator==` nor `pred` shall invalidate iterators or subranges, nor modify elements in the ranges `[first1,last1)1 or `[first2,last2)`.

4. *Complexity:* `O(m * n)`, where `m == last2 - first1` and `n = last1 - first1 - (last2 - first2)`.

5. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.


### Find first `[alg.find.first.of]`

```
template<class ExecutionPolicy,
         class InputIterator, class ForwardIterator>
  InputIterator
    find_first_of(ExecutionPolicy &&exec,
                  InputIterator first1, InputIterator last1,
                  ForwardIterator first2, ForwardIterator last2);

template<class ExecutionPolicy,
         class InputIterator, class ForwardIterator,
         class BinaryPredicate>
  InputIterator
    find_first_of(ExecutionPolicy &&exec,
                  InputIterator first1, InputIterator last1,
                  ForwardIterator first2, ForwardIterator last2,
                  BinaryPredicate pred);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The first iterator `i` in the range `[first1,last1)` such that for some iterator `j` in the range
    `[first2,last2)` the following conditions hold: `*i == *j`, `pred(*i,*j) != false`. Returns `last1`
    if `[first2,last2)` is empty or if no such iterator is found.

3. *Requires:* Neither `operator==` nor `pred` shall invalidate iterators or subranges, nor modify elements in the ranges `[first1,last1)` or `[first2,last2)`.

4. *Complexity*: `O(m * n)`, where `m == last1 - first1` and `n == last2 - first2`.

5. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.


### Adjacent find `[alg.adjacent.find]`

```
template<class ExecutionPolicy,
         class ForwardIterator>
  ForwardIterator adjacent_find(ExecutionPolicy &&exec, ForwardIterator first, ForwardIterator last);

template<class ExecutionPolicy,
         class ForwardIterator, class BinaryPredicate>
  ForwardIterator adjacent_find(ExecutionPolicy &&exec, ForwardIterator first, ForwardIterator last,
                                BinaryPredicate pred);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The first iterator `i` such that both `i` and `i + 1` are in the range `[first,last)` for which the
    following corresponding conditions hold: `*i == *(i + 1)`, `pred(*i, *(i + 1)) != false`. Returns `last`
    if no such iterator is found.

3. *Requires:* Neither `operator==` nor `pred` shall invalidate iterators or subranges, nor modify elements in the range `[first,last)`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.


### Count `[alg.count]`

```
template<class ExecutionPolicy,
         class InputIterator, class EqualityComparable>
  typename iterator_traits<InputIterator>::difference_type
    count(ExecutionPolicy &&exec,
          InputIterator first, InputIterator last, const EqualityComparable &value);

template<class ExecutionPolicy,
         class InputIterator, class Predicate>
  typename iterator_traits<InputIterator>::difference_type
    count_if(ExecutionPolicy &&exec,
             InputIterator first, InputIterator last, Predicate pred);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The number of iterators `i` in the range `[first,last)` for which the following corresponding conditions hold: `*i == value`, `pred(*i) != false`.

3. *Complexity:* `O(last - first)`.

5. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Mismatch `[alg.mismatch]`

```
template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2>
  pair<InputIterator1,InputIterator2>
    mismatch(ExecutionPolicy &&exec,
             InputIterator1 first1, InputIterator1 last1, 
             InputIterator2 first2);

template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2, class BinaryPredicate>
  pair<InputIterator1,InputIterator2>
    mismatch(ExecutionPolicy &&exec,
             InputIterator1 first1, InputIterator1 last1,
             InputIterator2 first2, BinaryPredicate pred);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* A pair of iterators `i` and `j` such that `j == first2 + (i - first)` and `i` is the first iterator in the range `[first1,last1)` for which the following corresponding conditions hold:

    ```
     !(i == *(first2 + (i - first1)))
     pred(*i, *(first2 + (i - first1))) == false
    ```

   Returns the pair `last1` and `first2 + (last1 - first1)` if such an iterator `i` is not found.

3. *Complexity:* `O(last1 - first1)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Equal `[alg.equal]`

```
template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2>
  bool equal(ExecutionPolicy &&exec,
             InputIterator1 first1, InputIterator1 last1,
             InputIterator2 first2);

template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2, class BinaryPredicate>
  bool equal(ExecutionPolicy &&exec,
             InputIterator1 first1, InputIterator1 last1,
             InputIterator2 first2, BinaryPredicate pred);
```

1. *Effects*: The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `true` if for every iterator `i` in the range `[first1,last1)` the following corresponding conditions hold: `*i == *(first2 + (i - first1))`, `pred(*i, *(first2 + (i - first1))) != false`.
   Otherwise, returns `false`.

3. *Complexity:* `O(last1 - first1)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Search `[alg.search]`

```
template<class ExecutionPolicy,
         class ForwardIterator1, class ForwardIterator2>
  ForwardIterator1 search(ExecutionPolicy &&exec,
                          ForwardIterator1 first1, ForwardIterator1 last1,
                          ForwardIterator2 first2, ForwardIterator2 last2);

template<class ExecutionPolicy,
         class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
  ForwardIterator1 search(ExecutionPolicy &&exec,
                          ForwardIterator1 first1, ForwardIterator1 last1,
                          ForwardIterator2 first2, ForwardIterator2 last2,
                          BinaryPredicate pred);
```

1. *Effects*: Finds a subsequence of equal values in a sequence.

    The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The first iterator `i` in the range `[first1,last1 - (last2-first2)` such that for any non-negative integer `n` less than `last2 - first2` the following corresponding conditions hold:
    `*(i + n) == *(first2 + n), pred(*(i + n), *(first2 + n)) != false`. Returns `first1` if `[first2,last2)` is empty, otherwise returns `last1` if no such iterator is found.

3. *Complexity:* At most `(last1 - first1) * (last2 - first2)` applications of the corresponding predicate.


```
template<class ExecutionPolicy,
         class ForwardIterator, class Size, class T>
  ForwardIterator search_n(ExecutionPolicy &&exec,
                           ForwardIterator first, ForwardIterator last, Size count,
                           const T& value);

template<class ExecutionPolicy,
         class ForwardIterator, class Size, class T,
         class BinaryPredicate>
  ForwardIterator search_n(ExecutionPolicy &&exec,
                           ForwardIterator first, ForwardIterator last, Size count,
                           const T& value, BinaryPredicate pred);
```

1. *Requires:* The type `Size` shall be convertible to integral type.

2. *Effects:* Finds a subsequence of equal values in a sequence.

    The algorithm's execution is parallelized as determined by `exec`.

3. *Returns:* The first iterator `i` in the range `[first,last-count)` such that for any non-negative integer `n` less than `count` the following corresponding conditions hold:
    `*(i + n) == value, pred(*(i + n),value) != false`. Returns `last` if no such iterator is found.

4. *Complexity:* At most `last - first` applications of the corresponding predicate.


### Copy `[alg.copy]`

```
template<class ExecutionPolicy,
         class InputIterator, class OutputIterator>
  OutputIterator copy(ExecutionPolicy &&exec,
                      InputIterator first, InputIterator last,
                      OutputIterator result);
```

1. *Effects:* For each iterator `i` in the range `[first,last)`, performs `*(result + (i - first) = *i`.
    The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `result + (last - first)`.

3. *Requires:* `result` shall not be in the range `[first,last)`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class InputIterator, class Size, class OutputIterator>
  OutputIterator copy_n(ExecutionPolicy &&exec,
                        InputIterator first, Size n,
                        OutputIterator result);
```

1. *Effects:* For each non-negative integer `i < n`, performs `*(result + i) = *(first + i)`. The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `result + n`.

3. *Complexity:* `O(n)`.

4. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class InputIterator, class OutputIterator, class Predicate>
  OutputIterator
    copy_if(ExecutionPolicy &&exec,
            InputIterator first, InputIterator last,
            OutputIterator result, Predicate pred);
```

1. *Requires:* The ranges `[first,last)` and `[result,result + (last - first))` shall not overlap.

2. *Effects:* Copies all of the elements referred to by the iterator `i` in the range `[first,last)` for which `pred(*i)` is `true`. The algorithm's execution is parallelized as determined by `exec`.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* Stable.

    The signature shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.


### Move `[alg.move]`

```
template<class ExecutionPolicy,
         class InputIterator, class OutputIterator>
  OutputIterator
    move(ExecutionPolicy &&exec,
         InputIterator first, InputIterator last,
         OutputIterator result);
```

1. *Effects:* For each iterator `i` in the range `[first,last)`, performs `*(result + (i - first) = std::move(*i)`.
    The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `result - (last - first)`.

3. *Requires:* `result` shall not be in the range `[first,last)`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Swap `[alg.swap]`

```
template<class ExecutionPolicy,
         class ForwardIterator1, class ForwardIterator2>
  ForwardIterator2
    swap_ranges(ExecutionPolicy &&exec,
                ForwardIterator1 first1, ForwardIterator1 last1,
                ForwardIterator1 first2);
```

1. *Effects:* For each non-negative integer `n < (last1 - first1)` performs: `swap(*(first1 + n), *(first2 + n))`.
   The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* The two ranges `[first1,last1)` and `[first2,first2 + (last1 - first1))` shall not overlap.
   `*(first1 + n)` shall be swappable with `*(first2 + n)`.

3. *Returns:* `first2 + (last1 - first1)`.

4. *Complexity:* `O(last1 - first1)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Transform `[alg.transform]`

```
template<class ExecutionPolicy,
         class InputIterator, class OutputIterator,
         class UnaryOperation>
  OutputIterator transform(ExecutionPolicy &&exec,
                           InputIterator first, InputIterator last,
                           OutputIterator result, UnaryOperation op);

template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2, class OutputIterator,
         class BinaryOperation>
  OutputIterator
    transform(ExecutionPolicy &&exec,
              InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, OutputIterator result,
              BinaryOperation binary_op);
```

1. *Effects:* Assigns through every iterator `i` in the range `[result,result + (last1 - first1))` a new
   corresponding value equal to `op(*(first1 + (i - result))` or `binary_op(*(first1 + (i - result)), *(first2 + (i - result))`.
   The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* `op` and `binary_op` shall not invalidate iterators or subranges, or modify elements in the ranges
   `[first1,last1]`, `[first2,first2 + (last1 - first1)]`, and `[result,result + (last1 - first1)]`.

3. *Returns:* `result + (last1 - first1)`.

4. *Complexity:* `O(last - first)` or `O(last1 - first1)`.

5. *Remarks:* `result` may be equal to `first` in case of unary transform, or to `first1` or `first2` in case of binary transform.

    The signatures shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Replace `[alg.replace]`

```
template<class ExecutionPolicy,
         class ForwardIterator, class T>
  void replace(ExecutionPolicy &&exec,
               ForwardIterator first, ForwardIterator last,
               const T& old_value, const T& new_value);

template<class ExecutionPolicy,
         class ForwardIterator, class Predicate, class T>
  void replace_if(ExecutionPolicy &&exec,
                  ForwardIterator first, ForwardIterator last,
                  Predicate pred, const T& new_value);
```

1. *Requires:* The expression `*first = new_value` shall be valid.

2. *Effects:* Substitutes elements referred by the iterator `i` in the range `[first,last)` with `new_value`, when the following corresponding conditions hold: `*i == old_value`, `pred(*i) != false`.
   The algorithm's execution is parallelized as determined by `exec`.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class InputIterator, class OutputIterator, class T>
  OutputIterator
    replace_copy(ExecutionPolicy &&exec,
                 ForwardIterator first, ForwardIterator last,
                 OutputIterator result,
                 const T& old_value, const T& new_value);

template<class ExecutionPolicy,
         class InputIterator, class OutputIterator, class Predicate, class T>
  OutputIterator
    replace_copy_if(ExecutionPolicy &&exec,
                    InputIterator first, InputIterator last,
                    OutputIterator result,
                    Predicate pred, const T& new_value);
```

1. *Requires:* The results of the expressions `*first` and `new_value` shall be writable to the `result` output iterator. The ranges `[first,last)` and `[result,result + (last - first))` shall not overlap.

2. *Effects:* Assigns to every iterator `i` in the range `[result,result + (last - first))` either `new_value` or `*(first + (i - result))` depending on whether the following corresponding conditions hold:

    ```
    *(first + (i - result)) == old_value
    pred(*(first + (i - result))) != false
    ```

    The algorithm's execution is parallelized as determined by `exec`.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Fill `[alg.fill]`

```
template<class ExecutionPolicy,
         class ForwardIterator, class T>
  void fill(ExecutionPolicy &&exec,
            ForwardIterator first, ForwardIterator last, const T& value);

template<class ExecutionPolicy,
         class OutputIterator, class Size, class T>
  void fill_n(ExecutionPolicy &&exec,
              OutputIterator first, Size n, const T& value);
```

1. *Requires:* The expression `value` shall be writable to the output iterator. The type `Size` shall be convertible to an integral type.

2. *Effects:* The first algorithm assigns `value` through all the iterators in the range `[first,last)`. The second value assigns `value`
   through all the iterators in the range `[first,first + n)` if `n` is positive, otherwise it does nothing.
   The algorithm is parallelized as determined by `exec`.

3. *Returns:* `fill_n` returns `first + n` for non-negative values of `n` and `first` for negative values.

4. *Complexity:* `O(last - first)` or `O(n)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Generate `[alg.generate]`

```
template<class ExecutionPolicy,
         class ForwardIterator, class Generator>
  void generate(ExecutionPolicy &&exec,
                ForwardIterator first, ForwardIterator last, Generator gen);

template<class ExecutionPolicy,
         class OutputIterator, class Size, class Generator>
  OutputIterator generate_n(ExecutionPolicy &&exec,
                            OutputIterator first, Size n, Generator gen);
```

1. *Effects:* The first algorithm invokes the function object `gen` and assigns the value of `gen` through all the iterators in the range `[first,last)`.
    The second algorithm invokes the function object `gen` and assigns the return value of `gen` through all the iterators in the range `[first,first + n)` if `n` is
    positive, otherwise it does nothing. The algorithms execution is parallelized as determined by `exec`.

2. *Requires:* `gen` takes no arguments, `Size` shall be convertible to an integral type.

3. *Returns:* `generate_n` returns `first + n` for non-negative values of `n`and `first` for negative values.

4. *Complexity:* `O(last - first)` or `O(n)`.

5. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Remove `[alg.remove]`

```
template<class ExecutionPolicy,
         class ForwardIterator, class T>
  ForwardIterator remove(ExecutionPolicy &&exec,
                         ForwardIterator first, ForwardIterator last, const T& value);

template<class ExecutionPolicy,
         class ForwardIterator, class Predicate>
  ForwardIterator remove_if(ExecutionPolicy &&exec,
                            ForwardIterator first, ForwardIterator last, Predicate pred);
```

1. *Requires:* The type of `*first` shall satisfy the `MoveAssignable` requirements.

2. *Effects:* Eliminates all the elements referred to by iterator `i` in the range `[first,last)` for which the following corresponding conditions hold: `*i == value`, `pred(*i) != false`.
   The algorithm's execution is parallelized as determined by `exec`.

3. *Returns:* The end of the resulting range.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* Stable.

    The signatures shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.

6. *Note:* Each element in the range `[ret,last)`, where `ret` is the returned value, has a valid but unspecified state, because the algorithms can eliminate elements by swapping with or moving
    from elements that were originally in that range.

```
template<class ExecutionPolicy,
         class InputIterator, class OutputIterator, class T>
  OutputIterator
    remove_copy(ExecutionPolicy &&exec,
                InputIterator first, InputIterator last,
                OutputIterator result, const T& value);

template<class ExecutionPolicy,
         class InputIterator, class OutputIterator, class Predicate>
  OutputIterator
    remove_copy_if(ExecutionPolicy &&exec,
                   InputIterator first, InputIterator last,
                   OutputIterator result, Predicate pred);
```

1. *Requires:* The ranges `[first,last)` and `[result,result + (last - first))` shall not overlap. The expression `*result = *first` shall be valid.

2. *Effects:* Copies all the elements referred to by the iterator `i` in the range `[first,last)` for which the following corresponding conditions do not hold: `*i == value`, `pred(*i) != false`.
   The algorithm's execution is parallelized as determined by `exec`.

3. *Returns:* The end of the resulting range.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* Stable.

    The signatures shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.


### Unique `[alg.unique]`

```
template<class ExecutionPolicy,
         class ForwardIterator>
  ForwardIterator unique(ExecutionPolicy &&exec,
                         ForwardIterator first, ForwardIterator last);

template<class ExecutionPolicy,
         class ForwardIterator, typename BinaryPredicate>
  ForwardIterator unique(ExecutionPolicy &&exec,
                         ForwardIterator first, ForwardIterator last
                         BinaryPredicate pred);
```

1. *Effects:* For a nonempty range, eliminates all but the first element from every consecutive group of equivalent elements referred to by the iterator `i` in the range `[first + 1,last)` for which
   the following conditions hold: `*(i - 1) == *i` or `pred(*(i - 1), *i) != false`. The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* The comparison function shall be an equivalence relation. The type of `*first` shall satisfy the `MoveAssignable` requirements.

3. *Returns:* The end of the resulting range.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class InputIterator, class OutputIterator>
  OutputIterator
    unique_copy(ExecutionPolicy &&exec,
                InputIterator first, InputIterator last,
                OutputIterator result);

template<class ExecutionPolicy,
         class InputIterator, class OutputIterator, class BinaryPredicate>
  OutputIterator
    unique_copy(ExecutionPolicy &&exec,
                InputIterator first, InputIterator last,
                OutputIterator result, BinaryPredicate pred);
```

1. *Requires:* The comparison function shall be an equivalence relation. The ranges `[first,last)` and `[result,result + (last-first))` shall not overlap. The expression `*result = *first` shall be valid.
   If neither `InputIterator` nor `OutputIterator` meets the requirements of forward iterator then the value type of `InputIterator` shall be `CopyConstructible` and `CopyAssignable`. Otherwise `CopyConstructible` is not required.

2. *Effects:* Copies only the first element from every consecutive group of equal elements referred to by the iterator `i` in the range `[first,last)` for which the following corresponding conditions hold:
   `*i == *(i - 1)` or `pred(*i, *(i - 1)) != false`. The algorithm's execution is parallelized as determined by `exec`.

3. *Returns:* The end of the resulting range.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Reverse `[alg.reverse]`

```
template<class ExecutionPolicy,
         class BidirectionalIterator>
  void reverse(ExecutionPolicy &&exec,
               BidirectionalIterator first, BidirectionalIterator last);
```

1. *Effects:* For each non-negative integer `i <= (last - first)/2`, applies `iter_swap` to all pairs of iterator `first + i`, `(last - i) - 1`.
   The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* `*first` shall be swappable.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.


```
template<class ExecutionPolicy,
         class BidirectionalIterator, class OutputIterator>
  OutputIterator
    reverse_copy(ExecutionPolicy &&exec,
                 BidirectionalIterator first,
                 BidirectionalIterator last, OutputIterator result);
```

1. *Effects:* Copies the range `[first,last)` to the range `[result,result + (last - first))` such that for any non-negative integer `i < (last - first)` the following assignment takes place:
   `*(result + (last - first) - i) = *(first + i)`. The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* The ranges `[first,last)` and `[result,result + (last - first))` shall not overlap.

3. *Returns*: `result + (last - first)`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.


### Rotate `[alg.rotate]`

```
template<class ExecutionPolicy,
         class ForwardIterator>
  ForwardIterator rotate(ExecutionPolicy &&exec,
                         ForwardIterator first, ForwardIterator middle,
                         ForwardIterator last);
```

1. *Effects:* For each non-negative integer `i < (last - first)`, places the element from the position
    `first + i` into position `first + (i + (last - middle)) % (last - first)`.
    The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `first + (last - middle)`.

3. *Remarks:* This is a left rotate.

    The signature shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.

4. *Requires:* `[first,middle)` and `[middle,last)` shall be valid ranges. `ForwardIterator` shall satisfy the
    requirements of `ValueSwappable`. The type of `*first` shall satisfy the requirements of `MoveConstructible`
    and the requirements of `MoveAssignable`.

5. *Complexity:* `O(last - first)`.

```
template<class ExecutionPolicy,
         class ForwardIterator, class OutputIterator>
  OutputIterator
    rotate_copy(ExecutionPolicy &&exec,
                ForwardIterator first, ForwardIterator middle,
                ForwardIterator last, OutputIterator result);
```

1. *Effects:* Copies the range `[first,last)` to the range `[result,result + (last - first))` such that
    for each non-negative integer `i < (last - first)` the following assignment takes place:
    `*(result + i) = *(first + (i + (middle - first)) % (last - first)).
    The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `result + (last - first)`.

3. *Requires:* The ranges `[first,last)` and `[result,result + (last - first))` shall not overlap.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Partitions `[alg.partitions]`

```
template<class ExecutionPolicy,
         class InputIterator, class Predicate>
  bool is_partitioned(ExecutionPolicy &&exec,
                      InputIterator first, InputIterator last, Predicate pred);
```

1. *Requires:* `InputIterator`'s value type shall be convertible to `Predicate`'s argument type.

2. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

3. *Returns:* `true` if `[first,last)` is empty or if `[first,last)` is partitioned by `pred`, i.e. if all elements that satisfy `pred` appear before those that do not.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class ForwardIterator, class Predicate>
  ForwardIterator
    partition(ExecutionPolicy &&exec,
              ForwardIterator first,
              ForwardIterator last, Predicate pred);
```

1. *Effects:* Places all the elements in the range `[first,last)` that satisfy `pred` before all the elements that do not satisfy it.
   The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* An iterator `i` such that for any iterator `j` in the range `[first,i)`, `pred(*j) != false`, and for any iterator `k` in the range `[i,last)`, `pred(*k) == false`.

3. *Requires:* `ForwardIterator` shall satisfy the requirements of `ValueSwappable`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class BidirectionalIterator, class Predicate>
  BidirectionalIterator
    stable_partition(ExecutionPolicy &&exec,
                     BidirectionalIterator first,
                     BidirectionalIterator last, Predicate pred);
```

1. *Effects:* Places all the elements in the range `[first,last)` that satisfy `pred` before all the elements that do not satisfy it.
   The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* An iterator `i` such that for any iterator `j` in the range `[first,i)`, `pred(*j) != false`, and for any iterator `k` in the range `[i,last)`, `pred(*k) == false`.
   The relative order of the elements in both groups is preserved.

3. *Requires:* `BidirectionalIterator` shall satisfy the requirements of `ValueSwappable`. The type of `*first` shall satisfy the requirements of `MoveConstructible` and of
   `MoveAssignable`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class InputIterator, class OutputIterator1,
         class OutputIterator2, class Predicate>
pair<OutputIterator1, OutputIterator2>
partition_copy(ExecutionPolicy &&exec,
               InputIterator first, InputIterator last,
               OutputIterator1 out_true, OutputIterator2 out_false,
               Predicate pred);
```

1. *Requires:* `InputIterator`'s value type shall be `Assignable`, and shall be writable to the `out_true` and `out_false` `OutputIterator`s, and shall be convertible to `Predicate`'s
    argument type. The input range shall not overlap with either of the output ranges.

2. *Effects:* For each iterator `i` in `[first,last)`, copies `*i` to the output range beginning with `out_true` if `pred(*i)` is `true`, or to the output range beginning with `out_false` otherwise.
   The algorithm's execution is parallelized as determined by `exec`.

3. *Returns:* A pair `p` such that `p.first` is the end of the output range beginning at `out_true` and `p.second` is the end of the output range beginning at `out_false`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class ForwardIterator, class Predicate>
  ForwardIterator partition_point(ExecutionPolicy &&exec,
                                  ForwardIterator first,
                                  ForwardIterator last,
                                  Predicate pred);
```

1. *Requires:* `ForwardIterator`'s value type shall be convertible to `Predicate`'s argument type. `[first,last)` shall be partitioned by `pred`, i.e. all elements that satisfy `pred` shall appear
   before those that do not.

2. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

3. *Returns:* An iterator `mid` such that `all_of(first, mid, pred)` and `none_of(mid, last, pred)` are both `true`.

4. *Complexity:* `O(last - first)`.

5. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### `sort` `[alg.sort]`

```
template<class ExecutionPolicy,
         class RandomAccessIterator>
  void sort(ExecutionPolicy &&exec,
            RandomAccessIterator first, RandomAccessIterator last);

template<class ExecutionPolicy,
         class RandomAccessIterator, class Compare>
  void sort(ExecutionPolicy &&exec,
            RandomAccessIterator first, RandomAccessIterator last, Compare comp);
```

1. *Effects:* Sorts the elements in the range `[first,last)`. The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* `RandomAccessIterator` shall satisfy the requirements of `ValueSwappable`. The type of `*first` shall satisfy the requirements of `MoveConstructible` and of `MoveAssignable`.

3. *Complexity:* `O(n lg n)`, where `n = last - first`.

4. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### `stable_sort` `[stable.sort]`

```
template<class ExecutionPolicy,
         class RandomAccessIterator>
  void stable_sort(ExecutionPolicy &&exec,
                   RandomAccessIterator first, RandomAccessIterator last);

template<class ExecutionPolicy,
         class RandomAccessIterator, class Compare>
  void stable_sort(ExecutionPolicy &&exec,
                   RandomAccessIterator first, RandomAccessIterator last,
                   Compare comp);
```

1. *Effects:* Sorts the elements in the range `[first,last)`. The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* `RandomAccessIterator` shall satisfy the requirements of `ValueSwappable`. The type of `*first` shall satisfy the requirements of `MoveConstructible` and of `MoveAssignable`.

3. *Complexity:* `O(n lg n)`, where `n = last - first`.

4. *Remarks:* Stable.

    The signatures shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.

### `partial_sort` `[partial.sort]`

```
template<class ExecutionPolicy,
         class RandomAccessIterator>
  void partial_sort(ExecutionPolicy &&exec,
                    RandomAccessIterator first,
                    RandomAccessIterator middle,
                    RandomAccessIterator last);

template<class ExecutionPolicy,
         class RandomAccessIterator, class Compare>
  void partial_sort(ExecutionPolicy &&exec,
                    RandomAccessIterator first,
                    RandomAccessIterator middle,
                    RandomAccessIterator last,
                    Compare comp);
```

1. *Effects:* Places the first `middle - first` sorted elements from the range `[first,last)` into the range
    `[first,middle)`. The rest of the elements in the range `[middle,last)` are placed in an unspecified
    order.

    The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* `RandomAccessIterator` shall satisfy the requirements of `ValueSwappable`. The type of `*first` shall satisfy the requirements of `MoveConstructible` and of `MoveAssignable`.

    `middle` shall be in the range `[first,last)`.

3. *Complexity:* `O(m lg n)`, where `m = last - first` and `n = middle - first`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### `partial_sort_copy` `[partial.sort.copy]`

```
template<class ExecutionPolicy,
         class InputIterator, class RandomAccessIterator>
  RandomAccessIterator
    partial_sort_copy(ExecutionPolicy &&exec,
                      InputIterator first, InputIterator last,
                      RandomAccessIterator result_first,
                      RandomAccessIterator result_last);

template<class ExecutionPolicy,
         class InputIterator, class RandomAccessIterator,
         class Compare>
  RandomAccessIterator
    partial_sort_copy(ExecutionPolicy &&exec,
                      InputIterator first, InputIterator last,
                      RandomAccessIterator result_first,
                      RandomAccessIterator result_last,
                      Compare comp);
```

1. *Effects:* Places the first `min(last - first, result_last - result_first)` sorted elements into the range
    `[result_first,result_first + min(last - first, result_last - result_first))`.

    The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The smaller of: `result_last` or `result_first + (last - first)`.

3. *Requires:* `RandomAccessIterator` shall satisfy the requirements of `ValueSwappable`. The type of `*result_first` shall satisfy the requirements of `MoveConstructible` and of `MoveAssignable`.

4. *Complexity:* `O(m lg n)`, where `m = last - first` and `n = min(last - first, result_last - result_first)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.


### `is_sorted` `[is.sorted]`

```
template<class ExecutionPolicy,
         class ForwardIterator>
  bool is_sorted(ExecutionPolicy &&exec,
                 ForwardIterator first, ForwardIterator last);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `is_sorted_until(forward<ExecutionPolicy>(exec), first, last) == last`

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class ForwardIterator, class Compare>
  bool is_sorted(ExecutionPolicy &&exec,
                 ForwardIterator first, ForwardIterator last,
                 Compare comp);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `is_sorted_until(forward<ExecutionPolicy>(exec), first, last, comp) == last`

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signature shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class ForwardIterator>
  ForwardIterator is_sorted_until(ExecutionPolicy &&exec,
                                  ForwardIterator first, ForwardIterator last);

template<class ExecutionPolicy,
         class ForwardIterator, class Compare>
  ForwardIterator is_sorted_until(ExecutionPolicy &&exec,
                                  ForwardIterator first, ForwardIterator last,
                                  Compare comp);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* If `distance(first, last) < 2)`, returns `last`. Otherwise, returns the last iterator `i` in `[first,last)` for which the range `[first,i)` is sorted.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Nth element `[alg.nth.element]`

```
template<class ExecutionPolicy,
         class RandomAccessIterator>
  void nth_element(ExecutionPolicy &&exec,
                   RandomAccessIterator first, RandomAccessIterator nth,
                   RandomAccessIterator last);

template<class ExecutionPolicy,
         class RandomAccessIterator, class Compare>
  void nth_element(ExecutionPolicy &&exec,
                   RandomAccessIterator first, RandomAccessIterator nth,
                   RandomAccessIterator last, Compare comp);
```

1. *Effects:* Reorders the range `[first,last)` such that the element referenced by `nth` is the element that
    would be in that position if the whole range were sorted. Also for any iterator `i` in the range `[first,nth)`
    and any iterator `j` in the range `[nth,last)` the following corresponding condition holds: `!(*j < *i)` or `comp(*j, *i) == false`.

    The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* `RandomAccessIterator` shall satisfy the requirements of `ValueSwappable`. The type of `*first` shall satisfy the
    requirements of `MoveConstructible` and of `MoveAssignable`.

    `nth` shall be in the range `[first,last)`.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Merge `[alg.merge]`

```
template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2,
         class OutputIterator>
  OutputIterator
    merge(ExecutionPolicy &&exec,
          InputIterator1 first1, InputIterator1 last1,
          InputIterator2 first2, InputIterator2 last2,
          OutputIterator result);

template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2,
         class OutputIterator, class Compare>
  OutputIterator
    merge(ExecutionPolicy &&exec,
          InputIterator1 first1, InputIterator1 last1,
          InputIterator2 first2, InputIterator2 last2,
          OutputIterator result, Compare comp);
```

1. *Effects:* Copies all the elements of the two ranges `[first1,last1)` and `[first2,last2)` into the range `[result,result_last)`,
   where `result_last` is `result + (last1 - first1) + (last2 - first2)`, such that the resulting range satisfies
   `is_sorted(result, result_last)` or `is_sorted(result, result_last, comp)`, respectively.
   The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* The ranges `[first1,last1)` and `[first2,last2)` shall be sorted with respect to `operator<` or `comp`. The resulting
   range shall not overlap with either of the input ranges.

3. *Returns:* `result + (last1 - first1) + (last2 - first2)`.

4. *Complexity:* `O(m + n)`, where `m = last1 - first1` and `n = last2 - first2`.

5. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class BidirectionalIterator>
  void inplace_merge(ExecutionPolicy &&exec,
                     BidirectionalIterator first,
                     BidirectionalIterator middle,
                     BidirectionalIterator last);

template<class ExecutionPolicy,
         class BidirectionalIterator,
         class Compare>
  void inplace_merge(ExecutionPolicy &&exec,
                     BidirectionalIterator first,
                     BidirectionalIterator middle,
                     BidirectionalIterator last, Compare comp);
```

1. *Effects:* Merges two sorted consecutive ranges `[first,middle)` and `[middle,last)`, putting the result
    of the merge into the range `[first,last)`. The resulting range will be in non-decreasing order; that is,
    for every iterator `i` in `[first,last)` other than `first`, the condition `*i < *(i - 1)` or, respectively,
    `comp(*i, *(i - 1))` will be `false`.
    The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* The ranges `[first,middle)` and `[middle,last)` shall be sorted with respect to `operator<` or `comp`.
    `BidirectionalIterator` shall satisfy the requirements of `ValueSwappable`. The type of `*first` shall satisfy
    the requirementsof `MoveConstructible`and of `MoveAssignable`.

3. *Remarks:* Stable.

4. *Complexity:* `O(m + n`)`, where `m = middle - first` and `n = last - middle`.

5. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Includes `[includes]`

```
template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2>
  bool includes(ExecutionPolicy &&exec,
                InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, InputIterator2 last2);

template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2, class Compare>
  bool includes(ExecutionPolicy &&exec,
                InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, InputIterator2 last2,
                Compare comp);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* The ranges `[first1,last1)` and `[first2,last2)` shall be sorted with respect to `operator<` or `comp`.

3. *Returns:* `true` if `[first2,last2)` is empty or if every element in the range `[first2,last2)` is contained in the range `[first1,last1)`. Returns `false` otherwise.

4. *Complexity:* `O(m + n)`, where `m = last1 - first1` and `n = last2 - first2`.

5. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### `set_union` `[set.union]`

```
template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2,
         class OutputIterator>
  OutputIterator
    set_union(ExecutionPolicy &&exec,
              InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2,
              OutputIterator result);

template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2,
         class OutputIterator, class Compare>
  OutputIterator
    set_union(ExecutionPolicy &&exec,
              InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2,
              OutputIterator result, Compare comp);
```

1. *Effects:* Constructs a sorted union of the elements from the two ranges; that is, the set of elements that are present in one or both of the ranges.
   The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* The resulting range shall not overlap with either of the original ranges.

3. *Returns:* The end of the constructed range.

4. *Complexity:* `O(m + n)`, where `m = last1 - first1` and `n = last2 - first2`.

5. *Remarks:* If `[first1,last1)` contains $m$ elements that are equivalent to each other and `[first2,last2)` contains $n$ elements that are equivalent to them,
   then all $m$ elements from the first range shall be copied to the output range, in order, and then $max(n - m,0)$ elements from the second range shall be
   copied to the output range, in order.

    The signatures shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.

### `set_intersection` `[set.intersection]`

```
template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2,
         class OutputIterator>
  OutputIterator
    set_intersection(ExecutionPolicy &&exec,
                     InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result);

template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2,
         class OutputIterator, class Compare>
  OutputIterator
    set_intersection(ExecutionPolicy &&exec,
                     InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result, Compare comp);
```

1. *Effects:* Constructs a sorted intersection of the elements from the two ranges; that is, the set of elements that are present in both of the ranges.
   The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* The resulting range shall not overlap with either of the original ranges.

3. *Returns:* The end of the constructed range.

4. *Complexity:* `O(m + n)`, where `m = last1 - first1` and `n = last2 - first2`.

5. *Remarks:* If `[first1,last1)` contains $m$ elements that are equivalent to each other and `[first2,last2)` contains $n$ elements that are equivalent to them,
   the first $min(m,n)$ elements shall be copied from the first range to the output range, in order.

    The signatures shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.


### `set_difference` `[set.difference]`

```
template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2,
         class OutputIterator>
  OutputIterator
    set_difference(ExecutionPolicy &&exec,
                   InputIterator1 first1, InputIterator1 last1,
                   InputIterator2 first2, InputIterator2 last2,
                   OutputIterator result);

template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2,
         class OutputIterator, class Compare>
  OutputIterator
    set_difference(ExecutionPolicy &&exec,
                   InputIterator1 first1, InputIterator1 last1,
                   InputIterator2 first2, InputIterator2 last2,
                   OutputIterator result, Compare comp);
```

1. *Effects:* Copies the elements of the range `[first1,last1)` which are not present in the range `[first2,last2)` to the range beginning at `result`. The elements in the
   constructed range are sorted. The algorithm's execution is parallelized as determined by `exec`.

2. *Requires:* The resulting range shall not overlap with either of the original ranges.

3. *Returns:* The end of the constructed range.

4. *Complexity:* `O(m + n)`, where `m = last1 - first1` and `n = last2 - first2`.

5. *Remarks:* If `[first1,last1)` contains $m$ elements that are equivalent to each other and `[first2,last2)` contains $n$ elements that are equivalent to them, the last
   $max(m-n,0)$ elements from `[first1,last1)` shall be copied to the output range.

    The signatures shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.

### `set_symmetric_difference` `[set.symmetric.difference]`

```
template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2,
         class OutputIterator>
  OutputIterator
    set_symmetric_difference(ExecutionPolicy &&exec,
                             InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             OutputIterator result);

template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2,
         class OutputIterator, class Compare>
  OutputIterator
    set_symmetric_difference(ExecutionPolicy &&exec,
                             InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             OutputIterator result, Compare comp);
```

1. *Effects:* Copies the elements of the range `[first1,last1)` that are not present in the range `[first2,last2)`, and the elements of the range `[first2,last2)` that are not
   present in the range `[first1,last1)` to the range beginning at `result`. The elements in the constructed range are sorted. The algorithm's execution is parallelized as
   determined by `exec`.

2. *Requires:* The resulting range shall not overlap with either of the original ranges.

3. *Returns:* The end of the constructed range.

4. *Complexity:* `O(m + n)`, where `m = last1 - first1` and `n = last2 - first2`.

5. *Remarks:* If `[first1,last1)` contains $m$ elements that are equivalent to each other and `[first2,last2)` contains $n$ elements that are equivalent to them, then
   $|m-n|$ of those elements shall be copied to the output range: the last $m - n$ of these elements from `[first1,last1)` if $m > n$, and the last $n - m$ of these
   elements from `[first2,last2)` if $m < n$.

    The signatures shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Minimum and maximum `[alg.min.max]`

```
template<class ExecutionPolicy,
         class ForwardIterator>
  ForwardIterator min_element(ExecutionPolicy &&exec,
                              ForwardIterator first, ForwardIterator last);

template<class ExecutionPolicy,
         class ForwardIterator, class Compare>
  ForwardIterator min_element(ExecutionPolicy &&exec,
                              ForwardIterator first, ForwardIterator last,
                              Compare comp);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The first iterator `i` in the range `[first,last)` such that for any iterator `j` in the range `[first,last)` the following corresponding conditions hold:
   `!(*j < *i)` or `comp(*j, *i) == false`. Returns `last` if `first == last`.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class ForwardIterator>
  ForwardIterator max_element(ExecutionPolicy &&exec,
                              ForwardIterator first, ForwardIterator last);

template<class ExecutionPolicy,
         class ForwardIterator, class Compare>
  ForwardIterator max_element(ExecutionPolicy &&exec,
                              ForwardIterator first, ForwardIterator last,
                              Compare comp);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* The first iterator `i` in the range `[first,last)` such that for any iterator `j` in the range `[first,last)` the following corresponding conditions hold:
   `!(*i < *j)` or `comp(*i, *j) == false`. Returns `last` if `first == last`.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

```
template<class ExecutionPolicy,
         class ForwardIterator>
  pair<ForwardIterator, ForwardIterator>
    minmax_element(ExecutionPolicy &&exec,
                   ForwardIterator first, ForwardIterator last);

template<class ExecutionPolicy,
         class ForwardIterator, class Compare>
  pair<ForwardIterator, ForwardIterator>
    minmax_element(ExecutionPolicy &&exec,
                   ForwardIterator first, ForwardIterator last, Compare comp);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `make_pair(first, first)` if `[first,last)` is empty, otherwise `make_pair(m, M)`, where `m` is the first iterator in `[first,last)` such that no iterator in the
   range refers to a smaller element, and where `M` is the last iterator in `[first,last)` such that no iterator in the range refers to a larger element.

3. *Complexity:* `O(last - first)`.

4. *Remarks:* The signatures shall not participate in overload resolution if

    `is_execution_policy<ExecutionPolicy>::value` is `false`.

### Lexicographical comparison `[alg.lex.comparison]`

```
template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2>
  bool
    lexicographical_compare(ExecutionPolicy &&exec,
                            InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2);

template<class ExecutionPolicy,
         class InputIterator1, class InputIterator2, class Compare>
  bool
    lexicographical_compare(ExecutionPolicy &&exec,
                            InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2,
                            Compare comp);
```

1. *Effects:* The algorithm's execution is parallelized as determined by `exec`.

2. *Returns:* `true` if the sequence of elements defined by the range `[first1,last1)` is lexicographically
    less than the sequence of elements defined by the range `[first2,last2)` and `false` otherwise.

3. *Complexity:* `O(min(m,n))`, where `m = last1 - first1` and `n = last2 - first2`.

4. *Remarks:* If two sequences have the same number of elements and their corresponding elements are equivalent,
    then neither sequence is lexicographically less than the other. If one sequence is a prefix of the other, then
    the shorter sequence is lexicographically less than the longer sequence. Otherwise, the lexicographical comparison
    of the sequences yields the same result as the comparison of the first corresponding pair of elements that are not
    equivalent.

    An empty sequence is lexicographically less than any non-empty sequence, but not less than any empty sequence.

    The signatures shall not participate in overload resolution if `is_execution_policy<ExecutionPolicy>::value` is `false`.

## Existing generalized numeric operations from `<numeric>`

### Header `<numeric>` synopsis

```
namespace std {
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

