#include "includes/Plotting.h"
#include "includes/extractHistogram.h"
#include "TF1.h"
#include "TH1.h"
#include <stdio.h>
#include "includes/inputData.h"


TH1D* getSinglePhotonEff(const char* trainOutputMC);
TH1D* getPhotonConvProbability(const char* trainOutputMC);
TH2D* getTH2Dhisto(const char* trainOutputMC, const char* hname);
TCanvas* doDoubleCanvasTH2(TH2D* h1, TH2D* h2);
TH1D* GetTrueMC_PtReso(const char* trainOutputMC, Int_t color, Int_t markerStyle);


void mcQA(){

  Int_t runnNumber_LHC24f3c = 348247;
  Int_t runnNumber_LHC24f3c_NoMCTunedOnData = 337539;
  Int_t runNumberData = 351015; //pass7 with cut variations

  const char* trainOutputMC_LHC24f3c = Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root", runnNumber_LHC24f3c);
  const char* trainOutputMC_LHC24f3c_NoMCTunedOnData =  Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root", runnNumber_LHC24f3c_NoMCTunedOnData);

  const char* signalExtractionMC_Pi0_LHC24f3c = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_MC_%d.root", runnNumber_LHC24f3c);
  const char* signalExtractionTrueMC_Pi0_LHC24f3c = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_TrueMC_%d.root", runnNumber_LHC24f3c);

  const char* signalExtractionMC_Eta_LHC24f3c = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_MC_%d.root", runnNumber_LHC24f3c);
  const char* signalExtractionTrueMC_Eta_LHC24f3c = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_TrueMC_%d.root", runnNumber_LHC24f3c);

  const char* signalExtractionMC_Pi0_LHC24f3c_NoMCTunedOnData = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_MC_%d.root", runnNumber_LHC24f3c_NoMCTunedOnData);
  const char* signalExtractionTrueMC_Pi0_LHC24f3c_NoMCTunedOnData = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Pi0_TrueMC_%d.root", runnNumber_LHC24f3c_NoMCTunedOnData);

  const char* signalExtractionMC_Eta_LHC24f3c_NoMCTunedOnData = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_MC_%d.root", runnNumber_LHC24f3c_NoMCTunedOnData);
  const char* signalExtractionTrueMC_Eta_LHC24f3c_NoMCTunedOnData = Form("outputSignalExtraction/LHC22oPass7/SignalExtraction_Eta_TrueMC_%d.root", runnNumber_LHC24f3c_NoMCTunedOnData);


  ///Event selection
  TH1D* hCollisionCounter_LHC24f3c_NoMCTunedOnData = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter", signalExtractionMC_Pi0_LHC24f3c));
  SetHistogramPropertiesAlt(hCollisionCounter_LHC24f3c_NoMCTunedOnData, "", "events", 0);
  hCollisionCounter_LHC24f3c_NoMCTunedOnData->GetYaxis()->SetRangeUser(1e5, 1e10);
  hCollisionCounter_LHC24f3c_NoMCTunedOnData->SetLineWidth(4);

  TH1D* hCollisionCounter_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/hCollisionCounter", signalExtractionMC_Pi0_LHC24f3c_NoMCTunedOnData));
  SetHistogramPropertiesAlt(hCollisionCounter_LHC24f3c, "", "events", 1);
  hCollisionCounter_LHC24f3c->GetYaxis()->SetRangeUser(1e5, 1e10);
  hCollisionCounter_LHC24f3c->SetLineWidth(4);

  TLegend* legendEvents = new TLegend(0.528249,0.672012,0.805085,0.87172);
  legendEvents->AddEntry((TObject*)0x0,"pp collisions", "");
  legendEvents->AddEntry((TObject*)0x0,"PCMPCM", "");
  legendEvents->AddEntry((TObject*)0x0,"#pi^{0} /#eta #rightarrow #gamma + #gamma", "");
  legendEvents->AddEntry(hCollisionCounter_LHC24f3c, "LHC24f3c", "lp");
  legendEvents->AddEntry(hCollisionCounter_LHC24f3c_NoMCTunedOnData, "LHC24f3c_NoMCTunedOnData", "lp");

    
  TObjArray* arrEvents = new TObjArray();
  arrEvents->Add(hCollisionCounter_LHC24f3c_NoMCTunedOnData);
  arrEvents->Add(hCollisionCounter_LHC24f3c);
  arrEvents->Add(legendEvents);
  TCanvas* canEvents = (TCanvas*)makeCanvas(arrEvents, 0, "CMYK|NoTime|LogY|", 0, 0);
  

  ///****************************************************************** Meson efficiencies  ************************************************************************************* */
  Double_t branchingRatioPi0 = 0.98823;
  Double_t branchingRatioEta = 0.3941;

  ///Run 2 efficiencies
  TH1D* effPi0Run2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Pi013TeV/EfficiencyPi0_INT7"));
  SetHistogramPropertiesAlt(effPi0Run2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
  effPi0Run2->SetMarkerStyle(4);
  effPi0Run2->GetXaxis()->SetRangeUser(0.4, 10);
  effPi0Run2->Scale(branchingRatioPi0);
  TH1D* effEtaRun2 = (TH1D*)extractHistogram(Form("/Users/yelmard/Downloads/data_PCMResultsFullCorrection_PP.root:/Eta13TeV/EfficiencyEta_INT7"));
  SetHistogramPropertiesAlt(effEtaRun2, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
  effEtaRun2->SetMarkerStyle(4);
  effEtaRun2->Scale(branchingRatioEta);

  ///Run 3 efficiencies, 
  ///Pi0
  TH1D* mesonAccXEffxBRTrueMC_Pi0_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", signalExtractionTrueMC_Pi0_LHC24f3c));
  SetHistogramPropertiesAlt(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
  TH1D* mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Pi0", signalExtractionTrueMC_Pi0_LHC24f3c_NoMCTunedOnData));
  SetHistogramPropertiesAlt(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 0);
  mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData->SetMarkerStyle(26);



  ///Eta
  TH1D* mesonAccXEffxBRTrueMC_Eta_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", signalExtractionTrueMC_Eta_LHC24f3c));
  SetHistogramPropertiesAlt(mesonAccXEffxBRTrueMC_Eta_LHC24f3c, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
  TH1D* mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData = (TH1D*)extractHistogram(Form("%s:/mesonAccXEffxBRTrue_Eta", signalExtractionTrueMC_Eta_LHC24f3c_NoMCTunedOnData));
  SetHistogramPropertiesAlt(mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);
  mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData->SetMarkerStyle(26);

  ///Ratios
  TH1D* effRatios_Pi0 = (TH1D*)mesonAccXEffxBRTrueMC_Pi0_LHC24f3c->Clone("effRatios_Pi0");
  effRatios_Pi0->Divide(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData);
  effRatios_Pi0->GetYaxis()->SetTitle("LHC24f3c / LHC24f3c_NoMCTunedOnData");
  effRatios_Pi0->GetYaxis()->SetRangeUser(0.9, 1.1);

  TH1D* effRatios_Eta = (TH1D*)mesonAccXEffxBRTrueMC_Eta_LHC24f3c->Clone("effRatios_Eta");
  effRatios_Eta->Divide(mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData);
  effRatios_Eta->GetYaxis()->SetTitle("f3c / f3b2");
  effRatios_Eta->GetYaxis()->SetRangeUser(0.9, 1.1);

  TLegend* legendEffs = new TLegend(0.528249,0.672012,0.805085,0.87172);
  legendEffs->AddEntry((TObject*)0x0,"pp collisions", "");
  legendEffs->AddEntry((TObject*)0x0,"PCMPCM", "");
  legendEffs->AddEntry((TObject*)0x0,"#pi^{0} /#eta #rightarrow #gamma + #gamma", "");
  legendEffs->AddEntry(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c, "#pi^{0} 13.6 TeV LHC24f3c", "lp");
  legendEffs->AddEntry(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData, "#pi^{0} 13.6 TeV LHC24f3c_NoMCTunedOnData", "lp");
  legendEffs->AddEntry(mesonAccXEffxBRTrueMC_Eta_LHC24f3c, "#eta 13.6 TeV LHC24f3c", "lp");
  legendEffs->AddEntry(mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData, "#eta 13.6 TeV LHC24f3c_NoMCTunedOnData", "lp");
  legendEffs->AddEntry(effPi0Run2, "#pi^{0} 13 TeV", "lp");
  legendEffs->AddEntry(effEtaRun2, "#eta 13 TeV", "lp");


  TObjArray* arrEffs = new TObjArray();
  arrEffs->Add(effPi0Run2);
  arrEffs->Add(effEtaRun2);
  arrEffs->Add(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c);
  arrEffs->Add(mesonAccXEffxBRTrueMC_Pi0_LHC24f3c_NoMCTunedOnData);
  arrEffs->Add(mesonAccXEffxBRTrueMC_Eta_LHC24f3c);
  arrEffs->Add(mesonAccXEffxBRTrueMC_Eta_LHC24f3c_NoMCTunedOnData);
  arrEffs->Add(legendEffs);

  TObjArray* arrRatioEffs = new TObjArray();
  arrRatioEffs->Add(effRatios_Pi0);
  arrRatioEffs->Add(effRatios_Eta);
  
  TCanvas* canEffs = (TCanvas*)makeCanvas(arrEffs, arrRatioEffs, "CMYK|NoTime|LogX|LogY", 0, 0);

  cout << "debuging... line: " << __LINE__ << endl;

  // ///pt resolution
  TH1D* hPtResolution1D_LHC24f3c = GetTrueMC_PtReso(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root", runnNumber_LHC24f3c), 14, 26);
  TH1D* hPtResolution1D_LHC24f3c_NoMCTunedOnData = GetTrueMC_PtReso(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root", runnNumber_LHC24f3c_NoMCTunedOnData), 13, 4);

  TH1D* hpTResolution1D_ratio = (TH1D*)hPtResolution1D_LHC24f3c->Clone("hpTResolution1D_ratio");
  hpTResolution1D_ratio->Divide(hPtResolution1D_LHC24f3c_NoMCTunedOnData);
  hpTResolution1D_ratio->GetYaxis()->SetTitle("LHC24f3c / LHC24f3c_NoMCTunedOnData");
  hpTResolution1D_ratio->GetYaxis()->SetRangeUser(0.9, 1.1);

  TLegend* legendPtReso = new TLegend(0.528249,0.672012,0.805085,0.87172);
  legendPtReso->AddEntry((TObject*)0x0,"pp collisions", "");
  legendPtReso->AddEntry((TObject*)0x0,"PCMPCM", "");
  legendPtReso->AddEntry((TObject*)0x0,"#gamma  #rightarrow  #it{e}^{+}#it{e}^{-}", "");
  legendPtReso->AddEntry(hPtResolution1D_LHC24f3c, "LHC24f3c", "lp");
  legendPtReso->AddEntry(hPtResolution1D_LHC24f3c_NoMCTunedOnData, "LHC24f3c_NoMCTunedOnData", "lp");

  TObjArray* arrPtReso = new TObjArray();
  arrPtReso->Add(hPtResolution1D_LHC24f3c);
  arrPtReso->Add(hPtResolution1D_LHC24f3c_NoMCTunedOnData);
  arrPtReso->Add(legendPtReso);

  TObjArray* arrPtResoRatio = new TObjArray();
  arrPtResoRatio->Add(hpTResolution1D_ratio);

  TCanvas* cPtReso = (TCanvas*)makeCanvas(arrPtReso, arrPtResoRatio, "CMYK|NoTime|LogX", 0, 0);




  // cout << "debuging... line: " << __LINE__ << endl;
  // ///v0 radius
  // cout << "trainOutputMC_LHC24f3c: " << trainOutputMC_LHC24f3c << endl;
   TH2D* hV0Radius_LHC24f3c = (TH2D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hRadius", runnNumber_LHC24f3c));
   hV0Radius_LHC24f3c->SetStats(0);
   TH2D* hV0Radius_LHC24f3c_NoMCTunedOnData = (TH2D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hRadius", runnNumber_LHC24f3c_NoMCTunedOnData));
   hV0Radius_LHC24f3c_NoMCTunedOnData->SetStats(0);
   TCanvas* cV0Radius = doDoubleCanvasTH2(hV0Radius_LHC24f3c, hV0Radius_LHC24f3c_NoMCTunedOnData);
  
   
  // ///AP plot
   TH2D* hAPplot_LHC24f3c = (TH2D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hAPplot", runnNumber_LHC24f3c));
  hAPplot_LHC24f3c->SetStats(0);
  hAPplot_LHC24f3c->GetYaxis()->SetRangeUser(0.001, 0.03);
  TH2D* hAPplot_LHC24f3c_NoMCTunedOnData = (TH2D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hAPplot", runnNumber_LHC24f3c_NoMCTunedOnData));
  hAPplot_LHC24f3c_NoMCTunedOnData->SetStats(0);
  hAPplot_LHC24f3c_NoMCTunedOnData->GetYaxis()->SetRangeUser(0.001, 0.03);

  TCanvas* cAPplot = doDoubleCanvasTH2(hAPplot_LHC24f3c, hAPplot_LHC24f3c_NoMCTunedOnData);

  ///TPCNsigmaEl

  TH2D* hTPCNsigmaEl_LHC22o = (TH2D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_Data_%d.root:/pcm-qc/V0Leg/primary/hTPCNsigmaEl", runNumberData));
  hTPCNsigmaEl_LHC22o->SetStats(0);

  TH2D* hTPCNsigmaEl_LHC24f3c = (TH2D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0Leg/primary/hTPCNsigmaEl", runnNumber_LHC24f3c));
  hTPCNsigmaEl_LHC24f3c->SetStats(0);

  TH2D* hTPCNsigmaEl_LHC24f3c_NoMCTunedOnData = (TH2D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0Leg/primary/hTPCNsigmaEl", runnNumber_LHC24f3c_NoMCTunedOnData));
  hTPCNsigmaEl_LHC24f3c_NoMCTunedOnData->SetStats(0);

  //TCanvas* cTPCNsigmaEl = doDoubleCanvasTH2(hTPCNsigmaEl_LHC24f3c, hTPCNsigmaEl_LHC24f3c_NoMCTunedOnData);

  TH1D* hTPCNsigmaEl1D_LHC24f3c = (TH1D*)hTPCNsigmaEl_LHC24f3c->ProjectionY();
  SetHistogramPropertiesAlt(hTPCNsigmaEl1D_LHC24f3c, "", "", 0);
  hTPCNsigmaEl1D_LHC24f3c->Scale(1./hTPCNsigmaEl1D_LHC24f3c->Integral());
  hTPCNsigmaEl1D_LHC24f3c->SetName("TPCNsigmaEl1D_LHC24f3c");
  hTPCNsigmaEl1D_LHC24f3c->GetXaxis()->SetTitle("n #sigma_{TPC}^{e}");

  TH1D* hTPCNsigmaEl1D_LHC24f3c_NoMCTunedOnData = (TH1D*)hTPCNsigmaEl_LHC24f3c_NoMCTunedOnData->ProjectionY();
  SetHistogramPropertiesAlt(hTPCNsigmaEl1D_LHC24f3c_NoMCTunedOnData, "", "", 1);
  hTPCNsigmaEl1D_LHC24f3c_NoMCTunedOnData->Scale(1./hTPCNsigmaEl1D_LHC24f3c_NoMCTunedOnData->Integral());
  hTPCNsigmaEl1D_LHC24f3c_NoMCTunedOnData->SetName("TPCNsigmaEl1D_LHC24f3c_NoMCTunedOnData");

  TH1D* hTPCNsigmaEl1D_LHC22o = (TH1D*)hTPCNsigmaEl_LHC22o->ProjectionY();
  SetHistogramPropertiesAlt(hTPCNsigmaEl1D_LHC22o, "", "", 14);
  hTPCNsigmaEl1D_LHC22o->Scale(1./hTPCNsigmaEl1D_LHC22o->Integral());
  hTPCNsigmaEl1D_LHC22o->SetName("hTPCNsigmaEl1D_LHC22o");


  TLegend* legendTPCNsigmaEl = new TLegend(0.528249,0.672012,0.805085,0.87172);
  legendTPCNsigmaEl->AddEntry((TObject*)0x0,"pp collisions", "");
  legendTPCNsigmaEl->AddEntry((TObject*)0x0,"PCMPCM", "");
  legendTPCNsigmaEl->AddEntry((TObject*)0x0,"#gamma  #rightarrow  #it{e}^{+}#it{e}^{-}", "");
  legendTPCNsigmaEl->AddEntry(hTPCNsigmaEl1D_LHC24f3c, "LHC24f3c", "lp");
  legendTPCNsigmaEl->AddEntry(hTPCNsigmaEl1D_LHC24f3c_NoMCTunedOnData, "LHC24f3c_NoMCTunedOnData", "lp");
  legendTPCNsigmaEl->AddEntry(hTPCNsigmaEl1D_LHC22o, "LHC22o_Pass7", "lp");


  TObjArray* arrTPCNsigmaEl = new TObjArray();
  arrTPCNsigmaEl->Add(hTPCNsigmaEl1D_LHC24f3c);
  arrTPCNsigmaEl->Add(hTPCNsigmaEl1D_LHC24f3c_NoMCTunedOnData);
  arrTPCNsigmaEl->Add(hTPCNsigmaEl1D_LHC22o);
  arrTPCNsigmaEl->Add(legendTPCNsigmaEl);


   TCanvas* canTPCNsigmaEl = (TCanvas*)makeCanvas(arrTPCNsigmaEl, 0, "CMYK|NoTime|LogaX LoagY", 0, 0);

  ///CosPA_Rxy

   TH2D* hCosPAG_Rxy_LHC24f3c = (TH2D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hCosPA_Rxy", runnNumber_LHC24f3c));
   hCosPAG_Rxy_LHC24f3c->SetStats(0);
   hCosPAG_Rxy_LHC24f3c->GetYaxis()->SetRangeUser(0.994, 1.0);

   TH2D* hCosPAG_Rxy_LHC24f3c_NoMCTunedOnData = (TH2D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hCosPA_Rxy", runnNumber_LHC24f3c_NoMCTunedOnData));
   hCosPAG_Rxy_LHC24f3c_NoMCTunedOnData->SetStats(0);
   hCosPAG_Rxy_LHC24f3c_NoMCTunedOnData->GetYaxis()->SetRangeUser(0.994, 1.0);

   TCanvas* cCosPAG_Rxy = doDoubleCanvasTH2(hCosPAG_Rxy_LHC24f3c, hCosPAG_Rxy_LHC24f3c_NoMCTunedOnData);


  
  // ///Ngammas per Collision
  // TH1D* hNGamma_LHC24f3c =(TH1D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hNgamma", runnNumber_LHC24f3c));
  // hNGamma_LHC24f3c->GetXaxis()->SetRangeUser(0, 10);
  // hNGamma_LHC24f3c->Sumw2();
  // hNGamma_LHC24f3c->SetName("hNGamma_LHC24f3c");
  // TH1D* hNGamma_LHC24f3c_NoMCTunedOnData = (TH1D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0/primary/hNgamma", runnNumber_LHC24f3c_NoMCTunedOnData));
  // hNGamma_LHC24f3c_NoMCTunedOnData->GetXaxis()->SetRangeUser(0, 10);
  // hNGamma_LHC24f3c_NoMCTunedOnData->Sumw2();
  // hNGamma_LHC24f3c_NoMCTunedOnData->SetName("hNGamma_LHC24f3c_NoMCTunedOnData");

  // SetHistogramPropertiesAlt(hNGamma_LHC24f3c, "#it{N}_{#gamma}", "counts", 0);
  // hNGamma_LHC24f3c->SetMarkerStyle(4);
  
  // SetHistogramPropertiesAlt(hNGamma_LHC24f3c_NoMCTunedOnData, "#it{N}_{#gamma}", "counts", 1);
  // hNGamma_LHC24f3c_NoMCTunedOnData->SetMarkerStyle(26);

  
  // TH1D* hNGamma_Ratio = (TH1D*)hNGamma_LHC24f3c->Clone();
  // hNGamma_Ratio->Divide(hNGamma_LHC24f3c_NoMCTunedOnData);
  // hNGamma_Ratio->SetMarkerStyle(8);

  // TLegend* legendNGamma = new TLegend(0.528249,0.672012,0.805085,0.87172);
  // legendNGamma->AddEntry((TObject*)0x0,"pp collisions", "");
  // legendNGamma->AddEntry((TObject*)0x0,"PCMPCM", "");
  // legendNGamma->AddEntry((TObject*)0x0,"#gamma  #rightarrow  #it{e}^{+}#it{e}^{-}", "");
  // legendNGamma->AddEntry(hNGamma_LHC24f3c, "LHC24f3c", "lp");
  // legendNGamma->AddEntry(hNGamma_LHC24f3c_NoMCTunedOnData, "LHC24f3c_NoMCTunedOnData", "lp");

  // TObjArray* arrNGamma = new TObjArray();
  // //arrNGamma->Add(hNGamma_LHC24f3c);
  // arrNGamma->Add(hNGamma_LHC24f3c_NoMCTunedOnData);
  // arrNGamma->Add(legendNGamma);


  // TObjArray* arrNGammaRatios = new TObjArray();
  // arrNGammaRatios->Add(hNGamma_Ratio);

  // TCanvas* cNGamma = (TCanvas*)makeCanvas(arrNGamma, arrNGammaRatios, "CMYK|NoTime|LogX LogY", 0, 0);

  // // ///NclsTPC

  // TH1D* hNclsTPC_LHC24f3c = (TH1D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0Leg/primary/hNclsTPC", runnNumber_LHC24f3c));
  // hNclsTPC_LHC24f3c->Sumw2();
  // hNclsTPC_LHC24f3c->SetName("hNclsTPC_LHC24f3c");
  // TH1D* hNclsTPC_LHC24f3c_NoMCTunedOnData = (TH1D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0Leg/primary/hNclsTPC", runnNumber_LHC24f3c_NoMCTunedOnData));
  // hNclsTPC_LHC24f3c_NoMCTunedOnData->Sumw2();
  // hNclsTPC_LHC24f3c_NoMCTunedOnData->SetName("hNclsTPC_LHC24f3c_NoMCTunedOnData");
  // SetHistogramPropertiesAlt(hNclsTPC_LHC24f3c, "#it{N}_{clsTPC}", "counts", 13);
  // hNclsTPC_LHC24f3c->SetMarkerStyle(8);
  // SetHistogramPropertiesAlt(hNclsTPC_LHC24f3c_NoMCTunedOnData, "#it{N}_{clsTPC}", "counts", 14);
  // hNclsTPC_LHC24f3c_NoMCTunedOnData->SetMarkerStyle(8);

  // TH1D* hNclsTPC_Ratio = (TH1D*)hNclsTPC_LHC24f3c->Clone();
  // hNclsTPC_Ratio->Divide(hNclsTPC_LHC24f3c_NoMCTunedOnData);


  // TLegend* legendNclsTPC = new TLegend(0.528249,0.672012,0.805085,0.87172);
  // legendNclsTPC->AddEntry((TObject*)0x0,"pp collisions", "");
  // legendNclsTPC->AddEntry((TObject*)0x0,"PCMPCM", "");
  // legendNclsTPC->AddEntry((TObject*)0x0,"#gamma  #rightarrow  #it{e}^{+}#it{e}^{-}", "");
  // legendNclsTPC->AddEntry(hNclsTPC_LHC24f3c, "LHC24f3c", "lp");
  // legendNclsTPC->AddEntry(hNclsTPC_LHC24f3c_NoMCTunedOnData, "LHC24f3c_NoMCTunedOnData", "lp");

  // TObjArray* arrNclsTPC = new TObjArray();
  // arrNclsTPC->Add(hNclsTPC_LHC24f3c);
  // arrNclsTPC->Add(hNclsTPC_LHC24f3c_NoMCTunedOnData);
  // arrNclsTPC->Add(legendNclsTPC);


  // TObjArray* arrNclsTPCRatios = new TObjArray();
  // arrNclsTPCRatios->Add(hNclsTPC_Ratio);

  // TCanvas* cNclsTPC = (TCanvas*)makeCanvas(arrNclsTPC, arrNclsTPCRatios, "CMYK|NoTime|LogX LogY", 0, 0);

  // cout << "debuging... line: " << __LINE__ << endl;
  // ///hNcrTPC

  // TH1D* hNcrTPC_LHC24f3c = (TH1D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0Leg/primary/hNcrTPC", runnNumber_LHC24f3c));
  // hNcrTPC_LHC24f3c->SetName("hNcrTPC_LHC24f3c");
  // hNcrTPC_LHC24f3c->Sumw2();
  // TH1D* hNcrTPC_LHC24f3c_NoMCTunedOnData = (TH1D*)extractHistogram(Form("inputFiles/LHC22oPass7/AnalysisResults_MC_%d.root:/pcm-qc-mc/V0Leg/primary/hNcrTPC", runnNumber_LHC24f3c_NoMCTunedOnData));
  // hNcrTPC_LHC24f3c_NoMCTunedOnData->SetName("hNcrTPC_LHC24f3c_NoMCTunedOnData");
  // hNcrTPC_LHC24f3c_NoMCTunedOnData->Sumw2();

  // SetHistogramPropertiesAlt(hNcrTPC_LHC24f3c, "#it{N}_{crTPC}", "counts", 13);
  // hNcrTPC_LHC24f3c->SetMarkerStyle(8);
  // SetHistogramPropertiesAlt(hNcrTPC_LHC24f3c_NoMCTunedOnData, "#it{N}_{crTPC}", "counts", 14);
  // hNcrTPC_LHC24f3c_NoMCTunedOnData->SetMarkerStyle(4);

  // TH1D* hNcrTPC_Ratio = (TH1D*)hNcrTPC_LHC24f3c->Clone("hNcrTPC_Ratio");
  // hNcrTPC_Ratio->Divide(hNcrTPC_LHC24f3c_NoMCTunedOnData);


  // TLegend* legendNcrTPC = new TLegend(0.528249,0.672012,0.805085,0.87172);
  // legendNcrTPC->AddEntry((TObject*)0x0,"pp collisions", "");
  // legendNcrTPC->AddEntry((TObject*)0x0,"PCMPCM", "");
  // legendNcrTPC->AddEntry((TObject*)0x0,"#gamma  #rightarrow  #it{e}^{+}#it{e}^{-}", "");
  // legendNcrTPC->AddEntry(hNcrTPC_LHC24f3c, "LHC24f3c", "lp");
  // legendNcrTPC->AddEntry(hNcrTPC_LHC24f3c_NoMCTunedOnData, "LHC24f3c_NoMCTunedOnData", "lp");

  // TObjArray* arrNcrTPC = new TObjArray();
  // arrNcrTPC->Add(hNcrTPC_LHC24f3c);
  // arrNcrTPC->Add(hNcrTPC_LHC24f3c_NoMCTunedOnData);
  // arrNcrTPC->Add(legendNcrTPC); 

  // TObjArray* arrNcrTPCRatio = new TObjArray();
  // arrNcrTPCRatio->Add(hNcrTPC_Ratio);

  // TCanvas* cNcrTPC = (TCanvas*)makeCanvas(arrNcrTPC, arrNcrTPCRatio, "CMYK|NoTime|LogX LogY", 0, 0);


  // ///hNclsITS

  // TH1D* hNclsITS_LHC24f3c = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/V0Leg/primary/hNclsITS", trainOutputMC_LHC24f3c));
  // TH1D* hNclsITS_LHC24f3c_NoMCTunedOnData = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/V0Leg/primary/hNclsITS", trainOutputMC_LHC24f3c_NoMCTunedOnData));

  // SetHistogramPropertiesAlt(hNclsITS_LHC24f3c, "#it{N}_{clsITS}", "counts", 13);
  // hNclsITS_LHC24f3c->SetMarkerStyle(8);
  // SetHistogramPropertiesAlt(hNclsITS_LHC24f3c_NoMCTunedOnData, "#it{N}_{clsITS}", "counts", 13);
  // hNclsITS_LHC24f3c_NoMCTunedOnData->SetMarkerStyle(4);

  // TLegend* legendNclsITS = new TLegend(0.528249,0.672012,0.805085,0.87172);
  // legendNclsITS->AddEntry((TObject*)0x0,"pp collisions", "");
  // legendNclsITS->AddEntry((TObject*)0x0,"PCMPCM", "");
  // legendNclsITS->AddEntry((TObject*)0x0,"#gamma  #rightarrow  #it{e}^{+}#it{e}^{-}", ""); 
  // legendNclsITS->AddEntry(hNclsITS_LHC24f3c, "LHC24f3c", "lp");
  // legendNclsITS->AddEntry(hNclsITS_LHC24f3c_NoMCTunedOnData, "LHC24f3c_NoMCTunedOnData", "lp");

  // TObjArray* arrNclsITS = new TObjArray();
  // arrNclsITS->Add(hNclsITS_LHC24f3c);
  // arrNclsITS->Add(hNclsITS_LHC24f3c_NoMCTunedOnData);
  // arrNclsITS->Add(legendNclsITS);

  // TCanvas* cNclsITS = (TCanvas*)makeCanvas(arrNclsITS, 0, "CMYK|NoTime|LogX LogY", 0, 0);

  ///Single photon efficiencies

  // TH1D* effSingleGamma_LHC24f3c = getSinglePhotonEff(trainOutputMC_LHC24f3c);
  // SetHistogramPropertiesAlt(effSingleGamma_LHC24f3c, "#it{p}_{T} GeV/#it{c}", "gamma rec. eff.", 13);
  // TH1D* effSingleGamma_LHC24f3c_NoMCTunedOnData = getSinglePhotonEff(trainOutputMC_LHC24f3c_NoMCTunedOnData);
  // SetHistogramPropertiesAlt(effSingleGamma_LHC24f3c_NoMCTunedOnData, "#it{p}_{T} GeV/#it{c}", "gamma rec. eff.", 13);
  // effSingleGamma_LHC24f3c_NoMCTunedOnData->SetMarkerStyle(26);

  // TH1D* effSingleGammaRatio = (TH1D*)effSingleGamma_LHC24f3c->Clone("effSingleGammaRatio");
  // effSingleGammaRatio->Divide(effSingleGamma_LHC24f3c_NoMCTunedOnData);
  // effSingleGammaRatio->GetYaxis()->SetTitle("f3c/f3b2");
  // effSingleGammaRatio->GetYaxis()->SetRangeUser(0.9, 1.1);
  
  // TH1D* gammaRecEfficiencyPtRun2 = (TH1D*)extractHistogram("/Users/yelmard/Downloads/SinglePhotonEff.root:/Photon_AccTimesEff");
  // SetHistogramProperties(gammaRecEfficiencyPtRun2, "gamma rec. eff.", 8);
  // gammaRecEfficiencyPtRun2->SetMarkerStyle(4);
  // gammaRecEfficiencyPtRun2->GetYaxis()->SetRangeUser(0.001, 0.2);


  // TLegend* legendSingleGamma = new TLegend(0.528249,0.672012,0.805085,0.87172);
  // legendSingleGamma->AddEntry((TObject*)0x0,"pp collisions", "");
  // legendSingleGamma->AddEntry((TObject*)0x0,"PCMPCM", "");
  // legendSingleGamma->AddEntry(effSingleGamma_LHC24f3c, "Run 3, LHC24f3c", "lp");
  // legendSingleGamma->AddEntry(effSingleGamma_LHC24f3c_NoMCTunedOnData, "Run 3, LHC24f3c_NoMCTunedOnData", "lp");
  // legendSingleGamma->AddEntry(gammaRecEfficiencyPtRun2, "Run 2", "lp");


  // TObjArray* arrSingleGamma = new TObjArray();
  // arrSingleGamma->Add(effSingleGamma_LHC24f3c);
  // arrSingleGamma->Add(effSingleGamma_LHC24f3c_NoMCTunedOnData);
  // arrSingleGamma->Add(gammaRecEfficiencyPtRun2);
  // arrSingleGamma->Add(legendSingleGamma);

  // TObjArray* arrSingleGammaRatio = new TObjArray();
  // arrSingleGammaRatio->Add(effSingleGammaRatio);

  // TCanvas* canSingleGamma = (TCanvas*)makeCanvas(arrSingleGamma, arrSingleGammaRatio, "CMYK|NoTime|LogX", 0, 0);

  ///Photon conversion probability
  // TH1D* convProbability_LHC24f3c = getPhotonConvProbability(trainOutputMC_LHC24f3c);
  // SetHistogramPropertiesAlt(convProbability_LHC24f3c, "#it{p}_{T} GeV/#it{c}", "conv. prob.", 13);
  // TH1D* convProbability_LHC24f3c_NoMCTunedOnData = getPhotonConvProbability(trainOutputMC_LHC24f3c_NoMCTunedOnData);
  // SetHistogramPropertiesAlt(convProbability_LHC24f3c_NoMCTunedOnData, "#it{p}_{T} GeV/#it{c}", "conv. prob.", 13);
  // convProbability_LHC24f3c_NoMCTunedOnData->SetMarkerStyle(26);


  // TH1D* convProbabilityRatio = (TH1D*)convProbability_LHC24f3c->Clone("convProbabilityRatio");
  // convProbabilityRatio->Divide(convProbability_LHC24f3c_NoMCTunedOnData);
  // convProbabilityRatio->GetYaxis()->SetTitle("f3c/f3b2");
  // convProbabilityRatio->GetYaxis()->SetRangeUser(0.9, 1.1);
  
  // TLegend* legendConvProbability = new TLegend(0.528249,0.672012,0.805085,0.87172);
  // legendConvProbability->AddEntry((TObject*)0x0,"pp collisions", "");
  // legendConvProbability->AddEntry((TObject*)0x0,"PCMPCM", "");
  // legendConvProbability->AddEntry(convProbability_LHC24f3c, "Run 3, LHC24f3c", "lp");
  // legendConvProbability->AddEntry(convProbability_LHC24f3c_NoMCTunedOnData, "Run 3, LHC24f3c_NoMCTunedOnData", "lp");


  // TObjArray* arrConvProbability = new TObjArray();
  // arrConvProbability->Add(convProbability_LHC24f3c);
  // arrConvProbability->Add(convProbability_LHC24f3c_NoMCTunedOnData);
  // arrConvProbability->Add(legendConvProbability);

  // TObjArray* arrConvProbabilityRatio = new TObjArray();
  // arrConvProbabilityRatio->Add(convProbabilityRatio);

  // TCanvas* canConvProbability = (TCanvas*)makeCanvas(arrConvProbability, arrConvProbabilityRatio, "CMYK|NoTime|LogX", 0, 0);

}


TH1D* getSinglePhotonEff(const char* trainOutputMC){
  Double_t fBinsPt[] = {0.40, 0.60, 0.80, 0.90, 1.00, 1.20, 1.40, 1.60, 1.80, 2.0, 2.50, 3.00, 3.50, 4.00,  4.50, 5.00, 5.5, 6.00, 7.00, 8.00, 10.00};
  Int_t nBinsPt = sizeof(*fBinsPt);
  
  TH1D* convPrimGammasPtUnrebinned =  (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/Generated/hPt_ConvertedPhoton", trainOutputMC));
  if(!convPrimGammasPtUnrebinned) std::cout<<"WARNING: convPrimGammasPtUnrebinned is null pointer\n";

  TH1D* recGammasUnrebinned = (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hPt", trainOutputMC));
  if(!recGammasUnrebinned) std::cout<<"WARNING: recGammasUnrebinned is null pointer\n"; 


  TH1D *convPrimGammasPt = RebinHistogram(convPrimGammasPtUnrebinned,nBinsPt, fBinsPt);
  TH1D *recGammas = RebinHistogram(recGammasUnrebinned,nBinsPt, fBinsPt);

  TH1D* gammaRecEfficiencyPt = (TH1D*)recGammas->Clone("gammaRecEfficiencyPt");
  gammaRecEfficiencyPt->Divide(recGammas, convPrimGammasPt, 1., 1., "b");
  return gammaRecEfficiencyPt;
}

TH1D* getPhotonConvProbability(const char* trainOutputMC){
  Double_t fBinsPt[] = {0.40, 0.60, 0.80, 0.90, 1.00, 1.20, 1.40, 1.60, 1.80, 2.0, 2.50, 3.00, 3.50, 4.00,  4.50, 5.00, 5.5, 6.00, 7.00, 8.00, 10.00};
  Int_t nBinsPt = sizeof(*fBinsPt);

  TH1D* genPrimGammasPtUnrebinned =  (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/Generated/hPt", trainOutputMC));
  if(!genPrimGammasPtUnrebinned) std::cout<<"WARNING: genPrimGammasPtUnrebinned is null pointer\n";

  TH1D* convPrimGammasPtUnrebinned =  (TH1D*)extractHistogram(Form("%s:/pcm-qc-mc/Generated/hPt_ConvertedPhoton", trainOutputMC));
  if(!convPrimGammasPtUnrebinned) std::cout<<"WARNING: convPrimGammasPtUnrebinned is null pointer\n";

  TH1D *genPrimGammasPt = RebinHistogram(genPrimGammasPtUnrebinned,nBinsPt  , fBinsPt);
  TH1D *convPrimGammasPt = RebinHistogram(convPrimGammasPtUnrebinned,nBinsPt, fBinsPt);


  TH1D* gammaConvProbabilityPt = (TH1D*)convPrimGammasPt->Clone("gammaConvProbabilityPt");
  gammaConvProbabilityPt->Divide(convPrimGammasPt, genPrimGammasPt, 1., 1., "b");
  
  return gammaConvProbabilityPt;
}


TH2D* getTH2Dhisto(const char* trainOutputMC, const char* hname){

  TH2D* h2D = (TH2D*)extractHistogram(Form("%s:/pcm-qc-mc/%s", trainOutputMC, hname));
  if(!h2D) std::cout<<"WARNING: " << hname << " is null pointer\n";
  h2D->SetTitle("");
  h2D->SetStats(0);
  return h2D;

}



TCanvas* doDoubleCanvasTH2(TH2D* h1, TH2D* h2){

  TRandom *random = new TRandom(0);

  TCanvas *c1 = new TCanvas(Form("%f",random->Rndm()), "", 1100, 800);
  c1->Divide(1, 2);
  cout << "debuggin line: " << __LINE__ << endl;
  c1->cd(1);
  gPad->SetLogz();
  h1->Draw("colz");
  c1->cd(2);
  gPad->SetLogz();
  h2->Draw("colz");

  return c1;

}




TH1D* GetTrueMC_PtReso(const char* trainOutputMC, Int_t color, Int_t markerStyle){
  TH2D* hPtResolution = (TH2D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hPtResolution", trainOutputMC));
  TH1D* hPtResolution1D = (TH1D*)hPtResolution->ProfileX();

  SetHistogramPropertiesAlt(hPtResolution1D, "#it{p}_{T} (GeV/#it{c})", "#delta#it{p}_{T}/#it{p}_{T}", color);
  hPtResolution1D->SetMarkerStyle(markerStyle);

  hPtResolution1D->GetYaxis()->SetRangeUser(0.001, 0.1);
  hPtResolution1D->GetXaxis()->SetRangeUser(0.1, 10.);

  return hPtResolution1D;
}
