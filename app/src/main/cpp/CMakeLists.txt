cmake_minimum_required(VERSION 3.22.1)

project("jnidemo")

#
# native-lib becomes libnative-lib.so in the APK.
# Java loads it with System.loadLibrary("native-lib").
#
add_library(
        native-lib
        SHARED
        native-lib.cpp)

#
# Android native logging library.
# It gives us __android_log_print, visible in Logcat.
#
find_library(
        log-lib
        log)

target_link_libraries(
        native-lib
        ${log-lib})
