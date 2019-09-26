#define g4eoccup_cxx
#include "g4eoccup.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void g4eoccup::Loop()
{
//   In a ROOT session, you can do:
//      root> .L g4eoccup.C
//      root> g4eoccup t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
 TCanvas *bb = new TCanvas("bb","bb",1000,500);
 TH2F *ffi_ZDC_xy;
 ffi_ZDC_xy   = new TH2F("ZDC_hside","ZDC_hside",300, -3000 , 3000, 300, -3000. , 3000.);

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
               printf("my ZDC %f , %f \n",  hit_x->at( jentry), hit_y->at( jentry));
    //      ffi_ZDC_xy->Fill( hit_x->at(i), hit_y->at(i));

      // if (Cut(ientry) < 0) continue;
   }

   ffi_ZDC_xy->Draw("colz");
}
