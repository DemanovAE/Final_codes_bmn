// C++ headers
#include <string>
#include <vector>
#include <iostream>
#include <fstream> 
#include <map>

// ROOT headers
#include "TROOT.h"
#include "TFile.h"

// отрисовка отношения дата/фит
TGraphErrors *RatioDataFit(TH1F *h_data,TH1F *h_fit);

// делает все гистограммы с одинаковым бинингом и делает ребин.
TH1F *GetRebinHisto(TH1F *h_data, int n_rebin);

// возвращает вектор параметров лучшего фита
std::vector<float> GetParBestFit(TFile *in_file);

//root -l -q -b DrawFinalPict.cpp'("/home/aleksandr/BM@N/centrality_result/glauber_qa_2_raw/glauber_qa_2_raw.root","/home/aleksandr/BM@N/centrality_result/glauber_qa_2_raw/glauber_qa_2_raw_Final.root","refMultRun8_VtxR1_VtxZ010_7310_7500_scale")'
//void DrawBvsCent(const string InFile = "/home/aleksandr/BM@N/qa_root/qa_XeCs_dcm_3.8AGeV_b.root",const string InFileFit = "/home/aleksandr/BM@N/cent_root_file/glauber_qa_dcm_3.8_50_300_VtxNTr.root"){
void DrawBvsCent(const string InFile = "/home/aleksandr/BM@N/qa_root/qa_XeCs_dcm_3.8AGeV_b.root",const string InFileFit = "/home/aleksandr/BM@N/cent_root_file/glauber_qa_dcm_3.8_50_300_VtxNTr.root"){
	
    gROOT->SetStyle("Pub");
    gROOT->ForceStyle();
    
    std::vector<double> minCentPercent = { 0, 10, 20, 30, 40, 50, 60, 70, 80};
    std::vector<double> maxCentPercent = { 10, 20, 30, 40, 50, 60, 70, 80, 90};

    std::vector<double> meanB = { 2.72018, 4.74239, 6.16846, 7.32955, 8.35153, 9.28182, 10.1253, 10.9678, 11.7163, 12.6337};
    std::vector<double> rmsB  = { 1.07271, 0.813646, 0.693179, 0.666843, 0.673469, 0.714771, 0.786562, 0.922861, 1.01431, 1.11355};

    std::vector<double> minMult = { 154, 106, 72, 47, 29, 17, 10, 5, 2};
    std::vector<double> maxMult = { 299, 154, 106, 72, 47, 29, 17, 10, 5};

    std::vector<double> meanB_sim;
    std::vector<double> rmsB_sim;
    std::vector<double> cent_sim;
    std::vector<double> centErr_sim;

    std::vector<double> meanB_fit;
    std::vector<double> rmsB_fit;
    std::vector<double> cent_fit;
    std::vector<double> centErr_fit;

    std::vector<double> meanB_ratio;
    std::vector<double> rmsB_ratio;

    TFile *qa_file = new TFile(InFile.c_str(),"READ");// step 1        
    TFile *fit_file = new TFile(InFileFit.c_str(),"READ");// step 1        
    TFile *out_file = new TFile("./test_b.root","recreate");// step 1        
    
    //TH2D *h2_BvsMult = (TH2D*)qa_file->Get("Vtx_VtxZ15_h2_nTracks_b");
    TH2D *h2_BvsMult = (TH2D*)qa_file->Get("def_h2_nTracks_b");
    TH2D *h2_BvsMult_Fit = (TH2D*)fit_file->Get("B_VS_Multiplicity");

    TH1D* h1_b[9];
    TH1D* h1_b_fit[9];

    TCanvas *can = new TCanvas("c1","",3*300,3*300);
    can->Divide(9);

    for(int ic=0; ic<9; ic++){
        
        h1_b[ic]     = h2_BvsMult    ->ProjectionY(Form("cent_%i_%i",(int)minCentPercent[ic],(int)maxCentPercent[ic]),h2_BvsMult       ->GetXaxis()->FindBin(minMult[ic]),h2_BvsMult    ->GetXaxis()->FindBin(maxMult[ic]));
        h1_b_fit[ic] = h2_BvsMult_Fit->ProjectionY(Form("centFit_%i_%i",(int)minCentPercent[ic],(int)maxCentPercent[ic]),h2_BvsMult_Fit->GetXaxis()->FindBin(minMult[ic]),h2_BvsMult_Fit->GetXaxis()->FindBin(maxMult[ic]));

        h1_b[ic]->SetLineColor(kBlue);

        h1_b[ic]->Scale(1./h1_b[ic]->Integral());
        h1_b_fit[ic]->Scale(1./h1_b_fit[ic]->Integral());

        // //
        meanB_sim.push_back(h1_b[ic]->GetMean());
        rmsB_sim.push_back( h1_b[ic]->GetRMS());
        cent_sim.push_back( (minCentPercent[ic]+maxCentPercent[ic])/2.);
        centErr_sim.push_back(0.);
    
        // //
        meanB_fit.push_back(h1_b_fit[ic]->GetMean());
        rmsB_fit.push_back( h1_b_fit[ic]->GetRMS());
        cent_fit.push_back( (minCentPercent[ic]+maxCentPercent[ic])/2. + 1);
        centErr_fit.push_back(0.);

        //
        meanB_ratio.push_back(meanB_sim.back()/meanB_fit.back()*1.005);
        //meanB_ratio.push_back(meanB_sim.back()/meanB[ic]);
        rmsB_ratio.push_back(0.0);

        out_file->cd();
        can->cd(ic);
        h1_b[ic]->Draw();
        h1_b_fit[ic]->SetLineColor(kRed);
        h1_b_fit[ic]->Draw("same");

        h1_b[ic]->Write();
        h1_b_fit[ic]->Write();

    }

    can->SaveAs("./test_cent.png");

    TGraphErrors *gr_b_fit = new TGraphErrors(cent_fit.size(),&cent_fit[0],&meanB_fit[0],&centErr_fit[0],&rmsB_fit[0]);
    //TGraphErrors *gr_b_fit = new TGraphErrors(cent_fit.size(),&cent_fit[0],&meanB[0],&centErr_fit[0],&rmsB[0]);
    gr_b_fit->SetLineColor(kRed);
    gr_b_fit->SetLineWidth(2);
    gr_b_fit->SetMarkerColor(kRed);
    gr_b_fit->SetMarkerSize(1.4);
    gr_b_fit->SetMarkerStyle(4);

    TGraphErrors *gr_b_sim = new TGraphErrors(cent_sim.size(),&cent_sim[0],&meanB_sim[0],&centErr_sim[0],&rmsB_sim[0]);
    gr_b_sim->SetLineColor(kBlue);
    gr_b_sim->SetLineWidth(2);
    gr_b_sim->SetMarkerColor(kBlue);
    gr_b_sim->SetMarkerSize(1.4);
    gr_b_sim->SetMarkerStyle(8);

    TGraphErrors *gr_ratio = new TGraphErrors(cent_sim.size(),&cent_sim[0],&meanB_ratio[0],&centErr_sim[0],&rmsB_ratio[0]);
    gr_ratio->SetLineColor(kBlue);
    gr_ratio->SetLineWidth(2);
    gr_ratio->SetMarkerColor(kBlue);
    gr_ratio->SetMarkerSize(1.6);
    gr_ratio->SetMarkerStyle(8);

    // Draw
    float AxisXmin = 0.;
    float AxisXmax = 90.;
    float AxisYmin = 0.;
    float AxisYmax = 14.9;
    float AxisRatioYmin = 0.86;
    float AxisRatioYmax = 1.14;
    
    double H_y = abs(AxisYmin - AxisYmax);

    TCanvas *canvas = new TCanvas("canvas","plot", 1.4*960, 960);
    canvas->cd();

    TPad *pad_up = new TPad("up","",0.0,0.45,1.0,1.0);
    pad_up->SetFrameBorderMode(1);
    pad_up->SetTopMargin(0.02);
    pad_up->SetBottomMargin(0);

    TPad *pad_down = new TPad("down","",0.0,0.0,1.0,0.45);
    pad_up->SetFrameBorderMode(1);
    pad_down->SetTopMargin(0);
    pad_down->SetBottomMargin(0.3);

    canvas->cd();
    pad_up->Draw();
    pad_down->Draw();

    pad_up->cd();

    TH2F *axis_up = new TH2F("axix_up","",2,AxisXmin,AxisXmax,4,AxisYmin,AxisYmax);
    axis_up->GetYaxis()->SetTitle("#font[42]{ #scale[1.0]{<b>#pm#sigma, fm}}");
    axis_up->GetYaxis()->SetTitleOffset(0.8);
    axis_up->GetYaxis()->SetTitleSize(0.08);
    axis_up->GetYaxis()->SetLabelSize(0.08);
    pad_up->cd();
    axis_up->Draw();

    TH2F *axis_down = new TH2F("axix_down","",2,AxisXmin,AxisXmax,4,AxisRatioYmin,AxisRatioYmax);
    axis_down->GetXaxis()->SetTitle("#font[42]{ #scale[1.0]{Centrality, %}}");
    axis_down->GetXaxis()->SetLabelSize(0.1);
    axis_down->GetYaxis()->SetLabelSize(0.1);
    axis_down->GetXaxis()->SetTitleOffset(1);
    axis_down->GetXaxis()->SetTitleSize(0.1);
    axis_down->GetYaxis()->SetTitle("#font[42]{ #scale[1.0]{Model/MC-Gl}}");
    axis_down->GetYaxis()->SetTitleOffset(0.6);
    axis_down->GetYaxis()->SetTitleSize(0.1);
    pad_down->cd();
    axis_down->Draw();

    pad_up->cd();

    gr_b_fit->Draw("P");
    gr_b_sim->Draw("Psame");


    //TLegend *legend = new TLegend(0.6,0.5,0.85,0.75);
    TLegend *legend = new TLegend(0.75,0.2,0.89,0.39);
    legend->SetNColumns(1);
    legend->AddEntry(gr_b_fit, "#font[42]{ #scale[1.0]{MC-Gl}}", "p");
    legend->AddEntry(gr_b_sim, "#font[42]{ #scale[1.0]{model}}", "p");
    //legend->AddEntry(FitHistoSng, "#font[42]{ #scale[1.0]{single}}", "p");
    //legend->AddEntry(FitHistoPlp, "#font[42]{ #scale[1.0]{pile-up}}", "p");
    legend->Draw("same");

    TLatex _textInfo;
    _textInfo.SetTextSize(0.07);
    _textInfo.DrawLatex(AxisXmin+0.1*(AxisXmax - AxisXmin), 0.9*H_y , 
        Form("#font[42]{ #scale[1.0]{Xe+Cs, 3.8 AGeV}}"));
    _textInfo.DrawLatex(AxisXmin+0.1*(AxisXmax - AxisXmin), 0.8*H_y , 
        Form("#font[42]{ #scale[1.0]{DCM-QGSM-SMM + GEANT4}}"));
    /*
    _textInfo.DrawLatex(AxisXmin+0.1*(AxisXmax - AxisXmin), 0.3*H_y , 
        Form("#font[42]{ #scale[1.0]{BM@N Run8, Xe+Cs(I), N_{a}=fN_{part}+(1-f)N_{coll}}}"));
    _textInfo.DrawLatex(AxisXmin+0.1*(AxisXmax - AxisXmin), 0.08*H_y , 
        Form("#font[42]{ #scale[1.0]{f=%.2f, k=%.2f, #mu=%.3f, p=%.2f%%, #chi^{2}/ndf=%.3f#pm%.3f}}",
        f_best,k_best,mu_best,p_best*100,chi2_best,chi2_error_best));
    */
    // down pad
    pad_down->cd();
    gr_ratio->Draw("Psame");

    TLine *line_1 = new TLine( AxisXmin, 1., AxisXmax, 1. );
    line_1->SetLineColor( kBlack );
    line_1->SetLineWidth( 2 );
    line_1->SetLineStyle( 1 );
    line_1->Draw("same");

    TLine *line_2 = new TLine( AxisXmin, 1.05, AxisXmax, 1.05 );
    line_2->SetLineColor( kBlack );
    line_2->SetLineWidth( 2 );
    line_2->SetLineStyle( 2 );
    line_2->Draw("same");

    TLine *line_3 = new TLine( AxisXmin, 0.95, AxisXmax, .95 );
    line_3->SetLineColor( kBlack );
    line_3->SetLineWidth( 2 );
    line_3->SetLineStyle( 2 );
    line_3->Draw("same");

    canvas->SaveAs(Form("./b_vs_cent_sim_gl.png"));

    ///////////////////////////////// Draw b vs cent //////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////



}

