import pylab

import numpy as np
import matplotlib
import matplotlib.pyplot as plt


    
base = './output'
class1= np.genfromtxt(base+'/ASquint_background.dat', comments='#',skip_header=3)

print class1

#class2= read_data_lines(base+'class/scf1_pk.dat')

norm = 1 #max(class1[1])/max(ceasy1[1])
norm2 = 1 #max(class1[1])/max(class2[1])
#norm2 = max(class1[1])/max(class2[1])

fig= plt.figure(figsize=(11,8))

#yrange
#pylab.xlim([0,250.]) 
#pylab.ylim([-0.0005,0.0035])

#labels
fontsize= 16
fontsizeq= fontsize+4
plt.xlabel("$1+z$",
           fontsize=fontsizeq)
plt.ylabel("$\\rho$",
           fontsize=fontsizeq)
plt.yticks(fontsize=fontsize)
plt.xticks([0,2,4,6,8,10],fontsize=fontsize)
fig.suptitle("Albrecht-Skordis quintessence (background)",fontsize=fontsizeq)


plt.plot(class1[::,0],class1[::,8],color='k',
         linewidth=2,linestyle='-',label="$\\rho_\\gamma$")
plt.plot(class1[::,0],class1[::,13],color='g',
        linewidth=2,linestyle='-',label="$\\rho_\\phi$")
plt.plot(class1[::,0],class1[::,10],color='r',
         linewidth=2,linestyle='--',label="$\\rho_{CDM}$")

#legend
plt.legend(loc="upper left",numpoints=1,
           prop={'size':fontsizeq},frameon=False)

#border width
ax= plt.gca()
ax.set_xscale('log')
ax.set_yscale('log')
ax.spines['right'].set_linewidth(2)
ax.spines['left'].set_linewidth(2)
ax.spines['bottom'].set_linewidth(2)
ax.spines['top'].set_linewidth(2)
for line in ax.get_xticklines()+ax.get_yticklines():
    line.set_markeredgewidth(2)
    line.set_markersize(8)

plt.savefig(base+"AS_quint0.pdf",bbox_inches='tight')

plt.show()
