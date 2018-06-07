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
using namespace std;
void fuoco(){
  int a,n;
  //cout << "Inserire numero lente:\n Convergente [1]\n Convergente [2]\n Convergente-Divergente [3]";
  //cin >> a;
  n=7;
  //lente conv 1
    //float p[]  = {200,225,250,275,300,325,350,375,400};
    //float q[]  = {190,176,168,161,155,150,148,143,141};
    float offset = 16;
  //lente conv 2
    //float p[]  = {300,325,350,375,400,425,450,475,0};
    //float q[]  = {301,281,267,259,250,243,238,232,0};
  //lente div 1
    float p[]  = {250,275,300,325,350,375,400,425,0};
    float q[]  = {235,222,209,200,195,188,185,180,0};

  float sp[n],sq[n];
  for(int i=0; i<n;i++){
    sp[i]=2./p[i];
    sq[i]=2./q[i];
    p[i]=1000./(p[i]+offset);
    q[i]=1000./(q[i]-offset);
    sp[i]*=p[i];
    sq[i]*=q[i];
    printf("p (m-1): (%.2f±%.2f) \t q(m-1): (%.2f±%.2f) \n",p[i],sp[i],q[i],sq[i]);
  }
  TCanvas *cf = new TCanvas("focus","1/q(1/p)",200,10,1200,800);
  cf->SetFillColor(0);
  cf->cd();
  TGraphErrors *gf = new TGraphErrors(n,p,q,sp,sq);
  gf->SetMarkerSize(0.6);
  gf->SetMarkerStyle(21);
  // Facile, titolo del grafico
  gf->SetTitle("Distanza focale Lente D");
  // Titoli degli assi
  gf->GetXaxis()->SetTitle("1/p [m^{-1}]");
  gf->GetYaxis()->SetTitle("1/q [m^{-1}]");
  gf->Draw("AP");
  cout << "\n\n --- Fit Fuoco \n" <<endl;
  TF1 *fit1 = new TF1("fit","1/[0]-[1]*x",0,7);
  fit1->SetParameter(0,1e-1);
  fit1->SetParameter(1,1);
  fit1->SetLineColor(14);
  gf->Fit(fit1,"RM+");
  cout << "Chi^2:" << fit1->GetChisquare() << ", number of DoF: " << fit1->GetNDF() << " (Probability: " << fit1->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
}
