QT += widgets
requires(qtConfig(filedialog))

HEADERS       = window.h \
    conversion.h \
    crc32.h \
    nibblize_4_4.h \
    nibblize_5_3_alt.h \
    nibblize_5_3_common.h \
    nibblize_5_3.h \
    nibblize_6_2.h \
    a2const.h \
    dsktowoz2.h \
    runlog.h
SOURCES       = main.cpp \
                window.cpp \
    crc32.c \
    conversion.cpp \
    dsktowoz2.c \
    nibblize_4_4.c \
    nibblize_5_3_alt.c \
    nibblize_5_3_common.c \
    nibblize_5_3.c \
    nibblize_6_2.c \
    runlog.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/dialogs/findfiles
INSTALLS += target
