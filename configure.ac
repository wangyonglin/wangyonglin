#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
## build settings
m4_define(VERSION_MAJOR,1)
m4_define(VERSION_MINOR,3)
m4_define(VERSION_PATCH,m4_esyscmd([git rev-list --count HEAD | tr -d '\n']))
AC_INIT([tiger], [VERSION_MAJOR.VERSION_MINOR.VERSION_PATCH],[admin@wangyonglin.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([user/user_main.c])
AC_CONFIG_HEADERS([config.h])
LIBTOOL
LT_INIT
# Checks for programs.
AC_PROG_CC




# Checks for libraries.
# FIXME: Replace `main' with a function in `-lcrypto':
AC_CHECK_LIB([crypto], [main])
# FIXME: Replace `main' with a function in `-lcurl':
AC_CHECK_LIB([curl], [main])
# FIXME: Replace `main' with a function in `-levent':
AC_CHECK_LIB([event], [main])
# FIXME: Replace `main' with a function in `-levent_openssl':
AC_CHECK_LIB([event_openssl], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lssl':
AC_CHECK_LIB([ssl], [main])

# Checks for header files.
AC_CHECK_HEADERS([float.h limits.h locale.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
#AC_FUNC_STRNLEN
AC_FUNC_STRTOD
AC_CHECK_FUNCS([bzero getpass localeconv memset strcasecmp strchr strdup strerror strrchr strspn strstr])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/toml/Makefile])
AC_CONFIG_FILES([src/mbedtls/Makefile])
AC_CONFIG_FILES([src/unix/Makefile])
AC_CONFIG_FILES([src/wangyonglin/Makefile])
AC_CONFIG_FILES([src/paho_mqtt_c/Makefile])
AC_CONFIG_FILES([src/cjson/Makefile])
AC_CONFIG_FILES([user/Makefile])
AC_OUTPUT
