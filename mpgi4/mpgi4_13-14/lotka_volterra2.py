# coding=utf-8
import numpy as np
import matplotlib.pyplot as plt


def plot_phasespace_and_solution(u0=1, v0=0.5):
    u,v = np.meshgrid(np.arange(0, 5, 0.25), np.arange(0, 6, 0.25))
    up = u * (v - 2)
    vp = v * (1 - u)
    lp = np.sqrt(up**2 + vp**2)
    I = lp > 0
    up[I] /= lp[I]
    vp[I] /= lp[I]
    plt.quiver(u, v, up, vp)

    u,v = np.meshgrid(np.arange(0.05, 5, 0.05), np.arange(0.05, 6, 0.05))
    z = np.log(u) - u + 2 * np.log(v) - v
    z0 = np.log(u0) - u0 + 2 * np.log(v0) - v0
    plt.contour(u, v, z, [z0,])


def lotka_volterra_explicit(u0=1, v0=0.5, N=100, h=0.05):
    """Integration der Lotka-Volterra Gleichung mit dem expliziten Euler-Verfahren.

    Argumente:
    u0 -- Anfangswert u
    v0 -- Anfangswert v
    N  -- Anzahl der Schritte
    h  -- Schrittweite

    Rückgabewert:
    Numpy Array der Dimension (2,N+1), [:,k] = (u_k, v_k)
    """
    I = np.empty((2,N+1))
    ui = u0
    vi = v0
    I[:,0] = ui, vi
    for k in range(N):
        uj = ui + h * ui * (vi - 2)
        vj = vi + h * vi * (1 - ui)
        ui, vi = uj, vj
        I[:,k+1] = ui, vi
    return I


def lotka_volterra_implicit(u0=1, v0=0.5, N=100, h=0.05):
    """Integration der Lotka-Volterra Gleichung mit dem impliziten Euler-Verfahren.

    Argumente:
    u0 -- Anfangswert u
    v0 -- Anfangswert v
    N  -- Anzahl der Schritte
    h  -- Schrittweite

    Rückgabewert:
    Numpy Array der Dimension (2,N+1), [:,k] = (u_k, v_k)
    """
    I = np.empty((2,N+1))
    ui = u0
    vi = v0
    I[:,0] = ui, vi
    for k in range(100):
        alpha = (1 + h * (1 + ui + vi) - 2 * h**2) / (h**2 - h)
        beta = -vi * (1 + 2*h) / (h**2 - h)
        r = np.roots([1, alpha, beta])
        if abs(r[0]-vi) < abs(r[1]-vi):
            vj = r[0]
        else:
            vj = r[1]
        uj = ui / (1 - h * (vj - 2))
        ui, vi = uj, vj
        I[:,k+1] = ui, vi
    return I


def lotka_volterra_rk4(u0=1, v0=0.5, N=100, h=0.05):
    """Integration der Lotka-Volterra Gleichung mit Runge-Kutta 4. Ordnung.

    Argumente:
    u0 -- Anfangswert u
    v0 -- Anfangswert v
    N  -- Anzahl der Schritte
    h  -- Schrittweite

    Rückgabewert:
    Numpy Array der Dimension (2,N+1), [:,k] = (u_k, v_k)
    """
    def f(yi):
        return np.array([ yi[0] * (yi[1] - 2), yi[1] * (1 - yi[0])])

    I = np.empty((2,N+1))
    yi = np.array([u0, v0])
    I[:,0] = yi
    for k in range(N):
        k1 = f(yi)
        k2 = f(yi + h/2.0 * k1)
        k3 = f(yi + h/2.0 * k2)
        k4 = f(yi + h * k3)
        yi = yi + h/6.0 * (k1 + 2.0*k2 + 2.0*k3 + k4)
        I[:,k+1] = yi
    return I

plt.subplot(1,3,1)
plot_phasespace_and_solution()
I = lotka_volterra_explicit()
plt.plot(I[0,0],I[1,0], 'ko')
plt.plot(I[0,:], I[1,:], 'rx-')
plt.gca().set_aspect('equal')
plt.xlabel('Euler explizit')

plt.subplot(1,3,2)
plot_phasespace_and_solution()
I = lotka_volterra_implicit()
plt.plot(I[0,0],I[1,0], 'ko')
plt.plot(I[0,:], I[1,:], 'rx-')
plt.gca().set_aspect('equal')
plt.xlabel('Euler implizit')

plt.subplot(1,3,3)
plot_phasespace_and_solution()
I = lotka_volterra_rk4()
plt.plot(I[0,0],I[1,0], 'ko')
plt.plot(I[0,:], I[1,:], 'rx-')
plt.gca().set_aspect('equal')
plt.xlabel('RK4')

plt.subplots_adjust(left=0.03, right=0.99, top=0.99, bottom=0.15)
plt.gcf().set_size_inches(7,3)
plt.savefig('../figures/lotka-volterra.pdf')
plt.show()
