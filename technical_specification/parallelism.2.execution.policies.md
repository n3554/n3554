# Execution policies [execpol] {#execpol}

## In general [execpol.general] {#execpol.general}

This subclause describes classes that represent *execution policies*. An
*execution policy* is an object that expresses the requirements on the ordering
of functions invoked as a consequence of the invocation of a standard
algorithm. Execution policies afford standard algorithms the discretion to
execute in parallel.

[*Example:*

    std::vector<int> vec = ...

    // standard sequential sort
    std::sort(vec.begin(), vec.end());                  

    using namespace std::experimental::parallelism;

    // explicitly sequential sort
    sort(seq, vec.begin(), vec.end());        

    // permitting parallel execution
    sort(par, vec.begin(), vec.end());        

    // permitting vectorization as well
    sort(vec, vec.begin(), vec.end());

    // sort with dynamically-selected execution
    size_t threshold = ...
    execution_policy exec = seq;
    if(vec.size() > threshold)
    {
      exec = par;
    }

    sort(exec, vec.begin(), vec.end());

-- *end example*]


[*Note:* Because different parallel architectures may require idiosyncratic
parameters for efficient execution, implementations of the Standard Library are
encouraged to provide additional execution policies to those described in this
Technical Specification as extensions. *-- end note*]

## Header `<experimental/execution_policy>` synopsis [execpol.synop] {#execpol.synop}

    #include <type_traits>

    namespace std { 
    namespace experimental {
    namespace parallelism {
      // 2.3, execution policy type trait
      template<class T> struct is_execution_policy;

      // 2.4, sequential execution policy
      class sequential_execution_policy;

      // 2.5, parallel execution policy
      class parallel_execution_policy;

      // 2.6, vector execution policy
      class vector_execution_policy

      // 2.7, dynamic execution policy
      class execution_policy;

      // 2.8, specialized algorithms
      void swap(sequential_execution_policy &a, sequential_execution_policy &b);
      void swap(parallel_execution_policy &a, parallel_execution_policy &b);
      void swap(vector_execution_policy &a, vector_execution_policy &b);
      void swap(execution_policy &a, execution_policy &b);

      // 2.9, standard execution policy objects
      extern const sequential_execution_policy seq;
      extern const parallel_execution_policy   par;
      extern const vector_execution_policy     vec;
    }
    }
    }

1. An implementation may provide additional execution policy types besides `parallel_execution_policy`, `sequential_execution_policy`, `vector_execution_policy`, or `execution_policy`.

## Execution policy type trait [execpol.type] {#execpol.type}

    namespace std {
    namespace experimental {
    namespace parallelism {
      template<class T> struct is_execution_policy
        : integral_constant<bool, see below> { };
    }
    }
    }

1. `is_execution_policy` can be used to detect parallel execution policies for the purpose of excluding function signatures from otherwise ambiguous overload resolution participation.

2. If `T` is the type of a standard or implementation-defined non-standard execution policy, `is_execution_policy<T>` shall be publicly derived from `integral_constant<bool,true>`,
otherwise from `integral_constant<bool,false>`.

3. The effect of specializing `is_execution_policy` for a type which is not defined by the library is unspecified.

    [*Note:* This provision reserves the privilege of creating non-standard execution policies to the library implementation. -- *end note*.]

## Sequential execution policy [execpol.seq] {#execpol.seq}

```
namespace std {
namespace experimental {
namespace parallelism {

  class sequential_execution_policy
  {
    void swap(sequential_execution_policy &other);
  };
}
}
}
```

1. The class `sequential_execution_policy` provides a mechanism to require a standard algorithm invocation to execute in a sequential order.

2. Implementations of `sequential_execution_policy` are permitted to provide additional non-standard data and function members.

    [*Note:* This provision permits `sequential_execution_policy` objects to be stateful. -- *end note*.]

```
void swap(sequential_execution_policy &other);
```
    
2. *Effects:* Swaps the state of `*this` and `other`.

## Parallel execution policy [execpol.par] {#execpol.par}

```
namespace std {
namespace experimental {
namespace parallelism {

  class parallel_execution_policy
  {
    void swap(parallel_execution_policy &other);
  };
}
}
}
```

1. The class `parallel_execution_policy` provides a mechanism to allow a standard algorithm invocation to execute in an unordered fashion when executed on separate threads, and indeterminately sequenced when executed on a single thread.

2. Implementations of `parallel_execution_policy` are permitted to provide additional non-standard data and function members.

    [*Note:* This provision permits `parallel_execution_policy` objects to be stateful. -- *end note*.]

```
void swap(parallel_execution_policy &other);
```
    
2. *Effects:* Swaps the state of `*this` and `other`.

## Vector execution policy [execpol.vec] {#execpol.vec}

```
namespace std {
namespace experimental {
namespace parallelism {

  class vector_execution_policy
  {
    void swap(vector_execution_policy &other);
  };
}
}
}
```

