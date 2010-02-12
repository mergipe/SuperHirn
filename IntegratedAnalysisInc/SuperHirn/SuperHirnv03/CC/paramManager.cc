
#define USE_PARAM_MANAGER
#include "use.h"




paramManager::paramManager(){
}

paramManager::~paramManager(){
  allParameters.clear();
}

paramManager::paramManager(const paramManager& tmp){
}

paramManager::paramManager(const paramManager* tmp){
}

paramManager& paramManager::operator=(const paramManager& tmp){
  return *this;
}
    

void paramManager::extractAndStoreParameters(){
  
  extractRootParameters();
  extractCostumParameters();
  replaceByGlobalParameters();
  storeALLParameterinXMLFile();
}


void paramManager::extractRootParameters(){

  // find root parameter file:
  read_param* param = new read_param();
  string rootFile = param->search_tag("ROOT PARAMETER FILE");
  delete param;
  param = NULL;
  
  // extract from file:
  extractParametersFromFile( rootFile );

}


void paramManager::replaceByGlobalParameters(){
  
  // find root parameter file:
  map<string, string>::iterator Start = read_param::global_parameters.begin();
  while( Start != read_param::global_parameters.end() )
    {
      map<string, string>::iterator F = allParameters.find( Start->first );
      if( F != allParameters.end() )
        {
          F->second = Start->second; 
        }
      Start++;
    }
}


void paramManager::extractCostumParameters(){
  
  // get costum parameter file:
  string rootFile = read_param::COSTUM_PARAM_FILE;
  
  // extract from file:
  extractParametersFromFile( rootFile );
  
  
  
}




void paramManager::extractParametersFromFile( string file){
  

  // open and extract all tags and their values:
  file_reader* reader = new file_reader();
  // set the parameter file:
  reader->set_current_file( file );
  // open file and save the status
  if( reader->open_file() ){
  
    
    // read file until the tag or end is reached:
    while( reader->get_line() != NULL){
      
      // check if tag found:
      string tmp = reader->get_line();
      if( tmp.find( "=") != string::npos){
        storeParameterLine( tmp );
      }
      
      reader->read_file_line();
    }
    
    
    reader->close_file();
  }
  delete reader;
  reader = NULL;
  
}



void paramManager::storeParameterLine( string in){
  
  string tag = in.substr(0, in.find("=") );
  string value = in.substr(in.find("=") + 1, in.size() );
  
  // save:
  map<string , string >::iterator F = allParameters.find( tag ); 
  if( F != allParameters.end() ){
    allParameters.erase( F );
  }
  
  allParameters.insert( make_pair( tag, value ) );
  
}

void paramManager::storeALLParameterinXMLFile( ){
  
  // create the outfile:
  read_param* def = new read_param();  
  string dirfile = "ANALYSIS_" + def->search_tag("MY PROJECT NAME");  
  delete def;
  def = NULL;
  if( dirfile[ dirfile.size() - 1 ] != '/'){
    dirfile += "/";
  }

  // add a time stamp:
  // get date and time:
  time_t rawtime;
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = localtime( &rawtime);  
  char tmp[254] ; 
  //sprintf( tmp, "%sparam%s", dirfile.c_str(), asctime(timeinfo) );
  sprintf( tmp, "%sSuperHirnParameters", dirfile.c_str() );
  // erase the last two lines:
  string file = tmp;
  /*
  file.erase( file.size()-2, 2);
  while( file.find(" ") != string::npos){
    file.erase(file.find(" "), 1);
  }
   */
  file += ".xml";
  
  
  // start writing
  ofstream* WRITER = new ofstream();
  WRITER->open(file.c_str(), ofstream::out); 
  if(WRITER->good()){
    
    char buffer[254];
   
    sprintf(buffer, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"); 
    WRITER->write(buffer, strlen(buffer));
    
    // write the parameters:
    map<string, string>::iterator P = allParameters.begin();
    while( P != allParameters.end() ){
      string tag = (*P).first;
      string val = (*P).second;

      sprintf(buffer, "<parameter key=\"%s\" value=\"%s\"/>\n", tag.c_str(), val.c_str()); 
      WRITER->write(buffer, strlen(buffer));
  
      P++;
    }
    
  }
  delete WRITER;
  WRITER = NULL;
  
  
}



void paramManager::restoreParamFile( string in ){
  
  XMLfile = in;
  
  TiXmlDocument* PARSER = new TiXmlDocument( XMLfile.c_str() );
  
  if( PARSER->LoadFile() ){
    printf("\t\t\tparsing XML Parameterfile '%s' ...\n", XMLfile.c_str() );    
  }
  else{   
    printf("\n\t\t*** Could not open XML Parameterfile %s ***\n", XMLfile.c_str() );
  }
  
  if( PARSER != NULL ){
    // get the element of the ms search summary and go through each of them:
    TiXmlNode* PARAM = PARSER->FirstChild( "parameter" ); 
    while( PARAM != NULL ){
      
        TiXmlElement* THIS_CHILD = PARAM->ToElement();
        string tag = THIS_CHILD->Attribute("key");
        string value = THIS_CHILD->Attribute("value");
        allParameters.insert( make_pair( tag, value ) );
        
        PARAM = PARAM->NextSibling();
    }
  }
  delete PARSER;
  PARSER = NULL;

  // store all parameters in a param file:
  storeALLParameterinPARAMFile( );
  
}


void paramManager::storeALLParameterinPARAMFile( ){
  
  // create the outfile:
  string file = "paramRestore.def";
  
  // start writing
  ofstream* WRITER = new ofstream();
  WRITER->open(file.c_str(), ofstream::out); 
  if(WRITER->good()){
    
    char buffer[254];
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime( &rawtime);  
    
    sprintf(buffer, "///////////////////////////////////////////////////////////////////////////////////\n");
      WRITER->write(buffer, strlen(buffer));
      sprintf(buffer, "//	CONFIGURATION FILE for the S U P E R H I R N   program                        //\n");
        WRITER->write(buffer, strlen(buffer));
        sprintf(buffer, "// automatically restored SuperHirn param.def file\n// from xml parameter file: %s\n", XMLfile.c_str()); 
          WRITER->write(buffer, strlen(buffer));
          
          sprintf(buffer, "//\n");
            WRITER->write(buffer, strlen(buffer));
            sprintf(buffer, "//  written by Lukas N Mueller, %s\n", asctime(timeinfo));
              WRITER->write(buffer, strlen(buffer));
              sprintf(buffer, "//  Lukas.Mueller@imsb.biol.ethz.ch\n");
                WRITER->write(buffer, strlen(buffer));
                sprintf(buffer, "//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich\n");
                  WRITER->write(buffer, strlen(buffer));
                  sprintf(buffer, "//\n");
                    WRITER->write(buffer, strlen(buffer));
                    sprintf(buffer, "//\n");
                      WRITER->write(buffer, strlen(buffer));
                      
                  
                  // write the parameters:
                  map<string, string>::iterator P = allParameters.begin();
    while( P != allParameters.end() ){
      string tag = (*P).first;
      string val = (*P).second;
      sprintf(buffer, "%s=%s\n", tag.c_str(), val.c_str()); 
      WRITER->write(buffer, strlen(buffer));
      sprintf(buffer, "//\n");
        WRITER->write(buffer, strlen(buffer));
        
      P++;
    }
    
  }
  delete WRITER;
  WRITER = NULL;
  
  
}
