///////////////////////////////////////////////////////////////////////////
//
//  written by Lukas N Mueller, 30.3.05
//  Lukas.Mueller@imsb.biol.ethz.ch
//  Group of Prof. Ruedi Aebersold, IMSB, ETH Hoenggerberg, Zurich
//
//  modified by Gustavo Mendes Maciel, 03.07.19
//  gmmaciel.bh2@gmail.com
//  Modified the constructor and the indentation
// 
//
// **********************************************************************//
// CLASS multi_merger:
//
// variable description:
//
//
// function description:
//
//
// **********************************************************************//

#define USE_NODE
#define USE_AMRTP_LC_MS_ALIGNER
#define USE_SPEC_MERGER
#define USE_SEARCHER
#define USE_LC_MS_SIMILARITY_MATRIX
#define USE_MULTI_MERGER

#include "LC_MS_TOOLS_USE.h"


////////////////////////////////////////////////
// constructor for the object multi_merger:
multi_merger::multi_merger(void *pinstance, LC_MS*(*pfct_get_LC_MS_ID)(void *pInstance, int ID), void(*pfct_delete_LC_MS_ID)(void *pInstance, int ID)) {

    alignment_tree = NULL;

    // instance of spec_manager:
    manager = pinstance;

    // instance of the function of spec_manager:
    Pget_LC_MS_ID = pfct_get_LC_MS_ID;		
    Pdelete_LC_MS_ID = pfct_delete_LC_MS_ID;		

    // get teh parameter if alignment plots should be 
    // done for the multiple alignment:
    read_param* def = new read_param();
    // check if the gnuplot is on:
    def->search_tag("gnuplot plot generator", &multiAView);
    if (multiAView) {
        def->search_tag("MASTER alignment plotting", &multiAView);
    }
    delete def;
    def = NULL;

}


//////////////////////////////////////////////////
// class desctructor of multi_merger
multi_merger::~multi_merger() {

    if (alignment_tree != NULL) {
        delete alignment_tree;
        alignment_tree = NULL;	
    }

    if (!merged_LC_MS.empty()) {
        merged_LC_MS.clear();
    }  

    manager = NULL;
    Pget_LC_MS_ID = NULL; 
    Pdelete_LC_MS_ID = NULL; 

}



//////////////////////////////////////////////////
// class copy constructor of multi_merger
multi_merger::multi_merger(const multi_merger& tmp) {
}


//////////////////////////////////////////////////
// copy constructor:
multi_merger& multi_merger::operator=(const multi_merger& tmp) {
    return *this;
}


//////////////////////////////////////////////////
// get a spectrum from spec_manager by ID:
LC_MS* multi_merger::get_LC_MS_by_ID(int ID) {

    LC_MS* target = NULL;

    // call the function in spec_manager to get a LC-MS by ID 
    target =  Pget_LC_MS_ID(manager, ID);

    // if target not NULL; then has been found in 
    // original LC-MS data of spec_manager -> return it
    if (target != NULL) {
        return target;
    }
    else{	

        // has not been found, then its a merged new one
        // serach in the merged spectrum pool here:
        vector<LC_MS>::iterator p = merged_LC_MS.begin();
        while (p != merged_LC_MS.end()) {
            if ((*p).get_spectrum_ID() == ID)
                return &(*p);
            p++;
        }
    }

    //printf("\nERROR:: LC/MS run with ID %d was not found in 'get_LC_MS_by_ID(int):line106'\n",ID);
    return NULL;
}

//////////////////////////////////////////////////
// insert a created merged spectrum:
void multi_merger::insert_merged_LC_MS(LC_MS* tmp) {
    merged_LC_MS.push_back(*tmp);
    tmp = NULL;
}





