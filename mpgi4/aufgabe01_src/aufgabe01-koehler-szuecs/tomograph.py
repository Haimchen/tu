import ellipse
import numpy as np
import matplotlib.pyplot as plt
import math
from numpy import dot
import GaussianElimination as gauss

"""
Sarah Koehler (sarah.c.koehler@gmail.com)
Dora Szuecs (szuucs.dora@gmail.com)
"""


def orthogonalVector(v):
    """
    Calculates an vector u, that is orthogonal to a given vector v

    inputs:
    v :: 2-dimensional vector

    outputs:
    u :: orthognal vector to v
    """
    u = np.array([-1.0 * v[1], v[0]])

    return u 

def rotateVector(v, phi):
    """
    rotates a given vector v by phi degree
    """
    rotationMatrix = np.array([[np.cos(phi), - np.sin(phi)], [np.sin(phi), np.cos(phi)]])
    u = np.dot(rotationMatrix, v)

    return u

def calculateStartPoints(shotIndex, nrays, nshots):
    """
    Calculates the start points and the direction vector for all rays of one shot

    inputs:
    shotIndex :: the index of the shot
    nrays :: number of rays per shots
    nshots :: number of shots

    outputs:
    startPoints :: an array containing all startPoints of the rays in this shot (nshots x 2)
    shotDirVector :: the direction vector of all rays in this shot

    """
    # the angle between two shots
    phi = np.deg2rad(180 / nshots)

    # first value of direction vector
    dirVector = np.array([-1, 0])

    # distance between two rays
    rayOffset = 2 / nrays

    shotDirVector = rotateVector(dirVector, shotIndex * phi)
    orthVector = orthogonalVector(shotDirVector)

    dirHelpVector = np.multiply(shotDirVector, - np.sqrt(2))

    startPoint = np.add(dirHelpVector, orthVector)

    startPoints = np.ndarray((nrays, 2), float)
    for k in range(0, nrays):
        v = np.multiply(orthVector, - k * rayOffset)

        start = np.add(startPoint, v)
        startPoints[k,:] = start
    return startPoints, shotDirVector


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
        startArray, shotDirVector = calculateStartPoints(i, nrays, nshots)

        for ind, val in enumerate(startArray):
            intensity = ellipse.trace(shotDirVector, val)

            sino[i, ind] = intensity

    
    # visualize sino
    plt.matshow(sino, cmap = plt.cm.gray)
    plt.show()

    #sino has to be returned for 3 c)

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

    # calculate starting points for each ray    
    for i in range(0, nshots):
        startArray, shotDirVector = calculateStartPoints(i, nrays, nshots)

        I, G, dt, px, py = ellipse.grid_intersect(ngrid, startArray, shotDirVector)
        
        for k in range(0, I.size):
            ray = I[k]
            quadrant = G[k]
            cutLength = dt[k]

            ray2shot = i * nrays + ray
            A[ray2shot, quadrant] = cutLength
 
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
    
    # compute intensities and vector b
    sino = sinogramm(nshots, nrays)
    b = np.reshape(sino, (-1, 1))

    
    # compute A and input values for Gauss
    A = equalMatrix(nshots, nrays, ngrid)

    AT = np.transpose(A)

    ATA = np.dot(AT, A)

    ATb = np.dot(AT, b)

    # use Gauss to solve equation
    M, b = gauss.gaussianElimination(ATA, ATb, use_pivoting = True)
    x = gauss.backSubstitution(M, b)


    #generate picture of toft (use matplotlib as plt)
    densities = np.array(x).reshape((ngrid, ngrid))

    # visualize densities
    plt.matshow(densities, cmap = plt.cm.gray)
    plt.show()

# example for sinogram in 64 * 64
sinogramm(64, 64)

# example for sinogram in 128 * 128
sinogramm(128, 128)

# example for tomograph graphic with 64*64 Grid
solveTg(128, 128, 64)