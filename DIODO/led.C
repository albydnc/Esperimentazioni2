#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
#include <TMath.h>
using namespace std;
# define M_PI           3.14159265358979323846  /* pi */

void led(){
  float R = 2200;
  float sR = R*0.05;
  float Id[]    = {0.03,0.04,0.06,0.07,.08,.09,.10,.11,.12,.14,.16,.20,1.58,6.94,17.10,26.86,43.08,71.00,99.7,116.5,254.7,423.3,559.5,733.3,875.0,1475,1881,2341,2792,3221,3668,4095,4543,4980,5432,5861,6297,6735,7198,7530,8093}; // V
  float Vd[]    = {10.6,128.5,322.1,489.8,542.1,629,787.4,854.4,958.9,1045,1152,1227,1389,1452,1487,1504,1524,1545,1560,1567,1601,1625,1638,1651,1660,1688,1703,1717,1729,1739,1749,1757,1766,1774,1781,1788,1794,1801,1808,1813,1821};
  int n = sizeof(Id)/sizeof(*Id);
  if(n != sizeof(Vd)/sizeof(*Vd)){
    cout<<"il numero di misure non corrisponde!!!"<<endl;
    while(1);
  }
  cout << n<<endl;
  // ---------------------- Quantità derivate ----------------------- //

  // Array che conterà incertezze su Vi e Vo
  float sId[n];
  float sVd[n];
  // ciclo for (loop) sulle misure
  for(int i=0; i<n;i++){
    sId[i] = 0.005*Id[i];
    if(Id[i]<100.0)sId[i]+=0.1;
    if(Id[i]>=100.0 && Id[i]<1000.0)sId[i]+=0.5;
    if(Id[i]>=1000.0)sId[i]+=5;
    sVd[i] = 0.001*Vd[i];
    sVd[i]<1000.0?sVd[i]+=0.5:sVd[i]+=5;
    Id[i]/=1000.0;
    sId[i]/=1000.0;
    printf("Vd(mV): (%.1f±%.1f) \t Id(mA): (%.4f±%.4f) \n",Vd[i],sVd[i],Id[i],sId[i]);

  }
    // --------------------- Grafico I(V)  ------------------------------ //

  TCanvas *cav = new TCanvas("I(V)","I(V)",200,10,600,400);
  cav->SetFillColor(0);
  cav->cd();
  TGraphErrors *gav = new TGraphErrors(n,Vd,Id,sVd,sId);
  gav->SetMarkerSize(0.6);
  gav->SetMarkerStyle(21);
  // Facile, titolo del grafico
  gav->SetTitle("I(V)");
  // Titoli degli assis
  gav->GetXaxis()->SetTitle("Vd [mV]");
  gav->GetYaxis()->SetTitle("Id [mA]");
  gav->Draw("AP");
  cout << "\n\n --- Fit Eq di Shockley \n" <<endl;
  TF1 *lfit = new TF1("fit","[0]*(pow(e,x/([1]*25.25))-1)",0,1900);
  lfit->SetParameter(1,5); //scelto a random
  lfit->SetLineColor(4);
  gav->Fit(lfit,"R+");
  cout << "Chi^2:" << lfit->GetChisquare() << ", number of DoF: " << lfit->GetNDF() << " (Probability: " << lfit->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  TCanvas *cad = new TCanvas("V(I)","V(I)",200,10,600,400);
  cad->SetFillColor(0);
  cad->cd();
  cad->SetLogx();
  TGraphErrors *gad = new TGraphErrors(n,Id,Vd,sId,sVd);
  gad->SetMarkerSize(0.6);
  gad->SetMarkerStyle(21);
  // Facile, titolo del grafico
  gad->SetTitle("V(I)");
  // Titoli degli assis
  gad->GetYaxis()->SetTitle("Vd [mV]");
  gad->GetXaxis()->SetTitle("Id [mA]");
  gad->Draw("AP");
cout << "\n\n --- Fit Eq di Shockley corretta per il LED \n" <<endl;
TF1 *fit = new TF1("fit","[0]*26*log(1+[1]*x)+[2]+[3]*x",0.001,9);
fit->SetParameter(0,2);
fit->SetParameter(1,1e9);
fit->SetParameter(2,0);
fit->SetParameter(3,1);
fit->SetLineColor(4);
gad->Fit(fit,"RM+");
cout << "Chi^2:" << fit->GetChisquare() << ", number of DoF: " << fit->GetNDF() << " (Probability: " << fit->GetProb() << ")." << endl;
cout << "--------------------------------------------------------------------------------------------------------" << endl;

}
