import numpy as np
import matplotlib.pyplot as plt

s1 = [1.168, 1.189, 1.05, 3.367, 3.26]
s2 = [1.342, 1.567, 14.14, 21.13]
x1 = [1e2,1e4,1e6,1e7,1e8]
x2 = [1e2,1e4,1e6,1e8]


# fig = plt.figure(1)
# 
# ax = plt.subplot(1,1,1)
# ax.set_title("GPU vs CPU Speed-up")
# 
# 
# ax.set_xlabel("Vector size")
# ax.set_ylabel("Speed-up")
# sum = ax.plot([x1,s1],label="Sum kernel")[0]
# fac = ax.plot([x2,s2],label="Factor count")[0]
# ax.legend(handles=[sum,fac])
# ax.set_xscale("log")
# plt.show()

# plt.title("GPU vs CPU Speed-up")
plt.xlabel("Vector size")
plt.ylabel("Speed-up")
# plt.plot(x1,s1,x2,s2)
sum, = plt.plot(x1,s1,label="Sum kernel")
fac, = plt.plot(x2,s2,label="Factor count")


plt.xscale("log")
plt.legend()
plt.show()