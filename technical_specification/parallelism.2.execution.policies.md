# Execution policies {#parallel.execpol}

## In general {#parallel.execpol.general}

This subclause describes classes that represent *execution policies*. An
*execution policy* is an object that expresses the requirements on the ordering
of functions invoked as a consequence of the invocation of a standard
algorithm. Execution policies afford standard algorithms the discretion to
execute in parallel.

[*Example:*

    std::vector<int> v = ...

    // standard sequential sort
    std::sort(vec.begin(), vec.end());                  

    using namespace std::experimental::parallel;

    // explicitly sequential sort
    sort(seq, v.begin(), v.end());        

    // permitting parallel execution
    sort(par, v.begin(), v.end());        

    // permitting vectorization as well
    sort(vec, v.begin(), v.end());

    // sort with dynamically-selected execution
    size_t threshold = ...
    execution_policy exec = seq;
    if(v.size() > threshold)
    {
      exec = par;
    }

    sort(exec, v.begin(), v.end());

-- *end example*]


[*Note:* Because different parallel architectures may require idiosyncratic
parameters for efficient execution, implementations of the Standard Library 
should provide additional execution policies to those described in this
Technical Specification as extensions. *-- end note*]

## Header `<experimental/execution_policy>` synopsis {#parallel.execpol.synop}

\begin{Verbatim}[commandchars=\\\[\]]
namespace std { 
namespace experimental {
namespace parallel {
  // 2.3, execution policy type trait
  template<class T> struct is_execution_policy;

  // 2.4, sequential execution policy
  class sequential_execution_policy;

  // 2.5, parallel execution policy
  class parallel_execution_policy;

  // 2.6, vector execution policy
  class vector_execution_policy;

  // 2.7, dynamic execution policy
  class execution_policy;

\remvrbline[  // 2.8, specialized algorithms]
\remvrbline[  void swap(sequential_execution_policy& a, sequential_execution_policy& b);]
\remvrbline[  void swap(parallel_execution_policy& a, parallel_execution_policy& b);]
\remvrbline[  void swap(vector_execution_policy& a, vector_execution_policy& b);]
\remvrbline[  void swap(execution_policy& a, execution_policy& b);]

  // 2.9, standard execution policy objects
  extern const sequential_execution_policy seq;
  extern const parallel_execution_policy   par;
  extern const vector_execution_policy     vec;
}
}
}
\end{Verbatim}

## Execution policy type trait {#parallel.execpol.type}

    namespace std {
    namespace experimental {
    namespace parallel {
      template<class T> struct is_execution_policy
        : integral_constant<bool, see below> { };
    }
    }
    }

1. `is_execution_policy` can be used to detect parallel execution policies for the purpose of excluding function signatures from otherwise ambiguous overload resolution participation.

2. If `T` is the type of a standard or implementation-defined execution policy, `is_execution_policy<T>` shall be publicly derived from `integral_constant<bool,true>`,
otherwise from `integral_constant<bool,false>`.

3. The behavior of a program that adds specializations for `is_execution_policy` is undefined.

## Sequential execution policy {#parallel.execpol.seq}

\begin{Verbatim}[commandchars=\\\[\]]
namespace std {
namespace experimental {
namespace parallel {

\remvrbline[  class sequential_execution_policy]
\remvrbline[  {]
\remvrbline[    void swap(sequential_execution_policy& other);]
\remvrbline[  };]
\addvrbline[  class sequential_execution_policy{};]
}
}
}
\end{Verbatim}

1. The class `sequential_execution_policy` is an execution policy type used as a unique type to disambiguate parallel algorithm overloading and require that a parallel algorithm's execution may not be parallelized.

\begin{Verbatim}[commandchars=\\\[\]]
\remvrbline[void swap(sequential_execution_policy& other);]
\end{Verbatim}
    
\color{remclr}
~~2. *Effects:* Swaps the state of `*this` and `other`.~~
\color{black}

## Parallel execution policy {#parallel.execpol.par}

\begin{Verbatim}[commandchars=\\\[\]]
namespace std {
namespace experimental {
namespace parallel {

\remvrbline[  class parallel_execution_policy]
\remvrbline[  {]
\remvrbline[    void swap(parallel_execution_policy& other);]
\remvrbline[  };]
\addvrbline[  class parallel_execution_policy{};]
}
}
}
\end{Verbatim}

1. The class `parallel_execution_policy` is an execution policy type used as a unique type to disambiguate parallel algorithm overloading and indicate that a parallel algorithm's execution may be parallelized.

\begin{Verbatim}[commandchars=\\\[\]]
\remvrbline[void swap(parallel_execution_policy& other);]
\end{Verbatim}
    
\color{remclr}
~~2. *Effects:* Swaps the state of `*this` and `other`.~~
\color{black}

## Vector execution policy {#parallel.execpol.vec}

\begin{Verbatim}[commandchars=\\\[\]]
namespace std {
namespace experimental {
namespace parallel {

\remvrbline[  class vector_execution_policy]
\remvrbline[  {]
\remvrbline[    void swap(vector_execution_policy& other);]
\remvrbline[  };]
\addvrbline[  class vector_execution_policy{};]
}
}
}
\end{Verbatim}

1. The class `vector_execution_policy` is an execution policy type used as a unique type to disambiguate parallel algorithm overloading and indicate that a parallel algorithm's execution may be vectorized.

\begin{Verbatim}[commandchars=\\\[\]]
\remvrbline[void swap(vector_execution_policy& other);]
\end{Verbatim}
    
\color{remclr}
~~2. *Effects:* Swaps the state of `*this` and `other`.~~
\color{black}

## Dynamic execution policy {#parallel.execpol.dynamic}

\begin{Verbatim}[commandchars=\\\[\]]
namespace std {
namespace experimental {
namespace parallel {

