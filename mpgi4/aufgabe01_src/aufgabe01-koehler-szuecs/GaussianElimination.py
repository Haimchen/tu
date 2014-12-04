"""
Sarah Koehler (sarah.c.koehler@gmail.com)
Dora Szuecs (szuucs.dora@gmail.com)
"""

import numpy as np

eps = 1e-16

def gaussianElimination(M, b, use_pivoting = True):
    

    """
    Gaussian Elimination with Pivoting.

    Inputs:
    M :: matrix representing linear system (numpy.array)
    b :: vector, representing right hand side (numpy.array)
    use_pivoting :: flag if pivoting should be used or no flag if 
                    pivoting should not be used
 
    Return:
    M :: input matrix in row echelon form (numpy.array)
    b :: input vector with pivoting permutations applied (numpy.array)

    """

    
    #TODO: implement gauss elimination with and without pivoting

    # matrix and vector not compatible     
    if M.shape[0] != b.shape[0]:
        raise ValueError('Matrix and vector are not of compatible size')

    # no definite solution        
    if M.shape[1] > M.shape[0]:
        raise ValueError('Too few equations to solve the linear system')

    # gauss elimination
    for i in range (0, M.shape[1]-1):
        if use_pivoting:
            row = findMax(M, i)
            swap(i, row, M, b)
        else:
            # no pivoting: avoiding division by zero 
            if abs(M[i,i]) < eps:
                for k in range (i+1, M.shape[0]):
                    if abs(M[k,i]) > 0.0:
                        swap(i, k, M, b)
                        break
                if k == M.shape[0]-1:
                    raise ValueError("Column filled with zeros")

        d = M[i,i]
        for j in range (i+1, M.shape[0]):
            b[j, :] = b[j, :] - ((M[j, i] / d) * b[i, :])
            M[j, :] = M[j, :] - ((M[j, i] / d) * M[i, :])

    return M, b


def findMax(M, i):
    """
    Finds the biggest element in a column between index i and the last row
    
    Inputs: M :: matrix representing linear system (numpy.array)
            i :: index of row that may has to be switched
    
    Return: row :: index of row that has the biggest element
    """

    temp = M[i, i]
    row = i
    # searching for the element with the biggest absolute value
    for j in range(i+1, M.shape[0]):
        if abs(M[j, i]) > abs(temp):
            temp = M[j, i]
            row = j
    if abs(temp) < eps:
        raise ValueError("Column filled with zeros")
    return row


def swap(s, t, M, b):
    """
    Performs swapping on the rows of the matrix and the vector
    
    Inputs: s :: index of row to swap row t
            t :: index of row to swap s 
            M :: matrix representing linear system (numpy.array)
            b :: vector, representing right hand side (numpy.array)

    Return: void

    """
    # swap rows in matrix
    temp = np.copy(M[s,:])
    M[s,:] = M[t,:]
    M[t,:] = temp

    # swap rows in vector
    temp = np.copy(b[s,:])
    b[s,:] = b[t,:]
    b[t,:] = temp

    return


def backSubstitution(M, b):
    """
    Back substitution for the solution of a linear system in row echelon form.

    Inputs:
    M :: matrix in row echelon representing linear system (numpy.array)
    b :: vector, representing right hand side (numpy.array)
    
    Returns:
    x :: solution of linear system (numpy.array)
    """

    #TODO: check, if a solution exists
    
    if abs(M[M.shape[1]-1, M.shape[1]-1]) < eps:
        for k in range (M.shape[1]-1, M.shape[0]):
            if abs(M[k, M.shape[1]-1]) > eps:
                break
            if k == M.shape[0]-1:
                raise ValueError("No definite solution")
    
   
    #TODO: size of x?
    x = np.zeros(M.shape[1])

    #TODO: your function

    # back substitution
    for r in reversed(range(0, x.shape[0])):    
        faktor = 0.0
        for s in reversed(range(r+1, x.shape[0])):
            faktor = faktor + x[s] * M[r, s]
        x[r] = (b[r] - faktor ) / M[r, r]
        
    # if the matrix is not in quadratic form, checking if solution exists
    if M.shape[1] < M.shape[0]:
        lastX = x[M.shape[1]-1]
        for t in range(M.shape[1], M.shape[0]):
            newX = b[t] / M[t, M.shape[1]-1]            
            if (abs(abs(lastX) - abs(newX)) > eps):
                raise ValueError("No definite solution")
            
    return x

def gaussTest( N, L):

    # generate two random matrices of size N x L and N x 1
    M = np.random.randn(N, L)
    b = np.random.randn(N, 1)

    # compute matrix-matrix product
    M, b = gaussianElimination(M, b, use_pivoting = True)
    myFunct = backSubstitution(M, b) 
    npFunct = np.linalg.solve( M, b)
    print(myFunct)
    print(npFunct)

    return


#gaussTest(5, 5)
