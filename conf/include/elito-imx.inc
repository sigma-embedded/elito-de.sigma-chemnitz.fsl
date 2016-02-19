XSERVER_DRIVER = "xf86-video-imxfb-vivante"
XSERVER = "\
  xserver-xorg \
  xf86-input-evdev \
  ${XSERVER_DRIVER}"

PREFERRED_PROVIDER_virtual/egl      = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgl    = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles1 = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles2 = "imx-gpu-viv"

PACKAGE_EXTRA_ARCHS_append = " fsl-imx6"

