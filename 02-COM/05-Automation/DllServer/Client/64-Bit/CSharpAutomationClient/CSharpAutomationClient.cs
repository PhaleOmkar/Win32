using System;
using System.Runtime.InteropServices;
using AutomationServerTypeLibForDotNet;

public class CSharpAutomationClient
{
	public static void Main()
	{
		CMyMathClass objCMyMathClass = new CMyMathClass();
		//IMyMath objIMyMath = (IMyMath) objCMyMathClass;
		int num1 = 75, num2 = 25, res = 0;

		res = objCMyMathClass.SumOfTwoIntegers(num1, num2);
		Console.WriteLine("The Sum Of " + num1 + " and " + num2 + " is " + res);

		res = objCMyMathClass.SubtractionOfTwoIntegers(num1, num2);
		Console.WriteLine("The Subtraction Of " + num1 + " and " + num2 + " is " + res);
	}
}
