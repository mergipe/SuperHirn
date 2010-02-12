///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
// 
//
// **********************************************************************//
// CLASS tree_build_matrix:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_CELL
#define USE_MATRIX_ELEMENT
#define USE_TREE_BUILD_MATRIX

#include "CLUSTER_LC_MS_ANALYSIS_USE.h"



////////////////////////////////////////////////
// constructor for the object tree_build_matrix:
tree_build_matrix::tree_build_matrix(){

  ///////////////////////////////////////
  // define here the clustering method:
  // NJ = true: neighbor joinig
  // NJ = false: UPMGA
  NJ = false;
  //////////////////////////////////////
  cluster_nb = -1;  
}

////////////////////////////////////////////////
// constructor for the object tree_build_matrix:
tree_build_matrix::tree_build_matrix(vector<alignment_pair> IN){
  
  ///////////////////////////////////////
  // define here teh clustering method:
  // NJ = true: neighbor joinig
  // NJ = false: UPMGA
  NJ = false;
  //////////////////////////////////////
    
  cluster_nb = -1;
  vector<alignment_pair>::iterator p = IN.begin();

  double o_score = 0;
  
  ///////////////////
  // create an array of clusters == terminal elements
  while(p != IN.end()){    
    
    increase_current_cluster_nb();
    
    // insert the first element into the name array
    matrix_element* tmp = new matrix_element((*p).get_name(),get_current_cluster_nb());
    name_array.insert(pair<int,matrix_element>(get_current_cluster_nb(),*tmp));
    tmp = NULL;
    p++;
  }
  
  ////////////////////////
  // create the tree_build_matrix
  p = IN.begin();
  
  while(p != IN.end()){
    
    // create first a column:
    vector<alignment_pair> list = (*p).get_alignment_list();
    vector<alignment_pair>::iterator col = list.begin();
    
    while(col != list.end()){
      
      o_score = set_correlation_score((*col).get_correlation_after());
      cell* tmp = new cell((*p).get_name(),(*col).get_name(),o_score);
      set_element(tmp, get_index_by_name((*p).get_name()), get_index_by_name((*col).get_name()));
      tmp = NULL;
      col++;
    }
    p++;
  }
  
  
  if(NJ) {
    ////////////////////////////////////
    // adjuts now the elements according
    // to neighbourhood joining method
    // -> normalize each distance by
    // average distance!
    // Dij = dij - ( ri + rj)
    // ri = 1 / (L-2) * sum(dik)
    tree_build_matrix_normalization();
  }
  
}


//////////////////////////////////////////////////
// class desctructor of tree_build_matrix
tree_build_matrix::~tree_build_matrix(){
  
  map<int, map<int,cell> >::iterator p = M.begin();
  while(p != M.end()){
    (*p).second.clear();
    p++;
  }
  M.clear();

  name_array.clear();
  cluster_nb = 0;
  average_distances.clear();
}


//////////////////////////////////////////////////
// copy constructor:
tree_build_matrix& tree_build_matrix::operator=(const tree_build_matrix& tmp){
  return *this;
}
    

////////////////////////////////////////////////////////////////////
// function which defines an similarity score base on
// - nb features in A and B
// - overlapping features(Q)
double tree_build_matrix::set_correlation_score(double Q){
  return Q;  
}


///////////////////////////////////////////////////////////////////////
// constructs a matrix from LC_MS_similarity_matrix
void tree_build_matrix::build_up_matrix(LC_MS_similarity_matrix* IN){
  
  map<int, map<int, SM_cell> >::iterator X = IN->get_X_ELEMENT_START();
  while( X != IN->get_X_ELEMENT_END() ){
    
    map<int, SM_cell>::iterator Y = (*X).second.begin();
    
    while( Y != (*X).second.end()){
      
      SM_cell* ELE = &((*Y).second);
      
      double this_score = set_correlation_score( ELE->get_OUT_SCORE() );
      cell* tmp = new cell( ELE->get_X_name(), ELE->get_Y_name(), this_score);
      set_element(tmp, ELE->get_X(), ELE->get_Y());
      tmp = NULL;
      
      Y++;
    }
    X++;
  }
  
  
  if(NJ) {
    ////////////////////////////////////
    // adjuts now the elements according
    // to neighbourhood joining method
    // -> normalize each distance by
    // average distance!
    // Dij = dij - ( ri + rj)
    // ri = 1 / (L-2) * sum(dik)
    tree_build_matrix_normalization();
  }
  
  IN = NULL;
}


