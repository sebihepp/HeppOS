
AC_INIT([HeppOS/kernel], [0.0.1], [sebihepp@googlemail.com])
AM_INIT_AUTOMAKE([-Wall foreign])


#AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AM_PROG_AS


# Check for installed cross-compiler
#AC_CHECK_PROG(CUSTOM_CC,x86_64-elf-gcc,yes)
#AS_VAR_IF([CUSTOM_CC], [yes], [], [AC_MSG_ERROR([x86_64-elf-gcc not found.])])

AC_CHECK_PROG(CUSTOM_CXX,x86_64-elf-g++,yes)
AS_VAR_IF([CUSTOM_CXX], [yes], [], [AC_MSG_ERROR([x86_64-elf-g++ not found.])])

#AC_CHECK_PROG(CUSTOM_CCAS,x86_64-elf-as,yes)
#AS_VAR_IF([CUSTOM_CCAS], [yes], [], [AC_MSG_ERROR([x86_64-elf-as not found.])])

#AC_CHECK_PROG(CUSTOM_LD,x86_64-elf-ld,yes)
#AS_VAR_IF([CUSTOM_LD], [yes], [], [AC_MSG_ERROR([x86_64-elf-ld not found.])])
####


# Check for installed limine
AC_CHECK_PROG(CUSTOM_LIMINE,limine,yes)
AS_VAR_IF([CUSTOM_LIMINE], [yes], [], [AC_MSG_ERROR([limine not found.])])

AC_CHECK_FILE(/usr/local/include/limine.h, [], [AC_MSG_ERROR([limine.h not found.])])
####


AC_CONFIG_FILES([
	Makefile
])

AC_OUTPUT
