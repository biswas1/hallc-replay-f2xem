#define wire_drift_times_cxx
#include "wire_drift_times.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#define NPLANES 12
#define NBINS 400
#define bin_min -50.5
#define bin_max 349.5
void wire_drift_times::Loop()
{
  //   In a ROOT session, you can do:
  //      Root > .L wire_drift_times.C
  //      Root > wire_drift_times t
  //      Root > t.GetEntry(12); // Fill t data members with entry number 12
  //      Root > t.Show();       // Show values of entry 12
  //      Root > t.Show(16);     // Read and show values of entry 16
  //      Root > t.Loop();       // Loop on all entries
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

  Long64_t nentries = fChain->GetEntriesFast();

  //Read Run Number from txt file
  int run_NUM;
  Long64_t num_evts;        //added
  string input_file;   //added
  TString f0 = "input_RUN.txt";
  ifstream infile(f0);
  infile >> input_file >> run_NUM >> num_evts;

  TString run = Form("run%d", run_NUM);
  //Declare plane names to loop over
  TString plane_names[NPLANES]={"1u1", "1u2", "1x1", "1x2", "1v1", "1v2", "2v2", "2v1", "2x2", "2x1", "2u2", "2u1"};

  //Declare a root file array to store individual DC cell drift times
  TString root_file[NPLANES];
  TFile *g[NPLANES];
   
  //integer to store total sense wires for a plane chosen by the user
  static const Int_t total_wires_x  = 79;  
  static const Int_t total_wires_uv = 107;
     
  Long64_t nbytes = 0, nb = 0;
   
  //Loop over all planes
  for (int ip = 0; ip < NPLANES; ip++){

    //Initialize a root file array to store individual DC cell drift times
    root_file[ip] = "../root_files/" + run + "/shms_DC_"+plane_names[ip]+Form("_%d_wire_histos.root", run_NUM);
    g[ip] = new TFile(root_file[ip], "RECREATE");
    g[ip]->cd();

    /*========================PLANES 1X1,1X2,2X1,2X2=====================================*/
	
    //If specific planes are encountered, treat them as follows:

    //variable declarations
    int wirenum;

    /*PLANE 1U1, 1V1, 2U1, 2V1*/				
    //If specific planes are encountered, treat them as follows:
    if(ip == 0 || ip == 1 || ip == 4 || ip == 5 || ip == 6 || ip == 7 || ip == 10 || ip == 11) {

      TH1F *cell_dt[total_wires_uv];    
      TH2F *drifttime_vs_wire = new TH2F("drifttime_vs_wire", "", 107., 0.,107., NBINS, bin_min, bin_max);
      
     
    
      drifttime_vs_wire->GetXaxis()->SetTitle("Wire Number");
      drifttime_vs_wire->GetYaxis()->SetTitle("Drift Time (ns)");

      //Initialize wire drift time histograms
      for (wirenum=1; wirenum<=total_wires_uv; wirenum++){
	cell_dt[wirenum-1] = new TH1F(Form("wire_%d", wirenum), "", NBINS, bin_min, bin_max);
                
      cell_dt[wirenum-1]->GetXaxis()->SetTitle("Drift Time (ns)");
      cell_dt[wirenum-1]->GetYaxis()->SetTitle("Number of Entries / 1 ns");
      }
	
      //Loop over all entries (triggers or events)   
      for (Long64_t jentry=0; jentry<num_evts; jentry++) {
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);   nbytes += nb;
	// if (Cut(ientry) < 0) continue;
 
	if (ip == 0) {
	  for (int i=0; i< Ndata_P_dc_1u1_wirenum; i++){
	    wirenum = int(P_dc_1u1_wirenum[i]);
	    //cout << " wire num: " << P_dc_1u1_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_1u1_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_1u1_time[i]);
	    drifttime_vs_wire->Fill(P_dc_1u1_wirenum[i], P_dc_1u1_time[i]);

	  }
	}
							
	if (ip == 1) {
	  for (int i=0; i< Ndata_P_dc_1u2_wirenum; i++){
	    wirenum = int(P_dc_1u2_wirenum[i]);
	    //cout << " wire num: " << P_dc_1u2_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_1u2_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_1u2_time[i]);
	    drifttime_vs_wire->Fill(P_dc_1u2_wirenum[i], P_dc_1u2_time[i]);

	  }
	}						
			
	if (ip == 4) {
	  for (int i=0; i< Ndata_P_dc_1v1_wirenum; i++){
	    wirenum = int(P_dc_1v1_wirenum[i]);
	    //cout << " wire num: " << P_dc_1v1_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_1v1_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_1v1_time[i]);
	    drifttime_vs_wire->Fill(P_dc_1v1_wirenum[i], P_dc_1v1_time[i]);

	  }
	}		
			
	if (ip == 5) {
	  for (int i=0; i< Ndata_P_dc_1v2_wirenum; i++){
	    wirenum = int(P_dc_1v2_wirenum[i]);
	    //cout << " wire num: " << P_dc_1v2_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_1v2_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_1v2_time[i]);
	    drifttime_vs_wire->Fill(P_dc_1v2_wirenum[i], P_dc_1v2_time[i]);

	  }
	}

	if (ip == 6) {
	  for (int i=0; i< Ndata_P_dc_2v2_wirenum; i++){
	    wirenum = int(P_dc_2v2_wirenum[i]);
	    //cout << " wire num: " << P_dc_2v2_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_2v2_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_2v2_time[i]);
	    drifttime_vs_wire->Fill(P_dc_2v2_wirenum[i], P_dc_2v2_time[i]);

	  }
	}

	if (ip == 7) {
	  for (int i=0; i< Ndata_P_dc_2v1_wirenum; i++){
	    wirenum = int(P_dc_2v1_wirenum[i]);
	    //cout << " wire num: " << P_dc_2v1_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_2v1_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_2v1_time[i]);
	    drifttime_vs_wire->Fill(P_dc_2v1_wirenum[i], P_dc_2v1_time[i]);

	  }
	}

	if (ip == 10) {
	  for (int i=0; i< Ndata_P_dc_2u2_wirenum; i++){
	    wirenum = int(P_dc_2u2_wirenum[i]);
	    //cout << " wire num: " << P_dc_2u2_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_2u2_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_2u2_time[i]);
	    drifttime_vs_wire->Fill(P_dc_2u2_wirenum[i], P_dc_2u2_time[i]);

	  }
	}

	if (ip == 11) {
	  for (int i=0; i< Ndata_P_dc_2u1_wirenum; i++){
	    wirenum = int(P_dc_2u1_wirenum[i]);
	    //cout << " wire num: " << P_dc_2u1_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_2u1_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_2u1_time[i]);
	    drifttime_vs_wire->Fill(P_dc_2u1_wirenum[i], P_dc_2u1_time[i]);

	  }
	}						
					
					
					
      }
    }
	
    if(ip == 2 || ip == 3 || ip == 8 || ip == 9) {

      TH1F *cell_dt[total_wires_x];    
      TH2F *drifttime_vs_wire = new TH2F("drifttime_vs_wire", "", 79., 0., 79., NBINS, bin_min, bin_max);
 
            
  
      drifttime_vs_wire->GetXaxis()->SetTitle("Wire Number");
      drifttime_vs_wire->GetYaxis()->SetTitle("Drift Time (ns)");    

      //Initialize wire drift time histograms
      for (int wirenum=1; wirenum<=total_wires_x; wirenum++){
	cell_dt[wirenum-1] = new TH1F(Form("wire_%d", wirenum), "", NBINS, bin_min, bin_max);
  
	cell_dt[wirenum-1]->GetXaxis()->SetTitle("Drift Time (ns)");
	cell_dt[wirenum-1]->GetYaxis()->SetTitle("Number of Entries / 1 ns");


      }
	
      //Loop over all entries (triggers or events)   
      for (Long64_t jentry=0; jentry<num_evts; jentry++) {
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);   nbytes += nb;
	// if (Cut(ientry) < 0) continue;
 
	if (ip == 2) {
	  for (int i=0; i< Ndata_P_dc_1x1_wirenum; i++){
	    wirenum = int(P_dc_1x1_wirenum[i]);
	    //cout << " wire num: " << P_dc_1x1_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_1x1_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_1x1_time[i]);
	    drifttime_vs_wire->Fill(P_dc_1x1_wirenum[i], P_dc_1x1_time[i]);

	  }
	}
							
	if (ip == 3) {
	  for (int i=0; i< Ndata_P_dc_1x2_wirenum; i++){
	    wirenum = int(P_dc_1x2_wirenum[i]);
	    //cout << " wire num: " << P_dc_1x2_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_1x2_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_1x2_time[i]);
	    drifttime_vs_wire->Fill(P_dc_1x2_wirenum[i], P_dc_1x2_time[i]);

	  }
	}						
			
	if (ip == 8) {
	  for (int i=0; i< Ndata_P_dc_2x2_wirenum; i++){
	    wirenum = int(P_dc_2x2_wirenum[i]);
	    //cout << " wire num: " << P_dc_2x2_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_2x2_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_2x2_time[i]);
	    drifttime_vs_wire->Fill(P_dc_2x2_wirenum[i], P_dc_2x2_time[i]);

	  }
	}		
			
	if (ip == 9) {
	  for (int i=0; i< Ndata_P_dc_2x1_wirenum; i++){
	    wirenum = int(P_dc_2x1_wirenum[i]);
	    //cout << " wire num: " << P_dc_2x1_wirenum[i] << endl;
	    //cout << "Time: " << P_dc_2x1_time[i] << endl;

	    //Fill the Histograms
	    cell_dt[wirenum-1]->Fill(P_dc_2x1_time[i]);
	    drifttime_vs_wire->Fill(P_dc_2x1_wirenum[i], P_dc_2x1_time[i]);

	  }
	}						
					
					
					
      }
    }
					


    //Write wire drift time histos to file
    g[ip]->Write();
    cout << "EVERYTHING OK in plane:" << ip << endl;	

  }


  //     cout << "\r          \r" << (float)sensewire / total_wires * 100.0 << "%" << flush; 

}
