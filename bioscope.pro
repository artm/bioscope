TEMPLATE = subdirs
SUBDIRS = lib demo

demo.depends = lib

OTHER_FILES += \
    .gitignore \
    README.md \
    common.pri

