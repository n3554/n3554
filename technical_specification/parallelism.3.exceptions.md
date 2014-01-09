# Parallel exceptions [exceptions] {#exceptions}

## Exception reporting behavior [exceptions.behavior] {#exceptions.behavior}

1. During the execution of a standard parallel algorithm, if the application of a function object terminates with an
   uncaught exception, the behavior of the program is determined by the type of execution policy used to invoke the algorithm.

   * If the execution policy object is of type `vector_execution_policy`, `std::terminate` shall be called.

   * If the execution policy object is of type `sequential_execution_policy` or `parallel_execution_policy`,
     the execution of the algorithm terminates with an `exception_list` exception. All uncaught exceptions thrown during
     the application of user-provided function objects shall be contained in the `exception_list`, however the number of such exceptions is unspecified.

      [*Note:* For example, the number of invocations of the user-provided function object in `for_each` is unspecified. When 
      `for_each` is executed sequentially, only one exception will be contained in the `exception_list` object -- *end note*]

      [*Note:* These guarantees imply that, unless the algorithm has failed to
      allocate memory and terminated with `std::bad_alloc`, all exceptions thrown 
      during the execution of the algorithm are communicated to the caller. It is 
      unspecified whether an algorithm implementation will "forge ahead" after 
      encountering and capturing a user exception. -- *end note*]

   * If the execution policy object is of any other type, the behavior is implementation-defined.

2. If temporary memory resources are required by the algorithm and none are available, the algorithm may terminate with an `std::bad_alloc` exception.

      [*Note:* The algorithm may terminate with the `std::bad_alloc` exception even if
      one or more user-provided function objects have terminated with an exception. 
      For example, this can happen when an algorithm fails to allocate memory while
      creating or adding elements to the `exception_list` object -- *end note*]

## Header `<experimental/exception>` synopsis {#exceptions.synop}

    namespace std {
    namespace experimental {
    namespace parallel {
      class exception_list : public exception
      {
        public:
          typedef exception_ptr     value_type;
          typedef const value_type& reference;
          typedef const value_type& const_reference;
          typedef size_t            size_type;
          typedef unspecified       iterator;
          typedef unspecified       const_iterator;
      
          size_t size() const;
          iterator begin() const;
          iterator end() const;
      
        private:
          std::list<exception_ptr> exceptions_; // exposition only
      };
    }
    }
    }

1. The class `exception_list` is a container of `exception_ptr` objects parallel algorithms may use to communicate uncaught exceptions encountered during parallel execution to the caller of the algorithm.

```
size_t size() const;
```

2. *Returns:* The number of `exception_ptr` objects contained within the `exception_list`.

```
exception_list::iterator begin() const;
```

3. *Returns:* An iterator pointing to the first `exception_ptr` object contained within the `exception_list`.

```
exception_list::iterator end() const;
```

4. *Returns:* An iterator pointing to one position past the last `exception_ptr` object contained within the `exception_list`.

