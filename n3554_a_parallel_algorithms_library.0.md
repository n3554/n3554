% A Parallel Algorithms Library | N3554
% Jared Hoberock; Jaydeep Marathe; Michael Garland; Olivier Giroux; Vinod Grover
  {jhoberock, jmarathe, mgarland, ogiroux, vgrover}@nvidia.com
  Artur Laksberg; Herb Sutter
  {arturl, hsutter}@microsoft.com
  Arch Robison
  arch.robison@intel.com
% 2013-03-15

Executive Summary
=================

We introduce a library of algorithms with parallel execution semantics. Some of these algorithms have semantics similar to the existing standard library of sequential algorithms. Some of the algorithms we propose are novel.

We introduce three parallel execution policies for parallel algorithm execution: `std::seq`, `std::par`, and `std::vec`, as well as a facility for vendors to provide non-standard execution policies as extensions. These policy objects
may be used to specify how a parallel algorithm should be executed:

```
std::vector<int> vec = ...

// legacy sequential sort
std::sort(vec.begin(), vec.end());                  

// explicit sequential sort
std::sort(std::seq, vec.begin(), vec.end());        

// parallel sort
std::sort(std::par, vec.begin(), vec.end());        

// vectorized sort
std::sort(std::vec, vec.begin(), vec.end());

// sort with dynamically-selected execution
size_t threshold = ...
std::execution_policy exec = std::seq;
if(vec.size() > threshold)
{
  exec = std::par;
}

std::sort(exec, vec.begin(), vec.end());

// parallel sort with non-standard implementation-provided execution policies:
std::sort(vectorize_in_this_thread, vec.begin(), vec.end());
std::sort(submit_to_my_thread_pool, vec.begin(), vec.end());
std::sort(execute_on_that_gpu, vec.begin(), vec.end());
std::sort(offload_to_my_fpga, vec.begin(), vec.end());
std::sort(send_this_computation_to_the_cloud, vec.begin(), vec.end());
```

Algorithms invoked with `std::seq` execute internally in sequential order in the calling thread.

Algorithms invoked with `std::par` are permitted to execute internally in an unordered fashion in unspecified threads. It is the caller's responsibility to ensure
that the invocation does not introduce data races or deadlocks.

Algorithms invoked with `std::vec` are permitted to execute internally in an
unordered fashion in unspecified threads. In addition to the restrictions
implied by `std::par`, it is the caller's responsibility to ensure that a
`std::vec` invocation does not throw exceptions or attempt to perform
synchronization operations.

Algorithms invoked without an execution policy execute as if they were invoked with `std::seq`.

An implementation may provide additional execution policies besides `std::seq`, `std::par`, or `std::vec`.

This proposal is a pure addition to the existing C++ standard library; we do not believe it alters the semantics of any existing functionality.

---------------------------

Design Notes and Outstanding Questions
======================================

Before introducing the detailed specification of our proposal, we begin by outlining our rationale and by noting that we have identified a number of outstanding questions which require further work to resolve. 

## General Considerations

This proposal is motivated by a strong desire to provide a standard model of
parallelism enabling performance portability across the broadest possible range
of parallel architectures. For this reason, we have exposed parallelism in the
most abstract manner possible in order to avoid presuming the existence of a
particular parallel machine model. In particular, we have intentionally avoided
a specification which would be required to introduce concurrency by creating
threads. Additionally, our proposal's semantics ensure that a sequential
implementation is always a valid realization of an algorithm invocation.

One limitation of STL-like algorithms is that they encourage the programmer to
engage in a style of programming which may be an obstacle to achieving large
absolute performance. For example, in situations where a sequential programmer
might implement a program using a single `for` loop, a parallel programmer
might express the same program as a sequence of separate `gather`, `for_each`,
and `scatter` phases. This is troublesome because in many cases the
performance of most STL algorithms is bounded by the speed of memory
bandwidth, and the rate of memory bandwidth scaling on parallel
architectures is slowing.

