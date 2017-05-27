import pandas as pd

import matplotlib.pyplot as plt

df = pd.read_csv("simout.txt", names = ["speed", "angle", "cte", "cte_filt",
                                        "steer_value", "steer_value_filt", 
                                        "p_error", "i_error", "d_error", 
                                        "steer_p", "steer_i", "steer_d"])

plt.subplot(5,1,1)
plt.plot(df.index, df.cte, df.index, df.cte_filt)

plt.subplot(5,1,2)
plt.plot(df.index, df.p_error, df.index, df.i_error, df.index, df.d_error, 'k')
plt.xlabel("Index")
plt.ylabel("Error")
plt.legend(["P", "I", "D"])

plt.subplot(5,1,3)
plt.plot(df.index, df.steer_value, df.index, df.steer_value_filt)
plt.xlabel("Index")
plt.ylabel("Steering")
plt.legend(["Raw", "Filtered"])

plt.subplot(5,1,4)
plt.plot(df.index, df.steer_p, df.index, df.steer_i, df.index, df.steer_d)
plt.xlabel("Index")
plt.ylabel("Steering")
plt.legend(["P", "I", "D"])

plt.subplot(5,1,5)
plt.plot(df.index, df.speed)
plt.xlabel("Index")
plt.ylabel("Speed")

plt.show()
