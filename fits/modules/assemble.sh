#!/bin/bash
function error
{
	echo -e "\033[31mERROR:\e[0m $1" 1>&2
}
function warning
{
	echo -e "\033[33mWARNING:\e[0m $1" 1>&2
}
# SGE/OGE job options
declare -a joboptions
# Is this a batch job or not?
if [ -z "$NSLOTS" ]
then
	NSLOTS=$(nproc)
fi
# List of resonances to eventually go in a ConfigurationParameter
declare -a resonances
declare -a sigwidths
declare -a sigstyles
declare -a sigcolours
# List of background components
declare -a components
declare -a bkgwidths
declare -a bkgstyles
declare -a bkgcolours
function getoption # <file> <option>
{
	while IFS= read line
	do
		if [[ "$line" == "#"*"$2:"*  ]]
		then
			echo $line | sed -r "s/\#\s*$2:\s?(.*)$/\1/"
			return
		fi
	done < $1 # feed in the file
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
# Name of the phi parameters, needed for p1*p3 etc.
declare phiname
# empty arrays for required tags, values are paths to XML files containing the tags
# <PDF> contains a <Name> tag and several <ConfigurationParameter> tags
declare -a signalpdf
declare -a backgroundpdf
# <DataSet> with optional cuts
declare -a dataset
# <Output> can contain several <ComponentProjection> tags
declare -a output
# <PhaseSpaceBoundary> contains several <Observable> tags
declare -a phasespaceboundary
# <ParameterSet> contains several <PhysicsParameter> tags
declare -a parameterset
# <ConstraintFunction> contains several <ExternalConstraint> tags
declare -a constraintfunction
# <Minimiser> i.e. just <MinimiserName>Minuit</MinimiserName>
declare minimiser
# <FitFunction> with optional weight variable
declare -a fitfunction
# read the arguments into arrays
for arg in "$@"
do
	if [ -e "$arg" ]
	then
		if [[ $arg == *"joboptions/"* ]]
		then
			joboptions+=("$arg")
		elif [[ $arg == *"pdf/signal/"* ]]
		then
			signalpdf+=("$arg")
		elif [[ $arg == *"pdf/background/"* ]]
		then
			backgroundpdf+=("$arg")
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
				particle=$(echo $arg | sed -r 's/.*resonances\/([a-zA-Z0-9]*)_.*/\1/g')
				if [[ $particle == *"phi"*"1020"* ]] # There are other phis, so the mass must be specified
				then
					phiname=$particle
				fi
			elif [[ $arg == *"fractions/"* ]]
			then
				# The first line should contain the spin and resonance shape
				particle=$(echo $arg | sed -r 's/.*fractions\/([a-zA-Z0-9]*)_.*/\1/g')
				resonances+=("${particle}$(getoption $arg shape | sed -r 's/spin-([012])\s*([A-Z][A-Z]).*$/(\1\,\2)/g')")
				sigwidths+=("$(getoption $arg width)")
				sigstyles+=("$(getoption $arg style)")
				sigcolours+=("$(getoption $arg colour)")
			elif [[ $arg == *"backgrounds/"* ]]
			then
				# The first line should contain the spin and resonance shape
				particle=$(echo $arg | sed -r 's/.*fractions\/([a-zA-Z0-9]*)_.*/\1/g')
				components+=("${particle}($(getoption $arg shape))")
				sigwidths+=("$(getoption $arg width)")
				sigstyles+=("$(getoption $arg style)")
				sigcolours+=("$(getoption $arg colour)")
			fi
		elif [[ $arg == *"constraintfunction/"* ]]
		then
			constraintfunction+=("$arg")
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
if [ ${#signalpdf[@]} -eq 0 && ${#backgroundpdf[@]} -eq 0 ]
then
	warning "No PDF given. Using default: Bs2PhiKKSignal"
	signalpdf+="pdf/Bs2PhiKKSignal/default.xml"
fi
if [ ${#dataset[@]} -eq 0 ]
then
	error "You must provide a DataSet"
	exit 1
fi
if [ ${#phasespaceboundary[@]} -eq 0 ]
then
	error "You must provide a PhaseSpaceBoundary"
	exit 1
fi
if [ ${#parameterset[@]} -eq 0 ]
then
	error "You must provide a ParameterSet"
	exit 1
fi
if [ ${#fitfunction[@]} -eq 0 ]
then
	warning "No FitFunction given. Using default: NegativeLogLikelihoodNumerical"
	fitfunction+=("fitfunction/default.xml")
fi
# Get the resonance options
for item in "${resonances[@]}"
do
	reslist="${reslist}${item} "
done
if [ ${#resonances[@]} -gt 1 ]
then
	for item in "${widths[@]}"
	do
		widthlist="${widthlist}${item}:"
	done
	widthlist="2:${widthlist}2"
	for item in "${styles[@]}"
	do
		stylelist="${stylelist}${item}:"
	done
	stylelist="1:${stylelist}5"
	for item in "${colours[@]}"
	do
		colourlist="${colourlist}${item}:"
	done
	colourlist="1:${colourlist}1"
fi
# write the XML file
for file in "${joboptions[@]}"
do
cat $file # every line of these files should start with a # character, which makes it ignored by RapidFit and it only picked up by the job submission script
done
echo "<RapidFit>"
# Build the parameter set
echo "	<ParameterSet>"
for file in "${parameterset[@]}"
do
	parsefile $file 2
done
echo "	</ParameterSet>"
# Choose the minimiser
echo "	<Minimiser>"
parsefile $minimiser 2
echo "	</Minimiser>"
# Build the fit function
echo "	<FitFunction>"
for file in "${fitfunction[@]}"
do
	parsefile $file 2
done
echo "		<Threads>${NSLOTS}</Threads>"
echo "	</FitFunction>"
# Build the phase space boundary
echo "	<CommonPhaseSpace>"
echo "		<PhaseSpaceBoundary>"
for file in "${phasespaceboundary[@]}"
do
	parsefile $file 3
done
echo "		</PhaseSpaceBoundary>"
echo "	</CommonPhaseSpace>"
echo "	<ToFit>"
# Build the PDF
if [ ${#signalpdf[@]} -gt 0 && ${#backgroundpdf[@]} -gt 0 ]
	echo "	<NormalisedSumPDF>"
	echo "		<FractionName>signal_fraction</FractionName>"
fi
# Build the signal PDF
if [ ${#signalpdf[@]} -gt 0 ]
	echo "		<PDF>"
	for file in "${signalpdf[@]}"
	do
		parsefile $file 3
	done
	echo "			<ConfigurationParameter>resonances:${reslist}</ConfigurationParameter>"
	echo "			<ConfigurationParameter>phiname:${phiname}</ConfigurationParameter>"
	echo "		</PDF>"
	echo "		<DataSet>"
fi
# Build the background PDF
if [ ${#backgroundpdf[@]} -gt 0 ]
	echo "		<PDF>"
	for file in "${backgroundpdf[@]}"
	do
		parsefile $file 3
	done
	echo "			<ConfigurationParameter>components:${bkglist}</ConfigurationParameter>"
	echo "		</PDF>"
fi
if [ ${#signalpdf[@]} -gt 0 && ${#backgroundpdf[@]} -gt 0 ]
	echo "	</NormalisedSumPDF>"
fi
# Build the dataset
echo "		<DataSet>"
for file in "${dataset[@]}"
do
	parsefile $file 3
done
echo "			<CommonPhaseSpace>"
echo "			</CommonPhaseSpace>"
echo "		</DataSet>"
# Optional external constraint functions
if [ ! ${#constraintfunction[@]} -eq 0 ]
then
	echo "		<ConstraintFunction>"
	for file in "${constraintfunction[@]}"
	do
		parsefile $file 3
	done
	echo "		</ConstraintFunction>"
fi
echo "	</ToFit>"
# Build the output projections
echo "	<Output>"
for file in "${output[@]}"
do
	echo "		<ComponentProjection>"
	parsefile $file 3
	echo "		  <Threads>${NSLOTS}</Threads>"
	if [ ${#resonances[@]} -gt 1 ]
	then
		if [ ${#widths[@]} -eq ${#resonances[@]} ]
		then
			echo "			<WidthKey>${widthlist}</WidthKey>"
		else
			warning "Only ${#widths[@]} width keys"
		fi
		if [ ${#styles[@]} -eq ${#resonances[@]} ]
		then
			echo "			<StyleKey>${stylelist}</StyleKey>"
		fi
		if [ ${#colours[@]} -eq ${#resonances[@]} ]
		then
			echo "			<ColorKey>${colourlist}</ColorKey>"
		fi
	fi
	echo "		</ComponentProjection>"
done
echo "	</Output>"
echo "</RapidFit>"
exit 0

