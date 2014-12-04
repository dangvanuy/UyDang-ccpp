ifndef RUA_DIR
$(error Undefined RUA environment variable. Set RUA_DIR to the root RUA source directory)
endif

ifndef RMCFLAGS
$(error Undefined RUA compilation flags. Use rmcflags to set them up prior to making these project)
endif

ifeq '$(wildcard $(DCCHD_DIR))' ''
$(error DCCHD_DIR must be defined and point to an existing DCCHD repository)
endif

ifeq '$(wildcard $(DIRECTFB_DIR))' ''
$(error DIRECTFB_DIR must be defined and point to an existing DirectFB repository)
endif

ifdef DIRECTFB_INSTALL_DIR
	        DFB_DIR = $(DIRECTFB_INSTALL_DIR)
else
	        DFB_DIR = $(DIRECTFB_DIR)/directfb
endif


USE_STD_LIB := 1

TARGET_TYPE = EXECUTABLE
include $(RUA_DIR)/scripts/inc.Target

LINKER = g++

COMPILKIND += withdl

LDFLAGS += \
	-Wl,-rpath-link -Wl,$(RUA_DIR)/lib		\
	-Wl,-rpath-link -Wl,$(DFB_DIR)/lib		\
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/core 		\
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dcchd 		\
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/mono		\
    -Wl,-rpath-link -Wl,$(DCCHD_DIR)/dtv		\
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dtv/capture 	\
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dtv/network	\
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dtv/acap	\
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dtv/tuner	\
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dtv/hal	\
	-Wl,-rpath-link -Wl,$(DCCHD_INSTALL_DIR)/lib


ifdef VERIMATRIX_PATH
LDFLAGS += -Wl,-rpath-link -Wl,$(VERIMATRIX_PATH)/libs
endif

LDFLAGS += -Wl,-rpath-link -Wl,$(SMP86XX_ROOTFS_PATH)/cross_rootfs/lib
LDFLAGS += -lcurl -ldl

RMCFLAGS += \
	-D_FILE_OFFSET_BITS=64 \
	-DLLAD_DIRECT \
	-DALLOW_OS_CODE	\
	-D_REENTRANT \
	-I. -I./include \
	-I./remote


RMCFLAGS += -DXMD_H=1
RMCFLAGS += -I$(DCCHD_DIR) -I$(DCCHD_DIR)/dcchd -I$(DCCHD_DIR)/core -I$(DCCHD_DIR)/mono -I$(RUA_DIR) -I$(RUA_DIR)/samples
RMCFLAGS += -I$(DFB_DIR)/include -I$(DFB_DIR)/include/freetype2 -I$(DFB_DIR)/include/directfb
RMCFLAGS += -I$(DFB_DIR)/include/directfb/direct  -I$(DFB_DIR)/include/directfb-internal
RMCFLAGS += -I$(DCCHD_DIR)/dtv -I$(DCCHD_DIR)/dtv/tuner/src -I$(DCCHD_DIR)/dtv/hal -I$(DCCHD_DIR)/mono -I$(DCCHD_DIR)/../test -I$(DCCHD_DIR)/dtv/network
RMCFLAGS += -I$(DFB_DIR)/lib

#QT
#LDFLAGS += -Wl,-rpath-link -Wl,$(QT_INSTALL_DIR)/lib
#JAVASCRIPTCORE_PATH=$(QT_DIR)/src/3rdparty/webkit/Source/JavaScriptCore
#RMCFLAGS += -I$(QT_INSTALL_DIR)/include -I$(QT_INSTALL_DIR)/include/Qt 	-I$(QT_INSTALL_DIR)/include/QtCore -I$(CORE_DIR) -I$(CORE_DIR)/include -I./include/npapi
#RMCFLAGS += -I$(PLAYER_INC_DIR) -I$(PLUGIN_INC_DIR) -I$(QT_DIR)/src/3rdparty/webkit/Source/WebCore/bridge -I$(QT_DIR)/src/3rdparty/webkit/WebCore/bridge
#RMCFLAGS += -I$(JAVASCRIPTCORE_PATH) -I$(JAVASCRIPTCORE_PATH)/.. -I$(JAVASCRIPTCORE_PATH)/assembler -I$(JAVASCRIPTCORE_PATH)/bytecode
#RMCFLAGS += -I$(JAVASCRIPTCORE_PATH)/bytecompiler -I$(JAVASCRIPTCORE_PATH)/debugger -I$(JAVASCRIPTCORE_PATH)/interpreter -I$(JAVASCRIPTCORE_PATH)/jit
#RMCFLAGS += -I$(JAVASCRIPTCORE_PATH)/parser -I$(JAVASCRIPTCORE_PATH)/pcre -I$(JAVASCRIPTCORE_PATH)/profiler -I$(JAVASCRIPTCORE_PATH)/runtime
#RMCFLAGS += -I$(JAVASCRIPTCORE_PATH)/wtf -I$(JAVASCRIPTCORE_PATH)/wtf/symbian -I$(JAVASCRIPTCORE_PATH)/wtf/unicode -I$(JAVASCRIPTCORE_PATH)/yarr
#RMCFLAGS += -I$(JAVASCRIPTCORE_PATH)/API -I$(JAVASCRIPTCORE_PATH)/ForwardingHeaders -I$(JAVASCRIPTCORE_PATH)/generated
#end QT

ifdef LATENS_PATH
RMCFLAGS += \
	-I$(LATENS_PATH)/include \
	-DLATENS_ENABLED=1
endif

MAIN_SRC = MainApp.cpp

SRC =	\
	Threads.cpp	\
	Engineer.cpp	\
	./remote/remote.c



EXTERNAL_STATIC_LINKS_WITH += 				\
        $(DCCHD_DIR)/dtv/libdcchd_dtv.so                \
        $(DCCHD_DIR)/dtv/capture/libdcchd_cap.so        \
        $(DCCHD_DIR)/dtv/acap/libdcchd_acap.so          \
        $(DCCHD_DIR)/dtv/network/libdcchd_mcast.so      \
        $(DCCHD_DIR)/dcchd/libdcchd.so                  \
		$(DCCHD_INSTALL_DIR)/lib/libdirect.so 			\
		$(DCCHD_INSTALL_DIR)/lib/libdirectfb.so 			\
		$(DCCHD_INSTALL_DIR)/lib/libfusion.so 			\
		$(RUA_DIR)/lib/librmcw.so 			\
		$(RUA_DIR)/lib/librmcore.so 			\
		$(RUA_DIR)/lib/librmpthreadw.so

include $(RUA_DIR)/scripts/inc.Makefile
