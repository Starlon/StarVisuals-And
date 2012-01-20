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
LOCAL_CFLAGS	:= -g

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := native-plasma
LOCAL_SRC_FILES := plasma.c
LOCAL_LDLIBS    := -lm -llog -landroid
LOCAL_CFLAGS	:= -g
LOCAL_STATIC_LIBRARIES := android_native_app_glue evaluator

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)