#!/bin/bash
function error
{
	echo -e "\033[31mERROR:\e[0m $1" 1>&2
}
function warning
{
	echo -e "\033[33mWARNING:\e[0m $1" 1>&2
}
function inform
{
	echo -e "\033[32mINFO:\e[0m $1" 1>&2
}
function getparticlename
{
	echo $1 | sed -r "s/.*$2\/([a-zA-Z0-9]*)_.*/\1/g"
}
# SGE/OGE job options
declare -a joboptions
# Is this a batch job or not?
if [[ -z "$NSLOTS" ]]
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
declare -a unitarityconstraints
# <Minimiser> i.e. just <MinimiserName>Minuit</MinimiserName>
declare minimiser
# <FitFunction> with optional weight variable
declare -a fitfunction
# whether or not we can distinguish between transverse polarisations
usephi=0
# read the arguments into arrays
for arg in "$@"
do
	if [[ -e "$arg" ]]
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
			if [[ $(grep -c "phi" $arg) -gt 0 ]]
			then
				usephi=1
			fi
		elif [[ $arg == *"parameterset/"* ]]
		then
			parameterset+=("$arg")
			if [[ $arg == *"resonances/"* ]]
			then
				particle=$(getparticlename $arg resonances)
				if [[ $particle == *"phi"*"1020"* ]] # There are other phis, so the mass must be specified
				then
					phiname=$particle
				fi
			elif [[ $arg == *"amplitudes/"* ]]
			then
				particle=$(getparticlename $arg amplitudes)
				if [[ $(grep -c "Aplus" $arg) -gt 0 ]]
				then
					unitarityconstraints+=("${particle}")
				fi
			elif [[ $arg == *"fractions/"* ]]
			then
				# The first line should contain the spin and resonance shape
				particle=$(getparticlename $arg fractions)
				resonances+=($(getoption $arg shape))
				for comb in $(seq 1 $(echo "$(getoption $arg shape)" | grep -o "," | wc -l))
				do
					sigwidths+=("$(getoption $arg width)")
					sigstyles+=("$(getoption $arg style)")
					sigcolours+=("$(getoption $arg colour)")
				done
			elif [[ $arg == *"backgrounds/"* ]]
			then
				# The first line should contain the spin and resonance shape
				particle=$(getparticlename $arg backgrounds)
				components+=("${particle}($(getoption $arg shape))")
				bkgwidths+=("$(getoption $arg width)")
				bkgstyles+=("$(getoption $arg style)")
				bkgcolours+=("$(getoption $arg colour)")
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
if [[ -z "$minimiser" ]]
then
	warning "No minimiser given. Using default: Minuit"
	minimiser="minimiser/minuit.xml"
