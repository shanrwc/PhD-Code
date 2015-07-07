//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 27.04.2011, MCP working group
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//////////////////
// HEADER FILES //
//////////////////

#include "StacoCBScaleEffFactors.h"

//*****************************************************************************

/////////////////
// CONSTRUCTOR //
/////////////////

StacoCBScaleEffFactors::StacoCBScaleEffFactors(void) {

    m_last_run_periodB = 178109;

////////////////////////////////////////////////////////////////
// FILL THE SCALE FACTOR AND SCALE FACTOR UNCERTAINTY VECTORS //
////////////////////////////////////////////////////////////////

    m_scale_factor_A = std::vector<double>(10);
    m_scale_factor_uncertainty_A = std::vector<double>(10);
    m_scale_factor_C_high_pt = std::vector<double>(10);
    m_scale_factor_uncertainty_C_high_pt = std::vector<double>(10);
    m_scale_factor_C_low_pt = std::vector<double>(10);
    m_scale_factor_uncertainty_C_low_pt = std::vector<double>(10);

    m_scale_factor_A[0] = 0.99679;
    m_scale_factor_A[1] = 1.00910;
    m_scale_factor_A[2] = 0.99299;
    m_scale_factor_A[3] = 0.96569;
    m_scale_factor_A[4] = 0.91706;
    m_scale_factor_A[5] = 0.99053;
    m_scale_factor_A[6] = 0.99441;
    m_scale_factor_A[7] = 0.98640;
    m_scale_factor_A[8] = 1.00846;
    m_scale_factor_A[9] = 0.99240;

    m_scale_factor_uncertainty_A[0] = 0.00325;
    m_scale_factor_uncertainty_A[1] = 0.00302;
    m_scale_factor_uncertainty_A[2] = 0.00379;
    m_scale_factor_uncertainty_A[3] = 0.00652;
    m_scale_factor_uncertainty_A[4] = 0.00646;
    m_scale_factor_uncertainty_A[5] = 0.00317;
    m_scale_factor_uncertainty_A[6] = 0.00306;
    m_scale_factor_uncertainty_A[7] = 0.00506;
    m_scale_factor_uncertainty_A[8] = 0.00490;
    m_scale_factor_uncertainty_A[9] = 0.00340;

    m_scale_factor_C_high_pt[0] = 0.99385;
    m_scale_factor_C_high_pt[1] = 1.00289;
    m_scale_factor_C_high_pt[2] = 0.99826;
    m_scale_factor_C_high_pt[3] = 0.96668;
    m_scale_factor_C_high_pt[4] = 0.90399;
    m_scale_factor_C_high_pt[5] = 0.98067;
    m_scale_factor_C_high_pt[6] = 0.99212;
    m_scale_factor_C_high_pt[7] = 0.95546;
    m_scale_factor_C_high_pt[8] = 0.99287;
    m_scale_factor_C_high_pt[9] = 0.98921;

    m_scale_factor_uncertainty_C_high_pt[0] = 0.00457;
    m_scale_factor_uncertainty_C_high_pt[1] = 0.00414;
    m_scale_factor_uncertainty_C_high_pt[2] = 0.00512;
    m_scale_factor_uncertainty_C_high_pt[3] = 0.00957;
    m_scale_factor_uncertainty_C_high_pt[4] = 0.00880;
    m_scale_factor_uncertainty_C_high_pt[5] = 0.00420;
    m_scale_factor_uncertainty_C_high_pt[6] = 0.00397;
    m_scale_factor_uncertainty_C_high_pt[7] = 0.00858;
    m_scale_factor_uncertainty_C_high_pt[8] = 0.00681;
    m_scale_factor_uncertainty_C_high_pt[9] = 0.00473;

    m_scale_factor_C_low_pt[0] = 0.98571;
    m_scale_factor_C_low_pt[1] = 1.00240;
    m_scale_factor_C_low_pt[2] = 1.00070;
    m_scale_factor_C_low_pt[3] = 0.98692;
    m_scale_factor_C_low_pt[4] = 0.89969;
    m_scale_factor_C_low_pt[5] = 0.99031;
    m_scale_factor_C_low_pt[6] = 0.99262;
    m_scale_factor_C_low_pt[7] = 0.96997;
    m_scale_factor_C_low_pt[8] = 0.99599;
    m_scale_factor_C_low_pt[9] = 0.99305;

    m_scale_factor_uncertainty_C_low_pt[0] = 0.00501;
    m_scale_factor_uncertainty_C_low_pt[1] = 0.00443;
    m_scale_factor_uncertainty_C_low_pt[2] = 0.00527;
    m_scale_factor_uncertainty_C_low_pt[3] = 0.00943;
    m_scale_factor_uncertainty_C_low_pt[4] = 0.00990;
    m_scale_factor_uncertainty_C_low_pt[5] = 0.00465;
    m_scale_factor_uncertainty_C_low_pt[6] = 0.00453;
    m_scale_factor_uncertainty_C_low_pt[7] = 0.00910;
    m_scale_factor_uncertainty_C_low_pt[8] = 0.00667;
    m_scale_factor_uncertainty_C_low_pt[9] = 0.00448;

// separate values for period B //
    m_scale_factor_A_periodB = std::vector<double>(10);
    m_scale_factor_uncertainty_A_periodB = std::vector<double>(10);
    m_scale_factor_C_periodB = std::vector<double>(10);
    m_scale_factor_uncertainty_C_periodB = std::vector<double>(10);

    m_scale_factor_A_periodB[0] = 0.9837;
    m_scale_factor_A_periodB[1] = 0.9879;
    m_scale_factor_A_periodB[2] = 0.9652;
    m_scale_factor_A_periodB[3] = 0.9179;
    m_scale_factor_A_periodB[4] = 0.9343;
    m_scale_factor_A_periodB[5] = 0.9286;
    m_scale_factor_A_periodB[6] = 0.9824;
    m_scale_factor_A_periodB[7] = 0.9720;
    m_scale_factor_A_periodB[8] = 0.9809;
    m_scale_factor_A_periodB[9] = 1.0080;

    m_scale_factor_uncertainty_A_periodB[0] = 0.0189;
    m_scale_factor_uncertainty_A_periodB[1] = 0.0202;
    m_scale_factor_uncertainty_A_periodB[2] = 0.0221;
    m_scale_factor_uncertainty_A_periodB[3] = 0.0406;
    m_scale_factor_uncertainty_A_periodB[4] = 0.0364;
    m_scale_factor_uncertainty_A_periodB[5] = 0.0249;
    m_scale_factor_uncertainty_A_periodB[6] = 0.0176;
    m_scale_factor_uncertainty_A_periodB[7] = 0.0367;
    m_scale_factor_uncertainty_A_periodB[8] = 0.0351;
    m_scale_factor_uncertainty_A_periodB[9] = 0.0184;

    m_scale_factor_C_periodB[0] = 0.9768;
    m_scale_factor_C_periodB[1] = 0.9583;
    m_scale_factor_C_periodB[2] = 0.9725;
    m_scale_factor_C_periodB[3] = 0.9861;
    m_scale_factor_C_periodB[4] = 0.9085;
    m_scale_factor_C_periodB[5] = 0.9906;
    m_scale_factor_C_periodB[6] = 1.0093;
    m_scale_factor_C_periodB[7] = 0.9339;
    m_scale_factor_C_periodB[8] = 0.9905;
    m_scale_factor_C_periodB[9] = 0.9767;

    m_scale_factor_uncertainty_C_periodB[0] = 0.0214;
    m_scale_factor_uncertainty_C_periodB[1] = 0.0210;
    m_scale_factor_uncertainty_C_periodB[2] = 0.0233;
    m_scale_factor_uncertainty_C_periodB[3] = 0.0388;
    m_scale_factor_uncertainty_C_periodB[4] = 0.0367;
    m_scale_factor_uncertainty_C_periodB[5] = 0.0166;
    m_scale_factor_uncertainty_C_periodB[6] = 0.0137;
    m_scale_factor_uncertainty_C_periodB[7] = 0.0550;
    m_scale_factor_uncertainty_C_periodB[8] = 0.0298;
    m_scale_factor_uncertainty_C_periodB[9] = 0.0201;

}

