#include "QA_function.C"

void rescaleaxis(TGraph *g,double scale){
	int N=g->GetN();
	double *y=g->GetY();
	int i=0; 
	while(i<N){
		y[i]=y[i] / scale; 
		i=i+1;
	} 
	g->GetHistogram()->Delete();
	g->SetHistogram(0);
}

double GetX_percent(TF1 *fit, double percent){
	if(fit==nullptr) return -9999;
	double max = -999;
	double min = -999;
	
	fit->GetRange(min,max);
	min=0;

	double FullInt = fit->Integral(min,max);
	fit->SetNpx(1000);
	TGraph *gr_percent = (TGraph*)fit->DrawIntegral("al");	
	rescaleaxis(gr_percent,FullInt);
	TGraph invg(gr_percent->GetN(), gr_percent->GetY(), gr_percent->GetX());
	
	return invg.Eval(percent);
}

void GemDigitNTracksFitGamma_X_new(TFile *inFile, DrawParameters obj_par, std::string path_save, TFile *outFile){

	TH1D *hist;

	std::vector<double> x;
	std::vector<double> x_err;
	std::vector<double> y;
	std::vector<double> y_err;
	std::vector<double> y_low;
	std::vector<double> y_high;
	std::vector<double> p0_fit;
	std::vector<double> p1_fit;
	std::vector<double> p2_fit;

	std::vector<double> y_per_low;
	std::vector<double> y_per_high;

	std::vector<int> rebin;
	for(int i=0; i<=100; i++)
		rebin.push_back(i);
	for(int i=102; i<=160; i=i+2)
		rebin.push_back(i);
	for(int i=165; i<=200; i=i+5)
		rebin.push_back(i);
	for(int i=210; i<=500; i=i+10)
		rebin.push_back(i);


	int StopFit = 350;
	double minFitRange = 5.;
	double maxFitRange = 10000.;
	
	TF1 *fit_range = new TF1("FirRange_GemDigit","24.*x+4000.",0,400);
	fit_range->SetLineColor(1);

	TF1 *fit_low  = new TF1("fl","pol3(0)",21,500);
	TF1 *fit_high = new TF1("fh","pol3(0)",0,500);
	fit_low  -> SetLineColor(kRed);
	fit_high -> SetLineColor(kRed);

	TH2D *pr1 = GetTH2(inFile,obj_par);
	if(pr1==nullptr){
		std::cout<<"Error!"<<std::endl;
		return;
	}
	
	TStopwatch timer1;
  	timer1.Start();

	for(int i=3; i<(int)(StopFit); i++){
		int h2BinX = pr1->GetXaxis()->GetBinCenter(i);
		if(h2BinX<1)continue;
		hist = (TH1D*)pr1->ProjectionY(Form("nTrack_%s_%i",pr1->GetName(),h2BinX),pr1->GetXaxis()->FindBin(h2BinX),pr1->GetXaxis()->FindBin(h2BinX));
		if(hist->GetEntries()==0) continue;
		if(hist->Integral()==0) continue;
		//hist->rebin
		hist->SetLineColor(kBlue);
		hist->SetTitle(Form("nTracks=%i",h2BinX));

		//TF1 *fit1 = new TF1(Form("f1_pr_nbd_%s_%i",pr1->GetName(),i),"[0]*ROOT::Math::negative_binomial_pdf(x,[1],[2])", minFitRange,maxFitRange);
		TF1 *fit1 = new TF1(Form("f1_pr_nbd_%s_%i",pr1->GetName(),i),"[0]*ROOT::Math::gamma_pdf(x,[1],[2])", minFitRange,fit_range->Eval(i));
		if(p0_fit.size()>1){
			fit1->SetParameters(p0_fit.back(),p1_fit.back(),p2_fit.back());
		}else{
			fit1->SetParameters(100000,0.02,20);
		}
		fit1->SetLineColor(kRed);
		hist->Fit(fit1,"QRM");
		hist->Fit(fit1,"QRM");

		// change 1.1
		double per_min = GetX_percent(fit1, 0.002);
		double per_max = GetX_percent(fit1, 0.998);

		x.push_back((double)h2BinX);
		x_err.push_back(0.);
		y_low.push_back(per_min);
		y_high.push_back(per_max);
		y_err.push_back(0.);

		p0_fit.push_back(fit1->GetParameter(0));
		p1_fit.push_back(fit1->GetParameter(1));
		p2_fit.push_back(fit1->GetParameter(2));

		//std::cout<<mean.back()<<"\t"<<mean_err.back()<<std::endl;
		outFile->cd();
		hist->Write();
	}

	timer1.Stop();
  	timer1.Print();

	if(x.size()==0) return;

	TGraphErrors *gr_low = new TGraphErrors(x.size(),&x[0],&y_low[0],&x_err[0],&y_err[0]);
	gr_low->SetName(Form("gr_%s",pr1->GetName()));
	gr_low->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_low->SetLineColor(2);
	gr_low->SetMarkerSize(1);
	gr_low->SetMarkerStyle(8);
	gr_low->SetMarkerColor(4);
	//gr_low->Fit(fit_low,"QRM");


	TGraphErrors *gr_high = new TGraphErrors(x.size(),&x[0],&y_high[0],&x_err[0],&y_err[0]);
	gr_high->SetName(Form("gr_%s",pr1->GetName()));
	gr_high->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_high->SetLineColor(2);
	gr_high->SetMarkerSize(1);
	gr_high->SetMarkerStyle(8);
	gr_high->SetMarkerColor(4);
	//gr_high->Fit(fit_high,"QRM");

	TCanvas *can = new TCanvas(Form("StsDigitCut_%s",pr1->GetName()),"",960,560);
	can->cd();
	pr1->Draw("colz");
	can->SetLogz();
	gr_low->Draw("Lsame");
	gr_high->Draw("Lsame");
	//fit_low ->Draw("same");
	//fit_high->Draw("same");
	fit_range->Draw("same");

	std::cout<<fit_low->GetParameter(0)<<"\t\t"<<fit_low->GetParameter(1)<<"\t\t"<<fit_low->GetParameter(2)<<"\t\t"<<fit_low->GetParameter(3)<<"\n";
	std::cout<<fit_high->GetParameter(0)<<"\t\t"<<fit_high->GetParameter(1)<<"\t\t"<<fit_high->GetParameter(2)<<"\t\t"<<fit_high->GetParameter(3)<<"\n";

	gSystem->Exec(Form("mkdir -p ./%s/StsDigitCut",path_save.c_str()));
	can->SaveAs(Form("./%s/StsDigitCut/%s2_nbd_X_new2.png",path_save.c_str(), pr1->GetName()));
	
	gr_low->Write();
	gr_high->Write();
	can->Write();

}

