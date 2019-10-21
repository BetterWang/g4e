{
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Mon Oct 15 11:09:51 2018 by ROOT version6.08/02)
//   from TTree flux/Geant4 flux digitized information
//   found on file: output.root
//////////////////////////////////////////////////////////


//Reset ROOT and connect tree file
   gROOT->Reset();gStyle->SetOptStat(11);
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("output.root");
   if (!f) {
     //   f = new TFile("output.root");
     //   f = new TFile("output_VM.root");
     //   f = new TFile("output_DIS.root");
 //    f = new TFile("jpsi_ee.root");
 //    f = new TFile("BEAGLE-vm.root");
     f = new TFile("PHP2.root");
 
	 //  f = new TFile("output_DIS1.root");
   }

   TTree *flux;
   f->GetObject("flux",flux);
   
//Declaration of leaves types
    vector<double>  *hitn;
   vector<double>  *id;
   vector<double>  *pid;
   vector<double>  *mpid;
   vector<double>  *tid;
   vector<double>  *mtid;
   vector<double>  *otid;
   vector<double>  *trackE;
   vector<double>  *totEdep;
   vector<double>  *avg_x;
   vector<double>  *avg_y;
   vector<double>  *avg_z;
   vector<double>  *avg_lx;
   vector<double>  *avg_ly;
   vector<double>  *avg_lz;
   vector<double>  *px;
   vector<double>  *py;
   vector<double>  *pz;
   vector<double>  *vx;
   vector<double>  *vy;
   vector<double>  *vz;
   vector<double>  *mvx;
   vector<double>  *mvy;
   vector<double>  *mvz;
   vector<double>  *avg_t;
   vector<double>  *nsteps;
   vector<double>  *procID;
   vector<double>  *pid_true;
   // List of branches
   TBranch        *b_hitn;   //!
   TBranch        *b_id;   //!
   TBranch        *b_pid;   //!
   TBranch        *b_mpid;   //!
   TBranch        *b_tid;   //!
   TBranch        *b_mtid;   //!
   TBranch        *b_otid;   //!
   TBranch        *b_trackE;   //!
   TBranch        *b_totEdep;   //!
   TBranch        *b_avg_x;   //!
   TBranch        *b_avg_y;   //!
   TBranch        *b_avg_z;   //!
   TBranch        *b_avg_lx;   //!
   TBranch        *b_avg_ly;   //!
   TBranch        *b_avg_lz;   //!
   TBranch        *b_px;   //!
   TBranch        *b_py;   //!
   TBranch        *b_pz;   //!
   TBranch        *b_vx;   //!
   TBranch        *b_vy;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_mvx;   //!
   TBranch        *b_mvy;   //!
   TBranch        *b_mvz;   //!
   TBranch        *b_avg_t;   //!
   TBranch        *b_nsteps;   //!
   TBranch        *b_procID;   //!
   TBranch        *b_pid_true;   //!

   // Set branch addresses.
    flux->SetBranchAddress("hitn", &hitn, &b_hitn);
   flux->SetBranchAddress("id", &id, &b_id);
   flux->SetBranchAddress("pid", &pid, &b_pid);
   flux->SetBranchAddress("mpid", &mpid, &b_mpid);
   flux->SetBranchAddress("tid", &tid, &b_tid);
   flux->SetBranchAddress("mtid", &mtid, &b_mtid);
   flux->SetBranchAddress("otid", &otid, &b_otid);
   flux->SetBranchAddress("trackE", &trackE, &b_trackE);
   flux->SetBranchAddress("totEdep", &totEdep, &b_totEdep);
   flux->SetBranchAddress("avg_x", &avg_x, &b_avg_x);
   flux->SetBranchAddress("avg_y", &avg_y, &b_avg_y);
   flux->SetBranchAddress("avg_z", &avg_z, &b_avg_z);
   flux->SetBranchAddress("avg_lx", &avg_lx, &b_avg_lx);
   flux->SetBranchAddress("avg_ly", &avg_ly, &b_avg_ly);
   flux->SetBranchAddress("avg_lz", &avg_lz, &b_avg_lz);
   flux->SetBranchAddress("px", &px, &b_px);
   flux->SetBranchAddress("py", &py, &b_py);
   flux->SetBranchAddress("pz", &pz, &b_pz);
   flux->SetBranchAddress("vx", &vx, &b_vx);
   flux->SetBranchAddress("vy", &vy, &b_vy);
   flux->SetBranchAddress("vz", &vz, &b_vz);
   flux->SetBranchAddress("mvx", &mvx, &b_mvx);
   flux->SetBranchAddress("mvy", &mvy, &b_mvy);
   flux->SetBranchAddress("mvz", &mvz, &b_mvz);
   flux->SetBranchAddress("avg_t", &avg_t, &b_avg_t);
   flux->SetBranchAddress("nsteps", &nsteps, &b_nsteps);
   flux->SetBranchAddress("procID", &procID, &b_procID);

   generated->SetBranchAddress("pid", &pid_true, &b_pid_true);
 
