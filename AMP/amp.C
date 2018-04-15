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

void LOWpass(){
  int n = 13;
  float freq[]  = {105.9,1071,10580,20530,31390,43520,58350,64250,71240,78760,84000,101900,218200};
  float Vi[]    = {743,737,725,718,725,725,725,725,725,725,725,725,731}; // V
  float Vo[]    = {3840,3680,3150,2250,1650,1310,1030,987,900,818,768,643,304};

  // ---------------------- Quantità derivate ----------------------- //

  // Array che conterà incertezze su Vi e Vo
  float sVi[n];
  float sVo[n];
  float sf[n];
  float T[n];
  float sT[n];
  // Array che conterrà i valori calcolati dell'attenuazione  e  incertezze
  float Av[n];
  float sAv[n];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n;i++){
    sVi[i] = 0.015*Vi[i]+0.005*2.0;
    sVo[i] = 0.015*Vo[i];
    if(Vo[i] < 2) Vo[i]+=0.005*0.2;
    if(Vo[i] >= 2) Vo[i]+=0.005;
    T[i]= 1/freq[i];
    freq[i] < 1000 ? sf[i]= 0.1 : sf[i]=10;
    sT[i]=sf[i]*T[i]/freq[i];
    Av[i]=Vo[i]/Vi[i];
    sAv[i] = Av[i]*sqrt(pow(sVo[i]/Vo[i],2)+pow(sVi[i]/Vi[i],2));
    printf("f(Hz): (%.2f±%.2f) \t Vi(V): (%.1f±%.1f) \t Vo(V): (%.2f±%.2f) \t Av(unitario): (%.f±%.4f) \n",freq[i],sf[i],Vi[i],sVi[i],Vo[i],sVo[i],Av[i],sAv[i]);
  }
    // --------------------- Grafico Av(freq)  ------------------------------ //

  TCanvas *ca2 = new TCanvas("av","A(f)",200,10,600,400);
  ca2->SetFillColor(0);
  ca2->SetLogx();
  ca2->cd();
  TGraphErrors *ga2 = new TGraphErrors(n,freq,Av,sf,sAv);
  ga2->SetMarkerSize(0.6);
  ga2->SetMarkerStyle(21);
  // Facile, titolo del grafico
  ga2->SetTitle("A(f)");
  // Titoli degli assi
  ga2->GetXaxis()->SetTitle("freq [Hz]");
  ga2->GetYaxis()->SetTitle("A [u]");
  ga2->Draw("AP");
  cout << "\n\n --- Fit Guadagno \n" <<endl;
  TF1 *fit1 = new TF1("fit","[0]*[1]/sqrt(x*x+[0]*[0])",0,250000);
  fit1->SetParameter(1,5);
  fit1->SetLineColor(14);
  ga2->Fit(fit1,"RM+");
  cout << "Chi^2:" << fit1->GetChisquare() << ", number of DoF: " << fit1->GetNDF() << " (Probability: " << fit1->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  TFitResultPtr res = ga2->Fit(fit1,"RMS+");
  TMatrixD cov = res->GetCovarianceMatrix();
  cov.Print();

}
void amp(){
  LOWpass();
}
