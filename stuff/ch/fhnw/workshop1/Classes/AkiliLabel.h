//
//  AkiliLabel.h
//  AkiliPad
//
//  Created by Mithin on 16/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef enum {
	VerticalAlignmentTop = 0, // default
	VerticalAlignmentMiddle,
	VerticalAlignmentBottom,
} VerticalAlignment;

@interface AkiliLabel : UILabel {
@private
	VerticalAlignment _verticalAlignment;
}

@property (nonatomic) VerticalAlignment verticalAlignment;

@end
