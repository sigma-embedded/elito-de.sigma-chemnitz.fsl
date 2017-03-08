XSERVER_DRIVER = "xf86-video-imxfb-vivante"
XSERVER = "xserver-xorg ${XSERVER_DRIVER}"

PREFERRED_PROVIDER_virtual/egl      = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgl    = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles1 = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles2 = "imx-gpu-viv"

FSL_MIRROR = "http://www.freescale.com/lgfiles/NMG/MAD/YOCTO"

MACHINE_SOCARCH ??= "${TUNE_PKGARCH}-mx6"
PACKAGE_EXTRA_ARCHS_append = " ${MACHINE_SOCARCH}"

# from https://github.com/Freescale/meta-freescale/blob/master/conf/machine/include/imx-base.inc
MACHINEOVERRIDES_EXTENDER_mx6q  = "imxipu:imxvpu:imxgpu2d:imxgpu3d"
MACHINEOVERRIDES_EXTENDER_mx6dl = "imxpxp:imxipu:imxvpu:imxgpu2d:imxgpu3d:imxepdc"
MACHINEOVERRIDES_EXTENDER_mx6sx = "imxpxp:imxgpu2d:imxgpu3d"
MACHINEOVERRIDES_EXTENDER_mx6sl = "imxpxp:imxgpu2d:imxepdc"
MACHINEOVERRIDES_EXTENDER_mx6ul = "imxpxp"
MACHINEOVERRIDES_EXTENDER_mx6ull = "imxpxp:imxepdc"
MACHINEOVERRIDES_EXTENDER_mx7d  = "imxpxp:imxepdc"

def imx_merge_overrides(order, *sets):
    tmp = set()

    if isinstance(order, str):
        order = order.split(':')

    for s in sets:
        if isinstance(s, str):
            s = s.split(':')

        tmp.update(s)

    return ':'.join(sorted(tmp, key = order.index))

# join Q + DL; processors do not differ very much
# NOTE: order matters!! imxgpu2d must be before imxgpu3d!
MACHINEOVERRIDES_EXTENDER_mx6qdl = "${@\
  imx_merge_overrides('imxpxp:imxipu:imxvpu:imxgpu2d:imxgpu3d:imxepdc', \
                      d.getVar('MACHINEOVERRIDES_EXTENDER_mx6q', False), \
                      d.getVar('MACHINEOVERRIDES_EXTENDER_mx6dl', False))}"
