#!/bin/bash
function error
{
	echo -e "\033[31mERROR:\e[0m $1" 1>&2
}
function warning
{
	echo -e "\033[33mWARNING:\e[0m $1" 1>&2
}
function parsefile # <file> <indentation depth>
{
	while IFS= read line
	do
		indent=""
		# indentation
		for i in `seq $2`
		do
			indent+="	"
		done
		# print the output
		echo "$indent$line"
	done < $1 # feed in the file
}
# List of resonances to eventually go in a ConfigurationParameter
declare reslist
# Name of the phi parameters, needed for p1*p3 etc.
declare phiname
# empty arrays for required tags, values are paths to XML files containing the tags
# <PDF> contains a <Name> tag and several <ConfigurationParameter> tagss
declare -a pdf
# <DataSet> with optional cuts
declare -a dataset
# <Output> can contain several <ComponentProjection> tags
declare -a output
# <PhaseSpaceBoundary> contains several <Observable> tags
declare -a phasespaceboundary
# <ParameterSet> contains several <PhysicsParameter> tags
declare -a parameterset
# <Minimiser> i.e. just <MinimiserName>Minuit</MinimiserName>
declare minimiser
# <FitFunction> with optional weight variable
declare -a fitfunction
# read the arguments into arrays
for arg in "$@"
do
	if [ -e "$arg" ]
	then
		if [[ $arg == *"pdf/"* ]]
		then
			pdf+=("$arg")
		elif [[ $arg == *"dataset/"* ]]
		then
			dataset+=("$arg")
		elif [[ $arg == *"output/"* ]]
		then
			output+=("$arg")
		elif [[ $arg == *"phasespaceboundary/"* ]]
		then
			phasespaceboundary+=("$arg")
		elif [[ $arg == *"parameterset/"* ]]
		then
			parameterset+=("$arg")
			if [[ $arg == *"resonances/"* ]]
			then
				# The first line should contain the spin and resonance shape
				particle=$(echo $arg | sed -r 's/.*resonances\/([a-zA-Z0-9]*)_.*/\1/g')
				reslist="${reslist}${particle}$(head -n 1 $arg | sed -r 's/#\s*spin-([012])\s*([A-Z][A-Z]).*$/(\1\,\2) /g')"
				if [[ $particle == *"phi"* ]]
				then
					phiname=$particle
				fi
			fi
		elif [[ $arg == *"minimiser/"* ]]
		then
			minimiser="$arg"
		elif [[ $arg == *"fitfunction/"* ]]
		then
			fitfunction+=("$arg")
		else
			warning "No rule for $arg (File will be ignored)"
		fi
	else
		error "Cannot find $arg"
		exit 1
	fi
done
# test that the required files have been given
if [ -z "$minimiser" ]
then
	warning "No minimiser given. Using default: Minuit"
	minimiser="minimiser/minuit.xml"
fi
if [ ${#pdf[@]} -eq 0 ]; then
	warning "No PDF given. Using default: Bs2PhiKKSignal"
	pdf+="pdf/default.xml"
fi
if [ ${#dataset[@]} -eq 0 ]; then
	error "You must provide a DataSet"
	exit 1
fi
if [ ${#phasespaceboundary[@]} -eq 0 ]; then
	error "You must provide a PhaseSpaceBoundary"
	exit 1
fi
if [ ${#parameterset[@]} -eq 0 ]; then
	error "You must provide a ParameterSet"
	exit 1
fi
if [ ${#fitfunction[@]} -eq 0 ]; then
	warning "No FitFunction given. Using default: NegativeLogLikelihoodNumerical"
	fitfunction+=("fitfunction/default.xml")
fi
# write the XML file
echo "<RapidFit>"
echo "	<ParameterSet>"
for file in "${parameterset[@]}"
do
	parsefile $file 2
done
echo "	</ParameterSet>"
echo "	<Minimiser>"
parsefile $minimiser 2
echo "	</Minimiser>"
echo "	<FitFunction>"
for file in "${fitfunction[@]}"
do
	parsefile $file 2
done
echo "	</FitFunction>"
echo "	<CommonPhaseSpace>"
echo "		<PhaseSpaceBoundary>"
for file in "${phasespaceboundary[@]}"
do
	parsefile $file 3
done
echo "		</PhaseSpaceBoundary>"
echo "	</CommonPhaseSpace>"
echo "	<ToFit>"
echo "		<PDF>"
for file in "${pdf[@]}"
do
	parsefile $file 3
done
echo "			<ConfigurationParameter>resonances:${reslist}</ConfigurationParameter>"
echo "			<ConfigurationParameter>phiname:${phiname}</ConfigurationParameter>"
echo "		</PDF>"
echo "		<DataSet>"
for file in "${dataset[@]}"
do
	parsefile $file 3
done
echo "			<CommonPhaseSpace>"
echo "			</CommonPhaseSpace>"
echo "		</DataSet>"
echo "	</ToFit>"
echo "	<Output>"
for file in "${output[@]}"
do
	parsefile $file 2
done
echo "	</Output>"
echo "</RapidFit>"
exit 0

