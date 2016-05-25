/**
 * \class L1TStage2uGT
 *
 * Description: DQM for L1 Micro Global Trigger.
 *
 * \author Mateusz Zarucki 2016
 * \author J. Berryhill, I. Mikulec
 * \author Vasile Mihai Ghete - HEPHY Vienna
 *
 */

#include "DQM/L1TMonitor/interface/L1TStage2uGT.h"

// Constructor
L1TStage2uGT::L1TStage2uGT(const edm::ParameterSet& params):
   l1tStage2uGtSource_(consumes<GlobalAlgBlkBxCollection>(params.getParameter<edm::InputTag>("l1tStage2uGtSource"))),
   monitorDir_(params.getUntrackedParameter<std::string> ("monitorDir", "")),
   verbose_(params.getUntrackedParameter<bool>("verbose", false))
{
   // empty
}

// Destructor
L1TStage2uGT::~L1TStage2uGT() {
   // empty
}

void L1TStage2uGT::dqmBeginRun(edm::Run const& iRun, edm::EventSetup const& evtSetup) {
   // empty 
}

void L1TStage2uGT::beginLuminosityBlock(const edm::LuminosityBlock& iLumi, const edm::EventSetup& evtSetup) { 
   // empty
}

void L1TStage2uGT::bookHistograms(DQMStore::IBooker &ibooker, edm::Run const&, edm::EventSetup const& evtSetup) {
   
   // Book histograms
   const int numLS = 2000;
   const double numLS_d = static_cast<double>(numLS);
   const int numAlgs = 512; // FIXME: Take number of algorithms from EventSetup
   const double numAlgs_d = static_cast<double>(numAlgs);
   const int numBx = 3564; 
   const double numBx_d = static_cast<double>(numBx);

   ibooker.setCurrentFolder(monitorDir_);
   
   // Algorithm bits 
   algoBits_after_bxomask_5BX_ = ibooker.book1D("algoBits_after_bxomask_5BX", "uGT: Algorithm Trigger Bits (after BX mask, before prescale) for 5 BX", numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_bxomask_5BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 1);
   
   algoBits_after_bxomask_1BX_ = ibooker.book1D("algoBits_after_bxomask_1BX", "uGT: Algorithm Trigger Bits (after BX mask, before prescale) for Triggering BX", numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_bxomask_1BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 1);
   
   algoBits_after_prescaler_5BX_ = ibooker.book1D("algoBits_after_prescaler_5BX", "uGT: Algorithm Trigger Bits (after prescale) for 5 BX", numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_prescaler_5BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 1);
   
   algoBits_after_prescaler_1BX_ = ibooker.book1D("algoBits_after_prescaler_1BX", "uGT: Algorithm Trigger Bits (after prescale) for Triggering BX", numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_prescaler_1BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 1);
  
   algoBits_after_mask_5BX_ = ibooker.book1D("algoBits_after_mask_5BX", "uGT: Algorithm Trigger Bits (after mask) for 5 BX", numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_mask_5BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 1);
   
   algoBits_after_mask_1BX_ = ibooker.book1D("algoBits_after_mask_1BX", "uGT: Algorithm Trigger Bits (after mask) for Triggering BX", numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_mask_1BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 1);
  
   // Algorithm bits correlation 
   algoBits_after_bxomask_corr_5BX_ = ibooker.book2D("algoBits_after_bxomask_corr_5BX","uGT: Algorithm Trigger Bit Correlation (after BX mask, before prescale) for 5 BX", numAlgs, -0.5, numAlgs_d-0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_bxomask_corr_5BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 1);
   algoBits_after_bxomask_corr_5BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 2);
   
   algoBits_after_bxomask_corr_1BX_ = ibooker.book2D("algoBits_after_bxomask_corr_1BX","uGT: Algorithm Trigger Bit Correlation (after BX mask, before prescale) for Triggering BX", numAlgs, -0.5, numAlgs_d-0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_bxomask_corr_1BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 1);
   algoBits_after_bxomask_corr_1BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 2);
   
   algoBits_after_prescaler_corr_5BX_ = ibooker.book2D("algoBits_after_prescaler_corr_5BX","uGT: Algorithm Trigger Bit Correlation (after prescale) for 5 BX", numAlgs, -0.5, numAlgs_d-0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_prescaler_corr_5BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 1);
   algoBits_after_prescaler_corr_5BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 2);
   
   algoBits_after_prescaler_corr_1BX_ = ibooker.book2D("algoBits_after_prescaler_corr_1BX","uGT: Algorithm Trigger Bit Correlation (after prescale) for Triggering BX", numAlgs, -0.5, numAlgs_d-0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_prescaler_corr_1BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 1);
   algoBits_after_prescaler_corr_1BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 2);
   
   algoBits_after_mask_corr_5BX_ = ibooker.book2D("algoBits_after_mask_corr_5BX","uGT: Algorithm Trigger Bit Correlation (after mask) for 5 BX", numAlgs, -0.5, numAlgs_d-0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_mask_corr_5BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 1);
   algoBits_after_mask_corr_5BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 2);
   
   algoBits_after_mask_corr_1BX_ = ibooker.book2D("algoBits_after_mask_corr_1BX","uGT: Algorithm Trigger Bit Correlation (after mask) for Triggering BX", numAlgs, -0.5, numAlgs_d-0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_mask_corr_1BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 1);
   algoBits_after_mask_corr_1BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 2);
   
   // Algorithm bits vs global BX number
   algoBits_after_bxomask_bx_global_5BX_ = ibooker.book2D("algoBits_after_bxomask_bx_global_5BX", "uGT: Algorithm Trigger Bits (after BX mask, before prescale) vs. Global BX Number for 5 BX", numBx, 0.5, numBx_d + 0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_bxomask_bx_global_5BX_->setAxisTitle("Global Bunch Crossing Number", 1); 
   algoBits_after_bxomask_bx_global_5BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 2);
   
   algoBits_after_bxomask_bx_global_1BX_ = ibooker.book2D("algoBits_after_bxomask_bx_global_1BX", "uGT: Algorithm Trigger Bits (after BX mask, before prescale) vs. Global BX Number for Triggering BX", numBx, 0.5, numBx_d + 0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_bxomask_bx_global_1BX_->setAxisTitle("Global Bunch Crossing Number", 1); 
   algoBits_after_bxomask_bx_global_1BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 2);
   
   algoBits_after_prescaler_bx_global_5BX_ = ibooker.book2D("algoBits_after_prescaler_bx_global_5BX", "uGT: Algorithm Trigger Bits (after prescale) vs. Global BX Number for 5 BX", numBx, 0.5, numBx_d + 0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_prescaler_bx_global_5BX_->setAxisTitle("Global Bunch Crossing Number", 1); 
   algoBits_after_prescaler_bx_global_5BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 2);
   
   algoBits_after_prescaler_bx_global_1BX_ = ibooker.book2D("algoBits_after_prescaler_bx_global_1BX", "uGT: Algorithm Trigger Bits (after prescale) vs. Global BX Number for Triggering BX", numBx, 0.5, numBx_d + 0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_prescaler_bx_global_1BX_->setAxisTitle("Global Bunch Crossing Number", 1); 
   algoBits_after_prescaler_bx_global_1BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 2);
  
   algoBits_after_mask_bx_global_5BX_ = ibooker.book2D("algoBits_after_mask_bx_global_5BX", "uGT: Algorithm Trigger Bits (after mask) vs. Global BX Number for 5 BX", numBx, 0.5, numBx_d + 0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_mask_bx_global_5BX_->setAxisTitle("Global Bunch Crossing Number", 1); 
   algoBits_after_mask_bx_global_5BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 2);

   algoBits_after_mask_bx_global_1BX_ = ibooker.book2D("algoBits_after_mask_bx_global_1BX", "uGT: Algorithm Trigger Bits (after mask) vs. Global BX Number for Triggering BX", numBx, 0.5, numBx_d + 0.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_mask_bx_global_1BX_->setAxisTitle("Global Bunch Crossing Number", 1); 
   algoBits_after_mask_bx_global_1BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 2);
  
   // Algorithm bits vs BX number in event
   algoBits_after_bxomask_bx_inEvt_5BX_ = ibooker.book2D("algoBits_after_bxomask_bx_inEvt_5BX", "uGT: Algorithm Trigger Bits (after BX mask, before prescale) vs. BX Number in Event for 5 BX", 5, -2.5, 2.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_bxomask_bx_inEvt_5BX_->setAxisTitle("Bunch Crossing Number in Event", 1);
   algoBits_after_bxomask_bx_inEvt_5BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 2);
   
   algoBits_after_bxomask_bx_inEvt_1BX_ = ibooker.book2D("algoBits_after_bxomask_bx_inEvt_1BX", "uGT: Algorithm Trigger Bits (after BX mask, before prescale) vs. BX Number in Event for Triggering BX", 5, -2.5, 2.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_bxomask_bx_inEvt_1BX_->setAxisTitle("Bunch Crossing Number in Event", 1);
   algoBits_after_bxomask_bx_inEvt_1BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 2);
   
   algoBits_after_prescaler_bx_inEvt_5BX_ = ibooker.book2D("algoBits_after_prescaler_bx_inEvt_5BX", "uGT: Algorithm Trigger Bits (after prescale) vs. BX Number in Event for 5 BX", 5, -2.5, 2.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_prescaler_bx_inEvt_5BX_->setAxisTitle("Bunch Crossing Number in Event", 1);
   algoBits_after_prescaler_bx_inEvt_5BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 2);
   
   algoBits_after_prescaler_bx_inEvt_1BX_ = ibooker.book2D("algoBits_after_prescaler_bx_inEvt_1BX", "uGT: Algorithm Trigger Bits (after prescale) vs. BX Number in Event for Triggering BX", 5, -2.5, 2.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_prescaler_bx_inEvt_1BX_->setAxisTitle("Bunch Crossing Number in Event", 1);
   algoBits_after_prescaler_bx_inEvt_1BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 2);
  
   algoBits_after_mask_bx_inEvt_5BX_ = ibooker.book2D("algoBits_after_mask_bx_inEvt_5BX", "uGT: Algorithm Trigger Bits (after mask) vs. BX Number in Event for 5 BX", 5, -2.5, 2.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_mask_bx_inEvt_5BX_->setAxisTitle("Bunch Crossing Number in Event", 1);
   algoBits_after_mask_bx_inEvt_5BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 2);
   
   algoBits_after_mask_bx_inEvt_1BX_ = ibooker.book2D("algoBits_after_mask_bx_inEvt_1BX", "uGT: Algorithm Trigger Bits (after mask) vs. BX Number in Event for Triggering BX", 5, -2.5, 2.5, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_mask_bx_inEvt_1BX_->setAxisTitle("Bunch Crossing Number in Event", 1);
   algoBits_after_mask_bx_inEvt_1BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 2);
  
   // Algorithm bits vs LS
   algoBits_after_bxomask_lumi_5BX_ = ibooker.book2D("algoBits_after_bxomask_lumi_5BX","uGT: Algorithm Trigger Bits (after BX mask, before prescale) vs. LS for 5 BX", numLS, 0., numLS_d, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_bxomask_lumi_5BX_->setAxisTitle("Luminosity Segment", 1);
   algoBits_after_bxomask_lumi_5BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 2);
   
   algoBits_after_bxomask_lumi_1BX_ = ibooker.book2D("algoBits_after_bxomask_lumi_1BX","uGT: Algorithm Trigger Bits (after BX mask, before prescale) vs. LS for Triggering BX", numLS, 0., numLS_d, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_bxomask_lumi_1BX_->setAxisTitle("Luminosity Segment", 1);
   algoBits_after_bxomask_lumi_1BX_->setAxisTitle("Algorithm Trigger Bits (after BX mask, before prescale)", 2);

   algoBits_after_prescaler_lumi_5BX_ = ibooker.book2D("algoBits_after_prescaler_lumi_5BX","uGT: Algorithm Trigger Bits (after prescale) vs. LS for 5 BX", numLS, 0., numLS_d, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_prescaler_lumi_5BX_->setAxisTitle("Luminosity Segment", 1);
   algoBits_after_prescaler_lumi_5BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 2);
  
   algoBits_after_prescaler_lumi_1BX_ = ibooker.book2D("algoBits_after_prescaler_lumi_1BX","uGT: Algorithm Trigger Bits (after prescale) vs. LS for Triggering BX", numLS, 0., numLS_d, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_prescaler_lumi_1BX_->setAxisTitle("Luminosity Segment", 1);
   algoBits_after_prescaler_lumi_1BX_->setAxisTitle("Algorithm Trigger Bits (after prescale)", 2);
  
   algoBits_after_mask_lumi_5BX_ = ibooker.book2D("algoBits_after_mask_lumi_5BX","uGT: Algorithm Trigger Bits (after mask) vs. LS for 5 BX", numLS, 0., numLS_d, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_mask_lumi_5BX_->setAxisTitle("Luminosity Segment", 1);
   algoBits_after_mask_lumi_5BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 2);
   
   algoBits_after_mask_lumi_1BX_ = ibooker.book2D("algoBits_after_mask_lumi_1BX","uGT: Algorithm Trigger Bits (after mask) vs. LS for Triggering BX", numLS, 0., numLS_d, numAlgs, -0.5, numAlgs_d-0.5);
   algoBits_after_mask_lumi_1BX_->setAxisTitle("Luminosity Segment", 1);
   algoBits_after_mask_lumi_1BX_->setAxisTitle("Algorithm Trigger Bits (after mask)", 2);

   // Prescale factor index 
   prescaleFactorSet_5BX_ = ibooker.book2D("prescaleFactorSet_5BX", "uGT: Index of Prescale Factor Set vs. LS for 5 BX", numLS, 0., numLS_d, 25, 0., 25.);
   prescaleFactorSet_5BX_->setAxisTitle("Luminosity Segment", 1);
   prescaleFactorSet_5BX_->setAxisTitle("Prescale Factor Set Index", 2);
   
   prescaleFactorSet_1BX_ = ibooker.book2D("prescaleFactorSet_1BX", "uGT: Index of Prescale Factor Set vs. LS for Triggering BX", numLS, 0., numLS_d, 25, 0., 25.);
   prescaleFactorSet_1BX_->setAxisTitle("Luminosity Segment", 1);
   prescaleFactorSet_1BX_->setAxisTitle("Prescale Factor Set Index", 2);
}

void L1TStage2uGT::analyze(const edm::Event& evt, const edm::EventSetup& evtSetup) {
   // FIXME: Remove duplicate definition of numAlgs 
   const int numAlgs = 512;
  
   if (verbose_) {
      edm::LogInfo("L1TStage2uGT") << "L1TStage2uGT DQM: Analyzing.." << std::endl;
   }
   
   // Get standard event parameters 
   int lumi = evt.luminosityBlock();
   int bx = evt.bunchCrossing();
      
   // Open uGT readout record
   edm::Handle<GlobalAlgBlkBxCollection> uGtAlgs;
   evt.getByToken(l1tStage2uGtSource_, uGtAlgs);
   
   if (!uGtAlgs.isValid()) {
      edm::LogInfo("L1TStage2uGT") << "Cannot find uGT readout record.";
      return;
   }
   
   // Get uGT algo bit statistics
   else {
      //algoBits_->Fill(-1.); // fill underflow to normalize // FIXME: needed? 
      for (int ibx = uGtAlgs->getFirstBX(); ibx <= uGtAlgs->getLastBX(); ++ibx) {
         for (auto itr = uGtAlgs->begin(ibx); itr != uGtAlgs->end(ibx); ++itr) { // FIXME: redundant loop? 
            
            // Fills prescale factor set histogram
            prescaleFactorSet_5BX_->Fill(lumi, itr->getPreScColumn());
            
            if(ibx == 0) {
               prescaleFactorSet_1BX_->Fill(lumi, itr->getPreScColumn());
            }
           
            // Fills algorithm bits histograms
            for(int algoBit = 0; algoBit < numAlgs; ++algoBit) {
              
               // Algorithm bits after BX mask, before prescale 
               if(itr->getAlgoDecisionInitial(algoBit)) {
                  
                  algoBits_after_bxomask_5BX_->Fill(algoBit);
                  algoBits_after_bxomask_lumi_5BX_->Fill(lumi, algoBit);
                  algoBits_after_bxomask_bx_inEvt_5BX_->Fill(ibx, algoBit); // FIXME: or itr->getbxInEventNr()/getbxNr()?
                  algoBits_after_bxomask_bx_global_5BX_->Fill(bx + ibx, algoBit);
                  
                  if(ibx == 0) {
                     algoBits_after_bxomask_1BX_->Fill(algoBit);
                     algoBits_after_bxomask_lumi_1BX_->Fill(lumi, algoBit);
                     algoBits_after_bxomask_bx_inEvt_1BX_->Fill(ibx, algoBit);
                     algoBits_after_bxomask_bx_global_1BX_->Fill(bx + ibx, algoBit);
                  }
 
                  for(int algoBit2 = 0; algoBit2 < numAlgs; ++algoBit2) {
                     if(itr->getAlgoDecisionInitial(algoBit2)) {
                        
                        algoBits_after_bxomask_corr_5BX_->Fill(algoBit, algoBit2);
                        
                        if(ibx == 0) {
                           algoBits_after_bxomask_corr_1BX_->Fill(algoBit, algoBit2);
                        }
                     }
                  }
               }  
               
               // Algorithm bits after prescale 
               if(itr->getAlgoDecisionInterm(algoBit)) {
                  
                  algoBits_after_prescaler_5BX_->Fill(algoBit);
                  algoBits_after_prescaler_lumi_5BX_->Fill(lumi, algoBit);
                  algoBits_after_prescaler_bx_inEvt_5BX_->Fill(ibx, algoBit); // FIXME: or itr->getbxInEventNr()/getbxNr()?
                  algoBits_after_prescaler_bx_global_5BX_->Fill(bx + ibx, algoBit);
                 
                  if(ibx == 0) {  
                     algoBits_after_prescaler_1BX_->Fill(algoBit);
                     algoBits_after_prescaler_lumi_1BX_->Fill(lumi, algoBit);
                     algoBits_after_prescaler_bx_inEvt_1BX_->Fill(ibx, algoBit);
                     algoBits_after_prescaler_bx_global_1BX_->Fill(bx + ibx, algoBit);
                  } 
                  
                  for(int algoBit2 = 0; algoBit2 < numAlgs; ++algoBit2) {
                     if(itr->getAlgoDecisionInterm(algoBit2)) {
                  
                        algoBits_after_prescaler_corr_5BX_->Fill(algoBit, algoBit2);
                        
                        if(ibx == 0) {
                           algoBits_after_prescaler_corr_1BX_->Fill(algoBit, algoBit2);
                        }
                     }
                  }
               }  
               
               // Algorithm bits after mask 
               if(itr->getAlgoDecisionFinal(algoBit)) {
                 
                  algoBits_after_mask_5BX_->Fill(algoBit);
                  algoBits_after_mask_lumi_5BX_->Fill(lumi, algoBit);
                  algoBits_after_mask_bx_inEvt_5BX_->Fill(ibx, algoBit); // FIXME: or itr->getbxInEventNr()/getbxNr()?
                  algoBits_after_mask_bx_global_5BX_->Fill(bx + ibx, algoBit);
                  
                  if(ibx == 0) {
                     algoBits_after_mask_1BX_->Fill(algoBit);
                     algoBits_after_mask_lumi_1BX_->Fill(lumi, algoBit);
                     algoBits_after_mask_bx_global_1BX_->Fill(bx, algoBit);
                     algoBits_after_mask_bx_inEvt_1BX_->Fill(ibx, algoBit);
                  } 
                 
                  for(int algoBit2 = 0; algoBit2 < numAlgs; ++algoBit2) {
                     if(itr->getAlgoDecisionFinal(algoBit2)) {
                
                        algoBits_after_mask_corr_5BX_->Fill(algoBit, algoBit2);
                        
                        if(ibx == 0) {
                           algoBits_after_mask_corr_1BX_->Fill(algoBit, algoBit2);
                        }
                     }
                  }
               }  
            }
         }
      }
   }
}

// End section
void L1TStage2uGT::endLuminosityBlock(const edm::LuminosityBlock& iLumi, const edm::EventSetup& evtSetup) {
   // empty
}
