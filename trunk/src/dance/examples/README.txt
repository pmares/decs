
==========
DLX Matrix Examples
==========


Introduction
----------

The files in this folder can be used together with the dance program. They
contain a boolean matrix representation of a specific exact cover problem. Given
enough time the Dancing Links algorithm used by the dance program will be able
to find all the solutions to solve these problem. The larger matrices require
more memory and they usually take longer to solve so you might want to start
with one of the smaller ones first.

The information lists how many columns and rows the matrix has along with the
number of non-zero elements (elements with the value 1). The density is a
measure of how much of the matrix is filled with non-zero elements. The number
of solutions is listed (if it is known). For some of the smaller matrices the
solutions may also be listed. Each of the solutions are listed in brackets
containing their row numbers.


micro.decs
----------
Columns   : 4
Rows      : 6
Elements  : 9
Density   : 37.5%
Solutions : 3 [0 3 4] [1 2] [2 4 5]

A very small and simple matrix used to test the dance program.


tiny.decs
----------
Columns   : 6
Rows      : 10
Elements  : 26
Density   : 43.3%
Solutions : 2 [1 7] [6 5]

A very simple matrix used for testing the dance program.
