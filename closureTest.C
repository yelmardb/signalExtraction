#include "includes/newSignalExtraction.h"
#include "includes/inputData.h"


TH1D* doEfficiencyCorrectionRun2(TH1D* h1YieldUncorrected, TH1D* effXAcc);
TH1D* doEfficiencyCorrection(TH1D* h1YieldUncorrected, TH1D* effXAcc);

void closureTest(){

    Double_t etaRange = 1.6;

    const char* meson = "Pi0";

    const char* inFileName_Pi0_DATA = strdup(Form("outputSignalExtraction/%s/SignalExtraction_%s_Data_%d.root", period, meson, runNumberData));
    const char* inFileName_Pi0_MC = strdup(Form("outputSignalExtraction/%s/SignalExtraction_%s_MC_%d.root", period, meson, runNumberMC));
    const char* inFileName_Pi0_TrueMC =  strdup(Form("outputSignalExtraction/%s/SignalExtraction_%s_TrueMC_%d.root", period, meson, runNumberMC));
    const char* inFileTrueMCName = strdup(Form("inputFiles/%s/AnalysisResults_MC_%d.root", period, runNumberTrueMC));


    TH1D* h1yieldUncorrected = (TH1D*)extractHistogram(Form("%s:/h1yieldUnNorm", inFileName_Pi0_MC)); //raw yield MC
    TH1D* mesonAccXEffxBRTrue = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", inFileName_Pi0_TrueMC)); //accXEffXBr_True
    TH1D* mesonAccXEffxBR = (TH1D*)extractHistogram(Form("%s:/mesonAccXEff_Pi0", inFileName_Pi0_MC)); //accXEffXBr_Rec

   
    TH1D* hCollisionCounterMC = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter", inFileName_Pi0_MC));
    Double_t nEventsMC = hCollisionCounterMC->GetBinContent(12);
    TH1D* hCollisionCounterTrueMC = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter", inFileName_Pi0_TrueMC));
    Double_t nEventsTrueMC = hCollisionCounterTrueMC->GetBinContent(12);
    TH1D* hCollisionCounterData = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter", inFileName_Pi0_DATA));
    Double_t nEventsData = hCollisionCounterData->GetBinContent(12);


    TH1D* h1yieldCorr_wTrueEff = doEfficiencyCorrection(h1yieldUncorrected, mesonAccXEffxBRTrue);
    FinalizePt(h1yieldCorr_wTrueEff, nEventsMC, etaRange); // cval = (etarange*nevents>0) ? (val)/(width * 2.0 * TMath::Pi() * etarange * nevents * pt) : 0;

    TH1D* h1yieldCorr_wRecEff = doEfficiencyCorrection(h1yieldUncorrected, mesonAccXEffxBR);
    FinalizePt(h1yieldCorr_wRecEff, nEventsTrueMC, etaRange); // cval = (etarange*nevents>0) ? (val)/(width * 2.0 * TMath::Pi() * etarange * nevents * pt) : 0;

    SetHistogramPropertiesAlt(h1yieldCorr_wTrueEff, "#it{p}_{T} GeV/#it{c}", "#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}", 0);
    SetHistogramPropertiesAlt(h1yieldCorr_wRecEff, "#it{p}_{T} GeV/#it{c}", "#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}", 1);
    h1yieldCorr_wTrueEff->SetMarkerStyle(4);
    h1yieldCorr_wRecEff->SetMarkerStyle(4);


    
    //Load and rebin generated Pi0 yield
    TH1D* h1GeneratedYieldPi0  = (TH1D*)extractHistogram(Form("%s:/pi0eta-to-gammagamma-mc-pcmpcm/Generated/Pi0/hPt", inFileTrueMCName));
    TArrayD *binarrGeneratedYield= (TArrayD*)h1yieldUncorrected->GetXaxis()->GetXbins();
    TH1D *h1GeneratedYieldPi0_Rebinned = (TH1D*)h1GeneratedYieldPi0->Clone("h1GeneratedYieldPi0_Rebinned");
    h1GeneratedYieldPi0_Rebinned->Reset();
    h1GeneratedYieldPi0_Rebinned = RebinHistogram(h1GeneratedYieldPi0,h1yieldUncorrected->GetNbinsX(),binarrGeneratedYield->GetArray());

    FinalizePt(h1GeneratedYieldPi0, nEventsTrueMC, etaRange); // cval = (etarange*nevents>0) ? (val)/(width * 2.0 * TMath::Pi() * etarange * nevents * pt) : 0;
    FinalizePt(h1GeneratedYieldPi0_Rebinned, nEventsTrueMC, etaRange);
    

    SetHistogramPropertiesAlt(h1GeneratedYieldPi0, "#it{p}_{T} GeV/#it{c}", "#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}", 14);
    h1GeneratedYieldPi0->SetMarkerStyle(24);
    SetHistogramPropertiesAlt(h1GeneratedYieldPi0_Rebinned, "#it{p}_{T} GeV/#it{c}", "#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}", 14);
    h1GeneratedYieldPi0_Rebinned->SetMarkerStyle(32);


    TH1D* h1Ratio_wTrueEff = (TH1D*)h1yieldCorr_wTrueEff->Clone("h1Ratio_wTrueEff");
    h1Ratio_wTrueEff->Divide(h1GeneratedYieldPi0_Rebinned);
    SetHistogramPropertiesAlt(h1Ratio_wTrueEff, "#it{p}_{T} GeV/#it{c}", "rec. MC/ gen. MC", 0);


    TH1D* h1Ratio_wRecEff = (TH1D*)h1yieldCorr_wRecEff->Clone("h1Ratio_wRecEff");
    h1Ratio_wRecEff->Divide(h1GeneratedYieldPi0_Rebinned);
    SetHistogramPropertiesAlt(h1Ratio_wRecEff, "#it{p}_{T} GeV/#it{c}", "corrected and rec. MC/ gen. MC", 1);


    TLegend* legend = new TLegend(0.528249,0.672012,0.805085,0.87172);
    legend->AddEntry((TObject*)0x0,"This work pp collisions", "");
    legend->AddEntry((TObject*)0x0,"PCMPCM", "");
    legend->AddEntry((TObject*)0x0,"Closure test", "");
    legend->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
    legend->AddEntry(h1Ratio_wTrueEff, "corrected w/ true eff.", "lp");
    legend->AddEntry(h1Ratio_wRecEff, "corrected w/ rec. eff.", "lp");
    legend->AddEntry(h1GeneratedYieldPi0, "generated MC", "lp");
    legend->AddEntry(h1GeneratedYieldPi0_Rebinned, "generated MC rebinned", "lp");


    TObjArray* arr = new TObjArray();
    arr->Add(h1yieldCorr_wTrueEff);
    arr->Add(h1yieldCorr_wRecEff);
    arr->Add(h1GeneratedYieldPi0);
    arr->Add(h1GeneratedYieldPi0_Rebinned);
    arr->Add(legend);

    TObjArray* arrRatio = new TObjArray();
    arrRatio->Add(h1Ratio_wTrueEff);
    arrRatio->Add(h1Ratio_wRecEff);

    TCanvas* canvas = makeCanvas(arr, arrRatio, "LogX LogY", 0, 0);



    ///RUN 2 

   TH1D* hUncorrectedYieldPi0Run2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/RAWYieldPerEventsPi0_INT7");
   TH1D* effPi0Run2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/EffTimesAccPi0_INT7"));
   Double_t branchingRatio = 0.98823;
   effPi0Run2->Scale(branchingRatio);

   TH1D* hCorrectedYieldPi0Run2 = (TH1D*)doEfficiencyCorrectionRun2(hUncorrectedYieldPi0Run2, effPi0Run2);
   SetHistogramPropertiesAlt(hCorrectedYieldPi0Run2, "#it{p}_{T} GeV/#it{c}", "#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}", 14);
   FinalizePtTmp(hCorrectedYieldPi0Run2, 1.);


   TH1D* hCorrectedYieldPi0Run3 = (TH1D*)extractHistogram(Form("%s:/h1yieldCorr", inFileName_Pi0_DATA)); //raw yield MC


   TH1D* hratio = (TH1D*)hCorrectedYieldPi0Run3->Clone("hratio");
   hratio = MakeRatioSpectra4Eta2Pi0Ratio(hCorrectedYieldPi0Run3, hCorrectedYieldPi0Run2, "quiet");
   SetHistogramPropertiesAlt(hratio, "#it{p}_{T} GeV/#it{c}", "corrected and rec. MC/ gen. MC", 1);



   TLegend* legend2 = new TLegend(0.528249,0.672012,0.805085,0.87172);
   legend2->AddEntry((TObject*)0x0,"This work pp collisions", "");
   legend2->AddEntry((TObject*)0x0,"PCMPCM", "");
   legend2->AddEntry((TObject*)0x0,"#pi^{0} #rightarrow #gamma + #gamma", "");
   legend2->AddEntry(hCorrectedYieldPi0Run2, "Eff x Acc. corrected yield (Run2)", "lp");
   legend2->AddEntry(hCorrectedYieldPi0Run3, "Eff x Acc. corrected yield (Run3)", "lp");


   TObjArray* arr2 = new TObjArray();
   arr2->Add(hCorrectedYieldPi0Run3);
   arr2->Add(hCorrectedYieldPi0Run2);

   TObjArray* arrRatio2 = new TObjArray();
   arrRatio2->Add(hratio);

   TCanvas* canvas2 = makeCanvas(arr2, arrRatio2, "LogX LogY", 0, 0);


   ///Cross section
   Double_t totXSectionRun3 = 59.4*1e+9; // in pbarn
   Double_t totXSectionRun2 = 1e+9*57.9; 


   TH1D* hDiffCrossSectionRun3 = (TH1D*)hCorrectedYieldPi0Run3->Clone("hDiffCrossSectionRun3");
   hDiffCrossSectionRun3->Scale(totXSectionRun3);

   TH1D* hDiffCrossSectionRun2 = (TH1D*)hCorrectedYieldPi0Run2->Clone("hDiffCrossSectionRun2");
   hDiffCrossSectionRun2->Scale(totXSectionRun2);

   TObjArray* arr3 = new TObjArray();
   arr3->Add(hDiffCrossSectionRun3);
   arr3->Add(hDiffCrossSectionRun2);


   TH1D* hratioXSec = (TH1D*)hDiffCrossSectionRun3->Clone("hratio");
   hratioXSec = MakeRatioSpectra4Eta2Pi0Ratio(hDiffCrossSectionRun3, hDiffCrossSectionRun2, "quiet");

    TObjArray* arrRatio3 = new TObjArray();
    arrRatio3->Add(hratioXSec);
    TCanvas* canvas3 = makeCanvas(arr3, arrRatio3, "LogX LogY", 0, 0);



}


