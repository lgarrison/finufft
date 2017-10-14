#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "finufft.h"
#include <fftw3.h>
#include "ndarray.h"

namespace py = pybind11;

// Code from Dan Foreman-Mackey:

// A custom error handler to propagate errors back to Python
class error : public std::exception {
public:
  error (const std::string& msg) : msg_(msg) {};
  virtual const char* what() const throw() {
    return msg_.c_str();
  }
private:
  std::string msg_;
};

// basic interface to FFTW planning options, accessed in order by fftw=0,1,2....
static int fftwoptslist[] = {FFTW_ESTIMATE,FFTW_MEASURE,FFTW_PATIENT,FFTW_EXHAUSTIVE};

// Create and copy over FINUFFT's options struct...
#define ASSEMBLE_OPTIONS               \
  nufft_opts opts;                     \
  opts.debug = debug;                  \
  opts.spread_debug = spread_debug;    \
  opts.spread_sort = spread_sort;      \
  opts.fftw = fftwoptslist[fftw];      \
  opts.modeord = modeord;

// dfm's, not used yet...
#define CHECK_FLAG(NAME)                            \
  if (ier != 0) {                                   \
    std::ostringstream msg;                         \
    msg << #NAME << " failed with code " << ier;    \
    throw finufft_error(msg.str());                 \
  }




