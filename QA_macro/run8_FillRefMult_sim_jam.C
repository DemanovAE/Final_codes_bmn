#include <string>
#include <iostream>
#include <fstream> 

using namespace ROOT;
using namespace ROOT::Math;
using namespace ROOT::RDF;

const double RunIdMin = 6600;
const double RunIdMax = 8500;
int RunIdBins= (int)(RunIdMax - RunIdMin);

struct HistoParameter
{
	std::string fName;
	std::string fFillObj;
	std::string fAxisTitle;
	int    fNBins;
	double  fMin;
	double  fMax;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  {Name histo, 		{Name histo, 		name of the column,  		Axis title, 				n bins, 	min, 		max}   //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::map< std::string, HistoParameter > QA_variable{	
	// Event parametrs
	{"b",							{"b",							"b",											";b",								200,		0,			20}},
	{"nTracks",				{"nTracks",				"track_multiplicity",			";N tracks",				1000,		0,			1000}},
	{"nTracksChM",		{"nTracksChM",		"track_multiplicity_ChM",	";N tracks",				1000,		0,			1000}},
	{"nTracksSts",		{"nTracksSts",		"sts_track_multiplicity",	";N tracks",				1000,		0,			1000}},
	{"nTracksSim",		{"nTracksSim",		"track_multiplicity_sim",	";N tracks",				1000,		0,			1000}},
	{"nTracksSim2",		{"nTracksSim2",		"track_multiplicity_sim2",";N tracks",				1000,		0,			1000}},
	{"nTracksSimPiKP",{"nTracksSimPiKP","track_multiplicity_sim_PiKP",";N tracks",		1000,		0,			1000}},
	{"nTracksSimAll",	{"nTracksSimAll",	"sim_track_multiplicity",	";N tracks",				1000,		0,			1000}},
	{"nTracksCh",			{"nTracksCh",			"track_multiplicity_ch",	";N tracks",				1000,		0,			1000}},
	{"nTracksGT",			{"nTracksGT",			"track_multiplicity_gt",	";N tracks",				1000,		0,			1000}},
	{"vtx_x",					{"vtx_x",					"vtxX",										";Vtx_{X} (cm)",		1000,		-5.,		5.}},
	{"vtx_y",					{"vtx_y",					"vtxY",										";Vtx_{Y} (cm)",		1000,		-5.,		5.}},
	{"vtx_z",					{"vtx_z",					"vtxZ",										";Vtx_{Z} (cm)",		2000,		-4.,		4.}},
	{"vtx_r",					{"vtx_r",					"vtxR",										";Vtx_{R} (cm)",		1000,		0.,			10.}},
	{"simvtx_x",			{"simvtx_x",			"simVtxX",								";Vtx_{X} (cm)",		1000,		-5.,		5.}},
	{"simvtx_y",			{"simvtx_y",			"simVtxY",								";Vtx_{Y} (cm)",		1000,		-5.,		5.}},
	{"simvtx_z",			{"simvtx_z",			"simVtxZ",								";Vtx_{Z} (cm)",		2000,		-4.,		4.}},
	{"simvtx_r",			{"simvtx_r",			"simvtxR",								";Vtx_{R} (cm)",		1000,		0.,			10.}},
	//sim tr
	{"tr_dca_x",		{"tr_dca_x",		"dcaX",					";DCA_{x} (cm)",			800,		-50,		50}},
	{"tr_dca_y",		{"tr_dca_y",		"dcaY",					";DCA_{y} (cm)",			800,		-50,		50}},
	{"tr_dca_r",		{"tr_dca_r",		"dcaR",					";DCA_{y} (cm)",			800,		-50,		50}},
	{"tr_nhits",			{"tr_nhits", 			"trNhits",								";N hits",					40,			0,			40}},
	{"tr_ChSim",			{"tr_ChSim", 			"simCharge",							";ch sim",					20,			-10,		10}},
	{"tr_sim_pT",			{"tr_sim_pT", 		"sim_pT",									";p_{T} (GeV/c)",		1000,		0,			10}},
	{"tr_sim_p",			{"tr_sim_p", 			"sim_p",									";p (GeV/c)",				2000,		-40,		40}},
	{"tr_sim_eta",		{"tr_sim_eta", 		"sim_eta",								";#eta",						1000,		0,			10}},
	{"tr_sim_phi",		{"tr_sim_phi", 		"sim_phi",								";#phi",						1000,		-5,			5}},
	//reco tr
	{"tr_ChS",				{"tr_Ch",	 				"trCharge",								";ch reco",					20,			-10,		10}},
	{"tr_pT",					{"tr_pT", 				"pT",											";p_{T} (GeV/c)",		1000,		0,			10}},
	{"tr_p",					{"tr_p", 					"p",											";p (GeV/c)",				2000,		-40,		40}},
	{"tr_eta",				{"tr_eta", 				"eta",										";#eta",						1000,		0,			10}},
	{"tr_phi",				{"tr_phi", 				"phi",										";#phi",						1000,		-5,			5}},
	// fhcall + hodo + scwall
	{"hodo_q",				{"hodo_q",				"hodoSumQ",								";Hodo Q^{2}", 			5000,		0,			20000}},
	{"hodo_q_cent",		{"hodo_q_cent",		"hodoIsCentral",					";Hodo Q^{2}", 			5000,		0,			20000}},
	{"hodo_q_per",		{"hodo_q_per",		"hodoIsPeripheral",				";Hodo Q^{2}", 			5000,		0,			20000}},
	{"scwall_q",			{"scwall_q",			"scwallSumQ",							";ScWall Q_{tot}", 	1000,		0,			1000}},
	{"fhcal&hodo_e",	{"fhcal&hodo_e",	"fhcalhodoSumE",					";E_{tot} (MeV)", 	5000,		0,			20000}},
	{"fhcal_e",				{"fhcal_e", 			"fhcalSumE",							";E_{tot} (MeV)",		5000,		0,			20000}}
};

const std::map< std::string, HistoParameter > QA_variable_2{
	{"nTracks_dca20",			{"nTracks_dca20",		"track_multiplicity_dca20",	";N tracks",				1000,				0,			1000}},
	{"nTracks_dca10",			{"nTracks_dca10",		"track_multiplicity_dca10",	";N tracks",				1000,				0,			1000}},
	{"nTracks_dca5",			{"nTracks_dca5",		"track_multiplicity_dca5",	";N tracks",				1000,				0,			1000}},
	{"nTracks_dca3",			{"nTracks_dca3",		"track_multiplicity_dca3",	";N tracks",				1000,				0,			1000}},
	{"nTracks_eta05",			{"nTracks_eta05",		"track_multiplicity_eta05",	";N tracks",				1000,				0,			1000}},
	{"nTracks_eta15",			{"nTracks_eta15",		"track_multiplicity_eta15",	";N tracks",				1000,				0,			1000}},
	{"nTracks_eta14",			{"nTracks_eta14",		"track_multiplicity_eta14",	";N tracks",				1000,				0,			1000}},
	{"nTracks_eta13",			{"nTracks_eta13",		"track_multiplicity_eta13",	";N tracks",				1000,				0,			1000}},
	{"nTracks_eta24",			{"nTracks_eta24",		"track_multiplicity_eta24",	";N tracks",				1000,				0,			1000}},
	{"nTracks_eta16",			{"nTracks_eta16",		"track_multiplicity_eta16",	";N tracks",				1000,				0,			1000}},
	{"nTracks_ch0",				{"nTracks_ch0",			"track_multiplicity_ch0",	";N tracks",				1000,				0,			1000}},
	{"nTracks_ch1",				{"nTracks_ch1",			"track_multiplicity_ch1",	";N tracks",				1000,				0,			1000}}
};

const std::vector<std::vector< std::string >> QA_MultDemenPlot{	
	{"tr_sim_p",	"Weight:simIsPrimary"},
	{"tr_sim_pT",	"Weight:simIsPrimary"},
	{"tr_sim_eta",	"Weight:simIsPrimary"},
	{"tr_sim_phi",	"Weight:simIsPrimary"},

	{"vtx_x",		"nTracks"},
	{"vtx_y",		"nTracks"},
	{"vtx_z",		"nTracks"},
	{"vtx_r",		"nTracks"},
	
	{"simvtx_x",	"nTracksSim"},
	{"simvtx_y",	"nTracksSim"},
	{"simvtx_z",	"nTracksSim"},
	{"simvtx_r",	"nTracksSim"},

	{"vtx_x",		"vtx_y"},
	{"simvtx_x",	"simvtx_y"},

	{"vtx_z",		"vtx_r"},
	{"vtx_z",		"vtx_x"},
	{"vtx_z",		"vtx_y"},
	{"vtx_x",		"vtx_y"},
	{"simvtx_z",	"simvtx_r"},
	{"simvtx_z",	"simvtx_x"},
	{"simvtx_z",	"simvtx_y"},
	{"simvtx_x",	"simvtx_y"},

	{"b", 	"nTracks"},
	{"b", 	"hodo_q"},
	{"b", 	"fhcal_e"},
	{"nTracks", 			"b"},
	{"nTracks", 			"hodo_q"},
	{"nTracks", 			"scwall_q"},
	{"nTracks", 			"fhcal_e"},
	{"nTracksCh", 			"b"},
	{"nTracksCh", 			"hodo_q"},
	{"nTracksCh", 			"scwall_q"},
	{"nTracksCh", 			"fhcal_e"},
	{"nTracksSim", 		"b"},
	{"nTracksSim", 		"hodo_q"},
	{"nTracksSim", 		"scwall_q"},
	{"nTracksSim", 		"fhcal_e"},
	{"nTracksSim2",		"b"},
	{"nTracksSim2",		"hodo_q"},
	{"nTracksSim2",		"scwall_q"},
	{"nTracksSim2",		"fhcal_e"},
	{"nTracksSimPiKP","b"},
	{"nTracksSimPiKP","hodo_q"},
	{"nTracksSimPiKP","scwall_q"},
	{"nTracksSimPiKP","fhcal_e"},
	{"fhcal_e",		"b"},
	{"hodo_q",		"b"},
	{"fhcal_e",		"hodo_q"}
};

const ROOT::RDF::TProfile1DModel SetProfile1Dobj(std::string _prefix, const HistoParameter &_par, const HistoParameter &_par_fill){
	return { (_prefix + "pr1_" + _par.fName + "_" + _par_fill.fName).c_str(),_par.fAxisTitle.c_str(), _par.fNBins, _par.fMin, _par.fMax };
}

const ROOT::RDF::TH1DModel SetH1obj(std::string _prefix, const HistoParameter &_par){
	return { (_prefix + "h1_" + _par.fName).c_str(),_par.fAxisTitle.c_str(), _par.fNBins, _par.fMin, _par.fMax };
}

const ROOT::RDF::TH2DModel SetH2obj(std::string _prefix, const HistoParameter &_parX, const HistoParameter &_parY){
	return { (_prefix+"h2_" + _parX.fName + "_" + _parY.fName).c_str(), ( _parX.fAxisTitle + _parY.fAxisTitle).c_str(), _parX.fNBins, _parX.fMin, _parX.fMax, _parY.fNBins, _parY.fMin, _parY.fMax };
}

const ROOT::RDF::TH3DModel SetH3obj(std::string _prefix, const HistoParameter &_parX, const HistoParameter &_parY, const HistoParameter &_parZ){
	return { (_prefix+"h3_" + _parX.fName + "_" + _parY.fName + "_" + _parZ.fName).c_str(), ( _parX.fAxisTitle + _parY.fAxisTitle + _parZ.fAxisTitle).c_str(), _parX.fNBins, _parX.fMin, _parX.fMax, _parY.fNBins, _parY.fMin, _parY.fMax, _parZ.fNBins, _parZ.fMin, _parZ.fMax };
}

template<typename T>
void WriteInFile(T& vector, TFile *file){
	file->cd();
	std::for_each( vector.begin(), vector.end(), []( auto h1 ){ h1->Write(); } );
}

template<typename T>
void AddStandartHisto(T& dd, std::string prefix, TFile *file){
	
	std::vector<ROOT::RDF::RResultPtr<::TH1D>> histo1;
	std::vector<ROOT::RDF::RResultPtr<::TH2D>> histo2;
	std::vector<ROOT::RDF::RResultPtr<::TH3D>> histo3;
	std::vector<ROOT::RDF::RResultPtr<::TProfile>> prof1;

	std::cout<<prefix<<std::endl;

	// Fill TH1D histo all QA_variable
	for(auto qa_obj : QA_variable){
		//std::cout<<qa_obj.first<<"\n";
		histo1.push_back( dd.Histo1D( SetH1obj(prefix,qa_obj.second) , qa_obj.second.fFillObj) );
	}

	if(prefix=="VtxR1_VtxZ020_DCA_" || prefix=="VtxR1_VtxZ020_DCA_bc1fd_" || prefix=="VtxR1_VtxZ010_DCA_____"){
		for(auto qa_obj : QA_variable_2){
			//std::cout<<qa_obj.first<<"\n";
			histo1.push_back( dd.Histo1D( SetH1obj(prefix,qa_obj.second) , qa_obj.second.fFillObj) );
		}
	}

	// Fill TH1D, TH2D, TH3D and TProfile1D with and without weight
	for(auto qa_obj : QA_MultDemenPlot){
		
		// Fill TH1D,TH2D,TH3D without weight 
		if(strncmp(qa_obj.back().c_str(),"Weight:",7) != 0){
			switch( (int)qa_obj.size() ){
				case 2:{
					histo2.push_back( dd.Histo2D( SetH2obj(prefix,QA_variable.at(qa_obj[0]),QA_variable.at(qa_obj[1])), 
						QA_variable.at(qa_obj[0]).fFillObj, QA_variable.at(qa_obj[1]).fFillObj));
					break;
				}
				case 3:{
					histo3.push_back( dd.Histo3D( SetH3obj(prefix,QA_variable.at(qa_obj[0]),QA_variable.at(qa_obj[1]),QA_variable.at(qa_obj[2])), 
						QA_variable.at(qa_obj[0]).fFillObj, QA_variable.at(qa_obj[1]).fFillObj, QA_variable.at(qa_obj[2]).fFillObj));
					break;
				}
			}
		}
		
		// Fill TH1D,TH2D,TH3D with weight
		if(strncmp(qa_obj.back().c_str(),"Weight:",7) == 0){
			
			std::string _fw = qa_obj.back();
			_fw.erase(_fw.begin(), _fw.begin()+7);
			
			switch( (int)qa_obj.size() ){
				case 2:{
					histo1.push_back( dd.Histo1D( SetH1obj(prefix,QA_variable.at(qa_obj[0])), QA_variable.at(qa_obj[0]).fFillObj, _fw) );
					histo1.back()->SetName(Form("%s_W=%s",histo1.back()->GetName(),_fw.c_str()));
					break;
				}
				case 3:{
					histo2.push_back( dd.Histo2D( SetH2obj(prefix,QA_variable.at(qa_obj[0]),QA_variable.at(qa_obj[1])), 
						QA_variable.at(qa_obj[0]).fFillObj, QA_variable.at(qa_obj[1]).fFillObj, _fw));
					histo2.back()->SetName(Form("%s_W=%s",histo2.back()->GetName(),_fw.c_str()));
					break;
				}
				case 4:{
					histo3.push_back( dd.Histo3D( SetH3obj(prefix,QA_variable.at(qa_obj[0]),QA_variable.at(qa_obj[1]),QA_variable.at(qa_obj[2])), 
						QA_variable.at(qa_obj[0]).fFillObj, QA_variable.at(qa_obj[1]).fFillObj, QA_variable.at(qa_obj[2]).fFillObj,_fw));
					histo3.back()->SetName(Form("%s_W=%s",histo3.back()->GetName(),_fw.c_str()));
					break;
				}
			}
		}

	}
	
  //dd.Foreach([](ULong64_t evtId){if (evtId % 1000 == 0) cout << "\r" << evtId;}, {"rdfentry_"}); // progress display

	WriteInFile(histo1,file);
	WriteInFile(histo2,file);
	WriteInFile(histo3,file);

}

// Fill slice M2 
template<typename T>
void AddHistoM2vsRunId(T& dd, std::string prefix, std::string NameM2, std::string pqCharge, TFile *file){
	
	std::vector<ROOT::RDF::RResultPtr<::TH2D>> histo2;
	std::vector<std::string> NameFillPar;
	
	std::cout<<prefix<<"\t"<<NameM2<<std::endl;

	double fP_start		= 0.1;
	double fP_finish	= 5.1;
	double fP_step  	= 0.2;
	int    fP_nbins		= (int)(abs( fP_finish - fP_start)/fP_step);
	string fChName = "P";
	if(pqCharge=="pq<0") fChName = "M";

	auto ddM2 = dd.Define("cut_pAll", Form("abs(p)>%f && abs(p)<%f && %s && tr_dca_r<5",fP_start,fP_finish,pqCharge.c_str()))
   	  					.Define(Form("%s_cut_pAll",NameM2.c_str()),Form("%s[cut_pAll]",NameM2.c_str()));

	for(int ip=0; ip<fP_nbins; ip++){
		
		double fPmin = fP_start + fP_step * ip;
		double fPmax = fP_start + fP_step * (ip+1);
		std::string fCut  			= Form("cut_p%i",ip);
		std::string fCutFormul		= Form("abs(p)>%f && abs(p)<%f && %s && tr_dca_r<5",fPmin,fPmax, pqCharge.c_str());
		std::string fM2WithCut 		= Form("%s_p%i_%s",	NameM2.c_str(),ip,fChName.c_str());
		std::string fM2WithCutFormul= Form("%s[%s]",	NameM2.c_str(),fCut.c_str());
		//std::cout<<fCut<<"\t"<<fCutFormul<<"\t"<<fM2WithCut<<"\t"<<fM2WithCutFormul<<"\n";

		ddM2 = ddM2.Define(fCut, fCutFormul)
				   .Define(fM2WithCut, fM2WithCutFormul);
   	  	NameFillPar.push_back(fM2WithCut);
	}

	histo2.push_back(ddM2.Histo2D({Form("%sh2_RunId_m2%s_pAll_%s",prefix.c_str(), NameM2.c_str(), fChName.c_str()),";run ID;m^{2} (GeV^{2}/c^{4})", RunIdBins, RunIdMin, RunIdMax, 750, -5., 10.}, "runId", Form("%s_cut_pAll",NameM2.c_str())) );
	for(int ip=0; ip<fP_nbins; ip++){
		histo2.push_back( ddM2.Histo2D({Form("%sh2_RunId_tr_m2_%s_p%i_%s",prefix.c_str(), NameM2.c_str(), ip, fChName.c_str()), 
											  ";run ID;m^{2} (GeV^{2}/c^{4})", RunIdBins, RunIdMin, RunIdMax, 750, -5., 10.}, 
											  "runId", NameFillPar[ip]) );
	}

	WriteInFile(histo2,file);

}

void RemoveBadRuns(std::vector<int> &_Runs, std::vector<int> _BadRUns){
	for(auto _run : _BadRUns){
		_Runs.erase(remove(_Runs.begin(), _Runs.end(), _run), _Runs.end());
	}
}


int getCharge(int pdg)
{
  if(pdg<1000000)
    return TDatabasePDG::Instance()->GetParticle(pdg)->Charge();
  else 
    return -999;
}

RVec<short> simCharge (const RVec<int> pdg)
{
  vector<short> ch;
  for (auto &p:pdg)
    ch.push_back(getCharge(p));
  return ch;
}

void run8_FillRefMult_sim_jam( std::string str_in_list, std::string out_file_name="qa.root"){
	
	TStopwatch timer1;
  	timer1.Start();

	TFileCollection collection( "collection", "", str_in_list.c_str() );
	auto* chain = new TChain( "t" );
	chain->AddFileInfoList( collection.GetList() );
	ROOT::RDataFrame d( *chain );

	const float PROTON_M = 0.93827208;
  const float PION_M = 0.13957061;
  const float Y_CM = 1.15141;
  const float DEUTERON_M = 1.875612928;
  const float TRITON_M = 2.7930000;
	const float FHCAL_Z = 980; // cm

	double VtxX_corr = -0.73;
	double VtxY_corr =  0.23;
	double VtxZ_corr = -0.0945;

  // additional functions

	auto BRCut = []( ROOT::VecOps::RVec<unsigned int> map ){ return map[0] & (1<<5); };	//
	auto ARCut = []( ROOT::VecOps::RVec<unsigned int> map ){ return map[0] & (1<<7); }; //
	auto ARCutMinB = []( ROOT::VecOps::RVec<unsigned int> map ){ return map[0] & (1<<5); }; // AR - 5 бит minimum-bias

	const auto function_fhcal_x = 
	  [FHCAL_Z]
	  ( ROOT::VecOps::RVec<std::vector<float>> vec_param ){
	      std::vector<float> vec_x{};
	      vec_x.reserve( vec_param.size() );
	      for( auto par : vec_param ){
	        auto x = par.at(0);
	        auto z = par.at(2);
	        auto tx = par.at(3);
	        auto dz = FHCAL_Z - z;
	        auto dx = tx * dz;
	        vec_x.push_back( x+dx );
	      }
	      return vec_x;
	    };
	const auto function_fhcal_y = 
	  [FHCAL_Z]
	  ( ROOT::VecOps::RVec<vector<float>> vec_param ){
	      std::vector<float> vec_y{};
	      vec_y.reserve( vec_param.size() );
	      for( auto par : vec_param ){
	        auto y = par.at(1);
	        auto z = par.at(2);
	        auto ty = par.at(4);
	        auto dz = FHCAL_Z - z;
	        auto dy = ty * dz;
	        vec_y.push_back( y+dy );
	      }
	      return vec_y;
	    };

	const auto dca_function = [](std::vector<float> vec_x, std::vector<float> vec_y){
		    std::vector<float> vec_r{};
		    vec_r.reserve(vec_x.size());
		    for (int i=0; i<vec_x.size(); ++i) {
		      auto x = vec_x.at(i);
		      auto y = vec_y.at(i);
		      auto r = std::sqrt( x*x + y*y );
		      vec_r.push_back(r);
		    }
		    return vec_r;
		};

	const auto Rapidity = [Y_CM]( ROOT::RVecF vec_pz, ROOT::RVecF vec_pq, ROOT::RVecF vec_m2 ){
			ROOT::RVecF vec_y{};
			vec_y.reserve( vec_pz.size() );
			for( int i=0; i<vec_pz.size(); ++i ){
				auto pz = vec_pz.at(i);
				auto p = vec_pq.at(i);
				auto E = sqrt( p*p + vec_m2.at(i) );
				auto y = 0.5 * log( ( E + pz )/( E - pz ) ) - Y_CM;
				vec_y.push_back( y );
			}
			return vec_y;
		};

	const auto rapidity_generator = []( auto particle_m, auto y_cm ){
	    return 
	    [particle_m, y_cm]( ROOT::RVecF vec_pz, ROOT::RVecF vec_pq ){
	      ROOT::RVecF vec_y{};
	      vec_y.reserve( vec_pz.size() );
	      for( int i=0; i<vec_pz.size(); ++i ){
	        auto pz = vec_pz.at(i);
	        auto p = vec_pq.at(i);
	        auto E = sqrt( p*p + particle_m*particle_m );
	        auto y = 0.5 * log( ( E + pz ) / ( E - pz ) ) - y_cm;
	        vec_y.push_back( y );
	      }
	      return vec_y;
	    };
	};

    const auto M2Function = []( ROOT::VecOps::RVec<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >> vec_mom, ROOT::VecOps::RVec<double> vec_beta ){
   		std::vector<float> vec_m2;
		for( int i=0; i<vec_mom.size(); i++ ){
		 auto p = vec_mom.at(i).P();
		 auto p2 = p*p;
		 auto beta = vec_beta.at(i);
		 auto beta2 = beta*beta;
		 auto gamma2 = 1 - beta2;
		 auto m2 = beta > -990. ? p2 / beta2 * gamma2 : -999.0;
		 vec_m2.push_back( m2 );
		}
		return vec_m2;
    };

    const auto SimMult = []( ROOT::VecOps::RVec<int> simId, ROOT::VecOps::RVec<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >> trMom, RVec<short> trCh){
    	int mult = 0.;    	
    	for( int i=0; i<trMom.size(); i++ ){
    		if(simId.at(i)==-1 && trCh.at(i)!=0 && trCh.at(i)!=-999) mult++;
    	}
    	return mult;
    };

    const auto SimMult2 = []( ROOT::VecOps::RVec<int> simId, ROOT::VecOps::RVec<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >> trMom, RVec<short> trCh){
    	int mult = 0.;    	
    	for( int i=0; i<trMom.size(); i++ ){
    		if(simId.at(i)!=-1)continue;
    		if(trCh.at(i)==3 || trCh.at(i)==-3)mult++;
    	}
    	return mult;
    };

    const auto SimMultPiKP = []( ROOT::VecOps::RVec<int> simId, ROOT::VecOps::RVec<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >> trMom, const RVec<int> pdg){
    	int mult = 0.;    	
    	for( int i=0; i<trMom.size(); i++ ){
    		if(simId.at(i)==-1){
    			int pdg_code = abs(pdg.at(i));
    			if( pdg_code==211 || pdg_code==321 || pdg_code==2212) mult++;
    		}
    	}
    	return mult;
    };

    const auto RecMult = []( ROOT::VecOps::RVec<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >> trMom, RVec<short> trCh){
    	int mult = 0.;    	
    	for( int i=0; i<trMom.size(); i++ ){
    		if(trCh.at(i)!=0)mult++;
    	}
    	return mult;
    };


  auto vtx_correction_generator = []( double calib ){
	  return [calib](double _vtx){return _vtx - calib; };
	};

	auto dd = d
//	ROOT::VecOps::RVec<int>
		.Define( "track_multiplicity", 		"return trMom.size();" )
		.Define( "sts_track_multiplicity",	"return stsTrackMomentum.size();" )
		//.Define( "sts_track_multiplicity", RecMult,	{"trMom","trCharge"})
		.Define( "simCharge",	simCharge, {"simPdg"} )
		.Define( "track_multiplicity_sim",	SimMult,  {"simMotherId","simMom","simCharge"} )
		.Define( "track_multiplicity_sim2",	SimMult2, {"simMotherId","simMom","simCharge"} )
		.Define( "track_multiplicity_sim_PiKP",	SimMultPiKP, {"simMotherId","simMom","simPdg"} )
		.Define( "sim_track_multiplicity", "int tracks=0; for( auto motherId : simMotherId ){ if(motherId == -1) tracks++; } return tracks;" )
		//Vertex
		.Define( "vtx_chi2_ndf", "return vtxChi2/vtxNdf;" )
		.Define( "vtxR", 	"return sqrt(vtxX*vtxX + vtxY*vtxY);" )
		.Define( "simvtxR",	"return sqrt(simVtxX*simVtxX + simVtxY*simVtxY);" )
		//VtxCorr
		.Define( "vtxXcorr", vtx_correction_generator(VtxX_corr), {"vtxX"})
		.Define( "vtxYcorr", vtx_correction_generator(VtxY_corr), {"vtxY"})
		.Define( "vtxZcorr", vtx_correction_generator(VtxZ_corr), {"vtxZ"})
		.Define( "vtxRcorr", "return sqrt(vtxXcorr*vtxXcorr + vtxYcorr*vtxYcorr);")
		//
		.Define( "Tof400_multiplicity", "return trBetaTof400.size()" )
		.Define( "Tof700_multiplicity", "return trBetaTof700.size()" )
		// MC Tracks
		.Define( "sim_p",  " std::vector<float> p;  for( auto mom : simMom ){ p.push_back( mom.P() ); } return p; " )
		.Define( "sim_px", " std::vector<float> px; for( auto mom : simMom ){ px.push_back( mom.Px() ); } return px; " )
		.Define( "sim_py", " std::vector<float> py; for( auto mom : simMom ){ py.push_back( mom.Py() ); } return py; " )
		.Define( "sim_pz", " std::vector<float> pz; for( auto mom : simMom ){ pz.push_back( mom.Pz() ); } return pz; " )
		.Define( "sim_pT", " std::vector<float> pT; for( auto mom : simMom ){ pT.push_back( mom.Pt() ); } return pT; " )
		.Define( "sim_phi"," std::vector<float> phi;for( auto mom : simMom ){ phi.push_back( mom.Phi() ); } return phi; " )
		.Define( "sim_eta", " std::vector<float> eta; for( auto mom : simMom ){ eta.push_back( mom.Eta() ); } return eta; " )
		.Define( "sim_pq", " std::vector<float> pq; for( int i=0; i<simMom.size(); i++ ){ pq.push_back( simMom.at(i).P()*simPdg.at(i)/abs(simPdg.at(i)) ); } return pq;" )
		.Define( "simIsPrimary", "std::vector<int> check; for( auto motherId : simMotherId ){ check.push_back( motherId == -1 ? 1 : 0 ); } return check;" )
		.Define( "simIsSecondary", "std::vector<int> check; for( auto motherId : simMotherId ){ check.push_back( motherId != -1 ? 1 : 0 ); } return check;" )
		// Global Tracks
		//.Define( "trRapidity", Rapidity, {"pz", "pq", "trM2"} )
		.Define( "tr_chi2_ndf", " ROOT::RVecF vec_par; for( int i=0; i<trChi2.size(); ++i ){ vec_par.push_back( trChi2.at(i) / trNdf.at(i) ); } return vec_par; " )
		.Define( "p",  " ROOT::RVecF p;   for( auto mom : trMom ){ p.push_back( mom.P() ); } 	 return p; " )
		.Define( "px", " ROOT::RVecF px;  for( auto mom : trMom ){ px.push_back( mom.Px() ); } 	 return px; " )
		.Define( "py", " ROOT::RVecF py;  for( auto mom : trMom ){ py.push_back( mom.Py() ); } 	 return py; " )
		.Define( "pz", " ROOT::RVecF pz;  for( auto mom : trMom ){ pz.push_back( mom.Pz() ); } 	 return pz; " )
		.Define( "pT", " ROOT::RVecF pT;  for( auto mom : trMom ){ pT.push_back( mom.Pt() ); } 	 return pT; " )
		.Define( "phi"," ROOT::RVecF phi; for( auto mom : trMom ){ phi.push_back( mom.Phi() ); } return phi; " )
		.Define( "eta"," ROOT::RVecF eta; for( auto mom : trMom ){ eta.push_back( mom.Eta() ); } return eta; " )
		.Define( "dcaX", 	" ROOT::RVecF tr_DcaX; for( int i=0; i<trDca.size(); ++i ){ tr_DcaX.push_back( trDca.at(i).X()  ); } return tr_DcaX; " )
		.Define( "dcaY", 	" ROOT::RVecF tr_DcaY; for( int i=0; i<trDca.size(); ++i ){ tr_DcaY.push_back( trDca.at(i).Y()  ); } return tr_DcaY; " )
    .Define( "dcaR", "return sqrt(dcaX*dcaX + dcaY*dcaY);" )
		.Define( "trM2Tof400", M2Function, { "trMom", "trBetaTof400" } )
 		.Define( "trM2Tof700", M2Function, { "trMom", "trBetaTof700" } )
		.Define( "pq", " ROOT::RVecF pq;  for( int i=0; i<trMom.size(); i++ ){ pq.push_back( trMom.at(i).P()/trCharge.at(i) ); } return pq;" )
		.Define( "trFhcalX", function_fhcal_x, {"trParamLast"} )
    .Define( "trFhcalY", function_fhcal_y, {"trParamLast"} )
    .Define( "trDcaX", " std::vector<float> vec_par; for( auto par : trParamFirst ){ vec_par.push_back( par.at(0) - vtxX ); } return vec_par; " )
  	.Define( "trDcaY", " std::vector<float> vec_par; for( auto par : trParamFirst ){ vec_par.push_back( par.at(1) - vtxY ); } return vec_par; " )
    .Define( "trDcaR", dca_function, {"trDcaX", "trDcaY"} )
		// Spectators
		.Define( "fhcal_x", " std::vector<float> fhcal_x; for( auto pos : fhcalModPos ){ fhcal_x.push_back( pos.X() ); } return fhcal_x; " )
		.Define( "fhcal_y", " std::vector<float> fhcal_y; for( auto pos : fhcalModPos ){ fhcal_y.push_back( pos.Y() ); } return fhcal_y; " )
		.Define( "fhcalSumE", "float fhcalE{}; for( auto modE : fhcalModE ){ fhcalE+=modE; } return 1000*fhcalE; " )
		.Define( "scwallSumQ", "float scwallQ{}; for( auto modQ : scwallModQ ){ scwallQ+=modQ; } return scwallQ; " )
		.Define( "hodoSumQ", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return 22916*hodoQ; " )
		.Define( "fhcalhodoSumE", "float fhcalE{}; for( auto modId = 0; modId < (fhcalModId.size()-1); modId++ ){ fhcalE+=fhcalModE.at(modId); } float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return 1000*fhcalE+0.35*22916*hodoQ; " )
		.Define( "hodoIsCentral", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return (hodoQ<40); " )
		.Define( "hodoIsPeripheral", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return (hodoQ>=40); " )
		//
		.Define("goodTrackChM", "pq < 0.")
		.Define("goodTrackCh", 	"trNhits>3 && pT>0.05 && dcaR<5")
    .Define("goodTrackGT",	"eta<3 && pT>0.05 && trNhits>3 && stsTrackChi2Ndf<5 && dcaR<5") 
		.Define("trMomChM",	"trMom[goodTrackChM]")
		.Define("trMomCh",	"trMom[goodTrackCh]")
		.Define("trMomGT",	"trMom[goodTrackGT]")
		.Define("track_multiplicity_ChM","return trMomChM.size();" )
		.Define("track_multiplicity_ch", "return trMomCh.size();" )
		.Define("track_multiplicity_gt", "return trMomGT.size();" )
		;

	// Out File
	auto file_out = TFile::Open( out_file_name.c_str(), "RECREATE" );
	file_out->cd();

	AddStandartHisto(dd,"def_",file_out);

	// m2 cuts
	auto dd_Vtx = dd
		//.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks > 1")
		;
	AddStandartHisto(dd_Vtx,"Vtx_",file_out);

	// m2 cuts
	auto dd_Carp = dd
		//.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks > 1")
		.Filter("sqrt(vtxZcorr*vtxZcorr) < 1.5")
		;
	AddStandartHisto(dd_Carp,"Carp_",file_out);

	// m2 cuts
	auto dd_alex = dd
		//.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks > 1")
		.Filter("vtxRcorr < 1.0")
		.Filter("sqrt(vtxZcorr*vtxZcorr) < 0.1")
		;
	AddStandartHisto(dd_alex,"VtxR1_VtxZ010_",file_out);

	file_out->Close();

	std::cout<<"QA_completed!"<<std::endl;
	
	timer1.Stop();
  	timer1.Print();
}
