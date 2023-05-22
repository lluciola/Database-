set terminal pngcairo size 800,600
set output 'insert_graph.png'
set title 'Performance graph of element insertions'
set xlabel 'Amount of strings inserted'
set ylabel 'Total time of execution in nanosec'
set grid
plot 'data_insert.txt' using 1:2 with linespoints title 'Performance curve'

set output 'search_graph.png'
set title 'Performance graph of element searches'
set xlabel 'Amount of strings searched'
set ylabel 'Total time of execution in nanosec'
set grid
plot 'data_search.txt' using 1:2 with linespoints title 'Performance curve'