# -*- coding: utf-8 -*-
"""
MPGI4 -- aufgaben0.1
@author: {clessig,sasha}@tu-berlin.de

Sarah Köhler (sarah.c.koehler@gmail.com)
Dora Szücs (szuucs.dora@gmail.com)
"""
import numpy as np
import matplotlib.pyplot as plt
import time


def timedcall( num_timing_iters, fn, *args):
    '''
    Run a function and measure execution time.

    Inputs:
    fn : function to be executed
    args : arguments to function fn

    Return:
    dt : execution time
    result : result of function

    Usage example:
      You want to time the function call "C = foo(A,B)".
       --> "T, C = timecall(foo, A, B)"
    '''

    t0 = time.time()
    for i in range(num_timing_iters) :
        result = fn(*args)
    t1 = time.time()

    dt = (t1 - t0) / num_timing_iters

    return dt, result


def matMult(A, B):
    '''Calculate product A * B of two matrices A,B.

    Inputs:
    A : first operand
    A : second operand

    Return:
    C : matrix product A * B
    '''

    # Test if shape of matrices if compatible
    if A.shape[1] != B.shape[0]:
        raise valueError('Matrices are not of compatible size')

    # Compute matrix product
    # TODO: ...

    C = np.zeros( (A.shape[0], B.shape[1]))

    for i in range(0, A.shape[0]):
        for j in range(0, B.shape[1]):
            for k in range(0, A.shape[1]):
                C[i][j] = C[i][j] + A[i][k] * B[k][j]


    return C


def matMultExperiment( Nmax = 200, N = 30):
    '''
    Compare performance of numpy matrix multiplication (np.dot()) and matMult above.

    Input:
    Nmax : maximum matrix size to be tested
    N : step size for matrix sizes

    Return:
    -

    Side effects:
    Generates performance plots and saves file 'matmult_performance.png'.
    '''

    X, Y_mat_mult, Y_numpy = [], [], []

    for m in range(2, Nmax, N):

        A = np.random.random((m,m))
        B = np.random.random((m,m))

        time_mat_mult, r_mat_mult = timedcall( 10, matMult, A, B)
        time_numpy, r_numpy = timedcall( 1000, np.dot, A, B)

        try:
            assert(np.allclose(r_numpy, r_mat_mult))
        except:
            print('Das Ergebnis ist noch nicht korrekt!')
            break
        X.append(m)
        Y_numpy.append(time_numpy)
        Y_mat_mult.append(time_mat_mult)

    # Plotting

    plt.figure()
    plt.loglog(X, Y_mat_mult, label = 'mat_mult')
    plt.loglog(X, Y_numpy, label = 'numpy')
    plt.grid()

    plt.legend(loc = 'upper left')
    plt.title("Numpy vs. for-loop matrix multiplication")
    plt.xlabel("Matrix size")
    plt.xlim( 2, Nmax)
    plt.ylabel("Time taken (s)")

    print("Completed experiment.")

    plt.savefig( './matmult_performance.png' )
    plt.show()


def matMultTest( N = 5):
    '''
    *Very* simple test function for matMult

    Input:
    N : test matrices are of size N x N

    Return:
    True if result is correct, otherwise false
    '''

    # generate two random matrices of size N x N
    A = np.random.randn( N, N)
    B = np.random.randn( N, N)

    # compute matrix-matrix product
    result_matMult = matMult( A, B)
    result_numpy = np.dot( A, B)

    return np.allclose( result_numpy, result_matMult)


if __name__ == '__main__':

    # run simple tests until matrix multiplication is correct
    ret = matMultTest()
    print( "Matrix multiplication is correct:", ret)

    # run experiment
    matMultExperiment()
