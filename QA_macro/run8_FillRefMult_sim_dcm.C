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
	{"b",							{"b",								"b",														";b",								200,		0,			20}},
	{"nTracks",				{"nTracks",					"track_multiplicity",						";N tracks",				1000,		0,			1000}},
	{"nTracksChM",		{"nTracksChM",			"track_multiplicity_ChM",				";N tracks",				1000,		0,			1000}},
	{"nTracksChM0",		{"nTracksChM0",			"track_multiplicity_ChM0",				";N tracks",				1000,		0,			1000}},
	{"nTracksSts",		{"nTracksSts",			"sts_track_multiplicity",				";N tracks",				1000,		0,			1000}},
	{"nTracksSim",		{"nTracksSim",			"track_multiplicity_sim",				";N tracks",				1000,		0,			1000}},
	{"nTracksSim2",		{"nTracksSim2",			"track_multiplicity_sim2",			";N tracks",				1000,		0,			1000}},
	{"nTracksSimGood",{"nTracksSimGood",	"track_multiplicity_good",			";N tracks",				1000,		0,			1000}},
	{"nTracksSimPiKP",{"nTracksSimPiKP",	"track_multiplicity_sim_PiKP",	";N tracks",				1000,		0,			1000}},
	{"nTracksSimAll",	{"nTracksSimAll",		"sim_track_multiplicity",				";N tracks",				1000,		0,			1000}},
	{"nTracksCh_eta3",{"nTracksCh_eta3",	"track_multiplicity_ch_eta3",		";N tracks",				1000,		0,			1000}},
	{"nTracksCh_eta2",{"nTracksCh_eta2",	"track_multiplicity_ch_eta2",		";N tracks",				1000,		0,			1000}},
	{"nTracksGT",			{"nTracksGT",				"track_multiplicity_gt",				";N tracks",				1000,		0,			1000}},
	{"vtx_x",					{"vtx_x",						"vtxX",													";Vtx_{X} (cm)",		1000,		-5.,		5.}},
	{"vtx_y",					{"vtx_y",						"vtxY",													";Vtx_{Y} (cm)",		1000,		-5.,		5.}},
	{"vtx_z",					{"vtx_z",						"vtxZ",													";Vtx_{Z} (cm)",		2000,		-4.,		4.}},
	{"vtx_z_corr",		{"vtx_z_corr",			"vtxZcorr",											";Vtx_{Z} (cm)",		2000,		-4.,		4.}},
	{"vtx_x_corr",		{"vtx_x_corr",			"vtxXcorr",											";Vtx_{X} (cm)",		1000,		-5.,		5.}},
	{"vtx_y_corr",		{"vtx_y_corr",			"vtxYcorr",											";Vtx_{Y} (cm)",		1000,		-5.,		5.}},
	{"vtx_r",					{"vtx_r",						"vtxR",													";Vtx_{R} (cm)",		1000,		0.,			10.}},
	{"simvtx_x",			{"simvtx_x",				"simVtxX",											";Vtx_{X} (cm)",		1000,		-5.,		5.}},
	{"simvtx_y",			{"simvtx_y",				"simVtxY",											";Vtx_{Y} (cm)",		1000,		-5.,		5.}},
	{"simvtx_z",			{"simvtx_z",				"simVtxZ",											";Vtx_{Z} (cm)",		2000,		-4.,		4.}},
	{"simvtx_r",			{"simvtx_r",				"simvtxR",											";Vtx_{R} (cm)",		1000,		0.,			10.}},
	//sim tr
	{"tr_dca_x",			{"tr_dca_x",		"dcaX",					";DCA_{x} (cm)",			800,		-50,		50}},
	{"tr_dca_y",			{"tr_dca_y",		"dcaY",					";DCA_{y} (cm)",			800,		-50,		50}},
	{"tr_dca_r",			{"tr_dca_r",		"dcaR",					";DCA_{y} (cm)",			800,		-50,		50}},
	{"tr_nhits",			{"tr_nhits", 			"trNhits",								";N hits",					40,			0,			40}},
	{"tr_ChSim",			{"tr_ChSim", 			"simCharge",							";ch sim",					20,			-10,		10}},
	{"tr_sim_pT",			{"tr_sim_pT", 		"sim_pT",									";p_{T} (GeV/c)",		1000,		0,			10}},
	{"tr_sim_p",			{"tr_sim_p", 			"sim_p",									";p (GeV/c)",				2000,		-40,		40}},
	{"tr_sim_eta",		{"tr_sim_eta", 		"sim_eta",								";#eta",						1000,		-10,			10}},
	{"tr_sim_eta_plab",{"tr_sim_eta_plab","sim_eta_plab",								";#eta",						1000,		-10,			10}},
	{"tr_sim_y",			{"tr_sim_y", 			"sim_y",									";#y",						1000,		-10,			10}},
	{"tr_sim_y_plab",	{"tr_sim_y_plab",	"sim_y_plab",							";#eta",						1000,		-10,			10}},
	{"tr_sim_phi",		{"tr_sim_phi", 		"sim_phi",								";#phi",						1000,		-5,			5}},
	//reco tr
	{"tr_ChS",				{"tr_Ch",	 				"trCharge",								";ch reco",					20,			-10,		10}},
	{"tr_pT",					{"tr_pT", 				"pT",											";p_{T} (GeV/c)",		1000,		0,			10}},
	{"tr_p",					{"tr_p", 					"p",											";p (GeV/c)",				2000,		-40,		40}},
	{"tr_eta",				{"tr_eta", 				"eta",										";#eta",						1000,		-10,			10}},
	{"tr_phi",				{"tr_phi", 				"phi",										";#phi",						1000,		-5,			5}},
	// fhcall + hodo + scwall
	{"hodo_q",				{"hodo_q",				"hodoSumQ",								";Hodo Q^{2}", 			5000,		0,			20000}},
	{"hodo_q_cent",		{"hodo_q_cent",		"hodoIsCentral",					";Hodo Q^{2}", 			5000,		0,			20000}},
	{"hodo_q_per",		{"hodo_q_per",		"hodoIsPeripheral",				";Hodo Q^{2}", 			5000,		0,			20000}},
	{"scwall_q",			{"scwall_q",			"scwallSumQ",							";ScWall Q_{tot}", 	1000,		0,			1000}},
	{"fhcal&hodo_e",	{"fhcal&hodo_e",	"fhcalhodoSumE",					";E_{tot} (MeV)", 	5000,		0,			20000}},
	{"fhcal_e",				{"fhcal_e", 			"fhcalSumE",							";E_{tot} (MeV)",		5000,		0,			20000}}
};

