  TGraph *fromKevToPE;
  TGraph *fromPEToKev;



void addPE(TH1 *h) {doit(h,0);}
void addKev(TH1 *h) {doit(h,1);}

void test() {
  printf ("Welcome ! \n \t If you have an x-axis in PE and need Kev do addKev(h) \n");
  printf ("    \t If you have an x-axis in KeV and need PE do addPE(h) \n");  
  TCanvas *c1=new TCanvas("c1","c1",800,400);
  c1->Divide(2,1);
  c1->cd(1);
  TH2D *h=new TH2D("h","h;S1[PE]; S2",100,0,30,100,-1,1);
  TF2 *fh2=new TF2("fh2","Gaus(x,30,10)*Gaus(y,0,0.2)");
  h->FillRandom("fh2",10000);
  gPad->Update();
  addKev(h);
  c1->cd(2);

  TH1D *h1=new TH1D("h1","h1;S1[PE]",100,0,30);
  TF1 *fh1=new TF1("fh1","Gaus(x,30,10)");
  h1->FillRandom("fh1",10000);
  h1->Draw("");
  gPad->Update();
  addKev(h1);
}


void doit(TH1 *h, int mode) {
  gStyle->SetOptStat(0);
  gPad->SetTicks(0,0);

  // Set up the Kev to PE, and the PE to Kev translation
  TFile *fL=TFile::Open("Leff.root");
  TGraph *LeffModelMean=LeffModelMean=(TGraph*) fL->Get("Leff");
  fromKevToPE=new TGraph();
  fromPEToKev=new TGraph();
  fromKevToPE->SetTitle("from KeV to PE; KeV; PE");
  fromPEToKev->SetTitle("from KeV to PE; PE;KeV");
  double Snr = 0.95;
  double See = 0.58;
  double Ly = 2.28 ; 
  for (double ekev=0; ekev<200; ekev+=0.1) {
    double  epe = LeffModelMean->Eval(ekev)*ekev*Snr/See*Ly ;    
    fromKevToPE->SetPoint(fromKevToPE->GetN(),ekev,epe);
    fromPEToKev->SetPoint(fromPEToKev->GetN(),epe,ekev);
  }

 
  TF1 *ftoPE = new TF1("KevtoPE",fFromKevToPE,0,100,0); ftoPE->SetTitle("From Kev to PE;KeV;PE");
  TF1 *ftoKeV = new TF1("PEtoKev",fFromPEToKev,0,100,0); ftoKeV->SetTitle("From PE to KeV; PE; KeV");
  h->Draw();
  double ymax=gPad->GetUymax();

     if (mode==1) { // input in PE, translate to KeV 
     double lowPE=h->GetXaxis()->GetBinLowEdge(1);
     double highPE=h->GetXaxis()->GetBinUpEdge(h->GetNbinsX());
     double lowKeV=fromPEToKev->Eval(lowPE);
     double highKeV=fromPEToKev->Eval(highPE);
     ftoPE->SetRange(lowKeV,highKeV);
     TGaxis *Ape = new TGaxis(lowPE,ymax,highPE,ymax,"KevtoPE",510,"-L");
     Ape->SetTitle("KeV");
     }
     if (mode==0) { // input in kev, translate to PE 
     double lowKev=h->GetXaxis()->GetBinLowEdge(1);
     double highKev=h->GetXaxis()->GetBinUpEdge(h->GetNbinsX());
     double lowPE= fromKevToPE->Eval(lowKev);
                   
     double highPE=fromKevToPE->Eval(highKev);
       ftoKeV->SetRange(lowPE,highPE);
       TGaxis *Ape = new TGaxis(lowKev,ymax,highKev,ymax,"PEtoKev",510,"-L");
       Ape->SetTitle("PE");
     }

   Ape->Draw();
     return;

}


Double_t fFromKevToPE(Double_t *x, Double_t *par)
   { Float_t xx =x[0];      return fromKevToPE->Eval(xx);}
Double_t fFromPEToKev(Double_t *x, Double_t *par)
   { Float_t xx =x[0];      return fromPEToKev->Eval(xx);}
