#include <iostream>           // ci serve per stampare a vIc100eo il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a vIc100eo
#include <TMath.h>
using namespace std;
# define M_PI           3.14159265358979323846  /* pi */

void BJT(){
  float Ic100[]    = {4.457,11.138,17.57,18.47,18.68,18.82,18.92,18.96,19.30,20.23,21.09,21.95,22.87,23.78,25.18,26.91};
  float Vc100[]    = {78.9,132.3,217,305,405,503,591,614,901,1899,2900,3950,5020,6170,7880,9950};
  float Ic150[]    = {5.653,15.13,25.43,31.62,32.24,32.94,33.09,33.69,35.38,36.82,38.51,39.62,41.30,44.17,48.23};
  float Vc150[]    = {71.6,117.2,172.7,255,349,424,518,824,1839,2830,3840,4650,5880,7840,9840};
  float Ic200[]    = {6.421,18.33,25.90,68.08,44.25,44.93,45.05,46.03,48.07,50.21,52.55,54.24,56.29,59.95,65.51};
  float Vc200[]    = {73.7,121.6,148.8,192.6,279,366,475,815,1702,2780,3910,4760,5800,7670,9750};
  float Ic250[]    = {7.064,17.79,29.78,41.56,51.74,55.87,56.44,56.88,57.45,60.46,63.30,66.01,68.63,72.14,76.57,83.26};
  float Vc250[]    = {64.6,102.5,135.8,169.8,218,294,367,495,697,1695,2790,3880,4830,6060,7670,9930};
  float Ic300[]    = {7.926,18.04,29.98,42.31,56.02,66.11,68.98,69.64,70.60,74.09,77.60,79.92,83.27,86.33,93.47,100.61};
  float Vc300[]    = {64.3,101.1,133.8,159.4,198.1,254,325,409,627,1598,2770,3470,4500,5460,7730,9440};
  float Ic350[]    = {7.536,18.26,29.54,42.53,52.74,66.02,75.32,78.87,79.89,80.54,85.15,89.01,92.64,97.94,101.65,109.19,117.18};
  float Vc350[]    = {62.5,104.9,138.8,174.2,202,242,288,416,477,579,1578,2630,3990,4640,5530,7510,9520};
  float Ic400[]    = {9.318,24.08,33.43,48.74,58.13,72.06,85.34,90.47,91.88,92.63,95.24,98.01,101.81,106.04,110.60,114.97,129.4,141.8};
  float Vc400[]    = {59.3,98.3,120.1,148.7,166.3,195.9,243,307,395,497,1000,1570,2440,3500,4440,5410,7420,10210};
  int n100 = sizeof(Ic100)/sizeof(*Ic100);
  if(n100 != sizeof(Vc100)/sizeof(*Vc100)){
    cout<<"il numero di misure 100 non corrisponde!!!"<<endl;
    while(1);
  }
  cout << n100<<endl;
  int n150 = sizeof(Ic150)/sizeof(*Ic150);
  if(n150 != sizeof(Vc150)/sizeof(*Vc150)){
    cout<<"il numero di misure 150 non corrisponde!!!"<<endl;
    while(1);
  }
  cout << n150<<endl;
  int n200 = sizeof(Ic200)/sizeof(*Ic200);
  if(n200 != sizeof(Vc200)/sizeof(*Vc200)){
    cout<<"il numero di misure 200 non corrisponde!!!"<<endl;
    while(1);
  }
  cout << n200<<endl;
  int n250 = sizeof(Ic250)/sizeof(*Ic250);
  if(n250 != sizeof(Vc250)/sizeof(*Vc250)){
    cout<<"il numero di misure 250 non corrisponde!!!"<<endl;
    while(1);
  }
  cout << n250<<endl;
  int n300 = sizeof(Ic300)/sizeof(*Ic300);
  if(n300 != sizeof(Vc300)/sizeof(*Vc300)){
    cout<<"il numero di misure 300 non corrisponde!!!"<<endl;
    while(1);
  }
  cout << n300<<endl;
  int n350 = sizeof(Ic350)/sizeof(*Ic350);
  if(n350 != sizeof(Vc350)/sizeof(*Vc350)){
    cout<<"il numero di misure 350 non corrisponde!!!"<<endl;
    while(1);
  }
  cout << n350<<endl;
  int n400 = sizeof(Ic400)/sizeof(*Ic400);
  if(n400 != sizeof(Vc400)/sizeof(*Vc400)){
    cout<<"il numero di misure 400 non corrisponde!!!"<<endl;
    while(1);
  }
  cout << n400<<endl;

  // ---------------------- Quantità derivate ----------------------- //

  // Array che conterà incertezze su Vi e Vo
  float sIc100[n100];
  float sVc100[n100];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n100;i++){
    sIc100[i] = 0.005*Ic100[i];
    if(sIc100[i]<10.0)sIc100[i]+=0.02;
    if(sIc100[i]>=10.0 && sIc100[i]<100.0)sIc100[i]+=0.005;
    if(sIc100[i]>=100.0)sIc100[i]+=(0.1+0.005*Ic100[i]);
    sVc100[i] = 0.005*Vc100[i];
    if(sVc100[i]<200.0)sVc100[i]+=0.2;
    if(sVc100[i]>=200.0 && sVc100[i]<2000.0)sVc100[i]+=2;
    if(sVc100[i]>=2000.0)sVc100[i]+=20;
    printf("Vc100(mV): (%.1f±%.1f) \t Ic100(mA): (%.4f±%.4f) \n",Vc100[i],sVc100[i],Ic100[i],sIc100[i]);

  }

  float sIc150[n150];
  float sVc150[n150];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n150;i++){
    sIc150[i] = 0.005*Ic150[i];
    if(sIc150[i]<10.0)sIc150[i]+=0.02;
    if(sIc150[i]>=10.0 && sIc150[i]<150.0)sIc150[i]+=0.005;
    if(sIc150[i]>=150.0)sIc150[i]+=(0.1+0.005*Ic150[i]);
    sVc150[i] = 0.005*Vc150[i];
    if(sVc150[i]<200.0)sVc150[i]+=0.2;
    if(sVc150[i]>=200.0 && sVc150[i]<2000.0)sVc150[i]+=2;
    if(sVc150[i]>=2000.0)sVc150[i]+=20;
    printf("Vc150(mV): (%.1f±%.1f) \t Ic150(mA): (%.4f±%.4f) \n",Vc150[i],sVc150[i],Ic150[i],sIc150[i]);

  }

  float sIc200[n200];
  float sVc200[n200];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n200;i++){
    sIc200[i] = 0.005*Ic200[i];
    if(sIc200[i]<10.0)sIc200[i]+=0.02;
    if(sIc200[i]>=10.0 && sIc200[i]<200.0)sIc200[i]+=0.005;
    if(sIc200[i]>=200.0)sIc200[i]+=(0.1+0.005*Ic200[i]);
    sVc200[i] = 0.005*Vc200[i];
    if(sVc200[i]<200.0)sVc200[i]+=0.2;
    if(sVc200[i]>=200.0 && sVc200[i]<2000.0)sVc200[i]+=2;
    if(sVc200[i]>=2000.0)sVc200[i]+=20;
    printf("Vc200(mV): (%.1f±%.1f) \t Ic200(mA): (%.4f±%.4f) \n",Vc200[i],sVc200[i],Ic200[i],sIc200[i]);

  }
  float sIc250[n250];
  float sVc250[n250];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n250;i++){
    sIc250[i] = 0.005*Ic250[i];
    if(sIc250[i]<10.0)sIc250[i]+=0.02;
    if(sIc250[i]>=10.0 && sIc250[i]<250.0)sIc250[i]+=0.005;
    if(sIc250[i]>=250.0)sIc250[i]+=(0.1+0.005*Ic250[i]);
    sVc250[i] = 0.005*Vc250[i];
    if(sVc250[i]<200.0)sVc250[i]+=0.2;
    if(sVc250[i]>=200.0 && sVc250[i]<2000.0)sVc250[i]+=2;
    if(sVc250[i]>=2000.0)sVc250[i]+=20;
    printf("Vc250(mV): (%.1f±%.1f) \t Ic250(mA): (%.4f±%.4f) \n",Vc250[i],sVc250[i],Ic250[i],sIc250[i]);

  }
  float sIc300[n300];
  float sVc300[n300];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n300;i++){
    sIc300[i] = 0.005*Ic300[i];
    if(sIc300[i]<10.0)sIc300[i]+=0.02;
    if(sIc300[i]>=10.0 && sIc300[i]<300.0)sIc300[i]+=0.005;
    if(sIc300[i]>=300.0)sIc300[i]+=(0.1+0.005*Ic300[i]);
    sVc300[i] = 0.005*Vc300[i];
    if(sVc300[i]<200.0)sVc300[i]+=0.2;
    if(sVc300[i]>=200.0 && sVc300[i]<2000.0)sVc300[i]+=2;
    if(sVc300[i]>=2000.0)sVc300[i]+=20;
    printf("Vc300(mV): (%.1f±%.1f) \t Ic300(mA): (%.4f±%.4f) \n",Vc300[i],sVc300[i],Ic300[i],sIc300[i]);

  }
  float sIc350[n350];
  float sVc350[n350];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n350;i++){
    sIc350[i] = 0.005*Ic350[i];
    if(sIc350[i]<10.0)sIc350[i]+=0.02;
    if(sIc350[i]>=10.0 && sIc350[i]<350.0)sIc350[i]+=0.005;
    if(sIc350[i]>=350.0)sIc350[i]+=(0.1+0.005*Ic350[i]);
    sVc350[i] = 0.005*Vc350[i];
    if(sVc350[i]<200.0)sVc350[i]+=0.2;
    if(sVc350[i]>=200.0 && sVc350[i]<2000.0)sVc350[i]+=2;
    if(sVc350[i]>=2000.0)sVc350[i]+=20;
    printf("Vc350(mV): (%.1f±%.1f) \t Ic350(mA): (%.4f±%.4f) \n",Vc350[i],sVc350[i],Ic350[i],sIc350[i]);

  }
  float sIc400[n400];
  float sVc400[n400];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n400;i++){
    sIc400[i] = 0.005*Ic400[i];
    if(sIc400[i]<10.0)sIc400[i]+=0.02;
    if(sIc400[i]>=10.0 && sIc400[i]<400.0)sIc400[i]+=0.005;
    if(sIc400[i]>=400.0)sIc400[i]+=(0.1+0.005*Ic400[i]);
    sVc400[i] = 0.005*Vc400[i];
    if(sVc400[i]<200.0)sVc400[i]+=0.2;
    if(sVc400[i]>=200.0 && sVc400[i]<2000.0)sVc400[i]+=2;
    if(sVc400[i]>=2000.0)sVc400[i]+=20;
    printf("Vc400(mV): (%.1f±%.1f) \t Ic400(mA): (%.4f±%.4f) \n",Vc400[i],sVc400[i],Ic400[i],sIc400[i]);

  }
    // --------------------- Grafico I(V)  ------------------------------ //

  TCanvas *cav = new TCanvas("I(V)","I(V)",200,10,600,400);
  cav->SetFillColor(0);
  cav->cd();
  TGraphErrors *gav400 = new TGraphErrors(n400,Vc400,Ic400,sVc400,sIc400);
  gav400->SetMarkerSize(0.6);
  gav400->SetMarkerStyle(21);
  // Facile, titolo del grafico
  gav400->SetTitle("I(V)");
  // Titoli degli assi
  gav400->GetXaxis()->SetTitle("Vc [mV]");
  gav400->GetYaxis()->SetTitle("Ic [mA]");
  gav400->Draw("AP");

  TGraphErrors *gav100 = new TGraphErrors(n100,Vc100,Ic100,sVc100,sIc100);
  gav100->SetMarkerSize(0.6);
  gav100->SetMarkerStyle(21);
  gav100->Draw("P+");

  TGraphErrors *gav150 = new TGraphErrors(n150,Vc150,Ic150,sVc150,sIc150);
  gav150->SetMarkerSize(0.6);
  gav150->SetMarkerStyle(21);
  gav150->Draw("P+");
  TGraphErrors *gav200 = new TGraphErrors(n200,Vc200,Ic200,sVc200,sIc200);
  gav200->SetMarkerSize(0.6);
  gav200->SetMarkerStyle(21);
  gav200->Draw("P+");
  TGraphErrors *gav250 = new TGraphErrors(n250,Vc250,Ic250,sVc250,sIc250);
  gav250->SetMarkerSize(0.6);
  gav250->SetMarkerStyle(21);
  gav250->Draw("P+");
  TGraphErrors *gav300 = new TGraphErrors(n300,Vc300,Ic300,sVc300,sIc300);
  gav300->SetMarkerSize(0.6);
  gav300->SetMarkerStyle(21);
  gav300->Draw("P+");
  TGraphErrors *gav350 = new TGraphErrors(n350,Vc350,Ic350,sVc350,sIc350);
  gav350->SetMarkerSize(0.6);
  gav350->SetMarkerStyle(21);
  gav350->Draw("P+");
  // --------------------------------- MAGICAL FIT TIME -----------------------------------------
