package ch.superdbpusher.xmlParsing;

/*
 * Simple console output of the progress of a specific task. 
 * 
 * @author Lukas N. Mueller
 */

public class XMLProgress {

	static public float total = 0;

	static void update(double progressPercentage) {
		
		
		final int width = 50; // progress bar width in chars
		System.out.print("\r[");
		int i = 0;
		for (; i <= (int) (progressPercentage * width); i++) {
			System.out.print(".");
		}
		for (; i < width; i++) {
			System.out.print(" ");
		}
		System.out.print("]");
	}


	private int initCounter;
	private String text;
	private int nbIt;
	private int updateSteps;
	private int progress;
	
	XMLProgress(int nbIt, String iText){

	  this.initCounter = 1;  
	  this.text = iText;
	  
	  // define number of iterations:
	  this.nbIt = nbIt;
	  
	  // define the update step (%):
	  this.updateSteps = 10;
	  this.progress = 0;
	  this.setProgress();
	}


	//////////////////////////////////////////////////
	// calculated the progress:
	private void setProgress(){
	  this.progress = (int) (Math.ceil(this.progress + (double)( this.nbIt)) / this.updateSteps) ;
	}


	//////////////////////////////////////////////////
	// update the progress:
	public void update(){
	  
	  this.initCounter++;
	  if(progress == (double)(this.initCounter)){
	    this.print((100*this.initCounter)/this.nbIt);
	    this.setProgress();
	  }
	  
	}

	/*
	 * print the progess
	 * @param int progess
	 */
	void print(int it){
	    
	  System.out.print( "\t");
	  System.out.print(it);
	  System.out.print("%");
	  System.out.print(this.text);
	  System.out.print("...");
	  if( it == 50 ){
		  System.out.println("\t");
	  }
	  
	  
	}


}