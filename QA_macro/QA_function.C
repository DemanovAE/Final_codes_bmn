#include <string>
#include <iostream>
#include <vector>
#include <TGraph.h>
#include <TMath.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TProfile3D.h>
#include "TFile.h"
#include <TH1.h>
#include <TH2.h>
#include <TPad.h>

const float PROTON_M = 0.93827208;
const float PION_M = 0.13957061;

const double RunIdMinSample = 8050;
const double RunIdMaxSample = 8060;

const double _RunIdStart = 7050;
const double _RunIdStop  = 8300;
const std::pair<double,double> th1_RanRange = {8005.,8005.};
const double RunIdMin = 6950;
const double RunIdMax = 8300;
const double hRun = (RunIdMax-RunIdMin);
const double nSigma = 3.0;
const int _iter = 10;

//const std::vector<int> GroupRunsRefMult = {7015,};

//for M2 analysis (check qa_run8_new.C)
const double fP_start	= 0.1;
const double fP_finish	= 5.1;
const double fP_step  	= 0.2;

std::vector<double> BadRunsGlobal;

struct DrawParameters
{
	std::string Name;
	std::string Suffix;
	float Marker = 0;
	float Color = 0;
	float Width = 0;
	std::vector<double> RangeX;
	std::vector<double> RangeY;
	int LogScale = 0;
	std::vector<double> iRunId;
};
/*
// диапазон должен быть одинаковый, бининг может быть разный
TH1D *RatioTH1(TH1D *hist_up, TH1D *hist_down){
	
	if(hist_up==nullptr){
		std::cout<<"Error: not found "<<hist_up->GetName()<<std::endl;
		return nullptr;
	}
	if(hist_down==nullptr){
		std::cout<<"Error: not found "<<hist_down->GetName()<<std::endl;
		return nullptr;
	}
	if(hist_up->GetXaxis()->GetXmin()!=hist_down->GetXaxis()->GetXmin()){
		std::cout<<"Error: different range! Check axis X min"<<std::endl;
		return nullptr;
	}
	if(hist_up->GetXaxis()->GetXmax()!=hist_down->GetXaxis()->GetXmax()){
		std::cout<<"Error: different range! Check axis X max"<<std::endl;
		return nullptr;
	}

	std::vector<double> vec_x;
	std::vector<double> vec_y;
	std::vector<double> vec_xe;
	std::vector<double> vec_ye;

	int _nbins	=	hist_up->GetXaxis()->GetNbins() > hist_down->GetXaxis()->GetNbins() ? hist_up->GetXaxis()->GetNbins() : hist_down->GetXaxis()->GetNbins();
	int _binMin =	hist_up->GetXaxis()->GetXmin();
	int _binMax =	hist_up->GetXaxis()->GetXmax();
	TH1D *result = new TH1D(Form("ratio_%s_%s",hist_up->GetName(),hist_down->GetName()),"",_nbins,(float)_binMin,(float)_binMax);

	for(int ib=1; ib<hist_up->GetXaxis()->GetNbins(); ib++){
		auto x = 
	}

	TGraphErrors *gr_1;
	TGraphErrors *gr_2;

}
*/

