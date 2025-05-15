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
//  {Name histo, 		{Name histo, 		name of the column in tree,	Axis title, 				n bins, 	min, 		max}   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const std::map< std::string, HistoParameter > QA_variable{
	{"RunId",			{"RunId",			"runId",					";Run ID", 				RunIdBins,	RunIdMin,	RunIdMax}},
	
	{"bc2asTime",		{"bc2asTime",		"bc2asTimes",				";t (ns)",					600,		0,			3000}},
	{"bc2asAmp",		{"bc2asAmp",		"bc2asAmplitude",			";bc2 S Amplitude",			800,		0,			4e4}},
	{"bc2msTime",		{"bc2msTime",		"bc2msTimes",				";t (ns)",					600,		0,			3000}},
	{"bc2msAmp",		{"bc2msAmp",		"bc2msAmplitude",			";bc1 S Amplitude",			800,		0,			4e4}},
	{"bc2asInt",		{"bc2asInt",		"bc2asIntegral",			";bc2asT Integral",			800,		0,			4e5}},
	{"bc2msInt",		{"bc2msInt",		"bc2msIntegral",			";bc2msT Integral",			800,		0,			4e5}},
	

	{"bc1sTime",		{"bc1sTime",		"bc1sTdcTimes",				";t (ns)",					600,		0,			3000}},
	{"bc1sAmp",			{"bc1sAmp",			"bc1sAmplitude",			";bc1 S Amplitude",			800,		0,			4e4}},
	{"bc1bAmp",			{"bc1bAmp",			"bc1bAmplitude",			";bc1 B Amplitude",			800,		0,			4e4}},
	{"bc1tAmp",			{"bc1tAmp",			"bc1tAmplitude",			";bc1 T Amplitude",			800,		0,			4e4}},
	{"bc1sIntNorm",		{"bc1sIntNorm",		"bc1sIntegralNorm",			";bc1 S Integral", 			800,		0,			10}},
	{"bc1sInt",			{"bc1sInt",			"bc1sIntegral",				";bc1 S Integral", 			800,		0,			4e5}},
	{"bc1bInt",			{"bc1bInt",			"bc1bIntegral",				";bc1 B Integral", 			800,		0,			4e5}},
	{"bc1tInt",			{"bc1tInt",			"bc1tIntegral",				";bc1 T Integral", 			800,		0,			4e5}},
	{"fdTime",			{"fdTime",			"fdTdcTimes",				";t (ns)",					600,		0,			3000}},
	{"fdAmp",			{"fdAmp",			"fdAmplitude",				";fd Amplitude", 			800,		0,			4e4}},
	{"fdInt",			{"fdInt",			"fdIntegral",				";fd Integral", 			1000,		-10000,		4e5}},
	{"fdIntNorm",		{"fdIntNorm",		"fdIntegralNorm",			";fd Integral", 			1000,		-1,			10}},	
	{"vcsTime",			{"vcsTime",			"vcsTdcTimes",				";t (ns)",					600,		0,			3000}},
	{"vcsAmp",			{"vcsAmp",			"vcsAmplitude",				";vcs Amplitude", 			800,		0,			4e4}},
	{"vcsInt",			{"vcsInt",			"vcsIntegral",				";vcs Integral", 			800,		0,			4e5}},
	{"bdNum",			{"bdNum",			"bd_num",					";BD mult",					200,		0,			200}},
	{"bdNumSingle",		{"bdNumSingle",		"bd_numSingle",				";BD mult",					200,		0,			200}},
	{"nbdMod",			{"nbdMod",			"nbdMod",					";BD modId",				200,		0,			200}},
	{"nbdModSingle",	{"nbdModSingle",	"nbdModSingle",				";BD modId",				200,		0,			200}},
	{"bdMult",			{"bdMult",			"bdMult",					";BD mult",					200,		0,			200}},
	{"bdModId",			{"bdModId",			"bdModId",					";BD modId",				100,		0,			100}},
	{"bdModAmp",		{"bdModAmp",		"bdModAmp",					";BD modAmp",				100,		0,			100}},
	// Event parametrs
	{"cent8_8120_8170",	{"cent8_8120_8170",	"Cent8_8120_8170",			"8120-8170;Centrality",		20,			-0.5,		19.5}},
	{"cent8_7400_7450",	{"cent8_7400_7450",	"Cent8_7400_7450",			"7400-7450;Centrality",		20,			-0.5,		19.5}},	
	{"cent12_8120_8170",{"cent12_8120_8170","Cent12_8120_8170",			"8120-8170;Centrality",		20,			-0.5,		19.5}},
	{"cent12_7400_7450",{"cent12_7400_7450","Cent12_7400_7450",			"7400-7450;Centrality",		20,			-0.5,		19.5}},
	{"nTracks",			{"nTracks",			"track_multiplicity",		";N tracks", 				1000,		0,			1000}},
	{"RefMult1",		{"RefMult1",		"ref1_multiplicity",		";N tracks", 				1000,		0.,			1000.}},
	{"RefMult2",		{"RefMult2",		"ref2_multiplicity",		";N tracks", 				1000,		0.,			1000.}},
	{"nStsTracks",		{"nStsTracks",		"sts_track_multiplicity",	";N sts tracks",			1000,		0,			1000}},
	{"simd_mult",		{"simd_mult",		"simdMult",					";SiMD multiplicity",		200,		0,			200}},
	{"gemDigits",		{"gemDigits",		"gemNdigits",				";GEM digits", 				2000,		0,			20000}},
	{"stsDigits",		{"stsDigits",		"stsNdigits",				";STS digits", 				1000,		0,			10000}},
	{"tof400Digits",	{"tof400Digits",	"tof400Ndigits",			";TOF-400 digits", 			400,		0,			400}},
	{"tof700Digits",	{"tof700Digits",	"tof700Ndigits",			";TOF-700 digits", 			400,		0,			400}},
	{"hodo_q",			{"hodo_q",			"hodoSumQ",					";Hodo Q^{2}", 				1500,		0,			15000}},
	{"scwall_q",		{"scwall_q",		"scwallSumQ",				";ScWall Q_{tot}", 			1000,		0,			1000}},
	{"fhcal&hodo_e",	{"fhcal&hodo_e",	"fhcalhodoSumE",			";E_{tot} (MeV)", 			1500,		0,			15000}},
	//PrimaryVertex
	{"vtx_x",			{"vtx_x",			"vtxX",						";Vtx_{X} (cm)", 			1000,		-5.,		5.}},
	{"vtx_y",			{"vtx_y",			"vtxY",						";Vtx_{Y} (cm)", 			1000,		-5.,		5.}},
	{"vtx_z",			{"vtx_z",			"vtxZ",						";Vtx_{Z} (cm)", 			2000,		-4.,		4.}},
	{"vtx_r",			{"vtx_r",			"vtxR",						";Vtx_{R} (cm)", 			1000,		-5.,		5.}},
	{"vtx_x_corr",		{"vtx_x_corr",		"vtxXcorr",					";Vtx_{X} (cm)",			1000,		-5.,		5.}},
	{"vtx_y_corr",		{"vtx_y_corr",		"vtxYcorr",					";Vtx_{Y} (cm)",			1000,		-5.,		5.}},
	{"vtx_z_corr",		{"vtx_z_corr",		"vtxZcorr",					";Vtx_{Z} (cm)",			2000,		-4.,		4.}},
	{"vtx_r_corr",		{"vtx_r_corr",		"vtxRcorr",					";Vtx_{R} (cm)",			1000,		0,			10.}},
	{"vtx_n_tracks",	{"vtx_n_tracks",    "vtxNtracks",				";N tracks for Vtx", 		500,		0,			500}},
	{"vtx_chi2",		{"vtx_chi2",    	"vtxChi2",					";Vtx #chi^{2}", 			1000,		0,			4000}},
	{"vtx_ndf",			{"vtx_ndf",    		"vtxNdf",					";Vtx ndf", 				1000,		0,			2000}},
	{"vtx_chi2_ndf",	{"vtx_chi2_ndf",    "vtx_chi2_ndf",				";Vtx #chi^{2}/NDF", 		400,		0,			40}},
	//MpdVertex
	{"vtx_x_mpd",		{"vtx_x_mpd",		"vtxXMpd",					";Vtx_{X} (cm)", 			1000,		-5.,		5.}},
	{"vtx_y_mpd",		{"vtx_y_mpd",		"vtxYMpd",					";Vtx_{Y} (cm)", 			1000,		-5.,		5.}},
	{"vtx_z_mpd",		{"vtx_z_mpd",		"vtxZMpd",					";Vtx_{Z} (cm)", 			2000,		-4.,		4.}},
	{"vtx_r_mpd",		{"vtx_r_mpd",		"vtxRMpd",					";Vtx_{R} (cm)", 			1000,		-5.,		5.}},
	{"vtx_n_tracks_mpd",{"vtx_n_tracks_mpd","vtxNtracksMpd",			";N tracks for Vtx", 		500,		0,			500}},
	{"vtx_chi2_mpd",	{"vtx_chi2_mpd", 	"vtxChi2Mpd",				";Vtx #chi^{2}", 			1000,		0,			4000}},
	{"vtx_ndf_mpd",		{"vtx_ndf_mpd", 	"vtxNdfMpd",				";Vtx ndf", 				1000,		0,			2000}},
	{"vtx_chi2_ndf_mpd",{"vtx_chi2_ndf_mpd","vtx_chi2_ndfMpd",			";Vtx #chi^{2}/NDF", 		400,		0,			40}},
	//
	{"tof400_mult",		{"tof400_mult", 	"Tof400_multiplicity",		";TOF-400 multiplicity",	1000,		0,			1000}},
	{"tof700_mult",		{"tof700_mult", 	"Tof700_multiplicity",		";TOF-700 multiplicity",	1000,		0,			1000}},
	{"tof400Matched",	{"tof400Matched", 	"Tof400Matched",			";TOF-400 Matched",			100,		0,			100}},
	{"tof700Matched",	{"tof700Matched", 	"Tof700Matched",			";TOF-700 Matched",			100,		0,			100}},
	{"tr_Tof700hit",	{"tr_Tof700hit", 	"trTof700hit",				";N hits",					40,			0,			40}},
	{"tr_Tof400hit",	{"tr_Tof400hit", 	"trTof400hit",				";N hits",					40,			0,			40}},
	// Global Track variables
	{"tr_chi2_vtx",		{"tr_chi2_vtx", 	"trChi2vtx",				";#chi^{2}",				1000,		0,			4000}},
	{"tr_ndf",			{"tr_ndf", 			"trNdf",					";NDF",						100,		0,			50}},
	{"tr_chi2",			{"tr_chi2", 		"trChi2",					";#chi^{2}",				1000,		0,			1000}},
	{"tr_chi2_ndf",		{"tr_chi2_ndf", 	"tr_chi2_ndf",				";#chi^{2}/ndf",			2500,		0,			2500}},
	{"tr_nhits",		{"tr_nhits", 		"trNhits",					";N hits",					40,			0,			40}},
	{"tr_px",			{"tr_px", 			"px",						";p_{x} (GeV/c)",			1000,		-10,		10}},
	{"tr_py",			{"tr_py", 			"py",						";p_{y} (GeV/c)",			1000,		-10,		10}},
	{"tr_pz",			{"tr_pz", 			"pz",						";p_{z} (GeV/c)",			1000,		0,			40}},
	{"tr_pT",			{"tr_pT", 			"pT",						";p_{T} (GeV/c)",			1000,		0,			10}},
	{"tr_p",			{"tr_p", 			"p",						";p (GeV/c)",				2000,		-40,		40}},
	{"tr_eta",			{"tr_eta", 			"eta",						";#eta",					1000,		0,			10}},
	{"tr_phi",			{"tr_phi", 			"phi",						";#phi",					1000,		-5,			5}},
	{"tr_tof700_t",		{"tr_tof700_t",		"tof700hitT",				";T (ns)",					1000,		0,			100}},
	{"tr_tof400_t",		{"tr_tof400_t",		"tof400hitT",				";T (ns)",					1000,		0,			100}},
	{"tr_x",			{"tr_x",			"tr_x",						";x (cm)",					1000,		-40,		40}},
	{"tr_y",			{"tr_y",			"tr_y",						";y (cm)",					1000,		-40,		40}},
	{"tr_z",			{"tr_z",			"tr_z",						";z (cm)",					500,		-10,		10}},
	{"tr_dca_x",		{"tr_dca_x",		"tr_dca_x",					";DCA_{x} (cm)",			800,		-50,		50}},
	{"tr_dca_y",		{"tr_dca_y",		"tr_dca_y",					";DCA_{y} (cm)",			800,		-50,		50}},
	{"tr_dca_z",		{"tr_dca_z",		"tr_dca_z",					";DCA_{z} (cm)",			800,		-50,		50}},
	{"tr_dca_r",		{"tr_dca_r",		"tr_dca_r",					";DCA_{R} (cm)",			800,		0,			40}},
	{"tr_dca_z_mpd",	{"tr_dca_z_mpd",	"tr_dca_rMpd",					";DCA_{R} (cm)",			800,		0,			40}},
	{"tr_dca_x_mpd",	{"tr_dca_x_mpd",	"tr_dca_xMpd",				";DCA_{x} (cm)",			800,		-50,		50}},
	{"tr_dca_y_mpd",	{"tr_dca_y_mpd",	"tr_dca_yMpd",				";DCA_{y} (cm)",			800,		-50,		50}},
	{"tr_dca_r_mpd",	{"tr_dca_r_mpd",	"tr_dca_zMpd",				";DCA_{R} (cm)",			800,		0,			40}},
	// mass2, beta, pid
	{"tr_pq",			{"tr_pq", 			"pq",						";p/q (GeV/c)",				1000, 		-10., 		10.}},
	{"tr_pq_good",		{"tr_pq_good",		"pq_good",					";p/q (GeV/c)",				1000, 		-10., 		10.}},	
	{"tr_m2_tof400",	{"tr_m2_tof400", 	"trM2Tof400",				";m^{2} (GeV^{2}/c^{4})",	750, 		-5., 		10.}},
	{"tr_m2_tof700",	{"tr_m2_tof700", 	"trM2Tof700",				";m^{2} (GeV^{2}/c^{4})",	750, 		-5., 		10.}},
	
	{"tr_beta_tof400",	{"tr_beta_tof400",	"trBetaTof400",				";#beta_{TOF-400}",			600, 		-1., 		5.}},
	{"tr_beta_tof700",	{"tr_beta_tof700",	"trBetaTof700",				";#beta_{TOF-700}",			600, 		-1., 		5.}},

	{"tr_m2_tof400_good",	{"tr_m2_tof400_good", 	"trM2Tof400_good",				";m^{2} (GeV^{2}/c^{4})",	750, 		-5., 		10.}},
	{"tr_m2_tof700_good",	{"tr_m2_tof700_good", 	"trM2Tof700_good",				";m^{2} (GeV^{2}/c^{4})",	750, 		-5., 		10.}},
	{"tr_beta_tof400_good",	{"tr_beta_tof400_good",	"trBetaTof400_good",				";#beta_{TOF-400}",			600, 		-1., 		5.}},
	{"tr_beta_tof700_good",	{"tr_beta_tof700_good",	"trBetaTof700_good",				";#beta_{TOF-700}",			600, 		-1., 		5.}},	
	{"tr_1_beta_tof400",{"tr_1_beta_tof400","beta400",					";1/#beta_{TOF-400}",		600, 		0., 		3.}},
	{"tr_1_beta_tof700",{"tr_1_beta_tof700","beta700",					";1/#beta_{TOF-700}",		600, 		0., 		3.}},
	{"trProtonY",		{"trProtonY",		"trProtonY",				";y",						500, 		-5., 		5.}},
	// STS track variables
	{"sts_x",			{"sts_x",			"sts_x",					";x (cm)",					1000,		-40.,		40.}},
	{"sts_y",			{"sts_y",			"sts_y",					";y (cm)",					1000,		-40.,		40.}},
	{"sts_z",			{"sts_z",			"sts_z",					";z (cm)",					1000,		0.,			200.}},
	{"sts_chi2_ndf",	{"sts_chi2_ndf", 	"stsTrackChi2Ndf",			";#chi^{2}/NDF",			1000,		0,			4000}},
	{"sts_nhits",		{"sts_nhits", 		"stsTrackNhits",			";N hits",					40,			0,			40}},
	{"sts_px",			{"sts_px", 			"sts_px",					";p_{x} (GeV/c)",			1000,		-10,		10}},
	{"sts_py",			{"sts_py", 			"sts_py",					";p_{y} (GeV/c)",			1000,		-10,		10}},
	{"sts_pz",			{"sts_pz", 			"sts_pz",					";p_{z} (GeV/c)",			1000,		0,			40}},
	{"sts_pT",			{"sts_pT", 			"sts_pT",					";p_{T} (GeV/c)",			1000,		0,			10}},
	{"sts_p",			{"sts_p", 			"sts_p",					";p (GeV/c)",				1000,		0,			40}},
	{"sts_eta",			{"sts_eta", 		"sts_eta",					";#eta",					1000,		0,			10}},
	{"sts_phi",			{"sts_phi", 		"sts_phi",					";#phi",					1000,		-5,			5}},
	// fhcall 
	{"fhcal_ModId",		{"fhcal_ModId", 	"fhcalModId",				";Mod Id",					60,			0,			60}},
	{"fhcal_ModId_e",	{"fhcal_ModId_e", 	"fhcalModE",				";E (MeV)",					500,		0,			500}},
	{"fhcal_e",			{"fhcal_e", 		"fhcalSumE",				";E_{tot} (MeV)",			1500,		0,			15000}},
	{"fhcal_pos_x",		{"fhcal_pos_x", 	"fhcal_x",					";x (cm)",					100,		-100,		100}},
	{"fhcal_pos_y",		{"fhcal_pos_y", 	"fhcal_y",					";y (cm)",					100,		-100,		100}},
	{"tr_fhcal_pos_x",	{"tr_fhcal_pos_x", 	"trFhcalX",					";x (cm)",					200,		-200,		200}},
	{"tr_fhcal_pos_y",	{"tr_fhcal_pos_y", 	"trFhcalY",					";y (cm)",					200,		-200,		200}},
	// beam pipe
	{"beam_hitMult_st0",{"beam_hitMult_st0","beamHitX_St0_mult",		";count",					40,			 0,			40}},
	{"beam_hitMult_st1",{"beam_hitMult_st1","beamHitX_St1_mult",		";count",					40,			 0,			40}},
	{"beam_hitMult_st2",{"beam_hitMult_st2","beamHitX_St2_mult",		";count",					40,			 0,			40}},
	{"beam_hit_x_st0",	{"beam_hit_x_st0", 	"beamHitX_St0",				";x (cm)",					200,		-5,			5}},
	{"beam_hit_y_st0",	{"beam_hit_y_st0", 	"beamHitY_St0",				";y (cm)",					200,		-5,			5}},
	{"beam_hit_x_st1",	{"beam_hit_x_st1", 	"beamHitX_St1",				";x (cm)",					200,		-5,			5}},
	{"beam_hit_y_st1",	{"beam_hit_y_st1", 	"beamHitY_St1",				";y (cm)",					200,		-5,			5}},
	{"beam_hit_x_st2",	{"beam_hit_x_st2", 	"beamHitX_St2",				";x (cm)",					200,		-5,			5}},
	{"beam_hit_y_st2",	{"beam_hit_y_st2", 	"beamHitY_St2",				";y (cm)",					200,		-5,			5}},
	{"beam_x",			{"beam_x", 			"beam_x",					";x (cm)",					200,		-5,			5}},
	{"beam_y",			{"beam_y", 			"beam_y",					";y (cm)",					200,		-5,			5}}
};

