import matplotlib
matplotlib.use('Agg')
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
 
# ── Leer datos 
df = pd.read_csv("calor_serial.csv")
 
tiempos = sorted(df["t"].unique())
x_vals  = sorted(df["x"].unique())
 
X_grid = np.array(x_vals)
T_grid = np.array(tiempos)
X_mesh, T_mesh = np.meshgrid(X_grid, T_grid)
 
U_num   = np.zeros((len(tiempos), len(x_vals)))
U_exact = np.zeros((len(tiempos), len(x_vals)))
 
for ti, t in enumerate(tiempos):
    fila = df[df["t"] == t].sort_values("x")
    U_num[ti, :]   = fila["u_num"].values
    U_exact[ti, :] = fila["u_exact"].values
 

#  GRÁFICA 1: Comparación numérica vs exacta al tiempo final

fig, axes = plt.subplots(1, 2, figsize=(12, 4))
 
ax = axes[0]
ax.plot(x_vals, U_num[-1, :],   "b--", label="Numérica",  linewidth=2)
ax.plot(x_vals, U_exact[-1, :], "r-",  label="Exacta",    linewidth=2)
ax.set_xlabel("Posición x")
ax.set_ylabel("Temperatura u(x,t)")
ax.set_title(f"Comparación al tiempo T = {tiempos[-1]:.4f}")
ax.legend()
ax.grid(True, alpha=0.3)
 
ax = axes[1]
error = np.abs(U_num[-1, :] - U_exact[-1, :])
ax.plot(x_vals, error, "g-", linewidth=2)
ax.set_xlabel("Posición x")
ax.set_ylabel("Error absoluto")
ax.set_title(f"Error  (media: {np.mean(error):.2e})")
ax.grid(True, alpha=0.3)
 
plt.tight_layout()
plt.savefig("calor_2D.png", dpi=150)
plt.close()
print("Guardado: calor_2D.png")
 

#  GRÁFICA 2: Evolución temporal

fig, ax = plt.subplots(figsize=(8, 5))
 
colores = plt.cm.plasma(np.linspace(0, 1, len(tiempos)))
for ti, t in enumerate(tiempos):
    ax.plot(x_vals, U_num[ti, :], color=colores[ti],
            linewidth=1.2, label=f"t={t:.3f}" if ti % 2 == 0 else "")
 
ax.set_xlabel("Posición x")
ax.set_ylabel("Temperatura u(x,t)")
ax.set_title("Evolución temporal — solución numérica")
ax.legend(loc="upper right", fontsize=7, ncol=2)
ax.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("calor_evolucion.png", dpi=150)
plt.close()
print("Guardado: calor_evolucion.png")
 
 
#  GRÁFICA 3: Superficie 3D

fig = plt.figure(figsize=(10, 6))
ax3 = fig.add_subplot(111, projection="3d")
 
surf = ax3.plot_surface(X_mesh, T_mesh, U_num,
                        cmap="plasma", edgecolor="none", alpha=0.9)
 
ax3.set_xlabel("Posición x")
ax3.set_ylabel("Tiempo t")
ax3.set_zlabel("Temperatura u")
ax3.set_title("Superficie 3D — u(x,t)")
fig.colorbar(surf, ax=ax3, shrink=0.5, label="Temperatura")
 
plt.tight_layout()
plt.savefig("calor_3D.png", dpi=150)
plt.close()
print("Guardado: calor_3D.png")