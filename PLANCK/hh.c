#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TLegend.h>
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
using namespace std;
// Corpo del programma. La funzione qui sotto deve avere lo stesso nome del file .C
void hh(){
  int n = 5;
  float v[] = {602,341,308,264,156};
  float ev[] = {17,11,11,8,4};
  float nv[] = {704,544,537,496,468};
  float env[] = {8,5,5,4,4};

    TCanvas *cf = new TCanvas("malus","malus",200,10,600,400);
    cf->SetFillColor(0);
    cf->cd();
    TGraphErrors *gf = new TGraphErrors(n,nv,v,env,ev);
    gf->SetMarkerSize(0.6);
    gf->SetMarkerStyle(21);
    // Facile, titolo del grafico
    gf->SetTitle("Planck");
    // Titoli degli assi
    gf->GetXaxis()->SetTitle("Frequenza (THz)");
    gf->GetYaxis()->SetTitle("Tensione (mV)");
    gf->Draw("AP");
    cout << "\n\n --- Fit --- \n" <<endl;
    TF1 *fit1 = new TF1("fit","[0]*x+[1]",465,710);
    fit1->SetParameter(0,4);
    fit1->SetParameter(1,-2e2);
    fit1->SetLineColor(14);
    gf->Fit(fit1,"RM+");
    cout << "Chi^2:" << fit1->GetChisquare() << ", number of DoF: " << fit1->GetNDF() << " (Probability: " << fit1->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

}