void DigitNTracksFitGaus(TFile *inFile, DrawParameters obj_par, std::string path_save, TFile *outFile){

	TH1D *hist;

	std::vector<double> x;
	std::vector<double> x_err;
	std::vector<double> y;
	std::vector<double> y_err;
	std::vector<double> y_low;
	std::vector<double> y_high;
	std::vector<double> ampl_fit;
	std::vector<double> mean_fit;
	std::vector<double> sigma_fit;

	int StopFit = 250;
	double nSigma = 3.;
	double minFitRange = 10.;
	double maxFitRange = 6000.;
	
	TF1 *fit_mean = new TF1("fm","pol3(0)",0,1000);
	TF1 *fit_low  = new TF1("fl","pol3(0)",21,1000);
	TF1 *fit_high = new TF1("fh","pol3(0)",0,1000);

	fit_mean -> SetLineColor(kRed);
	fit_low  -> SetLineColor(kRed);
	fit_high -> SetLineColor(kRed);

	TH2D *pr1 = GetTH2(inFile,obj_par);
	if(pr1==nullptr){
		std::cout<<"Error!"<<std::endl;
		return;
	}
		
	for(int i=3; i<(int)(StopFit); i++){
		int h2BinX = pr1->GetXaxis()->GetBinCenter(i);
		if(h2BinX<1)continue;
		hist = (TH1D*)pr1->ProjectionY(Form("nTrack_%s_%i",pr1->GetName(),h2BinX),pr1->GetXaxis()->FindBin(h2BinX),pr1->GetXaxis()->FindBin(h2BinX));
		if(hist->GetEntries()==0) continue;
		if(hist->Integral()==0) continue;
		
		hist->SetLineColor(kBlue);
		hist->SetTitle(Form("nTracks=%i",h2BinX));

		auto gaus = new TF1(Form("f1_pr_%s_%i",pr1->GetName(),i),"gaus(0)",minFitRange,maxFitRange);
		if(ampl_fit.size()>1){
			gaus->SetParameter(0,ampl_fit.back());
			gaus->SetParameter(1,mean_fit.back());
			gaus->SetParameter(2,sigma_fit.back());
		}
		gaus->SetLineColor(kRed);
		hist->Fit(gaus,"QRMH");
		
		auto gausPol = new TF1(Form("f1_1_pr_%s_%i",pr1->GetName(),i),"gaus(0)+pol1(3)",minFitRange,maxFitRange);
		gausPol->SetParameter(0,gaus->GetParameter(0));
		gausPol->SetParameter(1,gaus->GetParameter(1));
		gausPol->SetParameter(2,gaus->GetParameter(2));
		hist->Fit(gausPol,"QRM");

		x.push_back((double)h2BinX);
		y.push_back(gaus->GetParameter(1));
		y_low.push_back( gaus->GetParameter(1) - nSigma * gaus->GetParameter(2) );
		y_high.push_back(gaus->GetParameter(1) + nSigma * gaus->GetParameter(2) );

		ampl_fit.push_back(gaus->GetParameter(0));
		mean_fit.push_back(gaus->GetParameter(1));
		sigma_fit.push_back(gaus->GetParameter(2));

		x_err.push_back(0.);
		y_err.push_back(0.);

		//std::cout<<mean.back()<<"\t"<<mean_err.back()<<std::endl;
		outFile->cd();
		hist->Write();
	}

	if(x.size()==0) return;

	TGraphErrors *gr_mean = new TGraphErrors(x.size(),&x[0],&y[0],&x_err[0],&y_err[0]);
	gr_mean->SetName(Form("gr_%s",pr1->GetName()));
	gr_mean->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_mean->SetLineColor(1);
	gr_mean->SetMarkerSize(1);
	gr_mean->SetMarkerStyle(8);
	gr_mean->SetMarkerColor(4);
	gr_mean->Fit(fit_mean,"QRM");

	TGraphErrors *gr_low = new TGraphErrors(x.size(),&x[0],&y_low[0],&x_err[0],&y_err[0]);
	gr_low->SetName(Form("gr_%s",pr1->GetName()));
	gr_low->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_low->SetLineColor(1);
	gr_low->SetMarkerSize(1);
	gr_low->SetMarkerStyle(8);
	gr_low->SetMarkerColor(4);
	gr_low->Fit(fit_low,"QRM");


	TGraphErrors *gr_high = new TGraphErrors(x.size(),&x[0],&y_high[0],&x_err[0],&y_err[0]);
	gr_high->SetName(Form("gr_%s",pr1->GetName()));
	gr_high->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_high->SetLineColor(1);
	gr_high->SetMarkerSize(1);
	gr_high->SetMarkerStyle(8);
	gr_high->SetMarkerColor(4);
	gr_high->Fit(fit_high,"QRM");


	TCanvas *can = new TCanvas(Form("StsDigitCut_%s",pr1->GetName()),"",960,560);
	can->cd();
	pr1->Draw("colz");
	can->SetLogz();
	gr_mean->Draw("Lsame");
	gr_low->Draw("Lsame");
	gr_high->Draw("Lsame");
	fit_mean->Draw("same");
	fit_low ->Draw("same");
	fit_high->Draw("same");

	std::cout<<fit_low->GetParameter(0)<<"\t\t"<<fit_low->GetParameter(1)<<"\t\t"<<fit_low->GetParameter(2)<<"\t\t"<<fit_low->GetParameter(3)<<"\n";
	std::cout<<fit_high->GetParameter(0)<<"\t\t"<<fit_high->GetParameter(1)<<"\t\t"<<fit_high->GetParameter(2)<<"\t\t"<<fit_high->GetParameter(3)<<"\n";

	gSystem->Exec(Form("mkdir -p ./%s/StsDigitCut",path_save.c_str()));
	can->SaveAs(Form("./%s/StsDigitCut/%s2.png",path_save.c_str(), pr1->GetName()));
	
	gr_mean->Write();
	gr_low->Write();
	gr_high->Write();
	can->Write();

}