//////////////////////////////////////////////////////
//***************************************************
//
// starts the merging process using the guide tree:
void multi_merger::start_merging_process() {

    // parse the alignment tree:
    read_guide_tree();

    if (alignment_tree != NULL) {

        // get the highest id of teh alignemnt tree
        int merge_ID = find_max_id(alignment_tree);

        // create a list of basic IDs:
        // find the max ID:
        vector<int> IDs;
        for (int i = 0; i <= merge_ID; i++) {
            IDs.push_back(i);
        }

        while (alignment_tree->get_below_left() != NULL) {
            merge_ID++;
            // merge the closest pairs:
            merge_max_related_pair(merge_ID);
        }

        // get the master run now: only one remaining in the list of
        // merged LC-MS

        // show statues
        printf("\n\t\t** ALIGNED LC/MS MAP of input LC-MS runs was created... **\n");



        //////////////////////////////////////////////
        // initiate the MasterMap
        // set a nice new name:
        // choose according to the project name:
        read_param* def = new read_param();
        get_MASTER_LC_MS()->set_spec_name(def->search_tag("MY PROJECT NAME"));
        delete def;
        def = NULL;

    }

}

//////////////////////////////////////////////////////
// merges the 2 closest relatest pairs in current tree
void multi_merger::merge_max_related_pair(int new_merge_ID) {

    // will be the id's of an alignment pair
    ID_A = 0;
    ID_B = 0;

    // get an alignment pair: travers the tree down, always left:
    get_alignment_pair(alignment_tree);

    // show statues
    printf("\t\t-- Alignment/merging of LC-MS pair [%d,%d] into [%d]\n", ID_A, ID_B, new_merge_ID);

    // align now the LC_MS runs
    align_B_to_A(ID_A, ID_B);

    // now merge the 2 LC-MS  lruns:
    LC_MS* merged_LC_MS = merge_2_LC_MS(ID_A, ID_B);

    // set the id of the new LC_MS:
    merged_LC_MS->set_spectrum_ID(new_merge_ID);

    // print content of the merged spectrum:
    // merged_LC_MS->show_info();

    // insert now into merged LC_MS list:
    insert_merged_LC_MS(merged_LC_MS);

    delete merged_LC_MS;
    merged_LC_MS = NULL;

    // now remove the nodes with this id's
    // and set the father node with the new id:
    update_alignment_tree(alignment_tree, new_merge_ID);

}

///////////////////////////////////////////////////////
// aligns 2 LC-MS runs and returns corrected LC_MS B
void multi_merger::align_B_to_A(int ID_A, int ID_B) {

    // get the LC-MS to combine
    LC_MS* A = get_LC_MS_by_ID(ID_A);	
    LC_MS* B = get_LC_MS_by_ID(ID_B);

    // if either one is NULL, stopp:
    if ((B != NULL) && (A != NULL)) {

        //////////////////////////////////////////
        // align now the spectra:

        // create the alignment class:
        AmrtpLCMSAligner* ALL = new AmrtpLCMSAligner(multiAView);

        // set the LC/MS pairs:
        ALL->set_REFERENCE_LCMSMap(A);
        ALL->set_CORRECTION_LCMSMap(B);

        // do the alignment:
        ALL->START_LC_MS_Alignment();

        delete ALL;
        ALL = NULL;

    }

    A = NULL;
    B = NULL;
}


///////////////////////////////////////////////////////
// merge 2 LC-MS runs and returns the merged one
LC_MS* multi_merger::merge_2_LC_MS(int ID_A, int ID_B) {

    // fetch first LC_MS A:
    LC_MS* A = get_LC_MS_by_ID(ID_A);
    LC_MS* B = get_LC_MS_by_ID(ID_B);

    LC_MS_merger* MERGER = new LC_MS_merger();

    // set the spectra:
    MERGER->set_input_spectra(A, B);

    // start merging
    MERGER->start_merging();

    // get the merged LC-MS now:
    LC_MS* mergedLCMS = MERGER->get_merged_spectrum();

    delete MERGER;
    MERGER = NULL;

    // delete the original old ones:
    delete_LC_MS_by_ID(ID_A);
    delete_LC_MS_by_ID(ID_B);

    // return the alignmnet corrected LC_MS B:
    return mergedLCMS;
}




