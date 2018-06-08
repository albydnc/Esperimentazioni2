#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TLegend.h>
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
using namespace std;
// Corpo del programma. La funzione qui sotto deve avere lo stesso nome del file .C
void spettro(){
  int n = 7;
  float v[] = {1.6187,1.6192,1.6209,1.6223,1.6253,1.6433,1.6524};
  float nv[] = {620.5,614.8,594.8,577.6,546.1,433.9,400.3};
  /*int n = 4;
  float v[] = {1.6131,1.6201,1.6218,1.6306};
  float nv[] = {692.7,585.2,565.2,493.4};*/
  float ev[n],env[n];
  for(int i = 0; i<n; i++) ev[i]=0.0004,env[i]=1;

  TCanvas *cf = new TCanvas("malus","malus",200,10,1200,800);
    cf->SetFillColor(0);
    cf->cd();
    TGraphErrors *gf = new TGraphErrors(n,nv,v,env,ev);
    gf->SetMarkerSize(0.6);
    gf->SetMarkerStyle(21);
    // Facile, titolo del grafico
    gf->SetTitle("Indice Rifrazione (Hg)");
    // Titoli degli assi
    gf->GetXaxis()->SetTitle("#lambda [nm]");
    gf->GetYaxis()->SetTitle("n");
    gf->Draw("AP");
    cout << "\n\n --- Fit --- \n" <<endl;
    TF1 *fit1 = new TF1("fit","[0]/(x^2)+[1]",390,710);
    //fit1->SetParameter(0,1);
    //fit1->SetParameter(1,-2e2);
    fit1->SetLineColor(14);
    gf->Fit(fit1,"RM+");
    cout << "Chi^2:" << fit1->GetChisquare() << ", number of DoF: " << fit1->GetNDF() << " (Probability: " << fit1->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

}
