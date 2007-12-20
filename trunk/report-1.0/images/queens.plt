reset
set xlabel "Queens"
set ylabel "Updates"
set xr [2:16]
#set yr [0:100000000]
set logscale y
set format y "10^{%L}"
set xtics 1
unset key
#set ytics 10000000
#set key 8,80000000
plot "test-queens.txt" using 1:4 with linespoints


