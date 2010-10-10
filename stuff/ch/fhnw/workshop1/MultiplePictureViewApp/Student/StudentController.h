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

+ (NSMutableArray *) pictures;

/**
 * Get a list of available pictures
 * @return Pointer to NSMutableArray which contains all available pictures.
 */
+(NSMutableArray*) getPictures;


/**
 * Get a list of available pictures
 * @return Pointer to NSMutableArray which contains all available pictures.
 */
+(NSString *) getNavigationTitle;


/**
 * Get the name of a picture at certaimn index
 * @param iIndex int 
 * @return Name of the picture, nil if the index exceeds the list size.
 */
+(NSString*) getPicture:(int)iIndex; 

/**
 * Get the number of a pictures
 * @return Number of the picture.
 */
+(int) numberOfPictures;

/**
 * Extracts all images from the Picture folder in the application resource folder
 */
+(void) extractPicturesNames;

/**
 * Get the path of the picture folder in the application resource folder
 */
+(NSString*) getPictureFolderPath;

@end