  class execution_policy
  {
    public:
\remvrbline[      // 2.7.1, construct/assign/swap]
\addvrbline[      // 2.7.1, construct/assign]
      template<class T> execution_policy(const T& exec);
      template<class T> execution_policy& operator=(const T& exec);
\remvrbline[      void swap(execution_policy& other);]

      // 2.7.2, object access
      const type_info& target_type() const;
      template<class T> T* target();
      template<class T> const T* target() const;
  };
}
}
}
\end{Verbatim}

1. The class `execution_policy` is a dynamic container for execution policy objects. `execution_policy` allows dynamic control over standard algorithm execution.

    [*Example:*

        std::vector<float> sort_me = ...
        
        std::execution_policy exec = std::seq;
        
        if(sort_me.size() > threshold)
        {
          exec = std::par;
        }
         
        std::sort(exec, sort_me.begin(), sort_me.end());

    -- *end example*]

2. Objects of type `execution_policy` shall be constructible and assignable from objects of type `T` for which `is_execution_policy<T>::value` is `true`.

### `execution_policy` construct/assign/swap {#parallel.execpol.con}

    template<class T> execution_policy(const T &exec);

1. *Effects:* Constructs an `execution_policy` object with a copy of `exec`'s state.

2. *Requires:* `is_execution_policy<T>::value` is `true`

```
template<class T> execution_policy& operator=(const T& exec);
```

3. *Effects:* Assigns a copy of `exec`'s state to `*this`.

4. *Returns:* `*this`.

5. *Requires:* `is_execution_policy<T>::value` is `true`

\begin{Verbatim}[commandchars=\\\[\]]
\remvrbline[void swap(execution_policy& other);]
\end{Verbatim}

\color{remclr}
~~1. *Effects:* Swaps the stored object of `*this` with that of `other`.~~
\color{black}

### `execution_policy` object access {#parallel.execpol.access}

    const type_info& target_type() const;

1. *Returns:* `typeid(T)`, such that `T` is the type of the execution policy object contained by `*this`.

```
template<class T> T* target();
template<class T> const T* target() const;
```

2. *Returns:* If `target_type() == typeid(T)`, a pointer to the stored execution policy object; otherwise a null pointer.

3. *Requires:* `is_execution_policy<T>::value` is `true`

## \color{red}~~Execution policy specialized algorithms~~\color{black} {#parallel.execpol.algorithms}

\begin{Verbatim}[commandchars=\\\[\]]
\remvrbline[void swap(sequential_execution_policy& a, sequential_execution_policy& b);]
\remvrbline[void swap(parallel_execution_policy& a, parallel_execution_policy& b);]
\remvrbline[void swap(vector_execution_policy& a, vector_execution_policy& b);]
\remvrbline[void swap(execution_policy& a, execution_policy& b);]
\end{Verbatim}

\color{remclr}
~~1. *Effects:* `a.swap(b)`.~~
\color{black}

## Execution policy objects {#parallel.execpol.objects}

    namespace std {
    namespace experimental {
    namespace parallel {
      constexpr sequential_execution_policy seq = sequential_execution_policy();
      constexpr parallel_execution_policy   par = parallel_execution_policy();
      constexpr vector_execution_policy     vec = vector_execution_policy();
    }
    }
    }

1. The header `<execution_policy>` declares a global object associated with each type of execution policy defined by this technical specification.

