# -*- coding: utf-8 -*-
"""
@author: lessig
"""

import numpy as np
import scipy.special
from scipy.interpolate import interp1d


def f1(x) :
    """
    Test function f(x) on [-1,1]: constant function.

    Inputs:
    x :  positions x_i where to evaluate f(x) (numpy array of dim (n,) or scalar)

    Return:
    f : f(x_i) for given x_i (same as x)
    """

    if isinstance( x, np.ndarray) :
        f = 0.5 * np.ones(x.size)
    else :
        f = 0.5

    return f


def f2(x_in) :
    """
    Test function f(x) on [-1,1]: piecewise spline with singularities.

    Inputs:
    x :  positions x_i where to evaluate f(x) (numpy array of dim (n,) or scalar)

    Return:
    f : f(x_i) for given x_i (same as x)
    """

    # work-around for the function to work on 
    # scalar x
    input_is_array = True
    if not isinstance( x_in, np.ndarray) :
        input_is_array = False
        x_in = np.array( [x_in])

    # return value
    f = np.zeros( x_in.size)

    # evaluate piecewise segments

    idx = (x_in >= -1.0) & (x_in <= -0.3)
    x = x_in[idx]
    xx = np.linspace( -1.0, -0.3, 8)
    yy = np.array([-0.3, -0.1, 0.4, 0.2, 0.0, -0.6, -0.6, -0.1])
    ff = interp1d( xx, yy, kind='cubic')
    f[idx] = ff(x)

    idx = (x_in >= -0.3) & (x_in <= 0.17)
    x = x_in[idx]
    xx = np.array( [-0.3, -0.2, -0.1, 0.0, 0.1, 0.17])
    yy = np.array( [0.5, 0.2, 0.3, 0.2, 0.4, 1.5])
    ff = interp1d( xx, yy, kind='cubic')
    f[idx] = ff(x)

    idx = (x_in >= 0.17) & (x_in <= 0.8)
    x = x_in[idx]
    xx = np.array( [0.17, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8])
    yy = np.array( [1.5, -0.1, -0.3, -0.3, -0.12, 0.05, 0.1, 0.1])
    ff = interp1d( xx, yy, kind='cubic')
    f[idx] = ff(x)

    idx = (x_in >= 0.8) & (x_in <= 1.0)
    x = x_in[idx]
    xx = np.array( [ 0.8, 0.9, 0.95, 1.0])
    yy = np.array( [ 0.3, 0.2, -0.05, -0.1])
    ff = interp1d( xx, yy, kind='cubic')
    f[idx] = ff(x)

    # work around for scalar x
    if not input_is_array :
        f = f[0]

    return f


def f3(x) :
    """
    Test function f(x) on [-1,1]: step function.

    Inputs:
    x :  positions x_i where to evaluate f(x) (numpy array of dim (n,) or scalar)

    Return:
    f : f(x_i) for given x_i (same as x)
    """

    # work-around for the function to work on
    # scalar x
    input_is_array = True
    if not isinstance( x, np.ndarray) :
        input_is_array = False
        x = np.array( [x])

    heights = np.array( [ 0.2, 0.3, 0.35, 0.25, 0.1, -0.05, -0.15, -0.2,
                         -0.25, -0.1, 0.0, 0.2, 0.5, 0.7, 0.6, 0.5,
                          0.4, 0.35, 0.3, 0.35, 0.375])
    x_lims = np.linspace( -1.0, 1.0, 21)

    f = np.zeros( x.size)

    # fill in function values
    for i in range( x_lims.size - 1) :
        # find x values in current segment
        idx = (x > x_lims[i]) & (x <= x_lims[i+1]);
        f[idx] = heights[i];

    # work around for scalar x
    if not input_is_array :
        f = f[0]

    return f


def f4(x) :

    """
    Test function f(x) on [-1,1]: smooth, finite dimensional polynomial.

    Inputs:
    x :  positions x_i where to evaluate f(x) (numpy array of dim (n,) or scalar)

    Return:
    f : f(x_i) for given x_i (same as x)
    """

    # work-around for the function to work on
    # scalar x
    input_is_array = True
    if not isinstance( x, np.ndarray) :
        input_is_array = False
        x = np.array( [x])

    N = 10
    # coeffs = np.random.randn( N)
    # fix coefficients for reproducibility
    coeffs = np.array( [-1.14567827e+00,   3.28835810e-01,   1.11050601e+00,
                        -6.19385469e-02,  -1.89831963e+00,  -1.21694241e+00,
                         3.88707895e-01,  -1.88918142e-03,  -3.56023186e-02,
                        -1.40101031e+00])

    # construct function by summing together contributions from
    # different basis functions
    f = np.zeros( x.size)
    for i in range(N) :
        P_i = scipy.special.legendre( i)
        f += coeffs[i] * P_i(x)

    # work around for scalar x
    if not input_is_array :
        f = f[0]

    return f

def gauss(x) :
    """
    Test function f(x) on [-1,1]: Gauss bell with \sigma = 0.2 .

    Inputs:
    x :  positions x_i where to evaluate f(x)

    Return:
    f : f(x_i) for given x_i (same data type and shape as x)
    """

    sigma = 0.5

    f = 1.0 / (sigma * np.sqrt( 2.0 * np.pi)) * np.exp( -(x*x) / (2. * sigma * sigma))

    return f