One way to ameliorate such problems is to combine the use of parallel
algorithms with "fancy" iterators in the style of the Boost Iterator Library.
Iterators such as `transform_iterator` can fuse the effect of `std::transform`
into another algorithm call, while a `permutation_iterator` can fuse a scatter
or gather. By fusing together several "elemental" operations into a single
function consumed by a parallel algorithm, memory bandwidth requirements can be
reduced significantly. However, because this idea is orthogonal to the idea of
parallel algorithms, this proposal does not include a novel iterator library.

While we are enthusiastic that our library solution to parallelism can offer a
portable way to author programs for parallel architectures, we acknowledge that
it is only a partial solution for parallelism. We expect our library to coexist
in an ecosystem of standard language and library constructs which target
parallelism at varying levels of abstraction.

## Naming

Integrating and exposing a new library of parallel algorithms into the existing
C++ standard library is an interesting challenge. Because this proposal
introduces a large set of novel parallel algorithms with semantics subtly
different from their existing sequential counterparts, it is crucial to provide
the programmer with the means to safely and unambiguously express her
parallelization intent. By the same token, it is important for the library
interface to distinguish between the different concurrency guarantees provided
by parallel and sequential algorithms. Yet, the interface must be flexible
enough such that parallelization does not become burdensome.

The primary means by which the user will interact with the standard parallel
algorithms library will be by invoking parallel algorithms by name. Because
many parallel algorithm names are already taken by their sequential
counterparts (e.g., `sort`), we require a way to disambiguate these
invocations.

After considering a variety of alternative designs, we propose to integrate
parallelism into the existing standard algorithm names and distinguish
parallelism via parallel execution policies with distinct types. As the code
sample in the executive summary demonstrates, we feel that this scheme provides
deep integration with the existing standard library.

### Rejected Alternative Naming Schemes

Here, we discuss the rationale for rejecting alternative schemes we considered.

#### A `parallel_` Name Prefix

A simple way to disambiguate parallel algorithms from their sequential versions
would be simply to give them new, unique names. Indeed, this is the approach suggested
by Intel & Microsoft’s earlier paper N3429 and is the one taken in their libraries
(i.e. Threading Building Blocks & Parallel Patterns Library, respectively). It
is impossible for a human reader or implementation to confuse a call to
`std::for_each` for `std::parallel_for_each` and vice versa.

While such an approach could be standardized safely, it is unclear that this
scheme is scalable from the six or so algorithms provided by TBB & PPL to the
large set of algorithms we propose to parallelize. The following two code
examples demonstrate issues we anticipate.

By requiring the programmer to choose between two static names, it seems
impossible to allow the dynamic selection between the sequential or parallel
version of an algorithm without imposing an unnecessary burden on the
programmer:

```
std::vector sort_me = ...
size_t parallelization_threshold = ...

if(sort_me.size() > parallelization_threshold)
{
  std::parallel_sort(sort_me.begin(), sort_me.end());
}
else
{
  std::sort(sort_me.begin(), sort_me.end());
}
```

It is likely that this idiom would become a repetitive burden prone to
mistakes.

A similar problem exists for static decisions. Consider a function template
which wishes to make a static decision regarding parallelism:

```
template<bool parallelize>
void func(std::vector &vec)
{
  if(parallelize)
  {
    std::parallel_transform(vec.begin(), vec.end(), vec.begin(), f);
    std::parallel_sort(vec.begin(), vec.end());
    std::parallel_unique(vec.begin(), vec.end());
  }
  else
  {
    std::transform(vec.begin(), vec.end(), vec.begin(), f);
    std::sort(vec.begin(), vec.end());
    std::unique(vec.begin(), vec.end());
  }
}
```

This idiom requires the programmer to repeat the function body twice even
though the semantics of both implementations are largely identical.

Finally, such a scheme also seems unnecessarily verbose: a sophisticated
program composed of repeated calls to a large variety of parallel algorithms
would become a noisy repetition of `parallel_`.

We require a scheme which preserves the safety of unique names but which can
also be terse, flexible, and resilient to programmer error. Distinguishing
parallel algorithms by execution policy parameters ensures safe disambiguation
while also enabling the same terse style shared by the existing algorithms
library. The execution policy parameter also provides flexibility and solves
the problems of the previous two code examples.

