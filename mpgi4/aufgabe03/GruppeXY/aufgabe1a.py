import numpy as np
import matplotlib.pyplot as plt


# Runge fucnction

# x values in [-5,5]
x = np.linspace(-5, 5, 256, endpoint=True) 
# calculate y values using Runge function
y = 1 / (1 + (x * x))


##################################################
# Plotting

plt.grid(True)
plt.plot( x, y, 'r-')
plt.xlim( -5.1,5.1)
plt.xticks( np.linspace( -5.0,5.0,11.0))
plt.ylim( -0.1,1.1)
plt.subplots_adjust( left=0.05, right=0.98, top=0.98, bottom=0.05)

plt.savefig('./figures/aufgabe1a.pdf')
plt.show()

