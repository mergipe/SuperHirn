//
//  AppDelegate.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MainController.h";



@interface AppDelegate : NSObject <UIApplicationDelegate> 
{
    UIWindow *window;
	MainController* _navigationController;		
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet MainController* _navigationController;


@end
