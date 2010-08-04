//
//  ImageHandler.mm
//  AkiliPad
//
//  Created by Mithin on 10/06/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "ImageHandler.h"

@implementation ImageInfo

@synthesize filePath;
@synthesize title;

@end

@implementation ImageHandler

@synthesize currentId;
@synthesize lastImageIndex;
@synthesize imageInfoDict;
@synthesize contentView;

- (id)init {
	if(self = [super init]) {
		self.currentId = 1;
		self.lastImageIndex = 0;
		imageInfoDict = [[NSMutableDictionary alloc] init];
	}
	return self;
}

- (void)dealloc {
	[imageInfoDict release];
	[super dealloc];
}

- (void)loadImageData {
	//NSLog(@"Loading image browser info");
	//Clear existing info
	[self.imageInfoDict removeAllObjects];
	self.currentId = 1;
	self.lastImageIndex = 0;
	
	//Get the start index
	NSString *jsStr = @"var imgs = document.getElementsByTagName('img');"
						"var sIndex = 0;"
						"if(imgs.length) {"
						"	sIndex = imgs[0].id;"
						"}"
						"sIndex;";
	
	NSString *imgIndex = [contentView stringByEvaluatingJavaScriptFromString:jsStr];
	imgIndex = [imgIndex stringByReplacingOccurrencesOfString:@"image" withString:@""];
	lastImageIndex = [imgIndex intValue];
	
	//First get the number of images in the HTML
	jsStr = [NSString stringWithFormat:@"var imgCount=%d;"
											"var img = 'image';"
											"while(true) {"
											"		var e=document.getElementById(img+imgCount);"
											"		if(!e) {"
											"			break;"
											"		}\n"
											"       imgCount++;"
											"}\n"
											"imgCount;", lastImageIndex];
	int imageCount = [[contentView stringByEvaluatingJavaScriptFromString:jsStr] intValue] - lastImageIndex;
	//NSLog(@"Number of images = %d", imageCount);
	
	//Now save the img source in an array.
	for(int i = lastImageIndex; i < lastImageIndex + imageCount; i++) {
		jsStr = [NSString stringWithFormat:@"var e=document.getElementById('image%d');"
												"if(!e) {"
												"	0;"
												"	return;"
												"}\n"
												"e.src+\"::::\"+e.title;", i];
		NSString *resultStr = [contentView stringByEvaluatingJavaScriptFromString:jsStr];
		//NSLog(@"Img src = %@", resultStr);
		NSArray *imgArr = [resultStr componentsSeparatedByString:@"::::"];
		ImageInfo *imageInfo = [[ImageInfo alloc] init];
		imageInfo.filePath = [imgArr objectAtIndex:0];
		if([imgArr count] > 1) {
			imageInfo.title = [imgArr objectAtIndex:1];
		}
		
		[imageInfoDict setObject:imageInfo forKey:[NSNumber numberWithInt:i]];
	}
	
	lastImageIndex = lastImageIndex+imageCount;
}

@end
