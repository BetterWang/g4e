void g4e_beamline() {

    gROOT->ForceStyle();
    gStyle->SetHatchesLineWidth(2);
    TCanvas *myGEOM = new TCanvas("myGEOM", "myGEOM", 1000, 10, 1600, 600);
    myGEOM->Divide(6, 2);
    // myETA->Divide(2,1);
    gStyle->SetOptStat(1);
    gStyle->SetOptTitle(1);

    TFile *f1 = new TFile("jleic2019_hepmc1GeV_Si_d0:0.0mm_rnancm_m0.root");
    f1->ls();

    myGEOM->cd(1); TH2D *h1 = (TH2D *) f1->Get("d2_pos_iBDS1a_in"); h1->Draw("colz");
    myGEOM->cd(7); TH2D *h2 = (TH2D *) f1->Get("d2_pos_iBDS1a_out"); h2->Draw("colz");
   // myGEOM->cd(2); TH2D *h2 = (TH2D *) f1->Get("d2_pos_iBDS1b_in"); h2->Draw("colz");
    myGEOM->cd(2); TH2D *h3 = (TH2D *) f1->Get("d2_pos_iQDS1a_in"); h3->Draw("colz");
    myGEOM->cd(8); TH2D *h4 = (TH2D *) f1->Get("d2_pos_iQDS1a_out"); h4->Draw("colz");
  //  myGEOM->cd(4); TH2D *h4 = (TH2D *) f1->Get("d2_pos_iQDS1S_in"); h4->Draw("colz");
    myGEOM->cd(3); TH2D *h5 = (TH2D *) f1->Get("d2_pos_iQDS1b_in"); h5->Draw("colz");
    myGEOM->cd(9); TH2D *h6 = (TH2D *) f1->Get("d2_pos_iQDS1b_out"); h6->Draw("colz");
   // myGEOM->cd(6); TH2D *h6 = (TH2D *) f1->Get("d2_pos_iQDS2S_in"); h6->Draw("colz");
    myGEOM->cd(4); TH2D *h7 = (TH2D *) f1->Get("d2_pos_iQDS2_in");  h7->Draw("colz");
    myGEOM->cd(10); TH2D *h8 = (TH2D *) f1->Get("d2_pos_iQDS2_out");  h8->Draw("colz");
    //myGEOM->cd(8); TH2D *h8 = (TH2D *) f1->Get("d2_pos_iQDS3S_in"); h8->Draw("colz");


    myGEOM->cd(5); TH2D *h9 = (TH2D *) f1->Get("d2_pos_iBDS2_in");  h9->Draw("colz");
    myGEOM->cd(11); TH2D *h12 = (TH2D *) f1->Get("d2_pos_iBDS2_out");  h12->Draw("colz");

    myGEOM->cd(6);TH2D *h10 = (TH2D *) f1->Get("d2_pos_iBDS3_in"); h10->Draw("colz");
    myGEOM->cd(12);TH2D *h13 = (TH2D *) f1->Get("d2_pos_iBDS3_out"); h13->Draw("colz");

   // myGEOM->cd(11);TH2D *h11 = (TH2D *) f1->Get("d2_pos_iQDS4_in"); h11->Draw("colz");
  //  myGEOM->cd(15);TH2D *h14 = (TH2D *) f1->Get("d2_pos_iQDS4_out"); h14->Draw("colz");

    //    TH2D *h15 = (TH2D*)f1->Get("d2_pos_iQDS3S_in");
    //    h15->Draw("colz");

}