With execution policies, the first dynamic parallelization example becomes:

```
std::vector sort_me = ...
size_t threshold = ...

std::execution_policy exec = std::seq;

if(sort_me.size() > threshold)
{
  exec = std::par;
}

std::sort(exec, sort_me.begin(), sort_me.end());
```

The second static parallelization example becomes:

```
template<ExecutionPolicy>
void func(ExecutionPolicy &exec, std::vector &vec)
{
  std::transform(exec, vec.begin(), vec.end(), vec.begin(), f);
  std::sort(exec, vec.begin(), vec.end());
  std::unique(exec, vec.begin(), vec.end());
}
```

#### A Nested `std::parallel` Namespace

Another naming scheme would be to provide overloads of the existing standard algorithms in a nested `std::parallel` namespace.
This scheme would avoid many of the problems we identified with distinguishing parallel algorithms by a name prefix. However,
we observed that a namespace would introduce ambiguities when algorithms are invoked via argument dependent lookup:

```
void func(std::vector &vec)
{
  transform(vec.begin(), vec.end(), vec.begin(), f);
  sort(vec.begin(), vec.end());
  unique(vec.begin(), vec.end());
}
```

Are the algorithms invoked by `func` parallel or not? A reader must search for `using` to be sure.

Finally, we note that nested namespaces inside `std::` are unconventional and generally frowned upon.

## Execution Policies as Stateful Types

We propose that parallel execution policies have distinct, stateful types:

```
namespace std
{
  class sequential_execution_policy { ... };

  extern const sequential_execution_policy seq;

  class parallel_execution_policy { ... };

  extern const parallel_execution_policy par;

  class vector_execution_policy { .. };

  extern const vector_execution_policy vec;

  // a dynamic execution policy container
  class execution_policy { ... };
}
```

and that parallel algorithms receive these objects as their first, templatized parameter:

```
template<typename ExecutionPolicy,
         typename Iterator>
void algo(ExecutionPolicy &&exec, Iterator first, Iterator last);
```

Owing to the variety of parallel architectures we propose that implementations
be permitted to define non-standard implementation-defined execution policies
as extensions. We expect that users with special knowledge about their standard
library implementation and underlying parallel architecture will exploit these
policies to achieve higher performance.

We believe this design represents existing practice and have tabulated a list of some examples found in parallel algorithm libraries in production:

Library              Execution Policy Type                   Notes                                                                       
----------           ---------------------                   -----                                                                       
Thrust               ```thrust::execution_policy```          Controls algorithm dispatch to several
                                                             different parallel backend targets                  
TBB                  ```tbb::auto_partitioner```             Selects an automatic partitioning strategy
PPL                  ```concurrency::affinity_partitioner``` Improves algorithm cache affinity
Boost.MPI            ```boost::mpi::communicator```          Coordinates MPI ranks such that they can
                                                             cooperate in collective algorithms   
Parallel `libstdc++` ```__gnu_parallel::_Parallelism```      Selects from among several parallel
                                                             execution strategies
C++ AMP              ```concurrency::accelerator_view```     Controls algorithm execution locality
Bolt                 ```bolt::cl::control```                 Controls algorithm command queue, debug
                                                             information, load balancing, etc.
----------           ---------------------                   -----

Table: Examples of execution policies found in existing libraries

We propose that parallel algorithms receive execution policy objects as their first, instead of last, parameter primarily for two reasons:

  1. It mirrors the form of `std::async`'s interface.
  2. The first argument allows the reader to easily note the invocation's parallelization at a glance.
  2. It preserves the desirable property that algorithms invoked with a lambda look similar to a `for` loop:

     ```
     std::for_each(std::par, vec.begin(), vec.end(), [](int &x){
       x += 13;
     });
     ```

An alternative design would place the execution policy last and provide a default value:

```
template<typename Iterator,
         typename ExecutionPolicy>
void algo(Iterator first, Iterator last, ExecutionPolicy &&exec = std::par);
```

