#compare Mitchell (LPVIP) in python and C++
import xlns as xl
import xlnsconf.lpvip_ufunc
xl.sbdb_ufunc = xlnsconf.lpvip_ufunc.sbdb_ufunc_premitAddpremitSub
#xlnsconf.lpvip_ufunc.sbdb_ufunc_premitAddpremitSub(-1,0)//2

import os
xl.xlnssetF(7)

#for z in range(1,1024):
for z in range(128,150):
   py = (xl.xlns(-1)+xl.xlnsB**(-z)).x
   c = eval(os.popen("../build/tools/dbmit16 "+str(-z)).read())
   if (py-c) != 0:
     print(str(z)+" "+str(py-c),end=" ")
     print(py,end=" ")
     print(c)

