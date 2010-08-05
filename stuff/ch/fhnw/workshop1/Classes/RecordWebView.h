//
//  RecordCoverView.h
//  iCompetence
//
//  Authors: SH/LM on 24/04/10.
//  
//

#import <UIKit/UIKit.h>


	// Subclasses UIWebView to display all e5 web-based Record (html, javascript, etc)   
	//
@interface RecordWebView : UIWebView {
	int pageIndex;
	
	NSString *lastTappedElement;
}

@property NSInteger pageIndex;
@property(nonatomic, retain) NSString *lastTappedElement;

- (NSString *)elementAtX:(int)x andY:(int)y;
- (NSString *)parentElement:(NSString *)elementName;
- (NSString *)idByElement:(NSString *)elementName;

- (int)scrollPosition;
- (void)scrollToPosition:(int)yPos;
- (int)yPosOfElementWithName:(NSString *)elementName;
- (CGPoint)coordinatesOfElement:(NSString *)elementName;

@end
