LOCAL_DCANVAS_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_C_INCLUDES := $(LOCAL_DCANVAS_PATH)/include

LOCAL_SRC_ROOT := $(LOCAL_DCANVAS_PATH)/src

LOCAL_SRC_FILES := \
	$(LOCAL_SRC_ROOT)/Audio/Audio.cpp \
	$(LOCAL_SRC_ROOT)/Audio/SDLAudio.cpp \
	$(LOCAL_SRC_ROOT)/Canvas/Bitmap.cpp \
	$(LOCAL_SRC_ROOT)/Canvas/CanvasRenderingContext2D.cpp \
	$(LOCAL_SRC_ROOT)/Canvas/SDLBitmap.cpp \
	$(LOCAL_SRC_ROOT)/Canvas/Canvas.cpp \
	$(LOCAL_SRC_ROOT)/Canvas/SDLCanvasRenderingContext2D.cpp \
	$(LOCAL_SRC_ROOT)/Canvas/SDLOffscreenCanvas.cpp \
	$(LOCAL_SRC_ROOT)/Canvas/SDLWindowCanvas.cpp \
	$(LOCAL_SRC_ROOT)/duktape/bindings/audio/dukAudio.cpp \
	$(LOCAL_SRC_ROOT)/duktape/bindings/bitmap/dukBitmap.cpp \
	$(LOCAL_SRC_ROOT)/duktape/bindings/canvas/drawImage.cpp \
	$(LOCAL_SRC_ROOT)/duktape/bindings/canvas/dukCanvas.cpp \
	$(LOCAL_SRC_ROOT)/duktape/bindings/console/dukConsole.cpp \
	$(LOCAL_SRC_ROOT)/duktape/bindings/events/dukEvents.cpp \
	$(LOCAL_SRC_ROOT)/duktape/bindings/navigator/dukNavigator.cpp \
	$(LOCAL_SRC_ROOT)/duktape/bindings/websocket/dukWebsocket.cpp \
	$(LOCAL_SRC_ROOT)/duktape/duktape.c \
	$(LOCAL_SRC_ROOT)/duktape/dukUtils.cpp \
	$(LOCAL_SRC_ROOT)/Input/SDLKeyToDOMKey.cpp \
	$(LOCAL_SRC_ROOT)/Logger/Logger.cpp \
	$(LOCAL_SRC_ROOT)/Logger/Logger_android.cpp \
	$(LOCAL_SRC_ROOT)/Navigator/Gamepad.cpp \
	$(LOCAL_SRC_ROOT)/Navigator/Navigator.cpp \
	$(LOCAL_SRC_ROOT)/Navigator/SDLGamepad.cpp \
	$(LOCAL_SRC_ROOT)/Navigator/SDLNavigator.cpp \
	$(LOCAL_SRC_ROOT)/WebSocket/MessageEvent.cpp \
	$(LOCAL_SRC_ROOT)/WebSocket/WebSocket.cpp \
	$(LOCAL_SRC_ROOT)/WebSocket/WebSocketpp.cpp \
	$(LOCAL_SRC_ROOT)/DukJavaScriptEngine.cpp \
	$(LOCAL_SRC_ROOT)/EventPump.cpp \
	$(LOCAL_SRC_ROOT)/hexToByte.cpp \
	$(LOCAL_SRC_ROOT)/JavaScriptEngine.cpp \
	$(LOCAL_SRC_ROOT)/JSValue.cpp \
	$(LOCAL_SRC_ROOT)/main_android.cpp \
	$(LOCAL_SRC_ROOT)/SafeQueue.cpp \
	$(LOCAL_SRC_ROOT)/SDLEventPump.cpp \
	$(LOCAL_SRC_ROOT)/slurpFile.cpp \
	$(LOCAL_SRC_ROOT)/watchFile.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer SDL2_ttf

LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_CPPFLAGS := -std=c++17

LOCAL_LDLIBS += -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