///////////////////////////////////////////////////////////////////////
// merges elements and then returns the number of the
// created cluster ( starts at nb. terminal elements + 1)
int tree_build_matrix::merge_elements(int A, int B){
  
  int futur, past;
  
  // define which one to delete and which one to keep  
  if(A > B){
    futur = B;
    past = A;
  }
  else{
    futur = A;
    past = B;
  }
  
  
  ////////////////////////////////////////////////////////////////
  // now adjust the similarity scores of the new cluster k (="future_name")
  // calculated as following
  
  string futur_name = get_element_name(futur);
  string past_name = get_element_name(past);
  

  for(unsigned int i=0; i< name_array.size();i++){
    
    string this_name = get_element_name(i);
    
    if((this_name != futur_name) && (this_name != past_name)){
      
      float score_f = get_score_by_name(futur_name, this_name);  
      float score_p = get_score_by_name(past_name, this_name);
      float score_m = get_score_by_name(futur_name, past_name);
      float m_score = 0;
      
      
      if(NJ){
        m_score = (score_f + score_p - score_m ) / 2;
      }
      else{ 
        m_score = (score_f + score_p) / 2;
      }
      
      // insert the merge score into the future field:
      set_score_by_name(futur_name, this_name, m_score);
    }
  }

  // now modifiy the futur element names in the tree_build_matrix
  merge_element_name(futur, past);

  // now delete the past:
  erase_element(past);


  if(NJ){  
    ////////////////////////////////////
    // adjuts now the elements according
    // to neighbourhood joining method
    // -> normalize each distance by
    // average distance!
    // Dij = dij - ( ri + rj)
    // ri = 1 / (L-2) * sum(dik)
    tree_build_matrix_normalization();  
  }

  
  return get_current_cluster_nb();

}

/////////////////////////////////////////////////////////////////////
// functions to access the tree_build_matrix elements:
void tree_build_matrix::erase_element(int DEAD){


  // get the right column
  map<int, map<int,cell> >::iterator col = M.find(DEAD);
  
  if( col != M.end())
    M.erase(col);
  
  int key = 0;
  map<int,cell> tmp;
  
  // move now the following
  int count = DEAD + 1;
  col = M.find(count);
  
  while(col != M.end()){
    
    key = (*col).first - 1;
    tmp = (*col).second;
    M.erase(col);
    M.insert(pair<int, map<int,cell> >(key,tmp));
    
    count++;
    col = M.find(count);
  }
  
  // now delete in the rows:
  map<int,cell>::iterator row;
  cell tmp2;
  unsigned int col_count = 0;
  
  
  while(col_count < M.size()){
    
    // get first the column:
    col = M.find(col_count);

    // now find the correct element:
    // find the one to erase:
    row = (*col).second.find(DEAD);
    
    
    // go now through all elements and move them
    if(row != (*col).second.end()){
      //delete 	
      (*col).second.erase(row);
    }
    
    
    /////////////////////////////////////////
    // move others
    count = DEAD + 1;
    
    while(count <= get_M_size()){
      
      row = (*col).second.find(count);
      
      //cout<<count<<"?";
      
      if( row != (*col).second.end() ){
	key = (*row).first - 1;
	tmp2 = (*row).second;
	
	//cout<<tmp2.get_score();
	
	(*col).second.erase(row);
	(*col).second.insert(pair<int,cell>(key,tmp2));
      }
      
      //cout<<endl;
      count++;
      
    }
    ///////////////////////////////////////
     
    // go to next columne:
    col_count++;
  }
}


