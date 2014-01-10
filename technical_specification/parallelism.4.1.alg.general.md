# Parallel algorithms {#alg}

## In general {#alg.general}

This clause describes components that C++ programs may use to perform operations on containers and other sequences in parallel.

### Effect of execution policies on parallel algorithm execution {#alg.general.exec}

1. Parallel algorithms accept execution policy parameters which describe the manner in which they apply user-provided function objects.

2. The applications of function objects in parallel algorithms invoked with an execution policy object of type `sequential_execution_policy` execute in sequential order in the calling thread.

3. The applications of function objects in parallel algorithms invoked with an execution policy object of type `parallel_execution_policy` are permitted to execute in an unordered fashion in unspecified threads, or indeterminately sequenced if executed on one thread. 
   [*Note:* It is the caller's responsibility to ensure correctness, for example that the invocation does not introduce data races or deadlocks. –- *end note*]
   
     [*Example:*

         using namespace std::experimental::parallel;
         int a[] = {0,1};
         std::vector<int> v;
         for_each(par, std::begin(a), std::end(a), [&](int i) {
           v.push_back(i*2+1);
         });

     The program above has a data race because of the unsynchronized access to the container `v` -– *end example*]
   
     [*Example:*

         using namespace std::experimental::parallel;
         std::atomic<int> x = 0;
         int a[] = {1,2};
         for_each(par , std::begin(a), std::end(a), [](int n) {
           x.fetch_add(1 , std::memory_order_relaxed);
           // spin wait for another iteration to change the value of x
           while(x.load(std::memory_order_relaxed) == 1);
         });

     The above example depends on the order of execution of the iterations, and is therefore undefined (may deadlock). -– *end example*]

     [*Example:*

         using namespace std::experimental::parallel;
         int x;
         std::mutex m;
         int a[] = {1,2};
         for_each(par, std::begin(a), std::end(a), [&](int) {
           m.lock();
           ++x;
           m.unlock();
         });
    
     The above example synchronizes access to object `x` ensuring that it is incremented correctly. –- *end example*]

4. The applications of function objects in parallel algorithms invoked with an execution policy of type `vector_execution_policy` are permitted to execute in an unordered fashion in unspecified threads, or unsequenced if executed on one thread. [*Note:* as a consequence, function objects governed by the `vector_execution_policy` policy must not synchronize with each other. Specifically, they must not acquire locks. –- *end note*]

    [*Example:*

        using namespace std::experimental::parallel;
        int x;
        std::mutex m;
        int a[] = {1,2};
        for_each(vec, std::begin(a), std::end(a), [&](int) {
          m.lock();
          ++x;
          m.unlock();
        });
   

    The above program is invalid because the applications of the function object are not guaranteed to run on different threads.

    [*Note:* the application of the function object may result in two consecutive calls to `m.lock` on the same thread, which may deadlock -– *end note*]
    
    -- *end example*]

    [*Note:* The semantics of the `parallel_execution_policy` or the `vector_execution_policy` invocation allow the implementation to fall back to sequential execution if the system cannot parallelize an algorithm invocation due to lack of resources. -- *end note*.]

5. If they exist, a parallel algorithm invoked with an execution policy object of type `parallel_execution_policy` or `vector_execution_policy` may apply iterator member functions of a stronger category than its specification requires. In this case,
   the application of these member functions are subject to provisions 3. and 4. above, respectively.

    [*Note:* For example, an algorithm whose specification requires `InputIterator` but receives a concrete iterator of the category `RandomAccessIterator` may use `operator[]`. In this
    case, it is the algorithm caller's responsibility to ensure `operator[]` is race-free. -- *end note*.]

6. Algorithms invoked with an execution policy object of type `execution_policy` execute internally as if invoked with instances of type `sequential_execution_policy`, `parallel_execution_policy`, or a non-standard implementation-defined execution policy depending on the dynamic value of the `execution_policy` object.

7. The semantics of parallel algorithms invoked with an execution policy object of type other than those described by this Technical Specification are unspecified.

