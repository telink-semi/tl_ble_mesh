echo "************************* start of post build *************************"

os=$(uname)
echo "OS from uname is: ${os}"

if [ "${os}" = "Linux" ] ; then
    echo "Linux OS"
    echo "check_fw in Linux..."
    chmod 755 ../../../check_fw
    ../../../check_fw $1.bin
else
    echo "Windows OS"
    echo "check_fw in Windows..."
    ../../../tl_check_fw2.exe  $1.bin
	exec_result=$?
	echo  "---------------------------  SRAM info ---------------------------"
	str=$(grep -E "^[0-9a-f]{8} g[ ]+\*ABS\*.*_iram_use_size_" --text -o $1.lst)
	dec_num=$((16#${str:0:8}))
	dec_k_1=$((dec_num/1024))
	dec_k_2=$(((dec_num-dec_num/1024*1024)*10/1024))
	echo "IRAM Usage: "$dec_num"Byte" $dec_k_1"."$dec_k_2"kByte"
	str=$(grep -E "^[0-9a-f]{8} g[ ]+\*ABS\*.*_retention_size_" --text -o $1.lst)
	dec_num=$((16#${str:0:8}))
	dec_k_1=$((dec_num/1024))
	dec_k_2=$(((dec_num-dec_num/1024*1024)*10/1024))
	echo "Retention IRAM Usage: "$dec_num"Byte" $dec_k_1"."$dec_k_2"kByte"
	str=$(grep -E "^[0-9a-f]{8} g[ ]+\*ABS\*.*_dram_use_size_" --text -o $1.lst)
	dec_num=$((16#${str:0:8}))
	dec_k_1=$((dec_num/1024))
	dec_k_2=$(((dec_num-dec_num/1024*1024)*10/1024))
	echo "DRAM Usage(exclude stack): "$dec_num"Byte" $dec_k_1"."$dec_k_2"kByte"
	echo  "---------------------------  SRAM info end  ---------------------------"
fi

echo  "-------------------- SDK version info --------------------"
str=$(grep -E "[\$]{3}[a-zA-Z0-9 _.]+[\$]{3}" --text -o $1.bin | sed 's/\$//g')
if [ -z "$str" ]; 
    then echo "no SDK version found at the end of firmware, please check sdk_version.c and sdk_version.h"
else 
    echo "$str";
fi
echo  "-------------------- SDK version end  --------------------"

filesize=$(stat --format=%s $1.bin)
if [ $filesize -gt 262144 ] ; then
    echo "bin size is greater than 256KB, please refer to handbook!"
fi

echo "************************** end of post build **************************"
echo "this is post build!! current configure is :$1"