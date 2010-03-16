/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

/**
 *
 * @author Edwin
 */
public class clsModificationMassMapping {

    public clsModificationMassMapping() {
    }

    public String[][] GetMapTable() {

        // Initialize the array
        String[][] arMappingTable = {{"Carbamidomethyl (C)", "ICAT-D (C)", "ICAT-C (C)", "Carboxymethyl (C)", "Pyridylethyl (C)", "Propionamide (C)", "iTRAQ (N-term,K)", "Oxidation (M)", "Dioxidation (M)", "Oxidation (W)", "Dioxidation (W)", "Deamidation (N)", "Deamidation (Q)", "ICAT-D:2H(8) (C)", "ICAT-C:13C(9) (C)", "iTRAQ (N-term)", "iTRAQ (K)", "Cation:Na (E,D)", "Cation:K (E,D)", "Phospho (S)", "Phospho (T)", "Phospho (Y)", "Sulfo (Y)", "Acetyl (K)"}, {"57.021464@C", "442.224991@C", "227.126991@C", "58.005479@C", "105.057849@C", "71.037114@C", "144.102063@[,144.102063@K", "15.994915@M", "31.98983@M", "15.994915@W", "31.98983@W", "0.9848@N", "0.9848@Q", "8.0502@C", "9.0302@C", "144.102063@[", "144.102063@K", "21.982@D,21.982@E", "37.955885@D,37.955885@E", "79.966331@S", "79.966331@T", "79.966331@Y", "79.956815@Y", "42.010565@K"}};

        return arMappingTable;
    }
}
