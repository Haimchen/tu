# coding=utf-8
import numpy as np
import math
from numpy import dot
from operator import itemgetter

'''
    Dieses Paket dient einzig der Erstellung des Tofts.
    Hier sind keine Aufgaben zu bearbeiten. (Do NOT touch this)
'''

class Ellipse:
    def __init__(self, d, a, b, x0, y0, phi):
        self.d = d
        self.a = a
        self.b = b
        self.x0 = x0
        self.y0 = y0
        self.phi = phi

        sn = np.sin(-np.radians(phi))
        cs = np.cos(-np.radians(phi))
        R = np.array([[cs, -sn], [sn, cs]])
        D = np.array([[1.0/(a ** 2), 0], [0, 1.0/(b ** 2)]])
        C = np.array([x0, y0])

        self.A = dot(R.T, dot(D, R))
        self.B = dot(self.A, C)
        self.c = dot(self.B, C)

    def contains(self, x, y):
        X = np.array([x, y])
        return dot(X, dot(self.A,X)) - 2.0 * dot(self.B, X) + self.c <= 1

    def intersect(self, P, d):
        e2 = dot(d, dot(self.A, d))
        e1 = dot(d, 2.0 * dot(self.A, P) - 2.0 * self.B)
        e0 = dot(P, dot(self.A, P)) - 2.0 * dot(self.B, P) + self.c - 1
        t = np.roots([e2, e1, e0])
        t.sort()
        return t if t.dtype.kind == 'f' else None


# Toft (1996), p. 199
toft = [Ellipse(1.00, .6900, .9200,   .0,     .0,   .0),
        Ellipse(-.80, .6624, .8700,   .0, -.0184,   .0),
        Ellipse(-.20, .1100, .3100,  .22,     .0,  -18),
        Ellipse(-.20, .1600, .4100, -.22,     .0,   18),
        Ellipse( .10, .2100, .2500,   .0,   .350,   .0),
        Ellipse( .10, .0460, .0460,   .0,   .100,   .0),
        Ellipse( .10, .0460, .0460,   .0,  -.100,   .0),
        Ellipse( .10, .0460, .0230, -.08,  -.605,   .0),
        Ellipse( .10, .0230, .0230,   .0,  -.606,   .0),
        Ellipse( .10, .0230, .0460,  .06,  -.605,   .0)]


def phantom(n):
    I = np.zeros ((n, n))
    yg, xg = np.mgrid[-1:1:n*1j, -1:1:n*1j]
    for e in toft:
        asq = e.a ** 2
        bsq = e.b ** 2
        x = xg - e.x0
        y = yg - e.y0
        cosp = np.cos(np.radians(e.phi))
        sinp = np.sin(np.radians(e.phi))
        I[(((x*cosp + y*sinp)**2)/asq + ((y*cosp - x*sinp)**2)/bsq) <= 1.0] += e.d
    return I


def intersect(rp, rd):
    S = []
    for e in toft:
        T = e.intersect(rp, rd)
        if T != None:
            for t in T: S.append(rp + t * rd)
    return S


def trace(r, d):
    """
    Gibt die die abgeschwächte Intensität I_1 eines Strahles mit Startpunkt r und
    Richtung d zurück. Als Eingangsintensität wird dabei I_0=1 angenommen.
    """
    L = []
    for e in toft:
        T = e.intersect(r, d)
        if T is not None:
            L.append([T[0], e.d])
            L.append([T[1], -e.d])

    L.sort(key=itemgetter(0))
    rho = 0
    Vk = 0
    for i in range(len(L)-1):
        rho += L[i][1]
        l = L[i+1][0] - L[i][0]
        Vk += rho * l
    return math.exp(-Vk)


def grid_intersect(n, r, d):
    """
    Berechnet die Schnitte eines Rasters mit einer Menge vorgegebener
    paralleler Strahlen.

    Parameter:
    n  : Höhe bzw. Breite des für die Rekonstruktion verwendeten Rasters
    r  : Startpunkte der Strahlen; gespeichert als Zeilen einer Matrix (ndarray)
    d  : Richtungsvektor der Strahlen (array_like)

    Rückgabe:
    I  : Array mit Indizes der Strahlen (ndarray)
    G  : Array mit Indizes der geschnittenen Quadranten (ndarray)
    dt : Array mit Längen der im Quadranten verlaufenen Schnitte (ndarray)
    px : Array mit x-Koordinaten der Startpunkte des im Quadranten
         verlaufenen Schnittes (ndarray)
    px : Array mit y-Koordinaten der Startpunkte des im Quadranten
        verlaufenen Schnittes (ndarray)
    """
    oldwarn = np.seterr(invalid='ignore')
    nsamples = r.shape[0]

    x0 = np.tile(r[:,0], (n+1,1)).T
    y0 = np.tile(r[:,1], (n+1,1)).T

    invd = np.zeros((2,))
    for i in range(2):
        invd[i] = 1.0 / d[i] if d[i] != 0.0 else np.nan

    xx = np.tile(np.linspace(-1, 1, n+1), (nsamples,1))
    tx = (xx - x0) * invd[0]
    y = y0 + tx * d[1]
    tx[np.logical_or(y < -1.0, y >= 1.0)] = np.nan

    yy = xx
    ty = (yy - y0) * invd[1]
    x = x0 + ty * d[0]

    ty[np.logical_or(x < -1.0, x >= 1.0)] = np.nan

    t = np.hstack((tx, ty))
    t[np.logical_not(np.isfinite(t))] = np.nan
    x0 = np.hstack((x0, x0))
    y0 = np.hstack((y0, y0))

    R, _ = np.indices(t.shape)
    I = t.argsort(axis=1)
    t = t[R,I]
    x0 = x0[R,I]
    y0 = y0[R,I]
    t[np.fabs(np.diff(t)) < 1e-12] = np.nan
    I = t.argsort(axis=1)
    t = t[R,I]
    x0 = x0[R,I]
    y0 = y0[R,I]

    dt = np.hstack((np.diff(t), np.nan * np.ones((nsamples,1))))
    px = x0 + t * d[0]
    py = y0 + t * d[1]
    I,J = np.isfinite(dt).nonzero()

    px = px[I,J]
    py = py[I,J]
    dt = dt[I,J]

    ix = np.floor((1 + px + 0.5 * dt * d[0]) * n / 2).astype(int)
    iy = np.floor((1 + py + 0.5 * dt * d[1]) * n / 2).astype(int)
    G = n * iy + ix

    np.seterr(**oldwarn)
    return I, G, dt, px, py
