#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TLegend.h>
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
using namespace std;
// Corpo del programma. La funzione qui sotto deve avere lo stesso nome del file .C
void interferometro(){
  int n = 6;
  float dati[] = {6,13,19,22,27,30};
  float a[] = {34,69,90,103,124,138};
  float sd[n],sa[n];
  for(int i=0; i<n;i++){
      sd[i]=1;
      sa[i]=7;
      printf("angolo: (%.0f±%.0f) \t : (%.2f±%.2f) \n",a[i],sa[i],dati[i],sd[i]);
  }
    TCanvas *cf = new TCanvas("malus","malus",200,10,1800,1200);
    cf->SetFillColor(0);
    cf->cd();
    TGraphErrors *gf = new TGraphErrors(n,a,dati,sa,sd);
    gf->SetMarkerSize(0.6);
    gf->SetMarkerStyle(21);
    // Facile, titolo del grafico
    gf->SetTitle("Interferometro");
    // Titoli degli assi
    gf->GetXaxis()->SetTitle("Pressione (kPa)");
    gf->GetYaxis()->SetTitle("Frange");
    gf->Draw("AP");
    cout << "\n\n --- Fit --- \n" <<endl;
    TF1 *fit1 = new TF1("fit","[0]+[1]*x",0,140);
    fit1->SetParameter(0,0);
    //fit1->SetParameter(1,2);
    fit1->SetLineColor(14);
    gf->Fit(fit1,"RM+");
    cout << "Chi^2:" << fit1->GetChisquare() << ", number of DoF: " << fit1->GetNDF() << " (Probability: " << fit1->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

}
