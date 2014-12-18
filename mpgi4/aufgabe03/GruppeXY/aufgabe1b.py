import numpy as np
import matplotlib.pyplot as plt

def lagrange_intpoly(x, y):
    """
    Generate Lagrange interpolation polynomial.

    :param x: x-values of interpolation points
    :param y: y-values of interpolation points
    :return: polynomial as np.poly1d object
    """

    assert(len(x) == len(y))
    
    p = np.poly1d([1, 2, 3])
    # TODO: Kontruktion des Interpolationspolynoms

    return p

def interpolateRungeLagrange() :
    """
    Generate interpolation polynomials for Runge function

    :return: interpolation polynomials as np.poly1d objects
    """

    ps_lagrange = []
    for n in range(3,12):

        # generate interpolating polynomial
        # TODO: Interpolationspunkte generieren und Interpolationspolynom erzeugen
        x = np.linspace(-5, 5, n, endpoint=True) 
        y = 1 / (1 + (x * x))
        polynomial = lagrange_intpoly(x, y)
        ps_lagrange.append(polynomial)

    return ps_lagrange


# compute interpolation polynomials
ps_lagrange = interpolateRungeLagrange()

##################################################
# Plotting

rx = np.linspace( -5.0, 5.0, 100)
ry = 1.0 / (1.0 + rx**2)

# plot current interpolation polynomial
for i in range(len(ps_lagrange)):

    # sampling points
    xi = np.linspace( -5.0, 5.0, i+3)
    yi = 1.0 / (1.0 + xi**2)

    # evaluate polynomial
    p = ps_lagrange[i]
    py = p(rx)
    
    plt.subplot(3,3, i+1)
    plt.grid(True)
    plt.plot(rx, ry, 'r-')
    plt.plot(xi, yi, 'ko')
    plt.plot(rx, py, 'b-')
    plt.xlim(-5.2,5.2)
    plt.xticks([-5,0,5])
    plt.ylim(-1.0,2.0)

# Beautify and save plot
plt.subplots_adjust(left=0.05, right=0.97, top=0.98, bottom=0.05)
plt.savefig('./figures/aufgabe1b.pdf')
plt.show()

