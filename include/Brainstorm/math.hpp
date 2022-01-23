#define E 2.71828

class math
{
    public:
         static double Sigmoid(double x)
         {
            // 1/(1+E^-x)
            return 1/(1 + EXP(-1*(x)));
         }
         
         static double RELU(double x)
         {
            //x == x when x > 0
            //x == 0 when x <= 0
            return (x > 0)*x;
         }

         static double LRELU(double x)
         {
            //x == x when x > 0
            //x == 0.1*x when x <= 0
            return (x > 0)*x+((x <= 0)*x*0.1);
         }

        static double EXP(double a){
          union { double d; long long x; } u;
          u.x = (long long)(6497320848556798LL * a + 0x3fef127e83d16f12LL);

          return u.d;
        }

         static double dSigmoid(double x)
         {
            return Sigmoid(x)*(1 - Sigmoid(x));
         }

        static double dRELU(double x)
        {
           return (x > 0);
        }
        
        static double dLRELU(double x)
        {
           return (x > 0)+((x <= 0)*0.1);
        }

};