//*****************************************************************************

////////////////////////
// METHOD scaleFactor //
////////////////////////

double StacoCBScaleEffFactors::scaleFactor(
                                            const TLorentzVector & tlv) const {

    return scaleFactor(tlv, m_last_run_periodB+1);

}

//*****************************************************************************

///////////////////////////////////
// METHOD scaleFactorUncertainty //
///////////////////////////////////

double StacoCBScaleEffFactors::scaleFactorUncertainty(
                                            const TLorentzVector & tlv) const {

    return scaleFactorUncertainty(tlv, m_last_run_periodB+1);

}

//*****************************************************************************

//////////////////////////////
// METHOD scaleFactor(., .) //
//////////////////////////////

double StacoCBScaleEffFactors::scaleFactor(
                                        const TLorentzVector & tlv,
                                        const unsigned int & run_nb) const {

    int bin(m_eta_phi_binning.symmetricBin(&tlv)-1);
    if(bin<0) return 1.; //UNKNOWN BIN
    if (tlv.Eta()>0) {
        if (run_nb>m_last_run_periodB) {
            return m_scale_factor_A[bin];
        } else {
            return m_scale_factor_A_periodB[bin];
        }
    }
    if (run_nb>m_last_run_periodB) {
        if (tlv.Pt()>40000) {
            return m_scale_factor_C_high_pt[bin];
        } else {
            return m_scale_factor_C_low_pt[bin];   
        }
    } else {
        return m_scale_factor_C_periodB[bin];
    }

}

//*****************************************************************************

/////////////////////////////////////////
// METHOD scaleFactorUncertainty(., .) //
/////////////////////////////////////////

double StacoCBScaleEffFactors::scaleFactorUncertainty(
                                            const TLorentzVector & tlv,
                                            const unsigned int & run_nb) const {

    int bin(m_eta_phi_binning.symmetricBin(&tlv)-1);
    if(bin<0) return 1.; //UNKNOWN BIN
    if (tlv.Eta()>0) {
        if (run_nb>m_last_run_periodB) {
            return m_scale_factor_uncertainty_A[bin];
        } else {
            return m_scale_factor_uncertainty_A_periodB[bin];
        }
    }
    if (run_nb>m_last_run_periodB) {
        if (tlv.Pt()>40000) {
            return m_scale_factor_uncertainty_C_high_pt[bin];
        } else {
            return m_scale_factor_uncertainty_C_low_pt[bin];
        }
    } else {
        return m_scale_factor_uncertainty_C_periodB[bin];
    }

}

//*****************************************************************************

/////////////////////////////////////////////
// METHOD scaleFactorSystematicUncertainty //
/////////////////////////////////////////////

double StacoCBScaleEffFactors::scaleFactorSystematicUncertainty(
                                            const TLorentzVector & tlv) const {

    return 0.002+4.2e-6*tlv.E()/1000.0;

}
