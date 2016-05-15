#include<Python.h>
#include<numpy/arrayobject.h>

#include<boost/python.hpp>
#include<boost/numeric/ublas/vector.hpp>
#include<pyublas/numpy.hpp>

#include"stft_src/STFT.hpp"
#include"stft_src/invSTFT.hpp"

using namespace std;
using namespace boost::python;
using namespace boost::numeric;
using namespace pyublas;



boost::python::tuple
stft_glue(const boost::numeric::ublas::vector<double> &x,
        const int length,
        const int shift)
{
    const int n_frame = ((int)x.size() - length) / shift + 1;
    const int n_freq = length / 2 + 1;
//    std::cerr << n_frame << " " << n_freq << " " << length << " " << shift << std::endl;

    std::vector<double> y(x.size() + length * 10);
    std::cerr << x.size() << " " << y.size() << std::endl;
    for(size_t i = 0; i != y.size(); ++i)
        y[i] = i < x.size() ? x[i] : 0.0;
//    std::cerr << y.size() << std::endl;
    STFT stft(length, shift, n_frame);
    //allocation
    twoDimArray<double> abs_spec(n_frame, n_freq);
    twoDimArray<std::complex<double> > phase_spec(n_frame, n_freq);

    // exec stft
    stft.exec(&(y[0]), &abs_spec, &phase_spec);

    boost::numeric::ublas::matrix<double> abs_spec_boost(n_frame, n_freq);
    boost::numeric::ublas::matrix<double> phase_spec_boost(n_frame, n_freq);

    for(int i = 0; i != n_frame; ++i){
        for(int j = 0; j != n_freq; ++j){
            abs_spec_boost(i,j) = abs_spec[i][j];
            phase_spec_boost(i,j) = std::arg(phase_spec[i][j]);
        }
    }

    auto ret = boost::python::make_tuple(abs_spec_boost, phase_spec_boost);
    return ret;
}



// ----------------------------------------------------------------------------
BOOST_PYTHON_MODULE( STFT ){
	numeric::array::set_module_and_type( "numpy", "ndarray" );

/*
    class_<STFT>("stft")
//        .def("setparams",&STFT::STFT) // it is not needed to define the constructor explicitly?
        .def("exec",&STFT::exec)
        .def("n_frame",&STFT::get_n_frame)
        .def("n_freq",&STFT::get_n_freq);
*/

	boost::python::def( "stft", &stft_glue );
	import_array();
}
