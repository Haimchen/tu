# -*- coding: utf-8 -*-
"""
Created on Thu Dec 12 14:52:29 2013

Sarah Koehler (sarah.c.koehler@gmail.com)
Dora Szuecs (szuucs.dora@gmail.com)

@author: lessig
"""

# imports
import numpy as np
import matplotlib.pyplot as plt
import time
# convenience adjustments
np.set_printoptions( precision=3)
np.set_printoptions( suppress=True)
np.set_printoptions( linewidth=200)

eps = 1.0e-16

def dftMatrix( n) :
    """
    Construct DFT matrix of size n.

    Inputs:
    n : size of DFT dftMatrix


    Return:
    F : DFT matrix of size n.
    """
    if n == 0:
        raise ValueError("Tried to create Matrix of size 0")
    
    F = np.zeros( (n,n), dtype='complex128')
	
    # DFT matrix.
    
    F[0,:] = 1
    F[:,0] = 1
    
    omega = np.exp(np.complex128(-2.+0.j) * np.pi * np.complex128(0.+1.j) / n)
    for l in range(1, n):
        for k in range(1, n):
            F[l,k] = omega ** (l * k)
    return F

def plotHarmonics() :
    """
    Plot the discrete Fourier transform of
    """

    n = 128
    # list to store input signals to DFT
    sigs = []
    # Fourier-transformed signals in
    fsigs = []

    # TODO: extract harmonics out of  DFT matrix

    # plot the first 10 harmonic components
    n_plots = 10
    fig = plt.figure( figsize=(15,8))
    for i in range(n_plots) :

        fig.add_subplot( n_plots, 2, 2*i+1)
        plt.stem( sigs[i], '-rx')
        plt.xlim( 0, 128)
        plt.yticks([])
        if i < n_plots-1 :
            plt.xticks([])

        fig.add_subplot( n_plots, 2, 2*i+2)
        plt.plot( np.real( fsigs[i]))
        plt.plot( np.imag( fsigs[i]))
        plt.xlim( 0, 128)
        plt.yticks([])
        if i < n_plots-1 :
            plt.xticks([])

    plt.savefig( './figures/dft_harmonics.pdf')
    plt.show()


def dft( data, test = False) :
    """
    Perform real-valued discrete Fourier transform of data by constructing DFT matrix.

    Input:
    data : data to be transformed (np.array, shape=(n,), dtype='float64')
    test : if true the correctness of the transform is verified using suitable test cases

    Return:
    fdata : Fourier transformed data
    """

    fdata = data.copy()
    
    # compute DFT using DFT matrix
    n = fdata.shape[0]
    dftMat = dftMatrix(n)
    fdata =  np.dot(dftMat, data)

    # TODO: test basic properties of DFT / DFT matrix
    if test == True:
        print(dftMat)
        dftConj = dftMat.T
        print(dftConj)
        dftConj = dftConj.conj()
        print(dftConj)
        dftDot = np.dot(dftConj, dftMat)
        print(dftDot)
        identity = np.identity(n, dtype="float64") * (1.0 / n)
        errMat = dftDot - identity
        err = np.linalg.norm(errMat)
        print(err)
        assert (err < eps), "Matrix ist nicht unitär"

    return fdata


def shuffleBitReversedOrder( data) :
    """
    Shuffle elements of data using bit reversal of order of current index (helper for fft())

    Input:
    data : data to be transformed (np.array, shape=(n,), dtype='float64')

    Return:
    data : index with shuffled elements
    """

    # TODO: shuffle data by reversing bits in the index

    return data


def fft( data, inverse=False) :
    """
    Perform real-valued discrete Fourier transform of data using fast Fourier transform.

    Input:
    data_in : data to be transformed (np.array, shape=(n,), dtype='float64')

    Return:
    fdata : Fourier transformed data

    Note:
    This is not an optimized implementation but one to demonstrate the essential ideas
    of the fast Fourier transform.
    """

    fdata = np.asarray( data, dtype='complex128')
    n = fdata.size

    # check if input length is power of twoeuler
    assert( n > 0)
    assert( (n & (n - 1)) == 0)

    # TODO: implement FFT

    return fdata

def testPerformanceFFT() :
    """	
    Compare performance of FFT and DFT matrix

    Inputs:
    -

    Return:
    -

    """

    lens = [4, 16, 32, 64, 128, 256]
    #lens = [4, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192]
    n_rand = 10

    times = np.zeros( (2,len(lens)))

    for i, clen in enumerate(lens) :
        print("Performance Matrix/FFT n=%d" % clen)
        for j in range(n_rand) :

            data = np.random.randn( clen)

            t0 = time.time()
            r1 = dft( data)
            t1 = time.time()
            times[0,i] += t1 - t0

            t0 = time.time()
            r2 = fft( data)
            t1 = time.time()
            times[1,i] += t1 - t0

    times /= n_rand

    plt.figure()
    plt.plot( lens, times[0,:], '-bx')
    plt.plot( lens, times[1,:], '-rx')
    plt.legend( ['DFT', 'FFT'])
    plt.xlim( lens[0], lens[len(lens)-1])
    plt.xlabel("Size of input")
    plt.ylabel("Execution time")

    plt.savefig( './figures/times_dft_fft.pdf')
    plt.show()


def testFFT() :
    """	
    Test FFT implementation.

    Input:
    -

    Return:

    Side effects:
    - Plot error of FFT implementation and save in './figures/errs_fft.pdf'
    """

    lens = [4, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096]
    n_rand = 10

    errs = np.zeros( len(lens))

    for i, clen in enumerate(lens) :
        for j in range(n_rand) :

            data = np.random.randn( clen)
            data1 = fft( fft(data), True)

            errs[i] += np.linalg.norm( data1 - data)

    errs /= n_rand

    plt.figure()
    plt.plot( lens, errs, '-rx')
    plt.xlim( lens[0], lens[len(lens)-1])
    plt.xlabel("Size of input")
    plt.ylabel("Error")

    plt.savefig( './figures/errs_fft.pdf')
    plt.show()


def main() :
	
	z = np.array([1.2, 1.5, 1.8, 2.7])
	z2 = dft(z, True)
	print(z2)

    # test correctness of FFT
    # plotHarmonics()

    # test correctness of FFT
    # testFFT()

    # test performance of FFT
    # testPerformanceFFT()


main()

