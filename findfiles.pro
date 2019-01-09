QT += widgets
requires(qtConfig(filedialog))

HEADERS       = window.h
SOURCES       = main.cpp \
                window.cpp \
    crc32.c

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/dialogs/findfiles
INSTALLS += target