This design would collapse the "surface area" of the algorithms API
considerably and provide deeper integration into the existing standard
algorithms as execution policies become just a final, optional parameter.

Of the libraries we surveyed, Thrust, Boost.MPI, C++ AMP, and Bolt consistently
placed execution policy parameters first. PPL tended to place execution
policies last, but occasionally accepted execution policy-like hints such as
allocators first. TBB and GNU parallel `libstdc++` consistently placed
execution policies last.

### Rejected Execution Policy Designs

However, other designs are possible. An alternative design might require all
parallel execution policies to be derived from a common root type, say,
`std::execution_policy`:

```
namespace std
{

class execution_policy { ... };

class sequential_execution_policy : public execution_policy { ... };

extern const sequential_execution_policy seq;

class parallel_execution_policy : public execution_policy { ... };

extern const parallel_execution_policy par;

}
```

Instead of a template parameter, algorithm interfaces would receive references to `std::execution_policy`:

```
template<typename Iterator>
void algo(std::execution_policy &exec, Iterator first, Iterator last);
```

We rejected this design for a number of reasons:

  * Erasing the concrete type of the execution policy may make dispatching the algorithm's implementation more expensive than necessary. We worry that for `std::seq` invocations across small sequences, the cost of type erasure and algorithm dispatch could dominate the cost of the algorithm.
  * Requiring an execution policy's type to derive from a particular root type may make it impractical for implementations to define non-standard policies.
  * Requiring an execution policy's type to derive from a root would preclude treating policies as simple types with value semantics. Inheritance from a common root would require APIs to receive policies by reference or pointer.
  * By making the execution policy parameter a concrete type, we would have to commit to either lvalue or rvalue reference semantics for the parameter. With a template parameter, we may support both.
  * Erasing the concrete type of the execution policy would require the implementation to instantiate code for all possible policies for each algorithm invocation. Because parallel algorithm implementations are often significantly more
    complex than their sequential counterparts, this may result in substantial code generation at each call site.

In our survey of existing library designs, we observed that libraries tended
not to adopt a common root type for execution policies.

The exception is Thrust, which exposes a common execution policy root type
which allows users of the library to create novel execution policies. However,
this proposal's design reserves that privilege for the library implementation. 

Some libraries accept a variety of execution policy types to allow for
algorithm customization, while others require a single concrete type.

For example, both TBB and PPL allow for customization and receive their
partitioner arguments as template parameters. Similarly, GNU parallel
`libstdc++` exposes policies as a forest of inheritance trees. The roots of
individual trees are unrelated.

Other libraries do not appear to allow for a variety of policies and instead
provide a single concrete policy type. These types do not appear to allow
customization through inheritance. Boost.MPI, C++ AMP, and Bolt are examples.

Another alternative design might require all parallel execution policies to have the same type:

```
namespace std
{

class execution_policy { ... };

extern const execution_policy seq;

extern const execution_policy par;

}
```

in this alternative design, algorithms would receive such policies by value:

```
template<typename Iterator>
void algo(execution_policy exec, Iterator first, Iterator last);
```

This interface shares most of the same drawbacks as the previous, but allows trafficking execution polices by value.

On the other hand, our proposed algorithm parallel execution policy parameters
are similiar in form and spirit to `std::async`'s launch policy parameter,
which is a dynamic bitfield. There could be some value in mirroring the convention of
`std::async`'s interface in the parallel algorithms library.

## `for_each` Interface Consistency

Because a parallel version of `for_each` cannot accumulate state in its function object argument, the interface we propose
for `for_each` returns a copy of its `last` iterator parameter instead of a copy of its function object:

    template<class ExecutionPolicy, class InputIterator, class Function>
    InputIterator for_each(ExecutionPolicy &&exec,
                           InputIterator first, InputIterator last, Function f);

The rationale for this choice is to avoid discarding iterator information
originating in higher-level algorithms implemented through lowerings to
`for_each`.

