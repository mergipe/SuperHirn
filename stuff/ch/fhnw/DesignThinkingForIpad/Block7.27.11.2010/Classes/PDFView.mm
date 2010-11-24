#import "PDFView.h"

#import "StudentController.h"

@implementation PDFView


- (id)initWithFrame:(CGRect)frame 
{
    if ((self = [super initWithFrame:frame])) 
	{
		self.backgroundColor = [UIColor grayColor];
		NSArray *viewsToRemove = [self subviews];
		for (UIView *v in viewsToRemove) {
			[v removeFromSuperview];
		}
		pageNumber = 0;
    }
    return self;
}


-(void) nextPage
{
	pageNumber++;
}

-(void) previousPage
{
	pageNumber--;
}


- (void) setPdf:(NSString*) iPath
{
	//NSString* path  = [[StudentController getDataFolderPath] stringByAppendingPathComponent:iPath];
	myPageRef = CGPDFDocumentCreateWithURL((CFURLRef)[NSURL fileURLWithPath:iPath]);
	pageNumber = 1;
}

- (void)drawRect:(CGRect)rect 
{
	[self drawPDFinContext:UIGraphicsGetCurrentContext()];
}

-(void)drawPDFinContext:(CGContextRef)context
{
	// draw the pdf
	//draw a black rectangle around the pdf:
	CGContextSetLineWidth(context, 100); 
	CGContextScaleCTM(context, 1,2);
	CGContextBeginPath(context);
	CGFloat c[4] = {1,1,1,0.5};
	CGContextSetFillColor(context, c);
	CGContextFillRect(context, self.frame);
	CGContextStrokePath(context);
	
	CGContextTranslateCTM(context, -120, 535);
	CGContextScaleCTM(context, 1.3, -0.6);
	CGPDFPageRef page = CGPDFDocumentGetPage(myPageRef, pageNumber);
	CGContextSaveGState(context);
	CGAffineTransform pdfTransform = CGPDFPageGetDrawingTransform(page, kCGPDFCropBox, self.bounds, 0, true);
	CGContextConcatCTM(context, pdfTransform);
	CGContextDrawPDFPage(context, page);
	
	
	CGContextRestoreGState(context);

}


- (void)dealloc
{
	[super dealloc];
}


@end
