// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from libunity.djinni

#import "DBIRpcListener+Private.h"
#import "DBIRpcListener.h"
#import "DJIMarshal+Private.h"
#import "DJIObjcWrapperCache+Private.h"
#include <stdexcept>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

namespace djinni_generated {

class IRpcListener::ObjcProxy final
: public ::IRpcListener
, private ::djinni::ObjcProxyBase<ObjcType>
{
    friend class ::djinni_generated::IRpcListener;
public:
    using ObjcProxyBase::ObjcProxyBase;
    void onFilteredCommand(const std::string & c_filteredCommand) override
    {
        @autoreleasepool {
            [djinni_private_get_proxied_objc_object() onFilteredCommand:(::djinni::String::fromCpp(c_filteredCommand))];
        }
    }
    void onSuccess(const std::string & c_filteredCommand, const std::string & c_result) override
    {
        @autoreleasepool {
            [djinni_private_get_proxied_objc_object() onSuccess:(::djinni::String::fromCpp(c_filteredCommand))
                                                         result:(::djinni::String::fromCpp(c_result))];
        }
    }
    void onError(const std::string & c_filteredCommand, const std::string & c_errorMessage) override
    {
        @autoreleasepool {
            [djinni_private_get_proxied_objc_object() onError:(::djinni::String::fromCpp(c_filteredCommand))
                                                 errorMessage:(::djinni::String::fromCpp(c_errorMessage))];
        }
    }
};

}  // namespace djinni_generated

namespace djinni_generated {

auto IRpcListener::toCpp(ObjcType objc) -> CppType
{
    if (!objc) {
        return nullptr;
    }
    return ::djinni::get_objc_proxy<ObjcProxy>(objc);
}

auto IRpcListener::fromCppOpt(const CppOptType& cpp) -> ObjcType
{
    if (!cpp) {
        return nil;
    }
    return dynamic_cast<ObjcProxy&>(*cpp).djinni_private_get_proxied_objc_object();
}

}  // namespace djinni_generated
