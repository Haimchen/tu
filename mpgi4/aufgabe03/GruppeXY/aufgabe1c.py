import numpy as np
import matplotlib.pyplot as plt

def pcubic(x, y, yp):
    """

    :param x: x-values of interpolation points
    :param y: y-values of interpolation points
    :param yp: derivative values of interpolation points
    :return: interpolation polynomial as np.poly1d object
    """
  
    a = 0
    # TODO: Kubisches Interpolationspolynom erzeugen

    return np.poly1d(a)

def pcubicInterpolationRunge( n = 8) :
    """
    Compute cubic

    :param n: number of pieces used for cubic interpolation
    :return: interpolation points and piecewise polynomials as np.poly1d objects
    """

    # Determine input data for interpolation
    # TODO: Interpolationspunkte generieren sowie benoetigte Ableitungen
    sx = sy = syp = 0

    # Compute and plot piecewise interpolants
    ps = []
    for i in range(n-1):
        # compute local interpolant
        # TODO: Stueckweisen Interpolanten berechnen
        ps.append( [])

    return sx, sy, syp, ps


# compute piecewise interpolation
n = 8
sx, sy, sz, ps = pcubicInterpolationRunge( n)

##################################################
# Plotting

# Plot interpolation points
plt.plot( sx, sy, 'ko')

# Plot piecewise interpolants
for i in range(n-1):

    # plot local interpolant
    p = ps[i]
    px = np.linspace( sx[i], sx[i+1], 100/n)
    py = p( px)
    plt.plot( px, py, '-')

# Plot Runge function
rx = np.linspace(-5, 5, 100)
ry = 1.0 / (1 + rx**2)
plt.plot(rx, ry, '--', color='0.7')

# Beautify plot
plt.grid(True)
plt.xlim(-5.1,5.1)
plt.xticks(np.linspace(-5,5,11))
plt.ylim(-0.1,1.1)
plt.subplots_adjust(left=0.05, right=0.98, top=0.98, bottom=0.05)

# Save plot
plt.savefig('./figures/aufgabe1c.pdf')
plt.show()

