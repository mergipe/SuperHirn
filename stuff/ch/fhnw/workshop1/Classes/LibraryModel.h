//
//  LibraryModel.h
//  AkiliPad
//
//  Created by Mithin on 07/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

#define MAGAZINE_COUNT_PER_PAGE	9

@interface LibraryModel : NSObject {
	NSMutableArray *contentList;
	NSMutableArray *issueList;
	NSMutableArray *titleList;
}

+ (LibraryModel *)sharedModel;

@property(nonatomic, retain) NSMutableArray *contentList;
@property(nonatomic, retain) NSMutableArray *titleList;
@property(nonatomic, retain) NSMutableArray *issueList;
		  
- (void)loadLibraryContents;

@end