For example, because `for_each` returns an iterator, `copy` may be implemented through a lowering to `for_each`:

    template<class ExecutionPolicy, class InputIterator, class OutputIterator>
    OutputIterator copy(ExecutionPolicy &&exec,
                        InputIterator first, InputIterator last, OutputIterator result)
    {
      return std::get<1>(std::for_each(exec,
                                       __make_zip_iterator(first,result),
                                       __make_zip_iterator(last,result),
                                       __copy_function).get_iterator_tuple());
    }

Without `for_each`'s result, `copy` must be implemented through some other non-standard means, which may be burdensome. While implementations
of the standard library could work around this limitation, it would be regrettable to impose this burden on programmers who wish to implement algorithms
with a similar iterator interface.

This is also the motivation behind the addition of our proposed `for_each_n` algorithm, which may implement algorithms such as `copy_n`, `fill_n`, etc.

On the other hand, it may be safer to require our parallel `for_each` to simply return a copy of its function object for consistency's sake.

## Iterator Traversal Requirements

Even though random access to data is a prerequisite for parallel execution, we propose
that the interface to parallel algorithms should not impose additional
requirements over the existing standard library on the traversal categories of
their iterator parameters. In the absense of random access iterators, an
implementation may elect to fall back to sequential execution. Alternatively,
an implementation may elect to introduce temporary copies of
input and output ranges.

## Associativity/Commutativity of Binary Operators

Some parallel algorithms such as `reduce` place stricter requirements on the binary operations they receive than do analogous sequential algorithms such as `accumulate`.

In particular, `reduce` requires its binary operation parameter to be both mathematically associative and commutative in order to accomodate a parallel sum.

To our knowledge, what it means for a binary function object to be associative or commutative is not well-defined by the C++ standard. However, the standard does make such an effort for
other mathematical operations, such as strict weak comparison.

For algorithms which require associative binary operators like `reduce`, should we define concepts such as `AssociativeOperation` similarly to `Compare` instead of using `BinaryOperation`?

Because floating point operations are non-associative, a useful definition of this concept would need to be flexible.

## Machine Width and Space Complexity

Our proposal provides asymptotic upper bounds on work complexity for each
parallel algorithm in terms of the input size. Asymptotic guarantees on space
complexity would be useful as well, particularly because unlike the typical
sequential algorithm, many parallel algorithms require non-trivial temporary
storage. The size of such temporary storage requirements often depends on the
size of the parallel machine.

Unfortunately, C++ does not currently support a notion of parallel machine
size. The closest analogue seems to be the value returned by the function
`std::thread::hardware_concurrency`.

At first glance, relating work complexity to the result of
`std::thread::hardware_concurrency` might seem like a reasonable thing to do.
However, we note that the value of this function is merely advisory; it is not
guaranteed to correspond to an actual physical machine width.  The second more significant
problem with interpreting `std::thread::hardware_concurrency` as machine width
is that it presumes a particular machine model of parallelism, i.e., one in
which the basic primitive of parallelism is a single thread. While this is a
good model for some parallel architectures, it is a poor fit for others. For
example, the width of a parallel machine with a significant investment in SIMD
vector units would be ill-described in terms of threads.

## Container Support for Parallelism

A parallel algorithms library is a fine starting point for exposing parallelism
to programmers in an accessible manner. However, algorithms are only a part of
a complete solution for authoring parallel C++ programs. In addition to algorithms,
the standard library also provides containers for manipulating data in a safe
and convenient manner. While this proposal is focused exclusively on standard
algorithms, many of the operations on standard containers such as
`std::vector` also offer rich opportunities for parallelism. As in sequential programs, without support
for parallelism in containers, authoring sophisticated parallel programs will
become burdensome as programmers will be forced to manage data in an ad hoc
fashion.

Should containers such as `std::vector` be enhanced analogously to the standard
algorithms to support parallel execution? We plan to explore the design of such a
library in a future paper.

---------------------------

Algorithm execution policies
============================

Header `<execution_policy>` synopsis

