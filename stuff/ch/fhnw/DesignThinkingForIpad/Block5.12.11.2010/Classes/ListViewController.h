//
//  ListViewController.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//


#import <UIKit/UIKit.h>

@class ListViewControllerDelegate;

@protocol ListViewControllerDelegate <NSObject>;

/**
 * Upon selection/taping on a list item defined by its index
 * @param iItemIndex int
 * @author Lukas N Mueller
 */
-(void) didSelectItem:(int)iItemIndex;

@end


@interface ListViewController : UIViewController <UITableViewDataSource, UITableViewDelegate> 
{
	
@public
	UITableView* _table;
	UIImageView* _fontImage;
	id<ListViewControllerDelegate> delegate;
	
@private
	
	NSMutableArray* listOfFiles;	
	
	
}

@property(nonatomic, assign) id<ListViewControllerDelegate> delegate;
@property (nonatomic, retain) IBOutlet UITableView* _table;
@property (nonatomic, retain) IBOutlet UIImageView* _frontImage;


@end