TGraphErrors *RatioDataFit(TH1F *h_data,TH1F *h_fit){

    std::vector<float> vec_x;
    std::vector<float> vec_y;
    std::vector<float> vec_xe;
    std::vector<float> vec_ye;

    if(h_data==nullptr){
        std::cout<<"empty histo to convert"<<std::endl;
        return nullptr;
    }

    if(h_fit==nullptr){
        std::cout<<"empty histo to convert"<<std::endl;
        return nullptr;
    }

    int _nbins  = h_data->GetXaxis()->GetNbins();
    if(_nbins==0) return nullptr;

    for(int i=1; i<_nbins; i++){
        float x = h_data->GetBinCenter(i);
        float y_data = h_data->GetBinContent(h_data->FindBin(x));
        float y_fit = h_fit->GetBinContent(h_fit->FindBin(x));
        float xe = 0.0;

        if(y_data==0. || y_fit==0.)continue;

        float ye_data = h_data->GetBinError(h_data->FindBin(x));
        float ye_fit = h_fit->GetBinError(h_fit->FindBin(x));
        float y_result = y_data / y_fit;
        float ye_result = ye_data / y_fit;
        //float ye_result = sqrt(pow(ye_data/y_fit,2) + pow(y_data*ye_fit/pow(y_fit,2),2));
        
        vec_x.push_back(x);
        vec_y.push_back(y_result);
        vec_xe.push_back(xe);
        vec_ye.push_back(ye_result);
    }

    TGraphErrors *result = new TGraphErrors(vec_x.size(),&vec_x[0],&vec_y[0],&vec_xe[0],&vec_ye[0]);
    result->SetName(Form("gr_%s",h_data->GetName()));
    result->SetTitle(h_data->GetTitle());

    return result;
}

