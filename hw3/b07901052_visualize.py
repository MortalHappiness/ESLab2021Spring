import json
import matplotlib.pyplot as plt
import numpy as np

t = [0]
x = [0]
y = [0]
z = [0]
offset = []
factor = 0.05
with open("data.txt", "r") as f:
    lines = f.readlines()
    for line in lines:
        data = json.loads(line)
        # print(data)
        if len(offset) == 0:
            offset.append(data["x"])
            offset.append(data["y"])
            offset.append(data["z"])
        t.append(data["s"])
        x.append(x[-1] + factor * (data["x"] - offset[0]))
        y.append(y[-1] + factor * (data["y"] - offset[1]))
        z.append(z[-1] + factor * (data["z"] - offset[2]))
plt.title("pos vs time")
plt.plot(t, x, label="x")
plt.plot(t, y, label="y")
plt.plot(t, z, label="z")
plt.legend()
plt.xlabel("time")
plt.ylabel("pos")
plt.savefig("hw3_b07901052.png")
plt.show()