1. The class `vector_execution_policy` provides a mechanism to allow a standard algorithm invocation to execute in an unordered fashion when executed on separate threads, and unordered when executed on a single thread.

2. Implementations of `vector_execution_policy` are permitted to provide additional non-standard data and function members.

    [*Note:* This provision permits `vector_execution_policy` objects to be stateful. -- *end note*.]

```
void swap(vector_execution_policy &other);
```
    
2. *Effects:* Swaps the state of `*this` and `other`.

## Dynamic execution policy [execpol.dynamic] {#execpol.dynamic}

```
namespace std {
namespace experimental {
namespace parallelism {

  class execution_policy
  {
    public:
      // 2.7.1, construct/assign/swap
      template<class T> execution_policy(const T &exec);
      template<class T> execution_policy &operator=(const T &exec);
      void swap(execution_policy &other);

      // 2.7.2, object access
      const type_info& target_type() const;
      template<class T> T *target();
      template<class T> const T *target() const;
  };
}
}
}
```

1. The class `execution_policy` is a dynamic container for execution policy objects.

2. `execution_policy` allows dynamic control over standard algorithm execution.

   [*Example:*

       std::vector<float> sort_me = ...
       
       std::execution_policy exec = std::seq;
       
       if(sort_me.size() > threshold)
       {
         exec = std::par;
       }
        
       std::sort(exec, sort_me.begin(), sort_me.end());

   -- *end example*]

3. The stored dynamic value of an `execution_policy` object may be retrieved.

    [*Example:*

        void some_api(std::execution_policy exec, int arg1, double arg2)
        {
          if(exec.target_type() == typeid(std::seq))
          {
            std::cout << "Received a sequential policy" << std::endl;
            auto *exec_ptr = exec.target<std::sequential_execution_policy>();
          }
          else if(exec.target_type() == typeid(std::par))
          {
            std::cout << "Received a parallel policy" << std::endl;
            auto *exec_ptr = exec.target<std::parallel_execution_policy>();
          }
          else if(exec.target_type() == typeid(std::vec))
          {
            std::cout << "Received a vector policy" << std::endl;
            auto *exec_ptr = exec.target<std::vector_execution_policy>();
          }
          else
          {
            std::cout << "Received some other kind of policy" << std::endl;
          }
        }

    -- *end example*]

4. Objects of type `execution_policy` shall be constructible and assignable from any additional non-standard execution policy provided by the implementation.

### `execution_policy` construct/assign/swap

    template<class T> execution_policy(const T &exec);

1. *Effects:* Constructs an `execution_policy` object with a copy of `exec`'s state.

2. *Remarks:* This signature does not participate in overload resolution if
   `is_execution_policy<T>::value` is `false`.

```
template<class T> execution_policy &operator=(const T &exec);
```

3. *Effects:* Assigns a copy of `exec`'s state to `*this`.

4. *Returns:* `*this`.

5. *Remarks:* This signature does not participate in overload resolution if
   `is_execution_policy<T>::value` is `false`.

```
void swap(execution_policy &other);
```

1. *Effects:* Swaps the stored object of `*this` with that of `other`.

### `execution_policy` object access

    const type_info &target_type() const;

1. *Returns:* `typeid(T)`, such that `T` is the type of the execution policy object contained by `*this`.

```
template<class T> T *target();
template<class T> const T *target() const;
```

2. *Returns:* If `target_type() == typeid(T)`, a pointer to the stored execution policy object; otherwise a null pointer.
3. *Remarks:* This signature does not participate in overload resolution if
   `is_execution_policy<T>::value` is `false`.

## Execution policy specialized algorithms [execpol.algorithms] {#execpol.algorithms}

      void swap(sequential_execution_policy &a, sequential_execution_policy &b);
      void swap(parallel_execution_policy &a, parallel_execution_policy &b);
      void swap(vector_execution_policy &a, vector_execution_policy &b);
      void swap(execution_policy &a, execution_policy &b);

1. *Effects:* `a.swap(b)`.

## Standard execution policy objects [execpol.objects] {#execpol.objects}

    namespace std {
    namespace experimental {
    namespace parallelism {
      extern const sequential_execution_policy seq;
      extern const parallel_execution_policy   par;
      extern const vector_execution_policy     vec;
    }
    }
    }

1. The header `<execution_policy>` declares a global object associated with each standard execution policy.

2. Concurrent access to these objects shall not result in a data race.

```
const sequential_execution_policy seq;
```

3. The object `seq` requires a standard algorithm to execute sequentially.

```
const parallel_execution_policy par;
```

4. The object `par` allows a standard algorithm to execute in an unordered fashion when executed on separate threads, and indeterminately sequenced when executed on a single thread.

```
const vector_execution_policy vec;
```

5. The object `vec` allows a standard algorithm to execute in an unordered fashion when executed on separate threads, and unordered when executed on a single thread.

