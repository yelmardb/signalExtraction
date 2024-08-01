#include "includes/Plotting.h"
#include "includes/extractHistogram.h"



class MesonSignalExtraction {

    private:
        const char* inFileDataName;
        const char* inFileMCName;
        const char* meson;
        const char* run;
        TH2D* hMggPt_Same;
        TH2D* hMggPt_Mixed;
        TH1D* hCollisionCounter;
        const char* PathTo_hMggPtSame;
        const char* PathTo_hMggPtMixed;
        const char* PathTo_hCollisionCounter;
        const char* PathTo_hPtGen;
        const char* PathTo_hPtAcc;
        TH1D* hMggPt_Same1D;
        TH1D* hMggPt_Mixed1D;
        TH1D* hMggPt_SameScaled1D;
        TH1D* hMggPt_MixedScaled1D;

        Double_t npair_same = 0;
        Double_t npair_mixed = 0;
        Double_t mean = 0.0;
        Double_t sigma = 0.0;
        Double_t cb_alpha = 0.0;
        Double_t cb_n = 0.0;
        Double_t massWindowLow;
        Double_t massWindowHigh;
        Double_t theoMassMeson;


    public:
        std::vector<Double_t> pT;
        Double_t minBackgroundRange;
        Double_t maxBackgroundRange;
        Double_t minIntegrationRange;
        Double_t maxIntegrationRange;
        Bool_t isMC = false;

        TH1D* h1yield;
        TH1D* h1mean;
        TH1D* h1sigma;
        TH1D* h1ratio;
        TH1D* hCalculatedBackground;
        TH1D* h1yieldCorr;
        TH1D* mesonEff;
        TH1D* mesonAcc;
        TH1D* mesonAccXEff;
        TH1D* hpT_Rec;
        TH1D* hpT_Acc;
        TH1D* hpT_Gen;

        TFile* outputFile;
        const char* outputFileName;

        void SetPathToFile(const char* name){
            inFileDataName = name;
        }

        void SetPathsToHistos(){ 
            if(strcmp(run, "Run3") == 0){
                if(isMC){
                    PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma/Event/PCMPCM/after/hCollisionCounter";
                    PathTo_hMggPtSame = ":/pi0eta-to-gammagamma/Pair/PCMPCM/qc_qc/nocut/hMggPt_Same";
                    PathTo_hMggPtMixed = ":/pi0eta-to-gammagamma/Pair/PCMPCM/qc_qc/nocut/hMggPt_Mixed";
                    PathTo_hPtGen =  Form(":/pi0eta-to-gammagamma-mc/Generated/PCMPCM/hPt_%s", meson);
                    PathTo_hPtAcc =  Form(":/pi0eta-to-gammagamma-mc/Generated/PCMPCM/hPt_%s_Acc", meson);
                }
                else{
                    PathTo_hCollisionCounter = ":/pi0eta-to-gammagamma-pcmpcm/Event/after/hCollisionCounter";
                    PathTo_hMggPtSame = ":/pi0eta-to-gammagamma-pcmpcm/Pair/same/hMggPt";
                    PathTo_hMggPtMixed = ":/pi0eta-to-gammagamma-pcmpcm/Pair/mix/hMggPt";
                }
            }
            else if(strcmp(run, "Run2") == 0){
                std::cout << "Run2 not implemented yet" << std::endl;
            }            
        }

        void SetOutputFileName(const char* name){
            outputFileName = name;
        }

        void SetPtBinning(std::vector<Double_t> thispT){
            pT = thispT;
        }

        void SetMeson(const char* thismeson){
            meson = thismeson;
            if(strcmp(thismeson, "Pi0")  == 0) theoMassMeson = 0.135;
            else if(strcmp(thismeson, "Eta")  == 0) theoMassMeson = 0.545;
        }

        void SetRun(const char* thisrun){
            if(strcmp(thisrun, "Run2") == 0){
                run = thisrun;
            }
            else if(strcmp(thisrun, "Run3") == 0){
                run = thisrun;
            }
            std::cout << "INFO: Running on " << run << "..." << std::endl;
        }


        void SetMC(){
            isMC = true;
        }

        void SetIntegrationRange(Double_t min, Double_t max){
            minIntegrationRange = min;
            maxIntegrationRange = max;
        }

        void SetBackgroundRange(Double_t min, Double_t max){
            minBackgroundRange = min;
            maxBackgroundRange = max;
        }


        void SetMassWindow(Double_t low, Double_t high){
            massWindowLow = low;
            massWindowHigh = high;
        }

