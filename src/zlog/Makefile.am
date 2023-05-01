AUTOMAKE_OPTIONS=foreign
lib_LTLIBRARIES=libzlog.la
AM_CPPFLAGS=-I$(top_srcdir)/src/zlog/include/
libzlog_la_SOURCES= \
		buf.c \
		conf.c \
		lockfile.c \
		record.c \
		rule.c \
		zc_arraylist.c \
		zc_util.c \
		category.c \
		event.c \
		level.c \
		record_table.c \
		spec.c \
		zc_hashtable.c \
		zlog.c \
		category_table.c\
		format.c \
		level_list.c \
		mdc.c \
		rotater.c \
		thread.c \
		zc_profile.c \
		zlog-chk-conf.c
EXTRA_DIST	=	$(top_srcdir)/src/zlog/include/*