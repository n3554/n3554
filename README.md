n3554
=====

A proposal for a standard parallel algorithms library for ISO C++.

Building the PDF:

    $ pandoc --number-sections -H n3554_header.tex n3554_a_parallel_algorithms_library.*.md -o n3554_a_parallel_algorithms_library.pdf

Just generating LaTeX (you can edit the `.tex` and then `pdflatex` the output):

    $ pandoc --number-sections -H n3554_header.tex n3554_a_parallel_algorithms_library.*.md -o n3554_a_parallel_algorithms_library.tex

[Get `pandoc` here.](http://johnmacfarlane.net/pandoc/installing.html)

Building the examples:

    $ g++ -std=c++11 -Iinclude -fopenmp reverse.cpp for_each.cpp externs.cpp -o for_each

