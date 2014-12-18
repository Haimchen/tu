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
    x = np.linspace(-5, 5, n, endpoint=True) 
    y = 1 / (1 + (x * x))
    # FIXME
    # yp1 = (- 2 * x) / (pow((1 + (x * x)), 2))
    # yp2 = (8 * pow(x, 2)) / pow((pow(x, 2) + 1), 3) - (2 / (pow((1 + (x * x)), 2)))

    # construct linear system
    # TODO: lineares Gleichungssystem erstellen
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
    # TODO: lineares Gleichungssystem loesen
    coeffs = np.linalg.solve(A, b)
    print(coeffs.shape)
    # extract local pieces
    spline = []
    # TODO: lokale Polynome extrahieren und np.poly1d Okjekte erzeugen
    for k in range(n-1):
        spline.append(np.poly1d(coeffs[k*4:(k+1)*4, 0]))

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
