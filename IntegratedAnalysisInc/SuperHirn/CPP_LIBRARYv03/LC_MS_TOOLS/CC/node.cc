///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS node:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_NODE
#define USE_LEAF
#define USE_ALIGNMENT_PAIR

#include "LC_MS_TOOLS_USE.h"




////////////////////////////////////////////////
// constructor for the object node:
node::node(){
  upper = NULL;
  below_A = NULL;
  below_B = NULL;
  edge_A = 0;
  edge_B = 0;
  level= 0;
  terminal = false;
  id = -1;
  nb_peaks = 0;
}

////////////////////////////////////////////////
// constructor for the object node:
node::node(int IN_ID, string IN_name){
  upper = NULL;
  below_A = NULL;
  below_B = NULL;
  edge_A = 0;
  edge_B = 0;
  level= 0;
  terminal = false;
  id = IN_ID;
  name = IN_name;
  nb_peaks = 0;
}

// constructor for the object leaf from an
// alignment pair:
node::node(alignment_pair* IN, int IN_id){
  edge_A = 0;
  edge_B = 0;
  id = IN_id;
  // get info from alignment pair:
  name = IN->get_name();
  // set terminal
  terminal = true;
  level = 0;	
  below_A = NULL;
  below_B = NULL;
  upper = NULL;
}

////////////////////////////////////////////////
// constructor for the object node:
node::node(string IN){
	
  // check if the name contains
  // a LC-MS run id:
  // like: XYZ.txt[2]
  string tmp_id;
  unsigned int i = IN.rfind("]");
  if(i != string::npos){
    unsigned int j = IN.rfind("[");
    if(j != string::npos){
      tmp_id.assign(IN,j+1,i-j-1);
			IN.erase(j,i);
    }
		}
  if(tmp_id.size() > 0)
    id = atoi(tmp_id.c_str());
  else
    id = -1;
  
  upper = NULL;
  below_A = NULL;		
  below_B = NULL;
  edge_A = 0;
  edge_B = 0;
  name = IN;
  level= 0;
  terminal = true;
  nb_peaks = 0;
  
}


//////////////////////////////////////////////////
// copy constructor:
node::node(const node& tmp){
  edge_A = tmp.edge_A;
  edge_B = tmp.edge_B;
  id = tmp.id;
  below_A = tmp.below_A;
  below_B = tmp.below_B;
  upper = tmp.upper;
  terminal = tmp.terminal;
  level = tmp.level;
  name = tmp.name;
  nb_peaks = tmp.nb_peaks;
}

//////////////////////////////////////////////////
// copy constructor:
node::node(const node* tmp){
  edge_A = tmp->edge_A;
  edge_B = tmp->edge_B;
  id = tmp->id;
  below_A = tmp->below_A;
  below_B = tmp->below_B;
  upper = tmp->upper;
  terminal = tmp->terminal;
  level = tmp->level;
  name = tmp->name;
  nb_peaks = tmp->nb_peaks;
}



////////////////////////////////////////////////
// constructor for the object node:
node::node(node* A, node* B, int IN_level, int IN_id){

  edge_A = 0;
  edge_B = 0;
  
  id = IN_id;
  
  below_A = A;
  below_B = B;
  upper = NULL;
  
  // set upper from the elements
  A->set_upper(this);
  B->set_upper(this);
  
   // create a new name:
  name.assign(A->get_name(),0,A->get_name().find("."));
  name += "_" + below_B->get_name();
  
  terminal = false;
  level = IN_level;

  nb_peaks = 0;
}

//////////////////////////////////////////////////
// class desctructor of node
node::~node(){
  if(below_A != NULL){
    delete below_A;
    below_A = NULL;
		}
  if(below_B != NULL){
    delete below_B;
    below_B = NULL;
		}
  upper = NULL;
}


//////////////////////////////////////////////////
// copy constructor:
node& node::operator=(const node& tmp){
  
  edge_A = tmp.edge_A;
  edge_B = tmp.edge_B;
  id = tmp.id;
  
  if(below_A != NULL){
    delete below_A;
    below_A = NULL;
  }
  if(tmp.below_A != NULL){
    below_A = new node();
    *below_A = *(tmp.below_A);
  }
  
  if(below_B != NULL){
    delete below_B;
    below_B = NULL;
  }
  if(tmp.below_B != NULL){
    below_B = new node();
    *below_B = *(tmp.below_B);
  }
  
  if(upper != NULL){
    delete upper;
    upper = NULL;
	}
  if(tmp.upper != NULL){
    upper = new node();
    *upper = *(tmp.upper);
  }
  
  terminal = tmp.terminal;
  level = tmp.level;
  name = tmp.name;
  nb_peaks = tmp.nb_peaks;
  
  return *this;
}

