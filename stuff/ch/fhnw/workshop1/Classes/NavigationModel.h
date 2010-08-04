//
//  NavigationModel.h
//  AkiliPad
//
//  Created by Mithin on 18/05/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>



@interface NavigationModel : NSObject {

	int currentSpineIndex;
	
@private
	EPubReader *mEPUBReader;
}

@property(nonatomic) int currentSpineIndex;

+ (NavigationModel *)sharedModel;

- (void)resetEPUBReader;
- (void)loadNavigationFor:(NSString *)path;
- (int)topLevelItemCount;
- (NCXNavPoint *)navPointAtIndex:(int)index;

- (EPubReader *&)epubReader;
- (const NCXNavMap &)navMap;

- (NSString *)coverFileName;
- (NSString *)fallbackFileName;

- (void)setSpineIndexForArticleFilePath:(NSString *)filePath;
- (int)pageCountForArticleWithPath:(NSString *)articlePath;
- (void)setSpineIndexForItemId:(NSString *)itemID;
- (NSString *)filePathForNavPointWithId:(NSString *)navPointId;

- (NSString *)filePathForItemWithId:(NSString *)itmeID;

@end
