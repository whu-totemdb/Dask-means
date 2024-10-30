#include <jni.h>
#include <iostream>
#include "experiment.h"
#include <android/log.h>
#include <unistd.h>

#define LOG_TAG "DaskMeans"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
using namespace std;

extern "C" {

    JNIEXPORT jdouble JNICALL
    Java_com_example_dask_1means_MainActivity_testLloyd(JNIEnv *env, jobject obj) {
        LOGD("Starting Lloyd test");
        Experiment experiment;
        double total_runtime = experiment.test_Lloyd();
        LOGD("Lloyd test finished");
        return total_runtime;
    }

    JNIEXPORT jdouble JNICALL
    Java_com_example_dask_1means_MainActivity_testDaskMeans(JNIEnv *env, jobject obj) {
        LOGD("Starting DaskMeans test");
        Experiment experiment;
        double total_runtime = experiment.test_dask_means();
        LOGD("DaskMeans test finished");
        return total_runtime;
    }

    JNIEXPORT jdouble JNICALL
    Java_com_example_dask_1means_MainActivity_testNoInB(JNIEnv *env, jobject obj) {
        LOGD("Starting NoInB test");
        Experiment experiment;
        double total_runtime = experiment.test_NoInB();
        LOGD("NoInB test finished");
        return total_runtime;
    }

    JNIEXPORT jdouble JNICALL
    Java_com_example_dask_1means_MainActivity_testNoKnn(JNIEnv *env, jobject obj) {
        LOGD("Starting NoKnn test");
        Experiment experiment;
        double total_runtime = experiment.test_NoKnn();
        LOGD("NoKnn test finished");
        return total_runtime;
    }

    JNIEXPORT jdouble JNICALL
    Java_com_example_dask_1means_MainActivity_testNoBound(JNIEnv *env, jobject obj) {
        LOGD("Starting NoBound test");
        Experiment experiment;
        double total_runtime = experiment.test_NoBound();
        LOGD("NoBound test finished");
        return total_runtime;
    }

    JNIEXPORT jdouble JNICALL
    Java_com_example_dask_1means_MainActivity_testDualTree(JNIEnv *env, jobject obj) {
        LOGD("Starting DualTree test");
        Experiment experiment;
        double total_runtime = experiment.test_DualTree();
        LOGD("DualTree test finished");
        return total_runtime;
    }

    JNIEXPORT jdouble JNICALL
    Java_com_example_dask_1means_MainActivity_testHamerly(JNIEnv *env, jobject obj) {
        LOGD("Starting Hamerly test");
        Experiment experiment;
        double total_runtime = experiment.test_Hamerly();
        LOGD("Hamerly test finished");
        return total_runtime;
    }

    JNIEXPORT jdouble JNICALL
    Java_com_example_dask_1means_MainActivity_testDrake(JNIEnv *env, jobject obj) {
        LOGD("Starting Drake test");
        Experiment experiment;
        double total_runtime = experiment.test_Drake();
        LOGD("Drake test finished");
        return total_runtime;
    }

    JNIEXPORT jdouble JNICALL
    Java_com_example_dask_1means_MainActivity_testYinYang(JNIEnv *env, jobject obj) {
        LOGD("Starting YinYang test");
        Experiment experiment;
        double total_runtime = experiment.test_YinYang();
        LOGD("YinYang test finished");
        return total_runtime;
    }

    JNIEXPORT jdouble JNICALL
    Java_com_example_dask_1means_MainActivity_testElkan(JNIEnv *env, jobject obj) {
        LOGD("Starting Elkan test");
        Experiment experiment;
        double total_runtime = experiment.test_Elkan();
        LOGD("Elkan test finished");
        return total_runtime;
    }

}