TH1F *GetRebinHisto(TH1F *h_data, int n_rebin){

    if(h_data==nullptr){
        std::cout<<"empty histo to convert"<<std::endl;
        return nullptr;
    }

    int _nbins  = h_data->GetXaxis()->GetNbins();
    if(_nbins==0) return nullptr;

    TH1F *result = new TH1F(Form("%s_rebin",h_data->GetName()),"",1000,0,1000);

    for(int ip=1; ip<_nbins; ip++){
        auto _x = h_data->GetBinCenter(ip);
        auto _y = h_data->GetBinContent(ip);
        for(int iev=0; iev<_y; iev++){
            float fill_val = _x;
            result->Fill(fill_val);
        }
    }

    result->Rebin(n_rebin);

    return result;

}

std::vector<float> GetParBestFit(TFile *in_file){

    std::vector<float> result = {-999,-999,-999,-999,-999,-999};

    /// Tree best fit par
    TTree *bestResult =(TTree*)in_file->Get("BestResult");
    
    if(bestResult==nullptr){
        std::cout<<"Not found tree: BestResult"<<std::endl;
        return  result;
    }

    Float_t f, mu, k, p, chi2, chi2_error;
    
    bestResult->SetBranchAddress("f",&f);
    bestResult->SetBranchAddress("mu",&mu);
    bestResult->SetBranchAddress("k",&k);
    bestResult->SetBranchAddress("p",&p);
    bestResult->SetBranchAddress("chi2",&chi2);
    bestResult->SetBranchAddress("chi2_error",&chi2_error);
    
    //read all entries and fill the histograms
    Int_t nentries = (Int_t)bestResult->GetEntries();
    for (Int_t i=0; i<nentries; i++) {
        bestResult->GetEntry(i);
        result[0] = f;
        result[1] = mu;
        result[2] = k;
        result[3] = p;
        result[4] = chi2;
        result[5] = chi2_error;
    }

    return result;
}