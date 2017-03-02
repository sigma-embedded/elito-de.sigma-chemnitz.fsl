inherit image_types

_IMAGE_DEPENDS-rescue = "\
  virtual/elito-rescue-kernel:do_deploy \
"

_IMAGE_DEPENDS = "\
  parted-native:do_populate_sysroot \
  dosfstools-native:do_populate_sysroot \
  mtools-native:do_populate_sysroot \
  virtual/kernel:do_deploy \
  virtual/bootloader:do_deploy \
  ${@bb.utils.contains('PROJECT_FEATURES', 'rescuekernel', \
                       '${_IMAGE_DEPENDS-rescue}', '', d)} \
"

_SDCARD_GENERATION_COMMAND_mx6 = "generate_imx_sdcard"

# Default type of rootfs filesystem.
SDCARD_ROOTFS_TYPE ?= "ext4"

# Location of root filesystem which is written to the sdcard.
SDCARD_ROOTFS = "${IMGDEPLOYDIR}/${IMAGE_BASENAME}-${MACHINE}.${SDCARD_ROOTFS_TYPE}"

# Boot partition volume id
BOOTDD_VOLUME_ID ?= "Boot %(mach)s (%(mem)s)"

# Boot partition size [in KiB]
BOOT_SPACE ?= "32768"

BOOT_START ?= "1024"
BOOT_START[doc] = "Start of boot partion (in KiB)"

# Barebox environment size [in KiB]
BAREBOX_ENV_SPACE ?= "512"

# Set alignment to 4MB [in KiB]
IMAGE_ROOTFS_ALIGNMENT = "4096"

FSL_SDCARD = "${@get_fsl_sdcard(d)}"

VFAT_ALIGN ?= "63"

_sdcard_dd() {
    dd_if=$1
    shift
    dd if="$dd_if" of="$SDCARD" conv=notrunc "$@"
}

generate_imx_sdcard () {
    SDCARD="${IMGDEPLOYDIR}"/$1
    BOOTLOADER_BASE="${DEPLOY_DIR_IMAGE}/"$2
    VOLID=$3
    KERNEL_IMG="${DEPLOY_DIR_IMAGE}"/$4
    DTREE="${DEPLOY_DIR_IMAGE}"/$5
    RESCUE=$6

    # Align boot partition and calculate total SD card image size
    align=$(expr ${IMAGE_ROOTFS_ALIGNMENT})
    boot_start=$(expr ${BOOT_START})
    boot_space=$(expr ${BOOT_SPACE})
    vfat_align=$(expr ${VFAT_ALIGN})

    p0_s=$boot_start
    p0_e=$(expr $p0_s + $boot_space + $align - 1)
    p0_e=$(expr $p0_e - $p0_e % $align)
    p1_s=$p0_e
    p1_e=$(expr $p1_s + $ROOTFS_SIZE)

    SDCARD_SIZE=$(expr $p1_e + $align)


    dd if=/dev/zero of="$SDCARD" bs=1024 count=0 seek=$SDCARD_SIZE

    # Create partition table
    parted -s "$SDCARD" mklabel msdos
    parted -s "$SDCARD" -a none unit KiB mkpart primary fat32 "$p0_s" "$p0_e"
    parted -s "$SDCARD" -a none unit KiB mkpart primary       "$p1_s" "$p1_e"
    parted "$SDCARD" unit KiB print

    # Create boot partition image
    BOOT_BLOCKS=$(LC_ALL=C parted -s "$SDCARD" unit b print \
                      | awk '/ 1 / { print substr($4, 1, length($4 - 1)) / 1024 }')
    BOOT_BLOCKS=$(expr $BOOT_BLOCKS - $BOOT_BLOCKS % $vfat_align)

    t=`mktemp ${B}/boot.img.XXXXXX`
    rm -f "$t"			# mkfs.vfat fails with existing files
    mkfs.vfat -n "$VOLID" -S 512 -C $t $BOOT_BLOCKS
    mcopy -i $t -s "$KERNEL_IMG" ::"${KERNEL_IMAGETYPE}"
    mcopy -i $t -s "$DTREE"      ::oftree

    test -z "$RESCUE" || \
	mcopy -i $t -s "${DEPLOY_DIR_IMAGE}/$RESCUE" ::rescue

    _sdcard_dd "$BOOTLOADER_BASE".img    seek=1 bs=512 skip=1
    _sdcard_dd "$t"               bs=1024 seek="$p0_s"
    _sdcard_dd "${SDCARD_ROOTFS}" bs=1024 seek="$p1_s"
    rm -f "$t"			# mkfs.vfat fails with existing files
}

def get_fsl_sdcard(d):
    variants = map(lambda x: x.split(':'),
                   oe.data.typed_value('MACHINE_VARIANTS', d))
    images = []
    for v in variants:
        imgid = "sdcard-%s-%s" % (v[0], v[2])
        images.append(imgid)

    define_image_vars(d)

    return ' '.join(images)

def define_image_vars(d):
    import pipes

    variants = map(lambda x: x.split(':'),
                   oe.data.typed_value('MACHINE_VARIANTS', d))

    for v in variants:
        imgid = "sdcard-%s-%s" % (v[0], v[2])
        args=[
            "${IMAGE_NAME}${IMAGE_NAME_SUFFIX}.%s" % (imgid,),
            "barebox-%s-%s" % (v[0], v[2]),
            d.getVar("BOOTDD_VOLUME_ID", True) % {
                'mach' : v[0],
                'cpu'  : v[1],
                'mem'  : v[2]
            },
            "${KERNEL_IMAGETYPE}-${MACHINE}.bin",
            "%s.dtb" % (v[0])
        ]

        if bb.utils.contains('PROJECT_FEATURES', 'rescuekernel', True, False, d):
            args.append("${KERNEL_IMAGETYPE}-rescue-${MACHINE}.bin")

        d.setVar("IMAGE_TYPEDEP_%s" % imgid, "${SDCARD_ROOTFS_TYPE}")
        d.setVar("IMAGE_DEPENDS_%s" % imgid, "${_IMAGE_DEPENDS}")
        d.setVar("IMAGE_CMD_%s" % imgid,
                 "{\n  generate_imx_sdcard %s\n}" %
                 (' '.join(map(lambda x: pipes.quote(x), args))))
        #d.setVarFlag("IMAGE_CMD_%s" % imgid, "func", 1)

python () {
    define_image_vars(d)

}
