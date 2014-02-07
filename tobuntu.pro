TEMPLATE = subdirs
SUBDIRS += tobuntu-files

contains(MEEGO_EDITION,harmattan) {
    SUBDIRS += qubuntuone qubuntuone-declarative
    tobuntu-files.depends += qubuntuone
    qubuntuone-declarative.depends += qubuntuone

    OTHER_FILES += \
        qtc_packaging/debian_harmattan/rules \
        qtc_packaging/debian_harmattan/README \
        qtc_packaging/debian_harmattan/manifest.aegis \
        qtc_packaging/debian_harmattan/copyright \
        qtc_packaging/debian_harmattan/control \
        qtc_packaging/debian_harmattan/compat \
        qtc_packaging/debian_harmattan/changelog
}
