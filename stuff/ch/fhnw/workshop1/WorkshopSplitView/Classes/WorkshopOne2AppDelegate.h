//
//  WorkshopOne2AppDelegate.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 8/8/10.
//  Copyright FHNW 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class FullScreenViewController;
@class SwipeViewController;
@class RootViewController;
@class DetailViewController;
@interface WorkshopOne2AppDelegate : NSObject <UIApplicationDelegate> 
{
    
    UIWindow *window;
    SwipeViewController *_swipeView;
    FullScreenViewController *_fullView;

	/*
	 UISplitViewController *splitViewController;
    RootViewController *rootViewController;
    DetailViewController *detailViewController;
	 */
	//UINavigationController* _NavigationController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet SwipeViewController* _swipeView;
@property (nonatomic, retain) IBOutlet FullScreenViewController* _fullView;


/*
@property (nonatomic, retain) IBOutlet UINavigationController* _NavigationController;
@property (nonatomic, retain) IBOutlet UISplitViewController* splitViewController;
@property (nonatomic, retain) IBOutlet RootViewController *rootViewController;
@property (nonatomic, retain) IBOutlet DetailViewController *detailViewController;
*/
@end
