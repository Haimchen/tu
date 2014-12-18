import numpy as np
import matplotlib.pyplot as plt

def pcubic(x, y, yp):
    """

    :param x: x-values of interpolation points
    :param y: y-values of interpolation points
    :param yp: derivative values of interpolation points
    :return: interpolation polynomial as np.poly1d object
    """
    #print("x is", x)
    print("y is ", y)
    print("yp ", yp)
    a = 0
    # TODO: Kubisches Interpolationspolynom erzeugen
    vec = np.reshape(np.array((y, yp)), -1, 1)    
    m = np.array([[pow(x[0],3), pow(x[0],2), x[0], 1], [3*pow(x[0],2), 2 * x[0], 1, 0], [pow(x[1],3), pow(x[1],2), x[1], 1],  [3*pow(x[1],2), 2 * x[1], 1, 0]])    

    a = np.linalg.solve(m, vec)
    print("vec ", vec)
    return np.poly1d(a)

def pcubicInterpolationRunge( n = 8) :
    """
    Compute cubic

    :param n: number of pieces used for cubic interpolation
    :return: interpolation points and piecewise polynomials as np.poly1d objects
    """

    # Determine input data for interpolation
    # TODO: Interpolationspunkte generieren sowie benoetigte Ableitungen
    sx = np.linspace(-5, 5, n, endpoint=True)
    sy = 1 / (1 + (sx * sx))
    syp = -2 * sx / (pow(1 + pow(sx, 2), 2))

    # Compute and plot piecewise interpolants
    ps = []
    for i in range(n-1):
        # compute local interpolant
        # TODO: Stueckweisen Interpolanten berechnen
        ps.append(pcubic(sx[i:i+2], sy[i:i+2], syp[i:i+2]))

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

