reset
set xlabel "Queens"
set ylabel "Updates"
set xr [2:18]
set logscale y
set format y "10^{%L}"
set xtics 1
unset key
plot "test-queens.txt" using 1:4 with linespoints
