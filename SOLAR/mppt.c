#include <iostream>           // ci serve per stampare a vIceo il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a vIceo
#include <TMath.h>
using namespace std;

void mppt(){
  float d = 27;
  float Vp = 360;
  float I[]    = {18.89,15.86,13.23,11.26,9.66,8.618,7.729,6.989,6.355,5.838,5.390,4.675,4.138,3.075,2.820,2.641,2.160,1.927,1.647,1.336,1.124,0.851,0.687,0.493,0.346}; //acceso
  float V[]    = {211.5,262.5,286.52,300.5,308.7,316.6,320.8,325.0,328.3,331.0,333.0,336.6,338.4,342.9,343,347.9,349.4,350.4,351.6,352.4,353.3,354.2,354.8,355.4,356};   //tensione
  float R[]    = {0,5,10,15,20,25,30,35,40,45,50,60,70,100,110,120,150,170,200,250,300,400,500,700,1000};   //resistenza
  int n = sizeof(I)/sizeof(*I);
  if(n != sizeof(V)/sizeof(*V) && n != sizeof(R)/sizeof(*R)){
    cout<<"il numero di misure non corrisponde!!!"<<endl;
    while(1);
  }
  // ---------------------- Quantità derivate ----------------------- //
  // Array che conterà incertezze su Vi e Vo
  float sI[n];
  float sV[n];
  float P[n];
  float sP[n];
  float sR[n];

  // ciclo for (loop) sulle misure
  for(int i=0; i<n;i++){
    R[i]+=22;
    sI[i] = 0.005*I[i]; //0.5%
    if(sI[i]<10.0)sI[i]+=0.005;
    if(sI[i]>=10.0 && sI[i]<100.0)sI[i]+=0.05;
    sV[i] = 0.005*V[i]+0.3;
    sR[i] = 0.05*R[i];
    P[i]=V[i]*I[i]*1e-3;
    sP[i]=P[i]*sqrt(pow(sV[i]/V[i],2)+pow(sI[i]/I[i],2));
  }

    // --------------------- Grafico P(R)  ------------------------------ //

  TCanvas *cav = new TCanvas("P(R)","P(R)",200,10,600,400);
  cav->SetFillColor(0);
  cav->cd();
  TGraphErrors *gav = new TGraphErrors(n,R,P,sR,sP);
  gav->SetMarkerSize(0.6);
  gav->SetMarkerStyle(21);
  // Facile, titolo del grafico
  gav->SetTitle("P(R)");
  // Titoli degli assi
  gav->GetYaxis()->SetTitle("P [mW]");
  gav->GetXaxis()->SetTitle("R [#Omega]");
  gav->GetXaxis()->SetLimits(0,100);

  gav->Draw("AP");

// --------------------- Grafico I(R)  ------------------------------ //

TCanvas *caV = new TCanvas("I(V)","I(V)",200,10,600,400);
caV->SetFillColor(0);
caV->cd();
TGraphErrors *gaV = new TGraphErrors(n,V,I,sV,sR);
gaV->SetMarkerSize(0.6);
gaV->SetMarkerStyle(21);
// Facile, titolo del grafico
gaV->SetTitle("I(V)");
// Titoli degli assi
gaV->GetYaxis()->SetTitle("I [mA]");
gaV->SetMinimum(0);
gaV->SetMaximum(25);
gaV->GetXaxis()->SetTitle("V [mV]");
gaV->Draw("AP");


}
