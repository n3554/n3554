% A Parallel Algorithms Library | N3554
% Jared Hoberock; Jaydeep Marathe; Michael Garland; Olivier Giroux; Vinod Grover
  {jhoberock, jmarathe, mgarland, ogiroux, vgrover}@nvidia.com
  Artur Laksberg; Herb Sutter
  {arturl, hsutter}@microsoft.com
  Arch Robison
  arch.robison@intel.com
% 2013-03-15

Overview
========

We propose an extension of the C++ standard library that provides access
to parallel execution for a broad range of algorithms.  Many of these
algorithms correspond with algorithms already in the standard library,
while some are novel.  Our proposal is a pure extension, as it does not
alter the meaning of any existing functionality.  Our goal in this
proposal is to provide access to the performance benefits of parallelism
in a way that (1) can be easily adopted by programmers and (2) can be
supported efficiently on the broadest possible range of hardware
platforms.

We identify a collection of algorithms that permit efficient parallel
implementations.  We also introduce the concept of an *execution policy*,
that may be used to specify how these algorithms should be executed.
Execution policies become an optional parameter to a standard set of
algorithms, permitting the programmer to write code such as the
following:

    std::vector<int> vec = ...

    // previous standard sequential sort
    std::sort(vec.begin(), vec.end());                  

    // explicitly sequential sort
    std::sort(std::seq, vec.begin(), vec.end());        

    // permitting parallel execution
    std::sort(std::par, vec.begin(), vec.end());        

    // permitting vectorization as well
    std::sort(std::vec, vec.begin(), vec.end());

    // sort with dynamically-selected execution
    size_t threshold = ...
    std::execution_policy exec = std::seq;
    if(vec.size() > threshold)
    {
        exec = std::par;
    }

    std::sort(exec, vec.begin(), vec.end());


## Execution policies

In this proposal, we define three standard execution policies:
`std::seq`, `std::par`, and `std::vec`, as well as a facility for
vendors to provide non-standard policies as extensions.  The `std::seq`
policy requires that the called algorithm execute in sequential order on
the calling thread.  The other policies indicate that some form of
parallel execution is permitted.  Even when parallel execution is
possible, it is *never mandatory*.  An implementation is always
permitted to fallback to sequential execution.

By using `std::par` or `std::vec` a program simultaneously requests
parallel execution and indicates the manner in which the implementation
is allowed to apply user-provided function objects.  The `std::par`
policy indicates that function objects invoked by the algorithm may be
executed in an unordered fashion in unspecified threads, or
indeterminately sequenced if executed on one thread.  The `std::vec`
policy indicates that these function objects may execute in an unordered
fashion in unspecified threads, or unsequenced if executed on one
thread.  Complete details on these definitions are provided in the
section of this paper on *[Effect of policies on algorithm execution][]*.

We have designed the standard policies to be meaningful on the broadest
possible range of platforms.  Since programs based strictly on the
standard must be portable, our execution policies carefully avoid
platform-specific details that would tie a program too closely to a
particular implementation.  Our definition of `std::par` is intended to
permit an implementation to safely execute an algorithm across
potentially many threads.  The more restrictive `std::vec` policy is
intended to additionally permit vectorization within the implementation.

In addition to these standard policies, our proposal is designed to
permit individual implementations to provide additional non-standard
policies that might provide further means of controlling the execution
of algorithms.

    // possible non-standard, implementation-specific policies
    std::sort(vectorize_in_this_thread, vec.begin(), vec.end());
    std::sort(submit_to_my_thread_pool, vec.begin(), vec.end());
    std::sort(execute_on_that_gpu, vec.begin(), vec.end());
    std::sort(offload_to_my_fpga, vec.begin(), vec.end());
    std::sort(send_this_computation_to_the_cloud, vec.begin(), vec.end());

## Algorithms

We overload an existing algorithm name when the existing
C++11 specification allows sufficient discretion for a parallel
implementation (e.g., `transform`) or when we feel no other name would
be appropriate (e.g., `for_each`, `inner_product`).

* Section 3: [Overloads of Existing Algorithms Introduced by this Proposal][]

We propose to introduce a new algorithm name when the existing analogous
algorithm name implies a sequential implementation (e.g., `accumulate`
versus `reduce`).

* Section 4: [Novel Algorithms Introduced by this Proposal][]

Finally, we avoid defining any new functionality for algorithms that are
by nature sequential and hence do not permit a parallel implementation.

* Section 5: [Existing Algorithms Left Unchanged by this Proposal][]


---------------------------

Parallel execution of algorithms
================================

In this section, we describe our proposed model of parallel execution.
Later sections describe all the algorithms for which we propose
to permit parallel execution.


## Model of parallelism

As described above, our goal is to make parallel execution easily
available across the broadest possible range of platforms.  For this
reason, we have exposed parallelism in the most abstract manner possible
in order to avoid presuming the existence of a particular parallel
machine model. In particular, we have intentionally avoided a
specification which would be required to introduce concurrency by
creating threads.  We have also carefully avoided making parallel
execution mandatory; algorithm implementations are always permitted to
opt for sequential execution regardless of policy.

This design provides an approachable model of parallelism that should
feel familiar to any user of the STL.  However, its limitations mean
that it is only a partial solution to the problem of providing
parallelism to C++ programmers.  We expect our library to coexist in an
ecosystem of standard language and library constructs which target
parallelism at varying levels of abstraction.

### Composition with scheduling

Our proposed parallel execution policies, `std::par` and `std::vec`,
specify how an implementation is allowed to execute user-provided
function objects.  In other words, they specify *what* parallel work an
implementation can create, but they do not specify the orthogonal
concern of *where* this work should be executed.