const std::vector<std::vector< std::string >> QA_MultDemenPlot{
	//{"tr_nhits", 	"Weight:trIsProton"},
	{"bc1sInt",		"fdInt"},
	{"bc1sIntNorm",	"fdIntNorm"},
	{"bc1sAmp",		"fdAmp"},
	{"nTracks",		"bc1sAmp"},
	{"nTracks",		"bc1sInt"},
	{"nTracks",		"fdAmp"},
	{"nTracks",		"fdInt"},
	{"tof400_mult", "tof400Matched"},
	{"tof700_mult", "tof700Matched"},
	{"tof700_mult", "tof400_mult"},
	{"tof700Matched","tof400Matched"},
	{"nTracks",		"tof400Matched"},
	{"nTracks",		"tof700Matched"},
	{"nTracks",		"tof400Digits"},
	{"nTracks",		"tof700Digits"},
	{"nTracks",		"stsDigits"},
	{"nTracks",		"gemDigits"},
	{"nTracks",		"beam_hit_x_st2"},
	{"nTracks",		"beam_hit_y_st2"},
	
	{"cent8_8120_8170", 	"RefMult1"},
	{"cent8_7400_7450", 	"RefMult2"},
	{"cent12_8120_8170", 	"RefMult1"},
	{"cent12_7400_7450", 	"RefMult2"},

	{"vtx_x",		"vtx_x_mpd"},
	{"vtx_y",		"vtx_y_mpd"},
	{"vtx_z",		"vtx_z_mpd"},

	{"vtx_x",		"nTracks"},
	{"vtx_y",		"nTracks"},
	{"vtx_z",		"nTracks"},
	{"vtx_x_corr",	"nTracks"},
	{"vtx_y_corr",	"nTracks"},
	{"vtx_z_corr",	"nTracks"},
	{"vtx_x_corr",	"RefMult1"},
	{"vtx_y_corr",	"RefMult1"},
	{"vtx_z_corr",	"RefMult1"},
	{"vtx_x_corr",	"RefMult2"},
	{"vtx_y_corr",	"RefMult2"},
	{"vtx_z_corr",	"RefMult2"},

	{"vtx_x_mpd",	"vtx_y_mpd"},
	
	{"vtx_x",		"vtx_y"},
	{"vtx_x_corr",	"vtx_y_corr"},
	{"vtx_z",		"vtx_r"},
	{"vtx_z_corr",	"vtx_r_corr"},
	{"vtx_z",		"vtx_x"},
	{"vtx_z_corr",	"vtx_x_corr"},
	{"vtx_z",		"vtx_y"},
	{"vtx_z_corr",	"vtx_y_corr"},

	{"tr_x",		"tr_y"},
	{"tr_px",		"tr_py"},
	{"tr_eta",		"tr_phi"},
	{"tr_phi",		"tr_eta"},
	{"tr_pT",		"tr_eta"},
	{"tr_pT",		"tr_phi"},
	{"sts_x",		"sts_y"},
	{"nTracks",		"fhcal_e"},
	{"fhcal_ModId",	"fhcal_ModId_e"},
	{"beam_hit_x_st0","beam_hit_y_st0"},
	{"beam_hit_x_st1","beam_hit_y_st1"},
	{"beam_hit_x_st2","beam_hit_y_st2"},
	{"beam_x",		"beam_y"},

	//{"RunId", 		"fhcal_ModId",		"fhcal_ModId_e"},
	//{"RunId", 		"beam_x",			"beam_y"},
	//{"RunId", 		"beam_hit_x_st0",	"beam_hit_y_st0"},
	//{"RunId", 		"beam_hit_x_st1",	"beam_hit_y_st1"},
	//{"RunId", 		"beam_hit_x_st2",	"beam_hit_y_st2"},
	{"tr_pq",		"tr_beta_tof400"},
	{"tr_pq",		"tr_beta_tof700"},
	{"tr_pq_good",	"tr_beta_tof400_good"},
	{"tr_pq_good",	"tr_beta_tof700_good"},
	{"tr_pq",		"tr_1_beta_tof400"},
	{"tr_pq",		"tr_1_beta_tof700"},
	{"tr_pq",		"tr_m2_tof400"},
	{"tr_pq",		"tr_m2_tof700"},
	{"tr_pq_good",	"tr_m2_tof400_good"},
	{"tr_pq_good",	"tr_m2_tof700_good"},
	
	{"RunId",		"nTracks"},
	{"RunId",   	"RefMult1"},
	{"RunId",   	"RefMult2"},

	{"RunId",		"bc2asInt"},
	{"RunId",		"bc2asTime"},
	{"RunId",		"bc2asAmp"},
	{"RunId",		"bc2msInt"},
	{"RunId",		"bc2msTime"},
	{"RunId",		"bc2msAmp"},

	{"RunId",		"bc1sTime"},
	{"RunId",		"bc1sInt"},
	{"RunId",		"bc1sIntNorm"},
	{"RunId",		"bc1sAmp"},
	{"RunId",		"fdTime"},
	{"RunId",		"fdInt"},
	{"RunId",		"fdIntNorm"},
	{"RunId",		"fdAmp"},
	{"RunId",		"vcsTime"},
	{"RunId",		"vcsInt"},
	{"RunId",		"vcsAmp"},
	{"RunId",		"nStsTracks"},
	{"RunId",		"bdMult"},
	{"RunId",		"bdModId"},
	{"RunId",		"bdModAmp"},
	{"RunId",		"bdNum"},
	{"RunId",		"bdNumSingle"},
	{"RunId",		"nbdMod"},
	{"RunId",		"nbdModSingle"},
	{"RunId",		"stsDigits"},
	{"RunId",		"gemDigits"},
	
	{"RunId",		"tof400Digits"},
	{"RunId",		"tof700Digits"},
	
	{"RunId",		"fhcal_e"},
	{"RunId",		"fhcal_ModId"},
	{"RunId",		"fhcal_ModId_e"},
	{"RunId",		"fhcal_pos_x"},
	{"RunId",		"fhcal_pos_y"},

	{"RunId",		"hodo_q"},
	{"RunId",		"scwall_q"},
	{"RunId",		"vtx_x"},
	{"RunId",		"vtx_y"},
	{"RunId",		"vtx_z"},
	{"RunId",		"vtx_r"},	
	{"RunId", 		"vtx_z_corr"},
	{"RunId", 		"vtx_x_corr"},
	{"RunId", 		"vtx_y_corr"},
	{"RunId",		"vtx_n_tracks"},
	{"RunId",		"vtx_chi2"},
	{"RunId",		"vtx_ndf"},
	{"RunId",		"vtx_chi2_ndf"},
	{"RunId", 		"vtx_z_mpd"},
	{"RunId", 		"vtx_x_mpd"},
	{"RunId", 		"vtx_y_mpd"},
	{"RunId",		"vtx_n_tracks_mpd"},
	{"RunId",		"vtx_chi2_mpd"},
	{"RunId",		"vtx_ndf_mpd"},
	{"RunId",		"vtx_chi2_ndf_mpd"},
	{"RunId",		"tof400_mult"},
	{"RunId",		"tof700_mult"},
	{"RunId",		"tof400Matched"},
	{"RunId",		"tof700Matched"},

	{"RunId",		"tr_ndf"},
	{"RunId",		"tr_chi2"},
	{"RunId",		"tr_chi2_ndf"},
	{"RunId",		"tr_nhits"},
	{"RunId",		"tr_px"},
	{"RunId",		"tr_py"},
	{"RunId",		"tr_pz"},
	{"RunId",		"tr_p"},
	{"RunId",		"tr_pT"},
	{"RunId",		"tr_eta"},
	{"RunId",		"tr_phi"},
	{"RunId",		"tr_x"},
	{"RunId",		"tr_y"},
	{"RunId",		"tr_z"},
	{"RunId",		"tr_dca_x"},
	{"RunId",		"tr_dca_y"},
	{"RunId",		"tr_dca_z"},
	{"RunId",		"tr_dca_r"},
	{"RunId",		"tr_tof400_t"},
	{"RunId",		"tr_tof700_t"},
	{"RunId",		"sts_chi2_ndf"},
	{"RunId",		"sts_nhits"},
	{"RunId",		"sts_px"},
	{"RunId",		"sts_py"},
	{"RunId",		"sts_pz"},
	{"RunId",		"sts_p"},
	{"RunId",		"sts_pT"},
	{"RunId",		"sts_eta"},
	{"RunId",		"sts_phi"},
	{"RunId",		"sts_x"},
	{"RunId",		"sts_y"},
	{"RunId",		"sts_z"},

	{"RunId",		"beam_hit_x_st0"},
	{"RunId",		"beam_hit_y_st0"},
	{"RunId",		"beam_hit_x_st1"},
	{"RunId",		"beam_hit_y_st1"},
	{"RunId",		"beam_hit_x_st2"},
	{"RunId",		"beam_hit_y_st2"},
	{"RunId",		"beam_x"},
	{"RunId",		"beam_y"},
	{"RunId",		"beam_hitMult_st0"},
	{"RunId",		"beam_hitMult_st1"},
	{"RunId",		"beam_hitMult_st2"}
};


