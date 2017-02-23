ranges=(phirange 1080 1200 1350 1450 1550 1650 1800)
for range in ${ranges[@]}
do
	cp data_sweight.xml data_${range}_sweight.xml
	sed -i "s/mvacut_Sweighted/mvacut_${range}_Sweighted/g" data_${range}_sweight.xml
done
