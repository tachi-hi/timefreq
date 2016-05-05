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

// ----------------------------------------------------------------------------
BOOST_PYTHON_MODULE( myTest ){
	numeric::array::set_module_and_type( "numpy", "ndarray" );

    class_<STFT>("stft")
//        .def("setparams",&STFT::STFT) // it is not needed to define the constructor explicitly?
        .def("exec",&STFT::exec)
        .def("n_frame",&STFT::get_n_frame)
        .def("n_freq",&STFT::get_n_freq);

	import_array();
}
