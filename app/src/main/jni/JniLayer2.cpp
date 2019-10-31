//
// Created by zqguo on 2019/10/18.
//
#include <jni.h>
#include <stdio.h>
#include <android/log.h>
#include <string>
#include <iostream>
const char *TAG = "JniLayer";

#include <android/log.h>
//#define TAG "projectname" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

void logI(const char * tag, const char *fmt, ...){
    va_list argptr;
    char tmp[0xff] = { 0 };
    va_start(argptr, fmt);
    vsnprintf(tmp, 0xff, fmt, argptr);
    va_end(argptr);

    __android_log_print(ANDROID_LOG_INFO, tag, "%s", tmp);
}


jint native_div(JNIEnv *env, jobject obj, jint a, jint b) {
    return a/b;
}

jstring native_str(JNIEnv *env, jobject obj, jstring value){
    jboolean isCopy = JNI_FALSE;
    jsize len = env->GetStringUTFLength(value);
//    const char * utf =  env->GetStringUTFChars(value, &isCopy);
    char * utf =  (char *)env->GetStringUTFChars(value, &isCopy);
    logI(TAG, "native_str() isCopy: %s", isCopy?"true":"false");
    if(utf == NULL) {
        return NULL;
    }

    if(isCopy == JNI_TRUE) {
        logI(TAG, utf);
        env->ReleaseStringUTFChars(value, utf);//不释放会导致GC不会回收value，导致内存泄露


        logI(TAG, "strcpy(utf, \"12345\")");
        strcpy(utf, "12345");
        logI(TAG, utf);

        jstring demo = env->NewStringUTF("hello world");
        jsize len = env->GetStringUTFLength(demo);
        logI(TAG, "hello world's length: %d", len);
        env->GetStringUTFRegion(demo, 0, len, utf);//注：如果len>utf指向的内存长度，数组越界，导致异常
        logI(TAG, "exec GetStringUTFRegion()");
        logI(TAG, utf);
        env->DeleteLocalRef(demo);

    } else{
        //TODO ：不能操作指针修改字符串的内容，因为JVM中的原始字符串也会被更改，这会打破Java中字符串不可变的原则，导致崩溃。
    }

    return env->NewStringUTF(utf);
}

void native_addOne(JNIEnv *env, jobject obj, jintArray values){
    jboolean isCopy = JNI_FALSE;
    jint * pointer = env->GetIntArrayElements(values, &isCopy);
    logI(TAG, "native_addOne() isCopy: %s", isCopy?"true":"false");

    jsize size = env->GetArrayLength(values);
    for(int i=0; i<size; i++){
        pointer[i] += 1;
    }
    env->ReleaseIntArrayElements(values, pointer, 0);
//    env->ReleaseIntArrayElements(values, pointer, JNI_COMMIT);
//    env->ReleaseIntArrayElements(values, pointer, JNI_ABORT);

}

jbyteArray native_byteArray(JNIEnv *env, jobject obj, jbyteArray src) {
    jboolean isCopy = JNI_FALSE;
    jbyte * pointer = env->GetByteArrayElements(src, &isCopy);
//    typedef int8_t   jbyte;    /* signed 8 bits , defined from jni.h */
//    typedef __int8_t      int8_t;/*defined from stdint.h */
//    typedef signed char __int8_t;
//    故： jbyte * <==> signed char *
    jsize len = env->GetArrayLength(src);
    logI(TAG, "native_byteArray() isCopy: %s", isCopy?"true":"false");

    if(pointer == NULL) {
        return NULL;
    }

    jbyteArray dst = env->NewByteArray(10);
    env->SetByteArrayRegion(dst, 0, len>10?10:len, pointer);
    env->ReleaseByteArrayElements(src, pointer, 0);
    return dst;
}

