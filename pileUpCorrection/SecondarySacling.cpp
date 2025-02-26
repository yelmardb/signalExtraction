//
//  SecondarySacling.cpp
//  SecondaryScaling
//
//  Created by Patrick Huhn on 10.03.20.
//  Copyright © 2020 Patrick Huhn. All rights reserved.
//
#include "SecondarySacling.hpp"
#include "CommonPlotting.cpp"

#include "Fit/Fitter.h"
#include "RooAddPdf.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooAddPdf.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "TDirectoryFile.h"
#include "TF1.h"
#include "TFile.h"
#include "TFractionFitter.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVirtualFitter.h"
#include "TMath.h"

//TODO: eff. scaling pT and Nch dependent

namespace {
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using namespace CommonPlotting;
using namespace RooFit;
} // namespace

void SecondaryScaling::CalculateSecondaryScaling() {
    if (!_h2CorrMapDecay || !_h2CorrMapMaterial)
        SecondaryScaling::InitHistograms();
    if (!_h2CorrMapDecay || !_h2CorrMapMaterial) {
        cerr << "ERROR: 2D histos are not created.\n";
        return;
    }

    for (int ptBin = 0; ptBin < _pTBinning.size() - 1; ++ptBin) {
        auto const pTLow = _pTBinning[ptBin];
        auto const pTHigh = _pTBinning[ptBin + 1] - 0.01;
        SetPtRange(pTLow, pTHigh);
        cout << "";
        for (int multBin = 0; multBin < _multBinning.size() - 1; ++multBin) {
            auto const multLow = _multBinning[multBin];
            auto const multHigh = _multBinning[multBin + 1] - 0.01;
            SetMultRange(multLow, multHigh);
            if(_stCollSystem.compare("PbPb") == 0) SetCentRange(multLow, multHigh);

            _ScalingDecay = 1;
            _ScalingMaterial = 1;
            _ScalingSecondary = 1;

            MakeProjections();
            RebinHistograms();
            MakeTemplateFits();

            _hPulls->GetYaxis()->SetRangeUser(-_AmplitudePulls, _AmplitudePulls);
            _hPulls->GetXaxis()->SetRangeUser(-0.6, 0.6);
            _hPulls->SetName(Form("hPulls_pT_%lf_%lf-c%lf-%lf", _pTMin, _pTMax, _CentMin, _CentMax));

            _OutPutFile->cd();
            _hPulls->Write();
            _h1DataDCA->SetName(Form("h1DataDCA_pT_%lf_%lf-c%lf-%lf", _pTMin, _pTMax, _CentMin, _CentMax));
            _h1PriDCA->SetName(Form("h1PriDCA_pT_%lf_%lf-c%lf-%lf", _pTMin, _pTMax, _CentMin, _CentMax));
            _h1SecDCA->SetName(Form("h1SecDCA_pT_%lf_%lf-c%lf-%lf", _pTMin, _pTMax, _CentMin, _CentMax));
            _h1SecDecayDCA->SetName(Form("h1SecDecayDCA_pT_%lf_%lf-c%lf-%lf", _pTMin, _pTMax, _CentMin, _CentMax));
            _h1SecMaterialDCA->SetName(Form("h1SecMaterialDCA_pT_%lf_%lf-c%lf-%lf", _pTMin, _pTMax, _CentMin, _CentMax));


            auto const _Localbin = _h2CorrMapDecay->FindBin(
                                                            (pTLow + pTHigh) / 2., (multLow + multHigh) / 2.);
            if(_useTwoComponentFit){
                _h2CorrMapSecondary->SetBinContent(_Localbin, _ScalingSecondary);
            }
            else {
                _h2CorrMapDecay->SetBinContent(_Localbin, _ScalingDecay);
                _h2CorrMapMaterial->SetBinContent(_Localbin, _ScalingMaterial);
                auto const _EffScaling =
                    _ScalingDecay * (_mcSecDecayIntegral/_mcSecIntegral) +
                    _ScalingMaterial * (_mcSecMaterialIntegral/_mcSecIntegral);
                _h2CorrMapEffSecondary->SetBinContent(_Localbin, _EffScaling);
            }
        }
    }

    if(_OutPutFile){
        _OutPutFile->cd();
        _h2CorrMapDecay->Write();
        _h2CorrMapMaterial->Write();
        _h2CorrMapSecondary->Write();
        _h2CorrMapEffSecondary->Write();
    }
}

void SecondaryScaling::ChiSqTest() {
    double const chiSq = _FractionFitter->GetChisquare();
    double const NDF = _FractionFitter->GetNDF();
    double const criChiSq = TMath::ChisquareQuantile(1. - _pVal, NDF);

    if (chiSq <= criChiSq)
        SetGoodnessFitChiSq(kTRUE);
    else
        SetGoodnessFitChiSq(kFALSE);
}

