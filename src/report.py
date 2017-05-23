import pandas as pd

import matplotlib.pyplot as plt

df = pd.read_csv("simout.txt", names = ["speed", "angle", "cte", "steer_value", "p_error", "i_error", "d_error"])

plt.subplot(3,1,1)
plt.plot(df.index, df.p_error, df.index, df.i_error, df.index, df.d_error, 'k')
plt.xlabel("Index")
plt.ylabel("Error")
plt.legend(["P", "I", "D"])

plt.subplot(3,1,2)
plt.plot(df.index, df.steer_value)
plt.xlabel("Index")
plt.ylabel("Steering")

plt.subplot(3,1,3)
plt.plot(df.index, df.speed)
plt.xlabel("Index")
plt.ylabel("Speed")

plt.show()
