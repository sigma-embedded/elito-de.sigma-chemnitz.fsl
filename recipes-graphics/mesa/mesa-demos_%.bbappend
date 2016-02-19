PACKAGE_ARCH = "fsl-imx6"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append_mx6 = " file://Replace-glWindowPos2iARB-calls-with-glWindowPos2i.patch \
                    file://fix-clear-build-break.patch \
                    file://Additional-eglSwapBuffer-calling-makes-wrong-throttl.patch \
                    file://Add-OpenVG-demos-to-support-wayland.patch"

PACKAGECONFIG_remove = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'x11', '', \
         bb.utils.contains('DISTRO_FEATURES', 'wayland', 'glu', '', d), d)}"

PACKAGECONFIG_append = "\
    ${@bb.utils.contains('DISTRO_FEATURES', 'x11', '', \
       bb.utils.contains('DISTRO_FEATURES', 'wayland', 'wayland vg', '', d), d)} \
"

