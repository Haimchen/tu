 # -*- coding: utf-8 -*-
import numpy as np
import stickguy
"""
Sarah Köhler (sarah.c.koehler@gmail.com)
Dora Szücs (szuucs.dora@gmail.com)
"""

# new animation:
"""
Computes positions of joints for the stick guy.
Inputs:
param : list of parameters describing the pose
param[0]: height of hip
param[1]: angle of spine to vertical axis
param[2]: angle of upper arm 0 to spine
param[3]: angle of lower arm 0 to upper arm 0
param[4,5]: as above, other arm
param[6]: angle of neck/head to spine
param[7]: angle of upper leg 0 to vertical axis
param[8]: angle of lower leg 0 to upper leg 0
param[9,10]: as above, other leg
"""
keytime = np.linspace(0, 200, 9)

# stehend
keyframe = [np.array([-0.1, 0., 0.1, -0.1, -0.1, 0.1, 0.0, 0.1, -0.1, -0.1, 0.1])]
# in die Knie gehen
keyframe.append(np.array([-0.3, 0., 0.2, -0.1, -0.2, 0.1, 0.0, 0.2, -0.2, -0.2, 0.2]))
# im Sprung
keyframe.append(np.array([0., 0., 1.0, 0.0, -0.2, 0.1, 0.0, 0.13, -0.09, -0.13, 0.09]))
# oben
keyframe.append(np.array([0.2, 0., 2.9, 0.2, -0.1, 0.1, 0.0, 0.05, -0.05, -0.05, 0.05]))
# im sprung
keyframe.append(np.array([0., 0., 1.0, 0.0, -0.2, 0.1, 0.0, 0.13, -0.09, -0.13, 0.09]))
# landen mit in die Knie gehen
keyframe.append(np.array([-0.2, 0., 0.2, -0.1, -0.2, 0.1, 0.0, 0.2, -0.2, -0.2, 0.2]))
# stehend
keyframe.append(np.array([-0.1, 0., 0.1, -0.1, -0.1, 0.1, 0.0, 0.1, -0.1, -0.1, 0.1]))
keyframe.append(np.array([-0.1, 0., 0.1, -0.1, -0.1, 0.1, 0.0, 0.1, -0.1, -0.1, 0.1]))
keyframe.append(np.array([-0.1, 0., 0.1, -0.1, -0.1, 0.1, 0.0, 0.1, -0.1, -0.1, 0.1]))



"""
# x-values to be interpolated
keytime = np.linspace(0, 200, 21)
# y-values to be interpolated
keyframe = []
keyframe.append(np.array([0.,-0.05, -0.2,-0.2, 0.2,-0.2, 0.25, -0.3, 0.3, 0.1, 0.2]))
keyframe.append(np.array([0.,0.0, 0.2,-0.1, -0.2,-0.1, 0.1, 0.1,0.2, -0.3,0.3]))
for i in range(9):
    keyframe.append(keyframe[0])
    keyframe.append(keyframe[1])
keyframe.append(keyframe[0])
"""

assert(len(keytime) == len(keyframe))

def interpolateSpline(x, y) :
    """
    Intepolate the Runge function using a spline.

    :param n: number of interpolation points
    :return: list of local polynomials as np.poly1d objects
    """
    n = len(x)

    dim = 4 * (n - 1)
    b = np.zeros((dim, 1))
    A = np.zeros((dim, dim))

    for i in range(n-1):
        x1 = x[i]
        x2 = x[i+1]
        y1 = y[i]
        y2 = y[i+1]
        b[i*4:(i+1)*4, 0] = [y1, y2, 0, 0]

        A[i*4, i*4:(i+1)*4] = [pow(x1,3), pow(x1,2), x1, 1] 
        A[i*4+1, i*4:(i+1)*4] = [pow(x2,3), pow(x2,2), x2, 1]
        if (i != n-2):
            A[i*4+2, i*4:(i+2)*4] = [3*pow(x2,2), 2 * x2, 1, 0, -3*pow(x2,2), -2 * x2, -1, 0, ]
            A[i*4+3, i*4:(i+2)*4] = [6*x2, 2, 0, 0, -6*x2, -2, 0, 0]
        else: 
            A[i*4+2, 0:4] = [6*x[0], 2, 0, 0]
            A[i*4+3, i*4:(i+1)*4] = [6*x2, 2, 0, 0]
         
    # solve linear system for the coefficients of the spline
    coeffs = np.linalg.solve(A, b)

    # extract local pieces
    spline = []
    for k in range(n-1):
        spline.append(np.poly1d(coeffs[k*4:(k+1)*4, 0]))

    return spline

