using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ManagedServerForInterop
{
    [ClassInterface(ClassInterfaceType.AutoDual)]
    public class Math
    {
        public int MultiplicationValue { get; set; }
        public int DivisionValue { get; set; }

        public Math()
        {
            // empty
        }

        public int MultiplicationOfTwoIntegers(int num1, int num2)
        {
            MultiplicationValue = num1 * num2;
            MessageBox.Show(string.Format("Multiplication of {0} and {1} is {2}", num1, num2, MultiplicationValue));
            return MultiplicationValue;
        }

        public int DivisionOfTwoIntegers(int num1, int num2)
        {
            DivisionValue = num1 / num2;
            MessageBox.Show(string.Format("Division of {0} and {1} is {2}", num1, num2, DivisionValue));
            return DivisionValue;
        }

    }
}