ifeq ($(TARGET_CPU), $(filter $(TARGET_CPU), x86_64 A72))

include $(PRELUDE)

TARGET      := openvx_app_1

CSOURCES    := openvx_app.c

ifeq ($(TARGET_CPU),x86_64)

TARGETTYPE  := exe

CSOURCES    += main_x86.c

include $(VISION_APPS_PATH)/apps/concerto_x86_64_inc.mak

STATIC_LIBS += $(IMAGING_LIBS)

endif

ifeq ($(TARGET_CPU),A72)
ifeq ($(TARGET_OS),LINUX)

TARGETTYPE  := exe

CSOURCES    += main_linux_arm.c

include $(VISION_APPS_PATH)/apps/concerto_a72_inc.mak

STATIC_LIBS += $(IMAGING_LIBS)

endif
endif

ifeq ($(TARGET_CPU),A72)
ifeq ($(TARGET_OS),SYSBIOS)

TARGETTYPE  := library

include $(VISION_APPS_PATH)/apps/concerto_a72_inc.mak

endif
endif

IDIRS += $(IMAGING_IDIRS)



include $(FINALE)

endif
