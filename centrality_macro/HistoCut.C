#include <string>
#include <iostream>
#include <vector>

void HistoCut(Int_t CentralityClasses, string InFile, string OutFile, string InputHistoName){
	TFile *f1 = new TFile(InFile.c_str());  //glauber_qa.root
	TFile *f2 = new TFile(InFile.c_str());
	TH1F *FitHisto=(TH1F*)f1->Get("glaub_sng_histo");
	TH1F *FitHistoPlp=(TH1F*)f1->Get("glaub_plp_histo");
	TH1F *FitHistoAll=(TH1F*)f1->Get("glaub_fit_histo");
	TH1F *DataHisto=(TH1F*)f2->Get(InputHistoName.c_str());

	if(FitHisto==nullptr){
		std::cout<<"Errors! Not found histo"<<std::endl;
		return;
	}
	if(FitHistoPlp==nullptr){
		std::cout<<"Errors! Not found histo"<<std::endl;
		return;
	}
	if(FitHistoAll==nullptr){
		std::cout<<"Errors! Not found histo"<<std::endl;
		return;
	}
	if(DataHisto==nullptr){
		std::cout<<"Errors! Not found histo"<<std::endl;
		return;
	}

	Int_t bins = FitHisto->GetNbinsX();
	cout<<"bins="<<bins<<endl;
	Double_t max = FitHisto->GetXaxis()->GetBinCenter(bins);
	cout<<"max="<<max<<endl;
	Double_t min = FitHisto->GetYaxis()->GetXmin();
	cout<<"min="<<min<<endl;
	Double_t integral = FitHisto->Integral(2,bins);
	cout<<"integral="<<integral<<endl;

	TFile *f = new TFile(OutFile.c_str(), "recreate"); // HistoCutResult.root

	TH1F* ResultHisto[CentralityClasses];
	for (int i = 0; i < CentralityClasses; i++)   ResultHisto[i] = new TH1F(Form("CentralityClass_Fit %.1f%%-%.1f%%", i*100.0/CentralityClasses, (i+1)*100.0/CentralityClasses),";tracks;counts",bins, min, max);
	Int_t j=1;
	for (int i = 0; i < CentralityClasses; i++) {
		ResultHisto[i] -> SetLineColor(j);
		j++;
		if (j==9) j=1;
		}
	for (int i = 0; i < CentralityClasses; i++) ResultHisto[i] -> SetLineWidth(4);

	TH1F* CentralityHisto[CentralityClasses];
	for (int i = 0; i < CentralityClasses; i++)   CentralityHisto[i] = new TH1F(Form("CentralityClass %.1f%%-%.1f%%", i*100.0/CentralityClasses, (i+1)*100.0/CentralityClasses), ";tracks;counts", bins, min, max);
	j=1; 
	for (int i = 0; i < CentralityClasses; i++) {
		CentralityHisto[i] -> SetLineColor(j);
		j++;
		if (j==9) j=1;
		}
	for (int i = 0; i < CentralityClasses; i++) CentralityHisto[i] -> SetLineWidth(4);

	TH1F* Centrality_vs_Multiplisity = new TH1F("Centrality_vs_Multiplisity", ";tracks;CentralityPercent",bins, min, max);
	Centrality_vs_Multiplisity -> SetLineColor(1);
	Centrality_vs_Multiplisity -> SetLineWidth(4);
	
	j=0;
	Double_t sum=0.0;
	for (Int_t i=bins;i>=2;i--) {
        	sum = sum+FitHisto->GetBinContent(i);
		// cout<<sum<<endl;
		// cout<<j<<endl;
		// Cut pile-up dominant part
		bool is_pileup = false;
		for (int iplp=i; iplp>=2;iplp--){
			if (FitHisto->GetBinContent(iplp) < FitHistoPlp->GetBinContent(iplp))
				is_pileup = true;
		}
		if (is_pileup)
			continue;
		if (sum < (integral/CentralityClasses) && j<CentralityClasses) 
			ResultHisto[j] -> SetBinContent(i, FitHistoAll->GetBinContent(i));
		else {
			if (j<CentralityClasses-1) {j++; sum=0.0;}
			if (j<CentralityClasses-1) ResultHisto[j] -> SetBinContent(i, FitHistoAll->GetBinContent(i));
		}
	}

	j=CentralityClasses-1;
	for (Int_t i=2;i<=bins;i++) {
        	if ((ResultHisto[j]->GetBinContent(i))>0) {
			CentralityHisto[j] -> SetBinContent(i, DataHisto->GetBinContent(i));
			Centrality_vs_Multiplisity -> SetBinContent(i, (j+1)*100/CentralityClasses);
			}
		else {
			if (j>0) j--;
			if (j>0) {
				CentralityHisto[j]         -> SetBinContent(i, DataHisto->GetBinContent(i));
				Centrality_vs_Multiplisity -> SetBinContent(i, (j+1)*100/CentralityClasses);
				}
			}
   		}
	
	TTree *Borders=new TTree("Borders", "Borders");
	Int_t Ncc, BIN, MinBorder, MaxBorder;
	Float_t MinPercent, MaxPercent;
	Borders -> Branch("Ncc", &Ncc);
	Borders -> Branch("MinPercent", &MinPercent);
	Borders -> Branch("MaxPercent", &MaxPercent);
	Borders -> Branch("MinBorder", &MinBorder);
	Borders -> Branch("MaxBorder", &MaxBorder);

	j=0;
	for (Int_t i=1; i<=CentralityClasses; i++) {
		// cout<<i<<endl;
		Ncc=i;
		Int_t l=2;
		while ((ResultHisto[j]->GetBinContent(l))<=0 && l<bins) l++;
		MinPercent=(j)*100/CentralityClasses; 
		MinBorder=ResultHisto[j]->GetBinCenter(l); 
		BIN=l;
		for (Int_t q=BIN;q<=bins;q++) {
			if ((ResultHisto[j]->GetBinContent(q))<=0) {MaxPercent=(j+1)*100/CentralityClasses; MaxBorder=ResultHisto[j]->GetBinCenter(q); break;};
			if (q==bins) {MaxPercent=(j+1)*100/CentralityClasses; MaxBorder=ResultHisto[j]->GetBinCenter(q);};
		}
		Borders -> Fill();
		j++;
	}

	TCanvas* c1 = new TCanvas("HistoCutResult","HistoCutResult");
	TLegend legend1(0.1, 0.2, 0.3, 0.4, "HistoCutResult");
	for (int i = 0; i < CentralityClasses; i++) legend1.AddEntry(ResultHisto[i], Form("CentralityClass %.1f%%-%.1f%%", i*100.0/CentralityClasses, (i+1)*100.0/CentralityClasses));
	THStack *ResultHistos = new THStack("HistoCutResult","HistoCutResult");
	for (int i = 0; i < CentralityClasses; i++) ResultHistos -> Add(ResultHisto[i]);
	ResultHistos->Add(FitHistoAll);
	ResultHistos -> Draw("nostack");
	legend1.Draw("same");
	c1->Write();
	for (int i = 0; i < CentralityClasses; i++) ResultHisto[i] -> Write();

	TCanvas* c2 = new TCanvas("CentraliryClasses","CentralityClasses");
	TLegend legend2(0.1, 0.2, 0.3, 0.4, "CentraliryClasses");
	for (int i = 0; i < CentralityClasses; i++) legend2.AddEntry(CentralityHisto[i], Form("CentralityClass %.1f%%-%.1f%%", i*100.0/CentralityClasses, (i+1)*100.0/CentralityClasses));
	THStack *CentralityHistos = new THStack("HistoCutResult","HistoCutResult");
	for (int i = 0; i < CentralityClasses; i++) CentralityHistos -> Add(CentralityHisto[i]);
	CentralityHistos->Add(DataHisto);
	CentralityHistos -> Draw("nostack");
	legend2.Draw("same");
	c2->Write();
	for (int i = 0; i < CentralityClasses; i++) CentralityHisto[i] -> Write();
	Centrality_vs_Multiplisity -> Write();

	Borders -> Write();
	

	f->Close();
}
