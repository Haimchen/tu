import ellipse
import numpy as np
import matplotlib.pyplot as plt
import math
from numpy import dot



"""

"""
def orthogonalVector(v):
    u = np.array([-1.0 * v[1], v[0]])

    return u 

def rotateVector(v, phi):
    rotationMatrix = np.array([[np.cos(phi), - np.sin(phi)], [np.sin(phi), np.cos(phi)]])
    u = np.dot(rotationMatrix, v)

    return u

def sinogramm(nshots, nrays):
    """
    Aufgabe 3 a)
    Saves the trace values in a matrix
    
    Inputs:
    nshots :: number of shots
    nrays  :: number of rays per shot
        
    Output:
    sino :: trace values stored in a np.array 
    PNG file (use plt.imshow)
    """
    
    sino = np.zeros((nshots, nrays))

    # the angle between two shots
    phi = np.deg2rad(180 / nshots)

    # first value of direction vector
    dirVector = np.array([-1, 0])

    # distance between two rays
    rayOffset = 2 / nrays

    for i in range(0, nshots):
        
        shotDirVector = rotateVector(dirVector, i * phi)
        orthVector = orthogonalVector(shotDirVector)

        dirHelpVector = np.multiply(shotDirVector, -np.sqrt(2))

        startPoint = np.add(dirHelpVector, orthVector)
        for k in range(0, nrays):
            v = np.multiply(orthVector, - k * rayOffset)

            start = np.add(startPoint, v)

            intensity = ellipse.trace(shotDirVector, start)

            sino[i, k] = intensity

    
    # visualize sino
    plt.matshow(sino, cmap = plt.cm.gray)
    plt.show()

    #sino has to be returned for 3 c)
    print(sino)


    return sino

def equalMatrix(nshots, nrays, ngrid):
    """
    Aufgabe 3 b)
    Calculates the equality matrix (Ausgleichsmatrix)
    
    Inputs:
    nshots :: number of shots
    nrays  :: number of rays per shot
    ngrid  :: size of raster grid (ngrid*ngrid)
    
    Output:
    A :: Matrix containing the intersections (np.array)
    """

    A = np.zeros((nshots*nrays, ngrid*ngrid))
    #TODO: calculate starting points for each ray    
    
    #TODO: use grid_intersect
    #I, G, dt, px, py = ellipse.grid_intersect(?, ?, ?)
    
    #TODO: fill A with content from grid_intersect
    
    return A 

def solveTg(nshots, nrays, ngrid):
    """
    Aufgabe 3 c)
    evaluates the sinogramm using the matrix containing the intersect
    values and creates the tomograph picture
    
    Inputs:
    nshots :: number of shots
    nrays  :: number of rays per shot
    ngrid  :: size of raster grid (ngrid*ngrid)
    
    Output:
    PNG file (use plt.imshow and plt.show)
    """
    
    #TODO: use your sinogramm function and reshape for gauss
    
    #TODO: use your equalMatrix function

    #TODO: generate picture of toft (use matplotlib as plt)

sino = sinogramm(1024, 1024)