void SecondaryScaling::Pulls() {
  TH1D* _fitresult = (TH1D*)_FractionFitter->GetPlot();
  _fitresult->SetName("_fitresult");

  //_hPulls = new TH1D("hpulls", "", 35, -70., 70.);
  _hPulls = (TH1D*)_h1DataDCA->Clone("_hPulls");
  _hPulls->Reset();
  _hPulls->GetYaxis()->SetTitle("data - fit");
  _hPulls->GetXaxis()->SetTitle("DCA_{xy}");
  _hPullsPeak = new TH1D("_hPullsPeak", "", 35, -70., 70.);
  _hPullsPeak->Sumw2();
  _hPullsTails = new TH1D("_hPullsTails", "", 35, -70., 70.);
  _hPullsTails->Sumw2();

  double  pullPoint = 0.0;
  double dataPoint = 0.0;
  double fitPoint = 0.0;

  double  pullError = 0.0;
  double dataError = 0.0;
  double fitError = 0.0;

  for(int i = 0; i < _h1DataDCA->GetNbinsX(); i++) {
    pullPoint = 0.0;
    dataPoint = _h1DataDCA->GetBinContent(i);
    fitPoint = _fitresult->GetBinContent(i);
    dataError = _h1DataDCA->GetBinError(i);
    fitError = _fitresult->GetBinError(i);

    if(dataPoint>0.0 && fitPoint>0.0) {
        pullPoint = (dataPoint - fitPoint);
        _hPulls->SetBinContent(i, pullPoint);
        _hPulls->SetBinError(i, sqrt(dataError*dataError+fitError*fitError));
        if(TMath::Abs(_h1DataDCA->GetBinCenter(i)) < 0.2) _hPullsPeak->Fill(pullPoint);
        else _hPullsTails->Fill(pullPoint);
    }
  }
  _AmplitudePulls = _hPulls->GetMaximum();

    /*_hPulls->Scale(1.0/_hPulls->Integral());
  TF1  *gausfit = new TF1("f1","gaus", -100, 100);
  _AmplitudePulls = _hPulls->GetMaximum();
  gausfit->SetParameter(0, _AmplitudePulls);
  gausfit->SetParLimits(0, 0.6*_AmplitudePulls, 1.5*_AmplitudePulls);
_hPulls->Fit(gausfit, "");
  _hPulls->Fit(gausfit, "");
  _hPulls->Fit(gausfit, "");

  _MeanPulls = gausfit->GetParameter(1);
  double meanError = gausfit->GetParError(1); //
  _StdDevPulls= gausfit->GetParameter(2);
  double stdDevError = gausfit->GetParError(2);


  cout << endl << "Mean: " << _MeanPulls << " +- "<< meanError;
  cout << endl << "STDV: " << _StdDevPulls << " +- "<< stdDevError  << endl;


  if( (1.0 < _StdDevPulls + stdDevError) && (1.0 > _StdDevPulls - stdDevError) && (0.0 < _MeanPulls + meanError) && (0.0 > _MeanPulls - meanError)) SetGoodnessFitPulls(kTRUE); // Way too agressive cut, since errors are to small
  else SetGoodnessFitPulls(kFALSE);*/

}

