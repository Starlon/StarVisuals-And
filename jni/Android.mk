# Copyright (C) 2012 StarVisuals
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := ./include
LOCAL_SRC_FILES:= tinyalsa/mixer.c tinyalsa/pcm.c
LOCAL_MODULE := libtinyalsa
LOCAL_SHARED_LIBRARIES:= libcutils libutils
LOCAL_PRELINK_MODULE := false
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= libvisual
LOCAL_SRC_FILES := libvisual/lv_utils.c  libvisual/lv_beat.c  libvisual/lv_cache.c  libvisual/lv_cpu.c  libvisual/lv_gl.c  libvisual/lv_libvisual.c  libvisual/lv_mem.c  libvisual/lv_palette.c  libvisual/lv_rectangle.c  libvisual/lv_time.c  libvisual/lv_video.c  libvisual/lv_bin.c  libvisual/lv_collection.c  libvisual/lv_error.c  libvisual/lv_hashlist.c  libvisual/lv_list.c  libvisual/lv_morph.c   libvisual/lv_param.c   libvisual/lv_ringbuffer.c  libvisual/lv_transform.c  libvisual/lv_video_simd.c  libvisual/lv_actor.c  libvisual/lv_bmp.c   libvisual/lv_color.c   libvisual/lv_event.c   libvisual/lv_hashmap.c  libvisual/lv_log.c  libvisual/lv_object.c  libvisual/lv_plugin.c  libvisual/lv_songinfo.c  libvisual/lv_ui.c libvisual/lv_audio.c  libvisual/lv_buffer.c  libvisual/lv_config.c  libvisual/lv_fourier.c  libvisual/lv_input.c  libvisual/lv_math.c  libvisual/lv_os.c  libvisual/lv_random.c  libvisual/lv_thread.c
LOCAL_LDLIBS	:= -Wall
LOCAL_CFLAGS	:= -O0 -g
LOCAL_STATIC_LIBRARIES := android_native_app_glue
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := ./include
LOCAL_MODULE := actor_corona
LOCAL_SRC_FILES := corona/actor_corona.cpp corona/corona.cpp corona/palette.cpp
LOCAL_LDLIBS := -Wall
LOCAL_CFLAGS := -O0 -g
LOCAL_SHARED_LIBRARIES := libvisual
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := ./include
LOCAL_MODULE := actor_lv_scope
LOCAL_SRC_FILES := lv_scope/actor_lv_scope.c
LOCAL_LDLIBS := -Wall
LOCAL_CFLAGS := -O0 -g
LOCAL_SHARED_LIBRARIES := libvisual
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := ./include
LOCAL_MODULE := input_alsa
LOCAL_SRC_FILES := alsa/input_alsa.c
LOCAL_LDLIBS := -Wall
LOCAL_CFLAGS := -O0 -g
LOCAL_SHARED_LIBRARIES := libtinyalsa libvisual
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= evaluator
LOCAL_SRC_FILES	:= evaluator.c
LOCAL_LDLIBS	:= -Wall
LOCAL_CFLAGS	:= -O0 -g
include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := starvisuals
LOCAL_SRC_FILES := starvisuals.c
LOCAL_LDLIBS    := -lm -llog -landroid -Wall
LOCAL_CFLAGS	:= -O0 -g -Wall
LOCAL_STATIC_LIBRARIES := android_native_app_glue evaluator libvisual
include $(BUILD_SHARED_LIBRARY)

# FIXME include jni/libvisual-avs/Android.mk

$(call import-module,android/native_app_glue)