void DigitNTracksFitNBD_X(TFile *inFile, DrawParameters obj_par, std::string path_save, TFile *outFile){

	TH1D *hist;

	std::vector<double> x;
	std::vector<double> x_err;
	std::vector<double> y;
	std::vector<double> y_err;
	std::vector<double> y_low;
	std::vector<double> y_high;
	std::vector<double> p0_fit;
	std::vector<double> p1_fit;
	std::vector<double> p2_fit;

	int StopFit = 200;
	double nSigma = 3.;
	double minFitRange = 10.;
	double maxFitRange = 6000.;
	
	TF1 *fit_mean = new TF1("fm","pol3(0)",0,500);
	TF1 *fit_low  = new TF1("fl","pol3(0)",21,500);
	TF1 *fit_high = new TF1("fh","pol3(0)",0,500);

	fit_mean -> SetLineColor(kRed);
	fit_low  -> SetLineColor(kRed);
	fit_high -> SetLineColor(kRed);

	TH2D *pr1 = GetTH2(inFile,obj_par);
	if(pr1==nullptr){
		std::cout<<"Error!"<<std::endl;
		return;
	}
		
	for(int i=0; i<(int)(StopFit); i++){
		int h2BinX = pr1->GetXaxis()->GetBinCenter(i);
		if(h2BinX<1)continue;
		hist = (TH1D*)pr1->ProjectionY(Form("nTrack_%s_%i",pr1->GetName(),h2BinX),pr1->GetXaxis()->FindBin(h2BinX),pr1->GetXaxis()->FindBin(h2BinX));
		if(hist->GetEntries()==0) continue;
		if(hist->Integral()==0) continue;
		
		hist->SetLineColor(kBlue);
		hist->SetTitle(Form("nTracks=%i",h2BinX));

		TF1 *fit1 = new TF1(Form("f1_pr_nbd_%s_%i",pr1->GetName(),i),"[0]*ROOT::Math::negative_binomial_pdf(x,[1],[2])", minFitRange,maxFitRange);
		if(p0_fit.size()>1){
			fit1->SetParameters(p0_fit.back(),p1_fit.back(),p2_fit.back());
		}else{
			fit1->SetParameters(100000,0.02,20);
		}
		fit1->SetLineColor(kRed);
		hist->Fit(fit1,"QRM");
		hist->Fit(fit1,"QRM");

		double p = fit1->GetParameter(1);
		double r = fit1->GetParameter(2);

		double meanNBD = r*(1.-p) / p;
		double sigmaNBD = sqrt( r*(1.-p) / pow(p,2) );

		x.push_back((double)h2BinX);
		y.push_back(meanNBD);
		y_low.push_back( meanNBD - nSigma * sigmaNBD );
		y_high.push_back(meanNBD + nSigma * sigmaNBD );

		x_err.push_back(0.);
		y_err.push_back(0.);

		p0_fit.push_back(fit1->GetParameter(0));
		p1_fit.push_back(fit1->GetParameter(1));
		p2_fit.push_back(fit1->GetParameter(2));

		//std::cout<<mean.back()<<"\t"<<mean_err.back()<<std::endl;
		outFile->cd();
		hist->Write();
	}

	if(x.size()==0) return;

	TGraphErrors *gr_mean = new TGraphErrors(x.size(),&x[0],&y[0],&x_err[0],&y_err[0]);
	gr_mean->SetName(Form("gr_%s",pr1->GetName()));
	gr_mean->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_mean->SetLineColor(1);
	gr_mean->SetMarkerSize(1);
	gr_mean->SetMarkerStyle(8);
	gr_mean->SetMarkerColor(4);
	gr_mean->Fit(fit_mean,"QRM");

	TGraphErrors *gr_low = new TGraphErrors(x.size(),&x[0],&y_low[0],&x_err[0],&y_err[0]);
	gr_low->SetName(Form("gr_%s",pr1->GetName()));
	gr_low->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_low->SetLineColor(1);
	gr_low->SetMarkerSize(1);
	gr_low->SetMarkerStyle(8);
	gr_low->SetMarkerColor(4);
	gr_low->Fit(fit_low,"QRM");


	TGraphErrors *gr_high = new TGraphErrors(x.size(),&x[0],&y_high[0],&x_err[0],&y_err[0]);
	gr_high->SetName(Form("gr_%s",pr1->GetName()));
	gr_high->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_high->SetLineColor(1);
	gr_high->SetMarkerSize(1);
	gr_high->SetMarkerStyle(8);
	gr_high->SetMarkerColor(4);
	gr_high->Fit(fit_high,"QRM");


	TCanvas *can = new TCanvas(Form("StsDigitCut_%s",pr1->GetName()),"",960,560);
	can->cd();
	pr1->Draw("colz");
	can->SetLogz();
	gr_mean->Draw("Lsame");
	gr_low->Draw("Lsame");
	gr_high->Draw("Lsame");
	fit_mean->Draw("same");
	fit_low ->Draw("same");
	fit_high->Draw("same");

	std::cout<<fit_low->GetParameter(0)<<"\t\t"<<fit_low->GetParameter(1)<<"\t\t"<<fit_low->GetParameter(2)<<"\t\t"<<fit_low->GetParameter(3)<<"\n";
	std::cout<<fit_high->GetParameter(0)<<"\t\t"<<fit_high->GetParameter(1)<<"\t\t"<<fit_high->GetParameter(2)<<"\t\t"<<fit_high->GetParameter(3)<<"\n";

	gSystem->Exec(Form("mkdir -p ./%s/StsDigitCut",path_save.c_str()));
	can->SaveAs(Form("./%s/StsDigitCut/%s2_nbd_X.png",path_save.c_str(), pr1->GetName()));
	
	gr_mean->Write();
	gr_low->Write();
	gr_high->Write();
	can->Write();

}

