#include <iostream>
#include <string>
#include <array>
#include <vector>
#include "TCanvas.h"
#include "pileUpCorrection.C"
#include "include/pileUpCorrection.h"

namespace {
  using std::array;
  using std::cerr;
  using std::cout;
  using std::endl;
  using std::string;
  using std::vector;
} // namespace

int main() {

  const char* mcFileName = "/Users/yelmard/myAnalysis/Run3Pi0/mc/AnalysisResults2.root";
  const char* dataFileName = "/Users/yelmard/myAnalysis/Run3Pi0/mc/AnalysisResults2.root";


    
    std::vector<double> _pTArray{0.15, 0.5, 1, 3, 5, 10, 50};


    {
      PileUpCorrection _ScalObject{};
      _ScalObject.SetCollisionSystem("pp");
      _ScalObject.SetMCFileName(mcFileName);
      _ScalObject.SetDataFileName(dataFileName);
      _ScalObject.SetSaveFileName("DCA_fits_method0_2Template.root");
      _ScalObject.SetPtBinning(_pTArray);
      _ScalObject.SetTwoComponentFit();

      _ScalObject.InitHistograms();
      _ScalObject.CalculateSecondaryScaling();
  }



  /*  {
        SecondaryScaling _ScalObject{};
        _ScalObject.SetCollisionSystem("pp");
        _ScalObject.SetMCFileName(mcFileName);
        _ScalObject.SetDataFileName(dataFileName);
        _ScalObject.SetSaveFileName("DCA_fits_method0_3Template.root");
        _ScalObject.SetPtBinning(_pTArray);
        _ScalObject.SetMultBinning(_MultArray);

        _ScalObject.InitHistograms();
        _ScalObject.CalculateSecondaryScaling();
    }

  {
        SecondaryScaling _ScalObject{};
        _ScalObject.SetCollisionSystem("pp");
        _ScalObject.SetMCFileName(mcFileName);
        _ScalObject.SetDataFileName(dataFileName);
        _ScalObject.SetSaveFileName("DCA_fits_method0_2Template.root");
        _ScalObject.SetPtBinning(_pTArray);
        _ScalObject.SetMultBinning(_MultArray);
        _ScalObject.SetTwoComponentFit();

        _ScalObject.InitHistograms();
        _ScalObject.CalculateSecondaryScaling();
    }*/


  /*
  {
        SecondaryScaling _ScalObject{};
        _ScalObject.SetMCFileName(mcFileName);
        _ScalObject.SetDataFileName(dataFileName);
        _ScalObject.SetSaveFileName("DCA_fits_method1_3Template.root");
        _ScalObject.SetPtBinning(_pTArray);
        _ScalObject.SetMultBinning(_MultArray);
        _ScalObject.SetFitMethod(1); // RooFit
        //_ScalObject.SetTwoComponentFit();

        _ScalObject.InitHistograms();
        _ScalObject.CalculateSecondaryScaling();
    }

    {
        SecondaryScaling _ScalObject{};
        _ScalObject.SetMCFileName(mcFileName);
        _ScalObject.SetDataFileName(dataFileName);
        _ScalObject.SetSaveFileName("DCA_fits_method1_2Template.root");
        _ScalObject.SetPtBinning(_pTArray);
        _ScalObject.SetMultBinning(_MultArray);
        _ScalObject.SetFitMethod(1); // RooFit
        _ScalObject.SetTwoComponentFit();

        _ScalObject.InitHistograms();
        _ScalObject.CalculateSecondaryScaling();
    }*/


  return 0;
}
