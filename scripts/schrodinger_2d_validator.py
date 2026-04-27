#!/usr/bin/env python3
"""
Validador Python de orbitais 2D.
Compare os plots com o programa C++ pra ver se a física bate.

Requer: pip install numpy scipy matplotlib
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy.special import genlaguerre, lpmv, factorial

def radial(n, l, r, Z=1):
    """Parte radial R_nl(r)"""
    if l >= n or l < 0 or n < 1:
        return 0.0
    rho = 2 * Z * r / n
    norm = np.sqrt((2*Z/n)**3 * factorial(n-l-1) / (2*n*factorial(n+l)))
    lag = genlaguerre(n-l-1, 2*l+1)(rho)
    return norm * np.exp(-rho/2) * rho**l * lag

def angular(l, m, theta, phi):
    """Harmônico esférico real Y_lm"""
    abs_m = abs(m)
    norm = np.sqrt((2*l+1)*factorial(l-abs_m)/(4*np.pi*factorial(l+abs_m)))
    leg = lpmv(abs_m, l, np.cos(theta))
    if m > 0:
        return np.sqrt(2) * norm * leg * np.cos(m*phi)
    elif m < 0:
        return np.sqrt(2) * norm * leg * np.sin(abs_m*phi)
    else:
        return norm * leg

def psi_2d(n, l, m, x, z, Z=1):
    """ψ(x, z) no plano xz"""
    r = np.sqrt(x**2 + z**2)
    theta = np.arctan2(np.abs(x), z)
    return radial(n, l, r, Z) * angular(l, m, theta, 0.0)

def plot(n, l, m, Z=1, extent=15):
    """Plota |ψ|²"""
    x = np.linspace(-extent, extent, 400)
    z = np.linspace(-extent, extent, 400)
    X, Z = np.meshgrid(x, z)
    psi = np.vectorize(lambda xi, zi: psi_2d(n, l, m, xi, zi, Z))(X, Z)
    plt.figure(figsize=(8, 7))
    plt.imshow(psi**2, extent=[-extent, extent, -extent, extent],
               origin='lower', cmap='hot')
    plt.colorbar(label='|ψ|²')
    plt.title(f'Orbital {n}{"spdf"[l]} (m={m}) — Z={Z}')
    plt.xlabel('x (Bohr)')
    plt.ylabel('z (Bohr)')
    plt.show()

if __name__ == '__main__':
    plot(2, 1, 0, Z=1)  # 2p do hidrogênio
    plot(2, 1, 0, Z=8)  # 2p do oxigênio (aproximação)
