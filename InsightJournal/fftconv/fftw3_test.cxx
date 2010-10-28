#include <fftw3.h>
#include <stdlib.h>
#include "itkTimeProbe.h"
#include "vnl/vnl_math.h"

bool isPrime( int n )
{
  int last = (int)vcl_sqrt( n );
  for( int x=2; x<=last; x++ )
    {
    if( n%x == 0 )
      {
      return false;
      }
    }
  return true;
}

int greatestPrimeFactor( int n )
{
  int v = 2;
  while( v <= n )
    {
    if( n%v == 0 && isPrime( v ) )
      {
      if( n == v )
        {
        return v;
        }
      n /= v;
      v = 2;
      }
    else
      {
      v += 1;
      }
    }
  // throw exception
}


int main(int argc, char * argv[])
{
  fftwf_init_threads();
  fftwf_plan_with_nthreads(atoi(argv[1]));
//   std::cout << fftwf_import_system_wisdom() << std::endl;
  float *in;
  fftwf_complex *out;
  fftwf_plan p;
  in = (float*) fftw_malloc(sizeof(float) * 1000*1000*1000);
  out = (fftwf_complex*) fftw_malloc(sizeof(fftwf_complex) * 1000*1000*1000);
  for( int dim=2; dim<=512; dim+=1)
    {
//   p = fftwf_plan_dft_r2c_3d(dim, dim, dim, in, out, FFTW_PATIENT);
    p = fftwf_plan_dft_r2c_3d(dim, dim, dim, in, out, FFTW_ESTIMATE);

    itk::TimeProbe t;
    int nb_rep;
    if( dim <= 32 )
      nb_rep = 10000;
    else if( dim <= 64 )
      nb_rep = 1000;
    else if( dim <= 128 )
      nb_rep = 10;
    else
      nb_rep = 1;
    for( int rep=0; rep<nb_rep; rep++ )
      {

      t.Start();
      fftwf_execute(p); /* repeat as needed */
      t.Stop();
      }
    std::cout << dim << "\t" << t.GetMeanTime() << "\t" << dim*dim*dim/t.GetMeanTime() << "\t" << greatestPrimeFactor( dim ) << std::endl;
    fftwf_destroy_plan(p);
    }

  fftw_free(in); fftw_free(out);
}