void SecondaryScaling::InitHistograms() {
    //*********************************************
    //*********** Load from File ******************
    //*********************************************
    cout << "First, load histograms from files\n";
    if (_stMCFileName.size() < 5 || _stDataFileName.size() < 5) {
        cerr << "ERROR: Please set paths to MC and Data files" << endl;
        return;
    }

    cout << "Loading THnSparseD from files\n";

    //----------------------------------------------
    // opening files
    TFile _fileData(_stDataFileName.c_str(), "OPEN");
    if (_fileData.IsZombie()) {
        cerr << "ERROR: Could not read file " << _stDataFileName << "\n";
        return;
    }

    TFile _fileMC(_stMCFileName.c_str(), "OPEN");
    if (_fileMC.IsZombie()) {
        cerr << "ERROR: Could not read file " << _stMCFileName << "\n";
        return;
    }
    //----------------------------------------------

    //----------------------------------------------
    // loading histograms
    // DCAxy:pT:nTracks:MCinfo
    // MCinfo --> pri:sec:decay:material

    // data
    cout << "\nloading histograms from data file\n";
    auto _dirFileData = (TDirectoryFile*)_fileData.Get("TaskDCArStudy");
    if (!_dirFileData) {
        cerr << "ERROR: could not find directory file\n";
        return;
    }
    _dirFileData->SetName("_dirFileData");

    auto _listData = (TList*)_dirFileData->Get("TaskDCArStudy");
    if (!_listData) {
        cerr << "ERROR: could not find list\n";
        return;
    }
    _listData->SetName("_listData");

    _hnData = (THnSparseD*)_listData->FindObject("fHistDCA");
    if (!_hnData) {
        cerr << "ERROR: could not find histogram\n";
        return;
    }
    _hnData->SetName("_hnData");

    // MC
    cout << "loading histograms from mc file\n";
    auto _dirFileMC = (TDirectoryFile*)_fileMC.Get("TaskDCArStudy");
    if (!_dirFileMC) {
        cerr << "ERROR: could not find directory file\n";
        return;
    }
    _dirFileMC->SetName("_dirFileMC");

    auto _listMC = (TList*)_dirFileMC->Get("TaskDCArStudy");
    if (!_listMC) {
        cerr << "ERROR: could not find list\n";
        return;
    }
    _listMC->SetName("_listMC");

    _hnMC = (THnSparseD*)_listMC->FindObject("fHistDCA");
    if (!_hnMC) {
        cerr << "ERROR: could not find histogram\n";
        return;
    }
    _hnMC->SetName("_hnMC");
    //    _hnMC->SetDirectory(0);
    //----------------------------------------------
    _fileMC.Close();
    _fileData.Close();

    //*********************************************
    //********** create 2D Histos *****************
    //*********************************************
    _h2CorrMapDecay = new TH2D(
                               "_h2CorrMapDecay", ";#it{p}_{T} (GeV/#it{c});#it{N}_{ch}",
                               static_cast<int>(_pTBinning.size()) - 1, _pTBinning.data(),
                               static_cast<int>(_multBinning.size()) - 1, _multBinning.data());
    _h2CorrMapMaterial = new TH2D(
                                  "_h2CorrMapMaterial", ";#it{p}_{T} (GeV/#it{c});#it{N}_{ch}",
                                  static_cast<int>(_pTBinning.size()) - 1, _pTBinning.data(),
                                  static_cast<int>(_multBinning.size()) - 1, _multBinning.data());

    _h2CorrMapSecondary = new TH2D(
                                  "_h2CorrMapSecondary", ";#it{p}_{T} (GeV/#it{c});#it{N}_{ch}",
                                  static_cast<int>(_pTBinning.size()) - 1, _pTBinning.data(),
                                  static_cast<int>(_multBinning.size()) - 1, _multBinning.data());

    _h2CorrMapEffSecondary = new TH2D(
                                  "_h2CorrMapEffSecondary", ";#it{p}_{T} (GeV/#it{c});#it{N}_{ch}",
                                  static_cast<int>(_pTBinning.size()) - 1, _pTBinning.data(),
                                  static_cast<int>(_multBinning.size()) - 1, _multBinning.data());

    //*********************************************
    //********* create output file ****************
    //*********************************************

    _OutPutFile = TFile::Open(_stOutFileName.c_str(), "RECREATE");
}

