QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#qtcsv
INCLUDEPATH += D:/libs/qtcsv/include
LIBS += -LD:/libs/qtcsv/lib/release
LIBS += -lqtcsv

#PCL
PCLROOT = D:/libs/PCL
INCLUDEPATH += $${PCLROOT}/include/pcl-1.8 \
               $${PCLROOT}/3rdParty/Boost/include/boost-1_64 \
               $${PCLROOT}/3rdParty/Eigen/eigen3 \
               $${PCLROOT}/3rdParty/FLANN/include \

PCL_LIB = $${PCLROOT}/lib
LIBS += -L$${PCL_LIB}


LIBS += -lpcl_common_release -lpcl_features_release -lpcl_io_release -lpcl_search_release \
        -lpcl_surface_release -lpcl_kdtree_release -lpcl_segmentation_release -lpcl_filters_release

LIBS += $${PCLROOT}/3rdParty/Boost/lib/libboost_thread-vc140-mt-1_64.lib \
        $${PCLROOT}/3rdParty/Boost/lib/libboost_system-vc140-mt-1_64.lib \
        $${PCLROOT}/3rdParty/Boost/lib/libboost_date_time-vc140-mt-1_64.lib \
        $${PCLROOT}/3rdParty/Boost/lib/libboost_chrono-vc140-mt-1_64.lib \
        $${PCLROOT}/3rdParty/Boost/lib/libboost_filesystem-vc140-mt-1_64.lib \
        $${PCLROOT}/3rdParty/Boost/lib/libboost_iostreams-vc140-mt-1_64.lib

