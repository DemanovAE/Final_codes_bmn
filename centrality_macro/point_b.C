
void makeplotstyle(){
  
    TStyle *mystyle = new TStyle("PlottingInStyle", "Style for Summary Plots");
    mystyle->SetLineWidth(2);
    mystyle->SetPalette(1);
    mystyle->SetCanvasColor(10);
    mystyle->SetHistFillColor(10);
    mystyle->SetHistFillStyle(0);
    mystyle->SetOptTitle(0);
    mystyle->SetOptStat(0);
    mystyle->SetCanvasBorderMode(0);//removes the yellow frame around the canvas
    mystyle->SetPadLeftMargin(0.16);
    mystyle->SetPadBottomMargin(0.15);
    mystyle->SetPadTickX(1);
    mystyle->SetPadTickY(1);
    mystyle->SetAxisColor(1, "X");
    mystyle->SetAxisColor(1, "Y");
    mystyle->SetLabelColor(1, "X");
    mystyle->SetLabelColor(1, "Y");
    mystyle->SetTickLength(0.03, "X");
    mystyle->SetTickLength(0.03, "Y");
    mystyle->SetTitleXSize(0.05);
    mystyle->SetTitleYSize(0.05);
    mystyle->SetNdivisions(508, "X");
    mystyle->SetNdivisions(508, "Y");
    mystyle->SetTitleXOffset(1.2);
    mystyle->SetTitleYOffset(1.4);
    mystyle->SetLabelOffset(0.02, "X");
    mystyle->SetLabelOffset(0.02, "Y");
    mystyle->SetLabelSize(0.05, "X");
    mystyle->SetLabelSize(0.05, "Y");
    mystyle->SetEndErrorSize(4);
    
    //mystyle->SetGridx();

    TFile f("style.root", "RECREATE");
    f.cd();
    mystyle->Write();
    f.Close();
}



void point_b(){	
	
	//makeplotstyle();
	//TFile *fstyle = new TFile("./style.root");
	//TStyle *tsty = (TStyle *)fstyle->Get("PlottingInStyle");
	//tsty->cd();
 	gROOT->SetStyle("Pub");
	gROOT->ForceStyle();

	TCanvas *can = new TCanvas("","",1.4*960, 960);
	can->cd();

	TH2D *h_axis = new TH2D("",";Centrality, %;<b>#pm#sigma, fm",2,0,60,2,0,15);
	h_axis->Draw();

	//std::vector<double> Cent ={ 2.5,7.5,12.5,17.5,22.5,27.5,32.5,37.5,45,55};
	std::vector<double> Cent ={ 2.5,7.5,12.5,17.5,22.5,27.5,32.5,37.5,45,55};
	//std::vector<double> Cent ={ 5.0,15.,25,35,45,55};
	std::vector<double> CentErr;
	std::vector<double> CentMCGlauber = { 2.5, 7.5, 12.5, 17.5, 22.5, 27.5, 32.5, 37.5, 45, 55};
	//std::vector<double> MeanBMCGlauber ={ 2.77229, 4.54442, 5.93516, 7.04877, 8.03017, 8.92563, 9.75306, 10.5487, 11.3714};
	//std::vector<double> SigmaBMCGlauber ={ 1.22459, 1.06661, 0.919943, 0.865039, 0.872482, 0.90496, 0.980867, 1.06574, 1.15058};

std::vector<double> MeanBMCGlauber = { 2.26772, 3.25868, 4.17427, 4.99036, 5.67499, 6.29528, 6.8596, 7.36444, 8.03017, 8.92563, 9.65231, 10.0882, 10.513, 10.9304, 11.4766, 12.3227};
std::vector<double> SigmaBMCGlauber = { 1.11556, 1.11884, 1.03373, 0.936566, 0.874212, 0.845486, 0.825767, 0.823007, 0.872482, 0.90496, 0.942299, 0.993619, 1.03896, 1.08809, 1.14151, 1.21509};


	for(int i =0; i<CentMCGlauber.size(); i++)
	{
		CentErr.push_back(0.);
		CentMCGlauber[i] = Cent[i] /*- 0.3*/;
		SigmaBMCGlauber[i] = abs(SigmaBMCGlauber[i]);// - MeanBMCGlauber[i]);
		//std::cout<<SigmaBMCGlauber[i]<<std::endl;
	}
	TGraphErrors *grMC = new TGraphErrors(CentMCGlauber.size(),&CentMCGlauber[0],&MeanBMCGlauber[0],&CentErr[0],&SigmaBMCGlauber[0]);
	grMC->SetLineColor(kBlue);
	grMC->SetLineWidth(3);
	grMC->SetMarkerSize(2);
	grMC->SetMarkerColor(kBlue);
	grMC->SetMarkerStyle(22);
	grMC->Draw("Psame");

/*
	std::vector<double> CentGFit ={ 2,7,11,17,22,26,32,36,45,54};
	std::vector<double> MeanBGFit ={2.64483627,3.97481108,5.12342569,6.07052896,6.89672544,7.64231738,8.34760705,8.93198992,9.73803526,10.8060453};
	std::vector<double> SigmaBGFit = {3.69269521,5.06297229,6.09068010,6.95717884,7.74307304,8.46851385,9.07304785,9.69773299,10.6045340,11.6322418};
	//{3.91435768,5.28463476,6.31234256,7.17884130,7.98488664,8.69017632,9.33501259,9.95969773,10.7858942,11.7934508};
	

	for(int i =0; i<MeanBGFit.size(); i++)
	{
		CentErr.push_back(0.);
		CentGFit[i] = Cent[i];
		SigmaBGFit[i] = abs(SigmaBGFit[i] - MeanBGFit[i]);
		//std::cout<<SigmaBGFit[i]<<std::endl;
	}
	TGraphErrors *grGFit = new TGraphErrors(CentGFit.size(),&CentGFit[0],&MeanBGFit[0],&CentErr[0],&SigmaBGFit[0]);
	grGFit->SetLineColor(kRed);
	grGFit->SetLineWidth(3);
	grGFit->SetMarkerSize(2);
	grGFit->SetMarkerColor(kRed);
	grGFit->SetMarkerStyle(22);
	//grGFit->Draw("Psame");
*/
	auto _legend = new TLegend(0.2,0.75,0.5,0.85);
   _legend->AddEntry(grMC,"#font[42]{ #scale[1.0]{MC-Glauber}}","pl");
   //_legend->AddEntry(grGFit,"#font[42]{ #scale[1.0]{#Gamma-fit}}","pl");
   _legend->Draw("same");

	auto _legend2 = new TLegend(0.45,0.80,0.85,0.85);
   _legend2->SetHeader("#font[42]{ #scale[0.8]{ #splitline{BM@N run 8}{Xe-Cs(I) @ 3.8A GeV} }}","C");
   _legend2->Draw("same");

   can->SaveAs("./impactPar_2.png");


}