```
namespace std
{

template<class T> struct is_execution_policy;

class parallel_execution_policy
{
  public:
    void swap(parallel_execution_policy &other);

    // implementation-defined public members follow
    ...

  private:
    // implementation-defined state follows
    ...
};

void swap(parallel_execution_policy &a, parallel_execution_policy &b);

template<> struct is_execution_policy<parallel_execution_policy> : true_type {};

extern const parallel_execution_policy par;

class sequential_execution_policy
{
  public:
    void swap(sequential_execution_policy &other);

    // implementation-defined public members follow
    ...

  private:
    // implementation-defined state follows
    ...
};

void swap(sequential_execution_policy &a, sequential_execution_policy &b);

template<> struct is_execution_policy<sequential_execution_policy> : true_type {};

extern const sequential_execution_policy seq;

class vector_execution_policy
{
  public:
    void swap(vector_execution_policy &other);

    // implementation-defined public members follow
    ...

  private:
    // implementation-defined state follows
    ...
};

void swap(vector_execution_policy &a, vector_execution_policy &b);

template<> struct is_execution_policy<vector_execution_policy> : true_type {};

extern const vector_execution_policy vec;

// implementation-defined execution policy extensions follow
...

class execution_policy
{
  public:
    template<class ExecutionPolicy>
    execution_policy(const ExecutionPolicy &exec,
                     typename enable_if<
                       is_execution_policy<ExecutionPolicy>::value
                     >::type * = 0);

    template<class ExecutionPolicy>
    typename enable_if<
      is_execution_policy<ExecutionPolicy>::value,
      execution_policy &
    >::type
    operator=(const ExecutionPolicy &exec);

    void swap(execution_policy &other);

    // obtains the typeid of the stored target
    const type_info& target_type() const;

    // obtains a pointer to the stored target
    template<class ExecutionPolicy>
    typename enable_if<
      is_execution_policy<ExecutionPolicy>::value,
      ExecutionPolicy *
    >::type
    target();

    template<class ExecutionPolicy>
    typename enable_if<
      is_execution_policy<ExecutionPolicy>::value,
      const ExecutionPolicy *
    >::type
    target() const;

  private:
    ...
};

void swap(execution_policy &a, execution_policy &b);

}
```

Class template `is_execution_policy`
================================

```
namespace std {
  template<class T> struct is_execution_policy
    : integral_constant<bool, see below> { };
}
```

1. `is_execution_policy` can be used to detect parallel execution policies for the purpose of excluding parallel algorithm signatures from otherwise ambiguous overload resolution participation.

2. If `T` is the type of a standard or implementation-defined non-standard execution policy, `is_execution_policy<T>` shall be publicly derived from `integral_constant<bool,true>`,
otherwise from `integral_constant<bool,false>`.

3. The effect of specializing `is_execution_policy` for a type which is not defined by library is unspecified.

    [*Note:* This provision reserves the privilege of creating non-standard execution policies to the library implementation. -- *end note*.]

Class `execution_policy`
========================

1. Objects of type `execution_policy` may be used to dynamically control the invocation of parallel algorithms.

Execution Policies and Their Effect on Algorithm Execution
==========================================================

Execution policies describe the manner in which the algorithms apply the user-provided function objects.

1. The applications of the function objects in the algorithms invoked with the `sequential_execution_policy` execute in sequential order in the calling thread.

2. The applications of the function objects in the algorithms invoked with the `parallel_execution_policy` are permitted to execute in an unordered fashion in unspecified threads, or indeterminately sequenced if executed on one thread. 
   [*Note:* It is the caller's responsibility to ensure correctness, for example that the invocation does not introduce data races or deadlocks. –- *end note*] [*Example:*

    ```
    int a[] = {0,1};
    std::vector<int> v;
    std::for_each(std::par, std::begin(a), std::end(a), [&](int i) {
        v.push_back(i*2+1);
    });
    ```

    The program above has a data race because of the unsynchronized access to the container `v` -– *end example*] [*Example:*

    ```
    std::atomic<int> x = 0;
    int a[] = {1,2};
    std::for_each(std::par , std::begin(a), std::end(a), [](int n) {
        x.fetch_add( 1 , std::memory_order_relaxed );
        // spin wait for another iteration to change the value of x
        while( x.load( std::memory_order_relaxed ) == 1 )
            ;
    });
    ```

    The above example depends on the order of execution of the iterations, and is therefore undefined (may deadlock). -– *end example*]
    [*Example:*

    ```
    int x;
    std::mutex m;
    int a[] = {1,2};
    std::for_each( std::par , std::begin(a), std::end(a), [&](int) {
        m.lock();
        ++x;
        m.unlock();
    });
    ```
    
    The above example synchronizes access to object `x` ensuring that it is incremented correctly. –- *end example*]

