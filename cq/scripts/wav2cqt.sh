#!/bin/bash

# This script converts all the wav files into cqt files in csv format.
#
# usage:  wav2cqt.sh WAVDIR CQTDIR
#

WAVDIR=$1
CQTDIR=$2

N_THREADS=6

#=======================================================================================
function wav2cqt_onefile(){
	# CQT tool
	#----------------------------------------------------------
	WAV2CQT=./timefreq/cq/cq_sample

	# argument
	#----------------------------------------------------------
	file=$1
	OUTPUT_DIR=$2

	# temporary file name 
	# (we used md5 hash to avoid the conflict of the name) 
	#----------------------------------------------------------
	TEMP_FILE=`md5sum -b ${file} | cut -d " " -f 1`.wav
	TEMP_FILE_CQT=`md5sum -b ${file} | cut -d " " -f 1`.cqt

	# output filename
	#----------------------------------------------------------
	tmp=`basename ${file}`
	outputFilename=$OUTPUT_DIR/${tmp%.wav}.cqt

	# resampling to 16 kHz
	#----------------------------------------------------------
	sox $file -c 1 -b 16 -r 16000 ${TEMP_FILE}
	samplingRate=`soxi -r ${TEMP_FILE}`
	shift=`expr $samplingRate / 10` # it should be set suitably by the user

	# printf debug
	#----------------------------------------------------------
	echo Input File = $file
	echo Output File = $outputFilename

	# do
	#----------------------------------------------------------
	$WAV2CQT --input ${TEMP_FILE} \
		--srate $samplingRate \
		--fshift $shift \
		--startfreq 110 \
		--Q 45 \
		--resol 1 \
		--nnyq 72 \
		--Format csv > $outputFilename 2> /dev/null # do in parallel

  # sed -e '1d' $outputFilename > ${TEMP_FILE_CQT} # remove the initial line if needed (time = 0)
  # sed -e '$d' ${TEMP_FILE_CQT} > $outputFilename # remove the last line if needed ( time = end)

	rm ${TEMP_FILE} ${TEMP_FILE_CQT}
}

export -f wav2cqt_onefile

#=======================================================================================
function wav2cqt(){
	DATA_DIR=$1
	OUTPUT_DIR=$2

	# error check 1
	#------------------------------------
	if [ -e ${DATA_DIR} ]; then 
		:
	else 
		echo There is no directory named ${DATA_DIR}
		exit 1
	fi

	# error check 2
	#-------------------------------------
	if [ -e ${OUTPUT_DIR} ]; then 
		: 
	else
		mkdir ${OUTPUT_DIR}
	fi

	find ${DATA_DIR} -name \*.wav -print0 | xargs -0 -n1 -P${N_THREADS} -I % bash -c "wav2cqt_onefile \"%\" ${OUTPUT_DIR}"

}


#=======================================================================================
# convert all the wav data to CQT
#---------------------------------

time wav2cqt ${WAVDIR} ${CQTDIR}

