//
//  Item.h
//  iPad Block 1-4 
//
//  Created by Lukas Mueller.
//  Copyright by FHNW 2010. All rights reserved.
//


#import <UIKit/UIKit.h>

#import "Item.h"


@class SendEmailDelegate;

@protocol SendEmailDelegate <NSObject>;

-(void)sendEmail;

@end


@interface ContactDetail: Item<ItemDelegate>
{
	
	id<SendEmailDelegate> emailDelegate;

	
}

@property(nonatomic, assign) id<SendEmailDelegate> emailDelegate;



@end
