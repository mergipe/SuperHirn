//
//  untitled.m
//  AkiliPad
//
//  Created by Mithin on 26/05/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "Window.h"

@implementation Window

@synthesize viewToObserve;
@synthesize controllerThatObserves;
@synthesize gestureStartPoint;

- (id)initWithViewToObserver:(UIView *)view andDelegate:(id)delegate {
	
	if(self == [super init]) {
		self.viewToObserve = view;
		self.controllerThatObserves = delegate;
		self.gestureStartPoint = CGPointZero;
		initialDistance = -1;
	}
	
	return self;
}

- (void)dealloc {
    //[viewToObserve release];
    [super dealloc];
}

- (void)forwardTap:(NSDictionary *)dict {
	if(controllerThatObserves != nil && [controllerThatObserves respondsToSelector:@selector(tappedOnView: atPoint:)]) {
		UIView *view = [[dict allValues] objectAtIndex:0];
		CGPoint point = [[[dict allKeys] objectAtIndex:0] CGPointValue];
		[controllerThatObserves tappedOnView:view atPoint:point];
	}
}

- (void)forwardHorizontalSwipe:(NSDictionary *)dict {
	if(controllerThatObserves != nil && [controllerThatObserves respondsToSelector:@selector(swipedHorizontallyOnView:)]) {
		UIView *view = [[dict allValues] objectAtIndex:0];
		[controllerThatObserves swipedHorizontallyOnView:view];
	}
}

- (void)forwardVerticalSwipe:(NSDictionary *)dict {
	if(controllerThatObserves != nil && [controllerThatObserves respondsToSelector:@selector(swipedVerticallyOnView:)]) {
		UIView *view = [[dict allValues] objectAtIndex:0];
		[controllerThatObserves swipedVerticallyOnView:view];
	}
}

- (void)forwardDoubleTap:(NSDictionary *)dict {
	if(controllerThatObserves != nil && [controllerThatObserves respondsToSelector:@selector(doubleTappedOnView: atPoint:)]) {
		UIView *view = [[dict allValues] objectAtIndex:0];
		CGPoint point = [[[dict allKeys] objectAtIndex:0] CGPointValue];
		[controllerThatObserves doubleTappedOnView:view atPoint:point];
	}
}

- (void)forwardPinchIn:(NSDictionary *)dict {
	if(controllerThatObserves != nil && [controllerThatObserves respondsToSelector:@selector(pinchedInOnView: withPoints:)]) {
		NSArray *points = [dict valueForKey:@"TouchPoints"];
		UIView *v = [dict valueForKey:@"TouchView"];
		[controllerThatObserves pinchedInOnView:v withPoints:points];
	}
}

- (void)forwardPinchOut:(NSDictionary *)dict {
	if(controllerThatObserves != nil && [controllerThatObserves respondsToSelector:@selector(pinchedOutOnView: withPoints:)]) {
		NSArray *points = [dict valueForKey:@"TouchPoints"];
		UIView *v = [dict valueForKey:@"TouchView"];
		[controllerThatObserves pinchedOutOnView:v withPoints:points];
	}
}

- (CGFloat)distanceBetweenTwoPoints:(CGPoint)fromPoint toPoint:(CGPoint)toPoint {	
	float x = toPoint.x - fromPoint.x;
	float y = toPoint.y - fromPoint.y;
	
	return sqrt(x * x + y * y);
}

- (void)sendEvent:(UIEvent *)event {
    [super sendEvent:event];
	
							
	if (viewToObserve == nil || controllerThatObserves == nil) {
		return;
	}
	
    NSSet *touches = [event allTouches];
	//if (touches.count != 1) {
	//	return;
	//}
	UITouch *touch = touches.anyObject;
	if(touch.view == nil)
		return;
	
	if(touch.phase == UITouchPhaseBegan) {
		//touchBegan = YES;
		gestureStartPoint = [touch locationInView:viewToObserve];
		
		if(touches.count > 1) {
			///Track the initial distance between two fingers.
			UITouch *touch1 = [[touches allObjects] objectAtIndex:0];
			UITouch *touch2 = [[touches allObjects] objectAtIndex:1];
		
			CGPoint p1 = [touch1 locationInView:viewToObserve];
			CGPoint p2 = [touch2 locationInView:viewToObserve];
		
			initialDistance = [self distanceBetweenTwoPoints:p1 toPoint:p2];
		}
						   
		return;
	}
	else if(touch.phase == UITouchPhaseMoved) {
		if(touches.count > 1) {
			//The image is being zoomed in or out.
			UITouch *touch1 = [[touches allObjects] objectAtIndex:0];
			UITouch *touch2 = [[touches allObjects] objectAtIndex:1];
			CGPoint p1 = [touch1 locationInView:viewToObserve];
			CGPoint p2 = [touch2 locationInView:viewToObserve];
		
			//Calculate the distance between the two fingers.
			CGFloat finalDistance = [self distanceBetweenTwoPoints:p1 toPoint:p2];
		
			//Check if zoom in or zoom out.
			NSArray *touchPoints = [NSArray arrayWithObjects:[NSValue valueWithCGPoint:p1], [NSValue valueWithCGPoint:p2], nil];
			NSArray *values = [NSArray arrayWithObjects:touchPoints, touch1.view, nil];
			NSArray *keys = [NSArray arrayWithObjects:@"TouchPoints", @"TouchView", nil];
			NSDictionary *dict = [NSDictionary dictionaryWithObjects:values  forKeys:keys];
			if(initialDistance > 0 && initialDistance > finalDistance) {
				NSLog(@"Pinch In");
				[self performSelector:@selector(forwardPinchIn:) withObject:dict afterDelay:0.1];
			} 
			else if(initialDistance > 0 && initialDistance < finalDistance) {
				NSLog(@"Pinch Out");
				[self performSelector:@selector(forwardPinchOut:) withObject:dict afterDelay:0.1];
			}
			initialDistance = -1;
		}
		return;
	}
	
    if (touch.phase != UITouchPhaseEnded) {
		return;
	}
	
	CGPoint tapPoint = [touch locationInView:viewToObserve];
	if(tapPoint.x - 75 > gestureStartPoint.x && fabs(tapPoint.y - gestureStartPoint.y) < 75) {
		NSLog(@"Horizontal swipe");
		//User has swiped horizontally to scroll
		gestureStartPoint = CGPointZero;
		[self performSelector:@selector(forwardHorizontalSwipe:) withObject:touch.view afterDelay:0.1];
		return;
	}
	if(fabs(tapPoint.y - gestureStartPoint.y) > 100 && fabs(tapPoint.x - gestureStartPoint.x) < 50) {
		//User has swiped vertically to scroll
		NSLog(@"Vertical swipe");
		gestureStartPoint = CGPointZero;
		[self performSelector:@selector(forwardVerticalSwipe:) withObject:touch.view afterDelay:0.1];
		return;
	}
	else {
		if(touch.tapCount == 2) {
			NSLog(@"Double tap");
			[self performSelector:@selector(forwardDoubleTap:) withObject:touch.view afterDelay:0.1];
		}
	}
	NSDictionary *dict = [NSDictionary dictionaryWithObject:touch.view forKey:[NSValue valueWithCGPoint:tapPoint]];
	[self performSelector:@selector(forwardTap:) withObject:dict afterDelay:0.1];
}

@end
