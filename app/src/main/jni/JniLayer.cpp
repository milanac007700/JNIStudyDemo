//
// Created by zqguo on 2019/10/18.
//

#include <jni.h>


extern "C"
JNIEXPORT jstring JNICALL
Java_com_milanac007_jnistudydemo_JavaLayer_getNativeInitStr(JNIEnv *env, jobject instance) {
    return env->NewStringUTF("Welcome to JNI.");//根据utf-编码的字符串构造一个新的java.lang.String对象。
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_milanac007_jnistudydemo_JavaLayer_java_1add(JNIEnv *env, jobject instance, //TOTO: "_" : _1
                                                     jint a, jint b) {
    return a+b;
}

extern "C"
jint Java_com_milanac007_jnistudydemo_JavaLayer_java_1sub(JNIEnv *env, jobject instance, jint a, jint b){
    return a-b;
}

