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

TGraphErrors *ConvertTH1FtoGraph(TH1F *h_input, double shift_x){

    std::vector<float> vec_x;
    std::vector<float> vec_y;
    std::vector<float> vec_xe;
    std::vector<float> vec_ye;

    if(h_input==nullptr){
        std::cout<<"empty histo to convert"<<std::endl;
        return nullptr;
    }

    int _nbins  = h_input->GetXaxis()->GetNbins();
    if(_nbins==0) return nullptr;

    for(int i=1; i<_nbins; i++){
        float x = h_input->GetBinCenter(i);
        float y = h_input->GetBinContent(h_input->FindBin(x));
        float xe = 0.0;
        float ye = h_input->GetBinError(h_input->FindBin(x));

        if(y==0.)continue;
        
        vec_x.push_back(x+shift_x);
        vec_y.push_back(y);
        vec_xe.push_back(xe);
        vec_ye.push_back(ye);
    }

    TGraphErrors *result = new TGraphErrors(vec_x.size(),&vec_x[0],&vec_y[0],&vec_xe[0],&vec_ye[0]);
    result->SetName(Form("gr_%s",h_input->GetName()));
    result->SetTitle(h_input->GetTitle());
    result->SetMarkerStyle(h_input->GetMarkerStyle());
    result->SetMarkerSize(2);
    result->SetMarkerColor(h_input->GetMarkerColor());
    result->SetLineColor(h_input->GetLineColor());

    return result;
}

