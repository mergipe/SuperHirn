///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//
// **********************************************************************//
// CLASS ALIGN_ERROR:
// align_estimate features pair:
//
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//


#ifndef ALIGN_ERROR_H
#define ALIGN_ERROR_H
#define USE_STDIO
#define USE_NAMESPACE


class align_error{


    ////////////////////////////////////////////////
    // declaration of the private members:

private:

    ////////////////////////////////////////////////
    // declaration of the public members:

public:

    // tr_A is the retention which was not corrected
    double tr_A;
    // tr_B is the retention time which is or will be corrected
    double tr_B;
    double delta;
    double SD;

    align_error(const align_error& tmp){
      SD =  tmp.SD;
      tr_A =  tmp.tr_A;
      tr_B =  tmp.tr_B;
      delta =  tmp.delta;
    };
    
    align_error(double tr_A_in, double tr_B_in, double delta_in, double SD_in){

        tr_A = tr_A_in;
        tr_B = tr_B_in;
        delta = delta_in;
        SD = SD_in;
    }

    void show_info(){
        printf("%f\t%f\t%f\n",tr_B,delta,SD);
    };
    
    align_error& operator=(const align_error& tmp){
        SD =  tmp.SD;
        tr_A =  tmp.tr_A;
        tr_B =  tmp.tr_B;
        delta =  tmp.delta;
        return *this;
    };
    
   bool operator>=(const align_error& tmp){
        return (tr_A >= tmp.tr_A);
   };
   bool operator==(const align_error& tmp){
       return (tr_A == tmp.tr_A);
   };
   
    
   double get_delta(){return delta;};
   double get_SD(){return SD;};
   double get_tr_A(){return tr_A;};
   double get_tr_B(){return tr_B;};
   
};

#endif
