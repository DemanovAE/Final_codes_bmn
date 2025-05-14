#!/bin/bash

MainName=glauber_qa_50_220_gamma_vtxZ15_reweightB
#MainName=glauber_qa_5_raw_plp
#InmutHistoName=refMultRun8_VtxR1_VtxZ010_7400_7450_scale
InmutHistoName=Vtx_VtxZ15_h1_nTracksCh_eta3_scale
CentClass=10

NameInFile=${MainName}.root
NameHistoCut=${MainName}_HistoCut.root
NameFInal=${MainName}_Final.root
GlauberQA=/home/aleksandr/BM@N/cent_root_file/${NameInFile}
PathToOutDir=/home/aleksandr/BM@N/centrality_result/${MainName}
OutFinalFile=${PathToOutDir}/MultCut.h

DIR_MACRO=/home/aleksandr/BM@N/macro_cent
CurrenrDir=${PWD}

mkdir -p ${PathToOutDir}
cd ${PathToOutDir}

cp ${GlauberQA} ${PathToOutDir}

echo "Input glauber_qa file: ${GlauberQA}"
echo "Input name histo: ${InmutHistoName}"
echo "HistoCut file: ${PathToOutDir}/${NameHistoCut}"
echo "Funal file: ${PathToOutDir}/${NameFInal}"

echo "-------------------------------------------------------------------------------------"

echo "Start HistoCut.C"
root -q -b -l ${DIR_MACRO}/HistoCut.C"(${CentClass}, \"${GlauberQA}\", \"${PathToOutDir}/${NameHistoCut}\", \"${InmutHistoName}\")"
echo "End HistoCut.C"

echo "Start CentralityClasses.C"
root -q -b -l ${DIR_MACRO}/CentralityClasses.C"(${CentClass}, \"${GlauberQA}\", \"${PathToOutDir}/${NameHistoCut}\", \"${PathToOutDir}\",\"${PathToOutDir}/${NameFInal}\")"
echo "End CentralityClasses.C"

echo "Start printFinal.C"
root -q -b -l ${DIR_MACRO}/printFinal.C"(\"${PathToOutDir}/${NameFInal}\", \"${OutFinalFile}\")"
echo "End printFinal.C"

root -l -q -b ${DIR_MACRO}/DrawFinalPict.cpp"(\"${GlauberQA}\",\"${PathToOutDir}/${NameFInal}\",\"${InmutHistoName}\")"

cd ${CurrenrDir}