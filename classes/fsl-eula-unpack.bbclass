LICENSE_FLAGS = "license-freescale"

do_unpack[postfuncs] =+ "fsl_unpack"
fsl_unpack[dirs] = "${S}"
fsl_unpack() {
	sed '0,/^exit 0/d' ${WORKDIR}/${PN}-*.bin | tar xvjf - --strip-components=2
}
