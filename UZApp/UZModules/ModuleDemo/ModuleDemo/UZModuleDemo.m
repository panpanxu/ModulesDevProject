//
//  UZModuleDemo.m
//  UZModule
//
//  Created by kenny on 14-3-5.
//  Copyright (c) 2014年 APICloud. All rights reserved.
//

#import "UZModuleDemo.h"
#import "UZAppDelegate.h"
#import "NSDictionaryUtils.h"
#import "XiaoYuSDK.h"

@interface UZModuleDemo ()
<UIAlertViewDelegate,XiaoYuSDKDelegate>
{
    NSInteger _cbId;
}

@end

@implementation UZModuleDemo

- (id)initWithUZWebView:(UZWebView *)webView_ {
    if (self = [super initWithUZWebView:webView_]) {
        
    }
    return self;
}

- (void)dispose {
    //do clean
}

- (void)showAlert:(NSDictionary *)paramDict {
    _cbId = [paramDict integerValueForKey:@"cbId" defaultValue:0];
    NSString *message = [paramDict stringValueForKey:@"msg" defaultValue:nil];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:nil message:message delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
    [alert show];
}

#pragma mark - UIAlertViewDelegate
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
    if (_cbId > 0) {
        NSDictionary *ret = @{@"index":@(buttonIndex+1)};
        [self sendResultEventWithCallbackId:_cbId dataDict:ret errDict:nil doDelete:YES];
    }
}

- (void)createMeeting:(NSDictionary *)paramDict {
    
    XYRunEnvironment env = (XYRunEnvironment)1;
    BOOL isLoginMode = false;
    
    NSString *exitId = @"44c29d08f73bc5e651260042a174f102b317b26e";
    
    [XiaoYuSDK initWithExtID:exitId
                    delegate:self
                   LoginMode:isLoginMode
                  Enviroment:env
               privateServer:@""];
    
    XYMeetingParam *param = [[XYMeetingParam alloc] init];
    param.meetingName = @"Sample meeting";
    param.startTime = [NSDate dateWithTimeIntervalSinceNow:3600].timeIntervalSince1970 * 1000;
    param.duration = 3600000;
    param.requirePassword = YES;
    param.maxParticipantCount = 15;
    [XiaoYuSDK createMeeting:param];
}

#pragma mark - XiaoYuSDKDelegate
-(void)XYDidCreateMeetingWithResult:(XYErrorCode)result forParam:(XYMeetingParam *)param returnMeeting:(XYMeetingCallee *)meeting
{
    if (result == XYErrorCode_OK)
    {
        NSString *text = [NSString stringWithFormat:@" 创建会议成功!\n 会议号 : %@\n 入会密码 : %@", meeting.number, meeting.password];
        NSLog(text);
        
//        XYMeetingCallee *meeting = [[XYMeetingCallee alloc] init];
        XYUser *user = [[XYUser alloc] init];
        //    user.displayName = @"sample user";
        //    user.extUserId = @"sample123";
        
        [XiaoYuSDK makeCallMeeting:meeting byUser:user];
    }
    else
    {
        NSString *errInfo = [NSString stringWithFormat:@" 创建会议失败!\n 错误号 : %ld", (long)result];
        NSLog(errInfo);
    }
}

-(void)XYDidMakeCallWithResult:(XYErrorCode)result forCallee:(XYBaseCallee *)callee
{
    if (result != XYErrorCode_OK)
    {
        NSString *errInfo = [NSString stringWithFormat:@" 呼叫失败!\n 错误号 : %ld", (long)result];
        NSLog(errInfo);
    }
}

-(void)XYCallStateChange:(XYCallState)state
{
    NSLog(@"%s -> state = %ld", __FUNCTION__, state);
}

-(void)XYParticipantChange:(int)participantCount
{
    NSLog(@"%s -> participantCount = %d", __FUNCTION__, participantCount);
}

@end
