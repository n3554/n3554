## Specialized memory algorithms [alg.memory] {#alg.memory}

This subclause defines function templates for constructing multiple objects in uninitialized memory buffers in parallel.

### Header `<experimental/memory>` synopsis [alg.memory.synop] {#alg.memory.synop}

```
namespace std {
namespace experimental {
namespace parallelism {
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
}
}
```

### `uninitialized_copy` [alg.memory.uninitialized.copy] {#alg.memory.uninitialized.copy}

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

### `uninitialized_fill` [alg.memory.uninitialized.fill] {#alg.memory.uninitialized.fill}

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

### `uninitialized_fill_n` [alg.memory.uninitialized.fill.n] {#alg.memory.uninitialized.fill.n}

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

