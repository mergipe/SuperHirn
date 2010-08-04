//
//  AkiliPadEnvironment.m
//  AkiliPad
//
//  Created by Lukas Müller on 6/18/10.
//  Copyright 2010 MMI AG. All rights reserved.
//

#import "AkiliPadEnvironment.h"


@implementation AkiliPadEnvironment

static AkiliPadEnvironment *sharedInstance = nil;



#pragma mark class level access methodes to AkliPadEnvironment attributes:



+ (UIFont*)fallbackFont
{
	return [AkiliPadEnvironment getInstance]->e5FontTitle;
}


+ (UIFont*)titleFont
{
	return [AkiliPadEnvironment getInstance]->e5FontTitle;
}

+ (UIFont*)subtitleFont
{
	return [AkiliPadEnvironment getInstance]->e5FontSubtitle;
}

+ (UIFont*)textFont
{
	return [AkiliPadEnvironment getInstance]->e5FontText;
}

+ (UILabel*)e5Label
{
	CGRect frame = CGRectMake(0, 0, 400, 44);
	UILabel *label = [[[UILabel alloc] initWithFrame:frame] autorelease];
	label.backgroundColor = [UIColor clearColor];
	label.font = [AkiliPadEnvironment textFont];
	label.textAlignment = UITextAlignmentCenter;
	label.textColor = [UIColor whiteColor];	
	return label;
}


#pragma mark other class level methodes:

+ (AkiliPadEnvironment*)getInstance
{
 	@synchronized(self)
    {
		if (sharedInstance == nil) {
			sharedInstance = [[super allocWithZone:NULL] init];
			[sharedInstance initializeClass];
		}	
	}
	return sharedInstance;
}



#pragma mark instance level access methodes to class attributes:


- (void)initializeClass
{
		/////////////////////////////////////
		// set app fonts:
	self->e5FontText = [UIFont fontWithName: @"Black-Medium" size: 17];
	self->e5FontTitle = [UIFont fontWithName: @"Black-Medium" size: 17];
	self->e5FontSubtitle = [UIFont fontWithName: @"Black-Medium" size: 17];
	

}

+ (id)allocWithZone:(NSZone *)zone {
    @synchronized(self) {
        if (sharedInstance == nil) {
            sharedInstance = [super allocWithZone:zone];
            return sharedInstance;  // assignment and return on first allocation
        }
    }
    return nil; // on subsequent allocation attempts return nil
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (id)retain {
    return self;
}

- (unsigned)retainCount {
    return UINT_MAX;  // denotes an object that cannot be released
}

- (void)release {
		//do nothing
}

- (id)autorelease {
    return self;
}

@end


