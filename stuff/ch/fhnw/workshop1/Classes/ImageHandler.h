//
//  ImageHandler.h
//  AkiliPad
//
//  Created by Mithin on 10/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ImageInfo : NSObject {
	NSString *filePath;
	NSString *title;
}

@property(nonatomic, retain) NSString *filePath;
@property(nonatomic, retain) NSString *title;

@end

@interface ImageHandler : NSObject {
	UIWebView *contentView;
	int currentId;
	int lastImageIndex;
	
	NSMutableDictionary *imageInfoDict;
}

@property(nonatomic) int currentId;
@property(nonatomic) int lastImageIndex;
@property(nonatomic, retain) NSMutableDictionary *imageInfoDict;
@property(nonatomic, assign) UIWebView *contentView;

- (id)init;
- (void)loadImageData;

@end