TH1D* doEfficiencyCorrection(TH1D* h1YieldUncorrected, TH1D* effXAcc){

    TH1D* h1YieldUncorrectedCorrected = (TH1D*)h1YieldUncorrected->Clone("h1YieldUncorrectedCorrected");
    h1YieldUncorrectedCorrected->Reset();

    for(int i = 0; i <= h1YieldUncorrectedCorrected->GetNbinsX(); i++){
 
        Double_t contentYield = h1YieldUncorrected->GetBinContent(i);
        Double_t errorYield = h1YieldUncorrected->GetBinError(i);
        Double_t contentAccXEffxBR = effXAcc->GetBinContent(effXAcc->FindBin(h1YieldUncorrected->GetBinCenter(i)));
        Double_t errorAccXEffxBR = effXAcc->GetBinError(i);
        Double_t error = TMath::Sqrt( TMath::Power(errorYield/contentAccXEffxBR, 2) + TMath::Power(contentYield*errorAccXEffxBR/contentAccXEffxBR*contentAccXEffxBR, 2));

        h1YieldUncorrectedCorrected->SetBinContent(i, contentYield/contentAccXEffxBR);
        h1YieldUncorrectedCorrected->SetBinError(i, error);
    }

    return h1YieldUncorrectedCorrected;

}



