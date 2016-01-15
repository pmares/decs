## Introduction ##

The goal of this project is to implement [Donald E. Knuth](http://www-cs-faculty.stanford.edu/~knuth)'s recursive Dancing Links (DLX) algorithm as a parallel algorithm. This will enable the workload to be distributed among a network of computer systems using [BOINC](http://boinc.berkeley.edu/). By scaling the system in this manner we will be able to solve more complex problems than those which can be solved by a single system.

The DLX algorithm solves exact cover problems which includes: Polyomino tiling, Latin square puzzles, Sudoku, n-queens, set packing and set partitioning problems.

The "DECS Dance" release contains programs and libraries to generate DLX matrices and run the DLX algorithm on a single computer.

DECS was initially developed by Jan Magne Tjensvold as a project in computer engineering at the [University of Stavanger](http://www.uis.no).

## News ##

### 2008-12-12 ###
  * Experiencing some trouble getting the BOINC server to start. It seems to break down from time to time so it might be time to do a reinstall and start from scratch.

### 2008-05-14 ###
  * Power failure problem solved and a new UPS has been installed.

### 2008-04-24 ###
  * Problems with multiple power failures breaking the UPS for the BOINC server.

### 2007-12-17 ###
  * Added the package with MATLAB files using the GPenSIM package to model and simulate the DECS system using Petri nets. This is the code used in the report to simulate the system.

### 2007-12-12 ###
  * The DECS [BOINC server](http://evil.podzone.org/decs/) is being prepared for use. You can download the official [BOINC client](http://boinc.berkeley.edu/download.php) and join the project now. Currently the server can only facilitate Windows 32-bit and 64-bit clients, but Linux and other operating systems are on the way.

### 2007-10-04 ###
Version 0.2 of the DECS programs and libraries has been released. New additions includes:
  * degen program for general purpose DLX matrix generation with libnqdecs as a support library for generating n-queens matrices.
  * Full support in libdlx for solving general cover problems by using secondary columns.
  * Build support for [cmake](http://www.cmake.org) which makes it easier to build DECS on other platforms.

### 2007-10-01 ###
Version 0.1 of the dance program and the DLX library has been released. Check the [download page](http://code.google.com/p/decs/downloads/list) for more detail.