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
	[pictures addObject:@"Eisenbahn_Modell"];
	[pictures addObject:@"Software_Engineering"];
	[pictures addObject:@"This is shit"];
	return pictures;
	
}

+(NSString *) getNavigationTitle {
	
	return @"Pictures";	
}

@end
