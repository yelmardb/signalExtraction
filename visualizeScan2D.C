#include "includes/Plotting.h"

void visualizeScan2D() {
    // Open the results file
    std::ifstream infile("parameter_scan_results.txt");
    double p1, p2, p3, chi2, chi2_ndf;

    // Define binning and ranges for parameters [2] and [3]
    const int nBinsX = 20;  // Adjust based on your parameter step sizes
    const int nBinsY = 20;
    double p2_min = 0.005;  // Adjust according to the scanned range of parameter [2]
    double p2_max = 0.02;
    double p3_min = 0.005;    // Adjust according to the scanned range of parameter [3]
    double p3_max = 0.02;
    double p1_min = 0.52;    // Adjust according to the scanned range of parameter [1]
    double p1_max = 0.58;

    // Create a 2D histogram for p2 and p3
    TH2F* h2 = new TH2F("h2", "Chi2/NDF vs Parameters;P1 (height); P2 (width)", 
                        nBinsX, p1_min, p1_max, nBinsY, p2_min, p2_max);
    h2->GetXaxis()->SetRangeUser(p1_min, p1_max);
    h2->GetYaxis()->SetRangeUser(0., 0.1);
    h2->GetZaxis()->SetRangeUser(0, 2);

    // Read data and fill the histogram
    while (infile >> p1 >> p2 >> p3 >> chi2 >> chi2_ndf) {
        h2->Fill(p1, p2, chi2_ndf);  // X: P2, Y: P3, Z: Chi2/NDF
    }

    // Draw the histogram
    TCanvas* c1 = new TCanvas("c1", "Parameter Scan P2 vs P3", 800, 600);
    h2->SetContour(99);  // Smooth color levels
    h2->Draw("COLZ");    // "COLZ" for a colored 2D heatmap
}

