# -*- coding: utf-8 -*-
"""
@author: lessig
"""

# import system
import numpy as np
import matplotlib.pyplot as plt
import scipy.integrate

# import user
import signals as sig


def quadTrapez( signal, n_quad) :
    """"
    Quadrature approximation of \int_{-1}^1 f(x) dx using trapez rule.

    Inputs:
    signal : function f(x) whose integral is sought
    n_quad : number of quadrature points

    Return:
    integral : approximation to integral value

    """

    integral = 0.0

    # TODO: Implement trapez rule.

    return integral


def quadSimpson( signal, n_quad) :
    """"
    Quadrature approximation of \int_{-1}^1 f(x) dx using trapez rule.

    Inputs:
    signal : function f(x) whose integral is sought.
    n_quad : number of quadrature points.

    Return:
    integral: approximation of integral value

    """

    integral = 0.0

    # TODO: Implement Simpson's rule.

    return integral


def plotSignals( signals) :
    """
    Plot a set of functions / signals.
    """

    x = np.linspace( -1.0, 1.0, 1000)

    plt.figure()
    # axes
    plt.plot( [-1.0, 1.0], [0.0, 0.0], '-k', alpha=0.25)
    plt.plot( [0.0, 0.0], [-3.0, 3.0], '-k', alpha=0.25)

    legend_str = []
    for i, signal in enumerate(signals) :
        y = signal(x)
        plt.plot( x, y, '-')
        legend_str.append( signal.__name__)

    plt.legend( legend_str)
    plt.xlim( -1.0, 1.0)
    plt.ylim( -3.0, 3.0)
    plt.show()


def plotErrors( ints, ints_refs, signals, ns, quadrules) :
    """
    Plot the quadrature error for each signal for the different methods.

    Inputs:
    ints : integral values computed with quadrature.
    ints_refs : reference solutions for integral values.
    signals : list of signal (function handles).
    ns : list with numbers of quadrature points.
    quadrules : list with quadrature rules (function handles)

    Returns:
    None.

    Side effects:
    Generates:
      ./figures/quad_err_f2.pdf
      ./figures/quad_err_f3.pdf
      ./figures/quad_err_f3.pdf
      ./figures/quad_err_gauss.pdf
    """

    # plot convergence
    for i in range( 1, len(signals)) :

        plt.figure()

        # compute errors
        errs = np.abs( ints[i,:,:] - ints_refs[i])

        legend_str = []
        for j in range( len(quadrules)) :
            plt.plot( ns, errs[:,j], '-x')
            legend_str.append( quadrules[j].__name__)

        plt.legend( legend_str)
        plt.yscale( 'log')
        plt.xscale( 'log')
        plt.xlim( np.min(ns), np.max(ns))
        plt.title( 'Signal: ' + signals[i].__name__)

        plt.savefig( './figures/quad_err_' + signals[i].__name__ + '.pdf')
        plt.show()


def main() :

    # number of quadrature points
    ns = [10, 50, 100, 500, 1000, 10000]

    # test signals
    signals = [ sig.f1, sig.f2, sig.f3, sig.f4, sig.gauss ]
    plotSignals( signals)

    # quadrature rules
    quadrules = [ quadTrapez, quadSimpson]

    # array to store results
    ints = np.zeros( (len(signals), len(ns), len(quadrules)) )

    # for all test signals
    for i, signal in enumerate(signals) :

        # for all numbers of quadrature points
        for j, n in enumerate(ns) :

            # for all quadrature rules
            for k, quadrule in enumerate(quadrules) :

                # evaluate quadrature rule
                ints[i,j,k] = quadrule( signal, n)


    # compute reference solutions
    ints_refs = np.zeros( len(signals))
    for i, signal in enumerate(signals) :
        (val,err) = scipy.integrate.quad( signal, -1.0, 1.0, limit=150)
        ints_refs[i] = val

    # plot errors
    plotErrors( ints, ints_refs, signals, ns, quadrules)

main()
