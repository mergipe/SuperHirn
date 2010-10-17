//
//  StudentController.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface StudentController : NSObject 
{

}


/**
 * Get a list of detected files
 * @return Pointer to NSMutableArray which contains all available files.
 */
+(NSMutableArray*) getFiles;



/**
 * Get the name of a file at certain index
 * @param iIndex int 
 * @return Name of the file, nil if the index exceeds the list size.
 */
+(NSString*) getFile:(int)iIndex; 

/**
 * Get the number of a files
 * @return Number of the files.
 */
+(int) numberOfFiles;

/**
 * Extracts all files from the data folder in the application resource folder
 */
+(void) extractFileNames;

/**
 * Get the path of the data folder in the application resource folder
 */
+(NSString*) getDataFolderPath;

@end
