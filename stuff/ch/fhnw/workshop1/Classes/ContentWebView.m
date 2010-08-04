//
//  ArticleCoverView.m
//  AkiliPad
//
//  Created by Mithin on 24/04/10.
//  Copyright 2010 Techtinium Corporation. All rights reserved.
//

#import "ContentWebView.h"


@implementation ContentWebView

@synthesize pageIndex;
@synthesize lastTappedElement;

- (void)setPageIndex:(NSInteger)newPageIndex {
	pageIndex = newPageIndex;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

- (void)dealloc {
	[lastTappedElement release];
    [super dealloc];
}

- (NSString *)elementAtX:(int)x andY:(int)y {
	NSString *jsStr = [NSString stringWithFormat:@"		xPos = function(node) {"
					   "       var x=0;"
					   "       var n=node;"
					   "       while(n) {"
					   "               x += n.offsetLeft;"
					   "               n = n.offsetParent;"
					   "       }"
					   "       return x;"
					   "}\n"
					   "yPos = function(node) {"
					   "       var y=0;"
					   "       var n=node;"
					   "       while(n) {"
					   "               y += n.offsetTop;"
					   "               n = n.offsetParent;"
					   "       }\n"
					   "       return y;"
					   "}\n"
					   "elementAt = function(x, y, node, level) {"
					   "       if(level == null || level == undefined)"
					   "               level=1;"
					   "       var result=null;"
					   "       var n=node;"
					   "       var c=node.firstChild;"
					   "       while(c) {"
					   "               var ex=xPos(c);"
					   "               var ey=yPos(c);"
					   "               if(ex<=x && ex+c.offsetWidth>=x && ey<=y && ey+c.offsetHeight>=y) {"
					   "                       if(!result || result.score>c.offsetHeight) {"
					   "                               result=new Object;"
					   "                               result.node=c;"
					   "                               result.score=c.offsetHeight;"
					   "                       }\n"
					   "               }\n"
					   "               var r=elementAt(x, y, c, level+1);"
					   "               if(r && (!result || r.score<result.score))"
					   "                       result=r;"
					   "               c=c.nextSibling;"
					   "       }\n"
					   "       return result;"
					   "}\n"
					   "var r=elementAt(%d, %d, document);"
					   "if(r)"	
					   "       r.node.id;"
					   "else"
					   "       null;", x, y];
	
	return [self stringByEvaluatingJavaScriptFromString:jsStr];
}

- (NSString *)parentElement:(NSString *)elementName {
	NSLog(@"Finding parent node for %@", elementName);
	NSString *jsStr = [NSString stringWithFormat:@"document.getElementById('%@').parentNode.id;", elementName];
	NSString *rStr = [self stringByEvaluatingJavaScriptFromString:jsStr];
	NSLog(@"JS Return string = %@", rStr);
	return rStr;
}

- (NSString *)idByElement:(NSString *)elementName {
	NSString *jsStr = [NSString stringWithFormat:@"var orignode=%@;"
					   "var node=orignode;"
					   "var lev=0;"
					   "while(node && !node.id) {"
					   "	 node=node.parentNode;"
					   "    lev++;"
					   "}\n"
					   "if(!node)"
					   "	 '';"
					   "else if(lev==0)"
					   "    node.id;"
					   "else {"
					   "    var s='';"
					   "    var cur=orignode;"
					   "    while(lev) {"
					   "        var p=cur.parentNode;"
					   "        var c=p.firstChild;"
					   "        var cs = '.firstChild';"
					   "        while(c != cur) {"
					   "            cs += '.nextSibling';"
					   "            c = c.nextSibling;"
					   "        }"
					   "        s=cs + s;"
					   "        cur=p;"
					   "        lev--;"
					   "    }"
					   "    \"document.getElementById('\" + node.id + \"')\" +s;"
					   "}", elementName];
	
	return [self stringByEvaluatingJavaScriptFromString:jsStr];
}

- (int)scrollPosition {
	NSString *yStr = [self stringByEvaluatingJavaScriptFromString:@"window.pageYOffset"];
	return [yStr intValue];
}

- (void)scrollToPosition:(int)yPos {
	[self stringByEvaluatingJavaScriptFromString:[NSString stringWithFormat:@"window.scrollTo(0, %d);", yPos]];
}

- (int)yPosOfElementWithName:(NSString *)elementName {
	NSString *jsStr = [NSString stringWithFormat:@"var element=document.getElementById('%@');"
					   "if(element == null)"
					   "       exit;"
					   "var y=0;"
					   "while(element!=null) {"
					   "       y += element.offsetTop;"
					   "       element = element.offsetParent;"
					   "}\n"
					   "y;", elementName];
	NSString *resultStr = [self stringByEvaluatingJavaScriptFromString:jsStr];
	if(resultStr && [resultStr length]) {
		return [resultStr intValue];
	}
	
	return -1;
}

- (CGPoint)coordinatesOfElement:(NSString *)elementName {
	NSString *jsStr = [NSString stringWithFormat:@"var e=%@;"
					   "var curLeft=curTop=0;"
					   "while(e && !e.offsetTop) e=e.parentNode;"
					   "do {"
					   "	if(e.offsetLeft)"
					   "		curLeft += e.offsetLeft;"
					   "	if(e.offsetTop)"
					   "		curTop += e.offsetTop;"
					   "} while(e = e.offsetParent);"
					   "curLeft;", elementName];
	CGFloat x = [[self stringByEvaluatingJavaScriptFromString:jsStr] floatValue];
	
	jsStr = @"curTop;";
	CGFloat y = [[self stringByEvaluatingJavaScriptFromString:jsStr] floatValue];
	return CGPointMake(x, y);
}

@end
