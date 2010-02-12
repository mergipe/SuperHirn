///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS result_manager:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef RESULT_MANAGER_H
#define RESULT_MANAGER_H



class result_manager{

    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:

    ////////////////////////////////////////////////
    // declaration of the public members:

public:

    // class destructor
    ~result_manager();

    // class constructor
    result_manager();

    vector<alignment_pair> alignment_partner;


    //////////////////////////////////////////////////
    // overload operators:
    result_manager& operator=(const result_manager&);
    result_manager& operator<=(const result_manager&);
    result_manager& operator>=(const result_manager&);
    result_manager& operator<(const result_manager&);
    result_manager& operator>(const result_manager&);


    ///////////////////////////////////////////////////
    // create a new alignment partner which can then be compared to others:
    void set_new_alignment_partner(string);
    
    // create a new alignment pair
    void create_new_pair(string, string);

    // calculate the overalap of an alignment pair BEFORE
    // alignment
    void set_correlation_before(string, string, double);
    // calculate the overalap of an alignment pair AFTER
    // alignment
    void set_correlation_after(string, string, double);
        
    // search the alignment partner list for a pratner by name
    alignment_pair* get_partner_by_name(string);

    // shows the alignment result in nice table format:
    void show_alignment_overview();
    // show alignment info
    void show_info();

    // print overview tutel
    void print_titel();

    ///////////////////////////////
    // start here all the get / set
    // function to access the
    // variables of the class

    // return the inserted alignment pairs:
    vector<alignment_pair> get_alignment_pairs(){return alignment_partner;};
};

#endif

    
