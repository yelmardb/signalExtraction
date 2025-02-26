// const char* period = "LHC22oPass6";
// Int_t runNumberData = 220661; //pass6
// Int_t runNumberMC = 214158;  //pass6 
// Int_t runNumberTrueMC = 214158;
//dirty patch fix asap
//Int_t runNumberTrueMC = 223972; //pass6

// const char* period = "LHC22oPass4_lowIR";
// Int_t runNumberData = 287232; //pass4, low IR
// Int_t runNumberMC = root289060;  //pass4, low IR
// Int_t runNumberTrueMC = 289060; //pass4, low IR

//  const char* period = "LHC23Pass4_ThinSmall";
// Int_t runNumberData = 292354;
// Int_t runNumberMC = 292422;
// Int_t runNumberTrueMC = 292422;
// // Int_t runNumberData = 287710; //(no TPC only tracks in this train run)292354 
// // Int_t runNumberMC = 289046;  (no TPC only tracks in this train run)
// // Int_t runNumberTrueMC = 289046; (no TPC only tracks in this train run)


// //LHC22oPass7
  const char* period = "LHC22oPass7";
  Int_t runNumberData = 351015; //pass7 with cut variations
  Int_t runNumberMC = 348247;  //pass7 with cut variations
  Int_t runNumberTrueMC = 348247; //pass7 with cut variations
  
  
  ///337539, pass7 LHC24f3c without PIDTPC_MCTunedOnData, 10% of statistics
  // 348247, pass7 LHC24f3c, 10% of statistics
  // 348340, pass7 LHC24f3b2, 10% of statistics
  // 333566, pass7 LHC24f3b, 10% of statistics
  //300332, pass7 LHC24f3b, 100% of statistics


// Int_t runNumberData = 292919; //pass7
// Int_t runNumberMC = 292424;  //pass7
// Int_t runNumberTrueMC = 292424; //pass7
//Int_t runNumberData = 255807; //pass7 (no TPC only tracks in this train run)
//Int_t runNumberMC = 250643;  //pass7 (no TPC only tracks in this train run)
//Int_t runNumberTrueMC = 250643; //pass7 (no TPC only tracks in this train run)


///*******************************************************************************************************///
///BINNING

std::vector<Double_t> fBinsPi0PtMC = {0.40, 0.70, 0.90, 1.00, 1.1, 1.20, 1.40, 1.60, 1.80, 2.0, 2.50, 3.00, 3.50, 4.00, 6.0, 10.0};//official for pass6
//std::vector<Double_t> fBinsPi0PtMC = {0.40,  1.00, 1.5, 2.0, 2.50, 3.00, 4.00, 6.0, 10.0};//official for pass6


//std::vector<Double_t> fBinsPi0PtMC = { 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 
                               //        3.0, 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4.0, 4.2, 4.4, 4.6, 4.8, 5.0, 5.5, 6.0, 6.5, 7.0,10.0};  //alternative 2 for pass6


std::vector<Double_t> fBinsEtaPt= {0.4, 1.0, 1.6, 2.,2.5, 3.0, 3.5, 4.0, 6.0, 10.0}; //official for pass6
//std::vector<Double_t> fBinsEtaPt= {0.4, 1.0, 3.0, 6, 10.};  //alternative for  pass6


//std::vector<Double_t> fBinsEtaPt= {0.40, 0.70, 1., 1.3, 1.6,  2., 2.5, 3.0, 3.5, 4.0, 5.0, 6.0, 8.0, 10.};


 std::vector<const char*> cutModes = {""};//
//std::vector<const char*> cutModes = {"", "cutMode101", "cutMode102", "cutMode103", "cutMode104", "cutMode105", "cutMode106", "cutMode107", "cutMode108", "cutMode109", "cutMode110", "cutMode111", "cutMode112", "cutMode113", "cutMode114", "cutMode115", "cutMode116", "cutMode117"};
//, "cutMode118", "cutMode119", "cutMode120", "cutMode121"};
