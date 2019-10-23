void g4e_beamline() {

    gROOT->ForceStyle();
    gStyle->SetHatchesLineWidth(2);
    TCanvas *myGEOM = new TCanvas("myGEOM", "myGEOM", 1000, 10, 1600, 600);
    myGEOM->Divide(6, 2);
    // myETA->Divide(2,1);
    gStyle->SetOptStat(1);
    gStyle->SetOptTitle(1);

    TFile *f1 = new TFile(" jleic2019_hepmc1GeV_Si_d0:0.0mm_rnancm_m0.root");
    //TFile *f1 = new TFile("test.root");
    f1->ls();

    myGEOM->cd(1); TH2D *h1 = (TH2D *) f1->Get("d2_hist4_iBDS1a_in"); h1->Draw("colz");

    TEllipse *el1 = new TEllipse(-249.88,0.,500.,500.); el1->SetFillStyle(0); el1->SetLineColor(3); el1->Draw();
    myGEOM->cd(7); TH2D *h2 = (TH2D *) f1->Get("d2_hist24_iBDS1a_out"); h2->Draw("colz");
    TEllipse *el2 = new TEllipse(-327.11,0.,500.,500.); el2->SetFillStyle(0); el2->SetLineColor(3); el2->Draw();
   // myGEOM->cd(2); TH2D *h2 = (TH2D *) f1->Get("d2_pos_iBDS1b_in"); h2->Draw("colz");

    myGEOM->cd(2); TH2D *h3 = (TH2D *) f1->Get("d2_hist8_iQDS1a_in"); h3->Draw("colz");
    TEllipse *el3 = new TEllipse(-353.6,0.,92.,92.); el3->SetFillStyle(0); el3->SetLineColor(2); el3->Draw();
    myGEOM->cd(8); TH2D *h4 = (TH2D *) f1->Get("d2_hist28_iQDS1a_out"); h4->Draw("colz");
   TEllipse *el4 = new TEllipse(-472.79,0.,92.,92.); el4->SetFillStyle(0); el4->SetLineColor(2); el4->Draw();
  //  myGEOM->cd(4); TH2D *h4 = (TH2D *) f1->Get("d2_pos_iQDS1S_in"); h4->Draw("colz");

    myGEOM->cd(3); TH2D *h5 = (TH2D *) f1->Get("d2_hist10_iQDS1b_in"); h5->Draw("colz");
    TEllipse *el5 = new TEllipse(-520.47,0.,130.,130.); el5->SetFillStyle(0); el5->SetLineColor(2); el5->Draw();
   myGEOM->cd(9); TH2D *h6 = (TH2D *) f1->Get("d2_hist30_iQDS1b_out"); h6->Draw("colz");
     TEllipse *el6 = new TEllipse(-639.67,0.,130.,130.); el6->SetFillStyle(0); el6->SetLineColor(2); el6->Draw();
  // myGEOM->cd(6); TH2D *h6 = (TH2D *) f1->Get("d2_pos_iQDS2S_in"); h6->Draw("colz");

    myGEOM->cd(4); TH2D *h7 = (TH2D *) f1->Get("d2_hist12_iQDS2_in");  h7->Draw("colz");
    TEllipse *el7 = new TEllipse(-687.34,0.,190.,190.); el7->SetFillStyle(0); el7->SetLineColor(2); el7->Draw();
   myGEOM->cd(10); TH2D *h8 = (TH2D *) f1->Get("d2_hist32_iQDS2_out");  h8->Draw("colz");
    TEllipse *el8 = new TEllipse(-925.73,0.,190.,190.); el8->SetFillStyle(0); el8->SetLineColor(2); el8->Draw();
    //myGEOM->cd(8); TH2D *h8 = (TH2D *) f1->Get("d2_pos_iQDS3S_in"); h8->Draw("colz");


    myGEOM->cd(5); TH2D *h9 = (TH2D *) f1->Get("d2_hist6_iBDS2_in");  h9->Draw("colz");
    TEllipse *el9 = new TEllipse(-1137.63,0.,400.,400.); el9->SetFillStyle(0); el9->SetLineColor(3); el9->Draw();
     myGEOM->cd(11); TH2D *h12 = (TH2D *) f1->Get("d2_hist26_iBDS2_out");  h12->Draw("colz");
    TEllipse *el10 = new TEllipse(-1349.58,0.,400.,400.); el10->SetFillStyle(0); el10->SetLineColor(3); el10->Draw();

    myGEOM->cd(6);TH2D *h10 = (TH2D *) f1->Get("d2_hist7_iBDS3_in"); h10->Draw("colz");
    TEllipse *el11 = new TEllipse(-1349.58,0.,45.,45.); el11->SetFillStyle(0); el11->SetLineColor(3); el11->Draw();
  myGEOM->cd(12);TH2D *h13 = (TH2D *) f1->Get("d2_hist27_iBDS3_out"); h13->Draw("colz");
    TEllipse *el12 = new TEllipse(-1521.79,0.,45.,45.); el12->SetFillStyle(0); el12->SetLineColor(3); el12->Draw();

   // myGEOM->cd(11);TH2D *h11 = (TH2D *) f1->Get("d2_pos_iQDS4_in"); h11->Draw("colz");
  //  myGEOM->cd(15);TH2D *h14 = (TH2D *) f1->Get("d2_pos_iQDS4_out"); h14->Draw("colz");

    //    TH2D *h15 = (TH2D*)f1->Get("d2_pos_iQDS3S_in");
    //    h15->Draw("colz");

}