///////////////////////////////////////////////////////////////////
// adjuts now the elements according
// to neighbourhood joining method
// -> normalize each distance by
// average distance!
// Dij = dij - ( ri + rj)
// ri = 1 / (L-1) * sum(dik)
void tree_build_matrix::tree_build_matrix_normalization(){

  // print_tree_build_matrix();
  
  
  average_distances.clear();
  double divisor = get_M_size() - 1;
  
  // make an array of avarage distances for every element:
  for(int i=0; i<get_M_size();i++){
    
    double average = 0;
    cell* tmp = NULL;
    
    
    // first in x-direction:
    for(int y=0; y<get_M_size();y++){
      tmp = get_element(i,y);
      if(tmp != NULL)
	average += tmp->get_score(); 
      
    }
    
    // first in y-direction:
    for(int x=0; x<get_M_size();x++){
      tmp = get_element(x,i);
      if(tmp != NULL)
	average += tmp->get_score();
    }
    
    average /= divisor;

    // now save the average
    average_distances.insert(pair<int,double>(i,average));
    
    // cout<<i<<"-"<<average<<"   ";
  }
  
  
  
  // refill the new tree_build_matrix with the normalized values:
  for(int x=0; x<get_M_size();x++){
    
    double av_dis_A = (*(average_distances.find(x))).second;
    cell* tmp = NULL;
    
    for(int y=x+1; y<get_M_size(); y++){
      
      double av_dis_B = (*(average_distances.find(y))).second;
      
      // check if there is a filled cell:
      tmp = get_element(x,y);
      if(tmp != NULL){
	double score = tmp->get_score();
	score = score - ( av_dis_B + av_dis_A );
	tmp->set_score(score);
	tmp = NULL;
      }
      
    }
  }
  
  //print_tree_build_matrix();
  
}




/////////////////////////////////////////////////////////////////////
// functions to access the tree_build_matrix elements:
cell* tree_build_matrix::get_element(int x, int y){


    // get the right column
    map<int, map<int,cell> >::iterator col = M.find(x);

    if( col == M.end())
      return NULL;
    else{
      // get now the row:      
      map<int,cell>::iterator row = (*col).second.find(y);
      
      if( row == (*col).second.end())
	return NULL;
      else
	return &((*row).second);
    }
}


/////////////////////////////////////////////////////////////////////
// functions to insert the tree_build_matrix elements:
void tree_build_matrix::set_element(cell* IN, int x, int y){

  // get the right column
  map<int, map<int,cell> >::iterator col = M.find(x);
  
  // insert a column
  if(col == M.end()){
    // create the column map:
    map<int,cell> column;
    // insert it
    M.insert(pair< int, map<int,cell> >(x, column));
    // get now the right column
    col = M.find(x);
  }
  
  
  // insert now the cell at position y
  (*col).second.insert(pair<int,cell>(y,*IN));
   
}

////////////////////////////////////////////////////////////////////
// get name of the tree_build_matrix elemnet in row/ column i:
string tree_build_matrix::get_element_name(int i){

  map<int, matrix_element>::iterator p = name_array.find(i);

  if(p != name_array.end())
    return (*p).second.get_name();
  else{
    return "";  
  }
}

////////////////////////////////////////////////////////////////////
// get name of the tree_build_matrix elemnet in row/ column i:
int tree_build_matrix::get_element_id(int i){

  map<int, matrix_element>::iterator p = name_array.find(i);
  
  if(p != name_array.end())
    return (*p).second.get_id();
  else{
    return -1;  

  }
}

////////////////////////////////////////////////////////////////////
// get index of the tree_build_matrix elemnet in row/ column i by name:
int tree_build_matrix::get_element_name_index(string IN){

  map<int, matrix_element>::iterator p = name_array.begin();
 
  while(p != name_array.end()){
    
    if((*p).second.get_name() == IN){
      return (*p).first;
    }
    p++;
  }

  return -1;

}


////////////////////////////////////////////////////////////////////
// set name of the tree_build_matrix elemnet in row/ column i:
void tree_build_matrix::merge_element_name(int NEW, int OLD){

  // pre calculate the edges of the new cluster:
  double score = get_element(NEW,OLD)->get_score();

  map<int, double>::iterator P = average_distances.find(NEW);
  double av_A = 0;
  double av_B = 0;

  if(NJ){
    if( P != average_distances.end()){
      av_A = (*P).second;
    }
    P = average_distances.find(OLD);
    if( P != average_distances.end()){
      av_B = (*P).second;
    }
  }

  double edge_A = (score + av_A - av_B) / 2;
  double edge_B = (score + av_B - av_A) / 2;
  
  // create a new name:
  string merge_name;
  merge_name.assign(get_element_name(NEW),0,get_element_name(NEW).find("."));
  merge_name += "_" + get_element_name(OLD);
  
  // increase the cluster number:
  increase_current_cluster_nb();
  
  // delete from the name_array the OLD element:
  map<int, matrix_element>::iterator p = name_array.find(OLD);  
  name_array.erase(p);
  
  // adjust the keys of the rows which are above the one deleted:
  matrix_element tmp;
  int key;
  
  OLD++;
  p = name_array.find(OLD);
  
  while(p != name_array.end()){
    
    key = (*p).first - 1;
    tmp = (*p).second;
    name_array.erase(p);
    name_array.insert(pair<int, matrix_element>(key,tmp));
    OLD++;
    p = name_array.find(OLD);
  }
  
  // insert the new name  
  p = name_array.find(NEW);
  (*p).second.set_name(merge_name);
  (*p).second.set_id(get_current_cluster_nb());
  
  // set the edges of the new cluster:
  (*p).second.set_edge_A(edge_A);
  (*p).second.set_edge_B(edge_B);
  
}


