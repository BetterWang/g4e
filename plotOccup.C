
void plotOccup(){

  gStyle->SetOptStat(0);
  char myname[50];
 

  //     TFile *f1 = TFile::Open("msel5_100x10php.root");
  //    TFile *f2 = TFile::Open("msel5_100x10dis100.root");
  //  TFile *f3 = TFile::Open("msel5_100x10dis1000.root");

  //  TFile *f1 = TFile::Open("msel2_100x10php.root");
  // TFile *f2 = TFile::Open("msel2_100x10dis100.root");
  //TFile *f3 = TFile::Open("msel2_100x10dis1000.root");


  //  TFile *f1 = TFile::Open("msel4_100x10php.root");
  TFile *f1 = TFile::Open("vertex16_e-5GeV_Si_d0:0.0mm_r0cm_m1.root");
  TCanvas *bb = new TCanvas("bb","bb",1000,500);
  bb->Divide(4,3);
  
  for (int i=0;i<12;i++)  {
    sprintf(myname,"hmatrixOccup%d",i);
    TH2F *occup = (TH2F*)f1->Get(myname);
   bb->cd(i+1);
   occup->Draw("colz");

  }



 
 

  bb->Print("vtx_occup.pdf");

  return;
}
