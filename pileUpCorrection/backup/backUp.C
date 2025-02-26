#include "include/pileUpCorrection.h"
#include "include/Plotting.h"

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
#include "include/extractHistogram.h"
//TODO: eff. scaling pT and Nch dependent

namespace {
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using namespace RooFit;
} // namespace

void PileUpCorrection::CalculateSecondaryScaling() {
    if (!_h2CorrMapDecay || !_h2CorrMapMaterial)
        PileUpCorrection::InitHistograms();
    if (!_h2CorrMapDecay || !_h2CorrMapMaterial) {
        cerr << "ERROR: 2D histos are not created.\n";
        return;
    }

    for (int ptBin = 0; ptBin < _pTBinning.size() - 1; ++ptBin) {
        auto const pTLow = _pTBinning[ptBin];
        auto const pTHigh = _pTBinning[ptBin + 1] - 0.01;
        SetPtRange(pTLow, pTHigh);

            _ScalingDecay = 1;
            _ScalingMaterial = 1;
            _ScalingSecondary = 1;

            MakeProjections();
            RebinHistograms();
            MakeTemplateFits();

            // _hPulls->GetYaxis()->SetRangeUser(-_AmplitudePulls, _AmplitudePulls);
            // _hPulls->GetXaxis()->SetRangeUser(-0.6, 0.6);
            // _hPulls->SetName(Form("hPulls_pT_%lf_%lf", _pTMin, _pTMax));

            _OutPutFile->cd();
            //_hPulls->Write();
            _h1DataDCA->SetName(Form("h1DataDCA_pT_%lf_%lf", _pTMin, _pTMax));
            _h1DCAValidatedCollisions->SetName(Form("_1DCAValidatedCollisions_pT_%lf_%lf", _pTMin, _pTMax));
            _h1DCANotValidatedCollisions->SetName(Form("h1DCANotValidatedCollisions_pT_%lf_%lf", _pTMin, _pTMax));
           

            // auto const _Localbin = _h2CorrMapDecay->FindBin((pTLow + pTHigh) / 2., (multLow + multHigh) / 2.);
            // _h2CorrMapSecondary->SetBinContent(_Localbin, _ScalingSecondary);
    }

    if(_OutPutFile){
        _OutPutFile->cd();
        // _h2CorrMapSecondary->Write();
        // _h2CorrMapEffSecondary->Write();
    }
}

void PileUpCorrection::ChiSqTest() {
    double const chiSq = _FractionFitter->GetChisquare();
    double const NDF = _FractionFitter->GetNDF();
    double const criChiSq = TMath::ChisquareQuantile(1. - _pVal, NDF);

    if (chiSq <= criChiSq)
        SetGoodnessFitChiSq(kTRUE);
    else
        SetGoodnessFitChiSq(kFALSE);
}


