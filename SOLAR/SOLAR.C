#include <iostream>           // ci serve per stampare a vIceo il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a vIceo
#include <TMath.h>
using namespace std;

void SOLAR(){
  float Ic[]    = {2.659,2.982,3.212,3.323,3.808,4.267,4.820,5.420,6.240,7.262,10.13,11.62,14.31,18.58,23.60}; //acceso
  float Vc[]    = {42.1,41.5,48.9,78.2,90.0,100,111.7,129.5,142.2,163.9,138.4,183.9,224.7,273,339.9};         //acceso amperometro
  float Vp[]    = {184,190.2,200,211.5,221,233,244.2,255,267.6,284.3,302.2,317.8,342.0,373.7,409};            //acceso a vuoto
  float Ib[]    = {1.169,1.188,1.316,1.383,1.378,1.510,1.629,1.745,1.939,2.188,2.470,2.260,2.289,2.293,2.319}; //cono ombra
  float Vb[]    = {17,18.1,20,32.5,32.5,35.3,37.7,40.2,44.1,48.9,33.5,35.5,35.9,33.4,33.5};                   //ombra amperometro
  float Vpb[]   = {112,119,118,213.3,137.6,146,154.5,157.9,161.6,170.9,175.7,165.0,165.7,158.7,167.7};        //ombra a vuoto
  float D[]     = {90,85,80,75,70,65,60,55,50,45,40,35,30,25,20};                                             //distanza
  int n = sizeof(Ic)/sizeof(*Ic);
  if(n != sizeof(Vc)/sizeof(*Vc) && n != sizeof(Vp)/sizeof(*Vp)){
    cout<<"il numero di misure non corrisponde!!!"<<endl;
    while(1);
  }
  // ---------------------- Quantità derivate ----------------------- //
  // Array che conterà incertezze su Vi e Vo
  float sIc[n];
  float sVc[n];
  float sIb[n];
  float sVb[n];
  float Icb[n];
  float sIcb[n];
  float sVp[n];
  float sVpb[n];
  float sD[n];
  float Icc[n];
  float sIcc[n];
  float P[n];
  float sP[n];
  float Pb[n];
  float sPb[n];
  float eta[n];
  float seta[n];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n;i++){
    D[i]-=2.6;
    sD[i] = 0.2;
    sIc[i] = 0.005*Ic[i]; //0.5%
    if(sIc[i]<10.0)sIc[i]+=0.005;
    if(sIc[i]>=10.0 && sIc[i]<100.0)sIc[i]+=0.05;
    sVc[i] = 0.005*Vc[i];
    sVc[i]<400.0?sVc[i]+=0.3:sVc[i]+=3;
    sVp[i] = 0.005*Vp[i];
    sVp[i]<400.0?sVp[i]+=0.3:sVp[i]+=3;
    sIc[i]+=0.2;
    sIb[i] = 0.005*Ib[i]; //0.5%
    if(sIb[i]<10.0)sIb[i]+=0.005;
    if(sIb[i]>=10.0 && sIb[i]<100.0)sIb[i]+=0.05;
    sVb[i] = 0.005*Vb[i];
    sVb[i]<400.0?sVb[i]+=0.3:sVb[i]+=3;
    sVpb[i] = 0.005*Vpb[i];
    sVpb[i]<400.0?sVpb[i]+=0.3:sVpb[i]+=3;
    sIb[i]+=0.2;
    Icc[i]= Vp[i]*Ic[i]/(Vp[i]-Vc[i]); //corrente corto circuito
    sIcc[i] = sqrt(pow(Ic[i]*Vc[i]*sVp[i]/pow(Vp[i]-Vc[i],2),2)+pow(Ic[i]*Vp[i]*sVc[i]/pow(Vp[i]-Vc[i],2),2)+pow(Vp[i]*sIc[i]/(Vp[i]-Vc[i]),2));
    P[i]=Icc[i]*Vp[i]*1e-3; //potenza di corto circuito
    sP[i]=sqrt(pow(sIcc[i]/Icc[i],2)+pow(sVp[i]/Vp[i],2))*P[i];

    Icb[i]= Vpb[i]*Ib[i]/(Vpb[i]-Vb[i]); //corrente cc di buio
    sIcb[i] = sqrt(pow(Ib[i]*Vb[i]*sVpb[i]/pow(Vpb[i]-Vb[i],2),2)+pow(Ib[i]*Vpb[i]*sVb[i]/pow(Vpb[i]-Vb[i],2),2)+pow(Vpb[i]*sIb[i]/(Vpb[i]-Vb[i]),2));
    Pb[i]=Icb[i]*Vpb[i]*1e-3;            //potenza di buio
    sPb[i]=sqrt(pow(sIcb[i]/Icb[i],2)+pow(sVpb[i]/Vpb[i],2))*Pb[i];
    float Lamp=12.1*4.4*1000;
    float sLamp = Lamp*sqrt(pow((0.1/12.1),2)+pow((0.1/4.4),2));
    P[i] -= Pb[i]; //potenza diretta irradiante
    sP[i]+= sPb[i];
    eta[i]=P[i]*4*D[i]*D[i]/(2.825*2.825*Lamp);
    seta[i]=4*sqrt(pow(pow(D[i],2)/(Lamp*pow(2.825,2))*sP[i],2)+pow(2*P[i]*D[i]/(Lamp*2.825*2.825)*sD[i],2)+pow(2*P[i]*pow(D[i],2)/(Lamp*pow(2.825,3))*(0.001),2)+pow(P[i]*pow(D[i],2)/pow(2.825*Lamp,2)*sLamp,2));
    //printf("Vc(mV): (%.1f±%.1f) \t Ic(mA): (%.3f±%.3f) \t Vp(mV): (%.1f±%.1f)\t Icc(mA): (%.3f±%.3f) \t P(W): (%.3f±%.3f)\n",Vc[i],sVc[i],Ic[i],sIc[i],Vp[i],sVp[i],Icc[i],sIcc[i],P[i],sP[i]);
  }

    // --------------------- Grafico P(d)  ------------------------------ //

  TCanvas *cav = new TCanvas("P(d)","P(d)",200,10,600,400);
  cav->SetFillColor(0);
  cav->cd();
  TGraphErrors *gav = new TGraphErrors(n,D,P,sD,sP);
  gav->SetMarkerSize(0.6);
  gav->SetMarkerStyle(21);
  // Facile, titolo del grafico
  gav->SetTitle("P(D)");
  // Titoli degli assi
  gav->GetYaxis()->SetTitle("P [mW]");
  gav->GetXaxis()->SetTitle("D [cm]");
  gav->Draw("AP");

  // --------------------------------- MAGICAL FIT TIME -----------------------------------------
