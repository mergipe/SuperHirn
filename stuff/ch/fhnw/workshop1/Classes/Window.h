//
//  untitled.h
//  iCompetence
//
//  Authors: SH/LM on 26/05/10.
//  
//

#import <Foundation/Foundation.h>

@protocol WindowDelegate <NSObject>

@optional
- (void)tappedOnView:(UIView *)view atPoint:(CGPoint)point;
- (void)swipedHorizontallyOnView:(UIView *)view;
- (void)swipedVerticallyOnView:(UIView *)view;
- (void)doubleTappedOnView:(UIView *)view atPoint:(CGPoint)point;
- (void)pinchedInOnView:(UIView *)view withPoints:(NSArray *)points;
- (void)pinchedOutOnView:(UIView *)view withPoints:(NSArray *)points;

@end

@interface Window : UIWindow {
	UIView *viewToObserve;
    id <WindowDelegate> controllerThatObserves;
	
@private
	CGPoint gestureStartPoint;
	CGFloat initialDistance;
}

@property (nonatomic, retain) UIView *viewToObserve;
@property (nonatomic, assign) id <WindowDelegate> controllerThatObserves;
@property (nonatomic) CGPoint gestureStartPoint;

@end
