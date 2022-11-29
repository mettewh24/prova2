#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFormula.h>
#include <TGraphErrors.h>
#include <TROOT.h>
#include <TStyle.h>

#include <iostream>

void micro_100() {
  Double_t V_max = 0;
  Double_t V_min = 0;
  std::cout << '\n' << "insert V_min and V_max" << '\n';
  std::cin >> V_min >> V_max;

  TCanvas *c_100 = new TCanvas("c_100", "Caratteristica I-V 100");
  TGraphErrors *h_100 = new TGraphErrors("I-V_100.txt", "%lg%lg%lg%lg");
  h_100->GetXaxis()->SetTitle("Voltage (mV)");
  h_100->GetYaxis()->SetTitle("Current (mA)");
  c_100->SetGrid();
  h_100->SetTitle("I-V 100");
  TF1 *fit_grafico = new TF1("100", "[0]*x+[1]", V_min, V_max);
  h_100->Fit(fit_grafico, "R");
  h_100->Draw();
  c_100->Print("c_100.png");

  gStyle->SetOptFit(1111);
  TCanvas *c_fit_100 =
      new TCanvas("c_fit_100", "Caratteristica V-I 100 (per fit)");
  TGraphErrors *prova = new TGraphErrors("V-I_100.txt", "%lg%lg%lg%lg");
  TF1 *fit_100 = new TF1("100", "[0]+x*[1]", fit_grafico->Eval(V_min),
                         fit_grafico->Eval(V_max));
  fit_100->SetParNames("a", "b");
  prova->Fit(fit_100, "R");
  prova->Draw();
  c_fit_100->Print("c_fit_100.png");

  Double_t S_1 = 0;
  Double_t S_x = 0;
  Double_t S_y = 0;
  Double_t S_xx = 0;
  Double_t S_xy = 0;
  for (int i = 0; i < 32; ++i) {
    Double_t y = h_100->GetPointX(i);
    Double_t x = h_100->GetPointY(i);
    Double_t sigma_inver = 1 / (h_100->GetErrorX(i) * h_100->GetErrorX(i));
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