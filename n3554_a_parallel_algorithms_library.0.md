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

We introduce a library of algorithms with parallel execution semantics.
Some of these algorithms have semantics similar to the existing standard
library of sequential algorithms. Some of the algorithms we propose are
novel.

We introduce three parallel execution policies for parallel algorithm
execution: `std::seq`, `std::par`, and `std::vec`, as well as a facility
for vendors to provide non-standard execution policies as extensions.
These policy objects may be used to specify how a parallel algorithm
should be executed:

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

In addition to these standard policies, our proposal is designed to
permit individual implementations to provide additional non-standard
policies that provide further means of controlling the execution of
algorithms.

    // parallel sort with non-standard implementation-provided execution policies:
    std::sort(vectorize_in_this_thread, vec.begin(), vec.end());
    std::sort(submit_to_my_thread_pool, vec.begin(), vec.end());
    std::sort(execute_on_that_gpu, vec.begin(), vec.end());
    std::sort(offload_to_my_fpga, vec.begin(), vec.end());
    std::sort(send_this_computation_to_the_cloud, vec.begin(), vec.end());

## Execution policies

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

## Algorithms

This proposal is a pure addition to the existing C++ standard library; we do not believe it alters the semantics of any existing functionality.

We propose to overload an existing algorithm name when the existing C++11 specification allows sufficient discretion for a parallel implementation (e.g., `transform`) or when we feel no other name would be appropriate (e.g., `for_each`, `inner_product`).

We propose to introduce a new algorithm name when the existing analogous algorithm name implies a sequential implementation (e.g., `accumulate` versus `reduce`).

Finally, we avoid defining any new functionality for algorithms that are
by nature sequential and hence do not permit a parallel implementation.

Detailed specifications for each of these three classes of algorithms
can be found in the following sections:

* Section 3: [Overloads of Existing Algorithms Introduced by this Proposal][]
* Section 4: [Novel Algorithms Introduced by this Proposal][]
* Section 5: [Existing Algorithms Left Unchanged by this Proposal][]

---------------------------

Parallel execution of algorithms
================================




## Execution policy definitions

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

1. Objects of type `execution_policy` may be used to dynamically control the invocation of parallel algorithms.

### Example Usage of `execution_policy`

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


## Effect of policies on algorithm execution

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

6. Implementations of `sequential_execution_policy`, `parallel_execution_policy`, and `vector_execution_policy` are permitted to provide additional non-standard data and function members.

    [*Note:* This provision permits objects of these types to be stateful. -- *end note*.]


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
unspecified. [*Note:* for example, the number of invocations of the user-provide function object in `std::for_each` is unspecified. When 
`std::for_each` is executed serially, only one exception will be contained in the exception_list object -- *end note*]

[*Note:* that these guarantees imply that all exceptions thrown during the execution of the algorithm are communicated to the caller. It is 
unspecified whether an algorithm implementation will "forge ahead" after encountering and capturing a user exception. -- *end note*]



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

## Interaction with threads

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

