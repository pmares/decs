reset
#set title "Dancing Links S heuristic performance"
set xlabel "Recursion level"
set ylabel "Updates"
set xr [0:13]
set yr [0:100000000]
#set format y "%.0f"
set xtics 1
set ytics 10000000
set key 6,80000000
plot \
"test-s-heuristic.txt" using 1:5 title 'Updates with S heuristic' with linespoints, \
"test-no-s-heuristic.txt" using 1:5 title 'Updates without S heuristic' with linespoints

#"test-s-heuristic.txt" using 1:2 title 'Nodes with S heuristic' with linespoints, \
#"test-no-s-heuristic.txt" using 1:2 title 'Nodew without S heuristic' with linespoints


