#compare ideal in python and C++
import xlns as xl
import os
xl.xlnssetF(7)

for z in range(0,1024):
   py = (xl.xlns(1)+xl.xlnsB**(-z)).x
   c = eval(os.popen("../build/tools/sb16 "+str(-z)).read())
   if (py-c) != 0:
     print(str(z)+" "+str(py-c),end=" ")
     print(py,end=" ")
     print(c)

