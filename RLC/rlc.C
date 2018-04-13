#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
#include <TMath.h>
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TMatrixD.h>
using namespace std;
void BANDpass(){
  int n = 22;
  float R = 94;
  float C = 5.077e-6;
  float freq[]  = {20.54,55.57,87.58,100.7,204.2,306.6,399,456.2,505.7,547.1,608.6,693.7,804.7,1605,1939,2451,2801,3157,3734,4088,4313,4674};
  float Vi[]    = {3.71,3.79,3.68,3.62,3.10,2.68,2.50,2.45,2.46,2.46,2.53,2.62,2.73,3.25,3.35,3.46,3.53,3.56,3.64,3.65,3.67,3.70}; // V
  float Vo[]    = {0.265,0.662,0.993,1.09,1.82,2.21,2.34,2.34,2.34,2.35,2.29,2.23,2.12,1.51,1.37,1.17,1.06,1,0.868,0.837,0.781,0.706};
  float skew[]  = {-11410,13940,-2340,7870,4175,-282.5,2402,-40.0,0.000,22.50,45.00,80.00,96.00,93.00,85.50,72.50,64.00,61.50,54.00,48.37,48.25,44.84};

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
  float phase[n];
  float sPhase[n];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n;i++){
    sVi[i] = 0.015*Vi[i]+0.005*0.5;
    sVo[i] = 0.015*Vo[i];
    if(Vo[i] < 0.3) Vo[i]+=0.005*0.05;
    if(Vo[i] >= 0.3 && Vo[i] < 1.1) Vo[i]+=0.005*0.2;
    if(Vo[i] >= 1.1 ) Vo[i]+=0.005*0.5;
    T[i]= 1/freq[i];
    sf[i]=0.05*freq[i];
    sT[i]=sf[i]*T[i]/freq[i];
    Av[i]=Vo[i]/Vi[i];
    sAv[i] = Av[i]*sqrt(pow(sVo[i]/Vo[i],2)+pow(sVi[i]/Vi[i],2));
    //phase[i] = 360*freq[i]*abs(skew[i])*1e-6;
    skew[i]<-1 ? sPhase[i] = phase[i]*sqrt(pow(sf[i]/freq[i],2)+pow(1/skew[i],2)):sPhase[i] = phase[i]*sqrt(pow(sf[i]/freq[i],2)+pow(0.01/skew[i],2));
    printf("f(Hz): (%.2f±%.2f) \t Vi(V): (%.1f±%.1f) \t Vo(V): (%.2f±%.2f) \t Av(unitario): (%.f±%.4f) \n",freq[i],sf[i],Vi[i],sVi[i],Vo[i],sVo[i],Av[i],sAv[i]);
    //skew[i]<0 ? skew[i]=T[i]+skew[i] : skew[i]=skew[i]; // converto le fasi in positivo
    phase[i] = 360*freq[i]*skew[i]*1e-6;
    phase[i] > 90 ? phase[i]-=360 : 1;
    phase[i]=-phase[i];
    sPhase[i]=5;
    //skew[i]>1000 ? sPhase[i] = phase[i]*sqrt(pow(sf[i]/freq[i],2)+pow(5000/skew[i],2)):sPhase[i] = phase[i]*sqrt(pow(sf[i]/freq[i],2)+pow(1/skew[i],2));
    //printf("f(Hz): (%.2f±%.2f) \t Vi(V): (%.1f±%.1f) \t Vo(V): (%.2f±%.2f) \t Av(unitario): (%.f±%.4f) \n",freq[i],sf[i],Vi[i],sVi[i],Vo[i],sVo[i],AdB[i],sAdB[i]);
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
  TF1 *lfit = new TF1("fit","x*[0]*[0]/sqrt(x*x*pow([0],4)*(1+[1])*(1+[1])+[2]*[2]*(x*x-[0]*[0])*(x*x-[0]*[0]))",30,5000);
  lfit->SetParameter(0,500);
  lfit->SetParameter(1,1e-1);
  lfit->SetParameter(2,300);
  lfit->SetLineColor(14);
  gav->Fit(lfit,"R+");
  cout << "Chi^2:" << lfit->GetChisquare() << ", number of DoF: " << lfit->GetNDF() << " (Probability: " << lfit->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  TFitResultPtr res = gav->Fit(lfit,"RMS+");
  TMatrixD cov = res->GetCovarianceMatrix();
  cov.Print();
   // --------------------- Grafico phase(freq)  ------------------------------ //
  TCanvas *cph = new TCanvas("ph","Phase(f)",200,10,600,400);
  cph->SetFillColor(0);
  cph->cd();
  cph->SetLogx();
  TGraphErrors *gph = new TGraphErrors(n,freq,phase,sf,sPhase);
  gph->SetMarkerSize(0.6);
  gph->SetMarkerStyle(21);
  // Facile, titolo del grafico
  gph->SetTitle("Fase(f)");
  // Titoli degli assi
  gph->GetXaxis()->SetTitle("freq [Hz]");
  gph->GetYaxis()->SetTitle("Fase [deg]");
  gph->Draw("AP");
  cout << "\n\n --- Fit Fase \n" <<endl;
  TF1 *afit = new TF1("afit","(180/3.1415)*atan(([2]/(1+[1]))*([0]*[0]-x*x)/([0]*[0]*x))",0,5000);
  afit->SetParameter(0,500);
  afit->SetParameter(1,1e-1);
  afit->SetParameter(2,300);
  afit->SetLineColor(14);
  gph->Fit(afit,"R+");
  cout << "Chi^2:" << afit->GetChisquare() << ", number of DoF: " << afit->GetNDF() << " (Probability: " << afit->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  TCanvas *cV = new TCanvas("Vh","Vi(f)",200,10,600,400);
  cV->SetFillColor(0);
  cV->cd();
  cV->SetLogx();
  TGraphErrors *gV = new TGraphErrors(n,freq,Vi,sf,sVi);
  gV->SetMarkerSize(0.6);
  gV->SetMarkerStyle(21);
  // Facile, titolo del grafico
  gV->SetTitle("V_{in}(f)");
  gV->SetLineColor(14);
  gV->SetMinimum(0);
  // Titoli degli assi
  gV->GetXaxis()->SetTitle("freq [Hz]");
  gV->GetYaxis()->SetTitle("V_{in} [V]");
  gV->Draw("LAP");
}
void rlc(){
BANDpass();
}
