	//
	//  ImageLegendView.mm
	//  AkiliPad
	//
	//  Created by Lukas Mueller on 21.6.2010
	//  Copyright 2010 Blankpage AG. All rights reserved.
	//

#import <UIKit/UIKit.h>

	/// IMageLegendView presents the image gallery legend of an image
@interface ImageLegendView : UIView {
	
	
@private
		/// text of the legend
	UITextView *textLegend;
		/// dimensions of the text box
	CGPoint textDimension;
}

@property(nonatomic, retain) UITextView *textLegend;


	/// set the content of galery legend
- (void) setLegend:NSString;
	/// hide the legend
- (void) hide;
	/// show the legende
- (void) show;
	/// get the visibility status of the legend
- (BOOL) isVisible;

	/// get the size of the legend text box
- (CGPoint) size;

@end
