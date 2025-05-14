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
void DrawRefMult(string InFileGlauberQA, string InFileFinal, string NameHisto){
	
    gROOT->SetStyle("Pub");
    gROOT->ForceStyle();
    
    TFile *qa_file = new TFile(InFileGlauberQA.c_str(),"READ");// step 1        
    TFile *final_file = new TFile(InFileFinal.c_str(),"READ");// step 1        

    int n_rebin = 2;

    TH1F *HistoMult     = GetRebinHisto((TH1F*)qa_file->Get(NameHisto.c_str()),n_rebin);
    TH1F *FitHistoSng   = GetRebinHisto((TH1F*)qa_file->Get("glaub_sng_histo"),n_rebin);
    TH1F *FitHistoPlp   = GetRebinHisto((TH1F*)qa_file->Get("glaub_plp_histo"),n_rebin);
    TH1F *FitHistoAll   = GetRebinHisto((TH1F*)qa_file->Get("glaub_fit_histo"),n_rebin);

    TGraphErrors *gr_ratio = RatioDataFit(HistoMult,FitHistoAll);
    std::vector<float> fitPar = GetParBestFit(qa_file);

    float f_best    = fitPar.at(0);
    float mu_best   = fitPar.at(1);
    float k_best    = fitPar.at(2);
    float p_best    = fitPar.at(3);
    float chi2_best = fitPar.at(4);
    float chi2_error_best = fitPar.at(5);

    // Draw
    float AxisXmin = 0.1;
    float AxisXmax = 400.;
    float AxisRatioYmin = 0.71;
    float AxisRatioYmax = 1.29;
    
    double MinYHisto = 2.;
    double MaxYHisto = 1.5e6;
    double H_y = abs(MaxYHisto - MinYHisto);

    TCanvas *canvas = new TCanvas("canvas","plot", 1.4*960, 960);

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
    pad_up->SetLogy();

    TH2F *axis_up = new TH2F("axix_up","",2,AxisXmin,AxisXmax,4,MinYHisto,MaxYHisto);
    axis_up->GetYaxis()->SetTitle("#font[42]{ #scale[1.0]{dN_{ch}}}");
    axis_up->GetYaxis()->SetTitleOffset(0.8);
    axis_up->GetYaxis()->SetTitleSize(0.08);
    axis_up->GetYaxis()->SetLabelSize(0.08);
    pad_up->cd();
    axis_up->Draw();

    TH2F *axis_down = new TH2F("axix_down","",2,AxisXmin,AxisXmax,4,AxisRatioYmin,AxisRatioYmax);
    axis_down->GetXaxis()->SetTitle("#font[42]{ #scale[1.0]{N_{ch}}}");
    axis_down->GetXaxis()->SetLabelSize(0.1);
    axis_down->GetYaxis()->SetLabelSize(0.1);
    axis_down->GetXaxis()->SetTitleOffset(1);
    axis_down->GetXaxis()->SetTitleSize(0.1);
    axis_down->GetYaxis()->SetTitle("#font[42]{ #scale[1.0]{data/fit}}");
    axis_down->GetYaxis()->SetTitleOffset(0.6);
    axis_down->GetYaxis()->SetTitleSize(0.1);
    pad_down->cd();
    axis_down->Draw();

    pad_up->cd();
    //Single
    FitHistoSng->SetLineColor(3);
    FitHistoSng->SetMarkerSize(1.5);
    FitHistoSng->SetMarkerColor(3);
    FitHistoSng->SetMarkerStyle(26);
    FitHistoSng->Draw("sameE");
    //pile-up
    FitHistoPlp->SetLineColor(1);
    FitHistoPlp->SetMarkerSize(1.5);
    FitHistoPlp->SetMarkerColor(1);
    FitHistoPlp->SetMarkerStyle(32);
    FitHistoPlp->Draw("sameE");
    // data
    HistoMult->SetLineColor(4);
    HistoMult->SetMarkerColor(4);
    HistoMult->SetMarkerStyle(8);
    HistoMult->Draw("sameE");
    //All fit
    FitHistoAll->SetLineColor(2);
    FitHistoAll->SetMarkerColor(2);
    FitHistoAll->SetMarkerStyle(25);
    FitHistoAll->Draw("sameE");

    TLegend *legend = new TLegend(0.6,0.5,0.85,0.75);
    legend->SetNColumns(2);
    legend->AddEntry(HistoMult, "#font[42]{ #scale[1.0]{data}}", "p");
    legend->AddEntry(FitHistoAll, "#font[42]{ #scale[1.0]{fit}}", "p");
    legend->AddEntry(FitHistoSng, "#font[42]{ #scale[1.0]{single}}", "p");
    legend->AddEntry(FitHistoPlp, "#font[42]{ #scale[1.0]{pile-up}}", "p");
    legend->Draw("same");

    TLatex _textInfo;
    _textInfo.SetTextSize(0.07);
    _textInfo.DrawLatex(AxisXmin+0.2*(AxisXmax - AxisXmin), 0.3*H_y , 
        Form("#font[42]{ #scale[1.0]{BM@N Run8, Xe+Cs(I), N_{a}=fN_{part}+(1-f)N_{coll}}}"));
    _textInfo.DrawLatex(AxisXmin+0.2*(AxisXmax - AxisXmin), 0.08*H_y , 
        Form("#font[42]{ #scale[1.0]{f=%.1f, k=%.1f, #mu=%.2f, p=%.0f%%, #chi^{2}/ndf=%.2f#pm%.2f}}",
        f_best,k_best,mu_best,p_best*100,chi2_best,chi2_error_best));

    // down pad
    pad_down->cd();
    gr_ratio->SetLineColor(4);
    gr_ratio->SetMarkerColor(4);
    gr_ratio->SetMarkerStyle(4);
    gr_ratio->Draw("Psame");

    TLine *line_1 = new TLine( AxisXmin, 1., AxisXmax, 1. );
    line_1->SetLineColor( kBlack );
    line_1->SetLineWidth( 3 );
    line_1->SetLineStyle( 1 );
    line_1->Draw("same");

    TLine *line_2 = new TLine( AxisXmin, 1.05, AxisXmax, 1.05 );
    line_2->SetLineColor( kBlack );
    line_2->SetLineWidth( 3 );
    line_2->SetLineStyle( 2 );
    line_2->Draw("same");

    TLine *line_3 = new TLine( AxisXmin, 0.95, AxisXmax, .95 );
    line_3->SetLineColor( kBlack );
    line_3->SetLineWidth( 3 );
    line_3->SetLineStyle( 2 );
    line_3->Draw("same");


    canvas->SaveAs(Form("../res_%s.png",NameHisto.c_str()));

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