void DigitNTracksFitNBD_Y(TFile *inFile, DrawParameters obj_par, std::string path_save, TFile *outFile){

	TH1D *hist;

	std::vector<double> x;
	std::vector<double> x_err;
	std::vector<double> y;
	std::vector<double> y_err;
	std::vector<double> x_low;
	std::vector<double> x_high;

	std::vector<double> p0_fit;
	std::vector<double> p1_fit;
	std::vector<double> p2_fit;

	int StopFit = 230;
	double nSigma = 3.;
	double minFitRange = 0.;
	double maxFitRange = 400.;
	
	TF1 *fit_mean = new TF1("fm","pol3(0)",0,500);
	TF1 *fit_low  = new TF1("fl","pol3(0)",0,500);
	TF1 *fit_high = new TF1("fh","pol3(0)",0,500);

	fit_mean -> SetLineColor(kRed);
	fit_low  -> SetLineColor(kRed);
	fit_high -> SetLineColor(kRed);

	TH2D *pr1 = GetTH2(inFile,obj_par);
	if(pr1==nullptr){
		std::cout<<"Error!"<<std::endl;
		return;
	}
		
	for(int i=5; i<(int)(StopFit); i++){
		int h2BinX = pr1->GetYaxis()->GetBinCenter(i);
		if(h2BinX<1)continue;
		hist = (TH1D*)pr1->ProjectionX(Form("nTrack_%s_%i",pr1->GetName(),h2BinX),pr1->GetYaxis()->FindBin(h2BinX),pr1->GetYaxis()->FindBin(h2BinX));
		if(hist->GetEntries()==0) continue;
		if(hist->Integral()==0) continue;
		
		hist->SetLineColor(kBlue);
		hist->SetTitle(Form("nTracks=%i",h2BinX));

		TF1 *fit1 = new TF1(Form("f1_pr_nbd_%s_%i",pr1->GetName(),i),"[0]*ROOT::Math::negative_binomial_pdf(x,[1],[2])", minFitRange,maxFitRange);
		if(p0_fit.size()>1){
			fit1->SetParameters(p0_fit.back(),p1_fit.back(),p2_fit.back());
		}else{
			fit1->SetParameters(100000,0.15,10);
		}
		fit1->SetLineColor(kRed);
		hist->Fit(fit1,"QRM");
		hist->Fit(fit1,"QRM");

		double p = fit1->GetParameter(1);
		double r = fit1->GetParameter(2);

		double meanNBD = r*(1.-p) / p;
		double sigmaNBD = sqrt( r*(1.-p) / pow(p,2) );

		x.push_back(meanNBD);
		y.push_back((double)h2BinX);
		x_low.push_back( meanNBD - nSigma * sigmaNBD );
		x_high.push_back(meanNBD + nSigma * sigmaNBD );

		p0_fit.push_back(fit1->GetParameter(0));
		p1_fit.push_back(fit1->GetParameter(1));
		p2_fit.push_back(fit1->GetParameter(2));

		x_err.push_back(0.);
		y_err.push_back(0.);

		//std::cout<<mean.back()<<"\t"<<mean_err.back()<<std::endl;
		outFile->cd();
		hist->Write();
	}

	if(x.size()==0) return;

	TGraphErrors *gr_mean = new TGraphErrors(x.size(),&x[0],&y[0],&x_err[0],&y_err[0]);
	gr_mean->SetName(Form("gr_%s",pr1->GetName()));
	gr_mean->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_mean->SetLineColor(1);
	gr_mean->SetMarkerSize(1);
	gr_mean->SetMarkerStyle(8);
	gr_mean->SetMarkerColor(4);
	//gr_mean->Fit(fit_mean,"QRM");

	TGraphErrors *gr_low = new TGraphErrors(x.size(),&x_low[0],&y[0],&x_err[0],&y_err[0]);
	gr_low->SetName(Form("gr_%s",pr1->GetName()));
	gr_low->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_low->SetLineColor(1);
	gr_low->SetMarkerSize(1);
	gr_low->SetMarkerStyle(8);
	gr_low->SetMarkerColor(4);
	//gr_low->Fit(fit_low,"QRM");


	TGraphErrors *gr_high = new TGraphErrors(x.size(),&x_high[0],&y[0],&x_err[0],&y_err[0]);
	gr_high->SetName(Form("gr_%s",pr1->GetName()));
	gr_high->GetYaxis()->SetTitle(Form("Mean %s",pr1->GetYaxis()->GetTitle()));
	gr_high->SetLineColor(1);
	gr_high->SetMarkerSize(1);
	gr_high->SetMarkerStyle(8);
	gr_high->SetMarkerColor(4);
	//gr_high->Fit(fit_high,"QRM");


	TCanvas *can = new TCanvas(Form("StsDigitCut_%s",pr1->GetName()),"",960,560);
	can->cd();
	pr1->Draw("colz");
	can->SetLogz();
	gr_mean->Draw("Lsame");
	gr_low->Draw("Lsame");
	gr_high->Draw("Lsame");
	fit_mean->Draw("same");
	fit_low ->Draw("same");
	fit_high->Draw("same");

	std::cout<<fit_low->GetParameter(0)<<"\t\t"<<fit_low->GetParameter(1)<<"\t\t"<<fit_low->GetParameter(2)<<"\t\t"<<fit_low->GetParameter(3)<<"\n";
	std::cout<<fit_high->GetParameter(0)<<"\t\t"<<fit_high->GetParameter(1)<<"\t\t"<<fit_high->GetParameter(2)<<"\t\t"<<fit_high->GetParameter(3)<<"\n";

	gSystem->Exec(Form("mkdir -p ./%s/StsDigitCut",path_save.c_str()));
	can->SaveAs(Form("./%s/StsDigitCut/%s2_nbd_Y.png",path_save.c_str(), pr1->GetName()));
	
	gr_mean->Write();
	gr_low->Write();
	gr_high->Write();
	can->Write();

}


