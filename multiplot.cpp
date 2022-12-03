#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFormula.h>
#include <TGraphErrors.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TMultiGraph.h>
#include <TROOT.h>
#include <TStyle.h>

#include <iostream>

void multiplot() {
  Double_t V_max = 1000;
  Double_t V_min = 4000;

  gROOT->SetStyle("Modern");
  TCanvas *c1 = new TCanvas();
  c1->SetGrid();
  TMultiGraph *mgraph = new TMultiGraph("mgraph", "Caratteristiche I-V");

  TGraphErrors *h_100 = new TGraphErrors("I-V_100.txt", "%lg%lg%lg%lg");
  TF1 *fit_100 = new TF1("100", "[0]*x+[1]", V_min, V_max);
  h_100->Fit(fit_100, "R");
  fit_100->SetLineColor(kRed);
  h_100->SetLineColor(kBlue);
  mgraph->Add(h_100);

  TGraphErrors *h_200 = new TGraphErrors("I-V_200.txt", "%lg%lg%lg%lg");
  h_200->SetLineColor(kBlack);
  TF1 *fit_200 = new TF1("200", "[0]+x*[1]", V_min, V_max);
  fit_200->SetLineColor(kGreen);
  h_200->Fit(fit_200, "R");
  mgraph->Add(h_200);
  mgraph->GetXaxis()->SetTitle("Voltage (mV)");
  mgraph->GetYaxis()->SetTitle("Current (mA)");
  TLegend *t = new TLegend();
  t->SetHeader("Punti sperimentali", "C");
  t->AddEntry(h_100, "100 #muA");
  t->AddEntry(h_200, "200 #muA");
  t->AddEntry(fit_100, "Fit lineare a 100 #muA");
  t->AddEntry(fit_200, "Fit lineare a 200 #muA");

  mgraph->Draw("apl");
  t->Draw("SAME");
}