---------------------------

Appendix: Design Notes and Outstanding Questions
================================================

This appendix outlines the rationale behind some of our design choices
and identifies outstanding questions which may require further work to
resolve. 

## Rejected Naming Schemes

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

### A `parallel_` Name Prefix

A simple way to disambiguate parallel algorithms from their sequential versions
would be simply to give them new, unique names. Indeed, this is the approach suggested
by Intel & Microsoft's earlier paper N3429 and is the one taken in their libraries
(i.e., Threading Building Blocks & Parallel Patterns Library, respectively). It
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

### A Nested `std::parallel` Namespace

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