void SecondaryScaling::MakeProjections() {
    // delete if histogram is already there
    //    if(_h1PriDCA) delete _h1PriDCA;
    //    if(_h1SecDCA) delete _h1SecDCA;
    //    if(_h1SecDecayDCA) delete _h1SecDecayDCA;
    //    if(_h1SecMaterialDCA) delete _h1SecMaterialDCA;
    //    if(_h1DataDCA) delete _h1DataDCA;
    //
    cout << "Make projections according to pT and mult selection\n";
    //----------------------------------------------
    // set up some binnings and make projections
    // DCAxy:pT:nTracks:MCinfo
    // MCinfo -->  0=prim, 1=decay 2=material -1=data
    //
    // ------ MC -----

    //Set an integer for each axis an integer

    //TODO set this in header
//    if(_stCollSystem.compare("PbPb") == 0) MCinfoAxis = 4;

    cout << "\nmake projections from mc hist\n";
    auto const _binBoundarayMin = _hnMC->GetAxis(_DCAxyAxis)->FindBin(-_FitBoundary);
    auto const _binBoundarayMax = _hnMC->GetAxis(_DCAxyAxis)->FindBin(_FitBoundary);
    auto const _binpTMin = _hnMC->GetAxis(_pTAxis)->FindBin(_pTMin);
    auto const _binpTMax = _hnMC->GetAxis(_pTAxis)->FindBin(_pTMax);
    auto const _binmultMin = _hnMC->GetAxis(_multAxis)->FindBin(_MultMin);
    auto const _binmultMax = _hnMC->GetAxis(_multAxis)->FindBin(_MultMax);
    auto const _bincentMin = _hnMC->GetAxis(_centAxis)->FindBin(_CentMin);
    auto const _bincentMax = _hnMC->GetAxis(_centAxis)->FindBin(_CentMax);

    _hnMC->GetAxis(_DCAxyAxis)->SetRange(_binBoundarayMin,
                                _binBoundarayMax); // reject overflow
    _hnMC->GetAxis(_pTAxis)->SetRange(_binpTMin, _binpTMax);
    _hnMC->GetAxis(_multAxis)->SetRange(_binmultMin, _binmultMax);
    if(_stCollSystem.compare("PbPb") == 0) _hnMC->GetAxis(_centAxis)->SetRange(_bincentMin, _bincentMax);
    _hnMC->GetAxis(_MCinfoAxis)->SetRangeUser(0, 0);
    _h1PriDCA = (TH1D*)_hnMC->Projection(_DCAxyAxis);
    _h1PriDCA->SetName(Form("_h1PriDCA_%lf_%lf-c%lf-%lf", _pTMin, _pTMax, _CentMin, _CentMax));
    _h1PriDCA->Sumw2();
    _hnMC->GetAxis(_MCinfoAxis)->SetRangeUser(1, 2);
    _h1SecDCA = (TH1D*)_hnMC->Projection(_DCAxyAxis);
    _h1SecDCA->SetName(Form("_h1SecDCA_%lf_%lf-c%lf-%lf", _pTMin, _pTMax, _CentMin, _CentMax));
    _h1SecDCA->Sumw2();
    _hnMC->GetAxis(_MCinfoAxis)->SetRangeUser(1, 1);
    _h1SecDecayDCA = (TH1D*)_hnMC->Projection(_DCAxyAxis);
    _h1SecDecayDCA->SetName(Form("_h1SecDecayDCA_%lf_%lf-c%lf-%lf", _pTMin, _pTMax, _CentMin, _CentMax));
    _h1SecDecayDCA->Sumw2();
    _hnMC->GetAxis(_MCinfoAxis)->SetRangeUser(2, 2);
    _h1SecMaterialDCA = (TH1D*)_hnMC->Projection(_DCAxyAxis);
    _h1SecMaterialDCA->SetName(
                               Form("_h1SecMaterialDCA_%lf_%lf-c%lf-%lf", _pTMin, _pTMax, _CentMin, _CentMax));
    _h1SecMaterialDCA->Sumw2();

    // ------ data -----
    cout << "make projections from data hist\n";
    auto const _binBoundarayMinData =
    _hnData->GetAxis(_DCAxyAxis)->FindBin(-_FitBoundary);
    auto const _binBoundarayMaxData =
    _hnData->GetAxis(_DCAxyAxis)->FindBin(_FitBoundary);
    auto const _binpTMinData = _hnData->GetAxis(_pTAxis)->FindBin(_pTMin);
    auto const _binpTMaxData = _hnData->GetAxis(_pTAxis)->FindBin(_pTMax);
    auto const _binmultMinData = _hnData->GetAxis(_multAxis)->FindBin(_MultMin);
    auto const _binmultMaxData = _hnData->GetAxis(_multAxis)->FindBin(_MultMax);
    auto const _bincentMinData = _hnData->GetAxis(_centAxis)->FindBin(_CentMin);
    auto const _bincentMaxData = _hnData->GetAxis(_centAxis)->FindBin(_CentMax);

    _hnData->GetAxis(_DCAxyAxis)->SetRange(_binBoundarayMinData,
                                  _binBoundarayMaxData); // reject overflow
    _hnData->GetAxis(_pTAxis)->SetRange(_binpTMinData, _binpTMaxData);
    _hnData->GetAxis(_multAxis)->SetRange(_binmultMinData, _binmultMaxData);
    if(_stCollSystem.compare("PbPb") == 0) _hnData->GetAxis(_centAxis)->SetRange(_bincentMinData, _bincentMaxData);
    _h1DataDCA = (TH1D*)_hnData->Projection(_DCAxyAxis);
    _h1DataDCA->SetName(Form("_h1DataDCA_%lf_%lf-c%lf-%lf", _pTMin, _pTMax, _CentMin, _CentMax));
    _h1DataDCA->Sumw2();

    // normalize
    _dataIntegral =
    _h1DataDCA->Integral(_binBoundarayMinData, _binBoundarayMaxData);
    _mcPriIntegral = _h1PriDCA->Integral(_binBoundarayMin, _binBoundarayMax);
    _mcSecIntegral = _h1SecDCA->Integral(_binBoundarayMin, _binBoundarayMax);
    _mcSecDecayIntegral =
    _h1SecDecayDCA->Integral(_binBoundarayMin, _binBoundarayMax);
    _mcSecMaterialIntegral =
    _h1SecMaterialDCA->Integral(_binBoundarayMin, _binBoundarayMax);
    _mcTotalIntegral = _mcPriIntegral + _mcSecIntegral;

    if (_mcTotalIntegral < 1) {
        cerr << "Fatal error: total integral in mc is too small\n";
        return;
    }
    if (_dataIntegral < 1) {
        cerr << "Fatal error: total integral in data is too small\n";
        return;
    }

    _h1DataDCA->Scale(1. / _dataIntegral, "width");
    _h1PriDCA->Scale(1. / _mcTotalIntegral, "width");
    _h1SecDCA->Scale(1. / _mcTotalIntegral, "width");
    _h1SecDecayDCA->Scale(1. / _mcTotalIntegral, "width");
    _h1SecMaterialDCA->Scale(1. / _mcTotalIntegral, "width");

    cout << "\n\nDefault fractions: \n";
    cout << "\t pri/total: " << _mcPriIntegral / _mcTotalIntegral << "\n";
    cout << "\t sec/total: " << _mcSecIntegral / _mcTotalIntegral << "\n";
    cout << "\t secDecay/total: " << _mcSecDecayIntegral / _mcTotalIntegral
    << "\n";
    cout << "\t secMaterial/total: "
    << _mcSecMaterialIntegral / _mcTotalIntegral << "\n\n";
    //----------------------------------------------
}

