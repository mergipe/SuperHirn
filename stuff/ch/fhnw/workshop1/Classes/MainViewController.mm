//
//  MainViewController.m
//  iCompetence
//
//  Authors: SH/LM on 24/04/10.
//  
//

#import "MainViewController.h"

#import "MainPadAppDelegate.h"

@interface MainViewController()
@end



@implementation MainViewController


// The designated initializer.  Override if you create the controller programmatically and 
//want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    return self;
}



// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
	[super viewDidLoad];
	
	//Setup My Overview button
	UIBarButtonItem *libItem = [[UIBarButtonItem alloc] initWithTitle:@"Test" style:UIBarButtonItemStyleBordered 
															   target:self action:@selector(showMyOverview)];
	self.navigationItem.leftBarButtonItem = libItem;
	[libItem release];
	
	mWindow = (Window *)[[UIApplication sharedApplication].windows objectAtIndex:0];
}



- (void)viewWillAppear:(BOOL)animated 
{
	[super viewWillAppear:animated];
}


- (void)viewDidAppear:(BOOL)animated 
{
}


- (void)viewWillDisappear:(BOOL)animated 
{
	[super viewWillDisappear:animated];
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
    return YES;
}


- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation 
{
	
}


- (void)didReceiveMemoryWarning 
{
	
    [super didReceiveMemoryWarning];
}



- (void)viewDidUnload {
    [super viewDidUnload];
}


- (void)dealloc {
    [super dealloc];
}





#pragma mark UIWebView delegate methods
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request 
 navigationType:(UIWebViewNavigationType)navigationType 
{
	//NSString *urlString = [[request URL] absoluteString];
	//NSString *fileName = [urlString lastPathComponent];
	//NSLog(@"Link Clicked = %@ ", urlString);
	
	return NO;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView 
{
	self.navigationController.navigationBar.userInteractionEnabled = YES;
}

#pragma mark Window delegate methods
- (void)tappedOnView:(UIView *)view atPoint:(CGPoint)point 
{
	
	
	/*
	 //An UIWebView will always have class structure as
	 //UIBrowserView->UIScrollView->UIWebView
	 if(![[[view superview] superview] isKindOfClass:[UIWebView class]])
	 return;
	 
	 //First, figure out if we have to show Record cover pages or directly the Record
	 MainPadAppDelegate *delegate = [UIApplication sharedApplication].delegate;
	 if([delegate displayRecordCovers]) {
	 //User has clicked on the magazine, show Record lists
	 [self showRecordNavigationView];
	 }
	 else {
	 [self showRecordView];
	 }
	 */
}

@end