3. The applications of the function objects in the algorithms invoked with `vector_execution_policy` are permitted to execute in an unordered fashion in unspecified threads, or unsequenced if executed on one thread. [*Note:* as a consequence, function objects governed by the `vector_execution_policy` policy must not synchronize with each other. Specifically, they must not acquire locks. –- *end note*]
    [*Example:*

    ```
    int x;
    std::mutex m;
    int a[] = {1,2};
    std::for_each( std::vec , std::begin(a), std::end(a), [&](int) {
        m.lock();
        ++x;
        m.unlock();
    });
    ```

    The above program is invalid because the applications of the function object are not guaranteed to run on different threads.
    [*Note:* the application of the function object may result in two consecutive calls to `m.lock` on the same thread, which may deadlock -– *end note*] -– *end example*]

    [*Note:* The semantics of a `parallel_execution_policy` or `vector_execution_policy` invocation allow the implementation to fall back to sequential execution if the system cannot parallelize an algorithm invocation due to lack of resources. -- *end note*.]

4. An implementation may provide additional execution policy types besides `parallel_execution_policy`, `sequential_execution_policy`, `vector_execution_policy`, or `execution_policy`. Objects of type `execution_policy` must be constructible and assignable from any additional non-standard execution policy provided by the implementation.

5. Algorithms invoked with an execution policy argument of type `execution_policy` execute internally as if invoked with a `sequential_execution_policy`, a `parallel_execution_policy`, or a non-standard implementation-defined execution policy depending on the dynamic value of the `execution_policy` object.

6. Algorithms invoked without an execution policy argument execute as if they were invoked with an execution policy argument of type `sequential_execution_policy`.

7. Implementations of `sequential_execution_policy`, `parallel_execution_policy`, and `vector_execution_policy` are permitted to provide additional non-standard data and function members.

    [*Note:* This provision permits objects of these types to be stateful. -- *end note*.]

Example Usage of `execution_policy`:
====================================

`std::execution_policy` allows us to dynamically control algorithm execution:

    std::vector<float> sort_me = ...
    
    std::execution_policy exec = std::seq;
    
    if(sort_me.size() > threshold)
    {
      exec = std::par;
    }
    
    std::sort(exec, sort_me.begin(), sort_me.end());

`std::execution_policy` allows us to pass execution policies through a binary interface:

    void some_api(std::execution_policy exec, int arg1, double arg2);

    void foo()
    {
      // call some_api with std::par
      some_api(std::par, 7, 3.14);
    }

Retrieving the dynamic value from an `std::execution_policy` an API similar to `std::function`:

    void some_api(std::execution_policy exec, int arg1, double arg2)
    {
      if(exec.target_type() == typeid(std::seq))
      {
        std::cout << "Received a sequential policy" << std::endl;
        std::sequential_execution_policy *exec_ptr = exec.target<std::sequential_execution_policy>();
      }
      else if(exec.target_type() == typeid(std::par))
      {
        std::cout << "Received a parallel policy" << std::endl;
        std::parallel_execution_policy *exec_ptr = exec.target<std::parallel_execution_policy>();
      }
      else if(exec.target_type() == typeid(std::vec))
      {
        std::cout << "Received a vector policy" << std::endl;
        std::vector_execution_policy *exec_ptr = exec.target<std::vector_execution_policy>();
      }
      else
      {
        std::cout << "Received some other kind of policy" << std::eedl;
      }
    }