////////////////////////////////////////////////////////////////////
// get index of an element in the tree_build_matrix by name:
int tree_build_matrix::get_index_by_name(string IN){
  
  map<int, matrix_element>::iterator p = name_array.begin();
  
  while(p != name_array.end()){
    if((*p).second.get_name() == IN)
      return (*p).first; 
    p++;
  }
  
  return -1;
  
}


////////////////////////////////////////////////////////////////////
// get element score by name:
float tree_build_matrix::get_score_by_name(string A, string B){
 
  int i = get_element_name_index(A);
  int j = get_element_name_index(B);

  cell* tmp = get_element(i,j);
  if(tmp != NULL)
    return tmp->get_score();
  else{
    tmp = get_element(j,i);
    if(tmp != NULL)
      return tmp->get_score();
    else
      return 0;
  }
}

////////////////////////////////////////////////////////////////////
// get element score by name:
void tree_build_matrix::set_score_by_name(string A, string B, float score){
 
  int i = get_element_name_index(A);
  int j = get_element_name_index(B);

  cell* tmp = get_element(i,j);
  if(tmp != NULL)
    tmp->set_score(score);
  else{
    tmp = get_element(j,i);
    if(tmp != NULL)
      tmp->set_score(score);
    else
      printf("\nERROR::could not set score %f for element(%s|%s),tree_build_matrix::580\n",score,A.c_str(),B.c_str());
  }
}



////////////////////////////////////////////////////////////////////
// get dimension of the tree_build_matrix:
int tree_build_matrix::get_M_size(){
  return name_array.size();
}


////////////////////////////////////////////////////////////////////
// get the number of elements in the tree_build_matrix:
int tree_build_matrix::get_nb_elements(){
  return name_array.size();
}



//////////////////////////////////////////////////////////////////////
// print content of tree_build_matrix:
void tree_build_matrix::print_tree_build_matrix(){

  string sep = ";";
  
  printf("\nSimilarity tree_build_matrix for guide tree:\n");
  cell* tmp;
  
  printf("%s",sep.c_str());
  
  for(int i=0; i<get_M_size();i++){
    
    //printf("%d%s",get_element_id(i),sep.c_str());
    printf("%s(%d)%s",get_element_name(i).c_str(),get_element_id(i),sep.c_str());
    
  }
  
  
  for(int j=0; j<get_M_size();j++){
    
    printf("\n%d%s",get_element_id(j),sep.c_str());
    // printf("\n%s(%d)%s",get_element_name(j).c_str(),get_element_id(j),sep.c_str());
    
    
    for(int i=0; i<get_M_size();i++){
      
      tmp = get_element(j,i);
      
      
      if(tmp != NULL)
	printf("%f%s",tmp->get_score(),sep.c_str());
      else
	printf("-%s",sep.c_str());
      
    }
    
    
  }
  
  
  cout<<endl<<endl;
  
}


//////////////////////////////////////////////////////////////////////
// find element with max score in tree_build_matrix
void tree_build_matrix::find_min(int* X, int* Y){

  double min = -10000000;

  for(int j=0; j<get_M_size();j++){
    
    for(int i=0; i<get_M_size();i++){
      
      cell* tmp = get_element(j,i);
      
      if(tmp != NULL){
        if(tmp->get_score() >  min){
          min = tmp->get_score();
          *X = j;
          *Y = i;
        }
      }
    }
  }
}


