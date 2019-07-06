// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from libunity.djinni

#import "DBInputRecord.h"


@implementation DBInputRecord

- (nonnull instancetype)initWithAddress:(nonnull NSString *)address
                                  label:(nonnull NSString *)label
                                 isMine:(BOOL)isMine
{
    if (self = [super init]) {
        _address = [address copy];
        _label = [label copy];
        _isMine = isMine;
    }
    return self;
}

+ (nonnull instancetype)inputRecordWithAddress:(nonnull NSString *)address
                                         label:(nonnull NSString *)label
                                        isMine:(BOOL)isMine
{
    return [(DBInputRecord*)[self alloc] initWithAddress:address
                                                   label:label
                                                  isMine:isMine];
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"<%@ %p address:%@ label:%@ isMine:%@>", self.class, (void *)self, self.address, self.label, @(self.isMine)];
}

@end