TH1D *Resample(int nev, TH1D* const& hist)
{
    if(hist==nullptr){
    	std::cout<<"Error Resample: not found histo"<<std::endl;
    }
    auto result = new TH1D(Form("%s_resampled",hist->GetName()), 
        Form("resampled %s", hist->GetTitle()), 
        hist->GetNbinsX(), hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
    
    for (int i=0; i<nev; ++i){
        result->Fill(hist->GetRandom());
    }

    return result;
}

TH1D *Rescale(int nev, TH1D* const& hist)
{
    if(hist==nullptr){
    	std::cout<<"Error Resample: not found histo"<<std::endl;
    }
    TH1D *result = new TH1D();
    hist->Copy(*result);
    result->SetName(Form("%s_rescale",hist->GetName()));
    result->SetTitle(Form("resampled %s", hist->GetTitle()));

    double _scale = (double)nev / hist->Integral();

    result->Scale(_scale);

    return result;
}

TH1D* ScaleMult(TFile *_inFile, TH1D* h_input, int _EventInGlauber = 6000000){

	//TH1D* h_input = (TH1D*)_inFile->Get(_NameHist.c_str());
	
	int _nbins  = h_input->GetXaxis()->GetNbins();
	int _binMin = h_input->GetXaxis()->GetXmin();
	int _binMax = h_input->GetXaxis()->GetXmax();	

	double _RefMult = _EventInGlauber;
	//double _scale = (double)_RefMult / (double)h_input->GetEntries();
	double _scale = (double)_RefMult / (double)h_input->Integral();

	std::cout<<_scale<<"\n";

	TH1D *h_output = new TH1D(Form("%s_scale",h_input->GetName()),"",_nbins,(float)_binMin,(float)_binMax);

	for(int i=0; i<_nbins; i++){
		double x = h_input->GetBinCenter(i);
		double y = h_input->GetBinContent(h_input->FindBin(x));
		
		for(int j=0; j<(int)(y * _scale); j++){
			h_output->Fill(x);
		}
	}

	return h_output;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////// Fit SOft /////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::pair<float,float> FitRangeRunId(int _RunId){
	
	double fit_start = 80;
	double fit_stop  = 160;
	
	if(_RunId<7013){
		fit_start = 80;
		fit_stop  = 160;
	}else if(_RunId<7203){
		fit_start = 82;
		fit_stop  = 170;
	}else if(_RunId<7229){
		fit_start = 110;
		fit_stop  = 220;
	}else if(_RunId<7300){
		fit_start = 100;
		fit_stop  = 170;
	}else if(_RunId<7500){
		fit_start = 85;
		fit_stop  = 165;
	}else if(_RunId<7756){//bad
		fit_start = 100;
		fit_stop  = 180;
	}else if(_RunId<7761){//bad
		fit_start = 125;
		fit_stop  = 220;
	}else if(_RunId<7828){
		fit_start = 130;
		fit_stop  = 230;
	}else if(_RunId<7916){
		fit_start = 140;
		fit_stop  = 220;
	}else if(_RunId<7961){
		fit_start = 100;
		fit_stop  = 210;
	}else if(_RunId<8240){
		fit_start = 130;
		fit_stop  = 235;
	}else if(_RunId<8312){
		fit_start = 135;
		fit_stop  = 235;
	}

	return {fit_start,fit_stop};
}

TGraphErrors *ConvertTH1toTGraph(TH1D *h_input){

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
	float scale = h_input->Integral();

	for(int i=1; i<_nbins; i++){
		float x = h_input->GetBinCenter(i);
		float y = h_input->GetBinContent(h_input->FindBin(x)) / scale;
		float xe = 0.0;
		float ye = h_input->GetBinError(h_input->FindBin(x)) / scale;

		if(y==0.)continue;
		
		vec_x.push_back(x);
		vec_y.push_back(y);
		vec_xe.push_back(xe);
		vec_ye.push_back(ye);
	}

	TGraphErrors *result = new TGraphErrors(vec_x.size(),&vec_x[0],&vec_y[0],&vec_xe[0],&vec_ye[0]);
	result->SetName(Form("gr_%s",h_input->GetName()));
	result->SetTitle(h_input->GetTitle());

	return result;
}

std::vector<double> FitRefMultErf(TGraphErrors* _hist, TF1 *f1_fit, double width_step = 50){	
	
	int RunId = atoi(_hist->GetTitle());
	double minStart = FitRangeRunId(RunId).first;
	double maxStart = FitRangeRunId(RunId).second;
	int nStep = (maxStart - minStart - width_step);
	std::map<double,std::vector<double>> _param = {{9999.,{9999.,9999.,9999.,9999.,9999.,9999.}}};

	if(nStep<1) return _param.at(9999);

	for(int i=0; i<nStep; i++){
		//double _min = (double)i + minStart;
		double _min = minStart;
		double _max = minStart + (double)i + width_step;
		f1_fit->SetParameter(0,2.58e-03);
		f1_fit->SetParameter(1,2.25e-02);
		f1_fit->SetParameter(2,(_min+_max)/2.);
		_hist->Fit(f1_fit,"QR","",_min,_max);
		_hist->Fit(f1_fit,"QR","",_min,_max);
		_param.insert({f1_fit->GetChisquare()/f1_fit->GetNDF(),{_min,_max,f1_fit->GetParameter(0),f1_fit->GetParameter(1),f1_fit->GetParameter(2),f1_fit->GetChisquare()/f1_fit->GetNDF()}});
		//std::cout<<_min<<"\t"<<_max<<"\t"<<f1_fit->GetChisquare()<<"\t"<<f1_fit->GetParameter(0)<<"\t"<<f1_fit->GetParameter(1)<<"\t"<<f1_fit->GetParameter(2)<<"\n";
		//_hist->Write();
	}

	auto min_it = std::min_element(_param.begin(), _param.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

	if(_param.size()>1) return _param.at(min_it->first);
	
	return _param.at(9999);
}

std::vector<double> FindMinChi2RefMultErf(TH1D *_hist){
	
	double _minWidthStep = 67;
	double _maxWidthStep = 84;
	double nStep = _maxWidthStep - _minWidthStep;

	std::map<double,std::vector<double>> _param = {{9999.,{9999.,9999.,9999.,9999.,9999.}}};

	if(_hist->GetEntries()==0) return _param[0];
	if(_hist->Integral()==0) return _param[0];

	TGraphErrors *gr_input_to_fit = ConvertTH1toTGraph(_hist);

	TF1* f1_fit = new TF1(Form("fit_%s",_hist->GetName()),"[0] * ROOT::Math::erf(-1.*[1]*(x-[2])) + [0]",90,250);
	f1_fit->SetLineColor(kRed);

	for(int i=0; i<nStep; i++){
		double _width = _minWidthStep + (double)i;
		auto _res = FitRefMultErf(gr_input_to_fit,f1_fit,_width);
		_param.insert( { _res.back(), _res } );
	}

	auto min_it = std::min_element(_param.begin(), _param.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
	auto result = _param.at(min_it->first);
	
	//auto f1_fit = new TF1(Form("fit_%s",_hist->GetName()),"[0] * ROOT::Math::erf(-1.*[1]*(x-[2])) + [0]",result[0],result[1]);
	f1_fit->FixParameter(0,result[2]);
	f1_fit->FixParameter(1,result[3]);
	f1_fit->FixParameter(2,result[4]);
	gr_input_to_fit->Fit(f1_fit,"QRM","",result[0],result[1]);
	gr_input_to_fit->Write();

	if(_param.size()>1) return result;

	return _param[0];
}

TH1D* RefMultShift(TH1D* in_hist, TH1D* ref_hist, double h_Ref, std::vector<double> &par_fit){

	if(in_hist==nullptr){
		std::cout<<"Error: not found input histo"<<std::endl;
		return nullptr;
	}
	if(ref_hist==nullptr){
		std::cout<<"Error: not found ref histo"<<std::endl;
		return nullptr;
	}
	if(in_hist->GetXaxis()->GetXmin()!=ref_hist->GetXaxis()->GetXmin()){
		std::cout<<"Error: different range! Check axis X min"<<std::endl;
		return nullptr;
	}
	if(in_hist->GetXaxis()->GetXmax()!=ref_hist->GetXaxis()->GetXmax()){
		std::cout<<"Error: different range! Check axis X max"<<std::endl;
		return nullptr;
	}

	int _nbins  = in_hist->GetXaxis()->GetNbins();
	int _binMin = in_hist->GetXaxis()->GetXmin();
	int _binMax = in_hist->GetXaxis()->GetXmax();
	
	TH1D *result = new TH1D(Form("%s_%s",in_hist->GetName(),ref_hist->GetName()),"",_nbins,(float)_binMin,(float)_binMax);

	auto _res_in = FindMinChi2RefMultErf(in_hist);

	double _scale = 1.;
	if(_res_in[4]!=0 && h_Ref!=0){
		_scale = h_Ref / _res_in[4];
	}

	for(int ip=1; ip<_nbins; ip++){
		auto _x = in_hist->GetBinCenter(ip);
		auto _y = in_hist->GetBinContent(ip);
		for(int iev=0; iev<_y; iev++){
			float fill_val = (int)_x * _scale;
			result->Fill(fill_val);
		}
	}

	par_fit.clear();
	par_fit.push_back(_res_in[0]);// min range
	par_fit.push_back(_res_in[1]);// max range
	par_fit.push_back(_res_in[2]);// A
	par_fit.push_back(_res_in[3]);// sigma
	par_fit.push_back(_res_in[4]);// h - колено
	par_fit.push_back(_res_in[5]);// chi2
	par_fit.push_back(_scale);// scale factor
	return result;
}

TH1D* RefMultReweight(TH1D* in_hist, TH1D* ref_hist){
	if(in_hist==nullptr){
		std::cout<<"Error: not found input histo"<<std::endl;
		return nullptr;
	}
	if(ref_hist==nullptr){
		std::cout<<"Error: not found ref histo"<<std::endl;
		return nullptr;
	}
	if(in_hist->GetXaxis()->GetXmin()!=ref_hist->GetXaxis()->GetXmin()){
		std::cout<<"Error: different range! Check axis X min"<<std::endl;
		return nullptr;
	}
	if(in_hist->GetXaxis()->GetXmax()!=ref_hist->GetXaxis()->GetXmax()){
		std::cout<<"Error: different range! Check axis X max"<<std::endl;
		return nullptr;
	}

	int _nbins  = in_hist->GetXaxis()->GetNbins();
	int _binMin = in_hist->GetXaxis()->GetXmin();
	int _binMax = in_hist->GetXaxis()->GetXmax();
	
	TH1D *result = new TH1D(Form("%s_%s",in_hist->GetName(),ref_hist->GetName()),"",_nbins,(float)_binMin,(float)_binMax);

	for(int ie=1; ie<_nbins; ie++){
		double _x = in_hist->GetBinCenter(ie);
		double _y = in_hist->GetBinContent(ie);
		double _y_ref = ref_hist->GetBinContent(ref_hist->FindBin(_x));
		double weight = (_y==0 || _y_ref==0) ? 1. : _y_ref / _y;
		//double _ye = in_hist->GetBinError(ie) * weight;
	
		for(int ip=0; ip<(int)_y; ip++){
			result->Fill(_x,weight);
		}
		//result->Fill(_x, _y*weight);
		//result->SetBinError(in_hist->FindBin(_x),_ye);
	
	}

	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TH1D* TH1_EvalZeroBin2(TH1D *histo, double refMult_max = 350){
	
	int _nbins  = histo->GetXaxis()->GetNbins();
	int _binMin = histo->GetXaxis()->GetXmin();
	int _binMax = histo->GetXaxis()->GetXmax();

	TH1D *result = new TH1D(Form("h1_%s_%.1f",histo->GetName(),refMult_max),"",_nbins,(float)_binMin,(float)_binMax);

	std::vector<double> x;
	std::vector<double> y;
	std::vector<double> xe;
	std::vector<double> ye;
	
	std::vector<double> gr_x;
	std::vector<double> gr_y;
	std::vector<double> gr_y2;
	std::vector<double> gr_xe;
	std::vector<double> gr_ye;

	for(int ip=1; ip<_nbins-1; ip++){
		auto _x = histo->GetBinCenter(ip);
		auto _y = histo->GetBinContent(histo->FindBin(_x));
		auto _ye= histo->GetBinError(histo->FindBin(_x));
		x.push_back(_x);
		xe.push_back(0.);
		y.push_back(_y);
		ye.push_back(_ye);
		
		if(_y>0){
			gr_x.push_back(_x);
			gr_xe.push_back(0.);
			gr_y.push_back(_y);
			gr_y2.push_back(_ye);
			gr_ye.push_back(_ye);
		}
	}

	auto *gr_val = new TGraphErrors(gr_x.size(),&gr_x[0],&gr_y[0],&gr_xe[0],&gr_ye[0]);
	auto *gr_err = new TGraphErrors(gr_x.size(),&gr_x[0],&gr_y2[0],&gr_xe[0],&gr_xe[0]);

	for(int ip=1; ip<_nbins-1; ip++){
		auto _x = histo->GetBinCenter(ip);
		auto _y = (Double_t)gr_val->Eval(x[ip-1]);//, 0, "S"));
		auto _ye= (Double_t)gr_err->Eval(x[ip-1]);//, 0, "S"));
		
		for(int j=1; j<(int)_y; j++){
			result->Fill(_x);
		}
		
		//result->SetBinError(result->FindBin(_x),_ye);
		/*
		if(y[ip-1]==0 && x[ip-1]<refMult_max && ip>2){
			result->Fill(x[ip-1],  (Double_t)gr_val->Eval(x[ip-1]));//, 0, "S"));
			result->SetBinError(ip,(Double_t)gr_err->Eval(x[ip-1]));//, 0, "S"));
		}else{
			result->Fill(x[ip-1],y[ip-1]);
			result->SetBinError(ip,ye[ip-1]);
		}
		*/
	}

	return x.size()==0 ? nullptr : result;
}

TH1D* TH1_EvalZeroBin(TH1D *histo, double refMult_max = 350){
	
	int _nbins  = histo->GetXaxis()->GetNbins();
	int _binMin = histo->GetXaxis()->GetXmin();
	int _binMax = histo->GetXaxis()->GetXmax();

	TH1D *result = new TH1D(Form("h1_%s_%.1f",histo->GetName(),refMult_max),"",_nbins,(float)_binMin,(float)_binMax);

	std::vector<double> x;
	std::vector<double> y;
	std::vector<double> xe;
	std::vector<double> ye;
	
	std::vector<double> gr_x;
	std::vector<double> gr_y;
	std::vector<double> gr_y2;
	std::vector<double> gr_xe;
	std::vector<double> gr_ye;

	for(int ip=1; ip<_nbins-1; ip++){
		auto _x = histo->GetBinCenter(ip);
		auto _y = histo->GetBinContent(histo->FindBin(_x));
		auto _ye= histo->GetBinError(histo->FindBin(_x));
		x.push_back(_x);
		xe.push_back(0.);
		y.push_back(_y);
		ye.push_back(_ye);
		
		if(_y>0){
			gr_x.push_back(_x);
			gr_xe.push_back(0.);
			gr_y.push_back(_y);
			gr_y2.push_back(_ye);
			gr_ye.push_back(_ye);
		}
	}

	auto *gr_val = new TGraphErrors(gr_x.size(),&gr_x[0],&gr_y[0],&gr_xe[0],&gr_ye[0]);
	auto *gr_err = new TGraphErrors(gr_x.size(),&gr_x[0],&gr_y2[0],&gr_xe[0],&gr_xe[0]);

	for(int ip=1; ip<_nbins-1; ip++){
		auto _x = histo->GetBinCenter(ip);
		auto _y = (Double_t)gr_val->Eval(x[ip-1]);//, 0, "S"));
		auto _ye= (Double_t)gr_err->Eval(x[ip-1]);//, 0, "S"));
		
		result->Fill(_x, _y);
		result->SetBinError(result->FindBin(_x),_ye);
		/*
		if(y[ip-1]==0 && x[ip-1]<refMult_max && ip>2){
			result->Fill(x[ip-1],  (Double_t)gr_val->Eval(x[ip-1]));//, 0, "S"));
			result->SetBinError(ip,(Double_t)gr_err->Eval(x[ip-1]));//, 0, "S"));
		}else{
			result->Fill(x[ip-1],y[ip-1]);
			result->SetBinError(ip,ye[ip-1]);
		}
		*/
	}

	return x.size()==0 ? nullptr : result;
}

TH1D* TH1_reweight(TH1D *histo_ref, TH1D *histo_in){
	
	int _nbins  = histo_ref->GetXaxis()->GetNbins();
	int _binMin = histo_ref->GetXaxis()->GetXmin();
	int _binMax = histo_ref->GetXaxis()->GetXmax();

	TH1D *result = new TH1D();
	TH1D *result2 = new TH1D(Form("rew_%s_%s",histo_in->GetName(),histo_ref->GetName()),"",_nbins,(float)_binMin,(float)_binMax);
	histo_in->Copy(*result);
	result->Divide(histo_ref);

	for(int ip=0; ip<_nbins-1; ip++){
		double _x = result->GetBinCenter(ip);
		double w = result->GetBinContent(result->FindBin(_x));
		if(w==0 || TMath::IsNaN(w)) w=1.;
		double y = histo_in->GetBinContent(histo_in->FindBin(_x));
		double ye = histo_in->GetBinError(histo_in->FindBin(_x));
		result2->Fill(_x,1./w);
		result2->SetBinError(_x,ye);
	}

	result2->SetLineColor(histo_in->GetLineColor());
	result2->SetLineWidth(histo_in->GetLineWidth());

	return result2;
}



std::vector<double> FitHMultUser(TH1D *_hist, TFile *outFile, double min, double max){
	
	double min_Mult = min;
	double max_Mult = max;
	double wight_mult = (max-min)>=90 ? 90 : (max-min-10) ;
	//double wight_mult = 40;
	int nStep = (max_Mult - min_Mult - wight_mult) + 1;

	std::map<double,std::vector<double>> _param = {{9999.,{9999.,9999.,9999.,9999.,9999.}}};

	outFile->cd();
	for(int i=0; i<nStep; i++){
		double _min = (double)i + min_Mult;
		double _max = _min + wight_mult;
		auto f1_fit = new TF1(Form("_fit_%s",_hist->GetName()),"[0] * ROOT::Math::erf(-1.*[1]*(x-[2])) + [0]",90,250);
		f1_fit->SetParameter(0,3.58333e-03);
		f1_fit->SetParameter(1,2.16240e-02);
		f1_fit->SetParameter(2,min+wight_mult);
		_hist->Fit(f1_fit,"QRM","",_min,_max);
		_hist->Fit(f1_fit,"QRM","",_min,_max);
		_param.insert({f1_fit->GetChisquare(),{_min,_max,f1_fit->GetParameter(0),f1_fit->GetParameter(1),f1_fit->GetParameter(2)}});
		//std::cout<<_min<<"\t"<<_max<<"\t"<<f1_fit->GetChisquare()<<"\t"<<f1_fit->GetParameter(0)<<"\t"<<f1_fit->GetParameter(1)<<"\t"<<f1_fit->GetParameter(2)<<"\n";
		//_hist->Write();
	}

	auto min_it = std::min_element(_param.begin(), _param.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
	
	if(_param.size()>1) return _param.at(min_it->first);
	
	return _param[0];
}


void parRunByRun(double RunIdStart, double RunIdStop, std::vector<double> runId, std::vector<double> vec, double &h, double &s, double &e){

	double H=0.;
	double E=0.;
	double W=0.;
	h=0.;
	s=0.;
	e=0.;

	for(int i=0; i<vec.size(); i++){
		if(runId[i]<RunIdStart || runId[i]>RunIdStop)continue;
		H = H + vec[i];
		E = E + vec[i] * vec[i];
		W = W + 1;
	}
	h = H/W;
	s = sqrt( E/W - h*h );
	e = s / sqrt(W);
}

void parRunByRun(std::vector<double> runId, std::vector<double> vec, double &h, double &s, double &e){

	double H=0.;
	double E=0.;
	double W=0.;
	h=0.;
	s=0.;
	e=0.;

	for(int i=0; i<vec.size(); i++){
		if(runId[i]<_RunIdStart || runId[i]>_RunIdStop)continue;
		H = H + vec[i];
		E = E + vec[i] * vec[i];
		W = W + 1;
	}
	h = H/W;
	s = sqrt( E/W - h*h );
	e = s / sqrt(W);
}

void parRunByRun2(double RunIdStart, double RunIdStop, std::vector<double> runId, std::vector<double> vec, double &h, double &s, double &e){

	double H=0;
	double S=0;
	double W=0;

	for(int i=0; i<runId.size(); i++){
		H = H + vec[i]*vec[i];
		W = W + 1;
	}
	H = sqrt(H / W);
	
	for(int i=0; i<runId.size(); i++){
		S = S + pow(H - vec[i],2);
	}

	h = H;
	s = sqrt(S / W);
	e = 0.;

}

void SetVectorsInsideRMS(double RunIdStart, double RunIdStop, std::vector<double> x_input, std::vector<double> y_input, std::vector<double> &x, std::vector<double> &y, std::vector<double> &x_err, std::vector<double> &y_err, double _mean, double _rms){
	x.clear();
	y.clear();
	x_err.clear();
	y_err.clear();

	for(int i=0; i<x_input.size(); i++){

		if(x_input[i]<RunIdStart || x_input[i]>RunIdStop)continue;

		double lo = _mean - nSigma * _rms;
		double hi = _mean + nSigma * _rms;
		
		if(y_input[i] < hi && y_input[i] > lo){
			x.push_back(x_input[i]);
			x_err.push_back(0.);
			y.push_back(y_input[i]);
			y_err.push_back(0.);
		}
	}
}

void SetVectorsOutOfRMS(double RunIdStart, double RunIdStop, std::vector<double> x_input, std::vector<double> y_input, std::vector<double> &x, std::vector<double> &y, std::vector<double> &x_err, std::vector<double> &y_err, double _mean, double _rms){
	x.clear();
	y.clear();
	x_err.clear();
	y_err.clear();

	for(int i=0; i<x_input.size(); i++){

		if(x_input[i]<RunIdStart || x_input[i]>RunIdStop)continue;

		double lo = _mean - nSigma * _rms;
		double hi = _mean + nSigma * _rms;
		
		if(y_input[i] > hi || y_input[i] < lo){
			x.push_back(x_input[i]);
			x_err.push_back(0.);
			y.push_back(y_input[i]);
			y_err.push_back(0.);
		}
	}
}

TH1D *GetTH1(TFile *file, DrawParameters par)
{
	auto *histo = (TH1D*)file->Get((par.Suffix+par.Name).c_str());
	if(!histo){
		std::cout<<"Not found:\t"<<(par.Suffix+par.Name)<<std::endl;
		return nullptr;
	}
	
	//if(par.Name=="h1_vtx_ndf")histo->Rebin(2);
	if(par.Marker!=0)histo->SetMarkerStyle(par.Marker);
	if(par.Color !=0)histo->SetMarkerColor(par.Color);
	if(par.Color !=0)histo->SetLineColor(par.Color);
	if(par.Width !=0)histo->SetLineWidth(par.Width);
	if(par.RangeX.size()==2)histo->GetXaxis()->SetRangeUser(par.RangeX[0],par.RangeX[1]);
	if(par.RangeY.size()==2)histo->GetYaxis()->SetRangeUser(par.RangeY[0],par.RangeY[1]);
	histo->GetYaxis()->SetTitle("counts");
	
	return histo;
}

TH2D *GetTH2(TFile *file, DrawParameters par)
{
	auto *histo = (TH2D*)file->Get((par.Suffix+par.Name).c_str());
	if(!histo){
		std::cout<<"Not found:\t"<<(par.Suffix+par.Name)<<std::endl;
		return nullptr;
	}
	
	if(par.RangeX.size()==2)histo->GetXaxis()->SetRangeUser(par.RangeX[0],par.RangeX[1]);
	if(par.RangeY.size()==2)histo->GetYaxis()->SetRangeUser(par.RangeY[0],par.RangeY[1]);
	
	return histo;
}

TH2D *GetTH2D_M2(DrawParameters obj_par, TFile *inFile, string _TOF, string _charge, float min_p, float max_p){

	int fP_minBin	= min_p / fP_step - 1.;
	int fP_maxBin	= max_p / fP_step - 1.;

	std::vector<TH2D*> histoM2;

	for(int i = fP_minBin; i <= fP_maxBin; i++ ){
		histoM2.push_back(new TH2D());
		histoM2.back() = (TH2D*)inFile->Get(Form("ddM2P_h2_RunId_tr_m2_%s_p%i%s",_TOF.c_str(),i,_charge.c_str() )) ;

		if(!histoM2.back()){
			std::cout<<"Not found:\t"<<Form("ddM2P_h2_RunId_tr_m2_%s_p%i%s",_TOF.c_str(),i,_charge.c_str() )<<std::endl;
			return nullptr;
		}
	}

	for(int i=1; i<histoM2.size(); i++){
		histoM2[0]->Add(histoM2[i]);
	}

	return histoM2[0];
}

TH3D *GetTH3(TFile *file, DrawParameters par)
{
	auto *histo = (TH3D*)file->Get((par.Suffix+par.Name).c_str());
	if(!histo){
		std::cout<<"Not found:\t"<<(par.Suffix+par.Name)<<std::endl;
		return nullptr;
	}
	
	if(par.RangeX.size()==2)histo->GetXaxis()->SetRangeUser(par.RangeX[0],par.RangeX[1]);
	if(par.RangeY.size()==2)histo->GetYaxis()->SetRangeUser(par.RangeY[0],par.RangeY[1]);
	//if(par.RangeY.size()==2)histo->GetYaxis()->SetRangeUser(par.RangeY[0],par.RangeY[1]);
	
	return histo;
}

TProfile *GetTProfile1D(TFile *file, DrawParameters par)
{
	auto *histo = (TProfile*)file->Get((par.Suffix+"p1_runId_"+par.Name).c_str());
	if(!histo){
		std::cout<<"Not found:\t"<<(par.Suffix+par.Name)<<std::endl;
		return nullptr;
	}
	
	histo->SetMarkerStyle(8);
	histo->SetMarkerColor(2);
	histo->SetLineColor(2);
	histo->SetLineWidth(2);
	if(par.RangeY.size()==2)histo->GetYaxis()->SetRangeUser(par.RangeY[0],par.RangeY[1]);
	if(par.RangeX.size()==2)histo->GetXaxis()->SetRangeUser(par.RangeX[0],par.RangeX[1]);

	return histo;
}

void PrintBadRuns(TFile *file_inFile, std::string suf){;

	sort(BadRunsGlobal.begin(), BadRunsGlobal.end());
	BadRunsGlobal.erase(std::unique(BadRunsGlobal.begin(), BadRunsGlobal.end()), BadRunsGlobal.end());
	
	std::cout<<"\n\n";
	for(auto num : BadRunsGlobal){
		std::cout<<num<<", ";
	}
	std::cout<<"\n";

	double Mult_bad = 0;
	TH2D *h2 = GetTH2(file_inFile,{"h2_RunId_nTracks",suf,0,4,2.,{},{0,500}});
	TH1D *hist;
	for(int i=0; i<BadRunsGlobal.size(); i++){

		hist = (TH1D*)h2->ProjectionY(Form("run_%s_%i",h2->GetName(),i),h2->GetXaxis()->FindBin(BadRunsGlobal[i]),h2->GetXaxis()->FindBin(BadRunsGlobal[i]));		
		if(hist->GetEntries()==0) continue;
		if(hist->Integral()==0) continue;
		Mult_bad=Mult_bad+(double)hist->GetEntries();
	}
	std::cout<<Mult_bad<<"\n";
}

void makeplotstyle(){
  
    TStyle *mystyle = new TStyle("PlottingInStyle", "Style for Summary Plots");
    //mystyle->GetFrame()->SetFillColor(21);
    //mystyle->GetFrame()->SetBorderSize(115);
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
    mystyle->SetEndErrorSize(8);
    
    //mystyle->SetGridx();

    TFile f("style.root", "RECREATE");
    f.cd();
    mystyle->Write();
    f.Close();
}


/*
latex.DrawLatex(xMin + 0.05*xH, AxisYMin + 0.9*AxisYH ,"BM@N RUN8");
latex.DrawLatex(xMin + 0.7*xH, AxisYMin + 0.9*AxisYH, Form("#mu=%.4f",(float)h[0]));
latex.DrawLatex(xMin + 0.7*xH, AxisYMin + 0.84*AxisYH ,	Form("#sigma(#mu)=%.4f",(float)s[0]));
latex.DrawLatex(xMin + 0.7*xH, AxisYMin + 0.78*AxisYH ,	Form("#sigma(#Delta#mu)=%.4f",(float)s[1]));
std::string BadRuns = "";
for(auto dd : BadRuns_runId){
	BadRuns = BadRuns + Form("%.0f",dd) + ", ";
}
latex.DrawLatex(xMin + 0.05*xH, AxisYMin + 0.8*AxisYH ,	Form("%s",BadRuns.c_str()));
*/