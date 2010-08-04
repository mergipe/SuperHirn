    //
	//  ImageBrowserViewController.mm
	//  AkiliPad
	//
	//  Created by Mithin on 11/06/10.
	//  Copyright 2010 Techtinium Corporation. All rights reserved.
	//

#import "ImageBrowserViewController.h"
#import "AkiliImageView.h"
#import "ImageHandler.h"
#import "ImageLegendView.h"

@interface ImageBrowserViewController(PrivateMethods)

- (void)applyNewIndex:(int)newIndex toImageView:(AkiliImageView *)imageView;
- (void)loadContentAtIndex:(int)index inImageView:(AkiliImageView *)imageView;
- (void)setTextForTitle;
- (void)dismissImageBrowser;

@end

@implementation ImageBrowserViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and 
 //want to perform customization that is not appropriate for viewDidLoad.
 - (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
 if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
 // Custom initialization
 }
 return self;
 }
 */

- (id)initWithImageElementId:(NSString *)elementId andImageHandler:(ImageHandler *)imageHandler {
	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
		self = [super initWithNibName:@"ImageBrowserView_iPad" bundle:nil];
	}
	else {
		self = [super initWithNibName:@"ImageBrowserView" bundle:nil];
	}
	mImageHandler = imageHandler;
	currentPageIndex = [[elementId stringByReplacingOccurrencesOfString:@"image" withString:@""] intValue];
	currentPageIndex--;
	
	return self;
}

/*
 // Implement loadView to create a view hierarchy programmatically, without using a nib.
 - (void)loadView {
 }
 */


	// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	[super viewDidLoad];
	self.navigationController.navigationBar.barStyle = UIBarStyleBlackOpaque;
	
	mWindow = (Window *)[[UIApplication sharedApplication].windows objectAtIndex:0];
	
	UIBarButtonItem *closeItem = [[UIBarButtonItem alloc] initWithTitle:@"Schliessen" style:UIBarButtonItemStyleBordered 
																 target:self action:@selector(dismissImageBrowser)];
	self.navigationItem.rightBarButtonItem = closeItem;
	[closeItem release];
	
	
	self->legend = [[ImageLegendView alloc] init];
	
	CGPoint sizeL = [self->legend size];
	self->legendPosition.y = 1024 - sizeL.y;
	self->legendPosition.x = 0;
	
	CGRect r = [legend frame];
	r.origin.y = self->legendPosition.y;
	r.origin.x = self->legendPosition.x;
	[legend hide];
	
	[self.view addSubview:legend];
	
	[legend release];
		
	int total = [mImageHandler.imageInfoDict count];
	mScrollView.contentSize = CGSizeMake(total * mScrollView.frame.size.width, mScrollView.frame.size.height);
	mScrollView.contentOffset = CGPointMake(0.0, 0.0);
	
	if(currentImageView == nil) {
		currentImageView = [[AkiliImageView alloc] init];
			//set contentMode to scale aspect to fit
		currentImageView.contentMode = UIViewContentModeScaleAspectFit;
	}
	currentImageView.frame = CGRectMake(0.0, 0.0, mScrollView.frame.size.width, mScrollView.frame.size.height);
	
	if(nextImageView == nil) {
		nextImageView = [[AkiliImageView alloc] init];
			//set contentMode to scale aspect to fit
		nextImageView.contentMode = UIViewContentModeScaleAspectFit;
	}
	nextImageView.frame = CGRectMake(mScrollView.frame.size.width, 0.0, 
									 mScrollView.frame.size.width, mScrollView.frame.size.height);
	
	[self applyNewIndex:currentPageIndex toImageView:currentImageView];
	[self applyNewIndex:currentPageIndex+1 toImageView:nextImageView];
	
	[self loadContentAtIndex:currentPageIndex inImageView:currentImageView];
	[self loadContentAtIndex:currentPageIndex+1 inImageView:nextImageView];
	
	[mScrollView addSubview:currentImageView];
	[mScrollView addSubview:nextImageView];
}

