//
//  StudentController.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import <Foundation/Foundation.h>

@class Item;
@interface StudentController : NSObject 
{
	
}


/**
 * Get a list of detected files
 * @return Pointer to NSMutableArray which contains all available files.
 */
+(NSMutableArray*) getFiles;
+(NSMutableArray*) getPdfs: (NSString*) folder;


/**
 * Get the item that displays the image or pdf at this index with the give rectangle size
 * @param iIndex int 
 * @param iItemArea CGRect 
 * @return Name of the item, nil if the index exceeds the list size.
 */
+(Item*) getItem:(int) iIndex :(CGRect) iItemArea;

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
+(void) extractPdfNames: (NSString*) folder;


/**
 * Get the path of the data folder in the application resource folder
 */
+(NSString*) getDataFolderPath;

+(NSString*) getPdfFolderPath: (NSString*) folder;


@end
