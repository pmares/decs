reset
set xlabel "Recursion level"
set ylabel "Updates"
set xr [0:13]
set yr [0:100000000]
set xtics 1
set ytics 10000000
set key 6,80000000
plot \
"test-s-heuristic.txt" using 1:5 title 'Updates with S heuristic' with linespoints, \
"test-no-s-heuristic.txt" using 1:5 title 'Updates without S heuristic' with linespoints