fi
if [[ ${#signalpdf[@]} -eq 0 && ${#backgroundpdf[@]} -eq 0 ]]
then
	warning "No PDF given. Using default: Bs2PhiKKSignal"
	signalpdf+="pdf/Bs2PhiKKSignal/default.xml"
fi
if [[ ${#dataset[@]} -eq 0 ]]
then
	error "You must provide a DataSet"
	exit 1
fi
if [[ ${#phasespaceboundary[@]} -eq 0 ]]
then
	error "You must provide a PhaseSpaceBoundary"
	exit 1
fi
if [[ ${#parameterset[@]} -eq 0 ]]
then
	error "You must provide a ParameterSet"
	exit 1
fi
if [[ ${#fitfunction[@]} -eq 0 ]]
then
	warning "No FitFunction given. Using default: NegativeLogLikelihoodNumerical"
	fitfunction+=("fitfunction/default.xml")
fi

# Style of total PDF
if [[ ${#resonances[@]} -gt 1 || ${#components[@]} -gt 1 ]]
then
	widthlist="2"
	stylelist="1"
	colourlist="1"
fi

# Get the resonance options
for item in "${resonances[@]}"
do
	reslist="${reslist}${item} "
done

# Style of total signal PDF
if [[ ${#components[@]} -gt 0 && ${#resonances[@]} -gt 0 ]]
then
	if [[ ${#resonances[@]} -gt 1 ]]
	then
		widthlist="${widthlist}:0"
	else
		widthlist="${widthlist}:2"
	fi
	stylelist="${stylelist}:1"
	colourlist="${colourlist}:1"
fi

# Style the signal components
if [[ ${#resonances[@]} -gt 1 ]]
then
	for item in "${sigwidths[@]}"
	do
		widthlist="${widthlist}:${item}"
	done
	for item in "${sigstyles[@]}"
	do
		stylelist="${stylelist}:${item}"
	done
	for item in "${sigcolours[@]}"
	do
		colourlist="${colourlist}:${item}"
	done
	# Style of interference
	widthlist="${widthlist}:2"
	stylelist="${stylelist}:5"
	colourlist="${colourlist}:1"
fi

# Get the background components options
for item in "${components[@]}"
do
	bkglist="${bkglist}${item} "
done

# Style of total background PDF
if [[ ${#components[@]} -gt 0 && ${#resonances[@]} -gt 0 ]]
then
#	if [[ ${#components[@]} -gt 1 ]]
#	then
#		widthlist="${widthlist}:0"
#	else
		widthlist="${widthlist}:2"
#	fi
	stylelist="${stylelist}:1"
	colourlist="${colourlist}:2"
fi

# Style the background components
if [[ ${#components[@]} -gt 1 ]]
then
	for item in "${bkgwidths[@]}"
	do
#		widthlist="${widthlist}:${item}"
		widthlist="${widthlist}:0"
	done
	for item in "${bkgstyles[@]}"
	do
		stylelist="${stylelist}:${item}"
	done
	for item in "${bkgcolours[@]}"
	do
		colourlist="${colourlist}:${item}"
	done
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
if [[ ${#signalpdf[@]} -gt 0 && ${#backgroundpdf[@]} -gt 0 ]]
then
	echo "	<NormalisedSumPDF>"
	echo "		<FractionName>signal_fraction</FractionName>"
fi
# Build the signal PDF
if [[ ${#signalpdf[@]} -gt 0 ]]
then
	echo "		<PDF>"
	for file in "${signalpdf[@]}"
	do
		parsefile $file 3
	done
	echo "			<ConfigurationParameter>resonances:${reslist}</ConfigurationParameter>"
	echo "			<ConfigurationParameter>phiname:${phiname}</ConfigurationParameter>"
	echo "		</PDF>"
fi
# Build the background PDF
if [[ ${#backgroundpdf[@]} -gt 0 ]]
then
	echo "		<PDF>"
	for file in "${backgroundpdf[@]}"
	do
		parsefile $file 3
	done
	echo "			<ConfigurationParameter>components:${bkglist}</ConfigurationParameter>"
	echo "		</PDF>"
fi
if [[ ${#signalpdf[@]} -gt 0 && ${#backgroundpdf[@]} -gt 0 ]]
then
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
if [[ ${#constraintfunction[@]} -gt 0 || ${#unitarityconstraints[@]} -gt 0 ]]
then
	echo "		<ConstraintFunction>"
	for file in "${constraintfunction[@]}"
	do
		parsefile $file 3
	done
	if [[ ${#unitarityconstraints[@]} -gt 0 && ${usephi} -eq 1 ]]
	then
		echo "			<ExternalConstraint>"
		echo "				<Name>UNITARITY_SQ;${unitarityconstraints[@]};_Azero _Aplus</Name>"
		echo "				<Value>1.0</Value> # Sum of params must be less than Value"
		echo "				<Error>0.00001</Error> # NLL penalised by the amount (Amount above Value)^2 / Error^2"
		echo "			</ExternalConstraint>"
	fi
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
	if [[ ${#resonances[@]} -gt 1 || ${#components[@]} -gt 1 ]]
	then
		numsigkeys=${#resonances[@]}
		if [[ ${#sigwidths[@]} -eq ${numsigkeys} && ${#bkgwidths[@]} -eq ${#components[@]} ]]
		then
			echo "			<WidthKey>${widthlist}</WidthKey>"
		else
			warning "${#sigwidths[@]} signal width keys (${numsigkeys} expected) and ${#bkgwidths[@]} background width keys (${#components[@]} expected)"
		fi
		if [[ ${#sigstyles[@]} -eq ${numsigkeys} && ${#bkgstyles[@]} -eq ${#components[@]} ]]
		then
			echo "			<StyleKey>${stylelist}</StyleKey>"
		fi
		if [[ ${#sigcolours[@]} -eq ${numsigkeys} && ${#bkgcolours[@]} -eq ${#components[@]} ]]
		then
			echo "			<ColorKey>${colourlist}</ColorKey>"
		fi
	fi
	echo "		</ComponentProjection>"
done
echo "	</Output>"
echo "</RapidFit>"
exit 0

