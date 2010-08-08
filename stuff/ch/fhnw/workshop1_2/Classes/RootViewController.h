//
//  RootViewController.h
//  WorkshopOne2
//
//  Created by Sarah Hauser on 8/8/10.
//  Copyright FHNW 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class DetailViewController;

@interface RootViewController : UITableViewController {
    DetailViewController *detailViewController;
}

@property (nonatomic, retain) IBOutlet DetailViewController *detailViewController;

@end
