#!/bin/bash
usage()
{
   echo "USAGE: [-D] [-H] [-h] "
   echo "       -h = Don't panic! SEZ@Done helps you :D    "
   echo "       -D = copy default resources from ./resources/default-resources to any framework that changed."
   echo "       -H = copy modified resources from ./resources/changed-resources to any framework for force our modification to them."
   echo "please change some Env.var in script (eg:ESP_IDF_PATH) for packages local address" 
#    exit 1
if [ ! -z $1 ] ; then
    exit $1
fi
}

#--->>> SEZ@Done color map ---
color_reset=$'\E'"[00m"        #RESET
color_failed=$'\E'"[0;31m"     #RED
color_success=$'\E'"[0;32m"    #GREEN
color_yellow=$'\E'"[1;33m"     #YELLOW (ORANGE)
color_blue=$'\E'"[1;34m"       #BLUE
color_magenta=$'\E'"[1;35m"    #MAGENTA
color_cyan=$'\E'"[1;36m"       #CYAN
color_gray=$'\E'"[1;37m"       #GRAY
#--->>>

ESP_IDF_PATH="/home/ehsan/espressif-all/esp-idf"
ESP_IDF_VERSION="esp-idf-v5.2"
#ESP_MATTER_PATH=""
#ESP_MATTER_VERSION=""
DEFAULT_RESOURCES="./resources/default-resources"
CHANGED_RESOURCES="./resources/changed-resources"

default_src()
{ 
 
 echo -n "${color_yellow}----------------------------------------" && echo "${color_reset}"
 echo -n "${color_yellow}>>>     copy default resources       <<<" && echo "${color_reset}"
 echo -n "${color_yellow}----------------------------------------" && echo "${color_reset}"

 cp -rv $DEFAULT_RESOURCES/idf-5.2/components/partition_table/Kconfig.projbuild \
 	$ESP_IDF_PATH/$ESP_IDF_VERSION/components/partition_table/Kconfig.projbuild

# cp -rv $DEFAULT_RESOURCES/idf-5.2/ $ESP_IDF_PATH/$ESP_IDF_VERSION/

 echo -n "${color_success}---------------------------------------" && echo "${color_reset}"
 echo -n "${color_success}>> default res. copied, successfully <<" && echo "${color_reset}"
 echo -n "${color_success}---------------------------------------" && echo "${color_reset}"
}

humanization()
{
 echo -n "${color_yellow}----------------------------------------" && echo "${color_reset}"
 echo -n "${color_yellow}>>>     copy modified resources      <<<" && echo "${color_reset}"
 echo -n "${color_yellow}----------------------------------------" && echo "${color_reset}"


 cp -rv $CHANGED_RESOURCES/idf-5.2/components/partition_table/Kconfig.projbuild \
 	$ESP_IDF_PATH/$ESP_IDF_VERSION/components/partition_table/Kconfig.projbuild

 # cp -rv $CHANGED_RESOURCES/idf-5.2/ $ESP_IDF_PATH/$ESP_IDF_VERSION/

 echo -n "${color_success}----------------------------------------" && echo "${color_reset}"
 echo -n "${color_success}>> modified res. copied, successfully <<" && echo "${color_reset}"
 echo -n "${color_success}----------------------------------------" && echo "${color_reset}"
}

# check pass argument
while getopts "DHh" arg
do
    case $arg in
        D)
            default_src;;
        H)
            humanization;;
        h)
            usage 0;;
        ?)
            usage 1;;
esac
done