void Get_GraphiCuts_cuts(string iFile = "/home/aleksandr/BM@N/qa_root/qa_run8_vf_24.12_plp.root", string oFile = "./OUT_24.04_GraphCuts.root", string iSuf = "nVtxTr_"){	
	
	makeplotstyle();
	TFile *fstyle = new TFile("./style.root");
	TStyle *tsty = (TStyle *)fstyle->Get("PlottingInStyle");
	//tsty->cd();
 	gROOT->SetStyle("Pub");
	gROOT->ForceStyle();
	std::string path_to_project="/home/aleksandr/BM@N";
	std::string path = path_to_project + "/Final_codes_bmn/pict/QA_RunByRun_24.12_3gev";
	gSystem->Exec(Form("mkdir -p %s",path.c_str()));

	//qa_run8_vf_24.04_8005.root
	TFile *file_inFile 		= new TFile(iFile.c_str(),"READ");
	TFile *file_outFile 	= new TFile(oFile.c_str(),"RECREATE");	

	// RunByRun
	std::vector<DrawParameters> DrawObjectParH2;
	//DrawObjectParH2.push_back({"h2_nTracks_stsDigits","cut4_",0,4,2.,{0,500},{0,6000}});
	DrawObjectParH2.push_back({"h2_nTracks_gemDigits",iSuf,0,4,2.,{0,500},{0,15000}});

	//DigitNTracksFitGaus(file_inFile, DrawObjectParH2[0], path, file_outFile);
	//DigitNTracksFitNBD_X(file_inFile, DrawObjectParH2[0], path, file_outFile);
	GemDigitNTracksFitGamma_X_new(file_inFile, DrawObjectParH2[0], path, file_outFile);
	//DigitNTracksFitNBD_Y(file_inFile, DrawObjectParH2[0], path, file_outFile);

	file_outFile->Close();
	file_inFile->Close();

}