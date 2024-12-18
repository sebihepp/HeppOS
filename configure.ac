
AC_INIT([HeppOS], [0.0.1], [sebihepp@googlemail.com])
AM_INIT_AUTOMAKE([-Wall foreign])

#AC_CONFIG_HEADERS([
#	config.h
#])


AC_CONFIG_LINKS([
	qemu.sh:qemu.sh
	qemu_debug.sh:qemu_debug.sh
	qemu_uefi.sh:qemu_uefi.sh
	qemu_uefi_debug.sh:qemu_uefi_debug.sh
])

AC_CONFIG_FILES([
	Makefile
])

AC_CONFIG_SUBDIRS([
	kernel
])

AC_OUTPUT
