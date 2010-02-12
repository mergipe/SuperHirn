///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS progress_bar:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H



class progress_bar{

    
    ////////////////////////////////////////////////
    // declaration of the private members:

private:

    unsigned long nb_iterations;

    double update_step;

    double progress;
    
    bool print_endline;
    
    string progress_text;

    int internal_counter;

    ////////////////////////////////////////////////
    // declaration of the public members:

public:

    // class destructor
    ~progress_bar();

    // class constructor
    progress_bar(int, string);
    progress_bar(int);

    //////////////////////////////////////////////////
    // copy constructor:
    progress_bar& operator=(const progress_bar&);

    // calculated the progress:
    void set_progress();

    // update the progress:
    void update_progress(int);
    void update_progress();


    // print the progress:
    void print_progress(int);


        

    void reset_nb_iterations(int IN_iterations){nb_iterations=(unsigned long)(IN_iterations);};
    void reset_nb_iterations(){nb_iterations=0;};
    
    ///////////////////////////////
    // start here all the get / set
    // function to access the
    // variables of the class

    void set_update_step(double IN){update_step = IN;};
    double get_update_step(){return update_step;};

};

#endif
