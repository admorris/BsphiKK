//S:	4102.48±64.8044
//B:	77.1316±5.22503
{
double S = 4102.48;
double dS = 64.8044;
double B = 77.1316;
double dB = 5.22503;

double N = S+B;
double dN = sqrt(dS*dS + dB*dB);
double f = S/N;
double df = f*sqrt(pow(dS/S,2) + pow(dN/N,2));
printf("F = %f ± %f\n",f,df);
}
