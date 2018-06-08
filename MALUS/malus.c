#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TLegend.h>
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
using namespace std;
// Corpo del programma. La funzione qui sotto deve avere lo stesso nome del file .C
void malus(){
  int n = 36;
  //float dati[] = {1.75,2.22,2.50,2.79,2.84,2.72,2.55,2.09,1.63,1.10,0.70,0.33,0.11,0.03,0.11,0.33,0.69,1.12,1.59,2.00,2.40,2.64,2.74,2.65,2.40,2.02,1.71,1.14,0.75,0.33,0.11,0.03,0.12,0.39,0.77,1.25};
  float dati[] = {1.16,1.13,1.00,0.84,0.69,0.49,0.28,0.14,0.04,0.02,0.06,0.17,0.31,0.52,0.69,0.86,1.00,1.13,1.19,1.09,0.98,0.87,0.70,0.47,0.28,0.13,0.04,0.01,0.06,0.17,0.33,0.52,0.73,0.91,1.04,1.17};
  float sd[n],sa[n],a[n];
  for(int i=0; i<n;i++){
      sd[i]=0.01*dati[i]+0.01;
      a[i]=i*3.14/18;
      sa[i]=3.14/90;
      printf("angolo: (%.0f±%.0f) \t : (%.2f±%.2f) \n",a[i],sa[i],dati[i],sd[i]);
  }
    TCanvas *cf = new TCanvas("malus","malus",200,10,600,400);
    cf->SetFillColor(0);
    cf->cd();
    TGraphErrors *gf = new TGraphErrors(n,a,dati,sa,sd);
    gf->SetMarkerSize(0.6);
    gf->SetMarkerStyle(21);
    // Facile, titolo del grafico
    gf->SetTitle("Malus 2");
    // Titoli degli assi
    gf->GetXaxis()->SetTitle("angolo (rad)");
    gf->GetYaxis()->SetTitle("Intensità (A)");
    gf->Draw("AP");
    cout << "\n\n --- Fit --- \n" <<endl;
    TF1 *fit1 = new TF1("fit","[0]*cos(x+[1])*cos(x+[1])",0,6.3);
    fit1->SetParameter(0,3);
    fit1->SetParameter(1,0.7);
    fit1->SetLineColor(14);
    gf->Fit(fit1,"RM+");
    cout << "Chi^2:" << fit1->GetChisquare() << ", number of DoF: " << fit1->GetNDF() << " (Probability: " << fit1->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

}
