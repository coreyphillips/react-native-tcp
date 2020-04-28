/**
 * Copyright (c) 2015-present, Peel Technologies, Inc.
 * All rights reserved.
 */

#import <React/RCTBridgeModule.h>
#import "CocoaAsyncSocketCustomSSL/GCDAsyncSocketCustomSSL.h"

extern NSString *const RCTTCPErrorDomain;

enum RCTTCPError
{
    RCTTCPNoError = 0,           // Never used
    RCTTCPInvalidInvocationError,// Invalid method invocation
    RCTTCPBadConfigError,        // Invalid configuration
    RCTTCPBadParamError,         // Invalid parameter was passed
    RCTTCPSendTimeoutError,      // A send operation timed out
    RCTTCPSendFailedError,       // A send operation failed
    RCTTCPClosedError,           // The socket was closed
    RCTTCPOtherError,            // Description provided in userInfo
};

typedef enum RCTTCPError RCTTCPError;

@class TcpSocketClient;

@protocol SocketClientDelegate <NSObject>

- (void)onConnect:(TcpSocketClient*)client;
- (void)onConnection:(TcpSocketClient*)client toClient:(NSNumber *)clientID;
- (void)onData:(NSNumber *)clientID data:(NSData *)data;
- (void)onClose:(TcpSocketClient*)client withError:(NSError *)err;
- (void)onError:(TcpSocketClient*)client withError:(NSError *)err;
- (void)onSecureConnect:(TcpSocketClient*)client;
- (NSNumber*)getNextId;

@end

@interface TcpSocketClient : NSObject<GCDAsyncSocketDelegate>

@property (nonatomic, retain) NSNumber * id;
@property (nonatomic, weak) id<SocketClientDelegate> clientDelegate;
@property (nonatomic) BOOL useSsl;

///---------------------------------------------------------------------------------------
/// @name Class Methods
///---------------------------------------------------------------------------------------
/**
 * Initializes a new RCTTCPClient
 *
 * @param delegate The object holding the callbacks, usually 'self'.
 *
 * @return New RCTTCPClient
 */

+ (id)socketClientWithId:(NSNumber *)clientID andConfig:(id<SocketClientDelegate>) delegate;

///---------------------------------------------------------------------------------------
/// @name Instance Methods
///---------------------------------------------------------------------------------------
/**
 * Connects to a host and port
 *
 * @param port
 * @param host ip address
 * @param options NSDictionary which can have @"localAddress" and @"localPort" to specify the local interface
 * @param useSsl BOOL Use SSL/TLS connection
 * @return true if connected, false if there was an error
 */
- (BOOL)connect:(NSString *)host port:(int)port withOptions:(NSDictionary *)options useSsl:(BOOL)useSsl error:(NSError **)error;
- (BOOL)connect:(NSString *)host port:(int)port withOptions:(NSDictionary *)options error:(NSError **)error;

- (void)upgradeToSecure:(NSString *)host port:(int)port callback:(RCTResponseSenderBlock) callback;

/**
 * Starts listening on a local host and port
 *
 * @param local ip address
 * @param local port
 * @return true if connected, false if there was an error
 */
- (BOOL)listen:(NSString *)host port:(int)port error:(NSError **)error;

/**
 * Returns the address information
 *
 * @return NSDictionary with @"address" host, @"port" port, @"family" IPv4/IPv6
 */
- (NSDictionary<NSString *, id> *)getAddress;

/**
 * write data
 *
 */
- (void)writeData:(NSData*) data callback:(RCTResponseSenderBlock) callback;

/**
 * end client
 */
- (void)end;

/**
 * destroy client
 */
- (void)destroy;

- (void)socket:(GCDAsyncSocket *)sock didReceiveTrust:(SecTrustRef)trust
completionHandler:(void (^)(BOOL shouldTrustPeer))completionHandler;


@end
