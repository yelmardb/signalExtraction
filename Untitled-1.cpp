
    TFile* inFileSignalExtractionEta = TFile::Open("outputSignalExtraction/SignalExtraction_Eta_DATA_220661.root", "UPDATE");

    TH1D* hpTRecEta = (TH1D*)extractHistogram("outputSignalExtraction/SignalExtraction_Eta_MC_223972.root:/h1yield");
    TH1D* hpTAccEta = (TH1D*)extractHistogram("inputFiles/AnalysisResults_MC_214158.root:/Etaeta-to-gammagamma-mc/Generated/PCMPCM/hPt_Eta_Acc");
    TH1D* hpTGenEta = (TH1D*)extractHistogram("inputFiles/AnalysisResults_MC_214158.root:/Etaeta-to-gammagamma-mc/Generated/PCMPCM/hPt_Eta");
    TH1D* hCollisionCounter = (TH1D*)extractHistogram("inputFiles/AnalysisResults_MC_214158.root:/Etaeta-to-gammagamma-mc/Generated/PCMPCM/hCollisionCounter"); 
    Double_t nEvents = hCollisionCounter->GetBinContent(hCollisionCounter->FindBin(3.));

    hpTAccEta->Scale(1/nEvents); 
    hpTAccEta->Scale(1., "width");   
    hpTGenEta->Scale(1/nEvents); 
    hpTGenEta->Scale(1., "width");

 
    TH1D* hpTAccEtaRebinned = rebinHisto(hpTRecEta, hpTAccEta);
    TH1D* hpTGenEtaRebinned = rebinHisto(hpTRecEta, hpTGenEta);


    // Rebin histograms
    TH1D* mesonEffEta = (TH1D*)hpTRecEta->Clone("mesonEffEta");
    mesonEffEta = MakeRatioSpectra(hpTRecEta, hpTAccEta, "quiet");
    mesonEffEta->SetName("mesonEffEta");

    TH1D* mesonAccEta = (TH1D*)hpTAccEta->Clone("mesonAccEta");
    mesonAccEta = MakeRatioSpectra(hpTAccEtaRebinned, hpTGenEtaRebinned , "quiet");
    mesonAccEta->SetName("mesonAccEta");

    TH1D* mesonAccXEffEta = (TH1D*)hpTRecEta->Clone("mesonAccXEffEta");
    mesonAccXEffEta = MakeRatioSpectra(hpTRecEta, hpTGenEta, "quiet");
    mesonAccXEffEta->SetName("mesonAccXEffEta");
    mesonAccXEffEta->Scale(0.988131);

    SetHistogramPropertiesAlt(mesonEffEta, "#it{p}_{T} GeV/#it{c}", "efficiency", 1);
    SetHistogramPropertiesAlt(mesonAccEta, "#it{p}_{T} GeV/#it{c}", "acceptance", 1);
    SetHistogramPropertiesAlt(mesonAccXEffEta, "#it{p}_{T} GeV/#it{c}", "Acc x Eff x BR", 1);

    TH1D* h1yieldEta = (TH1D*)extractHistogram("outputSignalExtraction/SignalExtraction_Eta_DATA_220661.root:/h1yield");
    TH1D* h1yieldEtaRebinned = rebinHisto(mesonAccXEffEta, h1yieldEta);
    TH1D* h1yieldCorrEta = (TH1D*)h1yieldEtaRebinned->Clone("h1yieldCorrEta");
    h1yieldCorrEta->Reset();
    h1yieldCorrEta->SetName("h1yieldCorr");

    for(int i = 1; i < h1yieldCorrEta->GetNbinsX(); i++){
        Double_t pT = h1yieldCorrEta->GetBinCenter(i);
        h1yieldCorrEta->SetBinContent(i, h1yieldEtaRebinned->GetBinContent(i)/mesonAccXEffEta->GetBinContent(mesonAccXEffEta->FindBin(pT+0.01)));
        h1yieldCorrEta->SetBinError(i, h1yieldEtaRebinned->GetBinError(i)/mesonAccXEffEta->GetBinError(mesonAccXEffEta->FindBin(pT+0.01)));
    }
    h1yieldCorrEta->Scale(1., "width");
    SetHistogramPropertiesAlt(h1yieldCorrEta, "#it{p}_{T} GeV/#it{c}", "#frac{1}{#it{N}_{ev}} #frac{d#it{N}}{d#it{p}_{T}} (GeV/#it{c})^{-1}", 1);

    TH1D* hCrossSection = (TH1D*)h1yieldCorrEta->Clone("hCrossSection");
    hCrossSection->SetName("hCrossSection");
    hCrossSection->Scale((59.4/(2.*TMath::Pi()*etaRange))*1e+9);
    SetHistogramPropertiesAlt(hCrossSection, "#it{p}_{T} GeV/#it{c}", "#it{E} #frac{d^{3}#it{#sigma}}{d#it{p}^{3}} (pb GeV^{#minus2} #it{c}^{3})", 1);

    inFileSignalExtractionEta->cd();
    mesonEffEta->Write("",TObject::kOverwrite);
    mesonAccEta->Write("",TObject::kOverwrite);
    mesonAccXEffEta->Write("",TObject::kOverwrite);
    h1yieldCorrEta->Write("",TObject::kOverwrite);
    hCrossSection->Write("",TObject::kOverwrite);

    inFileSignalExtractionEta->Close();

    TObjArray* arr = new TObjArray();
    arr->Add(h1yieldCorrEta);    
    //TCanvas* can = (TCanvas*)makeCanvas(arr, 0, "LogX LogY CMYK|NoTime", 0, 0);

    TObjArray* arr2 = new TObjArray();
    arr2->Add(mesonAccXEffEta);    
   // TCanvas* can2 = (TCanvas*)makeCanvas(arr2, 0, "LogX LogY CMYK|NoTime", 0, 0);