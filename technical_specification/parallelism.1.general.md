# General {#parallel.general}

## Scope {#parallel.general.scope}

This Technical Specification describes requirements for implementations of an
interface that computer programs written in the C++ programming language may
use to invoke algorithms with parallel execution. The algorithms described by
this Technical Specification are realizable across a broad class of
computer architectures.

This Technical Specification is non-normative. Some of the functionality
described by this Technical Specification may be considered for standardization
in a future version of C++, but it is not currently part of any C++ standard.
Some of the functionality in this Technical Specification may never be
standardized, and other functionality may be standardized in a substantially
changed form.

The goal of this Technical Specification is to build widespread existing
practice for parallelism in the C++ standard algorithms library. It gives
advice on extensions to those vendors who wish to provide them.

## Normative references {#parallel.general.references}

The following reference document is indepensible for the application of this
document. For dated references, only the edition cited applies. For undated
references, the latest edition of the referenced document (including any
amendments) applies.

* ISO/IEC 14882:2011, Programming Languages -- C++

ISO/IEC 14882:2011 is herein called the C++ Standard. The library described in
ISO/IEC 14882:2011 clauses 17-30 is herein called the C++ Standard Library. The
C++ Standard Library components described in ISO/IEC 14882:2011 clauses 25 and
26.7 are herein called the C++ Standard Algorithms Library.

Unless otherwise specified, the whole of the C++ Standard Library introduction
[lib.library] is included into this Technical Specification by reference.

## Namespaces and headers {#parallel.general.namespaces}

Since the extensions described in this Technical Specification are
experimental and not part of the C++ Standard Library, they should not be
declared directly within namespace `std`. Unless otherwise specified, all
components described in this Technical Specification are declared in namespace
`std::experimental::parallel`.

[*Note:* Once standardized, the components described by this Technical Specification
are expected to be promoted to namespace `std`. -- *end note*]

Unless otherwise specified, references to other entities described in this
Technical Specification are assumed to be qualified with
`std::experimental::parallel`, and references to entities described in the
C++ Standard Library are assumed to be qualified with `std::`.

Extensions that are expected to eventually be added to an existing header
`<meow>` are provided inside the `<experimental/meow>` header, which shall
include the standard contents of `<meow>` as if by

    #include <meow>

## Terms and definitions {#parallel.general.defns}

For the purposes of this document, the terms and definitions given in the C++
Standard and the following apply.

A *parallel algorithm* is a function template described by this Technical Specification declared in namespace `std::experimental::parallel` with a formal template parameter named `ExecutionPolicy`.

Parallel algorithms can access objects indirectly accessible via
its arguments or via elements of its container by invoking the following
functions:

* Functions on those container elements that are required by its specification.
* User-provided function objects to be applied during the execution of the 
algorithm, if required by the specification.

These functions are herein called the *element access functions* of the parallel
algorithms.

[*Example:* The `sort` function may invoke the following element access 
functions:

* The `swap` function on the elements of the container (as per 25.4.1.1/2)
* The user-provided provided `Compare` function object. -- *end example*]

# Wording changes {#parallel.wording}

The wording changes proposed in this section are relative to the contents of
N3936.

In 17.6.5.9, change paragraph 8 as follows:

\remvrbline[Unless otherwise specified, C++ standard library functions shall
perform all operations solely within the current thread if those operations have
effects that are visible (1.10) to users] 

\addvrbline[A C++ standard library function with execution policy `vec` or `par`
shall not introduce a race on any object accessible by the means described in
clauses 2-5.  Otherwise, unless otherwise specified, C++ standard library
functions shall perform all operations solely within the current thread if
those operations have effects that are visible (1.10) to users.]

