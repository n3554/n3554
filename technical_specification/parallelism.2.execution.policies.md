# Execution policies [execpol] {#execpol}

## In general [execpol.general] {#execpol.general}

This subclause describes classes that represent *execution policies*. An
*execution policy* is an object that expresses the requirements on the ordering
of functions invoked as a consequence of the invocation of a standard
algorithm. Execution policies afford standard algorithms the discretion to
execute in parallel.

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

1. The class `sequential_execution_policy` provides a mechanism to require a standard algorithm invocation to execute in a sequential order.

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

    void swap(sequential_execution_policy &other);
    
1. *Effects:* Swaps the state of `*this` and `other`.

## Parallel execution policy [execpol.par] {#execpol.par}

1. The class `parallel_execution_policy` provides a mechanism to allow a standard algorithm invocation to execute in an unordered fashion when executed on separate threads, and indeterminately sequenced when executed on a single thread.

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

    void swap(parallel_execution_policy &other);
    
1. *Effects:* Swaps the state of `*this` and `other`.

## Vector execution policy [execpol.vec] {#execpol.vec}

1. The class `vector_execution_policy` provides a mechanism to allow a standard algorithm invocation to execute in an unordered fashion when executed on separate threads, and unordered when executed on a single thread.

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

    void swap(vector_execution_policy &other);
    
1. *Effects:* Swaps the state of `*this` and `other`.

## Dynamic execution policy [execpol.dynamic] {#execpol.dynamic}

1. The class `execution_policy` is a dynamic container for execution policy objects.

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

### `execution_policy` construct/assign/swap

    template<class T> execution_policy(const T &exec);

1. *Effects:* Constructs an `execution_policy` object with a copy of `exec`'s state.

2. *Remarks:* This signature does not participate in overload resolution if
   `is_execution_policy<T>::value` is `false`.

    template<class T> execution_policy &operator=(const T &exec);

1. *Effects:* Assigns a copy of `exec`'s state to `*this`.

2. *Returns:* `*this`.

3. `Remarks:* This signature does not participate in overload resolution if
   `is_execution_policy<T>::value` is `false`.

    void swap(execution_policy &other);

1. *Effects:* Swaps the stored object of `*this` with that of `other`.

### `execution_policy` object access

    const type_info &target_type() const;

1. *Returns:* `typeid(T)`, such that `T` is the type of the execution policy object contained by `*this`.

    template<class T> T *target();
    template<class T> const T *target() const;

1. *Returns:* If `target_type() == typeid(T)`, a pointer to the stored execution policy object; otherwise a null pointer.
2. `Remarks:* This signature does not participate in overload resolution if
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