//////////////////////////////////////////////////////////////////////
// access the cluster edges by id:
double tree_build_matrix::get_cluster_edge_A(int id){

  map<int,  matrix_element>::iterator p = name_array.begin();

  while(p != name_array.end()){
    //  cout<<(*p).second.get_id()<<" "<<endl;
    
    if((*p).second.get_id() == id)
      return (*p).second.get_edge_A();
    
    p++;
  }
  
  // not found, return 0
  return 0;  

}

//////////////////////////////////////////////////////////////////////
// access the cluster edges by id:
double tree_build_matrix::get_cluster_edge_B(int id){
  
  map<int, matrix_element>::iterator p = name_array.begin();
  
  while(p != name_array.end()){
    //cout<<(*p).second.get_id()<<" "<<endl;
    
    if((*p).second.get_id() == id)
      return (*p).second.get_edge_B();
    
    p++;
  }
  
  // not found, return 0
  return 0;

}


//////////////////////////////////////////////////////////////////////
// print content of tree_build_matrix to a file:
void tree_build_matrix::print_tree_build_matrix_to_file(){
  
  // get the input name from the input source directory:
  read_param* def = new read_param();
  
  // read input directory:
  string file = "ANALYSIS_";
  file += def->search_tag("MY PROJECT NAME");
  delete def;
  def = NULL;
  
  // remove backslash at end:
  if(file[file.size()-1] == '/')
    file.erase(file.size()-1,file.size()-1);
  
  // process the input path:
  unsigned int i = file.rfind("/");
  // no signe found:
  if( i != string::npos ){ 
    file.erase(0,i+1);
  }
  string tmp = "ANALYSIS_";
  tmp += file + "/";
  file.insert(0,tmp);
  
  // add extension for tree format
  file += "_sim_tree_build_matrix";
  file += ".txt";	
 
  // open a file, etc:
  ofstream fout;
  fout.open(file.c_str(), ofstream::out); 
  
  if(fout.good()){

    string sep = "\t";
    printf("\nPrint Similarity tree_build_matrix...\n");
    cell* tmp;
    printf("%s",sep.c_str());
    char buffer[256];
    
    for(int i=0; i<get_M_size();i++){
      sprintf(buffer,"%d%s",get_element_id(i),sep.c_str());   
      //fout.write(buffer,strlen(buffer));
    }
    
    for(int j=0; j<get_M_size();j++){
      
      sprintf(buffer,"\n%d:%s",get_element_id(j),sep.c_str());   
      //fout.write(buffer,strlen(buffer));
      sprintf(buffer,"\n");   
      fout.write(buffer,strlen(buffer));
      
      for(int i=0; i<get_M_size();i++){
        tmp = get_element(j,i);
        
        if(tmp != NULL){
          sprintf(buffer,"%f%s",tmp->get_score(),sep.c_str());
          fout.write(buffer,strlen(buffer));
        }
        else{
          sprintf(buffer,"0%s",sep.c_str()); 
          fout.write(buffer,strlen(buffer));
          
        }    
      }
    }
    cout<<endl;
    fout.close();
  }
  else{
    printf("\nERROR: Similarity tree_build_matrix could not be saved to '%s', tree_build_matrix.cc::776\n",file.c_str());
  }

}


//////////////////////////////////////////////////////////////////////
// make a picture of the tree_build_matrix:
void tree_build_matrix::print_tree_build_matrix_image_to_file(){
  
  /*
  ////////
  // get the input name from the input source directory:
  read_param* def = new read_param();
  // read input directory:
  string image_file;
  image_file += def->search_tag("MZXML DIRECTORY");
  delete def;
  def = NULL;
  // remove backslash at end:
  if(image_file[image_file.size()-1] != '/')
    image_file += "/";
  
  // add extension for image format
  image_file += "image_sim_tree_build_matrix";
  image_file += ".ps";	

  SIM_matrix_plotter* PLO = new SIM_matrix_plotter(image_file);
  PLO->set_matrix_size(get_M_size());
  PLO->init("");
  
  for(int i=0; i<get_M_size();i++){
    PLO->label_matrix(i, get_element_id(i));
  }
   */
}


//////////////////////////////////////////////////////////////
// insert element into the name array:
void tree_build_matrix::insert_into_name_array(string name){
  
  increase_current_cluster_nb();
  
  // insert the first element into the name array
  matrix_element* tmp = new matrix_element(name,get_current_cluster_nb());
  name_array.insert(pair<int,matrix_element>(get_current_cluster_nb(),*tmp));
  tmp = NULL;
}