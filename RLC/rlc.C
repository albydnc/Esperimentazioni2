#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
#include <TMath.h>
using namespace std;
# define M_PI           3.14159265358979323846  /* pi */
void BANDpass(){
  int n = 22;
  float R = 94;
  float C = 5.077e-6;
  float freq[]  = {20.54,55.57,87.58,100.7,204.2,306.6,399,456.2,505.7,547.1,608.6,693.7,804.7,1605,1939,2451,2801,3157,3734,4088,4313,4674};
  float Vi[]    = {3.71,3.79,3.68,3.62,3.10,2.68,2.50,2.45,2.46,2.46,2.53,2.62,2.73,3.25,3.35,3.46,3.53,3.56,3.64,3.65,3.67,3.70}; // V
  float Vo[]    = {0.265,0.662,0.993,1.09,1.82,2.21,2.34,2.34,2.34,2.35,2.29,2.23,2.12,1.51,1.37,1.17,1.06,1,0.868,0.837,0.781,0.706};
  float skew[]  = {-11410,13940,7870,4.175,};

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
  float AdB[n];
  float sAdB[n];
  float phase[n];
  float sPhase[n];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n;i++){
    sVi[i] = 0.015*Vi[i]+0.005*2.0;
    sVo[i] = 0.015*Vo[i];
    if(Vo[i] < 2) Vo[i]+=0.005*0.2;
    if(Vo[i] >= 2 && Vo[i] < 4) Vo[i]+=0.005*0.5;
    if(Vo[i] >= 4 && Vo[i] < 8) Vo[i]+=0.005;
    if(Vo[i] >= 8) Vo[i]+=0.010;
    T[i]= 1/freq[i];
    freq[i] < 1000 ? sf[i]= 0.01 : sf[i]=1;
    sT[i]=sf[i]*T[i]/freq[i];
    Av[i]=Vo[i]/Vi[i];
    AdB[i] = 20*log10(Av[i]);
    sAv[i] = Av[i]*sqrt(pow(sVo[i]/Vo[i],2)+pow(sVi[i]/Vi[i],2));
    sAdB[i] = 20*log(10)*sAv[i]/Av[i];
    phase[i] = 360*freq[i]*abs(skew[i])*1e-6;
    skew[i]<-1 ? sPhase[i] = phase[i]*sqrt(pow(sf[i]/freq[i],2)+pow(1/skew[i],2)):sPhase[i] = phase[i]*sqrt(pow(sf[i]/freq[i],2)+pow(0.01/skew[i],2));
    printf("f(Hz): (%.2f±%.2f) \t Vi(V): (%.1f±%.1f) \t Vo(V): (%.2f±%.2f) \t Av(unitario): (%.f±%.4f) \n",freq[i],sf[i],Vi[i],sVi[i],Vo[i],sVo[i],AdB[i],sAdB[i]);
  }
    // --------------------- Grafico Av(freq)  ------------------------------ //

  TCanvas *cav = new TCanvas("av","A(f)",200,10,600,400);
  cav->SetFillColor(0);
  cav->SetLogx();
  cav->cd();
  TGraphErrors *gav = new TGraphErrors(n,freq,Av,sf,sAv);
  gav->SetMarkerSize(0.6);
  gav->SetMarkerStyle(21);
  // Facile, titolo del grafico
  gav->SetTitle("A(f)");
  // Titoli degli assi
  gav->GetXaxis()->SetTitle("freq [Hz]");
  gav->GetYaxis()->SetTitle("A [u]");
  gav->Draw("AP");
  cout << "\n\n --- Fit Guadagno \n" <<endl;
  TF1 *lfit = new TF1("fit","x/sqrt(x*x+[0]*[0])",0,99000);
  lfit->SetParameter(0,5000);
  lfit->SetLineColor(4);
  gav->Fit(lfit,"M+");
  cout << "Chi^2:" << lfit->GetChisquare() << ", number of DoF: " << lfit->GetNDF() << " (Probability: " << lfit->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
    // --------------------- Grafico phase(freq)  ------------------------------ //
  TCanvas *cph = new TCanvas("ph","Phase(f)",200,10,600,400);
  cph->SetFillColor(0);
  cph->cd();
  cph->SetLogx();
  TGraphErrors *gph = new TGraphErrors(n,freq,phase,sf,sPhase);
  gph->SetMarkerSize(0.6);
  gph->SetMarkerStyle(21);
  // Facile, titolo del grafico
  gph->SetTitle("Phase(f)");
  // Titoli degli assi
  gph->GetXaxis()->SetTitle("freq [Hz]");
  gph->GetYaxis()->SetTitle("phase [deg]");
  gph->Draw("AP");
  cout << "\n\n --- Fit Fase \n" <<endl;
  TF1 *afit = new TF1("afit","(180/3.1415)*atan([0]/x)",0,150000);
  afit->SetParameter(0,5000);
  afit->SetLineColor(4);
  gph->Fit(afit,"RM+");
  cout << "Chi^2:" << afit->GetChisquare() << ", number of DoF: " << afit->GetNDF() << " (Probability: " << afit->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

}
void rlc(){
BANDpass();
}