- (void)viewWillAppear:(BOOL)animated {
	[super viewWillAppear:animated];
	
	[mScrollView setContentOffset:CGPointMake(mScrollView.frame.size.width * currentPageIndex, 0)];
	self.navigationItem.title = [NSString stringWithFormat:@"%d von %d", currentPageIndex+1, [mImageHandler.imageInfoDict count]];
	
}

- (void)viewDidAppear:(BOOL)animated {
	[super viewDidAppear:animated];
	mWindow.viewToObserve = self.view;
	mWindow.controllerThatObserves = self;
		//Set the correct title and set the size for label
	[self setTextForTitle];
}

- (void)viewWillDisappear:(BOOL)animated {
	[super viewWillDisappear:animated];
	mWindow.viewToObserve = nil;
}


/*
 // Override to allow orientations other than the default portrait orientation.
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
 // Return YES for supported orientations
 return (interfaceOrientation == UIInterfaceOrientationPortrait);
 }
 */

- (void)didReceiveMemoryWarning {
		// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
		// Release any cached data, images, etc that aren't in use.
}

/*- (void)viewDidUnload {
 [super viewDidUnload];
 // Release any retained subviews of the main view.
 // e.g. self.myOutlet = nil;
 }*/


- (void)dealloc {
	[currentImageView release];
	[nextImageView release];
	[super dealloc];
}

- (void)dismissImageBrowser {
	[self.navigationController dismissModalViewControllerAnimated:YES];
}

- (void)applyNewIndex:(int)newIndex toImageView:(AkiliImageView *)imageView {
	int pageCount = [mImageHandler.imageInfoDict count];
	
    BOOL outOfBounds = newIndex >= pageCount || newIndex < 0;
	
    if (!outOfBounds) {
        CGRect pageFrame = imageView.frame;
        pageFrame.origin.y = 0.0;
        pageFrame.origin.x = mScrollView.frame.size.width * newIndex;
        imageView.frame = pageFrame;
    }
    else {
        CGRect pageFrame = imageView.frame;
        pageFrame.origin.y = mScrollView.frame.size.height;
        pageFrame = pageFrame;
    }	
    imageView.pageIndex = newIndex;
}

- (void)loadContentAtIndex:(int)index inImageView:(AkiliImageView *)imageView {
	int pageCount = [mImageHandler.imageInfoDict count];
	if(index >= pageCount)
		return;
	
	ImageInfo *imgInfo = [mImageHandler.imageInfoDict objectForKey:[NSNumber numberWithInt:index+1]];
	NSString *filePath = imgInfo.filePath;
		//NSLog(@"Image file path = %@", filePath);
	if(!filePath)
		return;
	
	NSURL *url = [NSURL URLWithString:filePath];
	NSData *data = [NSData dataWithContentsOfURL:url];
	[imageView setImage:[UIImage imageWithData:data]];
}

- (void)togglePictureTitle {
	
	if ([legend isVisible]) 
	{
		[legend hide];
	}
	else{
		CGRect r = [legend frame];
		r.origin.y = self->legendPosition.y;
		r.origin.x = 0;
		[legend setFrame:r];
		[legend show];
		
	}
	
		//[self.view bringSubviewToFront:legend];
	/*
	 titleLabel.hidden = !titleLabel.hidden;
	 if(!titleLabel.hidden)
	 [self.view bringSubviewToFront:titleLabel];
	 */
	
}


