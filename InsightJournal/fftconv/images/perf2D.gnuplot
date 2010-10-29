set term postscript eps enhanced color
#set output "results.eps"
set xlabel "Kernel size"
set ylabel "Execution time (seconds)"
plot [] [0:0.25] "../images/perf2D.txt" using 1:2 with lines title "ConvolutionImageFilter", \
                 "../images/perf2D.txt" using 1:3 with lines title "FFTConvolutionImageFilter with FFTW",\
                 "../images/perf2D-wisdom.txt" using 1:3 with lines title "FFTConvolutionImageFilter with FFTW + wisdom",\
                 "../images/perf2D-vnl.txt" using 1:3 with lines title "FFTConvolutionImageFilter with vnl"\

