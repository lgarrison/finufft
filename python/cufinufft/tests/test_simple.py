import pytest

import numpy as np

import pycuda.autoinit
import pycuda.gpuarray as gpuarray

import cufinufft

import utils

DTYPES = [np.float32, np.float64]
SHAPES = [(64,), (64, 64), (64, 64, 64)]
MS = [256, 1024, 4096]
TOLS = [1e-2, 1e-3]
OUTPUT_ARGS = [False, True]

@pytest.mark.parametrize("dtype", DTYPES)
@pytest.mark.parametrize("shape", SHAPES)
@pytest.mark.parametrize("M", MS)
@pytest.mark.parametrize("tol", TOLS)
@pytest.mark.parametrize("output_arg", OUTPUT_ARGS)
def test_simple_type1(dtype, shape, M, tol, output_arg):
    real_dtype = dtype
    complex_dtype = utils._complex_dtype(dtype)

    dim = len(shape)

    fun = {1: cufinufft.nufft1d1,
           2: cufinufft.nufft2d1,
           3: cufinufft.nufft3d1}[dim]

    k = utils.gen_nu_pts(M, dim=dim).astype(real_dtype)
    c = utils.gen_nonuniform_data(M).astype(complex_dtype)

    k_gpu = gpuarray.to_gpu(k)
    c_gpu = gpuarray.to_gpu(c)

    if output_arg:
        fk_gpu = gpuarray.GPUArray(shape, dtype=complex_dtype)
        fun(*k_gpu, c_gpu, out=fk_gpu, eps=tol)
    else:
        fk_gpu = fun(*k_gpu, c_gpu, shape, eps=tol)

    fk = fk_gpu.get()

    ind = int(0.1789 * np.prod(shape))

    fk_est = fk.ravel()[ind]
    fk_target = utils.direct_type1(c, k, shape, ind)

    type1_rel_err = np.abs(fk_target - fk_est) / np.abs(fk_target)

    assert type1_rel_err < 10 * tol


@pytest.mark.parametrize("dtype", DTYPES)
@pytest.mark.parametrize("shape", SHAPES)
@pytest.mark.parametrize("M", MS)
@pytest.mark.parametrize("tol", TOLS)
@pytest.mark.parametrize("output_arg", OUTPUT_ARGS)
def test_simple_type2(dtype, shape, M, tol, output_arg):
    real_dtype = dtype
    complex_dtype = utils._complex_dtype(dtype)

    dim = len(shape)

    fun = {1: cufinufft.nufft1d2,
           2: cufinufft.nufft2d2,
           3: cufinufft.nufft3d2}[dim]

    k = utils.gen_nu_pts(M, dim=dim).astype(real_dtype)
    fk = utils.gen_uniform_data(shape).astype(complex_dtype)

    k_gpu = gpuarray.to_gpu(k)
    fk_gpu = gpuarray.to_gpu(fk)

    if output_arg:
        c_gpu = gpuarray.GPUArray((M,), dtype=complex_dtype)
        fun(*k_gpu, fk_gpu, out=c_gpu)
    else:
        c_gpu = fun(*k_gpu, fk_gpu)

    c = c_gpu.get()

    ind = M // 2

    c_est = c[ind]
    c_target = utils.direct_type2(fk, k[:, ind])

    type2_rel_err = np.abs(c_target - c_est) / np.abs(c_target)

    assert type2_rel_err < 10 * tol
