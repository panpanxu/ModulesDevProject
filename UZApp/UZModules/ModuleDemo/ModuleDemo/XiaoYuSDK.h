//
//  XiaoYuSDK.h
//  XiaoYuSDK
//
//  Created by zhaoyun on 16/1/5.
//  Copyright © 2016年 ainemo. All rights reserved.
//

#import <Foundation/Foundation.h>

/*! @brief 小鱼SDK错误码
 *
 */
typedef NS_ENUM(NSInteger, XYErrorCode)
{
    XYErrorCode_OK = 0,
    XYErrorCode_Unknown = 1,
    XYErrorCode_Uninitialized = 2,
    XYErrorCode_NetworkError = 3,
    XYErrorCode_TimeOut = 4,
    XYErrorCode_calleeTypeError=5,
    XYErrorCode_InvalidKey = 60001,
    XYErrorCode_KeyAlreadyExist = 60002,
    XYErrorCode_InvalidSignature = 60003,
    XYErrorCode_EnterpriseRequired = 60004,
    XYErrorCode_UserInOtherEnterprise = 60005,
    XYErrorCode_InvalidAppId = 60006,
    XYErrorCode_MeetingNumberNotExist = 60007,
    XYErrorCode_MeetingNumberNotInEnterprise = 60008,
    XYErrorCode_InvalidEnterpriseDeviceSN = 60009,
    XYErrorCode_InvalidEnterpriseUserId = 60010
};

typedef NS_ENUM(NSInteger, XYCallState)
{
    XYCallState_Unknown,
    XYCallState_Connecting,
    XYCallState_Connected,
    XYCallState_Disconnected
};

typedef NS_ENUM(NSInteger, XYRunEnvironment)
{
    XYRunEnvironment_PublicProduction,
    XYRunEnvironment_PublicDebug,
    XYRunEnvironment_Private
};

/*! @brief 创建会议室请求参数类
 *
 */
@interface XYMeetingParam : NSObject

@property (nonatomic, assign) SInt64 startTime;//会议开始时间的时间戳，以毫秒为单位
@property (nonatomic, assign) NSTimeInterval duration;//会议持续时间的时间戳，以毫秒为单位
@property (nonatomic, strong) NSString *meetingName;
@property (nonatomic, assign) int maxParticipantCount;
@property (nonatomic, assign) BOOL requirePassword;

@end

@interface XYBaseCallee : NSObject
@property (nonatomic, strong) NSString *number;
@end

/*! @brief 会议室类
 *
 */
@interface XYMeetingCallee : XYBaseCallee
@property (nonatomic, strong) NSString *password;
@end

/*! @brief 小鱼设备类
 *
 */
@interface XYDeviceCallee : XYBaseCallee
@end

/*! @brief H323设备类
 *
 */
@interface XYH323Callee : XYBaseCallee
@property (nonatomic, strong) NSString *password;
@end

/*! @brief TVBOX类
 *
 */
@interface XYTVBox : XYBaseCallee
@property (nonatomic, copy) NSString *password;
@end

/*! @brief 小鱼用户类
 *
 */
@interface XYUser : NSObject

@property (nonatomic, assign) long userId; // 用户ID
@property (nonatomic, strong) NSString *displayName;
@property (nonatomic, strong) NSString *securityKey; // 会话认证Key
@property (nonatomic, strong) NSString *cellPhone; // 手机号（包含国家码）eg: +86-18612438628
@property (nonatomic, strong) NSString *profilePicture; // 用户头像 (可选字段)
@property (nonatomic, copy) NSString *extUserId; //外部用户ID

@end

@protocol XiaoYuSDKDelegate <NSObject>

@optional

-(void)XYDidCreateMeetingWithResult:(XYErrorCode) result
                                  forParam:(XYMeetingParam*)param
                             returnMeeting:(XYMeetingCallee*)meeting;

-(void)XYDidMakeCallWithResult:(XYErrorCode)result
                            forCallee:(XYBaseCallee*)callee;

-(void)XYParticipantChange:(int)participantCount;

-(void)XYCallStateChange:(XYCallState)state;

@end

@interface XiaoYuSDK : NSObject

/*! @brief 初始化 XiaoYu SDK
 *
 * @attention 使用SDK其他接口前，一定要调用且只调用一次
 * @param extId 企业身份标识
 * @param delegate 接口返回回调
 * @param isLoginMode 登录模式/临时模式
 * @param enviroment 开发环境/正式环境/私有云环境
 * @param privateServer 私有环境下的server 地址
 */
+ (void)initWithExtID:(NSString*)extId
             delegate:(id<XiaoYuSDKDelegate>)delegate
            LoginMode:(BOOL)isLoginMode
           Enviroment:(XYRunEnvironment)enviroment
        privateServer:(NSString*)privateServer;

/*! @brief 创建云会议室
 *
 * @param param 云会议室参数
 */
+ (void)createMeeting:(XYMeetingParam*)param;


/*! @brief 呼叫云会议室
 *
 * @param meeting 云会议室对象
 * @param user 呼叫方用户对象
 */
+ (void)makeCallMeeting:(XYMeetingCallee*)meeting
                 byUser:(XYUser*)user;

/*! @brief 呼叫小鱼设备
 *
 * @param device 设备对象
 * @param user 呼叫方用户对象
 */
+ (void)makeCallDevice:(XYDeviceCallee*)device
                byUser:(XYUser*)user;


/*! @brief 呼叫小鱼设备
 *
 * @param H323 H323对象
 * @param user 呼叫方用户对象
 */
+ (void)makeCallH323:(XYH323Callee*)H323
                byUser:(XYUser*)user;


/*! @brief 呼叫TVBOX
 *
 * @param tvBox TVBOX对象
 * @param user 呼叫方用户对象
 */
+ (void)makeCallTVBox:(XYTVBox*)tvBox
              byUser:(XYUser*)user;

//挂断电话
+ (void)hangup;

@end
