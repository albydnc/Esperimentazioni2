#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
#include <TMath.h>
using namespace std;
# define M_PI           3.14159265358979323846  /* pi */

void diodo(){
  float R = 2200;
  float sR = R*0.05;
  float Id[]    = {0,0,0,0,29.4,41.1,87.1,136.1,183.2,221.1,238.1,289.6,327.9,361.7,393.5,660,860,1080,1310,1530,1740,1980,2420,2880,3320,3770,4210,4670,5140,5580,6050,6500,6920,7350,7810,8290,8730,9210,9640,10100,10520,11000}; // V
  float Vd[]    = {75.7,114.1,221.5,366.6,404,415,441,458,468,476,478,486,491,495,498,521,532,542,551,558,564,569,579,587,593,599,604,609,613,617,621,624,627,629,632,635,637,640,642,644,646,647};
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
    sId[i] = 0.15*Id[i];
    Id[i]<400.0?sId[i]+=0.3:sId[i]+=30;
    sVd[i] = 0.005*Vd[i];
    sVd[i]<400.0?sVd[i]+=0.3:sVd[i]+=3;
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
  TF1 *lfit = new TF1("fit","[0]*(pow(e,x/([1]*25.25))-1)",400,650);
  lfit->SetParameter(1,2);
  lfit->SetLineColor(14);
  gav->Fit(lfit,"RM+");
  cout << "Chi^2:" << lfit->GetChisquare() << ", number of DoF: " << lfit->GetNDF() << " (Probability: " << lfit->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

  TCanvas *cad = new TCanvas("V(I)","V(I)",200,10,600,400);
  cad->SetFillColor(0);
  cad->SetLogx();
  cad->cd();
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
TF1 *fit = new TF1("fit","[0]*26*log(1+x/[1])+[2]*x",0,11);
fit->SetParameter(0,2);
fit->SetParameter(1,1e-6);
fit->SetParameter(2,1);
fit->SetLineColor(14);
gad->Fit(fit,"RM+");
cout << "Chi^2:" << fit->GetChisquare() << ", number of DoF: " << fit->GetNDF() << " (Probability: " << fit->GetProb() << ")." << endl;
cout << "--------------------------------------------------------------------------------------------------------" << endl;

}
