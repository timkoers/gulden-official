// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from libunity.djinni

#import "DBMutationRecord.h"


@implementation DBMutationRecord

- (nonnull instancetype)initWithChange:(int64_t)change
                             timestamp:(int64_t)timestamp
                                txHash:(nonnull NSString *)txHash
                                status:(DBTransactionStatus)status
                                 depth:(int32_t)depth
{
    if (self = [super init]) {
        _change = change;
        _timestamp = timestamp;
        _txHash = [txHash copy];
        _status = status;
        _depth = depth;
    }
    return self;
}

+ (nonnull instancetype)mutationRecordWithChange:(int64_t)change
                                       timestamp:(int64_t)timestamp
                                          txHash:(nonnull NSString *)txHash
                                          status:(DBTransactionStatus)status
                                           depth:(int32_t)depth
{
    return [[self alloc] initWithChange:change
                              timestamp:timestamp
                                 txHash:txHash
                                 status:status
                                  depth:depth];
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"<%@ %p change:%@ timestamp:%@ txHash:%@ status:%@ depth:%@>", self.class, (void *)self, @(self.change), @(self.timestamp), self.txHash, @(self.status), @(self.depth)];
}

@end