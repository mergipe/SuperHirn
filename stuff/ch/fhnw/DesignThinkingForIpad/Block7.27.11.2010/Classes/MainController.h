//
//  MainController.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import "ItemViewController.h";

#import <UIKit/UIKit.h>

@class ListViewController;
@class SwipeViewController;
@interface MainController : UINavigationController<ItemViewControllerDelegate> 
{
	NSMutableArray* controllers;
	ItemViewController* rootController;

	ListViewController* list;
}
@end

@interface MainController()


/**
 * Add a new SwipeViewController
 * @param iController SwipeViewController*
 * @author Lukas Mueller
 */
-(void)addController:(SwipeViewController*)iController;



/**
 * Selects the controller at this index and pushes the controller into the navigation controller.
 * @param iIndex int
 * @author Lukas Mueller
 */
- (void) showController:(int) iIndex;


/**
 * Implementation of ItemViewController tap on item selection protocol
 * @param iItemIndex int
 * @author Lukas Mueller
 */
-(void)didSelectItem:(int)iItemIndex;

@end
