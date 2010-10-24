//
//  StudentController.mm
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import "StudentController.h"

#import "PictureView.h"
#import "ItemWebView.h"


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
	if( detectedFiles == nil )
	{
		[self extractFileNames];
	}
	
	if( ( iIndex >= 0 ) && ( iIndex < [detectedFiles count]) )
	{
		return [detectedFiles objectAtIndex:iIndex];
	}
	
	return nil;
}

+(int) numberOfFiles
{
	if( detectedFiles == nil )
	{
		[self extractFileNames];
	}
	
	if( detectedFiles != nil )
	{
		return [detectedFiles count];
	}
	
	return 0;
}


+(Item*) getItem:(int) iIndex :(CGRect) iItemArea
{
	if( detectedFiles == nil )
	{
		[self extractFileNames];
	}
	
	Item* item = nil;
	NSString* itemPath = [self getFile: iIndex];
	
	// check extension for which subitem shoudl be generated:
	NSString* ext = [itemPath pathExtension];
	if( [ext isEqualToString:@"jpg"]  || [ext isEqualToString:@"png"] || [ext isEqualToString:@"tif"] || [ext isEqualToString:@"gif"] )
	{
		PictureView* picItem = [[PictureView alloc] initWithFrame:iItemArea];
		[ picItem setItemIndex:iIndex];
		[ picItem setImage:itemPath];
		item = picItem;

	}
	else if( [ext isEqualToString:@"pdf"]  || [ext isEqualToString:@"html"] )
	{
		ItemWebView* webItem = [[ItemWebView alloc] initWithFrame:iItemArea];
		[ webItem setItemIndex:iIndex];
		[ webItem setUrl:itemPath];
		item = webItem;
		
	}
	
	return item;
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
