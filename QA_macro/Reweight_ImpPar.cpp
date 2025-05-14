#include "/home/aleksandr/BM@N/QA_bmn/QA_function.C"
const int _minRunId = 8120;
const int _maxRunId = 8170;

template<typename T>
TH1D* GetWeightTH1(T& th1_gmc, TH1D *th1_data){
	
	if(th1_gmc==nullptr){
		std::cout<<"th1_gmc - not found!"<<std::endl;
		return nullptr;
	};
	if(th1_data==nullptr){
		std::cout<<"th1_data - not found!"<<std::endl;
		return nullptr;
	};

	int n_x = th1_gmc->GetNbinsX();
	TH1D *hh_weight = new TH1D("h_b_weight","",200,0,20);

	for(int i=0; i<=n_x; i++){
		
		double x 	  = th1_gmc->GetBinCenter(i);
		double y_gmc  = th1_gmc->GetBinContent(th1_gmc->FindBin(x));
		double y_data = th1_data->GetBinContent(th1_data->FindBin(x));

		if(y_gmc==0 || y_data==0){
			hh_weight->SetBinContent(hh_weight->FindBin(x),0.);
			continue;
		}
		hh_weight->SetBinContent(hh_weight->FindBin(x), y_data / y_gmc);
	}

	return hh_weight;
}

void Reweight_ImpPar(std::string inDataFileName, std::string inGmsFileName = "/home/aleksandr/BM@N/qa_root/gmc_CsXe_27.7mb_40M.root", std::string out_file_name="sim_b_gmc_b.root"){

 	gROOT->SetStyle("Pub");
	gROOT->ForceStyle();
    gROOT->ForceStyle(0);
    gStyle->SetOptStat(0);
    gStyle->SetEndErrorSize(5);

    string NameTree = "nt_Cs_Xe";

	TFile *file_inFile 	= new TFile(inDataFileName.c_str(),"READ");	
	TFile *file_inGms 	= new TFile(inGmsFileName.c_str(),"READ");	

	ROOT::RDataFrame dd( NameTree.c_str(),file_inGms );

	std::vector<ROOT::RDF::RResultPtr<::TH1D>> histo1d;
	
	histo1d.push_back( dd.Histo1D({"h1_b",";b, fm", 200, 0, 20}, "B") );
	
	histo1d[0]->Scale(1./histo1d[0]->Integral(0,11,"width"));
	histo1d[0]->SetName("gms_B_norm");
	histo1d[0]->SetLineColor(kBlue);
	histo1d[0]->SetLineWidth(2.);


	TH1D *hist_data = (TH1D*)file_inFile->Get("def_h1_b");
	hist_data->Scale(1/hist_data->Integral(0,11,"width"));
	hist_data->SetName("data_B_norm");
	hist_data->SetLineWidth(2.);
	hist_data->SetLineColor(kBlack);

	TH1D *h_reweight_B = GetWeightTH1(histo1d[0],hist_data);

	TCanvas *can = new TCanvas("c1","",800,600);
	h_reweight_B->GetYaxis()->SetTitle("Weight");
	h_reweight_B->GetXaxis()->SetTitle("b, fm");
	h_reweight_B->SetLineWidth(2.);
	h_reweight_B->Draw();
	can->SaveAs("../pict/weight_B.png");

	TRandom3 *tR = new TRandom3();
	auto weight_b = [&h_reweight_B,&tR]( Float_t fB ){
		double P = h_reweight_B->GetBinContent(h_reweight_B->FindBin(fB));
        P = TMath::Min(P, 1.);
        double rP = 0;
        rP = tR->Rndm(); 
		return rP<P ? true : false;
	};

	auto d_reweight = dd.Filter(weight_b,{"B"});
	d_reweight.Snapshot(NameTree, out_file_name);
	
	histo1d.push_back( d_reweight.Histo1D({"h1_b_reweight",";b, fm", 200, 0, 20}, "B") );	
	histo1d[1]->Scale(1./histo1d[1]->Integral(0,11,"width"));
	histo1d[1]->SetName("gms_B_reweight");
	histo1d[1]->SetLineColor(kRed);

	TCanvas *can2 = new TCanvas("c2","",800,600);
	hist_data->GetYaxis()->SetTitle("dN/N");
	hist_data->Draw("histo");
	histo1d[0]->Draw("samehisto");
	histo1d[1]->Draw("samehisto");

    TLegend *legend = new TLegend(0.2,0.7,0.44,0.89);
    legend->AddEntry(hist_data, 		"#font[42]{ #scale[1.0]{DCM-QGSM-SMM}}","l");
    legend->AddEntry("gms_B_norm",		"#font[42]{ #scale[1.0]{MC-Gl}}", "l");
    legend->AddEntry("gms_B_reweight", 	"#font[42]{ #scale[1.0]{MC-Gl, reweight}}", "l");
    legend->Draw("same");

	can2->SaveAs("../pict/h1_b_reweight.png");

}