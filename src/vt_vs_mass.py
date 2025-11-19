import numpy as np
import matplotlib.pyplot as plt


mass = np.arange(.001, 10000)
gravity = 9.8
drag = 0.1
vterm = np.sqrt(mass*gravity/1000*drag)


plt.style.use("dark_background")
plt.plot(mass/1000, vterm)
plt.xlabel("Mass (kg)")
plt.ylabel("Terminal Velocity (m/s)")
plt.title(r"Terminal Velocity vs Mass in the Case Where $Air_{k} = 0.1$")
plt.savefig("vterm_vs_mass.png")
plt.show()


