#
# Makefile for COMSOLFieldView
#
# Pulling COMSOLFieldview into the new build system. It needs to use the
# new library versions of resources such as the scanner and the GeometricObjects
# system. As a wx application it has a significantly more complex Makefile to
# make sure that we meet all the requirements for integration with wx.
#
# This version reworked for the MacPro running Monterey and wx 3.2.1.
# It's a pretty heavy remake becuase this version is the old field viewer before
# I tried to re-organize it.
#
# BCollett 10/2022
#

#
#   Symbols to locate us in the source tree.
#
tool = COMSOLFieldViewer
scheme = debug
bcRoot = /Users/bcollett/Documents/GitHub
toolRoot = $(bcRoot)/$(tool)
srcs = $(toolRoot)/sources
incl = $(toolRoot)/sources
comsrcs = /Users/bcollett/Development/COMSOLWorkspace/COMSOL3DBin/CDSources
libs = 
tests = $(toolRoot)/tests
testsrc = $(tests)/src
d = $(toolRoot)/build/$(scheme)

#
#	Other name building symbols
#
libprefix = lib
libext = .a

#
#	Force Real to double
#
RDefine=-DReal=double
GLDefine=-DUseOpenGL=1
#
# Compiler flags
# Here we may need to find a way to inherit stuff once
# we have a multi-layered make system.
#
incDirs = -I$(incl) -I$(comsrcs) -I$(srcs)/Dialogs -I$(srcs)/Fields -I$(srcs)/Geometry -I$(srcs)/MouseTools -I$(srcs)/Scanner -I$(srcs)
CC = gcc -mmacosx-version-min=12
CXX = g++ -mmacosx-version-min=12
ABUILDER = ar
CFLAGS = -Wall -Wundef -g -O0 -fno-common -fvisibility=hidden -DGL_SILENCE_DEPRECATION=1
CXXFLAGS = -std=c++11 -g -O0 -fno-common -fvisibility=hidden -fvisibility-inlines-hidden -DUseOpenGL=1\
			$(GLDefine) $(incDirs) -DGL_SILENCE_DEPRECATION=1
CPPFLAGS = -D_FILE_OFFSET_BITS=64 -I$(bcRoot)/include
LDFLAGS =   

#
#	wx flags
#
wxCXXFlags = -I/Users/bcollett/Development/wxWidgets-3.2.1/build-cocoa-debug/lib/wx/include/osx_cocoa-unicode-static-3.2 \
	-I/Users/bcollett/Development/wxWidgets-3.2.1/include \
	-D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXMAC__ -D__WXOSX__ -D__WXOSX_COCOA__ 

wxLibs = -L/Users/bcollett/Development/wxWidgets-3.2.1/build-cocoa-debug/lib   \
	-framework IOKit -framework Carbon -framework Cocoa -framework AudioToolbox -framework QuartzCore -framework System \
	-framework OpenGL  -framework AGL -lpthread -liconv -lz \
	-lwx_osx_cocoau_core-3.2  -lwx_baseu-3.2 -lwxpng-3.2 -lwx_osx_cocoau_gl-3.2

#
#	Dependency symbols
#
lib_deps = $(d)/GLViewerCanvas.o $(d)/GLViewerFrame.o $(d)/FieldTexture.o $(d)/FieldView.o \
			$(d)/Listable.o  $(d)/ChoosePlaneDlg.o $(d)/ChoosePZPlane.o \
			$(d)/EField.o $(d)/ColorMapper.o $(d)/CoolWarmMapper.o \
			$(d)/FieldMapper.o  $(d)/LinFieldMapper.o $(d)/LogFieldMapper.o $(d)/RainbowMapper.o \
			$(d)/FieldViewerDoc.o $(d)/GLViewerView.o $(d)/CD3DField.o $(d)/assert.o \
			$(d)/Geometry2D.o $(d)/GeometricObject.o $(d)/Box3D.o $(d)/Cap3D.o $(d)/DisplayList.o $(d)/Ellipsoid3D.o \
			$(d)/Frame3D.o $(d)/FrameRect3D.o $(d)/GLAList.o $(d)/Group3D.o $(d)/Line3D.o $(d)/Point3D.o \
			$(d)/PolyLine3D.o $(d)/Rect3D.o  $(d)/RGBColor.o  $(d)/Triangle3D.o $(d)/Tube3D.o $(d)/Vector3D.o $(d)/Vertex3D.o \
			$(d)/MouseTool.o $(d)/GLMouseTools.o $(d)/trackball.o \
			$(d)/CSymbol.o $(d)/CSymbolTable.o $(d)/CTextScanner.o $(d)/CharClass.o \
			$(d)/COMSOLData.o $(d)/COMSOLData2D.o $(d)/COMSOLData3D.o $(d)/ReadField.o
