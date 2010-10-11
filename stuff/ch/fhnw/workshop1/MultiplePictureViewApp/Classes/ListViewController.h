//
//  ListViewController.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 9/15/10.
//  Copyright 2010 FHNW. All rights reserved.
//


#import <UIKit/UIKit.h>


@interface ListViewController : UIViewController <UITableViewDataSource, UITableViewDelegate> 
{
	NSMutableArray* listOfPictures;
}
@end

@interface ListViewController()


@end
