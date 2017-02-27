require imx-gpu-viv.inc

LIC_FILES_CHKSUM = "file://gpu-core/usr/include/gc_vdk.h;beginline=5;endline=11;md5=12c028cbbbedb4b8770267131500592c"

# FIXME: Drop for GA release
S = "${WORKDIR}/${PN}-${@'${PV}'.replace('5.0.11.p6.3', '5.0.11.p6.3-beta')}"

PACKAGES =+ " libglslc-mx6-dbg \
	libopencl-mx6-dbg \
	libopenvg-mx6-dbg \
	libvdk-mx6-dbg \
	libegl-mx6-dbg \
	libgal-mx6-dbg \
	libvivante-mx6-dbg \
	libvivante-dfb-mx6 \
	libwayland-viv-mx6-dbg \
	libgc-wayland-protocol-mx6-dbg \
	imx-gpu-viv-g2d-dbg \
	imx-gpu-viv-tools-dbg \
	imx-gpu-viv-tools-apitrace-dbg \
	imx-gpu-viv-demos-dbg \
"

USE_X11 = "${@base_contains("DISTRO_FEATURES", "x11", "yes", "no", d)}"
USE_DFB = "${@base_contains("DISTRO_FEATURES", "directfb", "yes", "no", d)}"
USE_WL = "${@base_contains("DISTRO_FEATURES", "wayland", "yes", "no", d)}"

IS_MX6SL = "0"
IS_MX6SL_mx6sl = "1"

