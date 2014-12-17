import numpy as np
import matplotlib.pyplot as plt

n = 8

def interpolateRungeSpline( n = 8) :
    """
    Intepolate the Runge function using a spline.

    :param n: number of interpolation points
    :return: list of local polynomials as np.poly1d objects
    """

    # interpolation points
    # TODO: Interpolationspunkte erzeugen

    # construct linear system
    # TODO: lineares Gleichungssystem erstellen

    # solve linear system for the coefficients of the spline
    # TODO: lineares Gleichungssystem loesen

    # extract local pieces
    spline = []
    # TODO: lokale Polynome extrahieren und np.poly1d Okjekte erzeugen

    return spline


n = 8
spline = interpolateRungeSpline( n)

##################################################
# Plotting

# Plot Runge function
rx = np.linspace( -5.0, 5.0, 100)
ry = 1.0 / (1.0 + rx**2)
plt.plot(rx, ry, '--', color='0.7', zorder=0)

# Plot interpolation points
x = np.linspace(-5.0, 5.0, n)
y = 1.0 / (1 + x**2)
plt.plot(x, y, 'ko')

# Plot spline interpolation
x = np.linspace(-5.0, 5.0, n)
for i in range(n-1):
    p = spline[i]
    tx = np.linspace( x[i], x[i+1], 20)
    ty = p(tx)
    plt.plot(tx, ty, '-')

# Beautify plot
plt.grid(True)
plt.xlim(-5.1,5.1)
plt.xticks(np.linspace(-5,5,11))
plt.ylim(-0.1,1.1)
plt.subplots_adjust(left=0.05, right=0.98, top=0.98, bottom=0.05)

# Save plot
plt.savefig('./figures/aufgabe1d.pdf')
plt.show()