jobject native_newjobject(JNIEnv *env, jobject obj){
    jclass cls = env->FindClass("com/milanac007/jnistudydemo/Student");
    if(env->ExceptionOccurred()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if(cls == NULL){
        return NULL;
    }

    //创建方式一
    jobject student = env->AllocObject(cls);
    jfieldID idID = env->GetFieldID(cls, "id", "I");
    env->SetIntField(student, idID, 1);
    jobject name = env->NewStringUTF("李雷");
    jfieldID nameID = env->GetFieldID(cls, "name", "Ljava/lang/String;");
    env->SetObjectField(student, nameID, name);

    jmethodID sexID = env->GetMethodID(cls, "setSex", "(I)V");
    if(sexID != NULL) {
        env->CallVoidMethod(student, sexID, 0);
        if(env->ExceptionOccurred()){
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
    }

    //创建方式二
//    jmethodID constructID = env->GetMethodID(cls, "<init>", "(ILjava/lang/String;I)V");
//    if(env->ExceptionOccurred()){
//        env->ExceptionDescribe();
//        env->ExceptionClear();
//        return NULL;
//    }
//    jstring name = env->NewStringUTF("韩美美");
//    jobject student = env->NewObject(cls, constructID, 2, name, 1);

    env->DeleteLocalRef(name);


    jmethodID toStringID = env->GetMethodID(cls, "toString", "()Ljava/lang/String;");
    jstring toStr = (jstring)env->CallObjectMethod(student, toStringID);
    if(env->ExceptionOccurred()){
        env->ExceptionDescribe();
        env->ExceptionClear();
        env->DeleteLocalRef(toStr);
    } else{
        const char *p = env->GetStringUTFChars(toStr, NULL);
        env->ReleaseStringUTFChars(toStr, p);
        env->DeleteLocalRef(toStr);
        logI(TAG, p);
    }

    return student;
}


jobjectArray native_newjobjectArray(JNIEnv *env, jobject obj) {
    jclass cls = env->FindClass("com/milanac007/jnistudydemo/Student");
    if(env->ExceptionOccurred()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if(cls == NULL){
        return NULL;
    }

    jmethodID initID = env->GetMethodID(cls, "<init>", "(ILjava/lang/String;I)V");

    jstring default_name = env->NewStringUTF("未知");
    jobject stu = env->NewObject(cls, initID, -1, default_name, -1);
    jobjectArray stuList = env->NewObjectArray(5, cls, stu);


    jstring name1 = env->NewStringUTF("李明");
    jobject stu1 = env->NewObject(cls, initID, 1, name1, 0);
    jstring name2 = env->NewStringUTF("王小虎");
    jobject stu2 = env->NewObject(cls, initID, 2, name2, 0);
    jstring name3 = env->NewStringUTF("王娇");
    jobject stu3 = env->NewObject(cls, initID, 3, name3, 1);

    env->SetObjectArrayElement(stuList, 0, stu1);
    env->SetObjectArrayElement(stuList, 1, stu2);
    env->SetObjectArrayElement(stuList, 2, stu3);

    return stuList;
}

void native_accessStaticMember(JNIEnv *env, jobject obj){
    jclass cls = env->FindClass("com/milanac007/jnistudydemo/Student");
    if(env->ExceptionOccurred()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if(cls == NULL){
        return ;
    }

    jfieldID codeID = env->GetStaticFieldID(cls, "code", "I");
    if(env->ExceptionOccurred()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    } else if(codeID != NULL){
        int code = env->GetStaticIntField(cls, codeID);
        logI(TAG, "code: 0x%02x", code);
        env->SetStaticIntField(cls, codeID, 0x0a);
        logI(TAG, "SetStaticIntField() code=0x0a");
        code = env->GetStaticIntField(cls, codeID);
        logI(TAG, "code: 0x%02x", code);
    }

    jmethodID versionID = env->GetStaticMethodID(cls, "getVersion", "()Ljava/lang/String;");
    if(env->ExceptionOccurred()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    } else if(versionID != NULL) {
        jstring versionStr = (jstring)env->CallStaticObjectMethod(cls, versionID);
        const char * version = env->GetStringUTFChars(versionStr, NULL);
        logI(TAG, "version :%s", version);
    }

}

void native_primitiveArrayCritical(JNIEnv *env, jobject obj, jintArray src) {
    jboolean isCopy = JNI_FALSE;
    int *pointer = (int *)env->GetPrimitiveArrayCritical(src, &isCopy);
    logI(TAG, "GetPrimitiveArrayCritical(), isCopy=%s", isCopy?"true":"false");
    jsize len = env->GetArrayLength(src);
    for(int i=0;i<len; i++){
        pointer[i] += 1;
    }
//    env->ReleasePrimitiveArrayCritical(src, pointer, 0);
//    env->ReleasePrimitiveArrayCritical(src, pointer, JNI_COMMIT);
    env->ReleasePrimitiveArrayCritical(src, pointer, JNI_ABORT);
}

void native_CallNonvirtualMethod(JNIEnv *env, jobject obj, jobject instance){
    jclass cls = env->GetObjectClass(instance);
    jmethodID toStringID1 = env->GetMethodID(cls, "toString", "()Ljava/lang/String;");
    if(env->ExceptionOccurred()){
        env->ExceptionDescribe();
        env->ExceptionClear();
        return;
    }

    jstring str1 = (jstring)env->CallObjectMethod(instance, toStringID1);
    const char *p1 =  env->GetStringUTFChars(str1, NULL);
    logI(TAG, "child toString(): %s", p1);


    jclass supperCls = env->GetSuperclass(cls);
    jmethodID toStringID2 = env->GetMethodID(supperCls, "toString", "()Ljava/lang/String;");
    if(env->ExceptionOccurred()){
        env->ExceptionDescribe();
        env->ExceptionClear();
        return;
    }
    jstring str2 = (jstring)env->CallNonvirtualObjectMethod(instance, supperCls, toStringID2);

    const char *p2 =  env->GetStringUTFChars(str2, NULL);
    logI(TAG, "supper toString(): %s", p2);

    // 删除局部引用（jobject或jobject的子类才属于引用变量），允许VM释放被局部变量所引用的资源
    env->DeleteLocalRef(cls);
    env->ReleaseStringUTFChars(str1, p1);
    env->DeleteLocalRef(str1);

    env->DeleteLocalRef(supperCls);
    env->ReleaseStringUTFChars(str2, p2);
    env->DeleteLocalRef(str2);

}

static JNINativeMethod gMethods[] = {
        {"java_div", "(II)I", (void *)native_div},
        {"java_addOne", "([I)V", (void *)native_addOne},
        {"java_str", "(Ljava/lang/String;)Ljava/lang/String;", (void *)native_str},
        {"java_byteArray", "([B)[B", (void *)native_byteArray},
        {"java_newjobject", "()Lcom/milanac007/jnistudydemo/Student;", (void *)native_newjobject},
        {"java_newjobjectArray", "()[Lcom/milanac007/jnistudydemo/Student;", (void *)native_newjobjectArray},
        {"java_accessStaticMember", "()V", (void *)native_accessStaticMember},
        {"java_primitiveArrayCritical", "([I)V", (void *)native_primitiveArrayCritical},
        {"java_CallNonvirtualMethod", "(Lcom/milanac007/jnistudydemo/Student;)V", (void *)native_CallNonvirtualMethod},
};


//这是调用native方法java的类路径
static const char * const kClassName = "com/milanac007/jnistudydemo/JavaLayer";
jint JNI_OnLoad(JavaVM *vm, void *reserved){
    JNIEnv *env = NULL;
    jint result = JNI_FALSE ;

    if(vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK) {
        logI(TAG, "GetEnv failed.");
        return result;
    }

    jclass myClass = env->FindClass(kClassName);
    if(myClass == NULL){
        logI(TAG, "can't get class %s\n", kClassName);
        jthrowable ex = env->ExceptionOccurred();
        if(ex) {
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        return result;
    }

    if(env->RegisterNatives(myClass, gMethods, sizeof(gMethods)/ sizeof(gMethods[0])) <0) {
        logI(TAG, "regeister native method failed\n");
        return result;
    }

    logI(TAG, "---JNI_OnLoad---");
    return JNI_VERSION_1_4;
}





