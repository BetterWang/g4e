#ifndef G4E_FLATIOCe_EMCAL_HH
#define G4E_FLATIOCe_EMCAL_HH

namespace g4e {
    struct Ce_EMCALIoData {
        const std::string& aName;
        int id;
        double aEtot_dep;
        int aNpe;
        double aADC;
        double aTDC;
        double axcrs;
        double aycrs;
        double azcrs;
    };

    struct Ce_EMCALIo
    {
        void BindToTree(TTree *tree)
        {
            tree->Branch("ce_emcal_count", &mDataCount);
            tree->Branch("ce_emcal_name", &mTowerNames);
            tree->Branch("ce_emcal_id", &mIds);
            tree->Branch("ce_emcal_etot_dep", &mTotalDepEnergies);
            tree->Branch("ce_emcal_npe", &mNpe);
            tree->Branch("ce_emcal_adc", &mAdc);
            tree->Branch("ce_emcal_tdc", &mTdc);
            tree->Branch("ce_emcal_xcrs", &mCrsX);
            tree->Branch("ce_emcal_ycrs", &mCrsY);
            tree->Branch("ce_emcal_zcrs", &mCrsZ);
            tree->Branch("ce_emcal_row", &mRowIds);
            tree->Branch("ce_emcal_col", &mColumnIds);
            tree->Branch("ce_emcal_section", &mSectionIds);
        }

        void Clear()
        {
            mTowerNames.clear();
            mTotalDepEnergies.clear();
            mNpe.clear();
            mAdc.clear();
            mTdc.clear();
            mCrsX.clear();
            mCrsY.clear();
            mCrsZ.clear();
            mSectionIds.clear();
            mRowIds.clear();
            mColumnIds.clear();
            mIds.clear();
            mDataCount = 0;
        }

        void Fill(Ce_EMCALIoData data)
        {
            mTowerNames.push_back(data.aName);
            mDataCount = mTowerNames.size();
            mIds.push_back(data.id);
            mTotalDepEnergies.push_back(data.aEtot_dep);
            mNpe.push_back(data.aNpe);
            mAdc.push_back(data.aADC);
            mTdc.push_back(data.aTDC);
            mCrsX.push_back(data.axcrs);
            mCrsY.push_back(data.aycrs);
            mCrsZ.push_back(data.azcrs);

            // decode id section, row and column
            int section = data.id / 1000000;
            int idRow = (data.id - section*1000000)/1000;
            int idCol = data.id - section*1000000 - idRow*1000;
            mSectionIds.push_back(section);
            mRowIds.push_back(idRow);
            mColumnIds.push_back(idCol);
        }

        uint64_t mDataCount = 0;
        std::vector<std::string> mTowerNames;  // PrimaryVertex->GetX0();
        std::vector<int>mIds;                  // tower id (1000000*section + 1000*row + col
        std::vector<int>mSectionIds;           // 0 - PWO, 1 - Glass
        std::vector<int>mRowIds;               // tower rowID
        std::vector<int>mColumnIds;            // tower columnID
        std::vector<double> mTotalDepEnergies; // PrimaryVertex->GetY0();
        std::vector<int>    mNpe;
        std::vector<double> mAdc;              // PrimaryVertex->GetZ0();
        std::vector<double> mTdc;              // PrimaryVertex->GetT0();
        std::vector<double> mCrsX;
        std::vector<double> mCrsY;
        std::vector<double> mCrsZ;
    };
}

#endif //G4E_FLATIOCe_EMCAL_HH
