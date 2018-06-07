#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TMatrixD.h>
#include <iomanip>            // ci serve per manipolare l'output a video
#include <TMath.h>
#include <TH1.h>
using namespace std;

void polar(){
  /*int classi = 6;
  int n = 13;
  float xmin=-0.15,xmax=0.45;
  float data[]={2,9,12,18,7,2};
  float no[]={1,1,4,5,4,8,9,9,6,1,0,2};*/
  /*
  int classi = 4;
  int n = 8;
  float xmin=2.9,xmax=3.25;
  float data[]={14,21,11,4};
  float no[]={4,10,16,5,10,1,3,1};*/
/*  int classi = 5;
  int n = 10;
  float xmin=172.9,xmax=173.35;
  float data[]={4,14,18,13,2};
  float no[]={2,2,10,3,11,7,6,7,1,1};*/
  TH1F *hist = new TH1F("hist","gauss",classi,xmin,xmax);
  for(int i = 0; i<classi;i++){
    hist->SetBinContent(i+1,data[i]);
  //  hist->SetBinError(i+1,)
}
  TH1F *h2 = new TH1F("h2","gauss",n,xmin,xmax);
  for(int i = 0; i<n;i++){
    h2->SetBinContent(i+1,no[i]);
  //  hist->SetBinError(i+1,)
}
  TF1 *f1 = new TF1("f1", "gaus");
  hist->Fit("f1");
  TF1 *f2 = new TF1("f2", "gaus");
  h2->Fit("f2");
  hist->Draw();
  h2->Draw("same");
  cout << "Chi^2:" << f1->GetChisquare() << ", number of DoF: " << f1->GetNDF() << " (Probability: " << f1->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  cout << "Chi^2:" << f2->GetChisquare() << ", number of DoF: " << f2->GetNDF() << " (Probability: " << f2->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

}
