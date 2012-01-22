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
LOCAL_MODULE	:= evaluator
LOCAL_SRC_FILES	:= evaluator.c
LOCAL_LDLIBS	:= -Wall
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= libvisual
LOCAL_SRC_FILES := lv_utils.c  lv_beat.c  lv_cache.c  lv_cpu.c  lv_gl.c  lv_libvisual.c  lv_mem.c  lv_palette.c  lv_rectangle.c  lv_time.c  lv_video.c  lv_bin.c  lv_collection.c  lv_error.c  lv_hashlist.c  lv_list.c  lv_morph.c   lv_param.c   lv_ringbuffer.c  lv_transform.c  lv_video_simd.c  lv_actor.c  lv_bmp.c   lv_color.c   lv_event.c   lv_hashmap.c  lv_log.c  lv_object.c  lv_plugin.c  lv_songinfo.c  lv_ui.c lv_audio.c  lv_buffer.c  lv_config.c  lv_fourier.c  lv_input.c  lv_math.c  lv_os.c  lv_random.c  lv_thread.c
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

$(call import-module,android/native_app_glue)