def interpolatePeriodicSpline(x, y) :
    """
    Intepolate the Runge function using a spline.

    :param n: number of interpolation points
    :return: list of local polynomials as np.poly1d objects
    """
    n = len(x)

    dim = 4 * (n - 1)
    b = np.zeros((dim, 1))
    A = np.zeros((dim, dim))

    for i in range(n-1):
        x1 = x[i]
        x2 = x[i+1]
        y1 = y[i]
        y2 = y[i+1]
        b[i*4:(i+1)*4, 0] = [y1, y2, 0, 0]

        A[i*4, i*4:(i+1)*4] = [pow(x1,3), pow(x1,2), x1, 1] 
        A[i*4+1, i*4:(i+1)*4] = [pow(x2,3), pow(x2,2), x2, 1]
        if (i != n-2):
            A[i*4+2, i*4:(i+2)*4] = [3*pow(x2,2), 2 * x2, 1, 0, -3*pow(x2,2), -2 * x2, -1, 0, ]
            A[i*4+3, i*4:(i+2)*4] = [6*x2, 2, 0, 0, -6*x2, -2, 0, 0]
        else: 
            A[i*4+2, 0:4] = [3 * pow(x[0],2), 2 * x[0], 1, 0]
            A[i*4+2, i*4:(i+1)*4] = [-3 * pow(x2,2), -2 * x2, -1, 0]
            A[i*4+3, 0:4] = [6 * x[0], 2, 0, 0]
            A[i*4+3, i*4:(i+1)*4] = [-6 * x2, -2, 0, 0]
    # solve linear system for the coefficients of the spline
    coeffs = np.linalg.solve(A, b)

    # extract local pieces
    spline = []
    for k in range(n-1):
        spline.append(np.poly1d(coeffs[k*4:(k+1)*4, 0]))

    return spline

def interpolateCubicNatural() :
    """
    Interpolate keyframes using a cubic spline with natural boundary conditions.

    :return: local cubic interpolants as python list
    """

    S = []

    # for all parameters
    for i in range(11):
        y = []
        # get i-th paramter
        for k in range(len(keyframe)):
            y.append(keyframe[k][i])

        interpolants = interpolateSpline(keytime, y)
        S.append(interpolants)
    return S


def interpolateCubicPeriodic() :
    """
    Interpolate keyframes using a cubic spline with periodic boundary conditions.

    :return: local cubic interpolants as python list
    """

    S = []

    # for all parameters
    for i in range(11):
        y = []
        # get i-th parameter
        for k in range(len(keyframe)):
            y.append(keyframe[k][i])

        interpolants = interpolatePeriodicSpline(keytime, y)
        S.append(interpolants)
    return S


def interpolateLinear( t):
    """
    Compute interpolated keyframe curframe at given time t.

    :param t: timing of the interpolated curframe
    :return: interpolated frame at time t as np.array (11 dim)
    """
    k = np.searchsorted( keytime, t, side='right') - 1
    u = (t - keytime[k]) / (keytime[k + 1] - keytime[k])
    curframe = (1.0-u)*keyframe[k] + u*keyframe[k+1]

    return curframe


def interpolateCubic( t):
    """
    Compute interpolated keyframe curframe at given time t.

    :param t: timing of the interpolated curframe
    :return: interpolated frame at time t as np.array (11 dim)
    """
    curframe = []
    frame = np.searchsorted( keytime, t, side='right') - 1

    for i in range(11):
        poly = S[i]
        res = poly[frame](t)
        curframe.append(res)

    return curframe


# compute cubic interpolation with natural boundary conditions
# S_natural = interpolateCubicNatural()
S_periodic = interpolateCubicPeriodic()

# decide which cubic interpolation to use
# S = S_natural
S = S_periodic

# decide on interpolation method
# stickguy.animate( keytime, keyframe, interpolateLinear)
stickguy.animate( keytime, keyframe, interpolateCubic)