h_deps = $(incl)/GLViewerCanvas.h $(incl)/GLViewerFrame.h $(incl)/FieldTexture.h $(incl)/FieldView.h \
		 $(incl)/FieldViewerDoc.h $(incl)/GLViewerFrame.h $(incl)/FieldView.h \
		 $(incl)/GLViewerView.h $(incl)/Listable.h $(incl)/Dialogs/ChoosePlaneDlg.h \
		 $(incl)/Dialogs/ChoosePZPlane.h $(incl)/Fields/CD3DField.h \
		 $(incl)/ColorMapper.h $(incl)/CoolWarmMapper.h \
		 $(incl)/FieldMapper.h $(incl)/LinFieldMapper.h $(incl)/LogFieldMapper.h \
		 $(incl)/RainbowMapper.h $(incl)/assert.h \
		 $(incl)/Geometry/Geometry2D.h $(incl)/Geometry/Geometry3d.h $(incl)/Geometry/GeometricObjects.h \
		 $(incl)/MouseTools/MouseTool.h $(incl)/MouseTools/GLMouseTools.h $(incl)/MouseTools/trackball.h \
		 $(incl)/Scanner/CSymbol.h $(incl)/Scanner/CSymbolTable.h $(incl)/Scanner/CTextScanner.h $(incl)/Scanner/Lexemes.h
#
#	First target is default
#
${tests}/FieldViewer : $(srcs)/FieldViewerApp.cpp $(lib_deps) $(h_deps) $(bc_libs)
	$(CXX) -o FieldViewer $(CXXFLAGS) -v $(wxCXXFlags) $(wxLibs) $(lib_deps) $(srcs)/FieldViewerApp.cpp

$(d)/GLViewerCanvas.o : $(srcs)/GLViewerCanvas.cpp $(h_deps)
	$(CXX) -c -o $(d)/GLViewerCanvas.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/GLViewerCanvas.cpp

$(d)/GLViewerFrame.o : $(srcs)/GLViewerFrame.cpp $(h_deps)
	$(CXX) -c -o $(d)/GLViewerFrame.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/GLViewerFrame.cpp

$(d)/GLViewerView.o : $(srcs)/GLViewerView.cpp $(h_deps)
	$(CXX) -c -o $(d)/GLViewerView.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/GLViewerView.cpp

$(d)/NDFieldView.o : $(srcs)/NDFieldView.cpp $(h_deps)
	$(CXX) -c -o $(d)/NDFieldView.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/NDFieldView.cpp

$(d)/FieldView.o : $(srcs)/FieldView.cpp $(h_deps)
	$(CXX) -c -o $(d)/FieldView.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/FieldView.cpp

$(d)/FieldViewerDoc.o : $(srcs)/FieldViewerDoc.cpp $(h_deps)
	$(CXX) -c -o $(d)/FieldViewerDoc.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/FieldViewerDoc.cpp

$(d)/FieldTexture.o : $(srcs)/FieldTexture.cpp $(h_deps)
	$(CXX) -c -o $(d)/FieldTexture.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/FieldTexture.cpp

$(d)/ArrayLine3D.o : $(srcs)/ArrayLine3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/ArrayLine3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/ArrayLine3D.cpp

