{
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Tue Sep 17 13:44:00 2019 by ROOT version6.16/00)
//   from TTree events/a Tree with vect
//   found on file: g4e_output.root
//////////////////////////////////////////////////////////


//Reset ROOT and connect tree file
   gROOT->Reset();
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("g4e_output.root");
   if (!f) {
      f = new TFile("g4e_output.root");
   }
    f->GetObject("events",events);

//Declaration of leaves types
TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  // Declaration of leaf types
   ULong64_t       event_id;
   ULong64_t       hit_count;
   vector<unsigned long> *hit_id;
   vector<unsigned long> *hit_trk_id;
   vector<string>  *hit_vol_name;
   vector<double>  *hit_x;
   vector<double>  *hit_y;
   vector<double>  *hit_z;
   vector<unsigned long> *hit_i_rep;
   vector<unsigned long> *hit_j_rep;
   vector<double>  *hit_e_loss;
   ULong64_t       trk_count;
   vector<unsigned long> *trk_id;
   vector<unsigned long> *trk_pdg;
   vector<unsigned long> *trk_part_id;
   vector<double>  *trk_vtx_x;
   vector<double>  *trk_vtx_y;
   vector<double>  *trk_vtx_z;
   vector<double>  *trk_vtx_dir_x;
   vector<double>  *trk_vtx_dir_y;
   vector<double>  *trk_vtx_dir_z;
   vector<double>  *trk_mom;
   ULong64_t       gen_prt_count;
   vector<unsigned long> *gen_prt_id;
   vector<unsigned long> *gen_prt_vtx_id;
   vector<unsigned long> *gen_prt_pdg;
   vector<unsigned long> *gen_prt_trk_id;
   vector<double>  *gen_prt_charge;
   vector<double>  *gen_prt_dir_x;
   vector<double>  *gen_prt_dir_y;
   vector<double>  *gen_prt_dir_z;
   vector<double>  *gen_prt_tot_mom;
   vector<double>  *gen_prt_tot_e;
   vector<double>  *gen_prt_time;
   vector<double>  *gen_prt_polariz_x;
   vector<double>  *gen_prt_polariz_y;
   vector<double>  *gen_prt_polariz_z;
   ULong64_t       gen_vtx_count;
   vector<unsigned long> *gen_vtx_id;
   vector<unsigned long> *gen_vtx_part_count;
   vector<double>  *gen_vtx_x;
   vector<double>  *gen_vtx_y;
   vector<double>  *gen_vtx_z;
   vector<double>  *gen_vtx_time;
   vector<double>  *gen_vtx_weight;

   // List of branches
   TBranch        *b_event_id;   //!
   TBranch        *b_hit_count;   //!
   TBranch        *b_hit_id;   //!
   TBranch        *b_hit_trk_id;   //!
   TBranch        *b_hit_vol_name;   //!
   TBranch        *b_hit_x;   //!
   TBranch        *b_hit_y;   //!
   TBranch        *b_hit_z;   //!
   TBranch        *b_hit_i_rep;   //!
   TBranch        *b_hit_j_rep;   //!
   TBranch        *b_hit_e_loss;   //!
   TBranch        *b_trk_count;   //!
   TBranch        *b_trk_id;   //!
   TBranch        *b_trk_pdg;   //!
   TBranch        *b_trk_part_id;   //!
   TBranch        *b_trk_vtx_x;   //!
   TBranch        *b_trk_vtx_y;   //!
   TBranch        *b_trk_vtx_z;   //!
   TBranch        *b_trk_vtx_dir_x;   //!
   TBranch        *b_trk_vtx_dir_y;   //!
   TBranch        *b_trk_vtx_dir_z;   //!
   TBranch        *b_trk_mom;   //!
   TBranch        *b_gen_prt_count;   //!
   TBranch        *b_gen_prt_id;   //!
   TBranch        *b_gen_prt_vtx_id;   //!
   TBranch        *b_gen_prt_pdg;   //!
   TBranch        *b_gen_prt_trk_id;   //!
   TBranch        *b_gen_prt_charge;   //!
   TBranch        *b_gen_prt_dir_x;   //!
   TBranch        *b_gen_prt_dir_y;   //!
   TBranch        *b_gen_prt_dir_z;   //!
   TBranch        *b_gen_prt_tot_mom;   //!
   TBranch        *b_gen_prt_tot_e;   //!
   TBranch        *b_gen_prt_time;   //!
   TBranch        *b_gen_prt_polariz_x;   //!
   TBranch        *b_gen_prt_polariz_y;   //!
   TBranch        *b_gen_prt_polariz_z;   //!
   TBranch        *b_gen_vtx_count;   //!
   TBranch        *b_gen_vtx_id;   //!
   TBranch        *b_gen_vtx_part_count;   //!
   TBranch        *b_gen_vtx_x;   //!
   TBranch        *b_gen_vtx_y;   //!
   TBranch        *b_gen_vtx_z;   //!
   TBranch        *b_gen_vtx_time;   //!
   TBranch        *b_gen_vtx_weight;   //!


   fChain->SetBranchAddress("event_id", &event_id, &b_event_id);
   fChain->SetBranchAddress("hit_count", &hit_count, &b_hit_count);
   fChain->SetBranchAddress("hit_id", &hit_id, &b_hit_id);
   fChain->SetBranchAddress("hit_trk_id", &hit_trk_id, &b_hit_trk_id);
   fChain->SetBranchAddress("hit_vol_name", &hit_vol_name, &b_hit_vol_name);
   fChain->SetBranchAddress("hit_x", &hit_x, &b_hit_x);
   fChain->SetBranchAddress("hit_y", &hit_y, &b_hit_y);
   fChain->SetBranchAddress("hit_z", &hit_z, &b_hit_z);
   fChain->SetBranchAddress("hit_i_rep", &hit_i_rep, &b_hit_i_rep);
   fChain->SetBranchAddress("hit_j_rep", &hit_j_rep, &b_hit_j_rep);
   fChain->SetBranchAddress("hit_e_loss", &hit_e_loss, &b_hit_e_loss);
   fChain->SetBranchAddress("trk_count", &trk_count, &b_trk_count);
   fChain->SetBranchAddress("trk_id", &trk_id, &b_trk_id);
   fChain->SetBranchAddress("trk_pdg", &trk_pdg, &b_trk_pdg);
   fChain->SetBranchAddress("trk_part_id", &trk_part_id, &b_trk_part_id);
   fChain->SetBranchAddress("trk_vtx_x", &trk_vtx_x, &b_trk_vtx_x);
   fChain->SetBranchAddress("trk_vtx_y", &trk_vtx_y, &b_trk_vtx_y);
   fChain->SetBranchAddress("trk_vtx_z", &trk_vtx_z, &b_trk_vtx_z);
   fChain->SetBranchAddress("trk_vtx_dir_x", &trk_vtx_dir_x, &b_trk_vtx_dir_x);
   fChain->SetBranchAddress("trk_vtx_dir_y", &trk_vtx_dir_y, &b_trk_vtx_dir_y);
   fChain->SetBranchAddress("trk_vtx_dir_z", &trk_vtx_dir_z, &b_trk_vtx_dir_z);
   fChain->SetBranchAddress("trk_mom", &trk_mom, &b_trk_mom);
   fChain->SetBranchAddress("gen_prt_count", &gen_prt_count, &b_gen_prt_count);
   fChain->SetBranchAddress("gen_prt_id", &gen_prt_id, &b_gen_prt_id);
   fChain->SetBranchAddress("gen_prt_vtx_id", &gen_prt_vtx_id, &b_gen_prt_vtx_id);
   fChain->SetBranchAddress("gen_prt_pdg", &gen_prt_pdg, &b_gen_prt_pdg);
   fChain->SetBranchAddress("gen_prt_trk_id", &gen_prt_trk_id, &b_gen_prt_trk_id);
   fChain->SetBranchAddress("gen_prt_charge", &gen_prt_charge, &b_gen_prt_charge);
   fChain->SetBranchAddress("gen_prt_dir_x", &gen_prt_dir_x, &b_gen_prt_dir_x);
   fChain->SetBranchAddress("gen_prt_dir_y", &gen_prt_dir_y, &b_gen_prt_dir_y);
   fChain->SetBranchAddress("gen_prt_dir_z", &gen_prt_dir_z, &b_gen_prt_dir_z);
   fChain->SetBranchAddress("gen_prt_tot_mom", &gen_prt_tot_mom, &b_gen_prt_tot_mom);
   fChain->SetBranchAddress("gen_prt_tot_e", &gen_prt_tot_e, &b_gen_prt_tot_e);
   fChain->SetBranchAddress("gen_prt_time", &gen_prt_time, &b_gen_prt_time);
   fChain->SetBranchAddress("gen_prt_polariz_x", &gen_prt_polariz_x, &b_gen_prt_polariz_x);
   fChain->SetBranchAddress("gen_prt_polariz_y", &gen_prt_polariz_y, &b_gen_prt_polariz_y);
   fChain->SetBranchAddress("gen_prt_polariz_z", &gen_prt_polariz_z, &b_gen_prt_polariz_z);
   fChain->SetBranchAddress("gen_vtx_count", &gen_vtx_count, &b_gen_vtx_count);
   fChain->SetBranchAddress("gen_vtx_id", &gen_vtx_id, &b_gen_vtx_id);
   fChain->SetBranchAddress("gen_vtx_part_count", &gen_vtx_part_count, &b_gen_vtx_part_count);
   fChain->SetBranchAddress("gen_vtx_x", &gen_vtx_x, &b_gen_vtx_x);
   fChain->SetBranchAddress("gen_vtx_y", &gen_vtx_y, &b_gen_vtx_y);
   fChain->SetBranchAddress("gen_vtx_z", &gen_vtx_z, &b_gen_vtx_z);
   fChain->SetBranchAddress("gen_vtx_time", &gen_vtx_time, &b_gen_vtx_time);
   fChain->SetBranchAddress("gen_vtx_weight", &gen_vtx_weight, &b_gen_vtx_weight);
   Notify();


//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// events->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname
TCanvas *bb = new TCanvas("bb","bb",1000,500);
 ZDC_hside   = new TH2F("ZDC_hside","ZDC_hside",300, -3000 , 3000, 300, -3000. , 3000.);

   Long64_t nentries = events->GetEntries();

TH2F *ffi_ZDC_xy;
   Long64_t nbytes = 0;
    for (Long64_t i=0; i<nentries;i++) {
         nbytes += events->GetEntry(i);

     //   if( strcmp(hit_vol_name->at(i),"" )<0.1) {
          printf("my ZDC %f , %f \n",  hit_x->at(i), hit_y->at(i));
     //     ffi_ZDC_xy->Fill( hit_x.at(i), hit_y.at(i));
     //   }

  }

    ffi_ZDC_xy->Draw("colz");

}
