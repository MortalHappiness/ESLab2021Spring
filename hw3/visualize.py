import json
import numpy as np
import matplotlib.pyplot as plt

x = list()
y = list()
z = list()
with open("data.txt") as fin:
    for line in fin:
        data = json.loads(line)
        x.append(data["x"])
        y.append(data["y"])
        z.append(data["z"])


fig = plt.figure()
ax = plt.axes(projection="3d")

# z_line = np.linspace(0, 15, 1000)
# x_line = np.cos(z_line)
# y_line = np.sin(z_line)
# ax.plot3D(x_line, y_line, z_line, 'gray')

x_points = np.array(x)
y_points = np.array(y)
z_points = np.array(z)
ax.plot3D(x_points, y_points, z_points, 'gray')
ax.scatter3D(x_points, y_points, z_points, c=z_points, cmap='hsv')

for i in range(len(x_points)):
    ax.text(x[i], y[i], z[i], str(i + 1), size=8, zorder=1, color='k')

ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')

plt.show()