void SecondaryScaling::RebinHistograms() {
    if (!_DoRebinning) {
        cout << "Skipping rebinnnig\n";
        return;
    }
    _h1DataDCA->Rebin(_RebinFactor);
    _h1PriDCA->Rebin(_RebinFactor);
    _h1SecDCA->Rebin(_RebinFactor);
    _h1SecDecayDCA->Rebin(_RebinFactor);
    _h1SecMaterialDCA->Rebin(_RebinFactor);
}

void SecondaryScaling::MakeTemplateFits() {
    cout << "\nNow performing template fits\n";

    if (0 == _FitMethod) { // use TFractionFitter
        cout << "Using TFractionFitter method\n";
//        ROOT::Math::MinimizerOptions::SetDefaultMinimizer(“Minuit2”);
        auto _objArr = new TObjArray(3);
        _objArr->Add(_h1PriDCA);
        if(_useTwoComponentFit)
        {
            _objArr->Add(_h1SecDCA);
        }
        else {
            _objArr->Add(_h1SecDecayDCA);
            _objArr->Add(_h1SecMaterialDCA);
        }

        _FractionFitter = new TFractionFitter(_h1DataDCA, _objArr, "Q");
        if (_DoExclusion) {
            for (int _bin = _h1DataDCA->FindBin(_ExcludeMin);
                 _bin <= _h1DataDCA->FindBin(_ExcludeMax); ++_bin) {
                _FractionFitter->ExcludeBin(_bin);
            }
        }
        // constrain fraction values
        _FractionFitter->Constrain(0, 0.5, 1.0); // pri
        _FractionFitter->Constrain(1, 0.0, 0.5); // secDecay or sec
        if(!_useTwoComponentFit){
            _FractionFitter->Constrain(2, 0.0,
                                   0.5); // secMaterial
        }
                                         // Setting additional parameter values
        auto _Fitter = _FractionFitter->GetFitter();

        _Fitter->Config().ParSettings(0).Set("primaries",
                                             _mcPriIntegral / _mcTotalIntegral,
                                             _StepSize, 0.8, 1.0);
        if(_useTwoComponentFit){
            _Fitter->Config().ParSettings(1).Set(
                                                 "secondaries", _mcSecIntegral / _mcTotalIntegral, _StepSize, 0.0,
                                                 0.5);
        }
        else {
            _Fitter->Config().ParSettings(1).Set(
                                                 "decay", _mcSecDecayIntegral / _mcTotalIntegral, _StepSize, 0.0,
                                                 0.4);
            _Fitter->Config().ParSettings(2).Set(
                                                 "material", _mcSecMaterialIntegral / _mcTotalIntegral, _StepSize,
                                                 0.0, 0.2);
        }
        // perform fit
        _FractionFitter->ReleaseRangeX();
        _FractionFitter->ReleaseRangeY();
        _FitStatus = _FractionFitter->Fit();
        _FitStatus = _FractionFitter->Fit();
        _FitStatus = _FractionFitter->Fit();
        // Add to Debug array;
        _DebugArray->Add(_h1PriDCA);
        if(_useTwoComponentFit){
            _DebugArray->Add(_h1SecDCA);
        }
        else {
            _DebugArray->Add(_h1SecDecayDCA);
            _DebugArray->Add(_h1SecMaterialDCA);
        }
        _DebugArray->Add(_h1DataDCA);
        _DebugArray->Add(_FractionFitter);

        if (0 == _FitStatus) { // fit successful
            cout << "chi-square/NDF: "
            << _FractionFitter->GetChisquare() / _FractionFitter->GetNDF()
            << "\n";
            cout << "chi-square: " << _FractionFitter->GetChisquare() << "\n";
            cout << "NDF: " << _FractionFitter->GetNDF() << "\n";
            cout << "Fit Prob: " << _FractionFitter->GetProb() << "\n";

            ChiSqTest();
            Pulls();

            if (_isFitGoodChiSq)
                cout << "Fit is good according the Chi-squared test"
                << "\n";
            else
                cout << "Fit is NOT good according the Chi-squared test"
                << "\n";
            if (_isFitGoodPulls)
                cout << "Fit is good according the pulls"
                << "\n";
            else
                cout << "Fit is NOT good according the pulls"
                << "\n";

            double factorPrim = 1, factorPrimError = 1, factorDecay = 1,
            factorDecayError = 1, factorMaterial = 1,
            factorMaterialError = 1, factorSecondary = 1, factorSecondayError = 1;
            _FractionFitter->GetResult(0, factorPrim, factorPrimError);
            if(_useTwoComponentFit){
                _FractionFitter->GetResult(1, factorSecondary, factorSecondayError);
            }
            else {
                _FractionFitter->GetResult(1, factorDecay, factorDecayError);
                _FractionFitter->GetResult(2, factorMaterial, factorMaterialError);
            }

            cout << "\nnew fractions: \n";
            cout << "\t pri/total: " << factorPrim << " +- " << factorPrimError
            << "\n";
            if(_useTwoComponentFit){
                cout << "\t secondary/total: " << factorSecondary << " +- "
                << factorSecondayError << "\n";
            } else {
                cout << "\t secDecay/total: " << factorDecay << " +- "
                << factorDecayError << "\n";
                cout << "\t secMaterial/total: " << factorMaterial << " +- "
                << factorMaterialError << "\n\n";
            }

            cout << "\nResulting scaling factors: \n";
            cout << "\t pri: "
            << factorPrim / (_mcPriIntegral / _mcTotalIntegral) << "\n";

            if(_useTwoComponentFit){
                _ScalingSecondary = factorSecondary / (_mcSecIntegral/_mcTotalIntegral);
                cout << "\t secondary: " << _ScalingSecondary << "\n";
            }
            else {
                _ScalingDecay =
                factorDecay / (_mcSecDecayIntegral / _mcTotalIntegral);
                cout << "\t decay: " << _ScalingDecay << "\n";
                _ScalingMaterial =
                factorMaterial / (_mcSecMaterialIntegral / _mcTotalIntegral);
                cout << "\t material: " << _ScalingMaterial << "\n\n";
            }

            MakePlot();
        } else { // fit failed
            cerr << "\n\nERROR: Fit didn't converged!!\n\n";
            return;
        }
    }
    else if (1 == _FitMethod) { // use RooFit PDF method
        cout << "Using RooFit method\n";

        // Converting histograms
        RooRealVar xAxis("xAxis", "DCA_{xy}", -1, 1);

        // MC
        RooDataHist dPriDCA("dPriDCA", "dPriDCA", xAxis, Import(*_h1PriDCA));
        RooDataHist dSecDCA("dSecDCA", "dSecDCA", xAxis,
                                 Import(*_h1SecDCA));
        RooDataHist dSecDecayDCA("dSecDecayDCA", "dSecDecayDCA", xAxis,
                                 Import(*_h1SecDecayDCA));
        RooDataHist dSecMaterialDCA("dSecMaterialDCA", "dSecMaterialDCA", xAxis,
                                    Import(*_h1SecMaterialDCA));
        // Data
        RooDataHist dDataDCA("dDataDCA", "dDataDCA", xAxis,
                             Import(*_h1DataDCA));

        // converting to PDFs
        RooHistPdf pPriDCA("pPriDCA", "pPriDCA", RooArgSet(xAxis), dPriDCA);
        RooHistPdf pSecDCA("pSecDCA", "pSecDCA", RooArgSet(xAxis),
                           dSecDCA);
        RooHistPdf pSecDec("pSecDec", "pSecDec", RooArgSet(xAxis),
                           dSecDecayDCA);
        RooHistPdf pSecMat("pSecMat", "pSecMat", RooArgSet(xAxis),
                           dSecMaterialDCA);

        // creating model with fractions
        auto const defaultFractionPri = _mcPriIntegral / _mcTotalIntegral;
        auto const defaultFractionSec = _mcSecIntegral / _mcTotalIntegral;
        auto const defaultFractionSecDec = _mcSecIntegral / _mcTotalIntegral;
        auto const defaultFractionSecMat = _mcSecDecayIntegral / _mcTotalIntegral;

        RooRealVar priFrac("priFrac", "priFrac", defaultFractionPri, defaultFractionPri/3., TMath::Min(1., defaultFractionPri*3));
        RooRealVar secFrac("secFrac", "secFrac", defaultFractionSec, defaultFractionSec/3., TMath::Min(0.5, defaultFractionSec*4));
        RooRealVar secDecayFrac("secDecayFrac", "secDecayFrac", defaultFractionSecDec, defaultFractionSecDec/3., TMath::Min(0.5, defaultFractionSecDec*4));
        RooRealVar secMatFrac("secMatFrac", "secMatFrac",defaultFractionSecMat, defaultFractionSecMat/3., TMath::Min(0.5, defaultFractionSecMat*4));


        // NB: Each coefficient is interpreted as the fraction of the
        // left-hand component of the i-th recursive sum, i.e.
        //
        //   sum4 = A + ( B + ( C + D)  with fraction fA, fB and fC expands to
        //
        //   sum4 = fA*A + (1-fA)*(fB*B + (1-fB)*(fC*C + (1-fC)*D))
        //
        //   model = pri + secDec + secMat --> fracPri + fracSecDec
        //   model = fracPri*pri + (1-fracPri) * ( secDec*fracSecDec + (1-fracSecDec)*secMat )
        // --> f'SecDec = (1-fracPir)*fracSecDec
        // --> f'SecMat = (1-fracPir)*(1-fracSecDec)
        RooAddPdf model("model", "pri+secDec+secMat",
                        RooArgList(pPriDCA, pSecDec, pSecMat),
                        RooArgList(priFrac, secDecayFrac));

        RooAddPdf model2("model2", "pri+sec",
                        RooArgList(pPriDCA, pSecDCA),
                        RooArgList(secFrac));
        // fit model to data
        if(!_useTwoComponentFit){
            model.fitTo(dDataDCA);
            model.Print("t");

            std::string saveName{
                "DCA_pT_" + std::to_string(_pTMin) + "-" + std::to_string(_pTMax)};
                // + "_mult" + std::to_string(_MultMin) + "-" + std::to_string(_MultMax)};

            RooPlot *xframe = xAxis.frame(Title(saveName.c_str()));
            dDataDCA.plotOn(xframe, LineColor(GetNiceColor(1)), MarkerColor(GetNiceColor(1)));
            model.plotOn(xframe, LineColor(GetNiceColor(2)));
            // Overlay the background component of model with a dashed line
            model.plotOn(xframe, Components(pPriDCA),LineColor(GetNiceColor(3)), LineStyle(kSolid));
            model.plotOn(xframe, Components(pSecDec),LineColor(GetNiceColor(4)), LineStyle(kSolid));
            model.plotOn(xframe, Components(pSecMat),LineColor(GetNiceColor(5)), LineStyle(kSolid));

            _OutPutFile->cd();
            xframe->Write();
        } else {
            model2.fitTo(dDataDCA);
            model2.Print("t");

            std::string saveName =
            "DCA_pT_2Template_" + std::to_string(_pTMin) + "-" + std::to_string(_pTMax) +
            "_mult" + std::to_string(_MultMin) + "-" + std::to_string(_MultMax);

            RooPlot *xframe2 = xAxis.frame(Title(saveName.c_str()));
            dDataDCA.plotOn(xframe2, LineColor(GetNiceColor(1)), MarkerColor(GetNiceColor(1)));
            model2.plotOn(xframe2, LineColor(GetNiceColor(2)));
            // Overlay the background component of model with a dashed line
            model2.plotOn(xframe2, Components(pPriDCA),LineColor(GetNiceColor(3)), LineStyle(kSolid));
            model2.plotOn(xframe2, Components(pSecDCA),LineColor(GetNiceColor(4)), LineStyle(kSolid));
            _OutPutFile->cd();
            xframe2->Write();
        }

        auto const fPri = priFrac.getValV();
        auto const fPriError = priFrac.getError();
        auto const fSec = 1 - secFrac.getValV();
        auto const fSecError = secFrac.getError()/secFrac.getValV() * fSec;
        auto const fSecDec = (1.-fPri)*secDecayFrac.getValV();
        auto const fSecMat = (1.-fPri)*(1.-secDecayFrac.getValV());


        // obtain fractions
        cout << "\nnew fractions: \n";
        cout << "\t pri/total: " << fPri << "\n";
        cout << "\t secDecay/total: " << fSecDec << "\n";
        cout << "\t secMaterial/total: " << fSecMat << "\n";
        cout << "\t Two template:\n";
        cout << "\t secondaries/total: " << fSec << "\n\n";

        cout << "\nResulting scaling factors: \n";
        cout << "\t pri: "
        << fPri / (_mcPriIntegral / _mcTotalIntegral) << "\n";
        _ScalingDecay =
        fSecDec / (_mcSecDecayIntegral / _mcTotalIntegral);
        cout << "\t decay: " << _ScalingDecay << "\n";
        _ScalingMaterial =
        fSecMat / (_mcSecMaterialIntegral / _mcTotalIntegral);
        cout << "\t material: " << _ScalingMaterial << "\n";
        cout << "\t Two template:\n";
        _ScalingSecondary = fSec / (_mcSecIntegral / _mcTotalIntegral);
        cout << "\t secondary: " << _ScalingSecondary << "\n";
        // create plot

    } else {
        cout << "WARNING: not supported fit method\n";
    }
}

