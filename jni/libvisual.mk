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


include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_bin
LOCAL_SRC_FILES	:= lv_bin.c
LOCAL_LDLIBS	:= -Wall
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_collection
LOCAL_SRC_FILES	:= lv_collection.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_error
LOCAL_SRC_FILES	:= lv_error.c
LOCAL_LDLIBS	:= -Wall
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_hashlist.c
LOCAL_SRC_FILES	:= lv_hashlist.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_list
LOCAL_SRC_FILES	:= lv_list.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_morph
LOCAL_SRC_FILES	:= lv_morph.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_ringbuffer
LOCAL_SRC_FILES	:= lv_ringbuffer.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_transform
LOCAL_SRC_FILES	:= lv_transform.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_video_simd
LOCAL_SRC_FILES	:= lv_video_simd.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_actor
LOCAL_SRC_FILES	:= lv_actor.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_bmp
LOCAL_SRC_FILES	:= lv_bmp.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_color
LOCAL_SRC_FILES	:= lv_color.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_event
LOCAL_SRC_FILES	:= lv_event.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_hashmap
LOCAL_SRC_FILES	:= lv_hashmap.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_log
LOCAL_SRC_FILES	:= lv_log.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_object
LOCAL_SRC_FILES	:= lv_object.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_plugin
LOCAL_SRC_FILES	:= lv_plugin.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_songinfo
LOCAL_SRC_FILES	:= lv_songinfo.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_ui
LOCAL_SRC_FILES	:= lv_ui.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_beat
LOCAL_SRC_FILES	:= lv_beat.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_cache
LOCAL_SRC_FILES	:= lv_cache.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_cpu
LOCAL_SRC_FILES	:= lv_cpu.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_gl
LOCAL_SRC_FILES	:= lv_gl.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_mem
LOCAL_SRC_FILES	:= lv_mem.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_palette
LOCAL_SRC_FILES	:= lv_palette.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_rectangle
LOCAL_SRC_FILES	:= lv_rectangle.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_time
LOCAL_SRC_FILES	:= lv_time.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE	:= lv_video
LOCAL_SRC_FILES	:= lv_video.c
LOCAL_LDLIBS	:= -Wall -Ilibvisual/
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := lv_libvisual
LOCAL_SRC_FILES := lv_libvisual.c
LOCAL_LDLIBS    := -Wall
LOCAL_CFLAGS	:= -O0 -g

include $(BUILD_STATIC_LIBRARY)


