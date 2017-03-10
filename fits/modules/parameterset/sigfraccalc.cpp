//S:	4102.48±64.8044
//B:	77.1316±5.22503
{
double S = 4102.48;
double dS = 64.8044;
double B = 77.1316;
double dB = 5.22503;

double f = S/(S+B);
double df = S * dB/std::pow(S+B,2);
printf("F = %f ± %f\n",f,df);
}
