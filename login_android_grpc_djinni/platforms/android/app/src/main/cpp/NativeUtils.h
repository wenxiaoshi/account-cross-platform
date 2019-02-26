//
// Created by melon on 2019/2/3.
//

#include <jni.h>
#include <dlfcn.h>
#include <string>

namespace utils{
    class NativeUtils{
    public:

        static JNIEnv* dlsmgetenv()
        {
            JNIEnv*(*getAndroidRuntimeEnv)();

            void*runtime = dlopen("/system/lib/libandroid_runtime.so", RTLD_NOW);
            getAndroidRuntimeEnv= (JNIEnv*(*)())dlsym(runtime, "_ZN7android14AndroidRuntime9getJNIEnvEv");
            return getAndroidRuntimeEnv();
        }

        static jstring getJStringByStdString(JNIEnv* env,std::string stdString){
            const char *str = stdString.data();
            jstring string = env->NewStringUTF (str);
            return string;
        }

        static std::string getStdStringByJString(JNIEnv* env,jstring string){
            const char* str = env->GetStringUTFChars(string,0);
            std::string stdString = str;
            return stdString;
        }

        static std::string getDeviceId(){
            JNIEnv* env = dlsmgetenv();
            jclass jc = env->FindClass("com/wechat/mylogin/utils/DeviceUtils");
            jmethodID jm = env->GetStaticMethodID(jc,"getDeviceId","()Ljava/lang/String;");
            jstring js = static_cast<jstring>(env->CallStaticObjectMethod(jc, jm));
            return getStdStringByJString(env,js);
        }

    };
}
