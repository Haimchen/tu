# -*- coding: utf-8 -*-
"""
Created on: 12/11/2013

@author: lessig

Sarah Köhler (sarah.c.koehler@gmail.com)
Dora Szücs (szuucs.dora@gmail.com)
"""
# import packages
import numpy as np
import matplotlib.pyplot as plt


def powerIteration( M, epsilon = -1.0) :
    """
    Compute largest eigenvector of matrix M using power iteration. It is assumed that the
    largest eigenvalue of M, in magnitude, is well separated.

    Inputs:
    M : matrix, assumed to have a well separated largest eigenvalue
    epsilon: epsilon used for convergence (default: 10 * machine precision)

    Return:
    vec : eigenvector associated with largest eigenvalue
    resids : residual for each iteration step
    """

    vec = np.ones(M.shape[0])
    last = np.ones(M.shape[0])
    
    resids = []
    res = 1
    
    while (res > epsilon):
    
      vec = np.dot(M, last)
      vec = vec * (1/np.linalg.norm(vec))
      res = np.linalg.norm(vec-last)
      resids.append(res)
      last = vec

    return vec, resids


def main() :

    # compute steady state solution of T using power iteration
    epsilon = 1e-15
    T = np.array([[0.5, 0.25, 0.25], [0.5, 0.0, 0.5], [0.25, 0.25, 0.25]])

    # print solution
    vec_T, residuals_T = powerIteration( T, epsilon) 
    print('evec(T) = ', vec_T)

    # compute error in steady state
    # (use np.linalg package to obtain reference solution)
    TEW, TEV = np.linalg.eig(T)
    i = max(TEW)

    dot = np.dot(TEV[:,i],vec_T)
    dot = np.abs(dot)
    if (dot < 1 + epsilon and dot > 1):
      dot = 1
    
    err = np.rad2deg(np.arccos(dot))  
    print("Error in eigenvector = %2.15f" % err)


    # compute first eigenvector of B using power iteration
    B = np.array([[0.6552, -0.3079, -0.2479], [-0.3079, 0.7251, -0.2202], [-0.2479, -0.2202, 0.8098]])
    vec_B, residuals_B = powerIteration( B, epsilon)
    print('evec(B) = ', vec_B)
    
    BEW, BEV = np.linalg.eig(B)
    


    # plot convergence behavior
    plt.plot( residuals_T, '-rx')
    plt.plot( residuals_B, '-b')
    plt.yscale( 'log')
    plt.xscale( 'log')
    plt.xlabel( 'Number of Iteration')
    plt.ylabel( 'Estimated Error')
    plt.legend( ['T', 'B'])

    plt.savefig( './figures/power_iteration_err.pdf')
    plt.show()

# run
if __name__ == "__main__":
    main()
