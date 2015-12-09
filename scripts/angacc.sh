#!/bin/bash
filename='pdfs/src/Bs2PhiKKAcceptance.Legendre_Coefficients.cpp'
cd ../fits
fitting -f acceptance.xml --calculateAcceptanceCoefficients | tee acceptance.log
awk '/BEGIN CODE/,/END CODE/' acceptance.log > 
