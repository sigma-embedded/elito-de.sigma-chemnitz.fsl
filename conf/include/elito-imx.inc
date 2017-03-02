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

# from https://github.com/Freescale/meta-freescale/blob/master/conf/machine/include/imx-base.inc
MACHINEOVERRIDES_EXTENDER_mx6q  = "imxipu:imxvpu:imxgpu2d:imxgpu3d"
MACHINEOVERRIDES_EXTENDER_mx6dl = "imxpxp:imxipu:imxvpu:imxgpu2d:imxgpu3d:imxepdc"
MACHINEOVERRIDES_EXTENDER_mx6sx = "imxpxp:imxgpu2d:imxgpu3d"
MACHINEOVERRIDES_EXTENDER_mx6sl = "imxpxp:imxgpu2d:imxepdc"
MACHINEOVERRIDES_EXTENDER_mx6ul = "imxpxp"
MACHINEOVERRIDES_EXTENDER_mx6ull = "imxpxp:imxepdc"
MACHINEOVERRIDES_EXTENDER_mx7d  = "imxpxp:imxepdc"