        TH1D* ProjHistogramX(TH2D* h2d, Double_t low, Double_t high){
            Double_t tmp = 1e-6;
            Int_t binLow = h2d->GetYaxis()->FindBin(low + tmp);
            Int_t binHigh = h2d->GetYaxis()->FindBin(high - tmp);

            TH1D* h1d;
            if(high <= pT.back() && low > 0){
                h1d = (TH1D*)h2d->ProjectionX(Form("%s_proj_pT_%.2f_%.2f", h2d->GetName(), low, high), binLow, binHigh);
                return h1d;
            } 
            else{
                cout << "ERROR: Histogram out of range" << endl;
                return nullptr;
            }
        }
        void LoadHistograms(){
            hCollisionCounter = (TH1D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hCollisionCounter)); 
            hMggPt_Mixed = (TH2D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hMggPtMixed));
            hMggPt_Mixed->SetName("hMggPt_Mixed");
            hMggPt_Same = (TH2D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hMggPtSame));
            hMggPt_Same->SetName("hMggPt_Same");
            if(isMC){
                hpT_Gen = (TH1D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hPtGen));
                hpT_Acc = (TH1D*)extractHistogram(Form("%s%s",inFileDataName, PathTo_hPtAcc));
            }
        }

        void InitHistograms(){
            outputFile = new TFile(Form("%s", outputFileName), "RECREATE");
            h1yield = new TH1D("h1yield", "", pT.size()-1, pT.data());
            h1yieldCorr = new TH1D("h1yieldCorr", "", pT.size()-1, pT.data());
            h1mean = new TH1D("h1mean", "", pT.size()-1, pT.data());
            h1sigma = new TH1D("h1sigma", "", pT.size()-1, pT.data());
            mesonEff = new TH1D("mesonEfficiency", "", pT.size()-1, pT.data());
            mesonAcc = new TH1D("mesonAcceptance", "", pT.size()-1, pT.data());
            mesonAccXEff = new TH1D("mesonAcceptanceXEfficiency", "", pT.size()-1, pT.data());
        }


         void SetFitParameters(){
            if(strcmp(meson, "Pi0") == 0){
                mean = 0.132;
                sigma = 0.005;
                cb_alpha = 0.6;
                cb_n = 12.0;
            }
            else if(strcmp(meson, "Eta") == 0){
                mean = 0.545;
                sigma = 0.008;
                cb_alpha = 0.6;
                cb_n = 12.0;
            }
        }


        void ExtractSignal(){ //This is the main function
            for(int i = 0; i < pT.size()-1; i++){
                hMggPt_Same1D = ProjHistogramX(hMggPt_Same, pT[i], pT[i+1]);
                hMggPt_Same1D->SetName(Form("hMggPt_Same1D_pT_%.2f_%.2f", pT[i], pT[i+1]));
                hMggPt_Mixed1D = ProjHistogramX(hMggPt_Mixed, pT[i], pT[i+1]);
                hMggPt_Mixed1D->SetName(Form("hMggPt_Mixed1D_pT_%.2f_%.2f", pT[i], pT[i+1]));
               
                if(pT[i] < 6. || isMC){
                    hMggPt_Same1D->Rebin(4);
                    hMggPt_Mixed1D->Rebin(4);
                } 
                else if(pT[i] > 6.){
                    hMggPt_Same1D->Rebin(8);
                    hMggPt_Mixed1D->Rebin(8);
                }   
                
                
                npair_same = hMggPt_Same1D->GetEntries();
                npair_mixed = hMggPt_Mixed1D->GetEntries();
  
                hMggPt_SameScaled1D = (TH1D*)hMggPt_Same1D->Clone(Form("hMggPt_SameScaled1D_pT_%.2f_%.2f", pT[i], pT[i+1]));
               // hMggPt_SameScaled1D->Scale(1./npair_same);
                hMggPt_SameScaled1D->SetTitle(Form("hMggPt_SameScaled1D_pT_%.2f_%.2f", pT[i], pT[i+1]));
                hMggPt_MixedScaled1D = (TH1D*)hMggPt_Mixed1D->Clone(Form("hMggPt_MixedScaled1D_pT_%.2f_%.2f", pT[i], pT[i+1]));
               // hMggPt_MixedScaled1D->Scale(1./npair_mixed); // By scaling hMggPt_MixedScaled1D by this factor, you adjust the mix histogram so that it has the same total number of entries as the same histogram
                hMggPt_MixedScaled1D->SetTitle(Form("hMggPt_MixedScaled1D_pT_%.2f_%.2f", pT[i], pT[i+1]));


                SetHistogramPropertiesAlt(hMggPt_SameScaled1D, "", "hMggPt_SameScaled1D", 1); 
                SetHistogramPropertiesAlt(hMggPt_MixedScaled1D, "", "hMggPt_MixedScaled1D",1);
                h1ratio = (TH1D*)hMggPt_SameScaled1D->Clone(Form("h1ratio_pT_%.2f_%.2f", pT[i], pT[i+1]));
                h1ratio->Reset();
                h1ratio->SetTitle(Form("h1ratio_pT_%.2f_%.2f", pT[i], pT[i+1]));
                h1ratio->Divide(hMggPt_SameScaled1D, hMggPt_MixedScaled1D, 1, 1, "B");


                TF1* fitSignal = new TF1(Form("fitSignal_pT_%.2f_%.2f", pT[i], pT[i+1]), "crystalball", 0., 1.0);
                fitSignal->SetNpx(10000);
                fitSignal->SetParameter(1, mean);
                fitSignal->SetParameter(2, sigma);
                fitSignal->FixParameter(3, cb_alpha);
                fitSignal->FixParameter(4, cb_n);
                fitSignal->SetParLimits(0, 0., hMggPt_SameScaled1D->GetMaximum()*1.1);
                fitSignal->SetParLimits(1, mean * 0.7, mean*1.3);
                fitSignal->SetParLimits(2, sigma * 0.5, sigma*1.5);

                TF1* fitBackground = new TF1(Form("fitBackground_pT_%.2f_%.2f", pT[i], pT[i+1]), "pol1", 0., 1.);
                fitBackground->SetNpx(10000);

                h1ratio->Fit(fitBackground, "SMEQ", "", minBackgroundRange, maxBackgroundRange);
                TH1D* hCalculatedBackground = (TH1D*)hMggPt_MixedScaled1D->Clone(Form("hCalculatedBackground_pT_%.2f_%.2f", pT[i], pT[i+1]));
                hCalculatedBackground->Multiply(fitBackground, 1.);

                TH1D* hCalculatedSignal = (TH1D*)hMggPt_SameScaled1D->Clone(Form("hCalculatedSignal_pT_%.2f_%.2f", pT[i], pT[i+1]));
                hCalculatedSignal->Add(hCalculatedBackground, -1.);
                Double_t height = hMggPt_SameScaled1D->GetBinContent(hMggPt_SameScaled1D->FindBin(mean));
                fitSignal->SetParameter(0, height);
                hCalculatedSignal->Fit(fitSignal, "SMEQ", "", massWindowLow, massWindowHigh);

                Double_t meanCalc = fitSignal->GetParameter(1);
                Double_t meanErrCalc  = fitSignal->GetParError(1);
                Double_t sigmaCalc = fitSignal->GetParameter(2);
                Double_t sigmaErrCalc  = fitSignal->GetParError(2);

                h1mean->SetBinContent(i+1, meanCalc);
                h1mean->SetBinError(i+1, meanErrCalc);
                h1sigma->SetBinContent(i+1, sigmaCalc);
                h1sigma->SetBinError(i+1, sigmaErrCalc);


                TH1D* hCalculatedSignalDummy =  (TH1D*)hCalculatedSignal->Clone(Form("hCalculatedSignalDummy_pT_%.2f_%.2f", pT[i], pT[i+1]));
               //hCalculatedSignalDummy->Scale(npair_same);
                Int_t tmpBin = hCalculatedSignalDummy->FindBin(mean + minIntegrationRange);
                Int_t tmpBin2 = hCalculatedSignalDummy->FindBin(mean + maxIntegrationRange);
                Double_t rawYieldErr;
                Double_t rawYield = hCalculatedSignalDummy->IntegralAndError(tmpBin,tmpBin2,rawYieldErr,"");
                h1yield->SetBinContent(i+1, rawYield);
                h1yield->SetBinError(i+1, rawYieldErr);

                SetHistogramPropertiesAlt(h1mean, "#it{p}_{T} GeV/#it{c}", "h1mean", 1);
                SetHistogramPropertiesAlt(h1sigma, "#it{p}_{T} GeV/#it{c}", "sigma", 1);
                SetHistogramPropertiesAlt(h1yield, "#it{p}_{T} GeV/#it{c}", "yield", 1);

                outputFile->cd();
                hMggPt_Same1D->Write();
                hMggPt_Mixed1D->Write();
                h1ratio->Write();
                hCalculatedSignal->Write();
                fitSignal->Write();
                hCalculatedBackground->Write();
                //fitBackground->Write();
            }

            Double_t nEvents = hCollisionCounter->GetBinContent(7);
            h1yield->Scale(1/nEvents); 
            h1yield->Scale(1., "width"); 

            hMggPt_Same->Write();
            hMggPt_Mixed->Write();
            h1mean->Write();
            h1sigma->Write();
            h1yield->Write();
            hCollisionCounter->Write();

            outputFile->Close();
        }


};