//////////////////////////////////////////////////////
// starts the merging process using the guide tree:
int multi_merger::find_max_id(node* IN) {

    int max = -1;
    int tmp = 0;

    // check left:
    if (IN->get_below_left() != NULL)
        tmp = find_max_id(IN->get_below_left());
    else
        tmp = IN->get_id();

    if (max < tmp)
        max = tmp;

    // check right
    if (IN->get_below_right() != NULL)
        tmp = find_max_id(IN->get_below_right());
    else
        tmp = IN->get_id();

    if (max < tmp)
        max = tmp;

    return max;
}


//////////////////////////////////////////////////////
// get an alignment pair:
// go down the tree to the left until find pair:
void multi_merger::get_alignment_pair(node* current) {

    // go first all the way left:
    while (current->get_below_left() != NULL) {
        current = current->get_below_left();
    }

    ID_A = current->get_id();

    // go up to father of this terminal:
    current = current->get_upper();
    // get now the right branch:
    current = current->get_below_right();

    // check if its a terminal:
    if (current->get_below_left() == NULL) {
        ID_B = current->get_id();
    }
    else{
        get_alignment_pair(current);
    }	
}


//////////////////////////////////////////////////////
// now remove the nodes with this id's
// and set the father node with the new id:
void multi_merger::update_alignment_tree(node* current, int merge_ID) {

    // get A:
    while (current->get_below_left() != NULL) {
        current = current->get_below_left();
    }

    if (current->get_id() == ID_A) {
        current = current->get_upper();

        // set the new ID:
        current->set_id(merge_ID);

        ///////////////////////////////////////////
        // delete here the real LC-MS runs with
        // the correspoding ID:
        delete_LC_MS_by_ID(current->get_below_left()->get_id());
        delete_LC_MS_by_ID(current->get_below_right()->get_id());		

        // now delete the left/right branches:
        current->delete_left();
        current->delete_right();
    }
    else{
        current = current->get_upper();
        update_alignment_tree(current->get_below_right(), merge_ID);
    }
}


//////////////////////////////////////////
// delete here the real LC-MS runs with
// the correspoding ID:
void multi_merger::delete_LC_MS_by_ID(int ID) {

    // search only in the merged new ones
    // serach in the merged spectrum pool here:
    vector<LC_MS>::iterator p = merged_LC_MS.begin();
    while (p != merged_LC_MS.end()) {
        if ((*p).get_spectrum_ID() == ID) {
            p = merged_LC_MS.erase(p);
            break;
        }
        p++;
    }

    if (p == merged_LC_MS.end()) {
        // if arrived here, then the LC-MS run was 
        // not found -> so delete in the original data:
        Pdelete_LC_MS_ID(manager, ID);
    }

}	


////////////////////////////////////////////////////////////////////////
// reads the structure of an alignment tree into the memory:	
void multi_merger::read_guide_tree() {

    // test to read the tree:
    tree_reader* IN_R = new tree_reader();

    // reade the tree from file:
    IN_R->read_tree_file();

    // get the tree structure from tree_reader:
    alignment_tree = IN_R->get_guide_tree(); 

    /////////////////////////////////////
    // in case no alignment tree is available
    // start creation of a fake alignment tree:
    if (alignment_tree == NULL) {
        createFakeAlignmentTree();
    }

    delete IN_R;
    IN_R = NULL;	

}


///////////////////////////////////////////////////////////////////////
// in case no alignment tree is available
// start creation of a fake alignment tree:
void multi_merger::createFakeAlignmentTree() {


    printf("\t\t -> Please Note: Building alignment tree in sequential order ...\n\n");

    // create a list of LC-MS runs:
    map<int, LC_MS*> LC_MS_map;
    int id = 0;
    while (get_LC_MS_by_ID(id) != NULL) {
        LC_MS* map = get_LC_MS_by_ID(id);
        LC_MS_map.insert(make_pair(map->get_spectrum_ID(), map));
        id++;
    }

    // start the tree construction:
    SequentialTreeCreator creator;
    // set the lc-ms map:
    creator.setLCMSMap(LC_MS_map);
    // start:
    creator.constructSequentialAlignmentTree();
    // get the tree:
    alignment_tree = creator.getSequentialTree();

    LC_MS_map.clear();
}

