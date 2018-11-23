using System.Runtime.InteropServices;

namespace MathsManagedServer
{
    public struct SolutionOfQuadraticEq
    {
        public int NoOfRoots;
        public double root1;
        public double root2;
    }
    
    // define interface IMath
    public interface IMath
    {
        SolutionOfQuadraticEq CalculateRootsOfEq(double a, double b, double c);      
    }

    [ClassInterface(ClassInterfaceType.AutoDispatch)]
    public class Math: IMath
    {
        
        public Math()
        {
            // empty 
        }
        
        public SolutionOfQuadraticEq CalculateRootsOfEq(double a, double b, double c)
        {
            SolutionOfQuadraticEq answer = new SolutionOfQuadraticEq();
            answer.NoOfRoots = 0;
            answer.root1 = 0;
            answer.root2 = 0;

            double delta = System.Math.Sqrt((b * b) - (4 * a * c));

            // how many roots the eqn has

            if (delta > 0)  // 2 roots
            {
                answer.NoOfRoots = 2;

                // calculate roots
                answer.root1 = (-b + delta) / (2 * a);
                answer.root2 = (-b - delta) / (2 * a);

            }
            
            else if (delta == 0)  // 1 root
            {
                answer.NoOfRoots = 1;

                // calculate roots
                answer.root1 = (-b / (2 * a));
            }


            else  // imaginary roots
            {
                answer.NoOfRoots = 0;

                // set roots to 0 
                answer.root1 = 0;
                answer.root2 = 0;
            }

            return answer;
        }
    }

}