In the current design, `std::execution_policy::target` returns a pointer similar to `std::function::target`. However, `std::execution_policy`'s current design precludes an "empty" or invalid state.
An alternative design might require `std::execution_policy::target` to return a reference and throw an exception in the case of type mismatch.

Reporting Exceptions
====================

Exception behavior of functions launched through a `std::sequential_execution_policy` argument
----------------------------------------------------------------------------------------------

An algorithm invoked with a sequential execution policy reports exceptional behavior in the same manner as the legacy algorithms.

Exception behavior of functions launched through a `std::parallel_execution_policy` argument
--------------------------------------------------------------------------------------------

An algorithm invoked with a parallel execution policy may report exceptional behavior by throwing an exception.

A parallel algorithm may report exceptional behavior to the caller by throwing one of three exception types:

  * If temporary memory resources are required by the algorithm and none are available, the algorithm may throw `std::bad_alloc`.
  * If parallel resources are required by the algorithm and none are available, the algorithm may throw `std::system_error`.
  * If one or more uncaught exceptions are thrown for any other reason during the execution of the algorithm:
    * The exception is collected in an `exception_list` associated with the algorithm's invocation.
    * If the `exception_list` associated with the algorithm's invocation is non-empty, it is thrown once all tasks have terminated.

Note that these guarantees imply that all exceptions thrown during the execution of the algorithm are communicated to the caller. It is unspecified
whether an algorithm implementation will "forge ahead" after encountering and capturing a user exception.

Exception behavior of functions launched through a `std::vector_execution_policy` argument
------------------------------------------------------------------------------------------

If program-defined code invoked by an algorithm invoked with a vector execution policy throws an exception, the behavior is undefined.


Header `<exception>` synopsis

```
namespace std {
  class exception_list
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
  
      const char *what() const;
  
    private:
      std::list<exception_ptr> exceptions_; // exposition only
  };
}
```

Interaction with Threads
========================

For algorithms invoked with a `sequential_execution_policy` argument:

1. Program-defined functions arising from the manipulation of algorithm parameters (e.g., functors, iterators, etc.) are invoked in the calling thread of execution.

2. Program-defined functions arising from the manipulation of algorithm parameters are invoked in sequential order.

3. Algorithms synchronize with the caller.

For algorithms invoked with a `parallel_execution_policy` argument:

1. It is the responsibility of the algorithm implementation to allocate temporary storage in a thread-safe manner.

    [*Note:* One way to guarantee this is to allocate all dynamic storage in the calling thread before concurrent computation begins. -- *end note*.]

2. It is unspecified in which thread(s) program-defined functions arising from the manipulation of algorithm parameters are invoked.

3. It is unspecified in which order program-defined functions arising from the manipulation of algorithm parameters are invoked.

4. Algorithm invocations synchronize with the caller.

5. Algorithms make no guarantee of forward progress.

    [*Note:* Program-defined code executed by an algorithm's invocation of a functor or iterator dereference cannot wait on the result of another invocation. -- *end note*.]

6. An algorithm's behavior is undefined if program-defined code executed through algorithm parameter manipulation may introduce a data race.

For algorithms invoked with a `vector_execution_policy` argument:

1. A vectorizable algorithm invocation inherits all the previous restrictions of `parallel_execution_policy`.

2. An algorithm's behavior is undefined if program-defined code executed through algorithm parameter manipulation throws an exception.

3. An algorithm's behavior is undefined if program-defined code executed through algorithm parameter mainpulation attempts to synchronize.

Proposed Additions to Algorithms and Algorithm-like functions
=============================================================

Here we enumerate the additions we propose to the standard algorithms library.

We propose to overload an existing algorithm name when the existing C++11 specification allows sufficient discretion for a parallel implementation (e.g., `transform`) or when we feel no other name would be appropriate (e.g., `for_each`, `inner_product`).

We propose to introduce a new algorithm name when the existing analogous algorithm name implies a sequential implementation (e.g., `accumulate` versus `reduce`).