////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 1-d
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int finufft1d1_cpp(py::array_t<double> xj,py::array_t<CPX> cj,int iflag,double eps,int ms,py::array_t<CPX> fk,int debug, int spread_debug, int spread_sort, int fftw, int modeord) {
    NDArray<double> xja(xj);
    NDArray<CPX> cja(cj);
    NDArray<CPX> fka(fk);
    if (xja.size!=cja.size)
        throw error("Inconsistent dimensions between xj and cj");
    if (fka.size!=ms)
        throw error("Incorrect size for fk");
    int nj=xja.size;
    ASSEMBLE_OPTIONS
    int ret=finufft1d1(nj,xja.ptr,cja.ptr,iflag,eps,ms,fka.ptr,opts);
    return ret;
}
int finufft1d2_cpp(py::array_t<double> xj,py::array_t<CPX> cj,int iflag,double eps,int ms,py::array_t<CPX> fk,int debug, int spread_debug, int spread_sort, int fftw, int modeord) {
    NDArray<double> xja(xj);
    NDArray<CPX> cja(cj);
    NDArray<CPX> fka(fk);
    if (xja.size!=cja.size)
        throw error("Inconsistent dimensions between xj and cj");
    if (fka.size!=ms)
        throw error("Incorrect size for fk");
    int nj=xja.size;
    ASSEMBLE_OPTIONS
    int ret=finufft1d2(nj,xja.ptr,cja.ptr,iflag,eps,ms,fka.ptr,opts);
    return ret;
}
int finufft1d3_cpp(py::array_t<double> x,py::array_t<CPX> c,int iflag,double eps,py::array_t<double> s,py::array_t<CPX> f,int debug, int spread_debug, int spread_sort, int fftw, int modeord) {
    NDArray<double> xa(x);
    NDArray<CPX> ca(c);
    NDArray<double> sa(s);
    NDArray<CPX> fa(f);
    if (xa.size!=ca.size)
        throw error("Inconsistent dimensions between x and c");
    if (sa.size!=fa.size)
        throw error("Inconsistent dimensions between s and f");
    int nj=xa.size;
    int nk=sa.size;
    ASSEMBLE_OPTIONS
    int ret=finufft1d3(nj,xa.ptr,ca.ptr,iflag,eps,nk,sa.ptr,fa.ptr,opts);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2-d
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int finufft2d1_cpp(py::array_t<double> xj,py::array_t<double> yj,py::array_t<CPX> cj,int iflag,double eps,int ms,int mt,py::array_t<CPX> fk,int debug, int spread_debug, int spread_sort, int fftw, int modeord) {
    NDArray<double> xja(xj);
    NDArray<double> yja(yj);
    NDArray<CPX> cja(cj);
    NDArray<CPX> fka(fk);
    if ((xja.size!=cja.size)||(yja.size!=cja.size))
        throw error("Inconsistent dimensions between xj or yj and cj");
    if (fka.size!=ms*mt)
        throw error("Incorrect size for fk");
    int nj=xja.size;
    ASSEMBLE_OPTIONS
    int ret=finufft2d1(nj,xja.ptr,yja.ptr,cja.ptr,iflag,eps,ms,mt,fka.ptr,opts);
    return ret;
}
int finufft2d2_cpp(py::array_t<double> xj,py::array_t<double> yj,py::array_t<CPX> cj,int iflag,double eps,int ms,int mt,py::array_t<CPX> fk,int debug, int spread_debug, int spread_sort, int fftw, int modeord) {
    NDArray<double> xja(xj);
    NDArray<double> yja(yj);
    NDArray<CPX> cja(cj);
    NDArray<CPX> fka(fk);
    if ((xja.size!=cja.size)||(yja.size!=cja.size))
        throw error("Inconsistent dimensions between xj or yj and cj");
    if (fka.size!=ms*mt)
        throw error("Incorrect size for fk");
    int nj=xja.size;
    ASSEMBLE_OPTIONS
    int ret=finufft2d2(nj,xja.ptr,yja.ptr,cja.ptr,iflag,eps,ms,mt,fka.ptr,opts);
    return ret;
}
int finufft2d3_cpp(py::array_t<double> x,py::array_t<double> y,py::array_t<CPX> c,int iflag,double eps,py::array_t<double> s,py::array_t<double> t,py::array_t<CPX> f,int debug, int spread_debug, int spread_sort, int fftw, int modeord) {
    NDArray<double> xa(x);
    NDArray<double> ya(y);
    NDArray<CPX> ca(c);
    NDArray<double> sa(s);
    NDArray<double> ta(t);
    NDArray<CPX> fa(f);
    if ((xa.size!=ca.size)||(ya.size!=ca.size))
        throw error("Inconsistent dimensions between x or y and c");
    if ((sa.size!=fa.size)||(ta.size!=fa.size))
        throw error("Inconsistent dimensions between s or t and f");
    int nj=xa.size;
    int nk=sa.size;
    ASSEMBLE_OPTIONS
    int ret=finufft2d3(nj,xa.ptr,ya.ptr,ca.ptr,iflag,eps,nk,sa.ptr,ta.ptr,fa.ptr,opts);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 3-d
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int finufft3d1_cpp(py::array_t<double> xj,py::array_t<double> yj,py::array_t<double> zj,py::array_t<CPX> cj,int iflag,double eps,int ms,int mt,int mu,py::array_t<CPX> fk,int debug, int spread_debug, int spread_sort, int fftw, int modeord) {
    NDArray<double> xja(xj);
    NDArray<double> yja(yj);
    NDArray<double> zja(zj);
    NDArray<CPX> cja(cj);
    NDArray<CPX> fka(fk);
    if ((xja.size!=cja.size)||(yja.size!=cja.size)||(zja.size!=cja.size))
        throw error("Inconsistent dimensions between xj or yj or zj and cj");
    if (fka.size!=ms*mt*mu)
        throw error("Incorrect size for fk");
    int nj=xja.size;
    ASSEMBLE_OPTIONS
    int ret=finufft3d1(nj,xja.ptr,yja.ptr,zja.ptr,cja.ptr,iflag,eps,ms,mt,mu,fka.ptr,opts);
    return ret;
}
int finufft3d2_cpp(py::array_t<double> xj,py::array_t<double> yj,py::array_t<double> zj,py::array_t<CPX> cj,int iflag,double eps,int ms,int mt,int mu,py::array_t<CPX> fk,int debug, int spread_debug, int spread_sort, int fftw, int modeord) {
    NDArray<double> xja(xj);
    NDArray<double> yja(yj);
    NDArray<double> zja(zj);
    NDArray<CPX> cja(cj);
    NDArray<CPX> fka(fk);
    if ((xja.size!=cja.size)||(yja.size!=cja.size)||(zja.size!=cja.size))
        throw error("Inconsistent dimensions between xj or yj or zj and cj");
    if (fka.size!=ms*mt*mu)
        throw error("Incorrect size for fk");
    int nj=xja.size;
    ASSEMBLE_OPTIONS
    int ret=finufft3d2(nj,xja.ptr,yja.ptr,zja.ptr,cja.ptr,iflag,eps,ms,mt,mu,fka.ptr,opts);
    return ret;
}
int finufft3d3_cpp(py::array_t<double> x,py::array_t<double> y,py::array_t<double> z,py::array_t<CPX> c,int iflag,double eps,py::array_t<double> s,py::array_t<double> t,py::array_t<double> u,py::array_t<CPX> f,int debug, int spread_debug, int spread_sort, int fftw, int modeord) {
    NDArray<double> xa(x);
    NDArray<double> ya(y);
    NDArray<double> za(z);
    NDArray<CPX> ca(c);
    NDArray<double> sa(s);
    NDArray<double> ta(t);
    NDArray<double> ua(u);
    NDArray<CPX> fa(f);
    if ((xa.size!=ca.size)||(ya.size!=ca.size)||(za.size!=ca.size))
        throw error("Inconsistent dimensions between x or y or z and c");
    if ((sa.size!=fa.size)||(ta.size!=fa.size)||(ua.size!=fa.size))
        throw error("Inconsistent dimensions between s or t or u and f");
    int nj=xa.size;
    int nk=sa.size;
    ASSEMBLE_OPTIONS
    int ret=finufft3d3(nj,xa.ptr,ya.ptr,za.ptr,ca.ptr,iflag,eps,nk,sa.ptr,ta.ptr,ua.ptr,fa.ptr,opts);
    return ret;
}

PYBIND11_MODULE(finufftpy_cpp, m) {
  m.doc() = "Python wrapper for FINUFFT (Magland no-data-copy version)";

  // 1-d
  m.def("finufft1d1_cpp", &finufft1d1_cpp, "Python wrapper for 1-d type 1 nufft",
        py::arg("xj").noconvert(),py::arg("cj").noconvert(),
        py::arg("iflag"),py::arg("eps"),py::arg("ms"),
	py::arg("fk").noconvert(),
	py::arg("debug"),py::arg("spread_debug"),py::arg("spread_sort"),
	py::arg("fftw"),py::arg("modeord"));
  
  m.def("finufft1d2_cpp", &finufft1d2_cpp, "Python wrapper for 1-d type 2 nufft",
        py::arg("xj").noconvert(),py::arg("cj").noconvert(),
        py::arg("iflag"),py::arg("eps"),py::arg("ms"),
	py::arg("fk").noconvert(),
	py::arg("debug"),py::arg("spread_debug"),py::arg("spread_sort"),
	py::arg("fftw"),py::arg("modeord"));

    
  m.def("finufft1d3_cpp", &finufft1d3_cpp, "Python wrapper for 1-d type 3 nufft",
        py::arg("x").noconvert(),py::arg("c").noconvert(),
        py::arg("iflag"),py::arg("eps"),
	py::arg("s").noconvert(),py::arg("f").noconvert(),
	py::arg("debug"),py::arg("spread_debug"),py::arg("spread_sort"),
	py::arg("fftw"),py::arg("modeord"));
  
  
  // 2-d
  m.def("finufft2d1_cpp", &finufft2d1_cpp, "Python wrapper for 2-d type 1 nufft",
        py::arg("xj").noconvert(),py::arg("yj").noconvert(),py::arg("cj").noconvert(),
        py::arg("iflag"),py::arg("eps"),py::arg("ms"),py::arg("mt"),
	py::arg("fk").noconvert(),
	py::arg("debug"),py::arg("spread_debug"),py::arg("spread_sort"),
	py::arg("fftw"),py::arg("modeord"));
  
  
  m.def("finufft2d2_cpp", &finufft2d2_cpp, "Python wrapper for 2-d type 2 nufft",
        py::arg("xj").noconvert(),py::arg("yj").noconvert(),py::arg("cj").noconvert(),
        py::arg("iflag"),py::arg("eps"),py::arg("ms"),py::arg("mt"),
	py::arg("fk").noconvert(),
	py::arg("debug"),py::arg("spread_debug"),py::arg("spread_sort"),
	py::arg("fftw"),py::arg("modeord"));
  
  
  m.def("finufft2d3_cpp", &finufft2d3_cpp, "Python wrapper for 2-d type 3 nufft",
        py::arg("x").noconvert(),py::arg("y").noconvert(),py::arg("c").noconvert(),
        py::arg("iflag"),py::arg("eps"),
	py::arg("s").noconvert(),py::arg("t").noconvert(),py::arg("f").noconvert(),
	py::arg("debug"),py::arg("spread_debug"),py::arg("spread_sort"),
	py::arg("fftw"),py::arg("modeord"));
  
  
  // 3-d
  m.def("finufft3d1_cpp", &finufft3d1_cpp, "Python wrapper for 3-d type 1 nufft",
        py::arg("xj").noconvert(),py::arg("yj").noconvert(),py::arg("zj").noconvert(),py::arg("cj").noconvert(),
        py::arg("iflag"),py::arg("eps"),py::arg("ms"),py::arg("mt"),py::arg("mu"),
	py::arg("fk").noconvert(),
	py::arg("debug"),py::arg("spread_debug"),py::arg("spread_sort"),
	py::arg("fftw"),py::arg("modeord"));
  
  
  m.def("finufft3d2_cpp", &finufft3d2_cpp, "Python wrapper for 3-d type 2 nufft",
        py::arg("xj").noconvert(),py::arg("yj").noconvert(),py::arg("zj").noconvert(),py::arg("cj").noconvert(),
        py::arg("iflag"),py::arg("eps"),py::arg("ms"),py::arg("mt"),py::arg("mu"),
	py::arg("fk").noconvert(),
	py::arg("debug"),py::arg("spread_debug"),py::arg("spread_sort"),
	py::arg("fftw"),py::arg("modeord"));
  
  
  m.def("finufft3d3_cpp", &finufft3d3_cpp, "Python wrapper for 3-d type 3 nufft",
        py::arg("x").noconvert(),py::arg("y").noconvert(),py::arg("z").noconvert(),py::arg("c").noconvert(),
        py::arg("iflag"),py::arg("eps"),
	py::arg("s").noconvert(),py::arg("t").noconvert(),py::arg("u").noconvert(),py::arg("f").noconvert(),
	py::arg("debug"),py::arg("spread_debug"),py::arg("spread_sort"),
	py::arg("fftw"),py::arg("modeord"));

}   // end of module