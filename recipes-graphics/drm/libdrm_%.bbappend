FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append_mx6 = " file://drm-update-arm.patch"

PACKAGE_ARCH = "fsl-imx6"
