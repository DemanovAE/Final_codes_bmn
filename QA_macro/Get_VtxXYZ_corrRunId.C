#include "QA_function.C"

void MeanVtxParRunByRun(TFile *inFile, DrawParameters obj_par, std::string path_save, TFile *outFile){

	TH1D *hist;
	TGraphErrors *Def;
	TGraphErrors *DefMean;

	std::vector<double> x_err;
	std::vector<double> runId;
	std::vector<double> mean;
	std::vector<double> meanGaus;
	std::vector<double> mean_err; //""

	TH2D *pr1 = GetTH2(inFile,obj_par);
	if(pr1==nullptr){
		std::cout<<"Error!"<<std::endl;
		return;
	}
		
	for(int i=0; i<(int)pr1->GetNbinsX(); i++){
		int h2RunId = pr1->GetXaxis()->GetBinCenter(i);
		//if(h2RunId<RunIdStart)continue;
		hist = (TH1D*)pr1->ProjectionY(Form("run_%s_%i",pr1->GetName(),h2RunId),pr1->GetXaxis()->FindBin(h2RunId),pr1->GetXaxis()->FindBin(h2RunId));
		if(hist->GetEntries()==0) continue;
		if(hist->Integral()==0) continue;
		
		hist->SetLineColor(kBlue);
		hist->SetTitle(Form("Run %i",h2RunId));

		double min = -1.;
		double max =  1.5;
		if(obj_par.Name=="h2_RunId_vtx_z"){
			min=-0.2;
			max= 0.2;
		}
		if(obj_par.Name=="h2_RunId_vtx_y"){
			min=-1.3;
			max= 1.3;
		}
		auto *fit_gaus = new TF1(Form("gaus_%s_%i",pr1->GetName(),h2RunId),"gaus",min,max);
		hist->Fit(fit_gaus,"QRM");

		runId.push_back((int)h2RunId);
		mean.push_back(hist->GetMean());
		meanGaus.push_back(fit_gaus->GetParameter(1));
		mean_err.push_back(0.);
		x_err.push_back(0.);
		

		//if(abs(mean.back()-meanGaus.back())>0.05)std::cout<<runId.back()<<"\t"<<mean.back()<<"\t"<<meanGaus.back()<<"\t"<<abs(mean.back()-meanGaus.back())<<"\n";
	}

	if(mean.size()==0) return;

	TCanvas *can = new TCanvas(Form("RunId_%s",pr1->GetName()),"",960,560);
	can->cd();

	Def = new TGraphErrors(mean.size(),&runId[0],&mean[0],&x_err[0],&mean_err[0]);
	Def->SetName(Form("grNew_%s",pr1->GetName()));
	Def->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	Def->SetLineColor(kBlue);
	Def->SetMarkerSize(1);
	Def->SetMarkerStyle(8);
	Def->SetMarkerColor(kBlue);
	Def->Draw("AP");

	DefMean = new TGraphErrors(meanGaus.size(),&runId[0],&meanGaus[0],&x_err[0],&mean_err[0]);
	DefMean->SetName(Form("grGausNew_%s",pr1->GetName()));
	DefMean->GetYaxis()->SetTitle(Form("Mean %s (Gaus)",pr1->GetYaxis()->GetTitle()));
	DefMean->SetLineColor(kRed);
	DefMean->SetMarkerSize(1);
	DefMean->SetMarkerStyle(8);
	DefMean->SetMarkerColor(kRed);
	DefMean->Draw("Psame");

   	outFile->cd();
   	Def->Write();
   	DefMean->Write();

	gSystem->Exec(Form("mkdir -p %s/VtxXYZ",path_save.c_str()));
	can->SaveAs(Form("%s/VtxXYZ/Mean_%s.png",path_save.c_str(), pr1->GetName()));
}


void Get_VtxXYZ_corrRunId(string iFile = "../qa_run8_vf_24.04_refMult2.root", string oFile = "../OUT_24.04_VtxXYZ.root", string iSuf = "nVtxTr_"){	
	
	makeplotstyle();
	TFile *fstyle = new TFile("./style.root");
	TStyle *tsty = (TStyle *)fstyle->Get("PlottingInStyle");
	tsty->cd();
 	//gROOT->SetStyle("Pub");
	gROOT->ForceStyle();

	TFile *file_inFile 	= new TFile(iFile.c_str(),"READ");
	TFile *file_outFile = new TFile(oFile.c_str(),"RECREATE");	

	// RunByRun
	std::string path_to_project="/home/aleksandr/BM@N";
	std::string path = path_to_project + "/Final_codes_bmn/pict/QA_RunByRun_24.12_3gev";
	gSystem->Exec(Form("mkdir -p %s",path.c_str()));

	std::string suf = iSuf;
	std::vector<DrawParameters> DrawObjectParH2;

	DrawObjectParH2.push_back({"h2_RunId_vtx_x",suf,0,4,2.,{},{-1,1.5},1});
	DrawObjectParH2.push_back({"h2_RunId_vtx_y",suf,0,4,2.,{},{-1.5,1.5},1});
	DrawObjectParH2.push_back({"h2_RunId_vtx_z",suf,0,4,2.,{},{-0.5,0.5},1});

	for(auto obj : DrawObjectParH2){
		MeanVtxParRunByRun(file_inFile,obj,path,file_outFile);
	}

	file_outFile->Close();
	file_inFile->Close();

}