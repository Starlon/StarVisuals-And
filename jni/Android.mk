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
include $(LOCAL_PATH)/libvisual.mk

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
LOCAL_CFLAGS	:= -O0 -g
#LOCAL_STATIC_LIBRARIES += lv_bin     lv_collection  lv_error    lv_hashlist   lv_list  lv_morph    lv_param      lv_ringbuffer  lv_transform  lv_video_simd  lv_actor   lv_bmp     lv_color       lv_event    lv_hashmap    lv_log   lv_object   lv_plugin     lv_songinfo    lv_ui         plasma lv_audio   lv_buffer  lv_config      lv_fourier  lv_input      lv_math  lv_os       lv_random     lv_thread      lv_utils      lv_beat    lv_cache   lv_cpu         lv_gl       lv_libvisual  lv_mem   lv_palette  lv_rectangle  lv_time        lv_video lv_libvisual
LOCAL_STATIC_LIBRARIES += android_native_app_glue evaluator

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
