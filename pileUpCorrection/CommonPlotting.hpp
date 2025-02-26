//
//  CommonPlotting.hpp
//  EffOnVsOff
//
//  Created by Patrick Huhn on 10.06.20.
//  Copyright © 2020 Patrick Huhn. All rights reserved.
//

#ifndef CommonPlotting_hpp
#define CommonPlotting_hpp

#include "TH1D.h"
#include "TCanvas.h"
#include "TH1.h" //**new
#include "TH1D.h"
#include "THn.h" //**new
#include "TLatex.h"
#include "TObjArray.h"
#include "TPad.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TH2.h"
#include "TImage.h"
#include "TLegend.h"
#include "TMath.h"
#include "TPaveText.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TTimeStamp.h"
#include "TString.h"
//C++
#include <stdio.h>
#include <array>
#include <iostream>
#include <string>

namespace CommonPlotting {
  void AddStringToLegend(TLegend* _leg, std::string _str);
  void AddStringToLegendCol2(TLegend* _leg, std::string _str);

  TCanvas* makeCanvas(TObjArray* histArray, TObjArray* ratioArray = 0,
      const char* controlstring = "", Short_t* colorArray = 0,
      Short_t* markerArray = 0);
  void SetHistogramProperties(TH1D* h, TString XTitle, TString YTitle,
      Int_t ColorIndex);

  Int_t GetNiceColor(Int_t i);

  void PlotArray(TObjArray* arraytoplot, const char* controlstring,
      Short_t* colorArray, Short_t* markerArray,
      Double_t relativeTextSize, Float_t lableSize, Float_t titleSize,
      Int_t lableFont, Int_t titleFont, Float_t titleOffsetY,
      Float_t titleOffsetX, Float_t margin, Double_t xMin,
      Double_t xMax, Double_t yMin = 0, Double_t yMax = 0);

  void SetHistogramProperties(TH2D* h, TString XTitle, TString YTitle,
      Int_t ColorIndex);

  TString niceExp(double x);
  TString legendEntry(TString tune, double nEvents);

  static std::string const _pTString{"#it{p}_{T} (GeV/#it{c})"};
  static TF1*     _LineAt1 = 0;

  struct DefaultPlot {
    std::array<double const, 4> const LegPos{0.15, 0.05, 0.35, 0.5};
    TObjArray*  _arr=0;
    TObjArray*  _arrRatio=0;
    TLegend*    _leg=0;
    TCanvas*    _canvas=0;

    DefaultPlot(){
      _arr = new TObjArray();
      _leg = new TLegend(LegPos[0], LegPos[1], LegPos[2], LegPos[3]);
    }
    void AddToLeg(std::string _str){
      AddStringToLegend(_leg, _str);
    }
    void AddToLeg(TH1D* h, std::string _str){
      _leg->AddEntry(h, _str.c_str(), "lp");
    }
    void AddHistUpper(TH1D* h){
      _arr->Add(h);
    }
    void AddHistLower(TH1D* h){
      if(!_arrRatio)
        _arrRatio = new TObjArray();
      _arrRatio->Add(h);
    }
    void CreateCanvas(std::string option="Square"){
      _arr->Add(_leg);
      if(_arrRatio && _LineAt1)
        _arrRatio->Add(_LineAt1);
      _canvas = makeCanvas(_arr, _arrRatio, option.c_str(), 0, 0);
    }
    void SaveCanvas(std::string _SaveName){
      if(!_canvas)
        CreateCanvas();
      _canvas->SaveAs(Form("%s.pdf", _SaveName.c_str()));
      _canvas->SaveAs(Form("%s.root", _SaveName.c_str()));
    }
    ~DefaultPlot(){
      if(_arr) delete _arr;
      if(_arrRatio) delete _arrRatio;
      if(_leg) delete _leg;
      if(_canvas) delete _canvas;
    }
  };



}

#endif /* CommonPlotting_hpp */
