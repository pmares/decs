
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

The information below lists how many rows, columns and non-zero elements the
matrix has along with it's density. Density is a measure of how large a
percentage of the matrix is filled with non-zero elements. The number of
solutions is listed (if it is known). For some of the smaller matrices the
individual solutions may also be listed. Each of the solutions are listed in
curly brackets containing their row numbers. The column number sometimes seen in
parenthesis is the number of secondary columns. This number can never be larger
than the number of columns. If a matrix has secondary columns it means that it
is a general cover problem in contrast to exact cover problems which have no


RCED format: rows x columns (secondary columns), non-zero elements, density


knuthx.decs
----------
RCED: 6x7, 16, 38.1%
Solutions: 1 {0 3 4}

This is the example matrix used by Donald E. Knuth in his "Dancing Links" paper.
The matrix is shown in (3) and figure 2, 3 and 4 in the paper.


micro.decs
----------
RCED: 6x4, 9, 37.5%
Solutions: 3 {0 3 4}, {1 2}, {2 4 5}

A very small and simple matrix used to test the dance program. This is the same
matrix used in the "General Distributed Exact Cover Solver" report by Jan Magne
Tjensvold.


tiny.decs
----------
RCED: 10x6, 26, 43.3%
Solutions: 2 {1 7}, {5 6}

A very simple matrix used for testing the dance program.


queens[2-26].decs
----------
RCED: See table below.
Solutions: 0->?

The n queens problem matrices for n=2 to 26. n=26 has yet to be solved. The
density rapidly decreases with increasing values for n, and for n=8 it's 9.38%
The list below shows the number of solutions for a given n as well as RCED.

 n           solutions    Rows, columns, elements, density
 1                   1    
 2                   0      4 x 6    (2),   12, 50%
 3                   0      9 x 12   (6),   32, 29.63%
 4                   2     16 x 18  (10),   60, 20.83%
 5                  10     25 x 24  (14),   96, 16%
 6                   4     36 x 30  (18),  140, 12.96%
 7                  40     49 x 36  (22),  192, 10.88%
 8                  92     64 x 42  (26),  252,  9.38%
 9                 352     81 x 48  (30),  320,  8.23%
10                 724    100 x 54  (34),  396,  7.33%
11                2680    121 x 60  (38),  480,  6.61%
12               14200    144 x 66  (42),  572,  6.02%
13               73712    169 x 72  (46),  672,  5.52%
14              365596    196 x 78  (50),  780,  5.10%
15             2279184    225 x 84  (54),  896,  4.74%
16            14772512    256 x 90  (58), 1020,  4.43%
17            95815104    289 x 96  (62), 1152,  4.15%
18           666090624    324 x 102 (66), 1292,  3.91%
19          4968057848    361 x 108 (70), 1440,  3.69%
20         39029188884    400 x 114 (74), 1596,  3.5%
21        314666222712    441 x 120 (78), 1760,  3.33%
22       2691008701644    484 x 126 (82), 1932,  3.17%
23      24233937684440    529 x 132 (86), 2112,  3.02%
24     227514171973736    576 x 138 (90), 2300,  2.89%
25    2207893435808352    625 x 144 (94), 2496,  2.77%
26                   ?    676 x 150 (98), 2700,  2.67%


queens5p[1-5].decs
----------
RCED: See table above for n=5.
Solutions: Each matrix has 2 solutions. Total number of solutions is 10.

A set of matrices for parallel processing. All the matrices are identical, but
they have been given different initialization vectors. That way they will only
process part of the original matrix and the total set of solutions is the same
as for the conventional queens5.decs matrix.