- (void)setTextForTitle {
	ImageInfo *imgInfo = [mImageHandler.imageInfoDict objectForKey:[NSNumber numberWithInt:currentImageView.pageIndex+1]];
	NSString *legendStr = imgInfo.title;
	if(![legendStr length]) {
		[legend setLegend:@""];
		return;
	}
	else
	{
		CGSize s = [legendStr sizeWithFont:titleLabel.font constrainedToSize:CGSizeMake(200.0, 1000.0)];
		legend.frame = CGRectMake(20.0, 25, 768, 50);
		[legend setLegend:legendStr];
		
		 CGRect r = [legend frame];
		 r.origin.y = self->legendPosition.y;
		 r.origin.x = 0;
		 [legend setFrame:r];
	}
	/*
	 CGSize s = [legendStr sizeWithFont:titleLabel.font constrainedToSize:CGSizeMake(200.0, 1000.0)];
	 titleLabel.frame = CGRectMake(20.0, self.view.frame.size.height - s.height - 50.0, s.width, s.height);
	 titleLabel.text = legendStr;
	 */
}

#pragma mark UIScrollView delegate methods
- (void)scrollViewDidScroll:(UIScrollView *)sender {

		// hide legende when swiping to another image
	[legend hide];

	int pageCount = [mImageHandler.imageInfoDict count];
		//titleLabel.hidden = YES;
	
	CGFloat pageWidth = mScrollView.frame.size.width;
	float fractionalPage = mScrollView.contentOffset.x / pageWidth;
    if(fractionalPage < 0)
		return;
	
	NSInteger lowerNumber = floor(fractionalPage);
	NSInteger upperNumber = lowerNumber + 1;
    
	currentPageIndex = lowerNumber;
	
	if (lowerNumber == currentImageView.pageIndex) {//User scrolling forward
		if (upperNumber != nextImageView.pageIndex && upperNumber < pageCount) {
			[self applyNewIndex:upperNumber toImageView:nextImageView];
			[self loadContentAtIndex:upperNumber inImageView:nextImageView];
		}
	}
	else if (upperNumber == currentImageView.pageIndex) {//User scrolling backward
		if (lowerNumber != nextImageView.pageIndex && lowerNumber >= 0) {
			[self applyNewIndex:lowerNumber toImageView:nextImageView];
			[self loadContentAtIndex:lowerNumber inImageView:nextImageView];
		}
	}
	else {
		if(lowerNumber == nextImageView.pageIndex) {
			[self applyNewIndex:upperNumber toImageView:currentImageView];
			[self loadContentAtIndex:upperNumber inImageView:currentImageView];
		}
		else if(upperNumber == nextImageView.pageIndex) {
			[self applyNewIndex:lowerNumber toImageView:currentImageView];
			[self loadContentAtIndex:lowerNumber inImageView:currentImageView];
		}
		else {
			[self applyNewIndex:lowerNumber toImageView:currentImageView];
			[self applyNewIndex:upperNumber toImageView:nextImageView];
			[self loadContentAtIndex:upperNumber inImageView:nextImageView];
			[self loadContentAtIndex:lowerNumber inImageView:currentImageView];
		}
	}
}

- (void)scrollViewDidEndScrollingAnimation:(UIScrollView *)newScrollView {
	CGFloat pageWidth = mScrollView.frame.size.width;
    float fractionalPage = mScrollView.contentOffset.x / pageWidth;
    NSInteger nearestNumber = lround(fractionalPage);
	
    if (currentImageView.pageIndex != nearestNumber) {
        AkiliImageView *swapView = currentImageView;
        currentImageView = nextImageView;
        nextImageView = swapView;
    }
	
		//Set the legend text.
	[self setTextForTitle];
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)newScrollView {
	[self scrollViewDidEndScrollingAnimation:mScrollView];
		//When image is updated, we have to update the title as well
		//currentPageIndex = index + 1;
	self.navigationItem.title = [NSString stringWithFormat:@"%d von %d", currentPageIndex+1, [mImageHandler.imageInfoDict count]];
	
}

#pragma mark Window delegate methods
- (void)tappedOnView:(UIView *)view atPoint:(CGPoint)point {
	NSLog(@"Tapped in Image browser");
	if(![view isKindOfClass:[UIScrollView class]]) {
		return;
	}
	
	[self togglePictureTitle];
}

- (void)pinchedInOnView:(UIView *)view withPoints:(NSArray *)points {
	[self dismissImageBrowser];
}

@end
