	//
	//  AkiliImageView.h
	//  AkiliPad
	//
	//  Created by Mithin on 11/06/10.
	//  Copyright 2010 Techtinium Corporation. All rights reserved.
	//

#import <UIKit/UIKit.h>


	// Subclasses UIImageView to display an image gallery  
	//
@interface AkiliImageView : UIImageView {
	int pageIndex;
}

@property(nonatomic) int pageIndex;

@end
