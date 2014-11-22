import numpy as np

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
             
    return M, b
    
    
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
    
    #TODO: size of x?
    x = np.zeros(1)
    
    #TODO: your function
        
    return x