//
//  StudentController.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import "StudentController.h"


@implementation StudentController

static NSMutableArray* detectedFiles = nil;

+(NSMutableArray*) getFiles 
{
	if( detectedFiles == nil )
	{
		[self extractFileNames];
	}
	
	return detectedFiles;	
}

+(NSString*) getFile:(int)iIndex 
{
	if( ( iIndex >= 0 ) && ( iIndex < [detectedFiles count]) )
	{
		return [detectedFiles objectAtIndex:iIndex];
	}
	
	return nil;
}

+(int) numberOfFiles
{
	if( detectedFiles != nil )
	{
		return [detectedFiles count];
	}
	
	return 0;
}



+(void) extractFileNames
{
	NSArray *entries = [[NSFileManager defaultManager] directoryContentsAtPath: [StudentController getDataFolderPath] ];	
	if( detectedFiles == nil )
	{
		detectedFiles = [[NSMutableArray alloc] init];
	}
	
	for( int i=0; i < [entries count]; i++)
	{
		NSString* myFile = [entries objectAtIndex:i];
		NSLog(@"File found in data folder: %@ ", myFile);
		[detectedFiles addObject:myFile];
	}

}

+(NSString*) getDataFolderPath
{
	return [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"Data/"];
}


@end
