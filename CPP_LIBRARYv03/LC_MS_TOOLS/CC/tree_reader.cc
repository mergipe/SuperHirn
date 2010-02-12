///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS tree_reader:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_NODE
#define USE_TREE_READER
#include "LC_MS_TOOLS_USE.h"



////////////////////////////////////////////////
// constructor for the object tree_reader:
tree_reader::tree_reader(){
 
  FIN = NULL;
  root = NULL;
  
  // get the input name from the input source directory:
  read_param* def = new read_param();
  
  // read input directory:
  tree_file += def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;

  // remove backslash at end:
  if(tree_file[tree_file.size()-1] != '/')
    tree_file += '/';

  string tmp = "ANALYSIS_";
  tmp += tree_file;
  
  // add extension for tree format
  tree_file = tmp + "my_guide_tree.tre";	
  
}

////////////////////////////////////////////////
// constructor for the object tree_reader:
tree_reader::tree_reader(char* IN){
  tree_file = IN;
  FIN = NULL;
  root = NULL;
}
////////////////////////////////////////////////
// constructor for the object tree_reader:
tree_reader::tree_reader(string IN){
  tree_file = IN;
  FIN = NULL;
  root = NULL;
}


//////////////////////////////////////////////////
// class desctructor of tree_reader
tree_reader::~tree_reader(){
	
		if(FIN != NULL){
    delete FIN;
    FIN = NULL;
		}
		
  root = NULL;
}

//////////////////////////////////////////////////
// class copy constructor of tree_reader
tree_reader::tree_reader(const tree_reader& tmp){
  tree_file = tmp.tree_file;
}

//////////////////////////////////////////////////
// class copy constructor of tree_reader
tree_reader::tree_reader(const tree_reader* tmp){
  tree_file = tmp->tree_file;
}


//////////////////////////////////////////////////
// copy constructor:
tree_reader& tree_reader::operator=(const tree_reader& tmp){
  tree_file = tmp.tree_file;
  return *this;
}

///////////////////////////////////////////////////
// close a file:
void tree_reader::close_file(){
  
  if(FIN != NULL){
    FIN->close();
    
    // delete the instance:
    delete FIN;
    FIN = NULL;
  }
}

///////////////////////////////////////////////////
// open a file:
bool tree_reader::open_file(){
  
		close_file();
		
		if(FIN != NULL){
    delete FIN;
    FIN = NULL;
		}
		
		FIN = new ifstream();   
  
		FIN->open(tree_file.c_str());
		
		if(FIN->good()){
    return true;
		}
		else{
    printf("\n\t\tWarning: could not parse alignment tree from '%s'\n", tree_file.c_str());
    return false;
		}
}

//////////////////////////////////////////////////////
// check end of file
bool tree_reader::end_of_file(){
  if(FIN->eof())
    return true;
  else
    return false;
}

//////////////////////////////////////////////////////
// reads a character in teh file
char tree_reader::get_character(){
  return FIN->get();
}


//////////////////////////////////////////////////////
// reads the tree-file
void tree_reader::read_tree_file(){
  
  node* current = NULL;
  
  string node_name;
  string edge;
  
  // open the file:
  if(open_file()){
    
    char T = ' ';
    
    while( (!end_of_file()) && (T != ';')){
      
      T = get_character();
      
      ////////////////////////////////////////////////
      // decide now what to do:
      switch(T){
        
        ////////////////////////////////////////////////
        // create a new internal node, start
        case '(':
          current = create_new_internal_node(current);
          break;
          
          ////////////////////////////////////////////////
          // move a level up towards root
        case ')':
          
          if(current->get_upper() != NULL){				
            
            T = get_character();
            
            T = get_character();
            // read on until, or ( or ):
            while((T != '(') && (T != ',') && (T != ';') && (T != ')') ){
              edge +=T;
              T = get_character();
            }
            FIN->seekg(-1,ios::cur);
            
            
            if(current->get_upper()->get_below_left() == current){
              current = current->get_upper();
              current->set_edge_left(atof(edge.c_str()));
            }
            else{
              current = current->get_upper();
              current->set_edge_right(atof(edge.c_str()));
            }
            edge.erase();
          }
          
          break;
          
          ///////////////////////////////////////////////
          // all the characters to jump over:
        case ',':
          break;
        case ' ':
          break;
        case ';':
          break;
          ///////////////////////////////////////////////
          
          
          ///////////////////////////////////////////////
          // insert a new terminal node:
        default:
          
          // read the name:
          while((T != ' ') && (T != ',') && (T != ':') && (T != ';')){
            node_name += T;
            T = get_character();
          }
          
          if(T == ';'){
            break;
          }
          
          
          T = get_character();
          // read on until
          while((T != '(') && (T != ',') && (T != ')') && (T != ';')){
            edge +=T;
            T = get_character();
          }
            
            // insert the new node now:
            if(node_name.size() > 0 ){
              create_new_terminal_node(current, node_name, edge);
              node_name.erase();
            }
            
            edge.erase();
										
          //move back with current position
          if(T != ';')
            FIN->seekg(-1,ios::cur);
      }
      
    }
    
  }	
}


//////////////////////////////////////////////////////////
// create a new internal node:
node* tree_reader::create_new_internal_node(node* IN){
  
  // check if the root already exists:
  if(IN == NULL){
    root = new node();
    return root;
		}
  else{
    node* tmp = NULL;
    // is not the root:
    // check first if left descended is not allocated
    if(IN->get_below_left() == NULL){
      IN->allocate_below_left();
      tmp = IN->get_below_left(); 
    }
    else{
      IN->allocate_below_right();
      tmp = IN->get_below_right(); 
    }
    return tmp;
		}
}


//////////////////////////////////////////////////////////
// create a new terminal node:
void tree_reader::create_new_terminal_node(node* IN, string name, string edge){
  
		// is not the root:
		// check first if left descended is not allocated
		if(IN->get_below_left() == NULL){
    // set the edge:
    double E = atof(edge.c_str()); 
    IN->set_edge_left(E);
    IN->allocate_below_left(name);
		}
		else{
    // set the edge:
    double E = atof(edge.c_str()); 
    IN->set_edge_right(E);
			IN->allocate_below_right(name);
		}
}


//////////////////////////////////////////////////////////
// printe the tree to screen:
void tree_reader::print_tree(){
  root->print_tre_format();
}

///////////////////////////////////////////////////////////////////
// save the constructed guide tree to file
void tree_reader::save_guide_tree(){
  
  printf("\n\t-- Guide Tree structure was save to file '%s'.\n",tree_file.c_str());
  
  // open a file, etc:
  ofstream* fout = new ofstream();
  // upper:
  fout->open (tree_file.c_str(), ofstream::out);
  // write nodes to file:
  root->write_node(fout);
  
  fout->close();
  delete fout;
  fout = NULL;
}




