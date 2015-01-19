# -*- coding: utf-8 -*-
"""
Created on Thu Dec 12 14:52:29 2013

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


def dftMatrix( n) :
    """
    Construct DFT matrix of size n.

    Inputs:
    n : size of DFT dftMatrix


    Return:
    F : DFT matrix of size n.
    """

    # allocate matrix
    F = np.zeros( (n, n), dtype='complex128')

    # principal term for DFT matrix
    omega = np.complex( np.exp( -np.pi * 2j/   n))

    # fill in matrix
    # (naive implementation, could be done more efficiently using
    # numpy threading capabilities)
    for a in range( n) :
        for b in range( n) :

            val = omega**(a*b)
            F[a,b] = val
            F[b,a] = val

    # normalize
    F = 1.0 / np.sqrt(n) * F

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

    # get DFT matrix
    F = dftMatrix(n)

    for i in range(n) :
        sig = np.zeros( n)
        sig[i] = 1.0

        fsig = F.dot(sig)

        # store current signal and Fourier transform
        sigs.append( sig)
        fsigs.append( fsig)

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
    
    # get DFT matrix
    F = dftMatrix( fdata.size)

    # test properties of DFT matrix
    if( True == test) :

        # check that F is unitary (better tested in dftMatrix() and always for debug mode
        # for production code)
        n = data.size
        err = np.linalg.norm( np.eye( n) - F.dot( F.transpose().conjugate()))
        assert( err <  np.finfo( F.dtype).eps * 10.0*n )

        # Fourier transform of real signal must be symmetric
        # (some care required for not even number of samples)


    # perform Fourier transform    
    fdata = F.dot( fdata)

    return fdata


def shuffleBitReversedOrder( data) :
    """
    Shuffle elements of data using bit reversal of order of current index (helper for fft())

    Input:
    data : data to be transformed (np.array, shape=(n,), dtype='float64')

    Return:
    data : index with shuffled elements
    """
    n = data.size
    nh = n / 2;

    # implement bit reversal
    # since bit reversal is symmetric we only have to compute the target for the swap
    # for the first half of the array
    for i in range(n) :

        # find the swap index for current i
        ii = np.int( i)
        m = np.int( nh)
        j = 0
        while m > 0 :
            j += m * np.mod(ii,2)
            m /= 2
            ii = np.int( ii / 2)

        # funky python swapping
        if( j > i) :
            data[i], data[j] = data[j], data[i]

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

    # check if input length is power of two
    assert( n > 0)
    assert( (n & (n - 1)) == 0)

    inverse_bit = 1.0
    if( inverse) :
        inverse_bit = -1.0

    # first step of FFT: shuffle elements
    fdata = shuffleBitReversedOrder( fdata)
    # second step, recursively merge transforms

    # for each level of the tree, or equivalently always determine DFT's of length m
    mm = 1
    while n > mm :

        # for all powers of k
        for k in range(0,mm) :

            omega_k = np.complex( np.exp( (-1.j * inverse_bit * 2.0 * np.pi * k) / (2*mm) ))

            # iterate over the whole data set
            for i in range(k,n,2*mm) :

                j = i + mm
                product = fdata[j] * omega_k

                fdata[j] = fdata[i] - product
                fdata[i] = fdata[i] + product

        mm *= 2


    # make transform unitary
    fdata = 1.0 / np.sqrt(n) * fdata

    return fdata

def testPerformanceFFT() :
    """
    Compare performance of FFT and DFT matrix

    Inputs:
    -

    Return:
    -

    """

    lens = [4, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192]
    n_rand = 10

    times = np.zeros( (2,len(lens)))

    for i, clen in enumerate(lens) :
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

    # test correctness of FFT
    plotHarmonics()

    # test correctness of FFT
    testFFT()

    # test performance of FFT
    testPerformanceFFT()


main()
