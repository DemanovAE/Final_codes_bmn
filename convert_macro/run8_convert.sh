#!/bin/bash

#SBATCH -p nica
#SBATCH -t 6:00:00
#SBATCH --mem=8GB
#SBATCH -J Converter
#SBATCH -a 1-999
#SBATCH -o /nica/mpd1/demanov/data_bmn/log/%A_%a.log
#SBATCH -x ncx157,ncx208,ncx138,ncx211,ncx187,ncx111,ncx112,ncx113,ncx115,ncx165,ncx177,ncx121,ncx123,ncx153,ncx171,ncx181,ncx214,ncx158,ncx159,ncx164,ncx171,ncx177,ncx180,ncx188,ncx203,ncx213,ncx216,ncx222,ncx223,ncx224,ncx225,ncx227

digi_directory=/eos/nica/bmn/exp/digi/run8/24.12.0/
dst_directory=/eos/nica/bmn/exp/dst/run8/24.12.0/
plp_directory=/eos/nica/mpd/users/ogolosov/bmn_pileup/hitInfo/

CONVERTER_MACRO=/lhep/users/demanov/bmn/convert_run8_24.12/convertBmn_run8.C

TASK_ID=$SLURM_ARRAY_TASK_ID

file_list=$1
out_dir=$2

mkdir -p $out_dir
mkdir -p "${out_dir}/log"
cd $out_dir

name_file_root=$(basename $(head -n $TASK_ID $file_list | tail -n 1 ))
name_file=${name_file_root%.root*}
input_file_dst=${dst_directory}/${name_file}.root
input_file_digi=${digi_directory}/${name_file}.root
input_file_plp=${plp_directory}/${name_file}.hitInfo.root
output_file=${out_dir}/${name_file}.tree.root
output_log=${out_dir}/log/${SLURM_ARRAY_JOB_ID}_${TASK_ID}_${name_file}.log

source /cvmfs/nica.jinr.ru/sw/os/login.sh legacy
source /cvmfs/bmn.jinr.ru/config/x86_64-centos7/cluster_config.sh
#source /cvmfs/bmn.jinr.ru/bmnroot/24.12.0/x86_64-centos7/bmnroot_config.sh
source /lhep/users/demanov/Soft/bmnroot/build/config.sh

echo "Node name: ${SLURMD_NODENAME}" &>> $output_log
echo "Job ID: ${SLURM_ARRAY_JOB_ID}" &>> $output_log
echo "Task ID: ${TASK_ID}" &>> $output_log
echo "line: ${TASK_ID}" &>> $output_log
echo "name: ${name_file}" &>> $output_log
echo "input digi: ${input_file_digi}" &>> $output_log
echo "input dst: ${input_file_dst}" &>> $output_log
echo "input plp: ${input_file_plp}" &>> $output_log
echo "output tree: ${output_file}" &>> $output_log
echo "output log: ${output_log}" &>> $output_log

time root -l -b -q ${CONVERTER_MACRO}"(\"${input_file_dst}\", \"${input_file_digi}\",\"${input_file_plp}\", \"${output_file}\")" &>> $output_log

echo "Done!" &>> $output_log
