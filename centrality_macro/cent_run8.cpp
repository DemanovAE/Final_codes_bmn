#include <string>
#include <iostream>
#include <fstream> 

using namespace ROOT;
using namespace ROOT::Math;
using namespace ROOT::RDF;

void RemoveBadRuns(std::vector<int> &_Runs, std::vector<int> _BadRUns){
	for(auto _run : _BadRUns){
		_Runs.erase(remove(_Runs.begin(), _Runs.end(), _run), _Runs.end());
	}
}

void cent_run8(std::string str_in_list, std::string out_file_name="qa.root", std::string in_fit_file="inFit.root"){	

	//CorrRunId.root

	auto file_fit = TFile::Open( in_fit_file.c_str(), "READ" );
	file_fit->cd();
	TGraphErrors *f1_FitVtxX= (TGraphErrors*)file_fit->Get("grNew_def_h2_RunId_vtx_x");
	TGraphErrors *f1_FitVtxY= (TGraphErrors*)file_fit->Get("grNew_def_h2_RunId_vtx_y");
	TGraphErrors *f1_FitVtxZ= (TGraphErrors*)file_fit->Get("grNew_def_h2_RunId_vtx_z");

	TGraphErrors *f1_FitRunIdFactor_1 = (TGraphErrors*)file_fit->Get("RunId_corr_factor_h2_RunId_nTracks_8120_8170");
	TGraphErrors *f1_FitRunIdFactor_2 = (TGraphErrors*)file_fit->Get("RunId_corr_factor_h2_RunId_nTracks_7400_7450");

	auto ARCut = []( ROOT::VecOps::RVec<unsigned int> map ){ return map[0] & (1<<7); }; //
	auto stsNdigitsMultCut = []( unsigned long sts_digits, unsigned long n_tracks ){  
		double sts_min = sts_digits-n_tracks*(4.81632+0.0332792*n_tracks-9.62078e-05*n_tracks*n_tracks);
		double sts_max = sts_digits-n_tracks*(19.4203-0.0518774*n_tracks+4.56033e-05*n_tracks*n_tracks); 
		return -74.0087 < sts_min && sts_max < 188.248;//188.248 
	};


	std::map<int,std::vector<int>> cent ={
		{0,{ 0,10}},
		{1,{10,20}},
		{2,{20,30}},
		{3,{30,40}},
		{4,{40,50}},
		{5,{50,60}},
		{6,{60,70}},
		{7,{70,80}}
	};

	auto GetCentrBin_8120_8170 = [](Double_t _refMult){ //RunId_corr_factor_h2_RunId_nTracks_8120_8170
		if(_refMult < 6) return -1;
		else if ( _refMult < 12 ) return 7; // 70-80%
		else if ( _refMult < 22 ) return 6; // 60-70%
		else if ( _refMult < 33 ) return 5; // 50-60%
		else if ( _refMult < 49 ) return 4; // 40-50%
		else if ( _refMult < 71 ) return 3; // 30-40%
		else if ( _refMult < 99 ) return 2; // 20-30%
		else if ( _refMult < 137) return 1; // 10-20%
		else if ( _refMult < 236) return 0; //  0-10%
		else if ( _refMult >=236) return -1;
		return -1;
	};

	auto GetCentrBin_7400_7450 = [](Double_t _refMult){ //RunId_corr_factor_h2_RunId_nTracks_7400_7450
		if(_refMult < 4) return -1;
		else if ( _refMult < 8  ) return 7; // 70-80%
		else if ( _refMult < 14 ) return 6; // 60-70%
		else if ( _refMult < 22 ) return 5; // 50-60%
		else if ( _refMult < 34 ) return 4; // 40-50%
		else if ( _refMult < 49 ) return 3; // 30-40%
		else if ( _refMult < 70 ) return 2; // 20-30%
		else if ( _refMult < 98 ) return 1; // 10-20%
		else if ( _refMult < 177) return 0; //  0-10%
		else if ( _refMult >=177) return -1;
		return -1;
	};


	///////////////////////////////////////////////////////
	std::map<int,std::vector<int>> cent2 ={
		{0, { 0, 5}},
		{1, { 5,10}},
		{2, {10,15}},
		{3, {15,20}},
		{4, {20,25}},
		{5, {25,30}},
		{6, {30,35}},
		{7, {35,40}},
		{8, {40,50}},
		{9, {50,60}},
		{10,{60,70}},
		{11,{70,80}}
	};

	auto GetCentrBin2_8120_8170 = [](Double_t _refMult){ //RunId_corr_factor_h2_RunId_nTracks_8120_8170
		if(_refMult < 6) return -1;
		else if ( _refMult < 12 ) return 11;// 70-80%
		else if ( _refMult < 22 ) return 10;// 60-70%
		else if ( _refMult < 33 ) return 9; // 50-60%
		else if ( _refMult < 49 ) return 8; // 40-50%
		else if ( _refMult < 59 ) return 7; // 35-40%
		else if ( _refMult < 71 ) return 6; // 30-35%
		else if ( _refMult < 84 ) return 5; // 25-30%
		else if ( _refMult < 99 ) return 4; // 20-25%
		else if ( _refMult < 116) return 3; // 15-20%
		else if ( _refMult < 137) return 2; // 10-15%
		else if ( _refMult < 161) return 1; //  5-10%
		else if ( _refMult < 236) return 0; //  0-5%
		else if ( _refMult >=236) return -1;
		return -1;
	};

	auto GetCentrBin2_7400_7450 = [](Double_t _refMult){ //RunId_corr_factor_h2_RunId_nTracks_7400_7450
		if(_refMult < 4) return -1;
		else if ( _refMult < 8  ) return 11;// 70-80%
		else if ( _refMult < 14 ) return 10;// 60-70%
		else if ( _refMult < 22 ) return 9; // 50-60%
		else if ( _refMult < 34 ) return 8; // 40-50%
		else if ( _refMult < 41 ) return 7; // 35-40%
		else if ( _refMult < 50 ) return 6; // 30-35%
		else if ( _refMult < 60 ) return 5; // 25-30%
		else if ( _refMult < 71 ) return 4; // 20-25%
		else if ( _refMult < 84 ) return 3; // 15-20%
		else if ( _refMult < 99 ) return 2; // 10-15%
		else if ( _refMult < 118) return 1; //  5-10%
		else if ( _refMult < 177) return 0; //  0-5%
		else if ( _refMult >=177) return -1;
		return -1;
	};


	std::vector<int> physical_runs{6667, 6668, 6669, 6670, 6671, 6672, 6673, 6674, 6675, 6676, 6677, 6678, 6679, 6680, 6681, 6683, 6684, 6685, 6686, 6687, 6689, 6690, 6691, 6692, 6694, 6695, 6696, 6698, 6699, 6732, 6733, 6734, 6737, 6738, 6739, 6740, 6745, 6752, 6753, 6760, 6761, 6765, 6766, 6767, 6768, 6769, 6771, 6772, 6773, 6774, 6779, 6780, 6782, 6783, 6785, 6786, 6788, 6794, 6795, 6797, 6799, 6800, 6803, 6815, 6816, 6817, 6818, 6819, 6820, 6821, 6822, 6879, 6882, 6883, 6884, 6886, 6887, 6889, 6891, 6900, 6901, 6902, 6903, 6904, 6905, 6906, 6907, 6908, 6909, 6910, 6911, 6915, 6916, 6918, 6919, 6920, 6921, 6923, 6924, 6926, 6927, 6928, 6929, 6930, 6931, 6932, 6933, 6934, 6935, 6936, 6937, 6939, 6940, 6968, 6970, 6972, 6973, 6975, 6976, 6977, 6978, 6979, 6980, 6981, 6982, 6983, 6984, 6990, 6991, 6992, 6993, 6994, 6995, 6997, 6998, 6999, 7000, 7002, 7003, 7004, 7005, 7006, 7008, 7009, 7010, 7011, 7012, 7030, 7031, 7032, 7033, 7034, 7035, 7037, 7038, 7040, 7041, 7042, 7043, 7044, 7046, 7047, 7048, 7049, 7050, 7051, 7052, 7053, 7054, 7055, 7056, 7075, 7076, 7077, 7078, 7081, 7082, 7083, 7084, 7086, 7087, 7091, 7092, 7093, 7094, 7096, 7097, 7098, 7100, 7101, 7102, 7103, 7104, 7125, 7126, 7127, 7128, 7129, 7130, 7131, 7132, 7133, 7135, 7136, 7137, 7138, 7146, 7149, 7150, 7151, 7154, 7155, 7156, 7157, 7159, 7160, 7161, 7162, 7163, 7164, 7165, 7166, 7167, 7168, 7173, 7174, 7175, 7176, 7177, 7178, 7179, 7180, 7181, 7182, 7184, 7186, 7187, 7188, 7191, 7192, 7193, 7194, 7195, 7200, 7202, 7203, 7205, 7206, 7207, 7208, 7209, 7211, 7212, 7213, 7214, 7215, 7216, 7217, 7218, 7219, 7220, 7223, 7225, 7255, 7258, 7261, 7263, 7265, 7267, 7268, 7269, 7271, 7272, 7274, 7276, 7278, 7279, 7281, 7284, 7286, 7288, 7290, 7291, 7312, 7313, 7320, 7321, 7322, 7323, 7325, 7326, 7327, 7328, 7337, 7342, 7343, 7344, 7345, 7346, 7348, 7349, 7351, 7352, 7353, 7354, 7355, 7356, 7357, 7358, 7359, 7361, 7363, 7364, 7365, 7367, 7369, 7374, 7376, 7377, 7378, 7379, 7380, 7381, 7382, 7386, 7387, 7388, 7389, 7390, 7391, 7392, 7393, 7395, 7396, 7397, 7398, 7399, 7400, 7401, 7402, 7403, 7405, 7406, 7408, 7409, 7410, 7411, 7412, 7413, 7414, 7415, 7417, 7418, 7419, 7421, 7422, 7423, 7425, 7427, 7428, 7429, 7431, 7432, 7433, 7434, 7435, 7437, 7439, 7440, 7441, 7442, 7444, 7445, 7446, 7447, 7449, 7451, 7452, 7453, 7454, 7455, 7456, 7457, 7458, 7460, 7461, 7469, 7471, 7472, 7473, 7474, 7477, 7478, 7480, 7481, 7482, 7483, 7484, 7487, 7488, 7489, 7490, 7491, 7492, 7493, 7495, 7497, 7498, 7500, 7501, 7502, 7513, 7514, 7515, 7517, 7519, 7520, 7521, 7528, 7529, 7530, 7531, 7532, 7533, 7534, 7537, 7538, 7539, 7542, 7543, 7545, 7546, 7547, 7549, 7550, 7551, 7552, 7553, 7554, 7564, 7565, 7566, 7567, 7569, 7570, 7572, 7573, 7574, 7575, 7577, 7579, 7581, 7584, 7585, 7586, 7587, 7590, 7591, 7592, 7596, 7597, 7599, 7600, 7604, 7605, 7606, 7607, 7608, 7609, 7611, 7612, 7613, 7622, 7623, 7625, 7626, 7627, 7628, 7630, 7631, 7633, 7634, 7635, 7636, 7638, 7639, 7640, 7641, 7643, 7644, 7645, 7646, 7647, 7649, 7655, 7656, 7657, 7659, 7660, 7662, 7663, 7664, 7665, 7666, 7668, 7669, 7670, 7671, 7673, 7674, 7675, 7676, 7677, 7678, 7679, 7681, 7682, 7684, 7685, 7687, 7688, 7689, 7690, 7692, 7693, 7694, 7696, 7698, 7700, 7701, 7702, 7703, 7704, 7705, 7710, 7712, 7713, 7714, 7715, 7716, 7717, 7718, 7721, 7723, 7724, 7725, 7726, 7727, 7728, 7729, 7730, 7732, 7733, 7734, 7735, 7736, 7737, 7751, 7752, 7753, 7755, 7756, 7761, 7762, 7763, 7764, 7766, 7767, 7768, 7769, 7771, 7772, 7775, 7776, 7778, 7779, 7780, 7781, 7783, 7784, 7785, 7786, 7788, 7789, 7790, 7791, 7794, 7795, 7796, 7797, 7798, 7801, 7802, 7803, 7814, 7816, 7819, 7821, 7824, 7825, 7828, 7829, 7830, 7831, 7832, 7834, 7835, 7836, 7842, 7843, 7845, 7846, 7847, 7848, 7850, 7851, 7852, 7853, 7855, 7856, 7857, 7858, 7859, 7865, 7868, 7869, 7870, 7871, 7873, 7874, 7876, 7877, 7878, 7880, 7882, 7883, 7884, 7885, 7886, 7887, 7890, 7891, 7892, 7893, 7894, 7896, 7897, 7898, 7899, 7900, 7901, 7903, 7904, 7905, 7906, 7907, 7908, 7910, 7911, 7912, 7913, 7914, 7931, 7932, 7933, 7935, 7937, 7938, 7939, 7941, 7942, 7944, 7948, 7949, 7950, 7952, 7954, 7955, 7957, 7958, 7960, 7961, 7962, 7963, 7965, 7966, 7967, 7975, 7977, 7978, 7979, 7981, 7982, 7986, 7988, 7989, 7990, 7991, 7992, 7995, 7996, 7997, 7998, 7999, 8000, 8001, 8002, 8004, 8005, 8006, 8007, 8008, 8009, 8013, 8014, 8015, 8016, 8018, 8020, 8021, 8022, 8023, 8026, 8027, 8028, 8029, 8030, 8031, 8032, 8033, 8038, 8039, 8040, 8041, 8042, 8044, 8045, 8046, 8047, 8048, 8050, 8051, 8052, 8053, 8055, 8056, 8057, 8058, 8059, 8061, 8063, 8064, 8065, 8066, 8068, 8069, 8070, 8071, 8072, 8074, 8075, 8076, 8077, 8079, 8080, 8081, 8082, 8084, 8086, 8087, 8088, 8089, 8090, 8097, 8100, 8101, 8102, 8104, 8106, 8108, 8109, 8110, 8111, 8112, 8113, 8115, 8116, 8117, 8118, 8119, 8121, 8122, 8123, 8124, 8129, 8130, 8131, 8133, 8137, 8138, 8139, 8140, 8141, 8142, 8144, 8156, 8157, 8158, 8159, 8160, 8161, 8162, 8165, 8166, 8167, 8168, 8169, 8170, 8173, 8174, 8175, 8176, 8177, 8180, 8183, 8184, 8186, 8188, 8190, 8191, 8192, 8193, 8195, 8196, 8198, 8199, 8202, 8203, 8204, 8205, 8206, 8207, 8208, 8209, 8210, 8211, 8212, 8213, 8217, 8219, 8220, 8221, 8228, 8229, 8230, 8231, 8235, 8236, 8238, 8239, 8240, 8242, 8244, 8245, 8246, 8247, 8248, 8250, 8251, 8253, 8254, 8255, 8256, 8257, 8258, 8265, 8266, 8267, 8268, 8270, 8271, 8273, 8274, 8275, 8276, 8277, 8278, 8279, 8281, 8284, 8286, 8287, 8288, 8289, 8290, 8292, 8293, 8294, 8295, 8297, 8298, 8299, 8300, 8305, 8306};
	std::vector<int> bad_runs	  {6968, 6970, 6972, 6973, 6975, 6976, 6977, 6978, 6979, 6980, 6981, 6982, 6983, 6984, 7313, 7326, 7415, 7417, 7435,7469, 7471, 7517, 7519, 7520, 7537, 7538, 7542, 7543, 7545, 7546, 7547, 7573, 7575, 7657, 7659, 7679, 7681, 7843, 7847, 7848, 7850, 7851, 7852, 7853, 7855, 7856, 7857, 7858, 7859, 7865, 7868, 7869, 7907, 7910, 7932, 7933, 7935, 7937, 7954, 7955, 8018, 8028, 8029,8030, 8031, 8032, 8033, 8115, 8121, 8167, 8201, 8202, 8203, 8204, 8205, 8206, 8207, 8208, 8209, 8210, 8211, 8212, 8213, 8215, 8284, 8281, 8289};
	//Для физ анализа спользовать раны с 6925 (runId > 6925). До этого проблемы с gem.
	std::cout<<physical_runs.size()<<"\t"<<bad_runs.size()<<"\n";
  	RemoveBadRuns(physical_runs,bad_runs);
  	std::cout<<physical_runs.size()<<"\n";

	auto PhisRunCut = [&physical_runs]( UInt_t run_id ){return std::find( physical_runs.begin(), physical_runs.end(), run_id) != physical_runs.end();};

	// Out File
	auto file_out = TFile::Open( out_file_name.c_str(), "RECREATE" );
	file_out->cd();

	TFileCollection collection( "collection", "", str_in_list.c_str() );
	auto* chain = new TChain( "t" );
	chain->AddFileInfoList( collection.GetList() );
	ROOT::RDataFrame d( *chain );

	auto dd = d
		.Filter("runId < 8312")
		.Filter("runId > 7016")
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		//New vertex
		.Define( "track_multiplicity", "return trMom.size();" )
		.Define( "vtxR", 	  "return sqrt(vtxX*vtxX + vtxY*vtxY);" )
		.Define( "vtxXcorr", [f1_FitVtxX](double _vtxX, UInt_t _runId){return _vtxX - f1_FitVtxX->Eval((double)_runId);},{"vtxX","runId"})
		.Define( "vtxYcorr", [f1_FitVtxY](double _vtxY, UInt_t _runId){return _vtxY - f1_FitVtxY->Eval((double)_runId);},{"vtxY","runId"})
		.Define( "vtxZcorr", [f1_FitVtxZ](double _vtxZ, UInt_t _runId){return _vtxZ - f1_FitVtxZ->Eval((double)_runId);},{"vtxZ","runId"})
		.Define( "vtxRcorr", "return sqrt(vtxXcorr*vtxXcorr + vtxYcorr*vtxYcorr);" )
		//
		.Define( "vtx_chi2_ndf", "return vtxChi2/vtxNdf;" )
		.Define( "stsNdigits","return stsDigits.size()" )
		// Cuts 
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks >= 2")
		.Filter("vtxRcorr < 1.0")
		.Filter("sqrt(vtxZcorr*vtxZcorr) < 0.1")
		.Filter(stsNdigitsMultCut,{"stsNdigits", "track_multiplicity"})
		// после катов уже делаем поправки и центральность
		.Define("track_multiplicity_corr_1",	[f1_FitRunIdFactor_1]  (unsigned long _mult, UInt_t _runId) { return (_mult * f1_FitRunIdFactor_1->Eval((Double_t)_runId));},{"track_multiplicity","runId"} )
		.Define("track_multiplicity_corr_2",	[f1_FitRunIdFactor_2]  (unsigned long _mult, UInt_t _runId) { return (_mult * f1_FitRunIdFactor_2->Eval((Double_t)_runId));},{"track_multiplicity","runId"} )
		.Define("Cent_8120_8170",[GetCentrBin_8120_8170] (Double_t _refMult) { return GetCentrBin_8120_8170(_refMult);},{"track_multiplicity_corr_1"})
		.Define("Cent_7400_7450",[GetCentrBin_7400_7450] (Double_t _refMult) { return GetCentrBin_7400_7450(_refMult);},{"track_multiplicity_corr_2"})
		.Define("Cent_8120_8170_2",[GetCentrBin2_8120_8170] (Double_t _refMult) { return GetCentrBin_8120_8170(_refMult);},{"track_multiplicity_corr_1"})
		.Define("Cent_7400_7450_2",[GetCentrBin2_7400_7450] (Double_t _refMult) { return GetCentrBin_7400_7450(_refMult);},{"track_multiplicity_corr_2"})
	;

	
	vector <RResultPtr<::TH1D >> histo1;;
	histo1.push_back( dd.Histo1D({"h1_cent_8120_8170", ";cent bin", 10, -1.5, 8.5}, "Cent_8120_8170" ));
	histo1.push_back( dd.Histo1D({"h1_cent_7400_7450", ";cent bin", 10, -1.5, 8.5}, "Cent_7400_7450" ));
	histo1.push_back( dd.Histo1D({"h1_cent_8120_8170_2", ";cent bin", 15, -1.5, 13.5}, "Cent_8120_8170" ));
	histo1.push_back( dd.Histo1D({"h1_cent_7400_7450_2", ";cent bin", 15, -1.5, 13.5}, "Cent_7400_7450" ));

	file_out->cd();
	std::for_each( histo1.begin(), histo1.end(), []( auto h1 ){ h1->Write(); } );

	file_out->Close();

	std::cout<<"cent_completed!"<<std::endl;
	
	//timer1.Stop();
  	//timer1.Print();

}