import numpy as np

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
          
    return P, L, U
    
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
    
    #TODO: size of x?
    x = np.zeros(1)
    
    #TODO: compute x
    
    return x

	
