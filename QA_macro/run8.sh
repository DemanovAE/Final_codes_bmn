#!/bin/bash

#SBATCH -p nica
#SBATCH -t 12:00:00
#SBATCH -J QA
#SBATCH --mem-per-cpu=8G
#SBATCH -a 1-728
#SBATCH -o /nica/mpd1/demanov/data_bmn/qa_run8_out/log/%A_%a.out
#SBATCH -e /nica/mpd1/demanov/data_bmn/qa_run8_out/log/%A_%a.err
#SBATCH -x ncx157,ncx208,ncx138,ncx211,ncx187,ncx111,ncx112,ncx113,ncx115,ncx165,ncx177,ncx121,ncx123,ncx153,ncx171,ncx181,ncx214,ncx158,ncx159,ncx164,ncx171,ncx177,ncx180,ncx188,ncx203,ncx213,ncx216,ncx222,ncx223,ncx224,ncx225,ncx227

source /cvmfs/nica.jinr.ru/sw/os/login.sh legacy
source /cvmfs/nica.jinr.ru/centos7/bmn/env.sh
source /lhep/users/demanov/Soft/bmnroot/build/config.sh

#source /lhep/users/ogolosov/soft/bmnroot/SetEnv.sh
#source /lhep/users/ogolosov/soft/bmnroot/install/bmnroot_config.sh

#SLURM_ARRAY_JOB_ID=99
#SLURM_ARRAY_TASK_ID=1112

export JOB_ID=${SLURM_ARRAY_JOB_ID}
export TASK_ID=$((SLURM_ARRAY_TASK_ID+9999))
#export TASK_ID=$((SLURM_ARRAY_TASK_ID+9999+999))
#export TASK_ID=$((SLURM_ARRAY_TASK_ID+9999+1998))
#export TASK_ID=$((SLURM_ARRAY_TASK_ID+9999+2997))
#export TASK_ID=$((SLURM_ARRAY_TASK_ID+9999+3996))

export MAIN_DIR=/lhep/users/demanov/bmn/qa
export IN_FIT_FILE=${MAIN_DIR}/OUT_24.04_runId.root
export INPUT_DIR=${MAIN_DIR}/lists
export INPUT_DIR_PLP=${MAIN_DIR}/lists_plp
export MACRO_EXE=${MAIN_DIR}/run8_qa_new.C
export OUT_DIR=/nica/mpd1/demanov/data_bmn/qa_run8_out/qa_24.12.0_plp_def_apr
export LOG_DIR=${OUT_DIR}/log

export OUT_FILE=${OUT_DIR}/qa_run8_${JOB_ID}_${TASK_ID}.root
export INPUT_FILE=${INPUT_DIR}/list_${TASK_ID}.list
export INPUT_FILE_PLP=${INPUT_DIR_PLP}/list_${TASK_ID}.list
export LOG=${LOG_DIR}/log_qa_run8_${JOB_ID}_${TASK_ID}.log

mkdir -p $OUT_DIR
mkdir -p $LOG_DIR

# Main process
echo "Node name: ${SLURMD_NODENAME}" &>> $LOG
echo "Fit file: ${IN_FIT_FILE}" &>> $LOG
echo "INFILE:  ${INPUT_FILE}" &>> $LOG
echo "INFILE PLP:  ${INPUT_FILE_PLP}" &>> $LOG
echo "Job Id:  ${JOB_ID}" &>> $LOG
echo "Task Id: ${TASK_ID}" &>> $LOG
echo "OUTFILE: ${OUT_FILE}" &>> $LOG

cd $MAIN_DIR
root -l -b -q $MACRO_EXE'("'${INPUT_FILE}'","","'${OUT_FILE}'","'${IN_FIT_FILE}'")' &>> $LOG
#root -l -b -q $MACRO_EXE'("'${INPUT_FILE}'","'${INPUT_FILE_PLP}'","'${OUT_FILE}'","'${IN_FIT_FILE}'")' &>> $LOG
echo "Job is finished" &>> $LOG
