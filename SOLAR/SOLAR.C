#include <iostream>           // ci serve per stampare a vIceo il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a vIceo
#include <TMath.h>
using namespace std;
# define M_PI           3.14159265358979323846  /* pi */

void SOLAR(){
  float Ic[]    = {2.659,2.982,3.212,3.323,3.808,4.267,4.820,5.420,6.240,7.262,10.13,11.62,14.31,18.58,23.60};
  float Vc[]    = {42.1,41.5,48.9,78.2,90.0,100,111.7,129.5,142.2,163.9,138.4,183.9,224.7,273,339.9};
  float Vp[]    = {184,190.2,200,211.5,221,233,244.2,255,267.6,284.3,302.2,317.8,342.0,373.7,409};
  float Ib[]    = {1.169,1.188,1.316,1.383,1.378,1.510,1.629,1.745,1.939,2.188,2.470,2.260,2.289,2.293,2.319};
  float Vb[]    = {17,18.1,20,32.5,32.5,35.3,37.7,40.2,44.1,48.9,33.5,35.5,35.9,33.4,33.5};
  float Vpb[]   = {112,119,118,213.3,137.6,146,154.5,157.9,161.6,170.9,175.7,165.0,165.7,158.7,167.7};
  float D[]     = {90,85,80,75,70,65,60,55,50,45,40,35,30,25,20};
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
  // ciclo for (loop) sulle misure
  for(int i=0; i<n;i++){
    D[i]-=2.6;
    sD[i] = 0.2;
    sIc[i] = 0.02*Ic[i]; //0.5%
    if(sIc[i]<10.0)sIc[i]+=0.005;
    if(sIc[i]>=10.0 && sIc[i]<100.0)sIc[i]+=0.05;
    sVc[i] = 0.005*Vc[i];
    sVc[i]<400.0?sVc[i]+=0.3:sVc[i]+=3;
    sVp[i] = 0.005*Vp[i];
    sVp[i]<400.0?sVp[i]+=0.3:sVp[i]+=3;
    sIc[i]+=0.2;
    sIb[i] = 0.02*Ib[i]; //0.5%
    if(sIb[i]<10.0)sIb[i]+=0.005;
    if(sIb[i]>=10.0 && sIb[i]<100.0)sIb[i]+=0.05;
    sVb[i] = 0.005*Vb[i];
    sVb[i]<400.0?sVb[i]+=0.3:sVb[i]+=3;
    sVpb[i] = 0.005*Vpb[i];
    sVpb[i]<400.0?sVpb[i]+=0.3:sVpb[i]+=3;
    sIb[i]+=0.2;
    Icc[i]= Vp[i]*Ic[i]/(Vp[i]-Vc[i]);
    sIcc[i] = sqrt(pow(Ic[i]*Vc[i]*sVp[i]/pow(Vp[i]-Vc[i],2),2)+pow(Ic[i]*Vp[i]*sVc[i]/pow(Vp[i]-Vc[i],2),2)+pow(Vp[i]*sIc[i]/(Vp[i]-Vc[i]),2));
    P[i]=Icc[i]*Vp[i]*1e-3;
    sP[i]=sqrt(pow(sIcc[i]/Icc[i],2)+pow(sVp[i]/Vp[i],2))*P[i];

    Icb[i]= Vpb[i]*Ib[i]/(Vpb[i]-Vb[i]);
    sIcb[i] = sqrt(pow(Ib[i]*Vb[i]*sVpb[i]/pow(Vpb[i]-Vb[i],2),2)+pow(Ib[i]*Vpb[i]*sVb[i]/pow(Vpb[i]-Vb[i],2),2)+pow(Vpb[i]*sIb[i]/(Vpb[i]-Vb[i]),2));
    Pb[i]=Icb[i]*Vpb[i]*1e-3;
    sPb[i]=sqrt(pow(sIcb[i]/Icb[i],2)+pow(sVpb[i]/Vpb[i],2))*Pb[i];

    P[i] -= Pb[i];
    sP[i]+= sPb[i];
    //printf("Vc(mV): (%.1f±%.1f) \t Ic(mA): (%.3f±%.3f) \t Vp(mV): (%.1f±%.1f)\t Icc(mA): (%.3f±%.3f) \t P(W): (%.3f±%.3f)\n",Vc[i],sVc[i],Ic[i],sIc[i],Vp[i],sVp[i],Icc[i],sIcc[i],P[i],sP[i]);

  }

    // --------------------- Grafico I(V)  ------------------------------ //

  TCanvas *cav = new TCanvas("P(D)","P(D)",200,10,600,400);
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
  TF1 *fit = new TF1("fit","[0]*(1-x*pow([1]*[1]+x*x,-0.5))+[2]",25,90);
  fit->SetParameter(0,1e4);
  fit->SetParameter(1,4);
  gav->Fit(fit,"RM+");
  cout << "Chi^2:" << fit->GetChisquare() << ", number of DoF: " << fit->GetNDF() << " (Probability: " << fit->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
}