TH1D* doEfficiencyCorrectionRun2(TH1D* h1YieldUncorrected, TH1D* effXAcc){

    h1YieldUncorrected->GetXaxis()->SetRangeUser(0.4, 10.);
    effXAcc->GetXaxis()->SetRangeUser(0.4, 10.);

    TH1D* h1YieldUncorrectedCorrected = (TH1D*)h1YieldUncorrected->Clone("h1YieldUncorrectedCorrected");
    h1YieldUncorrectedCorrected->Reset();

    for(int i = 0; i < h1YieldUncorrectedCorrected->GetNbinsX(); i++){
 
        Double_t contentYield = h1YieldUncorrected->GetBinContent(i);
        Double_t errorYield = h1YieldUncorrected->GetBinError(i);
        Double_t contentAccXEffxBR = effXAcc->GetBinContent(effXAcc->FindBin(h1YieldUncorrected->GetBinCenter(i)));
        Double_t errorAccXEffxBR = effXAcc->GetBinError(i);
        Double_t error = TMath::Sqrt( TMath::Power(errorYield/contentAccXEffxBR, 2) + TMath::Power(contentYield*errorAccXEffxBR/contentAccXEffxBR*contentAccXEffxBR, 2));

        h1YieldUncorrectedCorrected->SetBinContent(i, contentYield/contentAccXEffxBR);
        h1YieldUncorrectedCorrected->SetBinError(i, error);
    }

    h1YieldUncorrectedCorrected->GetXaxis()->SetRangeUser(0.4, 10.);

    return h1YieldUncorrectedCorrected;

}
