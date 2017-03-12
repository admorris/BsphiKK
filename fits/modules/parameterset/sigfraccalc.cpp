#include <cstdio>
#include <cstdlib>
int main(int argc, char* argv[])
{
	if(argc != 5)
	{
		printf("Usage: %s <signal yield> <sig yield error> <background yield> <bkg yield error>\n",argv[0]);
		return 1;
	}
	double S  = atof(argv[1]);
	double dS = atof(argv[2]);
	double B  = atof(argv[3]);
	double dB = atof(argv[4]);
	double f  = S/(S+B);
	double df = S * dB/((S+B)*(S+B));
	printf("F = %f ± %f\n",f,df);
	return 0;
}