//root -l -q -b DrawFinalPict.cpp'("/home/aleksandr/BM@N/centrality_result/glauber_qa_2_raw/glauber_qa_2_raw.root","/home/aleksandr/BM@N/centrality_result/glauber_qa_2_raw/glauber_qa_2_raw_Final.root","refMultRun8_VtxR1_VtxZ010_7310_7500_scale")'
void DrawFinalPict_b_sim(){
	
    gROOT->SetStyle("Pub");
    gROOT->ForceStyle();

    string outPath = "/home/aleksandr/BM@N/centrality_result";

    std::map<string,string> file_path_name = {
        {"raw",             "/home/aleksandr/BM@N/centrality_result/glauber_qa_2_raw/glauber_qa_2_raw_Final.root"},
        {"shift",           "/home/aleksandr/BM@N/centrality_result/glauber_qa_3_shift/glauber_qa_3_shift_Final.root"},
        {"reweight",        "/home/aleksandr/BM@N/centrality_result/glauber_qa_4_reweight/glauber_qa_4_reweight_Final.root"},
        {"ref, 8120-8170",  "/home/aleksandr/BM@N/centrality_result/glauber_qa_1_ref/glauber_qa_1_ref_Final.root"},
        {"raw, 7400-7450",  "/home/aleksandr/BM@N/centrality_result/glauber_qa_5_raw/glauber_qa_5_raw_Final.root"},
        {"raw, 7620-7640",  "/home/aleksandr/BM@N/centrality_result/glauber_qa_6_raw/glauber_qa_6_raw_Final.root"},
        {"raw, 7310-7500",  "/home/aleksandr/BM@N/centrality_result/glauber_qa_2_raw/glauber_qa_2_raw_Final.root"},
        {"old, 7400-7450",  "/home/aleksandr/BM@N/centrality_result/glauber_qa_5_noplp_b/glauber_qa_5_noplp_b_Final.root"},
        {"plp, 7400-7450",  "/home/aleksandr/BM@N/centrality_result/glauber_qa_5_raw_plp_2/glauber_qa_5_raw_plp_2_Final.root"}
    };

    std::map<string,std::vector<int>> MarkerStyleColor= {
        {"raw",             {22,1}},
        {"shift",           {53, 4}},
        {"reweight",        {54,2}},
        {"ref, 8120-8170",  {23,6}},
        {"raw, 7400-7450",  {54, 4}},
        {"raw, 7620-7640",  {53, 2}},
        {"raw, 7310-7500",  {22,1}},
        {"old, 7400-7450",  {54, 2}},
        {"plp, 7400-7450",  {22, 4}}
    };

    std::map<string,string> MarkerLegend={
        {"raw","raw (7310-7500)"},
        {"shift","shift"},
        {"reweight","reweight"},
        {"ref, 8120-8170","ref, 8120-8170"},
        {"raw, 7400-7450","raw, 7400-7450"},
        {"raw, 7620-7640","raw, 7620-7640"},
        {"raw, 7310-7500","raw, 7310-7500"},
        {"old, 7400-7450", "without pileup cut"},
        {"plp, 7400-7450", "with pileup cut"}
    };

    string name_histo = "B_average_VS_Centrality";

    //std::vector<string> draw_name={"ref, 8120-8170","raw","shift","reweight"};
    //std::vector<string> draw_name={"ref, 8120-8170","raw, 7310-7500","raw, 7400-7450","raw, 7620-7640"};
    std::vector<string> draw_name={"old, 7400-7450","plp, 7400-7450"};
    string outName = "";
    for(auto txt : draw_name){
        outName = outName + txt;
    }
    
    std::vector<TFile*> draw_file;
    std::vector<TH1F*> draw_histo;
    std::vector<TGraphErrors*> draw_graph;
    std::vector<TGraphErrors*> draw_histo_ratio;

    for(int i=0; i<draw_name.size(); i++){
        string name_file = file_path_name.at(draw_name[i]);
        draw_file.push_back( new TFile(name_file.c_str(),"READ"));

        draw_histo.push_back((TH1F*)draw_file.back()->Get(name_histo.c_str()));
        draw_histo.back()->SetName(Form("h%i_b_vs_cent",i));
        draw_histo.back()->SetMarkerStyle(MarkerStyleColor.at(draw_name[i])[0]);
        draw_histo.back()->SetMarkerSize(2);
        draw_histo.back()->SetMarkerColor(MarkerStyleColor.at(draw_name[i])[1]);
        draw_histo.back()->SetLineColor(MarkerStyleColor.at(draw_name[i])[1]);

        draw_graph.push_back(new TGraphErrors());
        draw_graph.back() = ConvertTH1FtoGraph(draw_histo.back(),0.5*i);
    }

    for(int i=1; i<draw_histo.size();i++){
        draw_histo_ratio.push_back(new TGraphErrors());
        draw_histo_ratio.back()=RatioDataFit(draw_histo[i],draw_histo[0]);
        draw_histo_ratio.back()->SetMarkerStyle(draw_histo[i]->GetMarkerStyle());
        draw_histo_ratio.back()->SetMarkerSize(2);
        draw_histo_ratio.back()->SetMarkerColor(draw_histo[i]->GetMarkerColor());
        draw_histo_ratio.back()->SetLineColor(draw_histo[i]->GetLineColor());
    }

    // Draw
    float AxisXmin = 0.011;
    float AxisXmax = 81.;
    float AxisRatioYmin = 0.87;
    float AxisRatioYmax = 1.13;
    double MinYHisto = 0.01;
    double MaxYHisto = 14.5;
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

    TH2F *axis_up = new TH2F("axix_up","",2,AxisXmin,AxisXmax,4,MinYHisto,MaxYHisto);
    axis_up->GetYaxis()->SetTitle("#font[42]{ #scale[1.0]{<b>#pmRMS, fm}}");
    axis_up->GetYaxis()->SetTitleOffset(0.8);
    axis_up->GetYaxis()->SetTitleSize(0.08);
    axis_up->GetYaxis()->SetLabelSize(0.07);
    pad_up->cd();
    axis_up->Draw();

    TH2F *axis_down = new TH2F("axix_down","",2,AxisXmin,AxisXmax,4,AxisRatioYmin,AxisRatioYmax);
    axis_down->GetXaxis()->SetTitle("#font[42]{ #scale[1.0]{centrality, %}}");
    axis_down->GetXaxis()->SetLabelSize(0.09);
    axis_down->GetYaxis()->SetLabelSize(0.09);
    axis_down->GetXaxis()->SetTitleOffset(1.2);
    axis_down->GetXaxis()->SetTitleSize(0.08);
    axis_down->GetYaxis()->SetTitle("#font[42]{ #scale[1.0]{X/ref}}");
    axis_down->GetYaxis()->SetTitleOffset(0.8);
    axis_down->GetYaxis()->SetTitleSize(0.08);
    pad_down->cd();
    axis_down->Draw();

    pad_up->cd();

    TLegend *legend = new TLegend(0.65,0.1,0.85,0.45);
    legend->SetNColumns(1);
    

    for(int i=0; i<draw_name.size(); i++){
        draw_graph.at(i)->Draw("sameP");
        legend->AddEntry(draw_graph.at(i), Form("#font[42]{ #scale[1.0]{%s}}",MarkerLegend.at(draw_name[i]).c_str()), "p");
    }
    legend->Draw("same");

    TLatex _textInfo;
    _textInfo.SetTextSize(0.07);
    _textInfo.DrawLatex(AxisXmin+0.1*(AxisXmax - AxisXmin), 0.85*H_y , 
        Form("#font[42]{ #scale[1.0]{BM@N Run8, Xe+Cs(I)}}"));

    // down pad
    pad_down->cd();

    for(int i=0; i<draw_histo_ratio.size(); i++){
        draw_histo_ratio.at(i)->Draw("sameP");
    }

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

    canvas->SaveAs(Form("%s/b_vs_cent_%s.png",outPath.c_str(),outName.c_str()));

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
        float ye_result = 0.0;
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