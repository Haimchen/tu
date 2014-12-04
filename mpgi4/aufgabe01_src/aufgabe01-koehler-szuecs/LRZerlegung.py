"""
Sarah Koehler (sarah.c.koehler@gmail.com)
Dora Szuecs (szuucs.dora@gmail.com)
"""

import numpy as np

eps = 1e-16

def LUDecomposition(M):
    """
    LU decomposition of matrix M.

    Inputs:
    M :: matrix to be decomposed (np.array)

    Return:
    P :: Permutation matrix of pivoting transformations (np.array)
    L :: L matrix of LU decomposition, lower triangular (np.array)
    U :: U matrix of LU decomposition, upper triangular (np.array)
    """
  
    M_rows, M_cols = np.shape(M)

    #Defining the return values
    P = np.identity(M_rows)     
    L = np.zeros((M_rows, M_rows))   
    U = np.copy(M)

    #TODO: split M into P, L and U

    # is the matrix in quadratic form?    
    if U.shape[0] != U.shape[1]:
        raise ValueError('Matrix is not in quadratic form')

    for i in range (0, U.shape[1]-1):
        row = findMax(U, i)
	
		# swapping rows in U, P, L
        if (i != row):
            temp = np.copy(U[i,:])
            U[i,:] = U[row,:]
            U[row,:] = temp
            
            temp = np.copy(P[i,:])
            P[i,:] = P[row,:]
            P[row,:] = temp 

            temp = np.copy(L[i,:])
            L[i,:] = L[row,:]
            L[row,:] = temp 
        
        d = U[i,i]
        for j in range (i+1, U.shape[0]):
            L[j, i] = (U[j, i] / d)
            U[j, :] = U[j, :] - ((U[j, i] / d) * U[i, :])
	
	# final form of L
    L = np.identity(M_rows) + L
    
    return P, L, U

def findMax(M, i):
    """
    Finds the biggest element in a column between index i and the last row
    
    Inputs: M :: matrix representing linear system (numpy.array)
            i :: index of row that may has to be switched
    
    Return: row :: index of row that has the biggest element
    """

    temp = M[i, i]
    row = i
    for j in range(i+1, M.shape[0]):
        if abs(M[j, i]) > abs(temp):
            temp = M[j, i]
            row = j
    if temp == 0.0:
        raise ValueError("Column filled with zeros")
    return row

    
def solveLU( P, L, U, b):
    """
    Solving matrix multiplication by using LUDecomposition

    Inputs:
    P :: Permutation matrix of pivoting transformations (np.array)
    L :: lower triangular matrix of LU (np.array)
    U :: upper triangular matrix of LU (np.array)
    b :: right hand side of linear system (np.array)

    Return:
    x :: solution of linear system represented by L,U,b (np.array)
    """
    
	# if the last number on the diagonal is a zero in matrix U
    n = U[U.shape[1]-1, U.shape[1]-1]
    if  (abs(n) < eps):
        raise ValueError("No solution or no definite solution")     
    

    #TODO: size of x?
    x = np.zeros(U.shape[1])
    y = np.zeros(L.shape[1])
    
    #TODO: compute x
    
    
    b = np.dot(P, b)
    
	# forward substitution
    for r in range(0, y.shape[0]): 	
        faktor = 0.0
        for s in range(0, r):
            faktor = faktor + y[s] * L[r, s]
        y[r] = (b[r] - faktor ) / L[r, r]      
   
    # back substitution	
    for r in reversed(range(0, x.shape[0])): 	
        faktor = 0.0
        for s in reversed(range(r+1, x.shape[0])):
            faktor = faktor + x[s] * U[r, s]
        x[r] = (y[r] - faktor ) / U[r, r]

    return x

def LRTest( N):
 
    # generate two random matrices of size N x N
    M = np.random.randn(N, N)
    b = np.random.randn(N, 1)
    
    # compute matrix-matrix product
    P, L, U = LUDecomposition(M)
    myFunct = solveLU( P, L, U, b) 
    npFunct = np.linalg.solve( M, b)
    print(myFunct)
    print(npFunct)

    return # np.allclose( myFunct, npFunct)


LRTest(5)



	