void PileUpCorrection::InitHistograms() {
    //*********************************************
    //*********** Load from File ******************
    //*********************************************
    cout << "First, load histograms from files\n";
    if (_stMCFileName.c_str().size() < 5 || _stDataFileName.c_str().size() < 5) {
        cerr << "ERROR: Please set paths to MC and Data files" << endl;
        return;
    }

    cout << "Loading THnSparseD from files\n";

    //----------------------------------------------

    //----------------------------------------------
    // loading histograms
    // DCAxy:pT
    // DCAz:pT:MCinfo; MCinfo --> v0collisionId != mcGammaCollisionId:v0collisionId = mcGammaCollisionId

    // data
    cout << "\nloading histograms from data file\n";



    ///DCAz distributions

    _hnData = (THnSparseD*)extractHistogram(Form("%s:/pcm-qc/V0/primary/hDCAz_Pt",_stDataFileName.c_str()));
    if (!_hnData) {
        cerr << "ERROR: could not find histogram\n";
        return;
    }
    _hnData->SetName("_hnData");

    // MC
    cout << "loading histograms from mc file\n";

    _hnMC = (THnSparseD*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hDCAz_Pt_collType",_stMCFileName.c_str()));
    if (!_hnMC) {
        cerr << "ERROR: could not find histogram\n";
        return;
    }
    _hnMC->SetName("_hnMC");

    
    //*********************************************
    //********* create output file ****************
    //*********************************************

    _OutPutFile = TFile::Open(_stOutFileName.c_str(), "RECREATE");
}

void PileUpCorrection::MakeProjections() {
    // delete if histogram is already there
    //    if(_h1DCAValidatedCollisions) delete _h1DCAValidatedCollisions;
    //    if(_h1DCANotValidatedCollisions) delete _h1DCANotValidatedCollisions;
    //    if(_h1SecDecayDCA) delete _h1SecDecayDCA;
    //    if(_h1SecMaterialDCA) delete _h1SecMaterialDCA;
    //    if(_h1DataDCA) delete _h1DataDCA;
    //
    cout << "Make projections according to pT\n";
    //----------------------------------------------
    // set up some binnings and make projections

    
    // ------ MC -----

    //Set an integer for each axis an integer


    cout << "\nmake projections from mc hist\n";
    auto const _binBoundarayMin = _hnMC->GetAxis(_DCAxyAxis)->FindBin(-_FitBoundary);
    auto const _binBoundarayMax = _hnMC->GetAxis(_DCAxyAxis)->FindBin(_FitBoundary);
    auto const _binpTMin = _hnMC->GetAxis(_pTAxis)->FindBin(_pTMin);
    auto const _binpTMax = _hnMC->GetAxis(_pTAxis)->FindBin(_pTMax);

    

    _hnMC->GetAxis(_DCAxyAxis)->SetRange(_binBoundarayMin,
                                _binBoundarayMax); // reject overflow
    _hnMC->GetAxis(_pTAxis)->SetRange(_binpTMin, _binpTMax);


    _hnMC->GetAxis(_MCinfoAxis)->SetRangeUser(0.5, 1.5);
    _h1DCAValidatedCollisions = (TH1D*)_hnMC->Projection(_DCAxyAxis);
    _h1DCAValidatedCollisions->SetName(Form("_h1DCAValidatedCollisions_%lf_%lf", _pTMin, _pTMax));
    _h1DCAValidatedCollisions->Sumw2();
    _hnMC->GetAxis(_MCinfoAxis)->SetRangeUser(-0.5, 0.5);
    _h1DCANotValidatedCollisions = (TH1D*)_hnMC->Projection(_DCAxyAxis);
    _h1DCANotValidatedCollisions->SetName(Form("_h1DCANotValidatedCollisions_%lf_%lf", _pTMin, _pTMax));
    _h1DCANotValidatedCollisions->Sumw2();


    // ------ data -----
    cout << "make projections from data hist\n";
    auto const _binBoundarayMinData = _hnData->GetAxis(_DCAxyAxis)->FindBin(-_FitBoundary);
    auto const _binBoundarayMaxData = _hnData->GetAxis(_DCAxyAxis)->FindBin(_FitBoundary);
    auto const _binpTMinData = _hnData->GetAxis(_pTAxis)->FindBin(_pTMin);
    auto const _binpTMaxData = _hnData->GetAxis(_pTAxis)->FindBin(_pTMax);


    _hnData->GetAxis(_DCAxyAxis)->SetRange(_binBoundarayMinData, _binBoundarayMaxData); // reject overflow
    _hnData->GetAxis(_pTAxis)->SetRange(_binpTMinData, _binpTMaxData);
    _h1DataDCA = (TH1D*)_hnData->Projection(_DCAxyAxis);
    _h1DataDCA->SetName(Form("_h1DataDCA_%lf_%lf", _pTMin, _pTMax));
    _h1DataDCA->Sumw2();

    // normalize
    _dataIntegral = _h1DataDCA->Integral(_binBoundarayMinData, _binBoundarayMaxData);
    _mcValColIntegral = _h1DCAValidatedCollisions->Integral(_binBoundarayMin, _binBoundarayMax);
    _mcNotValColIntegral = _h1DCANotValidatedCollisions->Integral(_binBoundarayMin, _binBoundarayMax);
    _mcTotalIntegral = _mcValColIntegral + _mcNotValColIntegral;



    if (_mcTotalIntegral < 1) {
        cerr << "Fatal error: total integral in mc is too small\n";
        return;
    }
    if (_dataIntegral < 1) {
        cerr << "Fatal error: total integral in data is too small\n";
        return;
    }

    _h1DataDCA->Scale(1. / _dataIntegral, "width");
    _h1DCAValidatedCollisions->Scale(1. / _mcTotalIntegral, "width");
    _h1DCANotValidatedCollisions->Scale(1. / _mcTotalIntegral, "width");

    cout << "\n\nDefault fractions: \n";
    cout << "\t validated col./total: " << _mcValColIntegral / _mcTotalIntegral << "\n";
    cout << "\t not validated col./total: " << _mcNotValColIntegral / _mcTotalIntegral << "\n";
    //----------------------------------------------
}

void PileUpCorrection::RebinHistograms() {
    if (!_DoRebinning) {
        cout << "Skipping rebinnnig\n";
        return;
    }
    _h1DataDCA->Rebin(_RebinFactor);
    _h1DCAValidatedCollisions->Rebin(_RebinFactor);
    _h1DCANotValidatedCollisions->Rebin(_RebinFactor);
}

void PileUpCorrection::MakeTemplateFits() {
    cout << "\nNow performing template fits\n";

    if (0 == _FitMethod) { // use TFractionFitter
        cout << "Using TFractionFitter method\n";
//        ROOT::Math::MinimizerOptions::SetDefaultMinimizer(“Minuit2”);
        auto _objArr = new TObjArray(3);
        _objArr->Add(_h1DCAValidatedCollisions);
        _objArr->Add(_h1DCANotValidatedCollisions);


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

        auto _Fitter = _FractionFitter->GetFitter();

        _Fitter->Config().ParSettings(0).Set("valCol", _mcValColIntegral / _mcTotalIntegral, _StepSize, 0.8, 1.0);
        _Fitter->Config().ParSettings(1).Set("notValCol", _mcNotValColIntegral / _mcTotalIntegral, _StepSize, 0.0, 0.5);

        // perform fit
        _FractionFitter->ReleaseRangeX();
        _FractionFitter->ReleaseRangeY();
        _FitStatus = _FractionFitter->Fit();
        _FitStatus = _FractionFitter->Fit();
        _FitStatus = _FractionFitter->Fit();
        // Add to Debug array;
        _DebugArray->Add(_h1DCAValidatedCollisions);
        _DebugArray->Add(_h1DCANotValidatedCollisions);
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
            //Pulls();

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

            double factorValCollisions = 1, factorValCollisionsError = 1, factorDecay = 1,
            factorDecayError = 1, factorMaterial = 1,
            factorMaterialError = 1, factorNotValCollisions = 1, factorSecondayError = 1;
            _FractionFitter->GetResult(0, factorValCollisions, factorValCollisionsError);
            if(_useTwoComponentFit){
                _FractionFitter->GetResult(1, factorNotValCollisions, factorSecondayError);
            }
            else {
                _FractionFitter->GetResult(1, factorDecay, factorDecayError);
                _FractionFitter->GetResult(2, factorMaterial, factorMaterialError);
            }

            cout << "\nnew fractions: \n";
            cout << "\t pri/total: " << factorValCollisions << " +- " << factorValCollisionsError
            << "\n";
            if(_useTwoComponentFit){
                cout << "\t secondary/total: " << factorNotValCollisions << " +- "
                << factorSecondayError << "\n";
            } else {
                cout << "\t secDecay/total: " << factorDecay << " +- "
                << factorDecayError << "\n";
                cout << "\t secMaterial/total: " << factorMaterial << " +- "
                << factorMaterialError << "\n\n";
            }

            cout << "\nResulting scaling factors: \n";
            cout << "\t pri: " << factorValCollisions / (_mcValColIntegral / _mcTotalIntegral) << "\n";
            _ScalingSecondary = factorNotValCollisions / (_mcNotValColIntegral/_mcTotalIntegral);

            cout << "\t secondary: " << _ScalingSecondary << "\n";

            MakePlot();
        } else { // fit failed
            cerr << "\n\nERROR: Fit didn't converged!!\n\n";
            return;
        }
    }
    // else if (1 == _FitMethod) { // use RooFit PDF method
    //     cout << "Using RooFit method\n";

    //     // Converting histograms
    //     RooRealVar xAxis("xAxis", "DCA_{xy}", -1, 1);

    //     // MC
    //     RooDataHist dValidatedCollisionsDCA("dValidatedCollisionsDCA", "dValidatedCollisionsDCA", xAxis, Import(*_h1DCAValidatedCollisions));
    //     RooDataHist dNotValidatedCollisionsDCA("dNotValidatedCollisionsDCA", "dNotValidatedCollisionsDCA", xAxis, Import(*_h1DCANotValidatedCollisions));

    //     // Data
    //     RooDataHist dDataDCA("dDataDCA", "dDataDCA", xAxis,
    //                          Import(*_h1DataDCA));

    //     // converting to PDFs
    //     RooHistPdf pValidatedCollisionsDCA("pValidatedCollisionsDCA", "pValidatedCollisionsDCA", RooArgSet(xAxis), dValidatedCollisionsDCA);
    //     RooHistPdf pNotValidatedCollisionsDCA("pNotValidatedCollisionsDCA", "pNotValidatedCollisionsDCA", RooArgSet(xAxis), dNotValidatedCollisionsDCA);
  
        
    //     // creating model with fractions
    //     auto const defaultFractionValCollisions = _mcValColIntegral / _mcTotalIntegral;
    //     auto const defaultFractionNotValCollisions = _mcNotValColIntegral / _mcTotalIntegral;


    //     RooRealVar valCollFrac("valCollFrac", "valCollFrac", defaultFractionValCollisions, defaultFractionValCollisions/3., TMath::Min(0.5, defaultFractionValCollisions*3));
    //     RooRealVar notValCollFrac("notValCollFrac", "notValCollFrac", defaultFractionNotValCollisions, defaultFractionNotValCollisions/3., TMath::Min(0.5, defaultFractionNotValCollisions*4));

    //     // NB: Each coefficient is interpreted as the fraction of the
    //     // left-hand component of the i-th recursive sum, i.e.
    //     //
    //     //   sum4 = A + ( B + ( C + D)  with fraction fA, fB and fC expands to
    //     //
    //     //   sum4 = fA*A + (1-fA)*(fB*B + (1-fB)*(fC*C + (1-fC)*D))
    //     //
    //     //   model = pri + secDec + secMat --> fracPri + fracSecDec
    //     //   model = fracPri*pri + (1-fracPri) * ( secDec*fracSecDec + (1-fracSecDec)*secMat )
    //     // --> f'SecDec = (1-fracPir)*fracSecDec
    //     // --> f'SecMat = (1-fracPir)*(1-fracSecDec)


    //     RooAddPdf model2("model2", "pri+sec", RooArgList(pValidatedCollisionsDCA, pNotValidatedCollisionsDCA), RooArgList(notValCollFrac));
    //     // fit model to data
    //     model2.fitTo(dDataDCA);
    //     model2.Print("t");

    //     std::string saveName = "DCA_pT_2Template_" + std::to_string(_pTMin) + "-" + std::to_string(_pTMax);

    //     RooPlot *xframe2 = xAxis.frame(Title(saveName.c_str()));
    //     dDataDCA.plotOn(xframe2, LineColor(GetNiceColor(1)), MarkerColor(GetNiceColor(1)));
    //     model2.plotOn(xframe2, LineColor(GetNiceColor(2)));
    //     // Overlay the background component of model with a dashed line
    //     model2.plotOn(xframe2, Components(pValidatedCollisionsDCA),LineColor(GetNiceColor(3)), LineStyle(kSolid));
    //     model2.plotOn(xframe2, Components(pNotValidatedCollisionsDCA),LineColor(GetNiceColor(4)), LineStyle(kSolid));
    //     _OutPutFile->cd();
    //     xframe2->Write();


    //     auto const fValColl = valCollFrac.getValV();
    //     auto const fNotValColl = valCollFrac.getError();
    //     auto const fNotValColl = 1 - notValCollFrac.getValV();
    //     auto const fSecError = notValCollFrac.getError()/notValCollFrac.getValV() * fNotValColl;



    //     // obtain fractions
    //     cout << "\nnew fractions: \n";
    //     cout << "\t val./total: " << fValColl << "\n";
    //     cout << "\t Two template:\n";
    //     cout << "\t secondaries/total: " << fNotValColl << "\n\n";

    //     cout << "\nResulting scaling factors: \n";
    //     cout << "\t pri: "
    //     << fValColl / (_mcValColIntegral / _mcTotalIntegral) << "\n";
    //     cout << "\t Two template:\n";
    //     _ScalingSecondary = fNotValColl / (_mcNotValColIntegral / _mcTotalIntegral);
    //     cout << "\t secondary: " << _ScalingSecondary << "\n";
    //     // create plot

    // } else {
        cout << "WARNING: not supported fit method\n";
    }
}