//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// flux->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname
 TCanvas *CC = new TCanvas("CC","CC",50,0,1200,1000);
 CC->Divide(4,2);
 TCanvas *cEMCAL = new TCanvas("cEMCAL","cEMCAL",50,0,1200,600);
 cEMCAL->Divide(4,3);


 test_tab();
 TH2F *hXY_planes[10],*eXY_planes[10],*eEMCAL, *hGEMForw[10],*GEMTRD,*GEMh,*SiDisks;
 TH2F *SiDisksE,*RP2Disks,*RP1Disks,*RP3Disks,*RP4Disks,*ZDC_hside;
 TH1F *hPtv5,*hPtv6,*hPtv7; 
 TH1F *httv5,*httv6,*httv7; 
 TH1F *hPt_rp3,*htt_rp3,*hPt_rp4,*htt_rp4;
 TH1F *httsmv5,*httsmv6,*httsmv7,*httsm_rp0,*httsm_rp1,*httsm_rp2,*httsm_rp3,*httsm_rp4;
 TH1F *hPt_rp0,*htt_rp0,*hPt_rp1,*htt_rp1,*hPt_rp2,*htt_rp2;
 TH2F *hPtPtotv5, *hPtPtotv6, *hPtPtotv7;
 TH2F *hPtPtotRP1, *hPtPtotRP2, *hPtPtotRP3, *hPtPtotRP4;
 int k,myid;
 double pt,ptot;
 char abname[100],abname1[100];
 for (k=0; k<8;k++) { 
   sprintf(abname,"hXY_planes_%d",k); 
   hXY_planes[k]   = new TH2F(abname,abname,300, -3000 , 3000, 300, -3000. , 3000.);
   sprintf(abname,"eXY_planes_%d",k); 
   eXY_planes[k]   = new TH2F(abname,abname,300, -3000 , 3000, 300, -3000. , 3000.);
  sprintf(abname," hGEMForw_%d",k); 
  hGEMForw[k]   = new TH2F(abname,abname,100, -1500 , 1500, 100, -1000. , 1000.);
  }
   eEMCAL   = new TH2F("eEMCAL","eEMCAL",100, -1500 , 1500, 100, -1000. , 1000.);
   GEMTRD   = new TH2F("GEMTRD","GEMTRD",100, -1500 , 1500, 100, -1000. , 1000.);
   GEMh   = new TH2F("GEMh","GEMh",100, -1500 , 1500, 100, -1000. , 1000.);
   SiDisks   = new TH2F("SiDisks","SiDisks",100, -1500 , 1500, 100, -1000. , 1000.);
   SiDisksE   = new TH2F("SiDisksE","SiDisksE",100, -1500 , 1500, 300, -1000. , 1000.);
   RP0Disks   = new TH2F("RP0Disks","RP0Disks",300, -3000 , 3000, 300, -3000. , 3000.);
   RP1Disks   = new TH2F("RP1Disks","RP1Disks",300, -3000 , 3000, 300, -3000. , 3000.);
   RP2Disks   = new TH2F("RP2Disks","RP2Disks",300, -3000 , 3000, 300, -3000. , 3000.);
   RP3Disks   = new TH2F("RP3Disks","RP3Disks",300, -3000 , 3000, 300, -1000. , 3000.);
   RP4Disks   = new TH2F("RP4Disks","RP4Disks",300, -3000 , 3000, 300, -3000. , 3000.);
   ZDC_hside   = new TH2F("ZDC_hside","ZDC_hside",300, -3000 , 3000, 300, -3000. , 3000.);
 
   hPtPtotv5   = new TH2F("hPtPtotv5","hPtPtotv5",200, 0. ,7., 200, 0. , 100.);
   hPtPtotv6   = new TH2F("hPtPtotv6","hPtPtotv6",200, 0. ,7., 200, 0. , 100.);
   hPtPtotv7   = new TH2F("hPtPtotv7","hPtPtotv7",200, 0. ,7., 200, 0. , 100.);

   hPtPtotRP1   = new TH2F("hPtPtotRP1","hPtPtotvRP1",200, 0. ,7., 200, 0. , 100.);
   hPtPtotRP2   = new TH2F("hPtPtotRP2","hPtPtotvRP2",200, 0. ,7., 200, 0. , 100.);
   hPtPtotRP3   = new TH2F("hPtPtotRP3","hPtPtotvRP3",200, 0. ,7., 200, 0. , 100.);
   hPtPtotRP4   = new TH2F("hPtPtotRP4","hPtPtotvRP4",200, 0. ,7., 200, 0. , 100.);

   hPtv5       = new TH1F("hPtv5","hPt-VP5 ",        200, 0. ,10.);
   hPtv6       = new TH1F("hPtv6","hPt-VP6",        200, 0. ,10.);
   hPtv7       = new TH1F("hPtv7","hPt-V7",        200, 0. ,10.);


   httv5       = new TH1F("httv5","httv5",        200, 0. ,50.);
   httv6       = new TH1F("httv6","httv6",        200, 0. ,50.);
   httv7       = new TH1F("httv7","httv7",        200, 0. ,50.);

   hPt_rp0   = new TH1F("hPt_rp0","hPt_rp0",200, 0. ,10.);
   htt_rp0   = new TH1F("htt_rp0","htt_rp0",200, 0. ,50.);
   hPt_rp1   = new TH1F("hPt_rp1","hPt_rp1",200, 0. ,10.);
   htt_rp1   = new TH1F("htt_rp1","htt_rp1",200, 0. ,50.);
   hPt_rp2   = new TH1F("hPt_rp2","hPt_rp2",200, 0. ,10.);
   htt_rp2   = new TH1F("htt_rp2","htt_rp2",200, 0. ,50.);
   hPt_rp3   = new TH1F("hPt_rp3","hPt_rp3",200, 0. ,10.);
   htt_rp3   = new TH1F("htt_rp3","htt_rp3",200, 0. ,50.);
   hPt_rp4   = new TH1F("hPt_rp4","hPt_rp4",200, 0. ,10.);
   htt_rp4   = new TH1F("htt_rp4","htt_rp4",200, 0. ,50.);

   httsmv5       = new TH1F("httsmv5","httsmv5",        50, 0. ,2.);
   httsmv6       = new TH1F("httsmv6","httsmv6",        50, 0. ,2.);
   httsmv7       = new TH1F("httsmv7","httsmv7",        50, 0. ,2.);
   httsm_rp0   = new TH1F("httsm_rp0","httsm_rp0",50, 0. ,2.);
   httsm_rp1   = new TH1F("httsm_rp1","httsm_rp1",50, 0. ,2.);
   httsm_rp2   = new TH1F("httsm_rp2","httsm_rp2",50, 0. ,2.);
   httsm_rp3   = new TH1F("httsm_rp3","httsm_rp3",50, 0. ,2.);
   httsm_rp4   = new TH1F("httsm_rp4","httsm_rp4",50, 0. ,2.);
  
  
   Long64_t nentries = flux->GetEntries();
   Int_t ct,cte, i_ct, cn[10],cne[10]; 
   Long64_t nbytes = 0;
   for (Long64_t iev=0; iev<nentries;iev++) {
     //printf("-----------> iev=%lld avg_x=%lu \n",iev,avg_x->size());
     nbytes += flux->GetEntry(iev);
      //flux->Print();
      ct=0; 
      if(iev%100==1)  printf("-----------> iev=%lld  \n",iev);
      generated->GetEntry(iev);
      //    for ( int j=0; j<pid_true->size(); j++) {
      //	printf("My PID=%f \n",pid_true->at(j));
      // }
      
      for (int i=0; i<avg_x->size(); i++) {
	//       	if(  abs(pid->at(i))!=2212) {  continue; } 
	if(  abs(pid->at(i))!=2112) {  continue; } 
   
	  pt=sqrt(px->at(i)*px->at(i)+py->at(i)*py->at(i))/1000.;
          ptot=sqrt(px->at(i)*px->at(i)+py->at(i)*py->at(i)+pz->at(i)*pz->at(i))/1000.;

	//---- virtual planes around beampipe --- 	
	for(k=0;k<8;k++) { 
 
	  if(iev%100==1)  printf("-----------> iev=%lld px=%f py=%f pz=%f (pt=%f,  ptot=%f) t=%f \n",iev,px->at(i)/1000.,py->at(i)/1000.,pz->at(i)/1000.,pt,ptot, pt*pt);

            
 	  myid=11910+k;	// && pid->at(i)==2212
	  if( abs(id->at(i)- myid)<0.1 ) { cn[k]++; ct+=1; i_ct=i;   
	    hXY_planes[k]->Fill( avg_x->at(i_ct), avg_y->at(i_ct));
	    //  printf(" VP hadron side i=%i avg_x=%f id=%f  PID=%f \n",i,avg_x->at(i), id->at(i),pid->at(i));

	    if(abs(id->at(i)-11917)<0.1) {
              hPtv7->Fill(pt); httv7->Fill(pt*pt); httsmv7->Fill(pt*pt);
	      hPtPtotv7->Fill(pt,ptot);
           }
	    if(abs(id->at(i)-11916)<0.1) {
              hPtv6->Fill(pt); httv6->Fill(pt*pt); httsmv6->Fill(pt*pt);
	      hPtPtotv6->Fill(pt,ptot);
           }
	    if(abs(id->at(i)-11915)<0.1) {
              hPtv5->Fill(pt); httv5->Fill(pt*pt);httsmv5->Fill(pt*pt);
              hPtPtotv5->Fill(pt,ptot); 
            }
	  }
	  myid=11810+k;
	  
	  if( abs(id->at(i)- myid)<0.1 && pid->at(i)==11) { cne[k]++; cte+=1; i_ct=i;   
	    eXY_planes[k]->Fill( avg_x->at(i_ct), avg_y->at(i_ct));
	    //  printf("VP electron side i=%i avg_x=%f id=%f PID=%f \n",i,avg_x->at(i), id->at(i),pid->at(i));
	  }
	  myid=82000+k*100;
	  if( abs(id->at(i)-myid )<0.1) {  
	    hGEMForw[k]->Fill( avg_x->at(i), avg_y->at(i));
	  }
	}
      
	//---- EMCAL - e-side ( vitrual flux id=34000, real id=34001) --- 	
        if( abs(id->at(i)-34000 )<0.1) {  
	  eEMCAL->Fill( avg_x->at(i), avg_y->at(i));
	}
	//---- ZDC-Hside --- 	
        if( abs(id->at(i)-12010 )<0.1) {  
          printf("my ZDC %f , %f \n",  avg_x->at(i), avg_y->at(i));
	  ZDC_hside->Fill( avg_x->at(i), avg_y->at(i));
	}
	//---- GEMTRD --- 	
        if( abs(id->at(i)-72101 )<0.1) {  
	  GEMTRD->Fill( avg_x->at(i), avg_y->at(i));
	}
	//---- GEMh --- 	
        if( abs(id->at(i)-72012 )<0.1) {  
	  GEMh->Fill( avg_x->at(i), avg_y->at(i));
	}
	//---- SiDisks --- 	
        if( abs(id->at(i)-52100 )<0.1) {  
	  SiDisks->Fill( avg_x->at(i), avg_y->at(i));
	}
	//---- siDisks-Eside --- 	
        if( abs(id->at(i)-53100 )<0.1) {  
	  SiDisksE->Fill( avg_x->at(i), avg_y->at(i));
	}

	//---- RP0  Disks --- 	
        if( abs(id->at(i)-92000)<10 ) { // printf("RP1Disks \n");
	  RP0Disks->Fill( avg_x->at(i), avg_y->at(i));
	  hPt_rp0->Fill(pt); htt_rp0->Fill(pt*pt);  httsm_rp0->Fill(pt*pt);
	  // hPtPtotRP1->Fill(pt,ptot);
	}
	//---- RP1-2  Disks --- 	
        if( abs(id->at(i)-92100)<10 ) { // printf("RP1Disks \n");
	  RP1Disks->Fill( avg_x->at(i), avg_y->at(i));
	  hPt_rp1->Fill(pt); htt_rp1->Fill(pt*pt);  httsm_rp1->Fill(pt*pt);
	  hPtPtotRP1->Fill(pt,ptot);
	}
	if( abs(id->at(i)-92200)<10 ) {  //printf("RP2Disks \n");
	  RP2Disks->Fill( avg_x->at(i), avg_y->at(i)); hPt_rp2->Fill(pt); htt_rp2->Fill(pt*pt);  httsm_rp2->Fill(pt*pt);
	  hPtPtotRP2->Fill(pt,ptot);
	}
	//---- RP3-4  Disks --- 	
        if( abs(id->at(i)-92300)<10 ) {  //printf("RP3Disks \n");
	  RP3Disks->Fill( avg_x->at(i), avg_y->at(i)); hPt_rp3->Fill(pt); htt_rp3->Fill(pt*pt); httsm_rp3->Fill(pt*pt);
	  // if (abs(id->at(i)-92308)<0.1) { hPt_rp3->Fill(pt); htt_rp3->Fill(pt*pt);}
	  hPtPtotRP3->Fill(pt,ptot);
	}
	if( abs(id->at(i)-92400)<10 ) {  //printf("RP4Disks \n");
	  RP4Disks->Fill( avg_x->at(i), avg_y->at(i)); hPt_rp4->Fill(pt); htt_rp4->Fill(pt*pt);httsm_rp4->Fill(pt*pt);
	  hPtPtotRP4->Fill(pt,ptot);
	  //  if (abs(id->at(i)-92408)<0.1) { hPt_rp4->Fill(pt); htt_rp4->Fill(pt*pt);}
	}

	

       } //--- end -- hit loop
      
           
      
   }  //---- end event loop
   
   double myscat=1.;
   sprintf(abname,"scat=%f",myscat); 
   sprintf(abname1,"scat=%f,same",myscat); 
 
   for (k=0;k<8;k++) { 
    
     CC->cd(k+1);  hXY_planes[k]->SetMarkerColor(2); 
     if(k<4) { hXY_planes[k]->GetXaxis()->SetRangeUser(-1500,1500);  hXY_planes[k]->GetYaxis()->SetRangeUser(-1500,1500); }
     hXY_planes[k]->Draw(abname); 
     printf ("CN[%d]=%d \n",k,cn[k]);
 
  }
   
   //  hGEMForw[1]->SetMarkerColor(kBlue);
 
   CC->cd(6);  ZDC_hside->SetMarkerColor(1);  ZDC_hside->Draw(abname1); 
 
  CC->cd(6); RP1Disks->SetMarkerColor(5);  RP1Disks->Draw(abname1); 
  CC->cd(6); RP2Disks->SetMarkerColor(6);  RP2Disks->Draw(abname1);  
  CC->cd(6); RP0Disks->SetMarkerColor(42);  RP0Disks->Draw(abname1); 
   CC->cd(8); RP3Disks->SetMarkerColor(3);  RP3Disks->Draw(abname1); 
  CC->cd(8); RP4Disks->SetMarkerColor(4);  RP4Disks->Draw(abname1); 
  
 
  cEMCAL->cd(1);   
 THStack hs("hs"," PT");
 hPt_rp2->Scale(0.1); hPt_rp2->SetLineColor(6); hPt_rp2->SetFillColor(6);  hs.Add( hPt_rp2);
 hPt_rp1->Scale(0.1); hPt_rp1->SetLineColor(5); hPt_rp1->SetFillColor(5); hs.Add( hPt_rp1);
 hPt_rp3->Scale(0.1); hPt_rp3->SetLineColor(3); hPt_rp3->SetFillColor(3);  hs.Add( hPt_rp3);
 hPt_rp4->Scale(0.1);  hPt_rp4->SetFillColor(4); hPt_rp4->SetFillColor(4);  hs.Add(hPt_rp4);
 hPt_rp0->Scale(0.1);  hPt_rp0->SetFillColor(42); hPt_rp0->SetFillColor(42);  hs.Add(hPt_rp0);

 hPtv7->SetFillStyle(0); hPtv7->SetLineColor(1);  hs.Add(hPtv7);
 hPtv6->SetFillStyle(0); hPtv6->SetLineColor(7);   hs.Add(hPtv6);
 hPtv5->SetFillStyle(0); hPtv5->SetLineColor(2);   hs.Add(hPtv5);
 hs.Draw("hist,nostack");


  cEMCAL->cd(2); 
  THStack hs2("hs2","t");
  htt_rp3->Scale(0.1); htt_rp3->SetFillColor(3); hs2.Add(htt_rp3); //htt_rp3->Draw("same");  
  htt_rp4->Scale(0.1);  htt_rp4->SetFillColor(4);  hs2.Add(htt_rp4); //htt_rp4->Draw("same"); 
  htt_rp1->Scale(0.1); htt_rp1->SetLineColor(5); htt_rp1->SetFillColor(5);  hs2.Add( htt_rp1);
  htt_rp2->Scale(0.1); htt_rp2->SetLineColor(6); htt_rp2->SetFillColor(6);  hs2.Add( htt_rp2);
  htt_rp0->Scale(0.1); htt_rp0->SetLineColor(42); htt_rp0->SetFillColor(42);  hs2.Add( htt_rp0);

  httv7->SetFillStyle(0); httv7->SetLineColor(1);   hs2.Add(httv7); //htt->Draw("same"); 
  httv6->SetFillStyle(0); httv6->SetLineColor(7);   hs2.Add(httv6); //htt->Draw("same"); 
  httv5->SetFillStyle(0); httv5->SetLineColor(2);   hs2.Add(httv5); //htt->Draw("same"); 
 hs2.Draw("hist,nostack");
 
  cEMCAL->cd(3); 
  THStack hs3("hs2","t-small");
  httsm_rp4->Scale(0.1); httsm_rp4->SetFillColor(4); hs3.Add(httsm_rp4); //htt_rp4->Draw("same"); 
  httsm_rp3->Scale(0.1); httsm_rp3->SetFillColor(3); hs3.Add(httsm_rp3); //htt_rp3->Draw("same");  
  httsm_rp2->Scale(0.1); httsm_rp2->SetFillColor(6); hs3.Add(httsm_rp2); //htt_rp4->Draw("same"); 
  httsm_rp1->Scale(0.1); httsm_rp1->SetFillColor(5); hs3.Add(httsm_rp1); //htt_rp3->Draw("same");  
  httsm_rp0->Scale(0.1); httsm_rp0->SetFillColor(42); hs3.Add(httsm_rp0); //htt_rp3->Draw("same");  
  httsmv7->SetFillStyle(0); httsmv7->SetLineColor(1);  hs3.Add(httsmv7); //htt->Draw("same"); 
  httsmv6->SetFillStyle(0); httsmv6->SetLineColor(7);  hs3.Add(httsmv6); //htt->Draw("same"); 
  httsmv5->SetFillStyle(0); httsmv5->SetLineColor(2);  hs3.Add(httsmv5); //htt->Draw("same"); 

  hs3.Draw("hist,nostack");


  cEMCAL->cd(5); hPtPtotv5->Draw("colz");
  cEMCAL->cd(6); hPtPtotv6->Draw("colz");
  cEMCAL->cd(7); hPtPtotv7->Draw("colz");
 
  cEMCAL->cd(9);  hPtPtotRP1->Draw("colz");
  cEMCAL->cd(10); hPtPtotRP2->Draw("colz");
  cEMCAL->cd(11); hPtPtotRP3->Draw("colz");
  cEMCAL->cd(12); hPtPtotRP4->Draw("colz");
 /*
  CC->cd(7);     SiDisks->SetMarkerColor(3);  SiDisks->Draw(abname1); 
  CC->cd(8);     GEMh->SetMarkerColor(4);  GEMh->Draw(abname1); 
  // CC->cd(9);     GEMTRD->SetMarkerColor(4);  GEMTRD->Draw("scat=10., same"); 
  CC->cd(9);     GEMTRD->SetMarkerColor(4);  GEMTRD->Draw(abname1); 
  CC->cd(9);     hGEMForw[1]->SetMarkerColor(3);  hGEMForw[1]->Draw(abname1); 
  CC->cd(10);     hGEMForw[5]->SetMarkerColor(3);  hGEMForw[5]->Draw(abname1); 
  */
  /* for (k=0;k<6;k++) { 
     CC->cd(k+12+1);  eXY_planes[k]->Draw(abname); 
     printf ("CNe[%d]=%d \n",k,cne[k]);
   }

   CC->cd(12+1);  SiDisksE->SetMarkerColor(3);  SiDisksE->Draw(abname1); 
   CC->cd(12+3);  eEMCAL->SetMarkerColor(4);  eEMCAL->Draw(abname1); 

   cEMCAL->cd(1);   eEMCAL->Draw(abname);
  */
}
void test_tab()
{
   TGMainFrame *main = new TGMainFrame(gClient->GetRoot(), 800, 600);

   TGHorizontalFrame *frame1 = new TGHorizontalFrame(main, 60, 20, kFixedWidth);
   TGTextButton *exitButton = new TGTextButton(frame1, "&Exit", 
                                               "gApplication->Terminate(0)");
   frame1->AddFrame(exitButton, new TGLayoutHints(kLHintsTop | kLHintsLeft | 
                                                  kLHintsExpandX, 2, 2, 2, 2));

   frame1->Resize(150, exitButton->GetDefaultHeight());
   main->AddFrame(frame1, new TGLayoutHints(kLHintsBottom | kLHintsRight, 2, 2, 5, 1));

   //--------- create the Tab widget
   TGTab *fTab = new TGTab(main, 300, 300);
   TGLayoutHints *fL3 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5);
   
   //--------- create an empty tab element
   TGCompositeFrame *tf = fTab->AddTab("Tab 1");

   //-------------- create a tab element with an embedded canvas and buttons
   tf = fTab->AddTab("Tab 2");
   TGCompositeFrame *fF3 = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);
   TGTextButton *fStartB = new TGTextButton(fF3, "Start &Filling Hists", "Start()");
   TGTextButton *fStopB  = new TGTextButton(fF3, "&Stop Filling Hists", "Stop()");
   fF3->AddFrame(fStartB, fL3);
   fF3->AddFrame(fStopB, fL3);

   TGCompositeFrame *fF5 = new TGCompositeFrame(tf, 60, 60, kHorizontalFrame);

   TGLayoutHints *fL4 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX |
                                          kLHintsExpandY, 5, 5, 5, 5);
   TRootEmbeddedCanvas *fEc1 = new TRootEmbeddedCanvas("ec1", fF5, 100, 100);
   fF5->AddFrame(fEc1, fL4);
   TRootEmbeddedCanvas *fEc2 = new TRootEmbeddedCanvas("ec2", fF5, 100, 100);
   fF5->AddFrame(fEc2, fL4);

   tf->AddFrame(fF3, fL3);
   tf->AddFrame(fF5, fL4);

   // make tab yellow
   Pixel_t yellow;
   gClient->GetColorByName("yellow", yellow);
   TGTabElement *tabel = fTab->GetTabTab("Tab 2");;
   tabel->ChangeBackground(yellow);

   //-------------- end embedded canvas

   main->AddFrame(fTab, new TGLayoutHints(kLHintsBottom | kLHintsExpandX |
                                          kLHintsExpandY, 2, 2, 5, 1));

   main->MapSubwindows();
   main->Resize();   // resize to default size
   main->MapWindow();
}
