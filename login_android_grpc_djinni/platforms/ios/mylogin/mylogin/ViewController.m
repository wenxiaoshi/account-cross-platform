//
//  ViewController.m
//  mylogin
//
//  Created by melon on 2019/6/21.
//  Copyright © 2019 melon. All rights reserved.
//

#import "ViewController.h"
#import "SCLoginCore.h"
#import "SCLoginListener.h"

@interface ViewController ()
@property (strong, nonatomic) IBOutlet UITextField *mEditPhone;
@property (strong, nonatomic) IBOutlet UITextField *mEditPassword;
@property (strong, nonatomic) SCLoginCore* mLoginCore;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.

    _mLoginCore = [SCLoginCore create:self];
    
    [_mLoginCore checkLoginStatus];
//    [_mLoginCore userSign:@"13533192332" password:@"123456"];
}

- (IBAction)handleIntentLoginPage:(id)sender {
    
}

- (IBAction)handleIntentSignAction:(id)sender {
    [_mLoginCore userLogin:@"13533192332" password:@"123456"];
//    [_mLoginCore userLogin:(@"13533192332") password:(@"1234")];
}

- (void)onLoginFinish:(nonnull SCActionResult *)result{
    
}

- (void)onSignFinish:(nonnull SCActionResult *)result{
    
}

- (void)onLogoutFinish:(nonnull SCActionResult *)result{
    
}

- (void)onCheckStatusFinish:(nonnull SCActionResult *)result{
    
}

- (void)onDisconnect:(nonnull SCActionResult *)result{
    
}

@end
