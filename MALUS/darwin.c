#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TLegend.h>
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
using namespace std;
// Corpo del programma. La funzione qui sotto deve avere lo stesso nome del file .C
void darwin(){
  int n = 10;
  float a[]= {22.48,29.61,37.70,41.09,51.62,65.45,83.64,98.25,109.37,120.69};
  float dati[] = {669,538,418,367,269,171,94,65,48,38};
  float sd[n],sa[n];
  for(int i=0; i<n;i++){
      sd[i]=5e-4*dati[i]+5;
      sa[i]=0.01;
      printf("angolo: (%.0f±%.0f) \t : (%.2f±%.2f) \n",a[i],sa[i],dati[i],sd[i]);
  }
    TCanvas *cf = new TCanvas("darwin","darwin",200,10,600,400);
    cf->SetFillColor(0);
    cf->cd();
    TGraphErrors *gf = new TGraphErrors(n,a,dati,sa,sd);
    gf->SetMarkerSize(0.6);
    gf->SetMarkerStyle(21);
    // Facile, titolo del grafico
    gf->SetTitle("Estinzione");
    // Titoli degli assi
    gf->GetXaxis()->SetTitle("x (mm)");
    gf->GetYaxis()->SetTitle("Intensita' (uA)");
    gf->Draw("AP");
    cout << "\n\n --- Fit --- \n" <<endl;
    TF1 *fit1 = new TF1("fit","[0]*pow(e,[1]*x)",20,125);
    fit1->SetParameter(0,2e3);
    fit1->SetParameter(1,-3e-2);
    fit1->SetLineColor(14);
    gf->Fit(fit1,"RM+");
    cout << "Chi^2:" << fit1->GetChisquare() << ", number of DoF: " << fit1->GetNDF() << " (Probability: " << fit1->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;
}
