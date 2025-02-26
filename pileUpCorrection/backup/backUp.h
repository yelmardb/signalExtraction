

// c++
#include <stdio.h>
#include <string>
#include <vector>
// root
#include "TFractionFitter.h"
#include "THnSparse.h"
#include "TObjArray.h"
// Predefinition
class TH1D;
class TH2D;
class TCanvas;
class TFile;



class PileUpCorrection {
  THnSparseD*         _hnData = nullptr;//DCAxy:pT
  THnSparseD*         _hnMC = nullptr;//DCAxy:pT:MCinfo; MCinfo --> v0collisionId != mcGammaCollisionId:v0collisionId = mcGammaCollisionId
  TH2D*               _h2CorrMapSecondary = nullptr;
  TH2D*               _h2CorrMapEffSecondary = nullptr;
  TH2D*               _h2CorrMapDecay = nullptr;
  TH2D*               _h2CorrMapMaterial = nullptr;
  TH1D*               _h1DataDCA = nullptr;
  TH1D*               _h1DCAValidatedCollisions = nullptr;
  TH1D*               _h1DCANotValidatedCollisions = nullptr;
  TH1D*               _hPulls = nullptr;
  TH1D*               _hPullsPeak = nullptr;
  TH1D*               _hPullsTails = nullptr;
  TFile*              _OutPutFile = nullptr;
  TFractionFitter*    _FractionFitter = nullptr;
  TObjArray*          _DebugArray = new TObjArray();
  std::string         _stMCFileName{};
  std::string         _stDataFileName{};
  std::string         _stCollSystem{"pp"};
  std::string         _stOutFileName{"DCA_fits.root"};
  std::vector<double> _pTBinning{};
  double              _dataIntegral = 0;
  double              _mcValColIntegral = 0;
  double              _mcNotValColIntegral = 0;
  double              _mcTotalIntegral = 0;
  double              _StdDevPulls = 0.0;
  double              _MeanPulls = 0.0;
  double              _AmplitudePulls = 0.0;
  float               _pTMin = 0.15;
  float               _pTMax = 50;
  float               _FitBoundary = 0.99;
  float               _StepSize = 1e-10;
  float               _ExcludeMin = -0.1;
  float               _ExcludeMax = 0.1;
  float               _PlotRangeMin = -0.6;
  float               _PlotRangeMax = 0.6;
  float               _ScalingSecondary = 1;
  float               _pVal = 0.05;
  int                 _FitStatus = -1;
  int                 _RebinFactor = 2;
  int                 _FitMethod = 0; // 0 = TFractionFitter; 1 = RooFit
    // axis
  int const           _DCAxyAxis = 0;
  int const           _pTAxis = 1;
  int const           _MCinfoAxis = 2;
  bool                _DoExclusion = false;
  bool                _DoRebinning = true;
  bool                _isFitGoodChiSq = false;
  bool                _isFitGoodPulls= false;
  bool                _useTwoComponentFit = false;



  public:
  // functions
  void MakeProjections();
  void RebinHistograms();
  void MakeTemplateFits();

  void CalculateSecondaryScaling();
  void ChiSqTest();
  void Pulls();
  void InitHistograms();

  void MakePlot();

  // Setter
  void SetPtMin(float pt){_pTMin=pt;}
  void SetPtMax(float pt){_pTMax=pt;}
  void SetPtRange(float ptMin, float ptMax) {_pTMin=ptMin; _pTMax=ptMax;}
  void SetFitMethod(int const method){_FitMethod=method;}
  void SetSaveFileName(std::string name) {_stOutFileName=name;}

  void SetMCFileName(std::string name){_stMCFileName=name;}
  void SetDataFileName(std::string name){_stDataFileName=name;}
  void SetCollisionSystem(std::string coll){_stCollSystem=coll;}
  void SetPtBinning(std::vector<double> bins) {_pTBinning=bins;}
  void SetFitBoundary(float bound){_FitBoundary=bound;}
  void SetStepSize(float size){_StepSize=size;}
  void SetExcludeRange(float min, float max){_ExcludeMin=min;_ExcludeMax=max;}
  void SetPlotRange(float min, float max){_PlotRangeMin=min;_PlotRangeMax=max;}
  void SetGoodnessFitChiSq(bool goodness){_isFitGoodChiSq = goodness;}
  void SetGoodnessFitPulls(bool goodness){_isFitGoodPulls = goodness;}
  void SetTwoComponentFit(bool use=true) {_useTwoComponentFit=use;}

  // Getter
 // auto const GetPtBinning() const {return _pTBinning;}
 // auto const GetMultBinning()   onst {return _multBinning;}
 // auto const Get2DCorrectionMapForSecDecay() const {return _h2CorrMapDecay;};
 // auto const Get2DCorrectionMapForSecMaterial() const {return _h2CorrMapMaterial;};
  //auto GetDebugArray() {return _DebugArray;}
};

