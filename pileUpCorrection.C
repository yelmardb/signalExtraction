#include "includes/Plotting.h"
#include "includes/extractHistogram.h"
#include "TF1.h"
#include "TH1.h"
#include <stdio.h>
#include "includes/inputData.h"





void pileUpCorrection(){
  //*******************************************************************************************************///
    ///Load 


    const char* inFileName_DATA = strdup(Form("../Run3Pi0/data/AnalysisResults.root"));
    const char* inFileName_TrueMC = strdup(Form("../Run3Pi0/mc/AnalysisResults.root"));


    ///DCAz distributions

    TH2D* hDCAxyz_V0_Data =  (TH2D*)extractHistogram(Form("%s:/pcm-qc/V0/hDCAz_Pt",inFileName_DATA));
    if(!hDCAxyz_V0_Data) cout << "WARNING: hDCAz_Pt in data"<< endl;

    TH2D* hDCAxyz_V0_MC =  (TH2D*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hDCAz_Pt",inFileName_TrueMC));
    if(!hDCAxyz_V0_MC) cout << "WARNING: hDCAz_Pt in MC"<< endl;


    for(int i = 3; i < 7; i++){

        hDCAxyz_V0_Data->GetYaxis()->SetRangeUser(fBinsPi0PtMC[i], fBinsPi0PtMC[i+1]);
        TH1D* hDCAz_V0_Data = (TH1D*)hDCAxyz_V0_Data->ProjectionY(Form("hDCAxyz_V0_MC_pT_%.2f_%.2f", fBinsPi0PtMC[i], fBinsPi0PtMC[i+1]));
        TH1D* hDCAz_V0_Data_NoPileUp = (TH1D*)hDCAxyz_V0_Data->ProjectionY(Form("hDCAxyz_V0_Data_pT_%.2f_%.2f", fBinsPi0PtMC[i], fBinsPi0PtMC[i+1]));

        hDCAz_V0_Data->Rebin(2);
        hDCAz_V0_Data_NoPileUp->Rebin(2);

    
        hDCAxyz_V0_MC->GetYaxis()->SetRangeUser(0.4, 3.0);
        TH1D* hDCAz_V0_MC = (TH1D*)hDCAxyz_V0_MC->ProjectionX(Form("hDCAxyz_V0_MC_pT_%.2f_%.2f", fBinsPi0PtMC[i], fBinsPi0PtMC[i+1]));
        hDCAz_V0_MC->Rebin(2);


        Int_t smoothingIterations = 20; // Number of smoothing iterations

        TH1D* hEstimatedPileUp =  (TH1D*)hDCAz_V0_Data->ShowBackground(smoothingIterations, "same");
        hDCAz_V0_Data_NoPileUp->Add(hEstimatedPileUp, -1);
       

        hDCAz_V0_Data->Scale(1/hDCAz_V0_Data->Integral());
        hDCAz_V0_MC->Scale(1/hDCAz_V0_MC->Integral());
        hDCAz_V0_Data_NoPileUp->Scale(1/hDCAz_V0_Data_NoPileUp->Integral());
        hEstimatedPileUp->Scale(1/hEstimatedPileUp->Integral());

        SetHistogramPropertiesAlt(hDCAz_V0_Data, "#it{DCA}_{z} (cm)", "norm. counts", 0);
        SetHistogramPropertiesAlt(hDCAz_V0_MC, "#it{DCA}_{z} (cm)", "norm. counts", 1);
        SetHistogramPropertiesAlt(hDCAz_V0_Data_NoPileUp, "#it{DCA}_{z} (cm)", "norm. counts", 2);
        SetHistogramPropertiesAlt(hEstimatedPileUp, "#it{DCA}_{z} (cm)", "norm. counts", 3);

        TLegend* leg = new TLegend(0.7, 0.7, 0.9, 0.9);
        leg->AddEntry(hDCAz_V0_Data, "Data, cat. 1", "lp");
        leg->AddEntry(hDCAz_V0_MC, "MC, cat. 1", "lp");
        leg->AddEntry(hEstimatedPileUp, "Estimated pile-up", "lp");
        leg->AddEntry(hDCAz_V0_Data_NoPileUp, "Data w/ pile-up subtracted", "lp");

        TObjArray* arr = new TObjArray();
        //arr->Add(hDCAz_V0_Data);
        arr->Add(hDCAz_V0_MC);
        //arr->Add(hEstimatedPileUp);
        //arr->Add(hDCAz_V0_Data_NoPileUp);
        arr->Add(leg);

        TCanvas* canDCAz = (TCanvas*)makeCanvas(arr, 0, "CMYK|NoTime ", 0, 0);
        
    }

    ///DCAz distributions
    THnD* hDCAz_V0_MCInfo = (THnD*)extractHistogram(Form("%s:/pcm-qc-mc/V0/primary/hDCAz_Pt_True",inFileName_TrueMC));
    if(!hDCAz_V0_MCInfo) cout << "WARNING: hDCAxyz_Pt_True in MC"<< endl;

    hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(-0.5, 0.5);
    TH1D* hDCAz_ValidatedCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(0);
    SetHistogramPropertiesAlt(hDCAz_ValidatedCollisions, "#it{DCA}_{z} (cm)", "norm. counts", 0);
    hDCAz_ValidatedCollisions->Rebin(2);
    hDCAz_ValidatedCollisions->Scale(1/hDCAz_ValidatedCollisions->Integral());
    hDCAz_ValidatedCollisions->GetXaxis()->SetRangeUser(-5.0, 5.0);

    hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(0.5, 1.5);
    TH1D* hDCAz_InvalidatedCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(0);
    SetHistogramPropertiesAlt(hDCAz_InvalidatedCollisions, "#it{DCA}_{z} (cm)", "norm. counts", 1);
    hDCAz_InvalidatedCollisions->Rebin(2);
    hDCAz_InvalidatedCollisions->Scale(1/hDCAz_InvalidatedCollisions->Integral());
    hDCAz_InvalidatedCollisions->GetXaxis()->SetRangeUser(-5.0, 5.0);

    hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(-0.5, 1.5);
    TH1D* hDCAz_AllCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(0);
    SetHistogramPropertiesAlt(hDCAz_AllCollisions, "#it{DCA}_{z} (cm)", "norm. counts", 2);
    hDCAz_AllCollisions->Rebin(2);
    hDCAz_AllCollisions->Scale(1/hDCAz_AllCollisions->Integral());




    TLegend* legend2 = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend2->AddEntry(hDCAz_ValidatedCollisions, "Validated collisions", "lp");
    legend2->AddEntry(hDCAz_InvalidatedCollisions, "Invalidated collisions", "lp");
    legend2->AddEntry(hDCAz_AllCollisions, "All collisions", "lp");

    TObjArray* arr2 = new TObjArray();
    arr2->Add(hDCAz_ValidatedCollisions);
    arr2->Add(hDCAz_InvalidatedCollisions);
    arr2->Add(hDCAz_AllCollisions);
    arr2->Add(legend2); 

    TCanvas* canDCAz2 = (TCanvas*)makeCanvas(arr2, 0, "CMYK|NoTime LogY ", 0, 0);


    TH2D* hCollisionCounter = (TH2D*)extractHistogram(Form("%s:/pcm-qc-mc/Event/after/hCollisionCounter",inFileName_TrueMC));
    Double_t nEvents = hCollisionCounter->GetBinContent(10);

  ///pT distributions
  hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(-0.5, 0.5);
  TH1D* hpT_ValidatedCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(1);
  SetHistogramPropertiesAlt(hpT_ValidatedCollisions, "p_{T} (GeV/c)", "norm. counts", 0);
  hpT_ValidatedCollisions->GetXaxis()->SetRangeUser(0.1, 10.0);
  hpT_ValidatedCollisions->Rebin(4);
  hpT_ValidatedCollisions->Scale(1./nEvents);


  hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(0.5, 1.5);
  TH1D* hpT_InvalidatedCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(1);
  SetHistogramPropertiesAlt(hpT_InvalidatedCollisions, "p_{T} (GeV/c)", "norm. counts", 1);
  hpT_InvalidatedCollisions->GetXaxis()->SetRangeUser(0.1, 10.0);
  hpT_InvalidatedCollisions->Rebin(4);
  hpT_InvalidatedCollisions->Scale(1./nEvents);
  

  hDCAz_V0_MCInfo->GetAxis(2)->SetRangeUser(-0.5, 1.5); 
  TH1D* hpT_AllCollisions = (TH1D*)hDCAz_V0_MCInfo->Projection(1);
  SetHistogramPropertiesAlt(hpT_AllCollisions, "p_{T} (GeV/c)", "norm. counts", 2);
  hpT_AllCollisions->GetXaxis()->SetRangeUser(0.1, 10.0);
  hpT_AllCollisions->GetYaxis()->SetRangeUser(1e-3, 1.5*hpT_AllCollisions->GetMaximum());
  hpT_AllCollisions->Rebin(4);
  hpT_AllCollisions->Scale(1./nEvents);

  TH1D* hpT_ValidatedCollisionsRatioTo_AllCollisions = (TH1D*)hpT_ValidatedCollisions->Clone("hpT_ValidatedCollisionsRatioToAllCollisions");
  hpT_ValidatedCollisionsRatioTo_AllCollisions->Divide(hpT_AllCollisions);

  TH1D* hpT_InvalidatedCollisionsRatioTo_AllCollisions = (TH1D*)hpT_InvalidatedCollisions->Clone("hpT_InvalidatedCollisionsRatioToAllCollisions");
  hpT_InvalidatedCollisionsRatioTo_AllCollisions->Divide(hpT_AllCollisions);


  TLegend* legend3 = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend3->AddEntry(hpT_ValidatedCollisions, "Validated collisions", "lp");
  legend3->AddEntry(hpT_InvalidatedCollisions, "Invalidated collisions", "lp");
  legend3->AddEntry(hpT_AllCollisions, "All collisions", "lp");

  TObjArray* arr3 = new TObjArray();
  arr3->Add(hpT_AllCollisions);
  arr3->Add(hpT_ValidatedCollisions);
  arr3->Add(hpT_InvalidatedCollisions);
  arr3->Add(legend3); 

  TObjArray* arr3Ratio = new TObjArray();
  arr3Ratio->Add(hpT_ValidatedCollisionsRatioTo_AllCollisions);
  arr3Ratio->Add(hpT_InvalidatedCollisionsRatioTo_AllCollisions);

  TCanvas* canpT2 = (TCanvas*)makeCanvas(arr3, arr3Ratio, "CMYK|NoTime LogX LogY ", 0, 0);


  TH1D* correctionFactor = (TH1D*)hpT_ValidatedCollisionsRatioTo_AllCollisions->Clone("correctionFactor");
  correctionFactor->Reset();

  for(int i = 1; i <= hpT_ValidatedCollisionsRatioTo_AllCollisions->GetNbinsX(); i++){
    if(hpT_ValidatedCollisionsRatioTo_AllCollisions->GetBinContent(i) == 0){
      correctionFactor->SetBinContent(i, 1.0);
      correctionFactor->SetBinError(i, 0.0);
    }

    else{
      correctionFactor->SetBinContent(i, 1./hpT_ValidatedCollisionsRatioTo_AllCollisions->GetBinContent(i));
      correctionFactor->SetBinError(i, 1./hpT_ValidatedCollisionsRatioTo_AllCollisions->GetBinError(i));
    } 
    cout << "correctionFactor: in" << i << " is  " << correctionFactor->GetBinContent(i) << endl;
    
  }

  TObjArray* arr4 =  new TObjArray();
  arr4->Add(correctionFactor);
  TCanvas* canpT3 = (TCanvas*)makeCanvas(arr4, 0, "CMYK|NoTime LogX  ", 0, 0);


}

