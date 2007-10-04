
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

The information below lists how many columns, rows and non-zero elements the
matrix has along with it's density. Density is a measure of how large a
percentage of the matrix is filled with non-zero elements. The number of
solutions is listed (if it is known). For some of the smaller matrices the
individual solutions may also be listed. Each of the solutions are listed in
brackets containing their row numbers. The column number sometimes seen in
parenthesis is the number of secondary columns. This number can never be larger
than the number of columns. If a matrix has secondary columns it means that it
is a general cover problem in contrast to exact cover problems which have no


CRED format: columns x rows (secondary columns), non-zero elements, density

micro.decs
----------
CRED: 4x6, 9, 37.5%
Solutions: 3 [0 3 4] [1 2] [2 4 5]

A very small and simple matrix used to test the dance program.


tiny.decs
----------
CRED: 6x10, 26, 43.3%
Solutions: 2 [1 7] [6 5]

A very simple matrix used for testing the dance program.


queens[2-26].decs
----------
CRED: ?x?, ?, ?
Solutions: 0->?

The n queens problem matrices for n=2 to 26. n=26 has yet to be solved. The
density rapidly decreases with increasing values for n, and for n=8 it's 0.9%
The list below shows the number of solutions for a given n.

 n	   	solutions
 1                   1
 2                   0
 3                   0
 4                   2
 5                  10
 6                   4
 7                  40
 8                  92
 9                 352
10                 724
11                2680
12               14200
13               73712
14              365596
15             2279184
16            14772512
17            95815104
18           666090624
19          4968057848
20         39029188884
21        314666222712
22       2691008701644
23      24233937684440
24     227514171973736
25    2207893435808352

