 # -*- coding: utf-8 -*-
import numpy as np
import stickguy


# 21 key frames to be interpolated

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
assert(len(keytime) == len(keyframe))


def interpolateCubicNatural() :
    """
    Interpolate keyframes using a cubic spline with natural boundary conditions.

    :return: local cubic interpolants as python list
    """

    S = []

    # for all parameters
    for i in range(11):

        # TODO: i-ten Parameter extrahieren.

        # TODO: Interpolation
        interpolants = []
        S.append(interpolants)

    return S

def interpolateCubicPeriodic() :
    """
    Interpolate keyframes using a cubic spline with natural boundary conditions.

    :return: local cubic interpolants as python list
    """

    S = []

    # for all parameters
    for i in range(11):

        # TODO: i-ten Parameter extrahieren.

        # TODO: Interpolation
        interpolants = []
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

    # TODO: Evaluation des Interpolationspolynoms zur Zeit t
    curframe = 0

    return curframe


# compute cubic interpolation with natural boundary conditions
S_natural = interpolateCubicNatural()
S_periodic = interpolateCubicPeriodic()

# decide which cubic interpolation to use
S = S_natural
# S = S_periodic

# decide on interpolation method
stickguy.animate( keytime, keyframe, interpolateLinear)
# stickguy.animate( keytime, keyframe, interpolateCubic)