float NewXRotate(float x, float y, float phi){
	//std::cout<<"x\t"<<x<<"\t"<<y<<"\t"<<phi<<"\t"<<x * TMath::Cos(phi) - y * TMath::Sin(phi)<<std::endl;
	return x * TMath::Cos(phi) - y * TMath::Sin(phi);
}
float NewYRotate(float x, float y, float phi){
	//std::cout<<"y\t"<<x<<"\t"<<y<<"\t"<<phi<<"\t"<<x * TMath::Sin(phi) + y * TMath::Cos(phi)<<std::endl;
	return x * TMath::Sin(phi) + y * TMath::Cos(phi);
}

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

template<typename T, typename TF>
void WriteInFile(T& vector, TF *out_obj){
	out_obj->cd();
	std::for_each( vector.begin(), vector.end(), []( auto h1 ){ h1->Write(); } );
}


template<typename T>
void AddStandartHisto(T& dd, std::string prefix, TFile *file){
	
	std::vector<ROOT::RDF::RResultPtr<::TH1D>> histo1;
	std::vector<ROOT::RDF::RResultPtr<::TH2D>> histo2;
	std::vector<ROOT::RDF::RResultPtr<::TH3D>> histo3;

	std::cout<<prefix<<std::endl;

	// Fill TH1D histo all QA_variable
	for(auto qa_obj : QA_variable){
		//std::cout<<qa_obj.first<<"\n";
		histo1.push_back( dd.Histo1D( SetH1obj(prefix,qa_obj.second) , qa_obj.second.fFillObj) );
	}

	// Fill TH1D, TH2D, TH3D and TProfile1D with and without weight
	for(auto qa_obj : QA_MultDemenPlot){

		//std::cout<<qa_obj[0]<<"\t"<<qa_obj[1]<<std::endl;
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

		//Fill TProfile1D
		//if(qa_obj.size()==2 && qa_obj[0]=="RunId"){
		//	prof1.push_back( dd.Profile1D( SetProfile1Dobj(prefix,QA_variable.at(qa_obj[0]),QA_variable.at(qa_obj[1])),
		//		QA_variable.at(qa_obj[0]).fFillObj, QA_variable.at(qa_obj[1]).fFillObj));
		//}
	}
	
	WriteInFile(histo1,file);
	WriteInFile(histo2,file);
	WriteInFile(histo3,file);
	//WriteInFile(prof1,file);

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

template<typename T>
std::vector<int> GetRunIdForJob(T& dd){
	std::vector<int> RunIdinJob;
	auto h_RunId = dd.Histo1D({"runId_hist",";Run Id", RunIdBins, RunIdMin, RunIdMax}, "runId");
	
	for(int i=0; i<h_RunId->GetNbinsX();i++){
		if(h_RunId->GetBinContent(i)==0)continue;
		RunIdinJob.push_back((int)h_RunId->GetBinCenter(i));
	}

	return RunIdinJob;	
}

template<typename T>
void AddHistoGraphCutvsRunId2(T& dd, std::string prefix, TFile *file, std::vector<int> _Runs){

	std::vector<ROOT::RDF::RResultPtr<::TH2D>> histo2;

	float nTrBinMin=0;
	float nTrBinMax=500;
	int nTrBin=abs(nTrBinMax-nTrBinMin);

	float nFsdDigiBinMin=0;
	float nFsdDigiBinMax=10000;
	int nFsdDigiBin=abs(nFsdDigiBinMax-nFsdDigiBinMin)/5.;

	float nGemDigiBinMin=0;
	float nGemDigiBinMax=20000;
	int nGemDigiBin=abs(nGemDigiBinMax-nGemDigiBinMin)/5.;
	
	auto dd_new = dd;

	for(auto &irun : _Runs){

		std::cout<<"RunId:\t"<<irun<<std::endl;
		auto _RunCut = [irun]( UInt_t run_id ){return (irun==(int)run_id) ? true : false;};
		histo2.push_back(dd_new.Filter(_RunCut,{"runId"} ).Histo2D( { "ModId_E",";ModId;E", 60, 0, 60, 500,	0, 500 }, "fhcalModId", "fhcalModE" ));		
		histo2.push_back(dd_new.Filter(_RunCut,{"runId"} ).Histo2D( { "tr_fsd",";nTracks;FSD digits", nTrBin,nTrBinMin,nTrBinMax,nFsdDigiBin,nFsdDigiBinMin,nFsdDigiBinMax }, "track_multiplicity", "stsNdigits" ));
		histo2.push_back(dd_new.Filter(_RunCut,{"runId"} ).Histo2D( { "tr_gem",";nTracks;GEM digits", nTrBin,nTrBinMin,nTrBinMax,nGemDigiBin,nGemDigiBinMin,nGemDigiBinMax }, "track_multiplicity", "gemNdigits" ));		
		histo2.push_back(dd_new.Filter(_RunCut,{"runId"} ).Histo2D( { "h2_pq_mass2_tof400", ";p/q (GeV/c);m^{2}_{TOF-400} (GeV^{2}/c^{4})", 600, -4.0, 8.0, 500, -0.5, 9.5 }, "pq", "trM2Tof400") );
		histo2.push_back(dd_new.Filter(_RunCut,{"runId"} ).Histo2D( { "h2_pq_mass2_tof700", ";p/q (GeV/c);m^{2}_{TOF-700} (GeV^{2}/c^{4})", 600, -4.0, 8.0, 500, -0.5, 9.5 }, "pq", "trM2Tof700") );
		//histo2.push_back(dd_new.Filter(_RunCut,{"runId"} ).Histo2D( { "h2_pq_mass2_tof701", ";p/q (GeV/c);m^{2}_{TOF-700} (GeV^{2}/c^{4})", 600, -4.0, 8.0, 500, -0.5, 9.5 }, "pq", "trM2Tof701_2") );
  	    
		TDirectoryFile *dirFile = new TDirectoryFile(Form("%s%i",prefix.c_str(),irun),Form("%s%i",prefix.c_str(),irun));
		WriteInFile(histo2,dirFile);
		histo2.clear();
		dirFile->Clear();
		file->cd();
	}
}

void RemoveBadRuns(std::vector<int> &_Runs, std::vector<int> _BadRUns){
	for(auto _run : _BadRUns){
		_Runs.erase(remove(_Runs.begin(), _Runs.end(), _run), _Runs.end());
	}
}

void run8_qa_new( std::string str_in_list, std::string str_in_list_plp, std::string out_file_name="qa.root", std::string in_fit_file="inFit.root"){
	
	TStopwatch timer1;
  	timer1.Start();

	// In git File
	auto file_fit = TFile::Open( in_fit_file.c_str(), "READ" );
	file_fit->cd();
	TGraphErrors *f1_FitBC1 = (TGraphErrors*)file_fit->Get("gr_cut1_h2_RunId_bc1sInt_1");
	TGraphErrors *f1_FitFD  = (TGraphErrors*)file_fit->Get("gr_cut1_h2_RunId_fdInt_1");
	TGraphErrors *f1_FitMult= (TGraphErrors*)file_fit->Get("gr_cut4_h2_RunId_nTracks");

	auto g1_FitVtxX = file_fit->Get<TGraphErrors>("grNew_def_h2_RunId_vtx_x");
	auto g1_FitVtxY = file_fit->Get<TGraphErrors>("grNew_def_h2_RunId_vtx_y");
	auto g1_FitVtxZ = file_fit->Get<TGraphErrors>("grNew_def_h2_RunId_vtx_z");

	auto g1_FitRunIdFactor_1 = file_fit->Get<TGraphErrors>("RunId_corr_factor_h2_RunId_nTracks_8120_8170");
	auto g1_FitRunIdFactor_2 = file_fit->Get<TGraphErrors>("RunId_corr_factor_h2_RunId_nTracks_7400_7450");
	
	TFileCollection collection( "collection", "", str_in_list.c_str() );
	auto* chain = new TChain( "t" );
	chain->AddFileInfoList( collection.GetList() );	
	ROOT::RDataFrame d( *chain );


    const float PROTON_M = 0.93827208;
    const float PION_M = 0.13957061;
    const float DEUTERON_M = 1.875612928;
    const float TRITON_M = 2.7930000;
    const float Y_CM = 1.15141;
  	const float FHCAL_Z = 980; // cm

    auto f1_2212_m_400 = new TF1("2212_mean_400", "pol1");
    f1_2212_m_400->SetParameter(0, 0.94612766);
    f1_2212_m_400->SetParameter(1, -0.026582296);

    auto f1_2212_s_400 = new TF1("2212_mean_400", "pol2");
    f1_2212_s_400->SetParameter(0, 0.087635220);
    f1_2212_s_400->SetParameter(1, -0.028676680);
    f1_2212_s_400->SetParameter(2, 0.013336097);

    auto f1_2212_m_700 = new TF1("2212_mean_700", "pol1");
    f1_2212_m_700->SetParameter(0, 0.91900468);
    f1_2212_m_700->SetParameter(1, 0.00070073927);

    auto f1_2212_s_700 = new TF1("2212_mean_700", "pol2");
    f1_2212_s_700->SetParameter(0, 0.017564553);
    f1_2212_s_700->SetParameter(1, 0.022982477);
    f1_2212_s_700->SetParameter(2, 0.011826542);

    auto f1_211_m_400 = new TF1("211_mean_400", "pol1");
    f1_211_m_400->SetParameter(0, 0.039112045);
    f1_211_m_400->SetParameter(1, -0.022688427);

    auto f1_211_s_400 = new TF1("211_mean_400", "pol2");
    f1_211_s_400->SetParameter(0, 0.041352984);
    f1_211_s_400->SetParameter(1, -0.037728531);
    f1_211_s_400->SetParameter(2, 0.021342278);

    auto f1_211_m_700 = new TF1("211_mean_700", "pol1");
    f1_211_m_700->SetParameter(0, -0.073757850);
    f1_211_m_700->SetParameter(1, 0.083993579);

    auto f1_211_s_700 = new TF1("211_mean_700", "pol2");
    f1_211_s_700->SetParameter(0, 0.015820599);
    f1_211_s_700->SetParameter(1, -0.016413646);
    f1_211_s_700->SetParameter(2, 0.035489405);

	auto f1_1000010020_m_400 = new TF1("1000010020_mean_400", "pol1");
    f1_1000010020_m_400->SetParameter(0, 3.6842476);
    f1_1000010020_m_400->SetParameter(1, -0.027055688);

    auto f1_1000010020_s_400 = new TF1("1000010020_mean_400", "pol2");
    f1_1000010020_s_400->SetParameter(0, 0.37564034);
    f1_1000010020_s_400->SetParameter(1, -0.10466069);
    f1_1000010020_s_400->SetParameter(2, 0.020659571);

    auto f1_1000010020_m_700 = new TF1("1000010020_mean_700", "pol1");
    f1_1000010020_m_700->SetParameter(0, 3.6920797);
    f1_1000010020_m_700->SetParameter(1, -0.026802983);

    auto f1_1000010020_s_700 = new TF1("1000010020_mean_700", "pol2");
    f1_1000010020_s_700->SetParameter(0, 0.28184248);
    f1_1000010020_s_700->SetParameter(1, -0.11097429);
    f1_1000010020_s_700->SetParameter(2, 0.038907383);

	auto f1_1000010030_m_400 = new TF1("1000010030_mean_400", "pol1");
    f1_1000010030_m_400->SetParameter(0, 8.0153109);
    f1_1000010030_m_400->SetParameter(1, 0.022075980);

    auto f1_1000010030_s_400 = new TF1("1000010030_mean_400", "pol2");
    f1_1000010030_s_400->SetParameter(0, 0.080301800);
    f1_1000010030_s_400->SetParameter(1, 0.19013640);
    f1_1000010030_s_400->SetParameter(2, -0.032232153);
	
	auto f1_1000010030_m_700 = new TF1("1000010030_mean_700", "pol1");
    f1_1000010030_m_700->SetParameter(0, 8.0169743);
    f1_1000010030_m_700->SetParameter(1, -0.049778937);

    auto f1_1000010030_s_700 = new TF1("1000010030_mean_700", "pol2");
    f1_1000010030_s_700->SetParameter(0, 0.058638053);
    f1_1000010030_s_700->SetParameter(1, 0.060602950);
    f1_1000010030_s_700->SetParameter(2, -0.012159416);

	//std::vector<int> physical_runs{6667, 6668, 6669, 6670, 6671, 6672, 6673, 6674, 6675, 6676, 6677, 6678, 6679, 6680, 6681, 6683, 6684, 6685, 6686, 6687, 6689, 6690, 6691, 6692, 6694, 6695, 6696, 6698, 6699, 6732, 6733, 6734, 6737, 6738, 6739, 6740, 6745, 6752, 6753, 6760, 6761, 6765, 6766, 6767, 6768, 6769, 6771, 6772, 6773, 6774, 6779, 6780, 6782, 6783, 6785, 6786, 6788, 6794, 6795, 6797, 6799, 6800, 6803, 6815, 6816, 6817, 6818, 6819, 6820, 6821, 6822, 6879, 6882, 6883, 6884, 6886, 6887, 6889, 6891, 6900, 6901, 6902, 6903, 6904, 6905, 6906, 6907, 6908, 6909, 6910, 6911, 6915, 6916, 6918, 6919, 6920, 6921, 6923, 6924, 6926, 6927, 6928, 6929, 6930, 6931, 6932, 6933, 6934, 6935, 6936, 6937, 6939, 6940, 6968, 6970, 6972, 6973, 6975, 6976, 6977, 6978, 6979, 6980, 6981, 6982, 6983, 6984, 6990, 6991, 6992, 6993, 6994, 6995, 6997, 6998, 6999, 7000, 7002, 7003, 7004, 7005, 7006, 7008, 7009, 7010, 7011, 7012, 7030, 7031, 7032, 7033, 7034, 7035, 7037, 7038, 7040, 7041, 7042, 7043, 7044, 7046, 7047, 7048, 7049, 7050, 7051, 7052, 7053, 7054, 7055, 7056, 7075, 7076, 7077, 7078, 7081, 7082, 7083, 7084, 7086, 7087, 7091, 7092, 7093, 7094, 7096, 7097, 7098, 7100, 7101, 7102, 7103, 7104, 7125, 7126, 7127, 7128, 7129, 7130, 7131, 7132, 7133, 7135, 7136, 7137, 7138, 7146, 7149, 7150, 7151, 7154, 7155, 7156, 7157, 7159, 7160, 7161, 7162, 7163, 7164, 7165, 7166, 7167, 7168, 7173, 7174, 7175, 7176, 7177, 7178, 7179, 7180, 7181, 7182, 7184, 7186, 7187, 7188, 7191, 7192, 7193, 7194, 7195, 7200, 7202, 7203, 7205, 7206, 7207, 7208, 7209, 7211, 7212, 7213, 7214, 7215, 7216, 7217, 7218, 7219, 7220, 7223, 7225, 7255, 7258, 7261, 7263, 7265, 7267, 7268, 7269, 7271, 7272, 7274, 7276, 7278, 7279, 7281, 7284, 7286, 7288, 7290, 7291, 7312, 7313, 7320, 7321, 7322, 7323, 7325, 7326, 7327, 7328, 7337, 7342, 7343, 7344, 7345, 7346, 7348, 7349, 7351, 7352, 7353, 7354, 7355, 7356, 7357, 7358, 7359, 7361, 7363, 7364, 7365, 7367, 7369, 7374, 7376, 7377, 7378, 7379, 7380, 7381, 7382, 7386, 7387, 7388, 7389, 7390, 7391, 7392, 7393, 7395, 7396, 7397, 7398, 7399, 7400, 7401, 7402, 7403, 7405, 7406, 7408, 7409, 7410, 7411, 7412, 7413, 7414, 7415, 7417, 7418, 7419, 7421, 7422, 7423, 7425, 7427, 7428, 7429, 7431, 7432, 7433, 7434, 7435, 7437, 7439, 7440, 7441, 7442, 7444, 7445, 7446, 7447, 7449, 7451, 7452, 7453, 7454, 7455, 7456, 7457, 7458, 7460, 7461, 7469, 7471, 7472, 7473, 7474, 7477, 7478, 7480, 7481, 7482, 7483, 7484, 7487, 7488, 7489, 7490, 7491, 7492, 7493, 7495, 7497, 7498, 7500, 7501, 7502, 7513, 7514, 7515, 7517, 7519, 7520, 7521, 7528, 7529, 7530, 7531, 7532, 7533, 7534, 7537, 7538, 7539, 7542, 7543, 7545, 7546, 7547, 7549, 7550, 7551, 7552, 7553, 7554, 7564, 7565, 7566, 7567, 7569, 7570, 7572, 7573, 7574, 7575, 7577, 7579, 7581, 7584, 7585, 7586, 7587, 7590, 7591, 7592, 7596, 7597, 7599, 7600, 7604, 7605, 7606, 7607, 7608, 7609, 7611, 7612, 7613, 7622, 7623, 7625, 7626, 7627, 7628, 7630, 7631, 7633, 7634, 7635, 7636, 7638, 7639, 7640, 7641, 7643, 7644, 7645, 7646, 7647, 7649, 7655, 7656, 7657, 7659, 7660, 7662, 7663, 7664, 7665, 7666, 7668, 7669, 7670, 7671, 7673, 7674, 7675, 7676, 7677, 7678, 7679, 7681, 7682, 7684, 7685, 7687, 7688, 7689, 7690, 7692, 7693, 7694, 7696, 7698, 7700, 7701, 7702, 7703, 7704, 7705, 7710, 7712, 7713, 7714, 7715, 7716, 7717, 7718, 7721, 7723, 7724, 7725, 7726, 7727, 7728, 7729, 7730, 7732, 7733, 7734, 7735, 7736, 7737, 7751, 7752, 7753, 7755, 7756, 7761, 7762, 7763, 7764, 7766, 7767, 7768, 7769, 7771, 7772, 7775, 7776, 7778, 7779, 7780, 7781, 7783, 7784, 7785, 7786, 7788, 7789, 7790, 7791, 7794, 7795, 7796, 7797, 7798, 7801, 7802, 7803, 7814, 7816, 7819, 7821, 7824, 7825, 7828, 7829, 7830, 7831, 7832, 7834, 7835, 7836, 7842, 7843, 7845, 7846, 7847, 7848, 7850, 7851, 7852, 7853, 7855, 7856, 7857, 7858, 7859, 7865, 7868, 7869, 7870, 7871, 7873, 7874, 7876, 7877, 7878, 7880, 7882, 7883, 7884, 7885, 7886, 7887, 7890, 7891, 7892, 7893, 7894, 7896, 7897, 7898, 7899, 7900, 7901, 7903, 7904, 7905, 7906, 7907, 7908, 7910, 7911, 7912, 7913, 7914, 7931, 7932, 7933, 7935, 7937, 7938, 7939, 7941, 7942, 7944, 7948, 7949, 7950, 7952, 7954, 7955, 7957, 7958, 7960, 7961, 7962, 7963, 7965, 7966, 7967, 7975, 7977, 7978, 7979, 7981, 7982, 7986, 7988, 7989, 7990, 7991, 7992, 7995, 7996, 7997, 7998, 7999, 8000, 8001, 8002, 8004, 8005, 8006, 8007, 8008, 8009, 8013, 8014, 8015, 8016, 8018, 8020, 8021, 8022, 8023, 8026, 8027, 8028, 8029, 8030, 8031, 8032, 8033, 8038, 8039, 8040, 8041, 8042, 8044, 8045, 8046, 8047, 8048, 8050, 8051, 8052, 8053, 8055, 8056, 8057, 8058, 8059, 8061, 8063, 8064, 8065, 8066, 8068, 8069, 8070, 8071, 8072, 8074, 8075, 8076, 8077, 8079, 8080, 8081, 8082, 8084, 8086, 8087, 8088, 8089, 8090, 8097, 8100, 8101, 8102, 8104, 8106, 8108, 8109, 8110, 8111, 8112, 8113, 8115, 8116, 8117, 8118, 8119, 8121, 8122, 8123, 8124, 8129, 8130, 8131, 8133, 8137, 8138, 8139, 8140, 8141, 8142, 8144, 8156, 8157, 8158, 8159, 8160, 8161, 8162, 8165, 8166, 8167, 8168, 8169, 8170, 8173, 8174, 8175, 8176, 8177, 8180, 8183, 8184, 8186, 8188, 8190, 8191, 8192, 8193, 8195, 8196, 8198, 8199, 8201, 8202, 8203, 8204, 8205, 8206, 8207, 8208, 8209, 8210, 8211, 8212, 8213, 8215, 8217, 8219, 8220, 8221, 8228, 8229, 8230, 8231, 8235, 8236, 8238, 8239, 8240, 8242, 8244, 8245, 8246, 8247, 8248, 8250, 8251, 8253, 8254, 8255, 8256, 8257, 8258, 8265, 8266, 8267, 8268, 8270, 8271, 8273, 8274, 8275, 8276, 8277, 8278, 8279, 8281, 8284, 8286, 8287, 8288, 8289, 8290, 8292, 8293, 8294, 8295, 8297, 8298, 8299, 8300, 8305, 8306};
    std::vector<int> physical_runs{6667, 6668, 6669, 6670, 6671, 6672, 6673, 6674, 6675, 6676, 6677, 6678, 6679, 6680, 6681, 6683, 6684, 6685, 6686, 6687, 6689, 6690, 6691, 6692, 6694, 6695, 6696, 6698, 6699, 6732, 6733, 6734, 6737, 6738, 6739, 6740, 6745, 6752, 6753, 6760, 6761, 6765, 6766, 6767, 6768, 6769, 6771, 6772, 6773, 6774, 6779, 6780, 6782, 6783, 6785, 6786, 6788, 6794, 6795, 6797, 6799, 6800, 6803, 6815, 6816, 6817, 6818, 6819, 6820, 6821, 6822, 6879, 6882, 6883, 6884, 6886, 6887, 6889, 6891, 6900, 6901, 6902, 6903, 6904, 6905, 6906, 6907, 6908, 6909, 6910, 6911, 6915, 6916, 6918, 6919, 6920, 6921, 6923, 6924, 6926, 6927, 6928, 6929, 6930, 6931, 6932, 6933, 6934, 6935, 6936, 6937, 6939, 6940, 6968, 6970, 6972, 6973, 6975, 6976, 6977, 6978, 6979, 6980, 6981, 6982, 6983, 6984, 6990, 6991, 6992, 6993, 6994, 6995, 6997, 6998, 6999, 7000, 7002, 7003, 7004, 7005, 7006, 7008, 7009, 7010, 7011, 7012, 7030, 7031, 7032, 7033, 7034, 7035, 7037, 7038, 7040, 7041, 7042, 7043, 7044, 7046, 7047, 7048, 7049, 7050, 7051, 7052, 7053, 7054, 7055, 7056, 7075, 7076, 7077, 7078, 7081, 7082, 7083, 7084, 7086, 7087, 7091, 7092, 7093, 7094, 7096, 7097, 7098, 7100, 7101, 7102, 7103, 7104, 7125, 7126, 7127, 7128, 7129, 7130, 7131, 7132, 7133, 7135, 7136, 7137, 7138, 7146, 7149, 7150, 7151, 7154, 7155, 7156, 7157, 7159, 7160, 7161, 7162, 7163, 7164, 7165, 7166, 7167, 7168, 7173, 7174, 7175, 7176, 7177, 7178, 7179, 7180, 7181, 7182, 7184, 7186, 7187, 7188, 7191, 7192, 7193, 7194, 7195, 7200, 7202, 7203, 7205, 7206, 7207, 7208, 7209, 7211, 7212, 7213, 7214, 7215, 7216, 7217, 7218, 7219, 7220, 7223, 7225, 7255, 7258, 7261, 7263, 7265, 7267, 7268, 7269, 7271, 7272, 7274, 7276, 7278, 7279, 7281, 7284, 7286, 7288, 7290, 7291, 7312, 7313, 7320, 7321, 7322, 7323, 7325, 7326, 7327, 7328, 7337, 7342, 7343, 7344, 7345, 7346, 7348, 7349, 7351, 7352, 7353, 7354, 7355, 7356, 7357, 7358, 7359, 7361, 7363, 7364, 7365, 7367, 7369, 7374, 7376, 7377, 7378, 7379, 7380, 7381, 7382, 7386, 7387, 7388, 7389, 7390, 7391, 7392, 7393, 7395, 7396, 7397, 7398, 7399, 7400, 7401, 7402, 7403, 7405, 7406, 7408, 7409, 7410, 7411, 7412, 7413, 7414, 7415, 7417, 7418, 7419, 7421, 7422, 7423, 7425, 7427, 7428, 7429, 7431, 7432, 7433, 7434, 7435, 7437, 7439, 7440, 7441, 7442, 7444, 7445, 7446, 7447, 7449, 7451, 7452, 7453, 7454, 7455, 7456, 7457, 7458, 7460, 7461, 7469, 7471, 7472, 7473, 7474, 7477, 7478, 7480, 7481, 7482, 7483, 7484, 7487, 7488, 7489, 7490, 7491, 7492, 7493, 7495, 7497, 7498, 7500, 7501, 7502, 7513, 7514, 7515, 7517, 7519, 7520, 7521, 7528, 7529, 7530, 7531, 7532, 7533, 7534, 7537, 7538, 7539, 7542, 7543, 7545, 7546, 7547, 7549, 7550, 7551, 7552, 7553, 7554, 7564, 7565, 7566, 7567, 7569, 7570, 7572, 7573, 7574, 7575, 7577, 7579, 7581, 7584, 7585, 7586, 7587, 7590, 7591, 7592, 7596, 7597, 7599, 7600, 7604, 7605, 7606, 7607, 7608, 7609, 7611, 7612, 7613, 7622, 7623, 7625, 7626, 7627, 7628, 7630, 7631, 7633, 7634, 7635, 7636, 7638, 7639, 7640, 7641, 7643, 7644, 7645, 7646, 7647, 7649, 7655, 7656, 7657, 7659, 7660, 7662, 7663, 7664, 7665, 7666, 7668, 7669, 7670, 7671, 7673, 7674, 7675, 7676, 7677, 7678, 7679, 7681, 7682, 7684, 7685, 7687, 7688, 7689, 7690, 7692, 7693, 7694, 7696, 7698, 7700, 7701, 7702, 7703, 7704, 7705, 7710, 7712, 7713, 7714, 7715, 7716, 7717, 7718, 7721, 7723, 7724, 7725, 7726, 7727, 7728, 7729, 7730, 7732, 7733, 7734, 7735, 7736, 7737, 7751, 7752, 7753, 7755, 7756, 7761, 7762, 7763, 7764, 7766, 7767, 7768, 7769, 7771, 7772, 7775, 7776, 7778, 7779, 7780, 7781, 7783, 7784, 7785, 7786, 7788, 7789, 7790, 7791, 7794, 7795, 7796, 7797, 7798, 7801, 7802, 7803, 7814, 7816, 7819, 7821, 7824, 7825, 7828, 7829, 7830, 7831, 7832, 7834, 7835, 7836, 7842, 7843, 7845, 7846, 7847, 7848, 7850, 7851, 7852, 7853, 7855, 7856, 7857, 7858, 7859, 7865, 7868, 7869, 7870, 7871, 7873, 7874, 7876, 7877, 7878, 7880, 7882, 7883, 7884, 7885, 7886, 7887, 7890, 7891, 7892, 7893, 7894, 7896, 7897, 7898, 7899, 7900, 7901, 7903, 7904, 7905, 7906, 7907, 7908, 7910, 7911, 7912, 7913, 7914, 7931, 7932, 7933, 7935, 7937, 7938, 7939, 7941, 7942, 7944, 7948, 7949, 7950, 7952, 7954, 7955, 7957, 7958, 7960, 7961, 7962, 7963, 7965, 7966, 7967, 7975, 7977, 7978, 7979, 7981, 7982, 7986, 7988, 7989, 7990, 7991, 7992, 7995, 7996, 7997, 7998, 7999, 8000, 8001, 8002, 8004, 8005, 8006, 8007, 8008, 8009, 8013, 8014, 8015, 8016, 8018, 8020, 8021, 8022, 8023, 8026, 8027, 8028, 8029, 8030, 8031, 8032, 8033, 8038, 8039, 8040, 8041, 8042, 8044, 8045, 8046, 8047, 8048, 8050, 8051, 8052, 8053, 8055, 8056, 8057, 8058, 8059, 8061, 8063, 8064, 8065, 8066, 8068, 8069, 8070, 8071, 8072, 8074, 8075, 8076, 8077, 8079, 8080, 8081, 8082, 8084, 8086, 8087, 8088, 8089, 8090, 8097, 8100, 8101, 8102, 8104, 8106, 8108, 8109, 8110, 8111, 8112, 8113, 8115, 8116, 8117, 8118, 8119, 8121, 8122, 8123, 8124, 8129, 8130, 8131, 8133, 8137, 8138, 8139, 8140, 8141, 8142, 8144, 8156, 8157, 8158, 8159, 8160, 8161, 8162, 8165, 8166, 8167, 8168, 8169, 8170, 8173, 8174, 8175, 8176, 8177, 8180, 8183, 8184, 8186, 8188, 8190, 8191, 8192, 8193, 8195, 8196, 8198, 8199, 8201, 8202, 8203, 8204, 8205, 8206, 8207, 8208, 8209, 8210, 8211, 8212, 8213, 8215, 8217, 8219, 8220, 8221, 8222, 8223, 8225, 8226, 8228, 8229, 8230, 8231, 8235, 8236, 8238, 8239, 8240, 8242, 8244, 8245, 8246, 8247, 8248, 8250, 8251, 8253, 8254, 8255, 8256, 8257, 8258, 8265, 8266, 8267, 8268, 8270, 8271, 8273, 8274, 8275, 8276, 8277, 8278, 8279, 8281, 8284, 8286, 8287, 8288, 8289, 8290, 8292, 8293, 8294, 8295, 8297, 8298, 8299, 8300, 8305, 8306, 8319, 8320, 8321, 8322, 8323, 8324, 8326, 8329, 8330, 8331, 8332, 8334, 8335, 8338, 8339, 8343, 8344, 8363, 8364, 8366, 8367, 8368, 8369, 8370, 8371, 8373, 8374, 8375, 8379, 8380, 8381, 8382, 8384, 8386, 8387, 8388, 8389, 8390, 8392, 8393, 8394, 8395, 8398, 8399, 8401, 8402, 8412, 8413, 8414, 8415, 8419, 8420, 8421, 8422, 8425, 8426, 8427};
    std::vector<int> badFd_runs{7075, 7076, 7077, 7078, 7081, 7083, 7084, 7086, 7087, 7091, 7092, 7093, 7096, 7097, 7098, 7100, 7101, 7102, 7103, 7104};
    std::vector<int> empty_target{6603, 6605, 6645, 6646, 6647, 6648, 6649, 6650, 6651, 6653, 6654, 6655, 6700, 6701, 6703, 6704, 6705, 6706, 6709, 6736, 6770, 6778, 6880, 6925, 6938, 6986, 6987, 7045, 7057, 7095, 7123, 7142, 7143, 7145, 7171, 7172, 7189, 7190, 7210, 7264, 7282, 7350, 7362, 7385, 7394, 7407, 7416, 7424, 7430, 7438, 7443, 7450, 7459, 7479, 7485, 7486, 7494, 7499, 7506, 7507, 7508, 7527, 7540, 7541, 7548, 7580, 7588, 7598, 7610, 7632, 7637, 7648, 7661, 7667, 7672, 7680, 7686, 7691, 7708, 7722, 7731, 7739, 7740, 7765, 7770, 7777, 7782, 7787, 7793, 7800, 7815, 7820, 7822, 7823, 7837, 7849, 7854, 7866, 7867, 7875, 7879, 7888, 7895, 7902, 7909, 7915, 7947, 7951, 7959, 7964, 7971, 7987, 7994, 8003, 8010, 8017, 8024, 8043, 8049, 8054, 8062, 8067, 8073, 8078, 8085, 8098, 8099, 8105, 8107, 8114, 8120, 8135, 8136, 8143, 8145, 8146, 8148, 8149, 8163, 8172, 8181, 8189, 8194, 8200, 8214, 8224, 8237, 8243, 8249, 8261, 8262, 8272, 8282, 8283, 8285, 8291, 8296, 8303, 8327, 8333, 8336, 8372, 8376, 8377, 8378, 8383, 8391, 8396, 8409, 8411, 8416, 8423};

    // additional functions
    //Centrality
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

    // Vtx Z cirrections functions
    auto vtx_correction_generator = []( TGraphErrors* g1_calib ){
	   return [g1_calib](double _vtx, UInt_t _runId){return _vtx - g1_calib->Eval( static_cast<double>(_runId) ); };
	};
	
	auto ref_mult_generator = []( TGraphErrors* g1_calib ){
	   return [g1_calib](unsigned long _mult, UInt_t _runId){ return (_mult * g1_calib->Eval( static_cast<double>(_runId) )); };
	};
    
    //BD functions
	const auto nbdMod_fun = [](ROOT::RVecD vec_bdModAmp, ROOT::VecOps::RVec<short> vec_bdModId){
		ROOT::RVecD vec_nbdMod{};
		vec_nbdMod.reserve( vec_bdModAmp.size() );
		for( int i=0; i<vec_bdModAmp.size(); ++i ){
			int mod = vec_bdModId.at(i);
			vec_nbdMod.push_back( mod );
		}
		return vec_nbdMod;
	};

	const auto nbdModSingle_fun = [](ROOT::RVecD vec_bdModAmp, ROOT::VecOps::RVec<short> vec_bdModId){
		ROOT::RVecD vec_nbdMod{};
		int n[70]={0};
		vec_nbdMod.reserve( vec_bdModAmp.size() );
		for( int i=0; i<vec_bdModAmp.size(); ++i ){
			int mod = vec_bdModId.at(i);
			n[mod]++;
		}
		for(int i=0; i<70; i++){
			if(n[i]>0)vec_nbdMod.push_back(i);
		}
		return vec_nbdMod;
	};

	const auto bd_numSingle_fun = [](ROOT::RVecD vec_bdModAmp, ROOT::VecOps::RVec<short> vec_bdModId){
		ROOT::RVecD vec_nbdMod{};
		int n[70]={0};
		vec_nbdMod.reserve( vec_bdModAmp.size() );
		for( int i=0; i<vec_bdModAmp.size(); ++i ){
			int mod = vec_bdModId.at(i);
			n[mod]++;
		}
		int nbd1 = 0;
		for(int i=0; i<70; i++){
			if(n[i]>0)nbd1++;;
		}
		return nbd1;
	};

	// FHCal functions
	const auto function_fhcal_x = [FHCAL_Z]( ROOT::VecOps::RVec<std::vector<float>> vec_param ){
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

	const auto function_fhcal_y = [FHCAL_Z]( ROOT::VecOps::RVec<vector<float>> vec_param ){
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

	// Tracks functions
	const auto dca_function = [](ROOT::RVecF vec_x, ROOT::RVecF vec_y){
		ROOT::RVecF vec_r{};
		vec_r.reserve(vec_x.size());
		for (int i=0; i<vec_x.size(); ++i) {
			auto x = vec_x.at(i);
			auto y = vec_y.at(i);
			auto r = std::sqrt( x*x + y*y );
			vec_r.push_back(r);
		}
		return vec_r;
	};

	const auto n_sigma_generator = []( auto f1_mean, auto f1_sigma ){
	    return 
	    [ f1_mean, f1_sigma ]
	    ( ROOT::RVecF vec_pq, ROOT::VecOps::RVec<float> vec_m2 ){
	        auto vec_n_sigma = std::vector<float>{};
	        vec_n_sigma.reserve( vec_pq.size() );
	        for( size_t i=0; i < vec_pq.size(); ++i ){
	          auto m2 = vec_m2.at(i);
	          auto pq = vec_pq.at(i);
	          auto mean = f1_mean->Eval(pq);
	          auto sigma = f1_sigma->Eval(pq);
	          auto n_sigma = ( m2 - mean ) / sigma;
	          vec_n_sigma.push_back( pq > 0 ? n_sigma : 999. );
	        }
	      return vec_n_sigma;
	    };
	  };

	// TOF 
	const auto is_particle_tof_generator = []( const double lo, const double hi ){
		return [lo, hi]( std::vector<float> vec_tof ){
		    	auto vec_is = std::vector<int>{};
		    	vec_is.reserve( vec_tof.size() );
		    	std::for_each( vec_tof.begin(), vec_tof.end(), [&vec_is, lo, hi](const auto nsigma) mutable { vec_is.push_back( lo < nsigma && nsigma < hi ? 1 : 0 ); } );
		    	return vec_is;
		    };
	};

	const auto IsParticle = []( std::vector<int> vec_400, std::vector<int> vec_700 ){
	    	auto vec_is = std::vector<int>( vec_400.size(), 0 );
	    	for( size_t i=0; i<vec_400.size(); ++i ){
	    		if( vec_400.at(i) == 1 )
	    			vec_is.at(i) = 1;
	    		if( vec_700.at(i) == 1 )
	    			vec_is.at(i) = 1;
	    	}
	    	return vec_is;
	    };
	
	const auto IsGoodParticle = []( 
		std::vector<int> vec_is_particle, 
		ROOT::RVecF vec_dca_r,
		ROOT::VecOps::RVec<int> vec_nhits,
		ROOT::VecOps::RVec<float> vec_chi2,
		std::vector<float> vec_fhcal_x, 
		std::vector<float> vec_fhcal_y 
	){
		auto vec_is = std::vector<double>( vec_is_particle.size(), 0 );
		for( int i=0; i<vec_is_particle.size(); ++i ){
			if( vec_dca_r.at(i) > 5 ) continue;
			if( vec_nhits.at(i) < 5 ) continue;
			if( vec_fhcal_x.at(i) > -30.0 && vec_fhcal_x.at(i) < 160 &&
				vec_fhcal_y.at(i) > -60.0 && vec_fhcal_y.at(i) < 60 ) continue;
			vec_is.at(i) = vec_is_particle.at(i);
		}
		return vec_is;
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

	auto PhisRunCut = [&physical_runs]( UInt_t run_id ){return std::find( physical_runs.begin(), physical_runs.end(), run_id) != physical_runs.end();};
	auto BRCut = []( ROOT::VecOps::RVec<unsigned int> map ){ return map[0] & (1<<5); };	//
	auto ARCut = []( ROOT::VecOps::RVec<unsigned int> map ){ return map[0] & (1<<7); }; //
	auto ARCutMinB = []( ROOT::VecOps::RVec<unsigned int> map ){ return map[0] & (1<<5); }; // AR - 5 бит minimum-bias
	
	auto bc1fdCut = []( ROOT::VecOps::RVec<float> bc1_int, ROOT::VecOps::RVec<float> fd_int ){
		float trigger = fd_int[0]-bc1_int[0]*0.602;
		return -25900 < trigger && trigger < -6030;
	};

	auto bc1fdCut_new = [](double x, double y){
		std::vector<double> meanX = {1.,2.,3.,4.,5.,6.,7.,8.,9.};
		std::vector<double> meanY = {0.,1.,2.,3.,4.,5.,6.,7.,8.};
		double sigma = 0.5;
		bool _key = false;
		for(int i=0; i<meanX.size(); i++){
			if(x > (meanX[i] - sigma) && x < (meanX[i] + sigma) && y > (meanY[i] - sigma) && y < (meanY[i] + sigma)) _key=true;
		}
		return _key;
	};

	auto Tof701_mod30 = []( ROOT::VecOps::RVec<short> tof_mod ){ 
		for(auto &mod : tof_mod)
			if(mod==30) return false;
		return true; 
	};

	auto Tof701_strip30 = []( ROOT::VecOps::RVec<short> tof_mod ){ 
		for(auto &mod : tof_mod)
			if(mod==30) return false;
		return true; 
	};

	auto bc1Cut = []( ROOT::VecOps::RVec<float> bc1_int ){ return 1e4 < bc1_int[0] && bc1_int[0] < 4e4; };
	auto stsNdigitsMultCut = []( unsigned long sts_digits, unsigned long n_tracks ){ 
		double sts_min = sts_digits-n_tracks*(4.81632+0.0332792*n_tracks-9.62078e-05*n_tracks*n_tracks);
		double sts_max = sts_digits-n_tracks*(19.4203-0.0518774*n_tracks+4.56033e-05*n_tracks*n_tracks); // 19.4203
		return -74.0087 < sts_min && sts_max < 188.248;//188.248 
	};

	auto stsNdigitsMultCut2 = []( unsigned long sts_digits, unsigned long n_tracks ){ 
		double sts_min = sts_digits-n_tracks*(17.6212-0.065777*n_tracks+8.84404e-05*n_tracks*n_tracks);
		double sts_max = sts_digits-n_tracks*(21.6309-0.0774261*n_tracks+0.000141619*n_tracks*n_tracks);
		return -549.584 < sts_min && sts_max < 354.607; 
	};


	auto dd = d
		//event
		.Filter("runId < 8312")
		.Filter("runId > 7016")
		
		.Redefine("k0cut", "k0closest.at(0)<-200 && k0closest.at(1)>200") 
    	.Redefine("kV0cut", "kV0closest.at(0)<-200 && kV0closest.at(1)>200") 
    	.Redefine("k1cut", "k1closest.at(0)<-1600 && k1closest.at(1)>600") 
    	.Redefine("kV1cut", "kV1closest.at(0)<-1800 && kV1closest.at(1)>600")
    	.Redefine("noPileup", "if(k1central && k0cut && kV0cut && k1cut && kV1cut) return 1; else if(k1central) return -1; else return 0;")
		
		.Define( "track_multiplicity", "return trMom.size();" )
		.Define( "ref1_multiplicity", ref_mult_generator( g1_FitRunIdFactor_1 ), {"track_multiplicity", "runId"} )
		.Define( "ref2_multiplicity", ref_mult_generator( g1_FitRunIdFactor_2 ), {"track_multiplicity", "runId"} )
		.Define( "sts_track_multiplicity", "return stsTrackMomentum.size();" )
		.Define( "vtx_chi2_ndf", "return vtxChi2/vtxNdf;" )
		.Define( "vtx_chi2_ndfMpd", "return vtxChi2Mpd/vtxNdfMpd;" )
		//Vertex
		.Define( "vtxRMpd",	  "return sqrt(vtxXMpd*vtxXMpd + vtxYMpd*vtxYMpd);" )
		.Define( "vtxR", 	  "return sqrt(vtxX*vtxX + vtxY*vtxY);" )
		.Define( "vtxXcorr", vtx_correction_generator(g1_FitVtxX), {"vtxX","runId"})
        .Define( "vtxYcorr", vtx_correction_generator(g1_FitVtxY), {"vtxY","runId"})
        .Define( "vtxZcorr", vtx_correction_generator(g1_FitVtxZ), {"vtxZ","runId"})
		.Define( "vtxRcorr", "return sqrt(vtxXcorr*vtxXcorr + vtxYcorr*vtxYcorr);" )
		// Weight RunId
		.Define( "RunIdWeight", "return 1.0")
		// Weight VtxZ
		.Define( "VtxZWeight", "return 1.0")
		// Wight RunId and VtxZ
		.Define("RunIdVtxZWeight","return 1.0")
		//Digit
		.Define( "gemNdigits","return gemDigits.size()" )
		.Define( "stsNdigits","return stsDigits.size()" )
		.Define( "tof400Ndigits","return tof400Digits.size()" )
		.Define( "tof700Ndigits","return tof700Digits.size()" )
		//.Define( "tof701Ndigits","return tof701Digits.size()" )
		.Define( "Tof400_multiplicity", "return trBetaTof400.size()" )
		.Define( "Tof700_multiplicity", "return trBetaTof700.size()" )
		//.Define( "Tof701_multiplicity", "return trBetaTof701.size()" )
		// BC1S
		.Define( "bc1sIntegralNorm",[f1_FitBC1](float _bc1, UInt_t _runId){ return _bc1 / f1_FitBC1->Eval((double)_runId);},{"bc1sIntegral","runId"} )
		.Define( "fdIntegralNorm",	[f1_FitFD] (float _fd,  UInt_t _runId){ return _fd  / f1_FitFD ->Eval((double)_runId);},{"fdIntegral",	"runId"} )
		// Triggers
		.Define( "bc1sTdcHits", " return bc1sTdcTimes.size(); " )
		.Define( "bc1tTdcHits", " return bc1tTdcTimes.size(); " )
		.Define( "bc1bTdcHits", " return bc1bTdcTimes.size(); " )
		.Define( "vcsTdcHits", 	" return vcsTdcTimes.size(); " )
		.Define( "fdTdcHits", 	" return fdTdcTimes.size(); " )
		//BD
		.Define( "bd_num", 		  " return bdModAmp.size(); " )
		.Define( "nbdMod", nbdMod_fun, {"bdModAmp","bdModId"})
		.Define( "nbdModSingle", nbdModSingle_fun, {"bdModAmp","bdModId"} )
		.Define( "bd_numSingle", bd_numSingle_fun, {"bdModAmp","bdModId"} )
		//BeamHit and track
		.Define( "beamHitX_St0_mult"," int hit{}; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==0) hit=hit+1; } return hit;" )
		.Define( "beamHitX_St1_mult"," int hit{}; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==1) hit=hit+1; } return hit;" )
		.Define( "beamHitX_St2_mult"," int hit{}; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==2) hit=hit+1; } return hit;" )
		.Define( "beamHitX_St0", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==0) hit.push_back( beamHitXYZ.at(i).X() ); } return hit;" )
		.Define( "beamHitY_St0", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==0) hit.push_back( beamHitXYZ.at(i).Y() ); } return hit;" )
		.Define( "beamHitX_St1", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==1) hit.push_back( cos(TMath::Pi()/6.)*beamHitXYZ.at(i).X() - sin(TMath::Pi()/6.)*beamHitXYZ.at(i).Y() ); } return hit;" )
		.Define( "beamHitY_St1", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==1) hit.push_back( sin(TMath::Pi()/6.)*beamHitXYZ.at(i).X() + cos(TMath::Pi()/6.)*beamHitXYZ.at(i).Y() ); } return hit;" )
		.Define( "beamHitX_St2", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==2) hit.push_back( cos(TMath::Pi()/3.)*beamHitXYZ.at(i).X() - sin(TMath::Pi()/3.)*beamHitXYZ.at(i).Y() ); } return hit;" )
		.Define( "beamHitY_St2", " ROOT::RVecF hit; for( int i=0; i<beamHitStation.size(); i++ ){ if(beamHitStation.at(i)==2) hit.push_back( sin(TMath::Pi()/3.)*beamHitXYZ.at(i).X() + cos(TMath::Pi()/3.)*beamHitXYZ.at(i).Y() ); } return hit;" )
		.Define( "beam_x", " ROOT::RVecF beam_x; for( auto bm_tr : beamTrackParameters ){ beam_x.push_back( bm_tr.at(0) ); } return beam_x; " )
		.Define( "beam_y", " ROOT::RVecF beam_y; for( auto bm_tr : beamTrackParameters ){ beam_y.push_back( bm_tr.at(1) ); } return beam_y; " )
		// Global Tracks
		.Define( "p",  " ROOT::RVecF p;   for( auto mom : trMom ){ p.push_back( mom.P() ); } 	 return p; " )
		.Define( "px", " ROOT::RVecF px;  for( auto mom : trMom ){ px.push_back( mom.Px() ); } 	 return px; " )
		.Define( "py", " ROOT::RVecF py;  for( auto mom : trMom ){ py.push_back( mom.Py() ); } 	 return py; " )
		.Define( "pz", " ROOT::RVecF pz;  for( auto mom : trMom ){ pz.push_back( mom.Pz() ); } 	 return pz; " )
		.Define( "pT", " ROOT::RVecF pT;  for( auto mom : trMom ){ pT.push_back( mom.Pt() ); } 	 return pT; " )
		.Define( "phi"," ROOT::RVecF phi; for( auto mom : trMom ){ phi.push_back( mom.Phi() ); } return phi; " )
		.Define( "eta"," ROOT::RVecF eta; for( auto mom : trMom ){ eta.push_back( mom.Eta() ); } return eta; " )
		.Define( "pq", " ROOT::RVecF pq;  for( int i=0; i<trMom.size(); i++ ){ pq.push_back( trMom.at(i).P()/trCharge.at(i) ); } return pq;" )
		.Define( "trFhcalX", function_fhcal_x, {"trParamLast"} )
        .Define( "trFhcalY", function_fhcal_y, {"trParamLast"} )
        .Define( "trDcaX", " ROOT::RVecF vec_par; for( auto par : globalTrackParameters ){ vec_par.push_back( par.at(0) - vtxX ); } return vec_par; " )
      	.Define( "trDcaY", " ROOT::RVecF vec_par; for( auto par : globalTrackParameters ){ vec_par.push_back( par.at(1) - vtxY ); } return vec_par; " )
        .Define( "trDcaR", dca_function, {"trDcaX", "trDcaY"} )
        .Define( "tr_DcaX", 	" ROOT::RVecF tr_DcaX; for( int i=0; i<trDca.size(); ++i ){ tr_DcaX.push_back( trDca.at(i).X()  ); } return tr_DcaX; " )
		.Define( "tr_DcaY", 	" ROOT::RVecF tr_DcaY; for( int i=0; i<trDca.size(); ++i ){ tr_DcaY.push_back( trDca.at(i).Y()  ); } return tr_DcaY; " )
		.Define( "tr_DcaZ", 	" ROOT::RVecF tr_DcaZ; for( int i=0; i<trDca.size(); ++i ){ tr_DcaZ.push_back( trDca.at(i).Z()  ); } return tr_DcaZ; " )
		.Define( "tr_chi2_ndf", " ROOT::RVecF vec_par; for( int i=0; i<trChi2.size(); ++i ){ vec_par.push_back( trChi2.at(i) / trNdf.at(i) ); } return vec_par; " )
		.Define( "tr_x", 	  	" ROOT::RVecF vec_par; for( auto par : globalTrackParameters ){ vec_par.push_back( par.at(0) ); } return vec_par; " )
		.Define( "tr_y", 		" ROOT::RVecF vec_par; for( auto par : globalTrackParameters ){ vec_par.push_back( par.at(1) ); } return vec_par; " )
		.Define( "tr_z", 		" ROOT::RVecF vec_par; for( auto par : globalTrackParameters ){ vec_par.push_back( par.at(2) ); } return vec_par; " )
		.Define( "tr_dca_x", 	" ROOT::RVecF vec_par; for( auto par : globalTrackParameters ){ vec_par.push_back( par.at(0) - vtxX ); } return vec_par; " )
		.Define( "tr_dca_y", 	" ROOT::RVecF vec_par; for( auto par : globalTrackParameters ){ vec_par.push_back( par.at(1) - vtxY ); } return vec_par; " )
		.Define( "tr_dca_z",	" ROOT::RVecF vec_par; for( auto par : globalTrackParameters ){ vec_par.push_back( par.at(2) - vtxZ ); } return vec_par; " )
        .Define( "tr_dca_r", dca_function, {"tr_dca_x", "tr_dca_y"} )
        .Define( "tr_dca_xMpd", " ROOT::RVecF vec_par; for( auto par : globalTrackParameters ){ vec_par.push_back( par.at(0) - vtxXMpd ); } return vec_par; " )
		.Define( "tr_dca_yMpd", " ROOT::RVecF vec_par; for( auto par : globalTrackParameters ){ vec_par.push_back( par.at(1) - vtxYMpd ); } return vec_par; " )
		.Define( "tr_dca_zMpd",	" ROOT::RVecF vec_par; for( auto par : globalTrackParameters ){ vec_par.push_back( par.at(2) - vtxZMpd ); } return vec_par; " )
        .Define( "tr_dca_rMpd", dca_function, {"tr_dca_xMpd", "tr_dca_yMpd"} )
        //TOF
		.Define( "Tof400Matched", "int Ntr{}; for( int i=0; i<trMom.size(); i++ ){ if(trBetaTof400.at(i) > -990.)Ntr++; } return Ntr; " )
		.Define( "Tof700Matched", "int Ntr{}; for( int i=0; i<trMom.size(); i++ ){ if(trBetaTof700.at(i) > -990.)Ntr++; } return Ntr; " )		
		//.Define( "Tof701Matched", "int Ntr{}; for( int i=0; i<trMom.size(); i++ ){ if(trBetaTof701.at(i) > -990.)Ntr++; } return Ntr; " )		
		.Define( "beta400", " ROOT::RVecF beta; for( int i=0; i<trMom.size(); i++ ){auto b = trBetaTof400.at(i); beta.push_back( b > -990. ? 1 / b : -999.0 ) ; } return beta;" )
		.Define( "beta700", " ROOT::RVecF beta; for( int i=0; i<trMom.size(); i++ ){auto b = trBetaTof700.at(i); beta.push_back( b > -990. ? 1 / b : -999.0 ) ; } return beta;" )
		//.Define( "beta701", " ROOT::RVecF beta; for( int i=0; i<trMom.size(); i++ ){auto b = trBetaTof701.at(i); beta.push_back( b > -990. ? 1 / b : -999.0 ) ; } return beta;" )
		
		.Define( "trNsigma2212M2Tof400", 		n_sigma_generator(f1_2212_m_400, f1_2212_s_400), 			{ "pq", "trM2Tof400" } )
		.Define( "trNsigma211M2Tof400", 		n_sigma_generator(f1_211_m_400, f1_211_s_400), 				{ "pq", "trM2Tof400" } )
		.Define( "trNsigma1000010020M2Tof400",	n_sigma_generator(f1_1000010020_m_400, f1_1000010020_s_400),{ "pq", "trM2Tof400" } )
		.Define( "trNsigma1000010030M2Tof400",	n_sigma_generator(f1_1000010030_m_400, f1_1000010030_s_400),{ "pq", "trM2Tof400" } )
		.Define( "trNsigma2212M2Tof700", 		n_sigma_generator(f1_2212_m_700, f1_2212_s_700), 			{ "pq", "trM2Tof700" } )
		.Define( "trNsigma211M2Tof700", 		n_sigma_generator(f1_211_m_700, f1_211_s_700), 				{ "pq", "trM2Tof700" } )
		.Define( "trNsigma1000010020M2Tof700",	n_sigma_generator(f1_1000010020_m_700, f1_1000010020_s_700),{ "pq", "trM2Tof700" } )
		.Define( "trNsigma1000010030M2Tof700",	n_sigma_generator(f1_1000010030_m_700, f1_1000010030_s_700),{ "pq", "trM2Tof700" } )
		
		.Define( "trIsProtonTof400", 	is_particle_tof_generator(-3, 3), {"trNsigma2212M2Tof400"} )
		.Define( "trIsProtonTof700", 	is_particle_tof_generator(-3, 3), {"trNsigma2212M2Tof700"} )
		.Define( "trIsProton", 			IsParticle, {"trIsProtonTof400", "trIsProtonTof700"} )
		.Define( "trIsGoodProton", 		IsGoodParticle, {"trIsProton", "tr_dca_r", "stsTrackNhits", "stsTrackChi2Ndf", "trFhcalX", "trFhcalY"} )
		.Define( "trIsDeuteronTof400",	is_particle_tof_generator(-3, 3), {"trNsigma1000010020M2Tof400"} )
		.Define( "trIsDeuteronTof700",	is_particle_tof_generator(-1, 3), {"trNsigma1000010020M2Tof700"} )
		.Define( "trIsDeuteron", 		IsParticle, {"trIsDeuteronTof400", "trIsDeuteronTof700"} )
		.Define( "trIsTritonTof400",	is_particle_tof_generator(-3, 3), {"trNsigma1000010030M2Tof400"} )
		.Define( "trIsTritonTof700",	is_particle_tof_generator(-3, 3), {"trNsigma1000010030M2Tof700"} )
		.Define( "trIsTriton", IsParticle, {"trIsTritonTof400", "trIsTritonTof700"} )
		.Define( "trProtonY", rapidity_generator(PROTON_M, Y_CM), {"pz", "pq"} )
		.Define( "trDeuteronY", rapidity_generator(DEUTERON_M, Y_CM), {"pz", "pq"} )
		.Define( "trTritonY", rapidity_generator(TRITON_M, Y_CM), {"pz", "pq"} )
		// STS tracks
		.Define( "sts_x", 	" ROOT::RVecF sts_x;  for( auto par : stsTrackParameters ){ sts_x.push_back( par.at(0) ); } return sts_x; " )
		.Define( "sts_y", 	" ROOT::RVecF sts_y;  for( auto par : stsTrackParameters ){ sts_y.push_back( par.at(1) ); } return sts_y; " )
		.Define( "sts_z", 	" ROOT::RVecF sts_z;  for( auto par : stsTrackParameters ){ sts_z.push_back( par.at(2) ); } return sts_z; " )
		.Define( "sts_px", 	" ROOT::RVecF sts_px; for( auto mom : stsTrackMomentum ){ sts_px.push_back( mom.Px() ); } return sts_px; " )
		.Define( "sts_py", 	" ROOT::RVecF sts_py; for( auto mom : stsTrackMomentum ){ sts_py.push_back( mom.Py() ); } return sts_py; " )
		.Define( "sts_pz", 	" ROOT::RVecF sts_pz; for( auto mom : stsTrackMomentum ){ sts_pz.push_back( mom.Pz() ); } return sts_pz; " )
		.Define( "sts_pT", 	" ROOT::RVecF sts_pT; for( auto mom : stsTrackMomentum ){ sts_pT.push_back( mom.Pt() ); } return sts_pT; " )
		.Define( "sts_p", 	" ROOT::RVecF sts_p;  for( auto mom : stsTrackMomentum ){ sts_p.push_back( mom.P() ); }   return sts_p; " )
		.Define( "sts_eta", " ROOT::RVecF sts_eta;for( auto mom : stsTrackMomentum ){ sts_eta.push_back( mom.Eta() ); } return sts_eta; " )
		.Define( "sts_phi", " ROOT::RVecF sts_phi;for( auto mom : stsTrackMomentum ){ sts_phi.push_back( mom.Phi() ); } return sts_phi; " )
		.Define( "tx_err", 	" ROOT::RVecF tx_err; for( auto cov : globalTrackCovMatrix ){ tx_err.push_back( cov.at(5) ); } return tx_err;")
		.Define( "ty_err", 	" ROOT::RVecF ty_err; for( auto cov : globalTrackCovMatrix ){ ty_err.push_back( cov.at(9) ); } return ty_err;")
		.Define( "qp_err", 	" ROOT::RVecF qp_err; for( auto cov : globalTrackCovMatrix ){ qp_err.push_back( cov.at(14) ); } return qp_err;")		 
		// BeamPipe
		// FHCal modules
		.Define( "fhcal_x", " ROOT::RVecF fhcal_x; for( auto pos : fhcalModPos ){ fhcal_x.push_back( pos.X() ); } return fhcal_x; " )
		.Define( "fhcal_y", " ROOT::RVecF fhcal_y; for( auto pos : fhcalModPos ){ fhcal_y.push_back( pos.Y() ); } return fhcal_y; " )
		.Define( "fhcalSumE", "float fhcalE{}; for( auto modE : fhcalModE ){ fhcalE+=modE; } return fhcalE; " )
		.Define( "scwallSumQ", "float scwallQ{}; for( auto modQ : scwallModQ ){ scwallQ+=modQ; } return scwallQ; " )
		.Define( "hodoSumQ", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return hodoQ; " )
		.Define( "fhcalhodoSumE", "float fhcalE{}; for( auto modId = 0; modId < (fhcalModId.size()-1); modId++ ){ fhcalE+=fhcalModE.at(modId); } float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return fhcalE+0.35*hodoQ; " )
		.Define( "hodoIsCentral", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return (hodoQ<40); " )
		.Define( "hodoIsPeripheral", "float hodoQ{}; for( auto modQ : hodoModQ ){ hodoQ+=modQ; } return (hodoQ>=40); " )
		// M2
		.Define("good_track", "pT>0.02 && eta<3.0 && trNhits>4")
		.Define("pq_good","pq[good_track]")
		.Define("trBetaTof400_good","trBetaTof400[good_track]")
		.Define("trBetaTof700_good","trBetaTof700[good_track]")
		//.Define("trBetaTof701_good","trBetaTof701[good_track]")
		//.Define("trBetaTof701_2_good","trBetaTof701_2[good_track]")
		.Define("trM2Tof400_good",	"trM2Tof400[good_track]")
		.Define("trM2Tof700_good",	"trM2Tof700[good_track]")
		//.Define("trM2Tof701_good",	"trM2Tof701[good_track]")
		//.Define("trM2Tof701_2_good","trM2Tof701_2[good_track]")
		//Centrality
		.Define("Cent8_8120_8170",	[GetCentrBin_8120_8170]	(Double_t _refMult) { return GetCentrBin_8120_8170(_refMult);},{"ref1_multiplicity"})
		.Define("Cent8_7400_7450",	[GetCentrBin_7400_7450]	(Double_t _refMult) { return GetCentrBin_7400_7450(_refMult);},{"ref2_multiplicity"})
		.Define("Cent12_8120_8170",	[GetCentrBin2_8120_8170] (Double_t _refMult) { return GetCentrBin2_8120_8170(_refMult);},{"ref1_multiplicity"})
		.Define("Cent12_7400_7450",	[GetCentrBin2_7400_7450] (Double_t _refMult) { return GetCentrBin2_7400_7450(_refMult);},{"ref2_multiplicity"})
		;

	
	cout << "Number of Events: " << *(dd.Count()) << endl;

	// Out File
	auto file_out = TFile::Open( out_file_name.c_str(), "RECREATE" );
	file_out->cd();

	AddStandartHisto(dd,"def_",file_out);

	// vertex cut
	auto dd_def = dd
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtx_chi2_ndfMpd > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks >= 2")
		.Filter("vtxNtracksMpd >= 2")
		;

	AddStandartHisto(dd_def,"nVtxTr_",file_out);
  	AddHistoGraphCutvsRunId2(dd_def,"nVtxTr_",file_out,GetRunIdForJob(dd_def));


	// plp cut 
	auto dd_plp = dd
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtx_chi2_ndfMpd > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks >= 2")
		.Filter("vtxNtracksMpd >= 2")
		.Filter("noPileup==1")
		;
	AddStandartHisto(dd_plp,"nVtxTr_plp_",file_out);
  	AddHistoGraphCutvsRunId2(dd_plp,"nVtxTr_plp_",file_out,GetRunIdForJob(dd_plp));



/*
	// plp cut (k0cur)
	auto dd_k0 = dd
		//.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks >= 2")
		.Filter("k0cut==1")
		;

	AddStandartHisto(dd_k0,"nVtxTr_k0_",file_out);
  	AddHistoGraphCutvsRunId2(dd_k0,"nVtxTr_k0_",file_out,GetRunIdForJob(dd_def));

	// plp cut
	auto dd_2 = dd
		//.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks >= 2")
		.Filter("noPileup==1")
		;

	AddStandartHisto(dd_2,"nVtxTr_plp_",file_out);
  	AddHistoGraphCutvsRunId2(dd_2,"nVtxTr_plp_",file_out,GetRunIdForJob(dd_def));

	// graph cut
	auto dd_3 = dd
		//.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks >= 2")
		.Filter("noPileup==1")
		.Filter(stsNdigitsMultCut2,{"stsNdigits", "track_multiplicity"})
		;

	AddStandartHisto(dd_3,"nVtxTr_plp_gr_",file_out);
  	AddHistoGraphCutvsRunId2(dd_3,"nVtxTr_plp_gr_",file_out,GetRunIdForJob(dd_def));

	// additional VtxR and VtxZ cuts
	auto dd_4 = dd
		.Filter(PhisRunCut,{"runId"} )
		.Filter(ARCut, {"triggerMapAR"}) //triggerMapAR
		.Filter("vtx_chi2_ndf > std::numeric_limits<float>::min()")
		.Filter("vtxNtracks >= 2")
		.Filter("noPileup==1")
		.Filter(stsNdigitsMultCut2,{"stsNdigits", "track_multiplicity"})
		.Filter("vtxRcorr < 1.0")
		.Filter("sqrt(vtxZcorr*vtxZcorr) < 0.1")
		;

	AddStandartHisto(dd_4,"nVtxTr_plp_VtxR1_VtxZ010_",file_out);

*/
 	std::map<float,string> bin_cut_txt={
  		{0,"Phys Runs (Raw)"},
  		{1,"CCT2"},
  		{2,"nTracks_{Vtx}>1"},
  		{3,"pile-up (k0cut)"},
  		{4,"pile-up (all)"},
  		{5,"Graph cut"},  		
  		{6,"Vtx_{R}<1.0cm"},
  		{7,"Vtx_{Z}<0.1cm"}
  	};
	int use_cuts = bin_cut_txt.size();

	auto dd_cut = dd
		.Define( "NumberCuts", [stsNdigitsMultCut2,PhisRunCut](
				UInt_t run_id,
				ROOT::VecOps::RVec<unsigned int> map, 
				Int_t vtxNtracks,
				bool _k0cut, 
				int noPileup, 
				Double_t vtxRcorr, 
				Double_t vtxZcorr,
				unsigned long sts_digits, 
				unsigned long n_tracks)
		{ 
			ROOT::RVecF vec_r{};
			vec_r.push_back(-1.);
			
			auto phis = PhisRunCut(run_id);
			auto cct2 = ( map[0] & (1<<7));
			auto vtx_ntrack = ( vtxNtracks >= 2 );
			auto pileup_k0 = (_k0cut==1) ? true : false;
			auto pileup = (_k0cut==1) ? true : false;
			auto VtxR = (vtxRcorr < 1.0);
			auto VtxZ = (vtxZcorr < 0.1);
			auto GrCut = stsNdigitsMultCut2(sts_digits,n_tracks);

			if(phis) vec_r.push_back(0.);
			if(phis && cct2) vec_r.push_back(1.);
			if(phis && cct2 && vtx_ntrack) vec_r.push_back(2.);
			if(phis && cct2 && vtx_ntrack && pileup_k0) vec_r.push_back(3.);
			if(phis && cct2 && vtx_ntrack && pileup_k0 && pileup) vec_r.push_back(4.);
			if(phis && cct2 && vtx_ntrack && pileup_k0 && pileup && GrCut) vec_r.push_back(5.);
			if(phis && cct2 && vtx_ntrack && pileup_k0 && pileup && GrCut && VtxR) vec_r.push_back(6.);
			if(phis && cct2 && vtx_ntrack && pileup_k0 && pileup && GrCut && VtxR && VtxZ) vec_r.push_back(7.);

			return vec_r;
		}, { "runId","triggerMapAR","vtxNtracks","k0cut","noPileup","vtxRcorr","vtxZcorr","stsNdigits","track_multiplicity"})
		.Define( "NumberCuts_Not", [stsNdigitsMultCut2,PhisRunCut](
				UInt_t run_id,
				ROOT::VecOps::RVec<unsigned int> map, 
				Int_t vtxNtracks,
				bool _k0cut, 
				int noPileup, 
				Double_t vtxRcorr, 
				Double_t vtxZcorr,
				unsigned long sts_digits, 
				unsigned long n_tracks)
		{ 
			ROOT::RVecF vec_r{};
			vec_r.push_back(-1.);
			
			auto phis = PhisRunCut(run_id);
			auto cct2 = ( map[0] & (1<<7));
			auto vtx_ntrack = ( vtxNtracks >= 2 );
			auto pileup_k0 = (_k0cut==1) ? true : false;
			auto pileup = (noPileup==1) ? true : false;
			auto VtxR = (vtxRcorr < 1.0);
			auto VtxZ = (vtxZcorr < 0.1);
			auto GrCut = stsNdigitsMultCut2(sts_digits,n_tracks);

			if(!phis) vec_r.push_back(0.);
			if(phis && !cct2) vec_r.push_back(1.);
			if(phis && cct2 && !vtx_ntrack) vec_r.push_back(2.);
			if(phis && cct2 && vtx_ntrack && !pileup_k0) vec_r.push_back(3.);
			if(phis && cct2 && vtx_ntrack && pileup_k0 && !pileup) vec_r.push_back(3.);
			if(phis && cct2 && vtx_ntrack && pileup_k0 && pileup && !GrCut) vec_r.push_back(5.);
			if(phis && cct2 && vtx_ntrack && pileup_k0 && pileup && GrCut && !VtxR) vec_r.push_back(6.);
			if(phis && cct2 && vtx_ntrack && pileup_k0 && pileup && GrCut && VtxR && !VtxZ) vec_r.push_back(7.);

			return vec_r;
		}, { "runId","triggerMapAR","vtxNtracks","k0cut","noPileup","vtxRcorr","vtxZcorr","stsNdigits","track_multiplicity"})
	;


	auto h_cut = dd_cut.Histo1D({"cuts", 	 ";;N events,%", use_cuts, -0.5, -0.5+(float)use_cuts}, "NumberCuts");
	for (int i = 0; i<use_cuts; i++){
      auto ix = h_cut->FindBin(i);
      h_cut->GetXaxis()->SetBinLabel(ix, bin_cut_txt.at(i).c_str());
      //h_cut_percent->GetXaxis()->SetBinLabel(ix, bin_cut_txt.at(i).c_str());
  	}
  	//h_cut->Scale(100./h_cut->GetBinContent(1));
  	h_cut->SetLineColor(kBlue);
  	h_cut->SetMarkerColor(kBlue);
  	h_cut->SetMarkerStyle(8);
  	h_cut->SetMarkerSize(2);
  	h_cut->Write();


	auto h_cutNo = dd_cut.Histo1D({"cutsNo", 	 ";;N events,%", use_cuts, -0.5, -0.5+(float)use_cuts}, "NumberCuts_Not");
	for (int i = 0; i<use_cuts; i++){
      auto ix = h_cutNo->FindBin(i);
      h_cutNo->GetXaxis()->SetBinLabel(ix, bin_cut_txt.at(i).c_str());
      //h_cutNo_percent->GetXaxis()->SetBinLabel(ix, bin_cut_txt.at(i).c_str());
  	}
  	//h_cutNo->Scale(100./h_cutNo->GetBinContent(1));
  	h_cutNo->SetLineColor(kRed);
  	h_cutNo->SetMarkerColor(kRed);
  	h_cutNo->SetMarkerStyle(8);
  	h_cutNo->SetMarkerSize(2);
  	h_cutNo->Write();

	file_out->Close();

	std::cout<<"QA_completed!"<<std::endl;
	
	timer1.Stop();
  	timer1.Print();
}
