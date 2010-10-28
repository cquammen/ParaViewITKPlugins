set term postscript eps enhanced color
#set output "results.eps"
set xlabel "Number of threads"
set ylabel "Execution time (seconds)"
plot [] [] "../images/perf_threads.txt" using 1:2 with linespoints title "ConvolutionImageFilter", \
           "../images/perf_threads.txt" using 1:3 with linespoints title "FFTConvolutionImageFilter with FFTW"\

