#!/bin/bash
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
# empty arrays for required tags, values are paths to XML files containing the tags
# <PDF> contains a <Name> tag and several <ConfigurationParameter> tagss
declare -a pdf
# <DataSet> is singular-valued
declare dataset
# <Output> can contain several <ComponentProjection> tags
declare -a output
# <PhaseSpaceBoundary> contains several <Observable> tags
declare -a phasespaceboundary
# <ParameterSet> contains several <PhysicsParameter> tags
declare -a parameterset
# <Minimiser> just do the default Minuit
declare minimiser
# <FitFunction> is either sweighted or not
declare fitfunction
# read the arguments into arrays
for arg in "$@"
do
	if [[ $arg == "pdf/"* ]]
	then
		pdf+=("$arg")
	elif [[ $arg == "dataset/"* ]]
	then
		dataset="$arg"
	elif [[ $arg == "output/"* ]]
	then
		output+=("$arg")
	elif [[ $arg == "phasespaceboundary/"* ]]
	then
		phasespaceboundary+=("$arg")
	elif [[ $arg == "parameterset/"* ]]
	then
		parameterset+=("$arg")
	elif [[ $arg == "minimiser/"* ]]
	then
		minimiser="$arg"
	elif [[ $arg == "fitfunction/"* ]]
	then
		fitfunction="$arg"
	else
		echo "I don't know what to do with $arg"
	fi
done
# write the XML file
echo "<RapidFit>"
echo "	<ParameterSet>"
for file in "${parameterset[@]}"
do
	parsefile $file 2
done
echo "	</ParameterSet>"
parsefile $minimiser 1
parsefile $fitfunction 1
echo "	<ToFit>"
echo "		<PDF>"
for file in "${pdf[@]}"
do
	parsefile $file 2
done
echo "		</PDF>"
echo "		<DataSet>"
parsefile $dataset 3
echo "			<PhaseSpaceBoundary>"
for file in "${phasespaceboundary[@]}"
do
	parsefile $file 4
done
echo "			</PhaseSpaceBoundary>"
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

