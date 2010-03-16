/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package PPP_Analyzer;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.text.DateFormat;
import java.util.Date;
import java.util.Scanner;

/**
 *
 * @author Silas
 */
public class CDHitClusterOutputParser {

    String clusterFile = null;
    Scanner s = null;
    Date errTime = null;

    public CDHitClusterOutputParser(String clusterFile) {
        this.clusterFile = clusterFile;
    }

    /**
     * @param args the command line arguments
     */
    public boolean parseClusterFile(String clusterName) {
        // TODO code application logic here
        boolean retVal = true;
        try {
            s = new Scanner(new BufferedReader(new FileReader(clusterFile)));
            s.useDelimiter(">Cluster");
            String sql = "insert into db_pass_gautam_stg.ts_cluster_results" +
                    "(cluster_name,cluster_number,cluster_rank,protein_length,protein_accession_id,hasStar,similarity_percentage) values";
            int clusterCounter = 1;
            if (s.hasNext()) {
                int recordsLoaded = Main.objDataAccess.WriteRecord("delete from db_pass_gautam_stg.ts_cluster_results where cluster_name='" + clusterName + "'");
                do {
                    String queryToExecute = "", proteinsMatched[][] = getAccessionNo(s.next().split("\n"));
                    if (proteinsMatched == null) {
                        continue;
                    }

                    for (int proteinCounter = 0; proteinCounter < proteinsMatched.length; proteinCounter++) {
                        if ((1 + proteinCounter) == (proteinsMatched.length)) {
                            queryToExecute += "('" + clusterName + "'," + Integer.parseInt(proteinsMatched[proteinCounter][0]) + "," + Integer.parseInt(proteinsMatched[proteinCounter][1]) + "," + Integer.parseInt(proteinsMatched[proteinCounter][2]) + ",'" + proteinsMatched[proteinCounter][3] + "'," + Integer.parseInt(proteinsMatched[proteinCounter][4]) + "," + Integer.parseInt(proteinsMatched[proteinCounter][5]) + ")";
                        } else {
                            queryToExecute += "('" + clusterName + "'," + Integer.parseInt(proteinsMatched[proteinCounter][0]) + "," + Integer.parseInt(proteinsMatched[proteinCounter][1]) + "," + Integer.parseInt(proteinsMatched[proteinCounter][2]) + ",'" + proteinsMatched[proteinCounter][3] + "'," + Integer.parseInt(proteinsMatched[proteinCounter][4]) + "," + Integer.parseInt(proteinsMatched[proteinCounter][5]) + "), ";
                        }

                        System.out.println("Cluster Number: " + proteinsMatched[proteinCounter][0] + "\tRank: " + proteinsMatched[proteinCounter][1] + "\tLength: " + proteinsMatched[proteinCounter][2] + "\tAccession ID: " + proteinsMatched[proteinCounter][3] + "\tHas Star: " + proteinsMatched[proteinCounter][4] + "\tSimilarity Percentage: " + proteinsMatched[proteinCounter][5]);
                    }

                    //if (clusterCounter == 0) {

                    recordsLoaded = Main.objDataAccess.WriteRecord(sql + queryToExecute);
                    if (recordsLoaded > 0) {
                        System.out.println("\n*** Loaded Data Successfully ***\n");
                    } else {
                        System.out.println("\n*** Failed To Loaded Data Successfully ***\n");
                    }
                    clusterCounter++;
                    /**/
                    //}
                } while (s.hasNext());
            }
        } catch (FileNotFoundException ex) {
            errTime = new Date();
            Main.currErrorMessage = DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + ": " + ex.getMessage();
            System.err.println(DateFormat.getDateTimeInstance(DateFormat.MEDIUM, DateFormat.MEDIUM).format(errTime).toString() + ": " + ex.getMessage());
            ex.printStackTrace();
            Main.emailError.SendEmail();
            System.exit(1);
            retVal = false;
        } finally {
            if (s != null) {
                s.close();
            }
        }
        return retVal;
    }

    private String[][] getAccessionNo(String[] clusterArray) {
        String[][] proteinsMatched = new String[(clusterArray.length - 1)][6];
        /*
         * proteinsMatched array shall be formatted according to:
         * index 0: cluster number
         * index 1: cluster rank
         * index 2: protein length
         * index 3: protein accession id
         * index 4: has star (1 for Yes, 0 for No)
         * index 5: similarity percentage
         */
        /*
         * clusterArray contains an array of proteins found within a cluster
         */
        for (int arrayCounter = 0; arrayCounter < proteinsMatched.length; arrayCounter++) {
            //if (clusterArray.length <= 2) {
            //  return null;
            //}
            proteinsMatched[arrayCounter][0] = clusterArray[0].trim();
            proteinsMatched[arrayCounter][1] = clusterArray[1 + arrayCounter].trim().substring(0, clusterArray[1 + arrayCounter].trim().indexOf("\t"));
            proteinsMatched[arrayCounter][2] = clusterArray[1 + arrayCounter].trim().substring(1 + clusterArray[1 + arrayCounter].trim().lastIndexOf("\t"), clusterArray[1 + arrayCounter].trim().indexOf("aa"));
            proteinsMatched[arrayCounter][3] = clusterArray[1 + arrayCounter].trim().substring(1 + clusterArray[1 + arrayCounter].trim().indexOf(">"), clusterArray[1 + arrayCounter].trim().indexOf("..."));
            if (clusterArray[1 + arrayCounter].trim().substring(clusterArray[1 + arrayCounter].trim().length() - 1).compareTo("*") == 0) {
                proteinsMatched[arrayCounter][4] = "1";
                proteinsMatched[arrayCounter][5] = "100";
            } else {
                proteinsMatched[arrayCounter][4] = "0";
                proteinsMatched[arrayCounter][5] = clusterArray[1 + arrayCounter].trim().substring(1 + clusterArray[1 + arrayCounter].lastIndexOf(" "), clusterArray[1 + arrayCounter].trim().length() - 1);
            }
            //System.out.println("\n\nProtein: "+clusterArray[1 + arrayCounter].trim()+"\tLast Char: "+clusterArray[1 + arrayCounter].trim().substring(clusterArray[1 + arrayCounter].trim().length() - 1)+"\n\n");
        }
        return proteinsMatched;
    }
}
