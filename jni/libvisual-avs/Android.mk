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
LOCAL_C_INCLUDES := libvisual-avs/common/
LOCAL_MODULE := common
LOCAL_SRC_FILES := libvisual-avs/common/avs_blend.c   libvisual-avs/common/avs_globals.c libvisual-avs/common/avs_config.c  libvisual-avs/common/avs_matrix.c libvisual-avs/common/avs_gfx.c     libvisual-avs/common/avs_sound.c
LOCAL_LDLIBS := -Wall
LOCAL_CFLAGS := -O0 -g
LOCAL_STATIC_LIBRARIES := libvisual
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := jni/libvisual-avs/common/ jni/libxml2/include/
LOCAL_MODULE := actor_AVS
LOCAL_SRC_FILES := libvisual-avs/src/actor_AVS.c  libvisual-avs/src/avs_serialize.c   libvisual-avs/src/lvavs_preset.c libvisual-avs/src/avs_parse.c  libvisual-avs/src/lvavs_pipeline.c  libvisual-avs/src/main.c
LOCAL_LDLIBS := -Wall
LOCAL_CFLAGS := -O0 -g
LOCAL_SHARED_LIBRARIES := libvisual libxml2
LOCAL_STATIC_LIBRARIES := common
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := jni/libvisual-avs/common/ jni/libvisual-avs/src/ jni/
LOCAL_MODULE := actor_avs_superscope
LOCAL_SRC_FILES := libvisual-avs/plugins/actor/superscope/actor_avs_superscope.c
LOCAL_LDLIBS := -Wall
LOCAL_CFLAGS := -O0 -g
LOCAL_STATIC_LIBRARIES := evaluator libvisual common
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_C_INCLUDES := jni/libvisual-avs/common/ jni/libvisual-avs/src/ jni/
LOCAL_MODULE := actor_avs_stars
LOCAL_SRC_FILES := libvisual-avs/plugins/actor/stars/actor_avs_stars.c
LOCAL_LDLIBS := -Wall
LOCAL_CFLAGS := -O0 -g
LOCAL_STATIC_LIBRARIES := libvisual common
include $(BUILD_SHARED_LIBRARY)

