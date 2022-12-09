#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TROOT.h>
#include <TStyle.h>

#include <iostream>

void micro_200() {
  Double_t V_max = 0;
  Double_t V_min = 0;
  std::cout << '\n' << "insert V_min and V_max" << '\n';
  std::cin >> V_min >> V_max;
  gStyle->SetOptFit(1111);

  TCanvas *c_200 = new TCanvas("c_200", "Caratteristica I-V 200");
  TGraphErrors *h_200 = new TGraphErrors("I-V_200.txt", "%lg%lg%lg%lg");
  h_200->GetXaxis()->SetTitle("Voltage (mV)");
  h_200->GetYaxis()->SetTitle("Current (mA)");
  c_200->SetGrid();
  h_200->SetTitle("I-V 100");
  TF1 *fit_grafico = new TF1("200", "[0]+x*[1]", V_min, V_max);
  h_200->Fit(fit_grafico, "R");
  h_200->Draw("ap");
  c_200->Print("c_200.png");


  gStyle->SetOptFit(1111);
  TCanvas *c_fit_200= new TCanvas("c_fit_200", "Caratteristica V-I 200 (per fit)");
  TGraphErrors *prova = new TGraphErrors("V-I_200.txt", "%lg%lg%lg%lg");
  TF1 *fit_200 = new TF1("100", "[0]+x*[1]", fit_grafico->Eval(V_min),
                         fit_grafico->Eval(V_max));
  fit_200->SetParNames("a", "b");
  prova->Fit(fit_200, "R");
  prova->Draw();
  c_fit_200->Print("c_fit_200.png");

  Double_t S_1 = 0;
  Double_t S_x = 0;
  Double_t S_y = 0;
  Double_t S_xx = 0;
  Double_t S_xy = 0;
  for (int i = 0; i < 32; ++i) {
    Double_t y = h_200->GetPointX(i);
    Double_t x = h_200->GetPointY(i);
    Double_t sigma_inver = 1 / (h_200->GetErrorX(i) * h_200->GetErrorX(i));
    S_1 = S_1 + sigma_inver;
    S_x = S_x + x * sigma_inver;
    S_y = S_y + y * sigma_inver;
    S_xx = S_xx + x * x * sigma_inver;
    S_xy = S_xy + y * x * sigma_inver;
  }

  Double_t a = (S_y * S_xx - S_x * S_xy) / (S_1 * S_xx - S_x * S_x);
  Double_t b = (S_1 * S_xy - S_x * S_y) / (S_1 * S_xx - S_x * S_x);
  Double_t sigma_a = TMath::Sqrt(S_xx / (S_1 * S_xx - S_x * S_x));
  Double_t sigma_b = TMath::Sqrt(S_1 / (S_1 * S_xx - S_x * S_x));

  std::cout << "a= " << a << " +/- " << sigma_a << '\n'
            << "b= " << b << " +/- " << sigma_b << '\n';
}