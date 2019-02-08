// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from demo.djinni

#include "login_listener.hpp"
#include <memory>

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

@protocol SCLoginListener;

namespace djinni_generated {

class LoginListener
{
public:
    using CppType = std::shared_ptr<::demo::LoginListener>;
    using CppOptType = std::shared_ptr<::demo::LoginListener>;
    using ObjcType = id<SCLoginListener>;

    using Boxed = LoginListener;

    static CppType toCpp(ObjcType objc);
    static ObjcType fromCppOpt(const CppOptType& cpp);
    static ObjcType fromCpp(const CppType& cpp) { return fromCppOpt(cpp); }

private:
    class ObjcProxy;
};

}  // namespace djinni_generated

