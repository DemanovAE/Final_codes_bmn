#include "QA_function.C"

void RunByRunAnalis(TFile *inFile, DrawParameters obj_par, std::string path_save, TFile *outFile){

	//_RunIdStart = 7000;
	//_RunIdStop 	= 8300;

	TH1D *hist;
	TH1D *hist_bad;
	TGraphErrors *Def;
	TGraphErrors *BadRunsGr;
   	TLatex text;

	std::vector<double> x_err;
	std::vector<double> runId;
	std::vector<double> mean;
	std::vector<double> mean_err; //""
	std::vector<double> sigma; //"s"

	std::vector<double> runId_iter;
	std::vector<double> runId_iter_err;
	std::vector<double> mean_iter;
	std::vector<double> mean_iter_err;

	std::vector<double> BadRunsMean_x_err;
	std::vector<double> BadRunsMean_runId;
	std::vector<double> BadRunsMean_mean;
	std::vector<double> BadRunsMean_mean_err; //""
	std::vector<double> BadRunsMean_sigma; //"s"

	std::vector<double> GoldMean;
	std::vector<double> GoldSigma;

	TH2D *pr1 = GetTH2(inFile,obj_par);
	if(pr1==nullptr){
		std::cout<<"Error!"<<std::endl;
		return;
	}
	
	for(int i=0; i<(int)pr1->GetNbinsX(); i++){
		int h2RunId = pr1->GetXaxis()->GetBinCenter(i);
		if(h2RunId<_RunIdStart || h2RunId>_RunIdStop)continue;
		hist = (TH1D*)pr1->ProjectionY(Form("run_%s_%i",pr1->GetName(),i),pr1->GetXaxis()->FindBin(h2RunId),pr1->GetXaxis()->FindBin(h2RunId));
		if(hist->GetEntries()==0) continue;
		if(hist->Integral()==0) continue;
		runId.push_back((int)h2RunId);
		//mean.push_back(hist->Integral());
		mean.push_back(hist->GetMean());
		mean_err.push_back(hist->GetMeanError());
		sigma.push_back(hist->GetStdDev());
		x_err.push_back(0.);
		//if(h2RunId==8005) std::cout<<pr1->GetName()<<"\t"<<hist->GetMean()<<"\n";
		//std::cout<<mean.back()<<"\t"<<mean_err.back()<<std::endl;
	}

	if(mean.size()==0) return;
	double h[2]; // mean
	double s[2]; // std deviations (RMS)
	double e[2]; // mean err
	parRunByRun(_RunIdStart,_RunIdStop,runId,mean,	h[0],s[0],e[0]);
	parRunByRun(_RunIdStart,_RunIdStop,runId,mean_err,h[1],s[1],e[1]);	
	//std::cout<<mean.size()<<"\t"<<h[0]<<"\t"<<s[0]<<"\t"<<e[0]<<"\n";
	//std::cout<<mean.size()<<"\t"<<h[1]<<"\t"<<s[1]<<"\t"<<e[1]<<"\n";
	double AxisYMax = h[0]+5*s[0];
	double AxisYMin = h[0]-5*s[0];
	double AxisYH   = (AxisYMax - AxisYMin);

	GoldMean.push_back(h[0]);
	GoldSigma.push_back(s[0]);
	SetVectorsOutOfRMS(	_RunIdStart,_RunIdStop,runId,mean,BadRunsMean_runId,BadRunsMean_mean,BadRunsMean_x_err,BadRunsMean_mean_err,GoldMean.back(),GoldSigma.back());
	SetVectorsInsideRMS(_RunIdStart,_RunIdStop,runId,mean,runId_iter,mean_iter,runId_iter_err,mean_iter_err,GoldMean.back(),GoldSigma.back());
	
	for(int i=0; i<_iter; i++){ // _iter in QA_functions
		GoldMean.push_back(0.);
		GoldSigma.push_back(0.);
		parRunByRun(_RunIdStart,_RunIdStop,runId_iter,mean_iter,GoldMean.back(),GoldSigma.back(),e[0]);
		SetVectorsOutOfRMS(_RunIdStart,_RunIdStop,runId,mean,BadRunsMean_runId,BadRunsMean_mean,BadRunsMean_x_err,BadRunsMean_mean_err,GoldMean.back(),GoldSigma.back());
		SetVectorsInsideRMS(_RunIdStart,_RunIdStop,runId,mean,runId_iter,mean_iter,runId_iter_err,mean_iter_err,GoldMean.back(),GoldSigma.back());
		//std::cout<<i<<"\t"<<runId_iter.size()<<"\t"<<GoldMean.back()<<"\t"<<GoldSigma.back()<<"\n";
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw RMS (test)////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	TCanvas *can22 = new TCanvas(Form("test_%s",pr1->GetName()),"",960,560);
	can22->cd();
	TH1D *hist_test = new TH1D(Form("testh_%s",pr1->GetName()),"",50,AxisYMin,AxisYMax);
	for(auto obj : mean){
		hist_test->Fill(obj);
	}
	hist_test->Draw();
	auto *legend_hhh = new TLegend(0.7,0.7,0.89,0.89);
	legend_hhh->AddEntry(hist_test,Form("#font[42]{ #scale[1.0]{#mu=%.4f}}",		hist_test->GetMean()),	"");
	legend_hhh->AddEntry(hist_test,Form("#font[42]{ #scale[1.0]{#sigma=%.4f}}",	hist_test->GetRMS()),	"");
	legend_hhh->AddEntry(hist_test,Form("#font[42]{ #scale[1.0]{#mu=%.4f}}",	h[0]),	"");
	legend_hhh->AddEntry(hist_test,Form("#font[42]{ #scale[1.0]{#sigma=%.4f}}",	s[0]),	"");
	legend_hhh->Draw("same");
	can22->SaveAs(Form("%s/test_%s.png",path_save.c_str(),pr1->GetName()));
	*/


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Ran-by-Run///////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TCanvas *can = new TCanvas(Form("RunId_%s",pr1->GetName()),"",5*500,2*500);
	can->cd();

	Def = new TGraphErrors(mean.size(),&runId[0],&mean[0],&x_err[0],&mean_err[0]);
	Def->SetName(Form("gr_%s",pr1->GetName()));
	Def->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	Def->SetLineColor(4);
	Def->SetMarkerSize(1);
	Def->SetMarkerStyle(20);
	Def->SetMarkerColor(4);
	auto max = (Def->GetYaxis()->GetXmax()>AxisYMax ? Def->GetYaxis()->GetXmax() : AxisYMax);
	auto min = (Def->GetYaxis()->GetXmin()<AxisYMin ? Def->GetYaxis()->GetXmin() : AxisYMin);
	Def->GetYaxis()->SetRangeUser(min, max);
	//std::cout<<min<<"\t"<<max<<"\t"<<AxisYMin<<"\t"<<AxisYMax<<"\n";
	TH2D *h2_axis = new TH2D("","",2,RunIdMin,RunIdMax,2,min,max);
	h2_axis->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	h2_axis->GetXaxis()->SetTitle(Form("%s",pr1->GetXaxis()->GetTitle()));
	h2_axis->Draw();
	Def->Draw("sameP");

	BadRunsGr = new TGraphErrors(BadRunsMean_mean.size(),&BadRunsMean_runId[0],&BadRunsMean_mean[0],&BadRunsMean_x_err[0],&BadRunsMean_mean_err[0]);
	BadRunsGr->SetName(Form("%s_BadRuns",Def->GetName()));
	BadRunsGr->SetLineColor(2);
	BadRunsGr->SetMarkerSize(1);
	BadRunsGr->SetMarkerStyle(Def->GetMarkerStyle());
	BadRunsGr->SetMarkerColor(2);
	BadRunsGr->Draw("Psame");

	for(int i=0; i<GoldMean.size(); i++){
		TLine *lineMean = new TLine(RunIdMin,GoldMean[i],RunIdMax,GoldMean[i]);
		lineMean->SetLineColorAlpha(kBlack,0.5);
		lineMean->SetLineStyle(1);
		lineMean->Draw("same");

		TLine *lineUp = new TLine(RunIdMin,nSigma*GoldSigma[i] + GoldMean[i],RunIdMax,nSigma*GoldSigma[i] + GoldMean[i]);
		lineUp->SetLineColorAlpha(kRed,0.5);
		lineUp->SetLineStyle(2);
		lineUp->Draw("same");

		TLine *lineDown = new TLine(RunIdMin, -nSigma*GoldSigma[i] + GoldMean[i],RunIdMax,-nSigma*GoldSigma[i] + GoldMean[i]);
		lineDown->SetLineColorAlpha(kRed,0.5);
		lineDown->SetLineStyle(2);
		lineDown->Draw("same");
	}

	std::string BadRuns;// = Form("%s: ",pr1->GetName());
   	if(BadRunsMean_runId.size()>0)BadRuns = BadRuns + Form("beyond #pm3#sigma: ");
   	for(int dd = 0; dd<BadRunsMean_runId.size(); dd++){
   		if(dd<BadRunsMean_runId.size()-1)BadRuns = BadRuns + Form("%.0f",BadRunsMean_runId[dd]) + ", ";
   		if(dd==BadRunsMean_runId.size()-1)BadRuns = BadRuns + Form("%.0f",BadRunsMean_runId[dd]);
   		BadRunsGlobal.push_back(BadRunsMean_runId[dd]);
   	}

   	//beyond ±3σ   
    TPad *pad1 = new TPad("pad_1", "", 0.  , 0., 0.8, 0.1, 0, 0, 0);
    pad1->Draw();
    pad1->cd();
    text.SetTextSize(0.5);
    text.SetTextColor(kRed);
	text.DrawLatex( 0.01, 0.1, BadRuns.c_str());

   	outFile->cd();
   	Def->Write();
   	BadRunsGr->Write();

	can->SaveAs(Form("%s/%s.png",path_save.c_str(), pr1->GetName()));
	can->Write();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw part (H1)////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TCanvas *can2 = new TCanvas(Form("%s_%i-%i",pr1->GetName(),(int)th1_RanRange.first,(int)th1_RanRange.second),"",3*500,2*500);
	can2->cd();
	hist = (TH1D*)pr1->ProjectionY(Form("run_%s_h1",pr1->GetName()),pr1->GetXaxis()->FindBin(th1_RanRange.first),pr1->GetXaxis()->FindBin(th1_RanRange.second));
	hist->GetYaxis()->SetTitle("dN/N");
	hist->GetXaxis()->SetTitleOffset(1);	
	if(obj_par.LogScale==1)can2->SetLogy();
	if(obj_par.Marker!=0)hist->SetMarkerStyle(obj_par.Marker);
	if(obj_par.Color !=0)hist->SetMarkerColor(obj_par.Color);
	if(obj_par.Color !=0)hist->SetLineColor(obj_par.Color);
	if(obj_par.Width !=0)hist->SetLineWidth(obj_par.Width);
	hist->Scale(1./hist->Integral());
	if(obj_par.LogScale==0)hist->GetYaxis()->SetRangeUser(0.,hist->GetMaximum()*1.4);
	hist->Draw("HIST");
	int BadRunsDraw = 0;

	if(BadRunsMean_runId.size()!=0){
		BadRunsDraw = BadRunsMean_runId.back();
		//if(obj_par.Name=="h2_RunId_fhcal_e"){
		//	BadRunsDraw = BadRunsMean_runId[BadRunsMean_runId.size()-3];
		//}
		hist_bad = (TH1D*)pr1->ProjectionY(Form("run_%s_h1b",pr1->GetName()),pr1->GetXaxis()->FindBin(BadRunsDraw),pr1->GetXaxis()->FindBin(BadRunsDraw));
		if(obj_par.LogScale==1)can2->SetLogy();
		if(obj_par.Marker!=0)hist_bad->SetMarkerStyle(obj_par.Marker);
		if(obj_par.Color !=0)hist_bad->SetMarkerColor(obj_par.Color);
		if(obj_par.Color !=0)hist_bad->SetLineColor(kRed);
		if(obj_par.Width !=0)hist_bad->SetLineWidth(obj_par.Width);
		hist_bad->Scale(1./hist_bad->Integral());
		hist_bad->Draw("sameHIST");
		//if(obj_par.Name=="h2_RunId_fhcal_e"){
		//	hist_bad->Draw("HIST");
		//	hist->Draw("HISTsame");
		//}
	}
	auto *legend = new TLegend(0.8,0.7,0.89,0.89);
	TLegendEntry* l2 = legend->AddEntry(hist,Form("%i",(int)th1_RanRange.first),"");
    l2->SetTextColor(kBlue);
	
	if(BadRunsMean_runId.size()!=0){
		TLegendEntry* l2 = legend->AddEntry(hist_bad,Form("%i",(int)BadRunsDraw),"");
    	l2->SetTextColor(kRed);
	}
	legend->Draw("same");
	
	gSystem->Exec(Form("mkdir -p %s/H1",path_save.c_str()));
    can2->SaveAs(Form("%s/H1/%s.png",path_save.c_str(), pr1->GetName()));
	can2->Write();

}

void GoldRuns_script(string _inFileName = "/home/aleksandr/BM@N/qa_root/qa_run8_vf_24.12_plp.root", string _outFileName="qa_24.12_out.root"){	
	
	//makeplotstyle();
	//TFile *fstyle = new TFile("./style.root");
	//TStyle *tsty = (TStyle *)fstyle->Get("PlottingInStyle");
	//tsty->cd();
 	gROOT->SetStyle("Pub");
	gROOT->ForceStyle();
	// RunByRun
	std::string path_to_project="/home/aleksandr/BM@N";
	std::string path = path_to_project + "/Final_codes_bmn/pict/QA_RunByRun_24.12";
	gSystem->Exec(Form("mkdir -p %s",path.c_str()));
	gSystem->Exec(Form("mkdir -p %s/H1",path.c_str()));
	TFile *file_inFile 	= new TFile(_inFileName.c_str(),"READ");	
	TFile *file_outFile = new TFile(Form("%s/%s",path.c_str(),_outFileName.c_str()),"RECREATE");


	TProfile *pr;
	TH1D *h1;
	TH2D *h2;

	std::string suf = "nVtxTr_";
	std::vector<DrawParameters> DrawObjectParH2RunId;
  

	DrawObjectParH2RunId.push_back({"h2_RunId_gemDigits",suf,0,4,3.,{},{0,6000},0});
	DrawObjectParH2RunId.push_back({"h2_RunId_stsDigits",suf,0,4,3.,{},{0,6000},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_tof400Digits",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_tof700Digits",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_tof701Digits",suf,0,4,3.,{},{},1});

	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_n_tracks",suf,0,4,3.,{},{0,200},1});                                                                                             
	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_chi2_ndf",suf,0,4,3.,{},{0,10},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_x",suf,0,4,3.,{},{-4,4}});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_y",suf,0,4,3.,{},{-4,4}});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_vtx_z",suf,0,4,3.,{},{-4,4}}); 

	DrawObjectParH2RunId.push_back({"h2_RunId_nTracks",suf,0,4,3.,{},{1,500},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_bdNum",suf,0,4,3.,{},{0,70},0});
	DrawObjectParH2RunId.push_back({"h2_RunId_bdNumSingle",suf,0,4,3.,{},{0,70},0});
	DrawObjectParH2RunId.push_back({"h2_RunId_nbdMod",suf,0,4,3.,{},{0,41},0});
	DrawObjectParH2RunId.push_back({"h2_RunId_nbdModSingle",suf,0,4,3.,{},{0,41},0});
	DrawObjectParH2RunId.push_back({"h2_RunId_bc1sInt",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_fdInt",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_fdAmp",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_bc1sAmp",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_vcsAmp",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_vcsInt",suf,0,4,3.,{},{},1});
	
	DrawObjectParH2RunId.push_back({"h2_RunId_fhcal_e",suf,0,4,3.,{},{},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_scwall_q",suf,0,4,3.,{},{0,600},1});
	DrawObjectParH2RunId.push_back({"h2_RunId_hodo_q",suf,0,4,3.,{},{},1});
	
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_x_st0",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_y_st0",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_x_st1",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_y_st1",suf,0,4,2,{},{-5,5}});	
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_x_st2",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_hit_y_st2",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_x",suf,0,4,2,{},{-5,5}});
	DrawObjectParH2RunId.push_back({"h2_RunId_beam_y",suf,0,4,2,{},{-5,5}});

	DrawObjectParH2RunId.push_back({"h2_RunId_tr_nhits",suf,0,4,3.,{},{0,20},0});                                                                
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_chi2_ndf",suf,0,4,3.,{},{0,10},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_eta",suf,0,4,3.,{},{0.,5.},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_phi",suf,0,4,3.,{},{-3.14,3.14},0});                              
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_p",suf,0,4,3.,{},{0,40},1});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_px",suf,0,4,3.,{},{-2,2},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_py",suf,0,4,3.,{},{-2,2},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_pz",suf,0,4,3.,{},{0.,6.},0});                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_pT",suf,0,4,3.,{},{0,2},0});                                                                 
	DrawObjectParH2RunId.push_back({"h2_RunId_tr_dca_r",suf,0,4,3.,{},{0,5.},0});                                                                                                   


	for(auto obj : DrawObjectParH2RunId){
		RunByRunAnalis(file_inFile, obj, path, file_outFile);
	}

	PrintBadRuns(file_inFile,suf);

	file_outFile->Close();
	file_inFile->Close();


}
