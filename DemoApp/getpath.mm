//
//  getpath.m
//  DemoApp
//
//  Created by tiff on 2015. 1. 18..
//  Copyright (c) 2015년 tiff. All rights reserved.
//

#import <Foundation/Foundation.h>

#include <DK/dk.h>

using namespace DKFoundation;

DKString AbsolutePathStringOfPyStd(void)
{
	DKString str;
	@autoreleasepool {
		NSBundle* bundle = [NSBundle bundleWithIdentifier:@"com.icondb.PyDK"];
		NSString* s = [bundle pathForResource:@"python34" ofType:@"zip"];
		NSLog(@"python std lib: %@", s);
		str.SetValue(s.UTF8String);
	}
	return str;
}
