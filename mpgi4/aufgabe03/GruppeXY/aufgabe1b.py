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
    n = len(x)
    
    # TODO: Kontruktion des Interpolationspolynoms
  
    for j in range (0, n):
        factor = 1
        arr = [] 
        for k in range(0, n):
            if (k == j):
                continue;
            arr.append(x[k])
            factor = factor * (1/(x[j]-x[k]))
        lj = np.poly1d(arr, True)
        lj = lj * factor * y[j]
        if (j == 0):
            p = lj

        else:
            p = p + lj
                
    return p

def interpolateRungeLagrange() :
    """
    Generate interpolation polynomials for Runge function

    :return: interpolation polynomials as np.poly1d objects
    """

    ps_lagrange = []
    for n in range(3,12):

        # generate interpolating polynomial
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