// Fit Early
  cout << "\n\n --- Fit 100uA \n" <<endl;
  TF1 *fit100 = new TF1("fit100","[0]*pow(e,[1])*(1+[2]*x)",500,10500);
  fit100->SetParameter(0,1e-6);
  fit100->SetParameter(1,4);
  fit100->SetParameter(2,1e-4);
  gav100->Fit(fit100,"RM+");
  cout << "Chi^2:" << fit100->GetChisquare() << ", number of DoF: " << fit100->GetNDF() << " (Probability: " << fit100->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

  cout << "\n\n --- Fit 150uA \n" <<endl;
  TF1 *fit150 = new TF1("fit150","[0]*pow(e,[1])*(1+[2]*x)",500,10500);
  fit150->SetParameter(0,1e-6);
  fit150->SetParameter(1,6);
  fit150->SetParameter(2,1e-4);
  gav150->Fit(fit150,"RM+");
  cout << "Chi^2:" << fit150->GetChisquare() << ", number of DoF: " << fit150->GetNDF() << " (Probability: " << fit150->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

  cout << "\n\n --- Fit 200uA \n" <<endl;
  TF1 *fit200 = new TF1("fit200","[0]*pow(e,[1])*(1+[2]*x)",500,10500);
  fit200->SetParameter(0,1e-6);
  fit200->SetParameter(1,8);
  fit200->SetParameter(2,1e-4);
  gav200->Fit(fit200,"RM+");
  cout << "Chi^2:" << fit200->GetChisquare() << ", number of DoF: " << fit200->GetNDF() << " (Probability: " << fit200->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

  cout << "\n\n --- Fit 250uA \n" <<endl;
  TF1 *fit250 = new TF1("fit250","[0]*pow(e,[1])*(1+[2]*x)",500,10500);
  fit250->SetParameter(0,1e-6);
  fit250->SetParameter(1,10);
  fit250->SetParameter(2,1e-4);
  gav250->Fit(fit250,"RM+");
  cout << "Chi^2:" << fit250->GetChisquare() << ", number of DoF: " << fit250->GetNDF() << " (Probability: " << fit250->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

  cout << "\n\n --- Fit 300uA \n" <<endl;
  TF1 *fit300 = new TF1("fit300","[0]*pow(e,[1])*(1+[2]*x)",500,10500);
  fit300->SetParameter(0,1e-6);
  fit300->SetParameter(1,12);
  fit300->SetParameter(2,1e-4);
  gav300->Fit(fit300,"RM+");
  cout << "Chi^2:" << fit300->GetChisquare() << ", number of DoF: " << fit300->GetNDF() << " (Probability: " << fit300->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

  cout << "\n\n --- Fit 350uA \n" <<endl;
  TF1 *fit350 = new TF1("fit350","[0]*pow(e,[1])*(1+[2]*x)",500,10500);
  fit350->SetParameter(0,1e-6);
  fit350->SetParameter(1,14);
  fit350->SetParameter(2,1e-4);
  gav350->Fit(fit350,"RM+");
  cout << "Chi^2:" << fit350->GetChisquare() << ", number of DoF: " << fit350->GetNDF() << " (Probability: " << fit350->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

  cout << "\n\n --- Fit 400uA \n" <<endl;
  TF1 *fit400 = new TF1("fit400","[0]*pow(e,[1])*(1+[2]*x)",500,10500);
  fit400->SetParameter(0,1e-6);
  fit400->SetParameter(1,16);
  fit400->SetParameter(2,1e-4);
  gav400->Fit(fit400,"RM+");
  cout << "Chi^2:" << fit400->GetChisquare() << ", number of DoF: " << fit400->GetNDF() << " (Probability: " << fit400->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;


}