$(d)/Listable.o : $(srcs)/Listable.cpp $(h_deps)
	$(CXX) -c -o $(d)/Listable.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Listable.cpp

$(d)/EField.o : $(srcs)/Fields/EField.cpp $(h_deps)
	$(CXX) -c -o $(d)/EField.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Fields/EField.cpp

$(d)/CD3DField.o : $(srcs)/Fields/CD3DField.cpp $(h_deps)
	$(CXX) -c -o $(d)/CD3DField.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Fields/CD3DField.cpp

$(d)/ColorMapper.o : $(srcs)/ColorMapper.cpp $(h_deps)
	$(CXX) -c -o $(d)/ColorMapper.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/ColorMapper.cpp

$(d)/CoolWarmMapper.o : $(srcs)/CoolWarmMapper.cpp $(h_deps)
	$(CXX) -c -o $(d)/CoolWarmMapper.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/CoolWarmMapper.cpp

$(d)/FieldMapper.o : $(srcs)/FieldMapper.cpp $(h_deps)
	$(CXX) -c -o $(d)/FieldMapper.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/FieldMapper.cpp

$(d)/LinFieldMapper.o : $(srcs)/LinFieldMapper.cpp $(h_deps)
	$(CXX) -c -o $(d)/LinFieldMapper.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/LinFieldMapper.cpp

$(d)/LogFieldMapper.o : $(srcs)/LogFieldMapper.cpp $(h_deps)
	$(CXX) -c -o $(d)/LogFieldMapper.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/LogFieldMapper.cpp

$(d)/RainbowMapper.o : $(srcs)/RainbowMapper.cpp $(h_deps)
	$(CXX) -c -o $(d)/RainbowMapper.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/RainbowMapper.cpp

$(d)/ChoosePlaneDlg.o : $(srcs)/Dialogs/ChoosePlaneDlg.cpp $(h_deps)
	$(CXX) -c -o $(d)/ChoosePlaneDlg.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Dialogs/ChoosePlaneDlg.cpp

$(d)/ChoosePZPlane.o : $(srcs)/Dialogs/ChoosePZPlane.cpp $(h_deps)
	$(CXX) -c -o $(d)/ChoosePZPlane.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Dialogs/ChoosePZPlane.cpp

$(d)/Box3D.o : $(srcs)/Geometry/Box3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Box3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Box3D.cpp

$(d)/Cap3D.o : $(srcs)/Geometry/Cap3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Cap3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Cap3D.cpp

$(d)/DisplayList.o : $(srcs)/Geometry/DisplayList.cpp $(h_deps)
	$(CXX) -c -o $(d)/DisplayList.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/DisplayList.cpp

$(d)/Ellipsoid3D.o : $(srcs)/Geometry/Ellipsoid3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Ellipsoid3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Ellipsoid3D.cpp

$(d)/Frame3D.o : $(srcs)/Geometry/Frame3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Frame3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Frame3D.cpp

$(d)/FrameRect3D.o : $(srcs)/Geometry/FrameRect3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/FrameRect3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/FrameRect3D.cpp

$(d)/GeometricObject.o : $(srcs)/Geometry/GeometricObject.cpp $(h_deps)
	$(CXX) -c -o $(d)/GeometricObject.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/GeometricObject.cpp

$(d)/Geometry2D.o : $(srcs)/Geometry/Geometry2D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Geometry2D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Geometry2D.cpp

$(d)/GLAList.o : $(srcs)/Geometry/GLAList.cpp $(h_deps)
	$(CXX) -c -o $(d)/GLAList.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/GLAList.cpp

$(d)/Group3D.o : $(srcs)/Geometry/Group3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Group3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Group3D.cpp

$(d)/Line3D.o : $(srcs)/Geometry/Line3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Line3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Line3D.cpp

$(d)/Point3D.o : $(srcs)/Geometry/Point3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Point3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Point3D.cpp

$(d)/PolyLine3D.o : $(srcs)/Geometry/PolyLine3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/PolyLine3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/PolyLine3D.cpp

