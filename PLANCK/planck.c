#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TLegend.h>
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video
using namespace std;
#define lambda_430
// Corpo del programma. La funzione qui sotto deve avere lo stesso nome del file .C
void planck(){
  #ifdef lambda_430
  int n = 21;
  float v[]= {0.5,55.3,105.4,153.2,200.2,250.4,305.3,352.3,402.6,508.7,604.2,703.7,804.2,1000,1200,1400,1600,1800,2200,2600,3000};
  float I[] = {178.8,166.1,150.9,135.3,119.7,106.7,94.5,84.3,74.4,55.2,40.5,28.7,18.9,7.1,2.1,0.4,-0.3,-0.9,-1.9,-2.7,-3};
  #endif
  #ifdef lambda_524
  int n = 23;
  float v[]={0.6,28.5,52,74.7,99,128.9,155.7,175.1,202.7,250,304.6,351.6,408.3,507.9,605.1,698.2,797.1,999,1402,1818,2222,2662,2992};
  float I[]={178.5,162.5,149.6,138,125.5,112.5,101.6,94.4,84.6,70,55.8,44.8,34.1,19,9,4,2.4,1.6,0.6,-0.5,-1.2,-1.7,-2.1};
  #endif
  #ifdef lambda_562
  int n = 22;
  float v[]={0.6,52,101.4,153.2,206.7,254.5,302.2,353.3,401,504.8,602.7,700,801.9,999,1200,1404,1604,1803,1999,2206,2599,3000};
  float I[]={220,178.4,147.7,120.3,97.5,78,61.8,47.2,35,14.9,4.5,1.6,0.7,-0.5,-1.6,-2.4,-3.1,-3.7,-4,-4.5,-5.7,-5.8};
  #endif
  #ifdef lambda_612
  int n = 18;
  float v[]={0.6,51.2,104.7,151.6,203.5,259.3,305.5,350,400.9,512,612.4,822.2,1001,1414,1799,2222,2626,2992};
  float I[]={198.6,157,120,95,70,49.5,35.3,24.3,15,2.9,0.8,0,0.6,-1.7,-2.6,-3.2,-3.7,-4.4};
  #endif
  #ifdef lambda_644
  int n = 22;
  float v[]={0.9,51.2,73.9,104.6,129,157.3,174.3,202.2,227.9,251.2,276.3,303.1,400.1,501.4,613.4,804.4,1001,1414,1818,2207,2626,3000};
  float I[]={241,184.8,121.2,96.6,80,64.2,51.2,39.5,31,23.5,18.3,13.2,3.5,0.2,-0.2,-0.9,-1.6,-2.9,-4,-4.4,-5,-5.2};
  #endif
  float sv[n],si[n];
  for(int i=0; i<n;i++){
      sv[i]=0.001*v[i];
      v[i]<1000.0? v[i]+=0.5:v[i]+=5;
      si[i]=0.004*I[i]+1;
      printf("v: (%.2f±%.2f) \t i: (%.2f±%.2f) \n",v[i],sv[i],I[i],si[i]);
  }
    TCanvas *cf = new TCanvas("planck","planck",200,10,600,400);
    cf->SetFillColor(0);
    cf->cd();
    TGraphErrors *gf = new TGraphErrors(n,v,I,sv,si);
    gf->SetMarkerSize(0.6);
    gf->SetMarkerStyle(21);
    // Facile, titolo del grafico
    gf->SetTitle("Planck 644 nm");
    // Titoli degli assi
    gf->GetXaxis()->SetTitle("mV");
    gf->GetYaxis()->SetTitle("pA");
    gf->Draw("AP");
    cout << "\n\n --- Fit --- \n" <<endl;
    TF1 *fit1 = new TF1("fit","[0]*x+[1]",1300,3100);
    fit1->SetParameter(0,0);
    fit1->SetParameter(1,0);
    fit1->SetLineColor(14);
    gf->Fit(fit1,"RM+");
    cout << "Chi^2:" << fit1->GetChisquare() << ", number of DoF: " << fit1->GetNDF() << " (Probability: " << fit1->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;
    TF1 *fit2 = new TF1("fit","[0]*x+[1]",0,280);
    fit2->SetParameter(0,-1);
    fit2->SetParameter(1,2e2);
    fit2->SetLineColor(14);
    gf->Fit(fit2,"RM+");
    cout << "Chi^2:" << fit2->GetChisquare() << ", number of DoF: " << fit2->GetNDF() << " (Probability: " << fit2->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;
    double m = fit1->GetParameter(0);
    double em = fit1->GetParError(0);
    double q = fit1->GetParameter(1);
    double eq = fit1->GetParError(1);
    double m1 = fit2->GetParameter(0);
    double em1 = fit2->GetParError(0);
    double q1 = fit2->GetParameter(1);
    double eq1 = fit2->GetParError(1);
    double x = (fit1->GetParameter(1)-fit2->GetParameter(1))/(fit2->GetParameter(0)-fit1->GetParameter(0));
    double y = m*x+q;
    double ev=sqrt((pow(eq,2)+pow(eq1,2))/pow(m-m1,2)+(pow(em,2)+pow(em1,2))*pow((q-q1)/pow(m-m1,2),2));
    cout << "V0: "<< x<<"+-"<<ev<<" I0: "<<y<<"\n";
    cout << "--------------------------------------------------------------------------------------------------------" << endl;

    /*TF1 *fite = new TF1("fit","[0]*pow(e,[1]*x)-[2]",0,3000);
    fite->SetParameter(0,2e2);
    //fite->SetParameter(1,2e2);
    fite->SetParameter(2,1);
    //fite->SetLineColor(0);
    gf->Fit(fite,"RM+");
    cout << "Chi^2:" << fite->GetChisquare() << ", number of DoF: " << fite->GetNDF() << " (Probability: " << fite->GetProb() << ")." << endl;
    cout << "--------------------------------------------------------------------------------------------------------" << endl;
    */
}
