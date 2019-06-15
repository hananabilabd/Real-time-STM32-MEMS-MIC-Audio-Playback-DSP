import numpy as np
l=[]
mytuple=(695, 677, 664, 679, 676, 652, 685, 717)
print mytuple[7]
l.extend(list(mytuple))
l.append(4)
#print mytuple[0]
np.set_printoptions(suppress=True)
#x= np.empty([2, 2])
x=np.array([[1,2],[3,4]])
print x
#x[:,1]=np.append(x[:,1],[3],axis =1)
#print x.shape
#print x
#print x[:,1]
from numpy import *

a = array([[1,80,75,85,90,95],
	       [2,75,80,75,85,100],
	       [2,80,80,80,90,95]])

#a= append(a,[[mytuple[0],mytuple[1],mytuple[2],mytuple[3],mytuple[4],mytuple[5]]],0)
#a= append(a,[[3,82,79,88,97,99]],0)
print a.shape[0]
print a[2]
print a[:,0]
#b =np.array([[]])
#b= np.empty([0,6])
#b= append(b,[[3,82,79,88,97,99]],0)
#print b