// Fit Early

  cout << "\n\n --- Fit P(D) \n" <<endl;
  TF1 *fit = new TF1("fit","[0]*[1]*pow([2],2)*pow(2*x,-2)+[3]",30,95);
  fit->SetParameter(0,5e4);
  fit->SetParameter(1,1e-1);
  fit->SetParameter(2,3);
  fit->SetParameter(3,1e-1);
  gav->Fit(fit,"RM+");
  cout << "Chi^2:" << fit->GetChisquare() << ", number of DoF: " << fit->GetNDF() << " (Probability: " << fit->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

// --------------------- Grafico V(d)  ------------------------------ //

TCanvas *caV = new TCanvas("V(d)","V(d)",200,10,600,400);
caV->SetFillColor(0);
caV->cd();
TGraphErrors *gaV = new TGraphErrors(n,D,Vp,sD,sVp);
gaV->SetMarkerSize(0.6);
gaV->SetMarkerStyle(21);
// Facile, titolo del grafico
gaV->SetTitle("V(d)");
// Titoli degli assi
gaV->GetYaxis()->SetTitle("V [mV]");
gaV->GetXaxis()->SetTitle("D [cm]");
gaV->Draw("AP");


// --------------------- Grafico I(d)  ------------------------------ //

TCanvas *cai = new TCanvas("I(d)","I(d)",200,10,600,400);
cai->SetFillColor(0);
cai->cd();
TGraphErrors *gai = new TGraphErrors(n,D,Icc,sD,sIcc);
gai->SetMarkerSize(0.6);
gai->SetMarkerStyle(21);
// Facile, titolo del grafico
gai->SetTitle("I(d)");
// Titoli degli assi
gai->GetYaxis()->SetTitle("I [mA]");
gai->GetXaxis()->SetTitle("D [cm]");
gai->Draw("AP");

// --------------------- Grafico eta(d)  ------------------------------ //

TCanvas *can = new TCanvas("eta (d)","eta (d)",200,10,600,400);
can->SetFillColor(0);
can->cd();
TGraphErrors *gan = new TGraphErrors(n,D,eta,sD,seta);
gan->SetMarkerSize(0.6);
gan->SetMarkerStyle(21);
// Facile, titolo del grafico
gan->SetTitle("#eta (d)");
// Titoli degli assi
gan->GetYaxis()->SetTitle("#eta");
gan->GetXaxis()->SetTitle("D [cm]");
gan->Draw("AP");
}
