//
//  StudentController.m
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//

#import "StudentController.h"


@implementation StudentController

//input for split view

+(NSMutableArray*) getPictures {
	
	NSMutableArray *pictures = [[NSMutableArray alloc] init];
	[pictures addObject:@"Eisenbahn_Modell.jpg"];
	[pictures addObject:@"Software_Engineering.jpg"];
	[pictures addObject:@"This is shit"];
	return pictures;
	
}

+(NSString*) getPicture:(int)iIndex 
{
	return @"Eisenbahn_Modell.jpg";
}


+(NSString *) getNavigationTitle {
	
	return @"Pictures";	
}

@end