/////////////////////////////////////////////////
// print the tree recirsuvely
void node::print_tree(){

  // print the node status:

  // ROOT:
  if( (upper == NULL) && (below_A != NULL) && (below_A != NULL) ){
      printf("\t[ROOT]\t\t");
      //  printf("%d Level: ID=%d(%s)\n",level,id,name.c_str());
      printf("%d Level: ID=%d, [%f | %f]\n",level,id,edge_A,edge_B);
  }
    
  // TERMINAL:
    else if( (upper != NULL) && (below_A == NULL) && (below_A == NULL) ){
      printf("\t[TERMINAL]\t");
      //  printf("%d Level: ID=%d(%s)\n",level,id,name.c_str());
      printf("%d Level: ID=%d\n",level,id);

  }

  // INTERNAL NODE
  else if( (upper != NULL) && (below_A != NULL) && (below_A != NULL) ){
      printf("\t[NODE]\t\t");
      //  printf("%d Level: ID=%d(%s)\n",level,id,name.c_str());
      printf("%d Level: ID=%d, [%f | %f]\n",level,id,edge_A,edge_B);
  }

  // ERROR IN THE TREE STRUCTURE:
  else{
       printf("\t[ERROR in tree structure:: NODE, 131]\t\t");
  }

  // print own content:

  // print the descending nodes:
  if(below_A != NULL)
    below_A->print_tree();
  if(below_B != NULL)
    below_B->print_tree();

}

/////////////////////////////////////////////////
// print the tree in "tre" -format 
void node::print_tre_format(){

	string sep = "";
	
	// print the node status:
	
    // ROOT:
    if( (upper == NULL) && (below_A != NULL) && (below_A != NULL) ){
        printf("%s(",sep.c_str());
        // print the descending nodes:
        below_A->print_tre_format();
        printf(":%f",edge_A);
        printf(",");
        below_B->print_tre_format();
        printf(":%f",edge_B);
        printf("%s);",sep.c_str());
    }

    // TERMINAL:
    else if( (upper != NULL) && (below_A == NULL) && (below_A == NULL) ){
        printf("%s%s[%d]",name.c_str(),sep.c_str(),id);
    }

    // INTERNAL NODE
    else if( (upper != NULL) && (below_A != NULL) && (below_A != NULL) ){
        printf("%s(",sep.c_str());
        // print the descending nodes:
        below_A->print_tre_format();
        printf(":%f",edge_A);
        printf(",");
        below_B->print_tre_format();
        printf(":%f",edge_B);
        printf("%s)",sep.c_str());
    }

		// only one element in tree!:
    else if( (upper == NULL) && (below_A == NULL) && (below_A == NULL) ){
			printf("(%s%s[%d]);",name.c_str(),sep.c_str(),id);
    }
		
    // ERROR IN THE TREE STRUCTURE:
    else{
			printf("\t[ERROR in tree structure:: NODE, 287]\t\t");
    }
}
/////////////////////////////////////////////////
// print the tree in "tre" -format 
void node::write_node(ofstream* fout){
	
	string sep = "";
	
	
	// ROOT:
	if( (upper == NULL) && (below_A != NULL) && (below_A != NULL) ){
		*fout<<sep<<"(";
		// print the descending nodes:
		below_A->write_node(fout);
		*fout<<":"<<edge_A;
		*fout<<",";
		below_B->write_node(fout);
		*fout<<":"<<edge_B;
		*fout<<sep<<");";
	}
	// TERMINAL:
	else if( (upper != NULL) && (below_A == NULL) && (below_A == NULL) ){
		*fout<<name<<"["<<id<<"]"<<sep;
	}
	
	// INTERNAL NODE
	else if( (upper != NULL) && (below_A != NULL) && (below_A != NULL) ){
		*fout<<sep<<"(";
		// print the descending nodes:
		below_A->write_node(fout);
		*fout<<":"<<edge_A;
		*fout<<",";
		below_B->write_node(fout);
		*fout<<":"<<edge_B;
		*fout<<sep<<")";
	}
	
	// ERROR IN THE TREE STRUCTURE:
	else{
		printf("\t[ERROR in tree structure:: NODE, 327]\t\t");
	}
}



/////////////////////////////////////////////////////////////////
// show info of this node:
void node::show_info(){
  char t='n',A='n',B='n',u='n';
  if(terminal)
    t='y';
  if(below_A != NULL)
    A='y';
  if(below_B != NULL)
    B='y'; 
  if(upper != NULL)
    u='y';

  printf("%s,id=%d,level=%d:terminal[%c],connectivity:upper[%c],below 1[%c],below 2[%c]\n",get_name().c_str(),id,level,t,u,A,B);

}
