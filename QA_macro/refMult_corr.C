#include "QA_function.C"

std::vector<int> _color = {1,4,6,2,28,43,34,3,30,12,34,46,49,23,33,14,41,29,9,20,42,43,47};

void RefMultCorrRunId(TFile *inFile, DrawParameters obj_par, std::string path_save, TFile *outFile,  TFile *outFileCorr, std::string RunIdtxt, std::vector<int> _run, std::vector<int> _step = {-1,-1,-1}){

	outFile->cd();
	
	TH1D *hist;
	TH1D *h1_ref;
	TH1D *h1_all_raw;
	TH1D *h1_all_shift;
	TH1D *h1_all_shift_reweight;

	std::vector<TH1D*> h1_refMultVtxZbins;
	std::vector<TH1D*> h1_refMultVtxZbins_ratio;
	std::vector<TH1D*> h1_refMultVtxZbins_shift;
	std::vector<TH1D*> h1_refMultVtxZbins_shift_ratio;
	std::vector<TH1D*> h1_refMultVtxZbins_shift_reweight;
	std::vector<TH1D*> h1_refMultVtxZbins_shift_reweight_ratio;

	std::vector<double> h_events;
	std::vector<double> fitPar;
	std::vector<double> h_scale;
	std::vector<double> h_chi2;
	std::vector<double> h_yA;
	std::vector<double> h_yS;
	std::vector<double> h_y;
	std::vector<double> h_ye;
	std::vector<double> h_x;
	std::vector<double> h_xe;

	int runStart 	= _run[0];
	int runStop  	= _run[1];
	int RefRunIdMin = _run[2];
	int RefRunIdMax = _run[3];

	std::string _suf_run_range = Form("_%i_%i",RefRunIdMin,RefRunIdMax);

	double _RefMultMax_zerobin = 270;
	if(RefRunIdMax==7450)_RefMultMax_zerobin = 170;
	if(RefRunIdMax==7660)_RefMultMax_zerobin = 220;
	if(RefRunIdMax==8170)_RefMultMax_zerobin = 270;

	TH2D *pr1 = GetTH2(inFile,obj_par);
	if(pr1==nullptr){
		std::cout<<"Error!"<<std::endl;
		return;
	}
	int TotalNumRun = (int)pr1->GetNbinsX();

	h1_ref =  (TH1D*)pr1->ProjectionY(Form("%s_%i_%i_ref",pr1->GetName(),RefRunIdMin,RefRunIdMax),pr1->GetXaxis()->FindBin(RefRunIdMin),pr1->GetXaxis()->FindBin(RefRunIdMax));
	if(h1_ref==nullptr) return;
	h1_ref->SetTitle(Form("%i",RefRunIdMin));
	auto res_ref = FindMinChi2RefMultErf(h1_ref);
	double H_Ref = res_ref[4];
	std::cout<<"Ref H:\t"<<H_Ref<<std::endl;

	for(int i=0; i<TotalNumRun; i++){
		
		int h2RunId = pr1->GetXaxis()->GetBinCenter(i);

		std::cout<<"Current run:\t"<<h2RunId<<"\t[ "<<i<<"/"<<TotalNumRun<<" ]"<<std::endl;
		
		if(h2RunId<runStart || h2RunId>runStop)continue;
		//if(h2RunId>=RefRunIdMin && h2RunId<=RefRunIdMax)continue;
		
		hist = (TH1D*)pr1->ProjectionY(Form("run_%s_%i",pr1->GetName(),h2RunId),pr1->GetXaxis()->FindBin(h2RunId),pr1->GetXaxis()->FindBin(h2RunId));
		hist->SetName(Form("%i_%s",h2RunId,pr1->GetName()));
		hist->SetTitle(Form("%i",h2RunId));
		
		if(hist->GetEntries()==0) continue;
		if(hist->Integral()==0) continue;

		TH1D *res_shift = RefMultShift(hist,h1_ref,H_Ref,fitPar);
		TH1D *res_weight = RefMultReweight(res_shift,h1_ref);
		
		hist->Scale(1./hist->Integral());
		res_shift->Scale(1./res_shift->Integral());
		res_weight->Scale(1./res_weight->Integral());

		h_events.push_back(hist->GetEntries() / 1.0e6);
		h_x.push_back(h2RunId);// RunId
		h_yA.push_back(fitPar[2]);//A
		h_yS.push_back(fitPar[3]);//sigma
		h_y.push_back(fitPar[4]);//h - колено
		h_chi2.push_back(fitPar[5]);// chi2/ndf
		h_scale.push_back(fitPar[6]);// runId scale
		h_ye.push_back(0.);
		h_xe.push_back(0.);

		h1_refMultVtxZbins.push_back(hist);
		h1_refMultVtxZbins_shift.push_back(res_shift);
		h1_refMultVtxZbins_shift_reweight.push_back(res_weight);
	}

	if(h1_refMultVtxZbins.size()==0) return;

	h1_ref->Scale(1./h1_ref->Integral());

	for(int i=0; i<h1_refMultVtxZbins.size();i++){
		auto h1_ratio = new TH1D();
		h1_ref->Copy(*h1_ratio);
		h1_refMultVtxZbins_ratio.push_back(h1_ratio);
		h1_refMultVtxZbins_ratio[i]->Divide(h1_refMultVtxZbins[i]);
		if(i==0){
			h1_all_raw = new TH1D();
			h1_refMultVtxZbins[0]->Copy(*h1_all_raw);
		}else{
			h1_all_raw->Add(h1_refMultVtxZbins[i]);
		}
	}
	for(int i=0; i<h1_refMultVtxZbins_shift.size();i++){
		auto h1_ratio = new TH1D();
		h1_ref->Copy(*h1_ratio);
		h1_refMultVtxZbins_shift_ratio.push_back(h1_ratio);
		h1_refMultVtxZbins_shift_ratio[i]->Divide(h1_refMultVtxZbins_shift[i]);
		if(i==0){
			h1_all_shift = new TH1D();
			h1_refMultVtxZbins_shift[0]->Copy(*h1_all_shift);
		}else{
			h1_all_shift->Add(h1_refMultVtxZbins_shift[i]);
		}
	}
	for(int i=0; i<h1_refMultVtxZbins_shift_reweight.size();i++){
		auto h1_ratio = new TH1D();
		h1_ref->Copy(*h1_ratio);
		h1_refMultVtxZbins_shift_reweight_ratio.push_back(h1_ratio);
		h1_refMultVtxZbins_shift_reweight_ratio[i]->Divide(h1_refMultVtxZbins_shift_reweight[i]);
		if(i==0){
			h1_all_shift_reweight = new TH1D();
			h1_refMultVtxZbins_shift_reweight[0]->Copy(*h1_all_shift_reweight);
		}else{
			h1_all_shift_reweight->Add(h1_refMultVtxZbins_shift_reweight[i]);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Histo ////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TCanvas *can111 = new TCanvas(Form("H_%s%s",pr1->GetName(),_suf_run_range.c_str()),"",960,560);
 	auto *Def111 = new TGraphErrors(h_x.size(),&h_x[0],&h_y[0],&h_xe[0],&h_ye[0]);
	can111->cd();
	Def111->GetYaxis()->SetTitle(Form("h"));
	Def111->GetXaxis()->SetTitle(Form("Run Id"));
	if(_step[0]==1)Def111->GetYaxis()->SetRangeUser(110, 210);
	//Def111->GetYaxis()->SetRangeUser(110, 190);
	Def111->SetLineColor(kBlue);
	Def111->SetMarkerSize(1);
	Def111->SetMarkerStyle(4);
	Def111->SetMarkerColor(kBlue);
	Def111->Draw("AP");
	//Def111->Write();
	can111->SaveAs(Form("./%s/H_RinId_%s%s.png",path_save.c_str(),pr1->GetName(),_suf_run_range.c_str()));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Histo ////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TCanvas *can112 = new TCanvas(Form("S_%s%s",pr1->GetName(),_suf_run_range.c_str()),"",960,560);
 	auto *Def112 = new TGraphErrors(h_x.size(),&h_x[0],&h_yS[0],&h_xe[0],&h_ye[0]);
	can112->cd();
	Def112->GetYaxis()->SetTitle(Form("#sigma"));
	Def112->GetXaxis()->SetTitle(Form("Run Id"));
	//if(_step[0]==1)Def112->GetYaxis()->SetRangeUser(110, 210);
	Def112->GetYaxis()->SetRangeUser(0.01, 0.04);
	Def112->SetLineColor(kBlue);
	Def112->SetMarkerSize(1);
	Def112->SetMarkerStyle(4);
	Def112->SetMarkerColor(kBlue);
	Def112->Draw("AP");
	//Def112->Write();
	can112->SaveAs(Form("./%s/S_RinId_%s%s.png",path_save.c_str(),pr1->GetName(),_suf_run_range.c_str()));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Histo ////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TCanvas *can113 = new TCanvas(Form("A_%s%s",pr1->GetName(),_suf_run_range.c_str()),"",960,560);
 	auto *Def113 = new TGraphErrors(h_x.size(),&h_x[0],&h_yA[0],&h_xe[0],&h_ye[0]);
	can113->cd();
	Def113->GetYaxis()->SetTitle(Form("A"));
	Def113->GetXaxis()->SetTitle(Form("Run Id"));
	//if(_step[0]==1)Def113->GetYaxis()->SetRangeUser(110, 210);
	Def113->GetYaxis()->SetRangeUser(0.001, 0.004);
	Def113->SetLineColor(kBlue);
	Def113->SetMarkerSize(1);
	Def113->SetMarkerStyle(4);
	Def113->SetMarkerColor(kBlue);
	Def113->Draw("AP");
	//Def113->Write();
	can113->SaveAs(Form("./%s/A_RinId_%s%s.png",path_save.c_str(),pr1->GetName(),_suf_run_range.c_str()));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Histo ////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TCanvas *can114 = new TCanvas(Form("A_%s%s",pr1->GetName(),_suf_run_range.c_str()),"",960,560);
 	auto *Def114 = new TGraphErrors(h_x.size(),&h_x[0],&h_chi2[0],&h_xe[0],&h_ye[0]);
	can114->cd();
	Def114->GetYaxis()->SetTitle(Form("#Chi^{2}/ndf"));
	Def114->GetXaxis()->SetTitle(Form("Run Id"));
	//if(_step[0]==1)Def114->GetYaxis()->SetRangeUser(110, 210);
	Def114->GetYaxis()->SetRangeUser(0.2, 1.8);
	Def114->SetLineColor(kBlue);
	Def114->SetMarkerSize(1);
	Def114->SetMarkerStyle(4);
	Def114->SetMarkerColor(kBlue);
	Def114->Draw("AP");
	//Def114->Write();
	can114->SaveAs(Form("./%s/Chi2_RinId_%s%s.png",path_save.c_str(),pr1->GetName(),_suf_run_range.c_str()));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Histo Ratio//////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TCanvas *can115 = new TCanvas(Form("HScale_%s%s",pr1->GetName(),_suf_run_range.c_str()),"",960,560);
 	auto *Def115 = new TGraphErrors(h_x.size(),&h_x[0],&h_scale[0],&h_xe[0],&h_ye[0]);
	can115->cd();
	Def115->GetYaxis()->SetTitle(Form("#font[42]{ #scale[1.0]{RunId_{Corr factor}}}"));
	Def115->GetXaxis()->SetTitle(Form("Run Id"));
	Def115->SetName(Form("RunId_corr_factor_%s_%i_%i",obj_par.Name.c_str(),RefRunIdMin,RefRunIdMax));
	Def115->SetLineColor(kBlue);
	Def115->SetMarkerSize(1);
	Def115->SetMarkerStyle(4);
	Def115->SetMarkerColor(kBlue);
	Def115->Draw("AP");
	outFileCorr->cd();
	Def115->Write();
	TLine *line1 = new TLine(runStart,1.,runStop,1.);
	line1->SetLineColorAlpha(kBlack,0.5);
	line1->SetLineStyle(1);
	line1->Draw("same");
	can115->SaveAs(Form("./%s/HScale_RinId_%s%s.png",path_save.c_str(),pr1->GetName(),_suf_run_range.c_str()));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Histo Ratio//////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TCanvas *can116 = new TCanvas(Form("HScale_%s%s",pr1->GetName(),_suf_run_range.c_str()),"",960,560);
 	auto *Def116 = new TGraphErrors(h_x.size(),&h_x[0],&h_events[0],&h_xe[0],&h_ye[0]);
	can116->cd();
	Def116->GetYaxis()->SetTitle(Form("#font[42]{ #scale[1.0]{Events, M}}"));
	Def116->GetXaxis()->SetTitle(Form("Run Id"));
	Def116->GetYaxis()->SetRangeUser(-0.01, 0.51);
	Def116->SetName(Form("RunId_events_%s_%i_%i",obj_par.Name.c_str(),RefRunIdMin,RefRunIdMax));
	Def116->SetLineColor(kBlue);
	Def116->SetMarkerSize(1);
	Def116->SetMarkerStyle(4);
	Def116->SetMarkerColor(kBlue);
	Def116->Draw("AP");
	TLine *line16 = new TLine(runStart,0.,runStop,0.);
	line16->SetLineColorAlpha(kBlack,0.5);
	line16->SetLineStyle(1);
	line16->Draw("same");
	outFileCorr->cd();
	Def116->Write();
	can116->SaveAs(Form("./%s/Events_RinId_%s%s.png",path_save.c_str(),pr1->GetName(),_suf_run_range.c_str()));


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Histo: Axis Set//////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TH2D *h2_axis = new TH2D("axis","",2,0,400,2,0.8e-5,0.5e-1);
	h2_axis->GetYaxis()->SetTitle(Form("dN/N, refMult"));

	TH2D *h2_axis_ratio = new TH2D("axis","",2,0,400,2,0.01,2.1);
	h2_axis_ratio->GetYaxis()->SetLabelSize(0.1);
	h2_axis_ratio->GetXaxis()->SetLabelSize(0.1);
	h2_axis_ratio->GetXaxis()->SetTitleOffset(1);
	h2_axis_ratio->GetXaxis()->SetTitleSize(0.1);
	h2_axis_ratio->GetYaxis()->SetTitleOffset(0.5);
	h2_axis_ratio->GetYaxis()->SetTitleSize(0.1);
	h2_axis_ratio->GetYaxis()->SetTitle(Form("ref(%i,%i)/X",RefRunIdMin,RefRunIdMax));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Histo Ratio//////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	outFile->cd();
	TCanvas *can1 = new TCanvas(Form("test_%s%s",pr1->GetName(),_suf_run_range.c_str()),"",3*760,3*560);
	can1->GetFrame()->SetFillColor(21);
    can1->GetFrame()->SetBorderSize(115);
	TPad *pad_up 	= new TPad("up", "", 0.0,0.4,1.,1., 0,0,0);
	pad_up->SetBottomMargin(0);
	TPad *pad_down 	= new TPad("down", "", 0.0,0.00,1.,0.4, 0,0,0);
	pad_down->SetTopMargin(0);
	pad_down->SetBottomMargin(0.3);

	can1->cd();
	pad_up->Draw();
	pad_down->Draw();

	auto *legend_can = new TLegend(0.5,0.7,0.89,0.89);
	legend_can->SetHeader(Form("#font[42]{ #scale[1.0]{Run Id %i-%i}}",runStart,runStop));
	legend_can->SetNColumns(10);
	
	pad_up->cd();
	h2_axis->Draw();
	pad_down->cd();
	h2_axis_ratio->Draw();

	for(int i=0; i<h1_refMultVtxZbins.size();i++){
		h1_refMultVtxZbins[i]->SetMarkerStyle(3);
		h1_refMultVtxZbins_ratio[i]->SetMarkerStyle(3);

		pad_up->cd();
		h1_refMultVtxZbins[i]->Draw("histoPsame");

		pad_down->cd();
		h1_refMultVtxZbins_ratio[i]->Draw("histoPsame");

		legend_can->AddEntry(h1_refMultVtxZbins[i],Form("#font[42]{ #scale[1.0]{%s}}",h1_refMultVtxZbins[i]->GetTitle()),"l");
	}
	pad_up->cd();




	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////
	

	TFile *f_fit = new TFile("/home/aleksandr/BM@N/centrality_result/glauber_qa_dcm_gamma_30_300_VtxZCut15_Bdcm/glauber_qa_dcm_gamma_30_300_VtxZCut15_Bdcm.root","read");
	TH1D *h_fit=(TH1D*)f_fit->Get("Vtx_h2_vtx_z_nTracks_vtxZcut_scale");
	h_fit->Scale(1./h_fit->Integral());
	h_fit->SetLineColor(kRed);
	h_fit->SetLineWidth(4);
	h_fit->SetMarkerColor(kRed);
	h_fit->SetMarkerStyle(8);
	h_fit->SetMarkerSize(1.4);
	h_fit->Draw("same");

	TFile *f_fit2 = new TFile("/home/aleksandr/BM@N/QA_bmn_sim/XeCs_dcm_3.8AGeV_mult_new.root","read");
	TH1D *h_fit2=(TH1D*)f_fit2->Get("def_h1_nTracksSim2");
	h_fit2->Scale(1./h_fit2->Integral());
	h_fit2->SetLineColor(kBlue);
	h_fit2->SetLineWidth(4);
	h_fit2->SetMarkerColor(kBlue);
	h_fit2->SetMarkerStyle(22);
	h_fit2->SetMarkerSize(1.4);
	h_fit2->Draw("same");

	//////////////////////////////////////
	//////////////////////////////////////
	//////////////////////////////////////

	//legend_can->Draw("same");
	pad_up->SetLogy();
	pad_down->cd();
	TLine *line1_2 = new TLine(1,1.,400,1.);
	line1_2->SetLineColorAlpha(kRed,1);
	line1_2->SetLineStyle(1);
	line1_2->Draw("same");
    can1->SaveAs(Form("./%s/RefMultRunId_%s%s.png",path_save.c_str(), pr1->GetName(),_suf_run_range.c_str()));
	can1->Write();


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Histo Ratio After Shift//////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	outFile->cd();
	TCanvas *can2 = new TCanvas(Form("test_%s%s",pr1->GetName(),_suf_run_range.c_str()),"",3*760,3*560);
	can2->GetFrame()->SetFillColor(21);
    can2->GetFrame()->SetBorderSize(115);
	TPad *pad_up2 	= new TPad("up", "", 0.0,0.4,1.,1., 0,0,0);
	pad_up2->SetBottomMargin(0);
	TPad *pad_down2 	= new TPad("down", "", 0.0,0.00,1.,0.4, 0,0,0);
	pad_down2->SetTopMargin(0);
	pad_down2->SetBottomMargin(0.3);

	can2->cd();
	pad_up2->Draw();
	pad_down2->Draw();

	auto *legend_can2 = new TLegend(0.5,0.7,0.89,0.89);
	legend_can2->SetHeader(Form("#font[42]{ #scale[1.0]{Run Id %i-%i}}",runStart,runStop));
	legend_can2->SetNColumns(10);
	
	pad_up2->cd();
	h2_axis->Draw();
	pad_down2->cd();
	h2_axis_ratio->Draw();

	for(int i=0; i<h1_refMultVtxZbins_shift.size();i++){
		h1_refMultVtxZbins_shift[i]->SetMarkerStyle(3);
		h1_refMultVtxZbins_shift_ratio[i]->SetMarkerStyle(3);
		
		pad_up2->cd();
		h1_refMultVtxZbins_shift[i]->Draw("histoPsame");
		
		pad_down2->cd();
		h1_refMultVtxZbins_shift_ratio[i]->Draw("histoPsame");	
		
		legend_can2->AddEntry(h1_refMultVtxZbins_shift[i],Form("#font[42]{ #scale[1.0]{%s}}",h1_refMultVtxZbins_shift[i]->GetTitle()),"l");
	}
	pad_up2->cd();
	//legend_can->Draw("same");
	pad_up2->SetLogy();
	pad_down2->cd();
	TLine *line1_22 = new TLine(1,1.,400,1.);
	line1_22->SetLineColorAlpha(kRed,1);
	line1_22->SetLineStyle(1);
	line1_22->Draw("same");
    can2->SaveAs(Form("./%s/RefMultRunId_%s%s_shift.png",path_save.c_str(), pr1->GetName(),_suf_run_range.c_str()));
	can2->Write();


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Histo Ratio After Shift and ReWeight/////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	outFile->cd();
	TCanvas *can3 = new TCanvas(Form("test_%s%s",pr1->GetName(),_suf_run_range.c_str()),"",3*760,3*560);
	can3->GetFrame()->SetFillColor(21);
    can3->GetFrame()->SetBorderSize(115);
	TPad *pad_up3 	= new TPad("up", "", 0.0,0.4,1.,1., 0,0,0);
	pad_up3->SetBottomMargin(0);
	TPad *pad_down3 	= new TPad("down", "", 0.0,0.00,1.,0.4, 0,0,0);
	pad_down3->SetTopMargin(0);
	pad_down3->SetBottomMargin(0.3);

	can3->cd();
	pad_up3->Draw();
	pad_down3->Draw();

	auto *legend_can3 = new TLegend(0.5,0.7,0.89,0.89);
	legend_can3->SetHeader(Form("#font[42]{ #scale[1.0]{Run Id %i-%i}}",runStart,runStop));
	legend_can3->SetNColumns(10);
	
	pad_up3->cd();
	h2_axis->Draw();
	pad_down3->cd();
	h2_axis_ratio->Draw();

	for(int i=0; i<h1_refMultVtxZbins_shift_reweight.size();i++){
		h1_refMultVtxZbins_shift_reweight[i]->SetMarkerStyle(3);
		h1_refMultVtxZbins_shift_reweight_ratio[i]->SetMarkerStyle(3);

		pad_up3->cd();
		h1_refMultVtxZbins_shift_reweight[i]->Draw("histoPsame");

		pad_down3->cd();
		h1_refMultVtxZbins_shift_reweight_ratio[i]->Draw("histoPsame");	

		legend_can3->AddEntry(h1_refMultVtxZbins_shift_reweight[i],Form("#font[42]{ #scale[1.0]{%s}}",h1_refMultVtxZbins_shift_reweight[i]->GetTitle()),"l");
	}
	pad_up3->cd();
	//legend_can->Draw("same");
	pad_up3->SetLogy();
	pad_down3->cd();
	TLine *line1_23 = new TLine(1,1.,400,1.);
	line1_23->SetLineColorAlpha(kRed,1);
	line1_23->SetLineStyle(1);
	line1_23->Draw("same");
    can3->SaveAs(Form("./%s/RefMultRunId_%s%s_shift_reweight.png",path_save.c_str(), pr1->GetName(),_suf_run_range.c_str()));
	can3->Write();


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////// Draw Histo Ratio After Shift and ReWeight (Hadd)//////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	h1_all_raw->SetName(Form("refMultRun8_VtxR1_VtxZ010_%i_%i_ref_%i_%i_Raw",runStart,runStop,RefRunIdMin,RefRunIdMax));
	h1_all_shift->SetName(Form("refMultRun8_VtxR1_VtxZ010_%i_%i_ref_%i_%i_Shift",runStart,runStop,RefRunIdMin,RefRunIdMax));
	h1_all_shift_reweight->SetName(Form("refMultRun8_VtxR1_VtxZ010_%i_%i_ref_%i_%i_raw_ShiftReweight",runStart,runStop,RefRunIdMin,RefRunIdMax));

	h1_all_raw->Write();
	h1_all_shift->Write();
	h1_all_shift_reweight->Write();

	TCanvas *can110 = new TCanvas(Form("all_%s%s",pr1->GetName(),_suf_run_range.c_str()),"",960,560);
	can110->cd();

	h1_ref->SetMarkerStyle(34);
	h1_ref->SetMarkerColor(kBlack);
	h1_ref->Draw("P");
	
	h1_all_raw->Scale(1./h1_all_raw->Integral());
	h1_all_raw->SetMarkerStyle(53);
	h1_all_raw->SetMarkerSize(0.5);
	h1_all_raw->SetMarkerColor(kRed);
	h1_all_raw->Draw("SameP");

	h1_all_shift->Scale(1./h1_all_shift->Integral());
	h1_all_shift->SetMarkerStyle(33);
	h1_all_shift->SetMarkerColor(kBlue);
	h1_all_raw->SetMarkerSize(0.5);
	h1_all_shift->Draw("sameP");
	
	h1_all_shift_reweight->Scale(1./h1_all_shift_reweight->Integral());
	h1_all_shift_reweight->SetMarkerStyle(25);
	h1_all_shift_reweight->SetMarkerColor(kMagenta);
	h1_all_shift_reweight->SetMarkerSize(0.5);
	h1_all_shift_reweight->Draw("sameP");
	
	can110->SetLogy();
	can110->SaveAs(Form("./%s/refMultAll_RinId_%s%s.png",path_save.c_str(),pr1->GetName(),_suf_run_range.c_str()));
	can110->Write();


}













void testMult(TFile *file_inFile_test, TFile *outFile, string _runIdRange, string _run, string _refRunId, string _sys){

	TH1D *raw = (TH1D*)file_inFile_test->Get(Form("refMultRun8_VtxR1_VtxZ010%s_%s",_sys.c_str(),_runIdRange.c_str()));
	TH1D *href = (TH1D*)file_inFile_test->Get(Form("refMultRun8_VtxR1_VtxZ010%s_%s",_sys.c_str(),_refRunId.c_str()));

	href->SetTitle("8140");
	raw->SetTitle(_run.c_str());

	outFile->cd();

	std::vector<double> fitPar;
	auto shift_par = FindMinChi2RefMultErf(href);
	TH1D *shift = RefMultShift(raw,href,shift_par[4],fitPar);
	TH1D *shift_weight = RefMultReweight(shift,href);

	std::cout<<"Ref H:\t"<<shift_par[4]<<std::endl;

	href->SetName(Form("refMultRun8_VtxR1_VtxZ010%s_%s_ref",_sys.c_str(),_refRunId.c_str()));
	raw->SetName(Form("refMultRun8_VtxR1_VtxZ010%s_%s",_sys.c_str(),_runIdRange.c_str()));
	shift->SetName(Form("refMultRun8_VtxR1_VtxZ010%s_%s_shift",_sys.c_str(),_runIdRange.c_str()));
	shift_weight->SetName(Form("refMultRun8_VtxR1_VtxZ010%s_%s_shift_reweight",_sys.c_str(),_runIdRange.c_str()));

	TH1D *shift_nonZero = TH1_EvalZeroBin2(shift,350);
	shift_nonZero->SetName(Form("refMultRun8_VtxR1_VtxZ010%s_%s_shift_nonZeroBin",_sys.c_str(),_runIdRange.c_str()));

	TH1D *shift_weight_nonZero = TH1_EvalZeroBin2(shift_weight,350);
	shift_weight_nonZero->SetName(Form("refMultRun8_VtxR1_VtxZ010%s_%s_shift_reweight_nonZeroBin",_sys.c_str(),_runIdRange.c_str()));

/*
	TH1D *href_scale 		 = ScaleMult(file_inFile_test,href, 500000);
	TH1D *raw_scale 		 = ScaleMult(file_inFile_test,raw,(_runIdRange=="7310_7500") ? 500000 : 500000);
	TH1D *shift_scale 		 = ScaleMult(file_inFile_test,shift,(_runIdRange=="7310_7500") ? 500000 : 500000);
	TH1D *shift_weight_scale = ScaleMult(file_inFile_test,shift_weight,(_runIdRange=="7310_7500") ? 500000 : 500000);
	TH1D *shift_nonZero_scale  		 = ScaleMult(file_inFile_test,shift_nonZero,(_runIdRange=="7310_7500") ? 500000 : 500000);
	TH1D *shift_weight_nonZero_scale = ScaleMult(file_inFile_test,shift_weight_nonZero,(_runIdRange=="7310_7500") ? 500000 : 500000);

*/
	TH1D *href_scale 		 = Resample(500000,href);
	TH1D *raw_scale 		 = Resample(500000,raw);
	TH1D *shift_scale 		 = Resample(500000,shift);
	TH1D *shift_weight_scale = Resample(500000,shift_weight);
	TH1D *shift_nonZero_scale  		 = Resample(500000,shift_nonZero);
	TH1D *shift_weight_nonZero_scale = Resample(500000,shift_weight_nonZero);

/*
	href->Scale(1./href->Integral());
	raw->Scale(1./raw->Integral());
	shift_nonZero->Scale(1./shift_nonZero->Integral());
	shift_weight_nonZero->Scale(1./shift_weight_nonZero->Integral());

	href_scale->Scale(1./href_scale->Integral());
	raw_scale->Scale(1./raw_scale->Integral());
	shift_nonZero_scale->Scale(1./shift_nonZero_scale->Integral());
	shift_weight_nonZero_scale->Scale(1./shift_weight_nonZero_scale->Integral());
*/
	
	href->Write();
	raw->Write();
	//shift->Write();
	shift_nonZero->Write();
	//shift_weight->Write();
	shift_weight_nonZero->Write();
	
	href_scale->Write();
	raw_scale->Write();
	//shift_scale->Write();
	shift_nonZero_scale->Write();
	//shift_weight_scale->Write();
	shift_weight_nonZero_scale->Write();

//	href->Scale(1./href->Integral());
//	raw->Scale(1./raw->Integral());
//	shift->Scale(1./shift->Integral());
//	shift_weight->Scale(1./shift_weight->Integral());

	TCanvas *can112 = new TCanvas(Form("S"),"",960,760);
	can112->SetLogy();
	raw->GetXaxis()->SetRangeUser(0,400);
	raw->GetYaxis()->SetTitle("#font[42]{ #scale[1.0]{dN/N}}");
	raw->SetMarkerStyle(53);
	raw->SetMarkerColor(kRed);
	raw->SetLineColor(kRed);
	raw->SetMarkerSize(1.2);
	raw->Draw("P");
	href->SetMarkerStyle(34);
	href->SetMarkerColor(kBlack);
	href->SetLineColor(kBlack);
	href->SetMarkerSize(1.2);
	href->Draw("sameP");
	shift->SetMarkerStyle(33);
	shift->SetMarkerColor(kBlue);
	shift->SetLineColor(kBlue);
	shift->SetMarkerSize(1.2);
	shift->Draw("sameP");
	shift_weight->SetMarkerStyle(33);
	shift_weight->SetMarkerColor(kMagenta);
	shift_weight->SetLineColor(kMagenta);
	shift_weight->SetMarkerSize(1.2);
	shift_weight->Draw("sameP");

    TLegend *legend = new TLegend(0.2,0.2,0.5,0.5);
    legend->SetNColumns(1);
    legend->AddEntry(href, "#font[42]{ #scale[1.0]{ref (8120-8170)}}", "P");
    legend->AddEntry(raw, "#font[42]{ #scale[1.0]{raw (7310-7500)}}", "P");
    legend->AddEntry(shift, "#font[42]{ #scale[1.0]{after shift}}", "P");
    legend->AddEntry(shift_weight, "#font[42]{ #scale[1.0]{after reweight}}", "P");
    legend->Draw("same");

	can112->SaveAs(Form("./test_%s.png",_runIdRange.c_str()));

	// test

}













void refMult_corr(){	
	
	makeplotstyle();
	TFile *fstyle = new TFile("./style.root");
	TStyle *tsty = (TStyle *)fstyle->Get("PlottingInStyle");
	tsty->cd();
 	//gROOT->SetStyle("Pub");
	//gROOT->ForceStyle();
	// RunByRun
	std::string path_to_project="/home/aleksandr/BM@N";
	std::string path = path_to_project + "/Final_codes_bmn/pict/QA_RunByRun_24.12";
	std::string path2 = path_to_project + "/Final_codes_bmn/pict/QA_RunByRun_24.12/refMult_corr";

	gSystem->Exec(Form("mkdir -p ./%s/",path.c_str()));
	gSystem->Exec(Form("mkdir -p ./%s",path2.c_str()));
	
	//TFile *file_inFile_test	= new TFile("../run8_mult_RunId_test_old.root","READ");// step 1		
	TFile *file_inFile 	= new TFile("/home/aleksandr/BM@N/qa_root/qa_run8_vf_24.12_plp.root","READ");// step 1		
	
	//TFile *file_inFile 	= new TFile("..//qa_run8_vf_24.04_MultVtxZ.root","READ");	
	//TFile *file_outFile = new TFile("../OUT_24.04_runId.root","UPDATE");
	TFile *file_outFile = new TFile(Form("./%s/refMult_VtxR1_VtxZ01.root",path.c_str()),"RECREATE");	
	TFile *file_outFileCorr = new TFile(Form("./%s/OUT_rec_24.04_refMultCorr.root",path.c_str()),"UPDATE");	

	std::string suf = "nVtxTr_";

	std::vector<double> VyxBins = {-1.0,-0.8,-0.6,-0.4,-0.2,0.2,0.4,0.6,0.8,1.0};
	std::vector<double> VzBins = {-0.1,-.08,-.06,-.04,.04,.06,.08,.1};
	std::vector<double> VrBins = {0.0,0.2,0.4,0.6,0.8,1.0};
	std::vector<int> RunIdBins = {7100,7200,7400,7500,7600,7700,7800,7900,8121,8130,8140,8161,8165};


	//RefMultCorrRunId(file_inFile, {"h2_RunId_nTracks",suf,0,4,3.,{},{0,400},1}, path2, file_outFile, file_outFileCorr, "Run Id",{7310,7500,8120,8170});
	RefMultCorrRunId(file_inFile, {"h2_RunId_nTracks",suf,0,4,3.,{},{0,400},1}, path2, file_outFile, file_outFileCorr, "Run Id",{7510,8300,8120,8170});

	/// Test
 	//testMult(file_inFile_test, file_outFile, "7310_7500", "7350", "8120_8170","");
	//testMult(file_inFile_test, file_outFile, "7400_7450", "7400", "8120_8170","");
	//testMult(file_inFile_test, file_outFile, "7620_7640", "7610", "8120_8170","");

	/// Test
 	//testMult(file_inFile_test, file_outFile, "7310_7500", "7350", "8120_8170","_GT");
	//testMult(file_inFile_test, file_outFile, "7400_7450", "7400", "8120_8170","_GT");
	//testMult(file_inFile_test, file_outFile, "7620_7640", "7610", "8120_8170","_GT");

	file_outFile->Close();
	file_inFile->Close();


}