do_install () {
    install -d ${D}${libdir}
    install -d ${D}${includedir}
    install -d ${D}${bindir}

    cp -P ${S}/gpu-core/usr/lib/*.so* ${D}${libdir}
    cp  ${S}/g2d/usr/lib/*.so* ${D}${libdir}
    cp -r ${S}/gpu-core/usr/include/* ${D}${includedir}
    cp -Pr ${S}/g2d/usr/include/* ${D}${includedir}
    cp -r ${S}/gpu-demos/opt ${D}
    cp -axr ${S}/gpu-tools/gmem-info/usr/bin/* ${D}${bindir}

    install -d ${D}${libdir}/pkgconfig

    # The preference order, based in DISTRO_FEATURES, is x11, wayland, directfb and fb
    if [ "${USE_X11}" = "yes" ]; then
        cp -r ${S}/gpu-core/usr/lib/dri ${D}${libdir}
        cp -axr ${S}/apitrace/x11/usr/lib/* ${D}${libdir}
        cp -axr ${S}/apitrace/x11/usr/bin/* ${D}${bindir}

        backend=x11

        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/egl_x11.pc ${D}${libdir}/pkgconfig/egl.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/glesv1_cm_x11.pc ${D}${libdir}/pkgconfig/glesv1_cm.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/glesv2_x11.pc ${D}${libdir}/pkgconfig/glesv2.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/vg_x11.pc ${D}${libdir}/pkgconfig/vg.pc
        mv ${D}${libdir}/libGAL_egl.dri.so ${D}${libdir}/libGAL_egl.so
        #FIXME: WL & X11 is not currently functional
        #if [ "${USE_WL}" = "yes" ]; then
        #    install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/gc_wayland_protocol.pc ${D}${libdir}/pkgconfig/gc_wayland_protocol.pc
        #    install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/wayland-egl.pc ${D}${libdir}/pkgconfig/wayland-egl.pc
        #    install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/wayland-viv.pc ${D}${libdir}/pkgconfig/wayland-viv.pc
        #fi
    elif [ "${USE_WL}" = "yes" ]; then
        cp -axr ${S}/apitrace/non-x11/usr/lib/* ${D}${libdir}
        cp -axr ${S}/apitrace/non-x11/usr/bin/* ${D}${bindir}

        backend=wl

        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/egl_wayland.pc ${D}${libdir}/pkgconfig/egl.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/glesv1_cm.pc ${D}${libdir}/pkgconfig/glesv1_cm.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/glesv2.pc ${D}${libdir}/pkgconfig/glesv2.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/vg.pc ${D}${libdir}/pkgconfig/vg.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/gc_wayland_protocol.pc ${D}${libdir}/pkgconfig/gc_wayland_protocol.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/wayland-egl.pc ${D}${libdir}/pkgconfig/wayland-egl.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/wayland-viv.pc ${D}${libdir}/pkgconfig/wayland-viv.pc
        mv ${D}${libdir}/libGAL_egl.${backend}.so ${D}${libdir}/libGAL_egl.so
    else
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/glesv1_cm.pc ${D}${libdir}/pkgconfig/glesv1_cm.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/glesv2.pc ${D}${libdir}/pkgconfig/glesv2.pc
        install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/vg.pc ${D}${libdir}/pkgconfig/vg.pc

        if [ "${USE_DFB}" = "yes" ]; then
            install -d ${D}/${sysconfdir}
            install -m 644 ${S}/gpu-core/etc/directfbrc ${D}/${sysconfdir}/directfbrc
            install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/egl_directfb.pc ${D}${libdir}/pkgconfig/egl.pc

            cp -r ${S}/gpu-core/usr/lib/directfb-* ${D}${libdir}
            cp -axr ${S}/apitrace/non-x11/usr/lib/* ${D}${libdir}
            cp -axr ${S}/apitrace/non-x11/usr/bin/* ${D}${bindir}
            backend=dfb
        else
            # Regular framebuffer
            install -m 0644 ${S}/gpu-core/usr/lib/pkgconfig/egl_linuxfb.pc ${D}${libdir}/pkgconfig/egl.pc
            cp -axr ${S}/apitrace/non-x11/usr/lib/* ${D}${libdir}
            cp -axr ${S}/apitrace/non-x11/usr/bin/* ${D}${bindir}
            backend=fb
        fi
        mv ${D}${libdir}/libGAL_egl.${backend}.so ${D}${libdir}/libGAL_egl.so
    fi

    # Install Vendor ICDs for OpenCL's installable client driver loader (ICDs Loader)
    install -d ${D}${sysconfdir}/OpenCL/vendors/
    install -m 0644 ${S}/gpu-core/etc/Vivante.icd ${D}${sysconfdir}/OpenCL/vendors/Vivante.icd

    # We'll only have one backend here so we rename it to generic name
    # and avoid rework in other packages, when possible
    mv ${D}${libdir}/libGL.so.1.2 ${D}${libdir}/libGL.so.1.2.0
    ln -sf libGL.so.1.2.0 ${D}${libdir}/libGL.so.1.2
    ln -sf libGL.so.1.2.0 ${D}${libdir}/libGL.so.1
    ln -sf libGL.so.1.2.0 ${D}${libdir}/libGL.so
    mv ${D}${libdir}/libEGL-${backend}.so ${D}${libdir}/libEGL.so.1.0
    ln -sf libEGL.so.1.0 ${D}${libdir}/libEGL.so.1
    ln -sf libEGL.so.1.0 ${D}${libdir}/libEGL.so
    mv ${D}${libdir}/libGAL-${backend}.so ${D}${libdir}/libGAL.so
    mv ${D}${libdir}/libVIVANTE-${backend}.so ${D}${libdir}/libVIVANTE.so

    # update libglesv2 as backend dependent
    rm -rf ${D}${libdir}/libGLESv2*
    cp ${S}/gpu-core/usr/lib/libGLESv2-${backend}.so ${D}${libdir}/libGLESv2.so.2.0.0
    ln -sf libGLESv2.so.2.0.0 ${D}${libdir}/libGLESv2.so.2
    ln -sf libGLESv2.so.2.0.0 ${D}${libdir}/libGLESv2.so

    # FIXME: The libGL.so* libraries depend on the X11 libraries and
    # libglapi.so from mesa. For non-X11 distros, neither of them are listed
    # as build dependencies, which can lead to non-deterministic package
    # dependencies. If the intention is that the GL backend should be usable
    # without X11, the linking should be fixed. For now just skip
    # packaging them.
    if [ "${USE_X11}" = "no" ]; then
        rm ${D}${libdir}/libGL.*
    fi

    # skip packaging wayland libraries if no support is requested
    if [ "${USE_WL}" = "no" ]; then
        rm ${D}${libdir}/libgc_wayland_protocol.*
        rm ${D}${libdir}/libwayland-viv.*
    fi

    for backend in wl x11 fb dfb dri; do
        find ${D}${libdir} -name "*-$backend.so" -exec rm '{}' ';'
        find ${D}${libdir} -name "*.$backend.so" -exec rm '{}' ';'
    done

    # FIXME: MX6SL does not have 3D support; hack it for now
    if [ "${IS_MX6SL}" = "1" ]; then
        rm -rf ${D}${libdir}/libCLC* ${D}${includedir}/CL \
               \
               ${D}${libdir}/libGL* ${D}${includedir}/GL* \ 
               \
               ${D}${libdir}/libGLES* \
               \
               ${D}${libdir}/libOpenCL* ${D}${includedir}/CL \
               \
               ${D}${libdir}/libOpenVG.so ${D}${libdir}/libOpenVG.3d.so \
               \
               ${D}${libdir}/libVivanteOpenCL.so \
               \
               ${D}/opt/viv_samples/vdk \
               ${D}/opt/viv_samples/es20 ${D}/opt/viv_samples/cl11

        ln -sf libOpenVG.2d.so ${D}${libdir}/libOpenVG.so
    fi

    find ${D}${libdir} -type f -exec chmod 644 {} \;
    find ${D}${includedir} -type f -exec chmod 644 {} \;

    chown -R root:root "${D}"
}

FILES_libclc-mx6-dbg = "${libdir}/.debug/libCLC${SOLIBS}"
FILES_libegl-mx6-dbg = "${libdir}/.debug/libEGL${SOLIBS}"
FILES_libgal-mx6-dbg = "${libdir}/.debug/libGAL${SOLIBS}"
FILES_libgl-mx6-dbg = "${libdir}/.debug/libGL.${SOLIBS}"
RDEPENDS_libgl-mx6 = "libglapi"
FILES_libgles-mx6-dbg = "${libdir}/.debug/libGLESv1*${SOLIBS} ${libdir}/.debug/libGLES_*${SOLIBS}"
FILES_libgles2-mx6-dbg = "${libdir}/.debug/libGLESv2${SOLIBS}"
FILES_libglslc-mx6-dbg = "${libdir}/.debug/libGLSLC${SOLIBS}"
FILES_libopencl-mx6-dbg = "${libdir}/.debug/libOpenCL${SOLIBS}"
FILES_libopenvg-mx6-dbg = "${libdir}/.debug/libOpenVG*${SOLIBS}"
FILES_libvdk-mx6-dbg = "${libdir}/.debug/libVDK${SOLIBS}"
FILES_libvivante-mx6-dbg = "${libdir}/.debug/libVIVANTE${SOLIBS}"

FILES_libvivante-dfb-mx6 = "${libdir}/directfb-*/gfxdrivers/libdirectfb_gal.so"
FILES_libvivante-dfb-mx6 += "${sysconfdir}/directfbrc"

FILES_libwayland-viv-mx6-dbg = "${libdir}/.debug/libwayland-viv${SOLIBS}"
FILES_libgc-wayland-protocol-mx6-dbg = "${libdir}/.debug/libgc_wayland_protocol${SOLIBS}"

FILES_imx-gpu-viv-g2d-dbg = "${libdir}/.debug/libg2d${SOLIBS}"
FILES_imx-gpu-viv-tools-dbg = "${bindir}/.debug/gmem_info"
FILES_imx-gpu-viv-tools-apitrace-dbg = "${libdir}/*/*/.debug"
FILES_imx-gpu-viv-demos-dbg = "/opt/viv_samples/*/*/.debug"