void PileUpCorrection::MakePlot() {
    if (0 != _FitStatus)
        return;

    _OutPutFile->cd();

    DefaultPlot plot;
    double factorValCollisions = 0, factorValCollisionsError = 0, factorNotValCollisions = 0, factorNotValCollisionsError = 0;
    _FractionFitter->GetResult(0, factorValCollisions, factorValCollisionsError);
    if(_useTwoComponentFit){
        _FractionFitter->GetResult(1, factorNotValCollisions, factorNotValCollisionsError);
    } 

    auto _ValColFit = (TH1D*)_FractionFitter->GetMCPrediction(0);
    TH1D* _NotValColFit = (TH1D*)_FractionFitter->GetMCPrediction(1);
    


    plot.AddToLeg(Form("%3.2lf < #it{p}_{T} < %3.2lf", _pTMin, _pTMax));

    SetHistogramProperties(_h1DataDCA, "DCA_{xy}", "counts", 1);

    TH1D* _result = (TH1D*)_FractionFitter->GetPlot();
    _result->SetName("_result");
    SetHistogramProperties(_result, "DCA_{xy}", "counts", 2);
    SetHistogramProperties(_ValColFit, "DCA_{xy}", "counts", 3);
    SetHistogramProperties(_NotValColFit, "DCA_{xy}", "counts", 4);

  
    plot.AddToLeg(_h1DataDCA, "data");
    plot.AddToLeg(_result, "template prediction");
    plot.AddToLeg(_ValColFit, "val. collisions");
    plot.AddToLeg(_NotValColFit, "unval. collisions");


    plot.AddHistUpper(_h1DataDCA);
    plot.AddHistUpper(_result);
    plot.AddHistUpper(_ValColFit);
    plot.AddHistUpper(_NotValColFit);


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
        "DCA_pT" + std::to_string(_pTMin) + "-" + std::to_string(_pTMax);
    plot._canvas->Write(saveName.c_str());

    // DefaultPlot plotPulls;
    // SetHistogramProperties(_hPulls, "Pulls", "norm. counts", 6);
    // _hPulls->GetXaxis()->SetRangeUser(-5.0, 5.0);
    // _hPulls->GetYaxis()->SetRangeUser(0.0, 0.25);
    // _hPulls->SetStats(1);
    // plotPulls.AddToLeg(_hPulls, "pulls");
    // plotPulls.AddHistUpper(_hPulls);
    // plotPulls.CreateCanvas("Square");
    // saveName = "Pulls_pT" + std::to_string(_pTMin) + "-" +
    // std::to_string(_pTMax) + "_mult" + std::to_string(_MultMin) +
    // "-" + std::to_string(_MultMax);
    // plotPulls._canvas->Write(saveName.c_str());
}