Our proposal is designed with the expectation that the C++ standard will
adopt some standard mechanism (e.g., executors or task schedulers) that
provide a way for programs to manage the question of *where* parallel
work will be performed.  To accommodate this direction, we anticipate
that our policies could be extended to accept an additional argument
specifying an object whose responsibility it is to control the placement
and scheduling of work.  This ability is necessary for scheduling
decisions made within algorithm implementations to compose well with
scheduling decisions made within the surrounding application.

### Composition across algorithms

One limitation of STL-like algorithms is that they encourage the programmer to
engage in a style of programming which may be an obstacle to achieving maximum
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
reduced significantly.  Our experience with previous implementations,
such as Thrust, shows that such iterator facilities can be quite
valuable.  However, because this idea is orthogonal to the idea of
parallel algorithms, this proposal does not include a novel iterator
library.

## Execution policy definitions

The execution policies `std::seq`, `std::par` and `std::vec` are defined as global instances of types
`std::sequential_execution_policy`, `std::parallel_execution_policy` and `std::vector_execution_policy`.

These types are defined in the header `<execution_policy>` as follows:

```
namespace std
{

template<class T> struct is_execution_policy;

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
```

### Class template `is_execution_policy`

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

### Class `execution_policy`

Objects of type `execution_policy` may be used to dynamically control the invocation of parallel algorithms. The type is
defined in the header `<execution_policy>` as follows:

```
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

### Example Usage of `execution_policy`

`std::execution_policy` allows dynamic control over algorithm execution:

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
        std::cout << "Received some other kind of policy" << std::endl;
      }
    }

In the current design, `std::execution_policy::target` returns a pointer similar to `std::function::target`. However, `std::execution_policy`'s current design precludes an "empty" or invalid state.
An alternative design might require `std::execution_policy::target` to return a reference and throw an exception in the case of type mismatch.


## Effect of policies on algorithm execution

Execution policies describe the manner in which standard algorithms apply user-provided function objects.

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

3. The applications of the function objects in the algorithms invoked with the `vector_execution_policy` are permitted to execute in an unordered fashion in unspecified threads, or unsequenced if executed on one thread. [*Note:* as a consequence, function objects governed by the `vector_execution_policy` policy must not synchronize with each other. Specifically, they must not acquire locks. –- *end note*]
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

    [*Note:* The semantics of the `parallel_execution_policy` or the `vector_execution_policy` invocation allow the implementation to fall back to sequential execution if the system cannot parallelize an algorithm invocation due to lack of resources. -- *end note*.]

4. If they exist, an algorithm invoked with the `parallel_execution_policy` or the `vector_execution_policy` may apply iterator member functions of a stronger category than its specification requires. In this case,
   the application of these member functions are subject to provisions 2. and 3. above, respectively.

    [*Note:* For example, an algorithm whose specification requires `InputIterator` but receives a concrete iterator of the category `RandomAccessIterator` may use `operator[]`. In this
    case, it is the algorithm caller's responsibility to ensure `operator[]` is race-free. -- *end note*.]

5. An implementation may provide additional execution policy types besides `parallel_execution_policy`, `sequential_execution_policy`, `vector_execution_policy`, or `execution_policy`. Objects of type `execution_policy` must be constructible and assignable from any additional non-standard execution policy provided by the implementation.

6. Algorithms invoked with an execution policy argument of type `execution_policy` execute internally as if invoked with instances of type `sequential_execution_policy`, `parallel_execution_policy`, or a non-standard implementation-defined execution policy depending on the dynamic value of the `execution_policy` object.

7. Implementations of types `sequential_execution_policy`, `parallel_execution_policy`, and `vector_execution_policy` are permitted to provide additional non-standard data and function members.

    [*Note:* This provision permits objects of these types to be stateful. -- *end note*.]

## Example Usage of `execution_policy`:

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

## Exception reporting behavior

An algorithm invoked with a sequential or parallel execution policy may report exceptional behavior by throwing an exception.

If program-defined code invoked by an algorithm invoked with a vector execution policy throws an exception, the behavior is undefined.

An algorithm may report exceptional behavior to the caller by throwing one of two exception types:

  * If temporary memory resources are required by the algorithm and none are available, the algorithm may throw `std::bad_alloc`.
  * If one or more uncaught exceptions are thrown for any other reason during the execution of the algorithm:
    * The exception is collected in an `exception_list` associated with the algorithm's invocation.
    * If the `exception_list` associated with the algorithm's invocation is non-empty, it is thrown once all tasks have terminated.

When an exception is thrown during the application of the user-provided function object, the algorithm throws an `exception_list` exception. 
Every evaluation of the user-provided function object must finish before the `exception_list exception` is thrown. Therefore, all exceptions 
thrown during the application of the user-provided function objects are contained in the `exception_list`, however the number of such exceptions is 
unspecified. [*Note:* For example, the number of invocations of the user-provide function object in `std::for_each` is unspecified. When 
`std::for_each` is executed serially, only one exception will be contained in the `exception_list` object -- *end note*]

[*Note:* These guarantees imply that all exceptions thrown during the execution of the algorithm are communicated to the caller. It is 
unspecified whether an algorithm implementation will "forge ahead" after encountering and capturing a user exception. -- *end note*]



Header `<exception>` synopsis

```
namespace std {
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
```

Proposed Additions to Algorithms and Algorithm-like functions
=============================================================

Here we enumerate the additions we propose to the standard algorithms library.

We propose to overload an existing algorithm name when the existing C++11 specification allows sufficient discretion for a parallel implementation (e.g., `transform`) or when we feel no other name would be appropriate (e.g., `for_each`, `inner_product`).

We propose to introduce a new algorithm name when the existing analogous algorithm name implies a sequential implementation (e.g., `accumulate` versus `reduce`).

