// Apologies in advance for all the hard-coded numbers
#include <iostream>
#include <memory>
#include "datum.h"
using plusminus::pm;

int main (int argc, char const* argv[])
{
	// External stuff
	datum BFBsPhiPhi = 1.84e-5 <pm> 0.18e-5;
	datum BFPhiKK = 0.489 <pm> 0.005;
	// f0_980
	datum relBFBsPhif0980pipi = 0.068 <pm> 0.011;
	datum relBFBsPhif0980KKuperr = relBFBsPhif0980pipi * (1./(0.75 <pm> 0.11) - 1) / BFPhiKK;
	datum relBFBsPhif0980KKloerr = relBFBsPhif0980pipi * (1./(0.75 <pm> 0.13) - 1) / BFPhiKK;
	std::cout << "N(Bs→ϕf0(980))/N(Bs→ϕϕ) = " << relBFBsPhif0980KKuperr.val() << " +" << relBFBsPhif0980KKuperr.err() << " −" << relBFBsPhif0980KKloerr.err() << std::endl;
	// f2(1270)
	datum relBFBsPhif21270pipi = 0.033 <pm> 0.006;
	datum relBFBsPhif21270KKuperr = relBFBsPhif21270pipi * (0.046 <pm> 0.004) / (0.848 <pm> 0.024) / BFPhiKK;
	datum relBFBsPhif21270KKloerr = relBFBsPhif21270pipi * (0.046 <pm> 0.004) / (0.848 <pm> 0.012) / BFPhiKK;
	std::cout << "N(Bs→ϕf2(1270))/N(Bs→ϕϕ) = " << relBFBsPhif21270KKuperr.val() << " +" << relBFBsPhif21270KKuperr.err() << " −" << relBFBsPhif21270KKloerr.err() << std::endl;
	// f0(1500)
	datum FFBsPhif01500pipi = 0.347 <pm> 0.034;
	datum relBFBsPhif01500pipi = FFBsPhif01500pipi * (0.348 <pm> 0.029) / 1.84;
	datum relBFBsPhif01500KK = relBFBsPhif01500pipi * (0.241 <pm> 0.028) / BFPhiKK;
	std::cout << "N(Bs→ϕf0(1500))/N(Bs→ϕϕ) = " << relBFBsPhif01500KK << std::endl;
	return 0;
}

