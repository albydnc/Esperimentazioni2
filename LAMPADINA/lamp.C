/*  PROGRAMMA LAMPADINA
Programma di base per l'analisi dati della caratteristica della lampadina.
Il programma va modificato per poter completare l'analisi.
In particolare, il seguente programma fa:
Grafico della curva i(V)
    --> si richiede di implementare la regressione con la curva teorica (vedi schede)
Grafico e una regressione della curva P(R)
    --> si richiede di implementare anche una regressione con una polinomiale di ordine 4

Le incertezze sulle misure sono indicative e vanno calcolate opportunamente in base al modello di tester adottato
La propagazione degli errori sulle quantità derivate va effettuata

Indicativamente, nel testo è presente dove [modifcare] il programma.

per eseguire il programma adottare uno dei due seguenti metodi alternativi:
1- da riga dei comandi (shell) digitare    root -l lampadina.C++
2- da riga dei comandi (shell) digitare    root -l    e una volta dentro a root digitare   .x lampadina.C++
*/



#include <iostream>           // ci serve per stampare a video il testo
#include <TGraphErrors.h>     // ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video 
using namespace std;
// Corpo del programma. La funzione qui sotto deve avere lo stesso nome del file .C
void lamp()
{

  // --------------------------- DATI ------------------------------- // 
  // Resistenza della lampadina, misurata in laboratorio con il multimetro
  const float R20 = 13.3; const float sR20 = 0.005*13.3 + 0.8; // ESEMPIO calcolo incertezza! Dipende dal modello!! Modificare 

  // numero misure prese
  const int nmisure = 24;

  
  // Dati presi in laboratorio: V = tensione, sV = incertezza tensione, i = intensità di corrente, si = incertezza intensità di corrente
  // consiglio di non mettere il valore per V = 0 V altrimenti si avranno problemi nella parte in cui si calcolano i logaritmi
  float V[]  = {0.3244,0.7458,1.187,1.589,2.054,2.510,3.013,3.435,3.955,4.374,4.837,5.366,5.825,6.286,6.733,7.256,7.706,8.243,8.703,9.175,9.650,10.10,10.68,11.11}; // V
  float i[]  = {11.40,18.02,23.44,27.79,32.42,36.45,40.55,43.78,47.53,50.47,53.50,56.95,59.70,62.47,65.13,68.11,70.55,73.40,75.80,78.20,80.55,82.85,85.61,87.58}; // mA
		


  // ---------------------------------------------------------------- //


  // ---------------------- Quantità derivate ----------------------- //

  // Array che conterà incertezze su i e V
  float sV[nmisure];
  float si[nmisure];
		
  // Array che conterrà i valori calcolati delle resistenze e loro incertezze
  float R[nmisure];
  float sR[nmisure];

  // Array che conterrà i valori calcolati delle potenze e loro incertezze
  float P[nmisure];
  float sP[nmisure];

  // ciclo for (loop) sulle misure
  for(int j=0; j<nmisure;++j){

    // ESEMPIO calcolo incertezza! Dipende dal modello!! [Modificare] 
    if      (V[j] < 1)  sV[j] = V[j]*0.001 + 5e-4;
    else if (V[j] < 10) sV[j] = V[j]*0.001 + 5e-3;
    else                sV[j] = V[j]*0.001 + 5e-2;  
    V[j]=abs(V[j]);
    // ESEMPIO calcolo incertezza! Dipende dal modello!! [Modificare]
    if (i[j] < 1) si[j] = i[j]*0.005 + 1e-3;
    else          si[j] = i[j]*0.005 + 5e-2;
    R[j]  = V[j]/i[j]*1000.0;  // in ohm
    V[j]==0 ? R[j]=0 : R[j]=R[j];
    
    R[j] == 0 ? sR[j]=1e-6 :sR[j] = R[j]*sqrt(pow((sV[j]/V[j]),2)+pow((si[j]/i[j]),2)); // Effettuare la propagazione degli errori
    P[j]  = V[j] * i[j];
    V[j]==0 ? P[j]=0 : P[j]+=0;

    P[j] == 0 ? sP[j]=1e-6 : sP[j] = P[j]*sqrt(pow((sV[j]/V[j]),2)+pow((si[j]/i[j]),2)); // Effettuare la propagazione degli errori
    i[j]/=1000.0;
    si[j]/=1000.0;
    // Stampa a video dei valori. \t inserisce un tab nel print out. Mettendo \n si va a capo invece
    cout << "N Misura " << j << ":\t V = (" << V[j] << " +- " << sV[j] << ") V, \t i = (" << i[j] << " +- " << si[j] << ") A,\t R = (" 
	 << setprecision(4) << R[j] << " +- " << sR[j] << ") Ohm, \t P = (" 
	 << P[j] << " +- " << sP[j] << ") mW." << endl;    
  }
  // ----------------------------------------------------------------- //



  
  // --------------------- Grafico i(V) ------------------------------ //
  // Creo il canvas (la tela) su cui disegnare il grafico. "ciV" è il il suo nome in memoria
  // è bene che sia unico dentro al programma. "i(v)" è semplicemente il titolo del canvas
  // le due coppie di numeri che seguono sono le coordinate dell'angolo superiore sinistro e le seconde
  // la dimensione in pixel lungo x e lungo y
  TCanvas *ciV = new TCanvas("ciV","i(v)",200,10,600,400);
  // Mi assicuro che la tela sia bianca (0 corrisponde al bianco, per altri colori vedi https://root.cern.ch/doc/master/classTColor.html)
  ciV->SetFillColor(0);
  // Mi assicuro che disegni su questa tela e non su un altra. Ciò è particolarmente importante quando ho tanti canvas aperti.
  ciV->cd();
  // Istanzio il grafico. Il costruttore che noi usiamo prende come argomenti:
  // il numero di misure da disegnare, l'array di misure x (=V), l'array di misure y (=i), e gli
  // array delle  rispettive incertezze
  TGraphErrors *giV = new TGraphErrors(nmisure,V,i,sV,si);
  // Nelle due righe successive disegno i punti del grafico con lo stile che più mi piace.
  // Vedi anche https://root.cern.ch/doc/master/classTAttMarker.html
  giV->SetMarkerSize(0.6);
  giV->SetMarkerStyle(21);
  // Facile, titolo del grafico
  giV->SetTitle("i(V)");
  // Titoli degli assi
  giV->GetXaxis()->SetTitle("V [V]");
  giV->GetYaxis()->SetTitle("i [A]");
  // Do istruzioni al grafico di disegnarsi sul canvas che ho selezionato preventivamente con cd()
  // Esistono diverse opzioni di disegno, vedi anche https://root.cern.ch/doc/master/classTGraphPainter.html
  // "AP" è molto semplice, gli stiamo chiedendo di disegnare gli assi (A) e i punti (P)
  giV->Draw("AP");
  
  cout << "\n\n --- Fit I(V) pow([0],1/([1]+1))*pow(x,([1]-1)/([1]+1)) \n" <<endl;
  TF1 *funz0 = new TF1("funz0","pow([0]*pow(x,([1]-1)),1/([1]+1))",0,12);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz0->SetLineColor(4); // Blu
  // Istruzione per fare il fit ai dati usando la funzione funz1 sopra definita
  // + significa che voglio aggiungere la funzione funz1 alla lista delle funzioni già disegnate sul canvas
  // R significa che voglio fare il fit solo in un determinato range, definito quando ho creato funz1. Quindi in questo caso (0-130) Ohm
  // In questo caso, mettere o non mettere l'opzione R è equivalente, visto che 0-130 copre tutto il range delle misure.
  // M dice di usare Hesse che fa una stima più accurata delle incertezze sui parametri. Nel caso di problemi rimuovere l'opzione. 
  giV->Fit(funz0,"RM+");

  cout << "Chi^2:" << funz0->GetChisquare() << ", number of DoF: " << funz0->GetNDF() << " (Probability: " << funz0->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

  // ----------------------------------------------------------------- //



  // --------------------- Grafico P(R) ------------------------------ //
  TCanvas *cPR = new TCanvas("cPR","P(R)",200,10,600,400);
  cPR->SetFillColor(0);
  cPR->cd();
  TGraphErrors *gPR = new TGraphErrors(nmisure,R,P,sR,sP);
  gPR->SetMarkerSize(0.6);
  gPR->SetMarkerStyle(21);
  gPR->SetTitle("P(R)");
  gPR->GetXaxis()->SetTitle("R [#Omega]");
  gPR->GetYaxis()->SetTitle("P [mW]");
  gPR->Draw("AP");
  // ----------------------------------------------------------------- //


  cout << "\n\n --- Ipotesi  [0]*pow(x,[1]) --- \n" <<endl;
  TF1 *funz1 = new TF1("funz1","[0]*pow(x,[1])",0,130);
  // Le tre seguenti linee servono per inizializzare i parametri della funzione.
  funz1->SetParameter(1,4);
  // Talvolta è necessario imporre che un determinato parametro sia in un intervallo di validità definito. Usare
  // questa funzione solo se con SetParameter il fit non converge o se il parametro dopo il fit esce dall'intervallo
  // con significato fisico (e.g., una temperatura assoluta < 0 K).
  funz1->SetParLimits(0,0,1);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz1->SetLineColor(1); // Blu
  // Istruzione per fare il fit ai dati usando la funzione funz1 sopra definita
  // + significa che voglio aggiungere la funzione funz1 alla lista delle funzioni già disegnate sul canvas
  // R significa che voglio fare il fit solo in un determinato range, definito quando ho creato funz1. Quindi in questo caso (0-130) Ohm
  // In questo caso, mettere o non mettere l'opzione R è equivalente, visto che 0-130 copre tutto il range delle misure.
  // M dice di usare Hesse che fa una stima più accurata delle incertezze sui parametri. Nel caso di problemi rimuovere l'opzione. 
  gPR->Fit(funz1,"RM+");

  cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;

  // Provare a fare fit a P(R) con funzione quartica: funz2 = [0]*pow(x,4) [Modificare]
  cout << "\n\n --- Ipotesi  [0]*pow(x,4) --- \n" <<endl;
  TF1 *funz2 = new TF1("funz2","[0]*pow(x,4)",0,130);
  // Le tre seguenti linee servono per inizializzare i parametri della funzione.
  funz2->SetParameter(0,pow(10,-7));
  // Talvolta è necessario imporre che un determinato parametro sia in un intervallo di validità definito. Usare
  // questa funzione solo se con SetParameter il fit non converge o se il parametro dopo il fit esce dall'intervallo
  // con significato fisico (e.g., una temperatura assoluta < 0 K).
  funz2->SetParLimits(0,0,1);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz2->SetLineColor(2); // Blu
  // Istruzione per fare il fit ai dati usando la funzione funz1 sopra definita
  // + significa che voglio aggiungere la funzione funz1 alla lista delle funzioni già disegnate sul canvas
  // R significa che voglio fare il fit solo in un determinato range, definito quando ho creato funz1. Quindi in questo caso (0-130) Ohm
  // In questo caso, mettere o non mettere l'opzione R è equivalente, visto che 0-130 copre tutto il range delle misure.
  // M dice di usare Hesse che fa una stima più accurata delle incertezze sui parametri. Nel caso di problemi rimuovere l'opzione. 
  gPR->Fit(funz2,"RM+");
  cout << "Chi^2:" << funz2->GetChisquare() << ", number of DoF: " << funz2->GetNDF() << " (Probability: " << funz2->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  // Provare a fare fit a P(R) con funzione quartica: funz3 = [0]*pow(x,4) [Modificare]
  cout << "\n\n --- Ipotesi  [0]*pow(x-[1],4) --- \n" <<endl;
  TF1 *funz3 = new TF1("funz3","[0]*pow(x-[1],4)",0,130);
  // Le tre seguenti linee servono per inizializzare i parametri della funzione.
  funz3->SetParameter(0,pow(10,-7));
  // Talvolta è necessario imporre che un determinato parametro sia in un intervallo di validità definito. Usare
  // questa funzione solo se con SetParameter il fit non converge o se il parametro dopo il fit esce dall'intervallo
  // con significato fisico (e.g., una temperatura assoluta < 0 K).
  funz3->SetParLimits(0,0,1);
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz3->SetLineColor(3); // Blu
  // Istruzione per fare il fit ai dati usando la funzione funz1 sopra definita
  // + significa che voglio aggiungere la funzione funz1 alla lista delle funzioni già disegnate sul canvas
  // R significa che voglio fare il fit solo in un determinato range, definito quando ho creato funz1. Quindi in questo caso (0-130) Ohm
  // In questo caso, mettere o non mettere l'opzione R è equivalente, visto che 0-130 copre tutto il range delle misure.
  // M dice di usare Hesse che fa una stima più accurata delle incertezze sui parametri. Nel caso di problemi rimuovere l'opzione. 
  gPR->Fit(funz3,"RM+");
  cout << "Chi^2:" << funz3->GetChisquare() << ", number of DoF: " << funz3->GetNDF() << " (Probability: " << funz3->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
// Provare a fare fit a P(R) con polinomiale di ordine 4. [Modificare]
  cout << "\n\n --- Ipotesi  [0]+x*[1]+x*x*[2]+pow(x,3)*[3]+pow(x,4)*[4] --- \n" <<endl;
  TF1 *funz4 = new TF1("funz4"," [0]+x*[1]+x*x*[2]+pow(x,3)*[3]+pow(x,4)*[4]",0,130);
  // Le tre seguenti linee servono per inizializzare i parametri della funzione.
  // Talvolta è necessario imporre che un determinato parametro sia in un intervallo di validità definito. Usare
  // questa funzione solo se con SetParameter il fit non converge o se il parametro dopo il fit esce dall'intervallo
  // con significato fisico (e.g., una temperatura assoluta < 0 K).
  // cambio colore alla linea della funzione in modo da distinguerla dalla polinomiale di ordine 4 quando la andrò a disegnare
  funz4->SetLineColor(4); // Blu
  // Istruzione per fare il fit ai dati usando la funzione funz1 sopra definita
  // + significa che voglio aggiungere la funzione funz1 alla lista delle funzioni già disegnate sul canvas
  // R significa che voglio fare il fit solo in un determinato range, definito quando ho creato funz1. Quindi in questo caso (0-130) Ohm
  // In questo caso, mettere o non mettere l'opzione R è equivalente, visto che 0-130 copre tutto il range delle misure.
  // M dice di usare Hesse che fa una stima più accurata delle incertezze sui parametri. Nel caso di problemi rimuovere l'opzione. 
  gPR->Fit(funz4,"R+");
  cout << "Chi^2:" << funz4->GetChisquare() << ", number of DoF: " << funz4->GetNDF() << " (Probability: " << funz4->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
}

