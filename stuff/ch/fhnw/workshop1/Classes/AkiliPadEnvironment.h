//
//  AkiliPadEnvironment.h
//  AkiliPad
//
//  Created by Lukas Müller on 6/18/10.
//  Copyright 2010 MMI AG. All rights reserved.
//




	// Singleton class to store app wide settings and provide access to them. Provides static methods for:
	// - getting Fonts for title, subtite, author, etc.
	//
@interface AkiliPadEnvironment : NSObject {




@private
		// font variable for different text types
	UIFont *e5FontText;
	UIFont *e5FontTitle;
	UIFont *e5FontSubtitle;
	
	
	

}

	// static singleton instance
//+ (AkiliPadEnvironment*)sharedInstance;

	// Get the font for regular text
+ (UIFont*)textFont;
	// Get the font for title
+ (UIFont*)titleFont;
	// Get the font for subtitle
+ (UIFont*)subtitleFont;
	// get a customized e5 app UILabel
+ (UILabel*)e5Label;


	// get instance
+ (AkiliPadEnvironment*)getInstance;

	// initializes the singleton class
- (void)initializeClass;



@end
