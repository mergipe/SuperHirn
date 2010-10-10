//
//  StudentController.m
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//

#import "StudentController.h"


@implementation StudentController

static NSMutableArray* pictures = nil;

+(NSMutableArray*) getPictures 
{
	if( pictures == nil )
	{
		[self extractPicturesNames];
	}
	
	return pictures;	
}

+(NSString*) getPicture:(int)iIndex 
{
	if( ( iIndex >= 0 ) && ( iIndex < [pictures count]) )
	{
		return [pictures objectAtIndex:iIndex];
	}
	
	return nil;
}


+(NSString *) getNavigationTitle {
	
	return @"Pictures";	
}


+(void) extractPicturesNames
{
	NSArray *pics = [[NSFileManager defaultManager] directoryContentsAtPath: [StudentController getPictureFolderPath] ];	
	if( pictures == nil )
	{
		pictures = [[NSMutableArray alloc] init];
	}
	
	for( int i=0; i < [pics count]; i++)
	{
		NSString* myImage = [pics objectAtIndex:i];
		NSLog(@"Image found in Pictures folder: %@ ", myImage);
		[pictures addObject:myImage];
	}

}

+(NSString*) getPictureFolderPath
{
	return [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"Pictures/"];
}


@end