void SecondaryScaling::MakePlot() {
    if (0 != _FitStatus)
        return;

    _OutPutFile->cd();

    DefaultPlot plot;
    double factorPrim = 0, factorPrimError = 0, factorDecay = 0,
    factorDecayError = 0, factorMaterial = 0, factorMaterialError = 0, factorSecondary = 0, factorSecondaryError = 0;
    _FractionFitter->GetResult(0, factorPrim, factorPrimError);
    if(_useTwoComponentFit){
        _FractionFitter->GetResult(1, factorSecondary, factorSecondaryError);
    } else {
        _FractionFitter->GetResult(1, factorDecay, factorDecayError);
        _FractionFitter->GetResult(2, factorMaterial, factorMaterialError);
    }

    auto _PriFit = (TH1D*)_FractionFitter->GetMCPrediction(0);
    TH1D *_DecayFit = 0, *_MaterialFit = 0, *_SecondaryFit = 0;
    if(_useTwoComponentFit){
        _SecondaryFit = (TH1D*)_FractionFitter->GetMCPrediction(1);
    }
    else {
        _DecayFit = (TH1D*)_FractionFitter->GetMCPrediction(1);
        _MaterialFit = (TH1D*)_FractionFitter->GetMCPrediction(2);
    }

    plot.AddToLeg(Form("%3.2lf < #it{p}_{T} < %3.2lf", _pTMin, _pTMax));
    plot.AddToLeg(Form("%3.2lf < #it{N}_{ch} < %3.2lf", _MultMin, _MultMax));

    SetHistogramProperties(_h1DataDCA, "DCA_{xy}", "counts", 1);

    TH1D* _result = (TH1D*)_FractionFitter->GetPlot();
    _result->SetName("_result");
    SetHistogramProperties(_result, "DCA_{xy}", "counts", 2);
    SetHistogramProperties(_PriFit, "DCA_{xy}", "counts", 3);
    if(_useTwoComponentFit){
        SetHistogramProperties(_SecondaryFit, "DCA_{xy}", "counts", 4);
    }
    else{
        SetHistogramProperties(_DecayFit, "DCA_{xy}", "counts", 4);
        SetHistogramProperties(_MaterialFit, "DCA_{xy}", "counts", 5);

    }
    plot.AddToLeg(_h1DataDCA, "data");
    plot.AddToLeg(_result, "template prediction");
    plot.AddToLeg(_PriFit, "primaries");

    plot.AddHistUpper(_h1DataDCA);
    plot.AddHistUpper(_result);
    plot.AddHistUpper(_PriFit);

    if(_useTwoComponentFit){
        plot.AddToLeg(_SecondaryFit, "secondaries");
        plot.AddHistUpper(_SecondaryFit);
    }
    else {
        plot.AddToLeg(_DecayFit, "decay");
        plot.AddToLeg(_MaterialFit, "material");
        plot.AddHistUpper(_DecayFit);
        plot.AddHistUpper(_MaterialFit);
    }

    _result->GetXaxis()->SetRange();
    _result->GetXaxis()->UnZoom();

    TH1D* _ratio = (TH1D*)_h1DataDCA->Clone("ratio");
    _ratio->Divide(_result);
    SetHistogramProperties(_ratio, "DCA_{xy}", "Data/template", 1);
    plot.AddHistLower(_ratio);
    _ratio->GetXaxis()->SetRangeUser(_PlotRangeMin, _PlotRangeMax);

    _h1DataDCA->GetYaxis()->SetRangeUser(1e-5, 2e3);
    _h1DataDCA->GetXaxis()->SetRangeUser(_PlotRangeMin, _PlotRangeMax);
    plot.CreateCanvas("LogY|Square");
    //  plot.SaveCanvas(saveName);
    std::string saveName{
        "DCA_pT" + std::to_string(_pTMin) + "-" + std::to_string(_pTMax) +
        "_mult" + std::to_string(_MultMin) + "-" + std::to_string(_MultMax)};
    plot._canvas->Write(saveName.c_str());

    DefaultPlot plotPulls;
    SetHistogramProperties(_hPulls, "Pulls", "norm. counts", 6);
    _hPulls->GetXaxis()->SetRangeUser(-5.0, 5.0);
    _hPulls->GetYaxis()->SetRangeUser(0.0, 0.25);
    _hPulls->SetStats(1);
    plotPulls.AddToLeg(_hPulls, "pulls");
    plotPulls.AddHistUpper(_hPulls);
    plotPulls.CreateCanvas("Square");
    //  saveName.replace(0, 15, "Pulls");
    //  plotPulls.SaveCanvas(saveName);
    saveName = "Pulls_pT" + std::to_string(_pTMin) + "-" +
    std::to_string(_pTMax) + "_mult" + std::to_string(_MultMin) +
    "-" + std::to_string(_MultMax);
    plotPulls._canvas->Write(saveName.c_str());
}
