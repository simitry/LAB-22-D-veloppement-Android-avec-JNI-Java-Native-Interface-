#include <jni.h>
#include <string>
#include <algorithm>
#include <climits>
#include <android/log.h>

/*
 * Native side of the JNI lab.
 *
 * The exported function names must match:
 * Java_<package>_<class>_<method>
 *
 * Package: com.example.jnidemo
 * Class:   MainActivity
 */

#define LOG_TAG "JNI_DEMO"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// 1) Simple native hello.
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_MainActivity_helloFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    LOGI("Appel de helloFromJNI depuis le natif");
    return env->NewStringUTF("Hello from C++ via JNI !");
}

// 2) Factorial with explicit error codes.
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_jnidemo_MainActivity_factorial(
        JNIEnv * /* env */,
        jobject /* this */,
        jint n) {

    if (n < 0) {
        LOGE("Erreur : n negatif");
        return -1;
    }

    long long fact = 1;

    for (int i = 1; i <= n; i++) {
        fact *= i;

        /*
         * Java receives jint, which maps to a 32-bit int.
         * Returning a value bigger than INT_MAX would be misleading, so we stop.
         */
        if (fact > INT_MAX) {
            LOGE("Overflow detecte pour n=%d", n);
            return -2;
        }
    }

    LOGI("Factoriel de %d calcule en natif = %lld", n, fact);
    return static_cast<jint>(fact);
}

// 3) Java String -> C++ std::string -> reversed Java String.
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jnidemo_MainActivity_reverseString(
        JNIEnv *env,
        jobject /* this */,
        jstring javaString) {

    if (javaString == nullptr) {
        LOGE("Chaine nulle recue");
        return env->NewStringUTF("Erreur : chaine nulle");
    }

    /*
     * GetStringUTFChars can copy or pin internal data depending on the runtime.
     * Either way, ReleaseStringUTFChars is mandatory after use.
     */
    const char *chars = env->GetStringUTFChars(javaString, nullptr);
    if (chars == nullptr) {
        LOGE("Impossible de lire la chaine Java");
        return env->NewStringUTF("Erreur JNI");
    }

    std::string text(chars);
    env->ReleaseStringUTFChars(javaString, chars);

    std::reverse(text.begin(), text.end());

    LOGI("String inversee = %s", text.c_str());
    return env->NewStringUTF(text.c_str());
}

// 4) Java int[] -> native pointer -> sum -> jint.
extern "C"
JNIEXPORT jint JNICALL
Java_com_example_jnidemo_MainActivity_sumArray(
        JNIEnv *env,
        jobject /* this */,
        jintArray array) {

    if (array == nullptr) {
        LOGE("Tableau nul");
        return -1;
    }

    jsize length = env->GetArrayLength(array);
    jint *elements = env->GetIntArrayElements(array, nullptr);

    if (elements == nullptr) {
        LOGE("Impossible d'acceder aux elements du tableau");
        return -2;
    }

    long long sum = 0;

    for (jsize i = 0; i < length; i++) {
        sum += elements[i];
    }

    /*
     * JNI_ABORT would discard changes. Here we use 0 because we did not change
     * the values, but either 0 or JNI_ABORT would be safe for read-only usage.
     */
    env->ReleaseIntArrayElements(array, elements, 0);

    if (sum > INT_MAX || sum < INT_MIN) {
        LOGE("Overflow sur la somme");
        return -3;
    }

    LOGI("Somme du tableau = %lld", sum);
    return static_cast<jint>(sum);
}
