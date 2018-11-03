import comtypes.client as cc
import comtypes
tlb_id = comtypes.GUID("{62DC7659-C0A3-447E-AE76-92CA0B234272}") #Type Lib GUID

cc.GetModule((tlb_id, 1, 0)) # 1 and 0 are major number and minor number of our Type Lib

import comtypes.gen.AutomationServerTypeLib as AutomationServer #AutomationServerTypeLib is Type lib name specified in IDL file


pIMyMath = cc.CreateObject("{8C321D8D-9289-447E-BBF3-6F58A6F6CB3E}",  None, None, AutomationServer.IMyMath) #Passed CoClass ID(clsid) and Interface name


result=pIMyMath.SumOfTwoIntegers(25,20)
print "Sum of 25 and 20 is:"+ str(result)
print "Requesting IMyMath using pIMyMath"
pIMyMath2=pIMyMath.QueryInterface(AutomationServer.IMyMath)

result2=pIMyMath.SubtractionOfTwoIntegers(250,100)
print "Subtraction of 250 and 100 is:"+ str(result2)