const std::vector<std::vector< std::string >> QA_MultDemenPlot{	

	{"vtx_x",		"nTracks"},
	{"vtx_y",		"nTracks"},
	{"vtx_z",		"nTracks"},
	{"vtx_r",		"nTracks"},
	
	{"simvtx_x",	"nTracksSim"},
	{"simvtx_y",	"nTracksSim"},
	{"simvtx_z",	"nTracksSim"},
	{"simvtx_r",	"nTracksSim"},

	{"vtx_z",		"vtx_r"},
	{"vtx_z",		"vtx_x"},
	{"vtx_z",		"vtx_y"},
	{"vtx_x",		"vtx_y"},
	{"simvtx_z",	"simvtx_r"},
	{"simvtx_z",	"simvtx_x"},
	{"simvtx_z",	"simvtx_y"},
	{"simvtx_x",	"simvtx_y"},

	{"tr_eta",	"tr_pT"},
	{"tr_eta",	"tr_phi"},
	{"tr_phi",	"tr_pT"},

	{"b", 	"nTracks"},
	{"b", 	"hodo_q"},
	{"b", 	"fhcal_e"},
	
	{"nTracksSimGood",		"b"},
	{"nTracksSimGood", 	"hodo_q"},
	{"nTracksSimGood",		"scwall_q"},
	{"nTracksSimGood",		"fhcal_e"},

	{"nTracks", 				"b"},
	{"nTracks", 				"hodo_q"},
	{"nTracks", 				"scwall_q"},
	{"nTracks", 				"fhcal_e"},
	
	{"nTracksChM", 				"b"},
	{"nTracksChM", 				"hodo_q"},
	{"nTracksChM", 				"scwall_q"},
	{"nTracksChM", 				"fhcal_e"},

	{"nTracksChM0", 				"b"},
	{"nTracksChM0", 				"hodo_q"},
	{"nTracksChM0", 				"scwall_q"},
	{"nTracksChM0", 				"fhcal_e"},

	{"nTracksGT",				"b"},
	{"nTracksGT",				"hodo_q"},
	{"nTracksGT",				"scwall_q"},
	{"nTracksGT",				"fhcal_e"},

	{"nTracksCh_eta2", 	"b"},
	{"nTracksCh_eta2", 	"hodo_q"},
	{"nTracksCh_eta2", 	"scwall_q"},
	{"nTracksCh_eta2", 	"fhcal_e"},
	
	{"nTracksCh_eta3", 	"b"},
	{"nTracksCh_eta3", 	"hodo_q"},
	{"nTracksCh_eta3", 	"scwall_q"},
	{"nTracksCh_eta3", 	"fhcal_e"},

	{"nTracksSim", 			"b"},
	{"nTracksSim", 			"hodo_q"},
	{"nTracksSim", 			"scwall_q"},
	{"nTracksSim", 			"fhcal_e"},
	
	{"nTracksSim2",			"b"},
	{"nTracksSim2",			"hodo_q"},
	{"nTracksSim2",			"scwall_q"},
	{"nTracksSim2",			"fhcal_e"},
	
	{"nTracksSimPiKP",	"b"},
	{"nTracksSimPiKP",	"hodo_q"},
	{"nTracksSimPiKP",	"scwall_q"},
	{"nTracksSimPiKP",	"fhcal_e"},
	
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

void run8_FillRefMult_sim_dcm( std::string str_in_list, std::string out_file_name="qa.root"){
	
	TStopwatch timer1;
  	timer1.Start();

	TFileCollection collection( "collection", "", str_in_list.c_str() );
	auto* chain = new TChain( "t" );
	chain->AddFileInfoList( collection.GetList() );
	ROOT::RDataFrame d( *chain );

	const float PROTON_M = 0.93827208;
  const float PION_M = 0.13957061;
  const float Y_CM = 1.15141;// 
  const float DEUTERON_M = 1.875612928;
  const float TRITON_M = 2.7930000;
	const float FHCAL_Z = 980; // cm

	double VtxX_corr = 0.717;
	double VtxY_corr = -0.22;
	double VtxZ_corr = 0.087;

  // additional functions

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

	const auto rapidity_generator_Plab = [Y_CM]( std::vector<float> vec_pz, std::vector<float> vec_pq, RVec<int> pdg){
      std::vector<float> vec_y{};
      vec_y.reserve( vec_pz.size() );
      for( int i=0; i<vec_pz.size(); ++i ){
        if(vec_pz.at(i)==-9999 || pdg.at(i)>1000000){
        	vec_y.push_back(-9999);
        	continue;
        }
        auto pz = vec_pz.at(i);
        auto p = vec_pq.at(i);
        auto particle_m = TDatabasePDG::Instance()->GetParticle(pdg.at(i))->Mass();
        auto E = sqrt( p*p + particle_m*particle_m );
        auto y = 0.5 * log( ( E + pz ) / ( E - pz ) ) + Y_CM;
        vec_y.push_back( y );
      }
      return vec_y;
    };

   	const auto pseudorapidity_Plab = []( std::vector<float> vec_pT, std::vector<float> vec_phi, std::vector<float> vec_y, RVec<int> pdg){
      std::vector<float> vec_eta{};
      vec_eta.reserve( vec_y.size() );
      for( int i=0; i<vec_y.size(); ++i ){
        if(vec_y.at(i)==-9999 || pdg.at(i)>1000000 ){
        	vec_eta.push_back(-9999);
        	continue;
        }
        auto particle_m = TDatabasePDG::Instance()->GetParticle(pdg.at(i))->Mass();
        auto mT = sqrt(vec_pT.at(i)*vec_pT.at(i) + particle_m*particle_m);
        auto pz_plab = mT * sinh(vec_y.at(i));
        auto px_plab = vec_pT.at(i) * cos(vec_phi.at(i));
        auto py_plab = vec_pT.at(i) * sin(vec_phi.at(i));
        auto p_lab 	 = sqrt(px_plab*px_plab+py_plab*py_plab+pz_plab*pz_plab);
        auto eta = 0.5 * log( ( p_lab + pz_plab ) / ( p_lab - pz_plab ) );
        vec_eta.push_back( eta );
      }
      return vec_eta;
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
   		ROOT::RVecF vec_m2;
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

    const auto SimMultGood = []( ROOT::VecOps::RVec<int> simId, ROOT::VecOps::RVec<ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >> trMom, RVec<short> trCh, std::vector<float> eta, std::vector<float> pt){
    	int mult = 0.;    	
    	for( int i=0; i<trMom.size(); i++ ){
    		if( simId.at(i)!=-1)	continue;
    		if( trCh.at(i)==0 )		continue;
    		if( eta.at(i)>3. || eta.at(i)<0.0 ) continue;
    		if( pt.at(i)<0.02 || pt.at(i)>2.0 )continue;
    		mult++;
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
		.Define( "sim_p",  " std::vector<float> p;  for( int i=0; i<simMom.size(); i++ ){ p.push_back( (simMotherId.at(i)==-1 & simCharge.at(i)!=0) ? simMom.at(i).P() : -9999 ); } return p; " )
		.Define( "sim_px", " std::vector<float> px; for( int i=0; i<simMom.size(); i++ ){ px.push_back( (simMotherId.at(i)==-1 & simCharge.at(i)!=0) ? simMom.at(i).Px() : -9999 ); } return px; " )
		.Define( "sim_py", " std::vector<float> py; for( int i=0; i<simMom.size(); i++ ){ py.push_back( (simMotherId.at(i)==-1 & simCharge.at(i)!=0) ? simMom.at(i).Py() : -9999 ); } return py; " )
		.Define( "sim_pz", " std::vector<float> pz; for( int i=0; i<simMom.size(); i++ ){ pz.push_back( (simMotherId.at(i)==-1 & simCharge.at(i)!=0) ? simMom.at(i).Pz() : -9999 ); } return pz; " )
		.Define( "sim_pT", " std::vector<float> pT; for( int i=0; i<simMom.size(); i++ ){ pT.push_back( (simMotherId.at(i)==-1 & simCharge.at(i)!=0) ? simMom.at(i).Pt() : -9999 ); } return pT; " )
		.Define( "sim_phi"," std::vector<float> phi;for( int i=0; i<simMom.size(); i++ ){ phi.push_back( (simMotherId.at(i)==-1 & simCharge.at(i)!=0) ? simMom.at(i).Phi() : -9999 ); } return phi; " )
		.Define( "sim_eta"," std::vector<float> eta; for( int i=0; i<simMom.size(); i++ ){ eta.push_back( (simMotherId.at(i)==-1 & simCharge.at(i)!=0) ? simMom.at(i).Eta() : -9999 ); } return eta; " )
		.Define( "sim_y",  " std::vector<float> y; 	for( int i=0; i<simMom.size(); i++ ){ y.push_back( (simMotherId.at(i)==-1 & simCharge.at(i)!=0) ? simMom.at(i).Rapidity() : -9999 ); } return y; " )
		.Define( "sim_pq", " std::vector<float> pq; for( int i=0; i<simMom.size(); i++ ){ pq.push_back( simMom.at(i).P()*simPdg.at(i)/abs(simPdg.at(i)) ); } return pq;" )
		.Define( "sim_y_plab", rapidity_generator_Plab	, {"sim_pz","sim_pq","simPdg"})
		.Define( "sim_eta_plab", pseudorapidity_Plab	, {"sim_pT","sim_phi","sim_y_plab","simPdg"})

		.Define( "simIsPrimary", "std::vector<int> check; for( auto motherId : simMotherId ){ check.push_back( motherId == -1 ? 1 : 0 ); } return check;" )
		.Define( "simIsSecondary", "std::vector<int> check; for( auto motherId : simMotherId ){ check.push_back( motherId != -1 ? 1 : 0 ); } return check;" )
		// Global Tracks
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
		.Define( "eta_mult",  " ROOT::RVecF eta_mult; for( auto mom : trMom ){ eta_mult.push_back( track_multiplicity ); } 	 return eta_mult; " )

		//
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
		.Define( "hodoSumQ", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return 1000*hodoQ; " )
		.Define( "fhcalhodoSumE", "float fhcalE{}; for( auto modId = 0; modId < (fhcalModId.size()-1); modId++ ){ fhcalE+=fhcalModE.at(modId); } float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return 1000*fhcalE+0.35*1000*hodoQ; " )
		.Define( "hodoIsCentral", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return (hodoQ<40); " )
		.Define( "hodoIsPeripheral", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return (hodoQ>=40); " )
		//
		//.Define("goodSimTrackChM", 	"sim_pT>0.05 && sim_eta<3 && sim_pq < 0.")
		.Define("goodTrackChM0", 		"pT>0.02 && pq < 0.")
		.Define("goodTrackChM", 		"pT>0.02 && dcaR<5 && eta<3 && pq < 0.")
		.Define("goodTrackCh_eta3",	"pT>0.02 && dcaR<5 && eta<3")
		.Define("goodTrackCh_eta2",	"pT>0.02 && dcaR<5 && eta<2")
    .Define("goodTrackGT",			"pT>0.02 && dcaR<5 && eta<3 && trNhits>4 && stsTrackChi2Ndf<5") 
		//
		.Define("trMomChM0",		"trMom[goodTrackChM0]")
		.Define("trMomChM",			"trMom[goodTrackChM]")
		.Define("trMomCh_eta3",	"trMom[goodTrackCh_eta3]")
		.Define("trMomCh_eta2",	"trMom[goodTrackCh_eta2]")
		.Define("trMomGT",			"trMom[goodTrackGT]")
		//
		.Define("track_multiplicity_ChM0","return trMomChM0.size();" )
		.Define("track_multiplicity_ChM","return trMomChM.size();" )
		.Define("track_multiplicity_ch_eta3", "return trMomCh_eta3.size();" )
		.Define("track_multiplicity_ch_eta2", "return trMomCh_eta2.size();" )
		.Define("track_multiplicity_gt", 			"return trMomGT.size();" )
		.Define( "track_multiplicity_good",	SimMultGood, {"simMotherId","simMom","simCharge","sim_eta_plab","sim_pT"} )
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
	auto dd_Vtx15 = dd
		//.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks > 1")
		.Filter("sqrt(vtxZcorr*vtxZcorr) < 0.15")
		;
	AddStandartHisto(dd_Vtx15,"Vtx_VtxZ15_",file_out);

		// m2 cuts
	auto dd_Vtx20 = dd
		//.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks > 1")
		.Filter("sqrt(vtxZcorr*vtxZcorr) < 0.20")
		;
	AddStandartHisto(dd_Vtx20,"Vtx_VtxZ20_",file_out);


	// 10.35 - 70%
	// 9.65 - 60%
	// 6.25 - 25%
	// m2 cuts
	auto dd_b1 = dd
		//.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks > 1")
		.Filter("b < 9.65")
		;
	AddStandartHisto(dd_b1,"VtxB9_",file_out);

/*
	auto dd_b2 = dd
		//.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks > 1")
		.Filter("b < 10.35")
		;
	AddStandartHisto(dd_b2,"VtxB10_",file_out);

	auto dd_b3 = dd
		//.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks > 1")
		.Filter("b < 6.25")
		;
	AddStandartHisto(dd_b3,"VtxB6_",file_out);
*/
/*
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
		.Filter("sqrt(vtxZcorr*vtxZcorr) < 0.2")
		;
	AddStandartHisto(dd_alex,"VtxR1_VtxZ010_",file_out);
*/
	file_out->Close();

	std::cout<<"QA_completed!"<<std::endl;
	
	timer1.Stop();
  	timer1.Print();
}