$(d)/Rect3D.o : $(srcs)/Geometry/Rect3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Rect3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Rect3D.cpp

$(d)/RGBColor.o : $(srcs)/Geometry/RGBColor.cpp $(h_deps)
	$(CXX) -c -o $(d)/RGBColor.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/RGBColor.cpp

$(d)/Triangle3D.o : $(srcs)/Geometry/Triangle3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Triangle3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Triangle3D.cpp

$(d)/Tube3D.o : $(srcs)/Geometry/Tube3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Tube3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Tube3D.cpp

$(d)/Vector3D.o : $(srcs)/Geometry/Vector3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Vector3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Vector3D.cpp

$(d)/Vertex3D.o : $(srcs)/Geometry/Vertex3D.cpp $(h_deps)
	$(CXX) -c -o $(d)/Vertex3D.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Geometry/Vertex3D.cpp

$(d)/GLMouseTools.o : $(srcs)/MouseTools/GLMouseTools.cpp $(h_deps)
	$(CXX) -c -o $(d)/GLMouseTools.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/MouseTools/GLMouseTools.cpp

$(d)/MouseTool.o : $(srcs)/MouseTools/MouseTool.cpp $(h_deps)
	$(CXX) -c -o $(d)/MouseTool.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/MouseTools/MouseTool.cpp

$(d)/trackball.o : $(srcs)/MouseTools/trackball.c $(h_deps)
	$(CC) -c -o $(d)/trackball.o $(CFLAGS) $(srcs)/MouseTools/trackball.c

$(d)/CharClass.o : $(srcs)/Scanner/CharClass.cpp $(h_deps)
	$(CXX) -c -o $(d)/CharClass.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Scanner/CharClass.cpp

$(d)/CSymbol.o : $(srcs)/Scanner/CSymbol.cpp $(h_deps)
	$(CXX) -c -o $(d)/CSymbol.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Scanner/CSymbol.cpp

$(d)/CSymbolTable.o : $(srcs)/Scanner/CSymbolTable.cpp $(h_deps)
	$(CXX) -c -o $(d)/CSymbolTable.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Scanner/CSymbolTable.cpp

$(d)/CTextScanner.o : $(srcs)/Scanner/CTextScanner.cpp $(h_deps)
	$(CXX) -c -o $(d)/CTextScanner.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/Scanner/CTextScanner.cpp

$(d)/assert.o : $(srcs)/assert.cpp $(h_deps)
	$(CXX) -c -o $(d)/assert.o $(CXXFLAGS) $(wxCXXFlags) $(srcs)/assert.cpp

$(d)/COMSOLData.o : $(comsrcs)/COMSOLData.c $(comsrcs)/COMSOLData.h
	$(CC) -c -o $(d)/COMSOLData.o $(CFLAGS) $(comsrcs)/COMSOLData.c

$(d)/COMSOLData2D.o : $(comsrcs)/COMSOLData2D.c $(comsrcs)/COMSOLData.h $(comsrcs)/COMSOLData2D.h
	$(CC) -c -o $(d)/COMSOLData2D.o $(CFLAGS) $(comsrcs)/COMSOLData2D.c

$(d)/COMSOLData3D.o : $(comsrcs)/COMSOLData3D.c $(comsrcs)/COMSOLData.h $(comsrcs)/COMSOLData3D.h
	$(CC) -c -o $(d)/COMSOLData3D.o $(CFLAGS) $(comsrcs)/COMSOLData3D.c

$(d)/ReadField.o : $(comsrcs)/ReadField.c $(comsrcs)/COMSOLData.h $(comsrcs)/COMSOLData3D.h $(comsrcs)/COMSOLData2D.h
	$(CC) -c -o $(d)/ReadField.o $(CFLAGS) $(comsrcs)/ReadField.c

clean :
	rm -rf $(toolRoot)/build/debug/* $(toolRoot)/build/release/* $(libs)/*

tidy : 
	rm -rf $(toolRoot)/build/debug/* $(toolRoot)/build/release/*

tests : ${tests}/FieldViewer

