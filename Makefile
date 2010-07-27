#############################################################################
# Makefile for building: kinvoice
# Generated by qmake (1.07a) (Qt 3.3.8) on: Tue Jul 27 21:35:06 2010
# Project:  kinvoice.pro
# Template: app
# Command: $(QMAKE) -o Makefile kinvoice.pro
#############################################################################

####### Compiler, tools and options

CC       = gcc
CXX      = g++
LEX      = flex
YACC     = yacc
CFLAGS   = -pipe -Wall -W -march=x86-64 -mtune=generic -O2 -pipe  -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT
CXXFLAGS = -pipe -Wall -W -march=x86-64 -mtune=generic -O2 -pipe  -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT
LEXFLAGS = 
YACCFLAGS= -d
INCPATH  = -I/opt/qt/mkspecs/default -I. -I$(QTDIR)/include -I.ui/ -I. -I.moc/
LINK     = g++
LFLAGS   = 
LIBS     = $(SUBLIBS) -L$(QTDIR)/lib -L/usr/X11R6/lib64 -lqt-mt -lXext -lX11 -lm
AR       = ar cqs
RANLIB   = 
MOC      = $(QTDIR)/bin/moc
UIC      = $(QTDIR)/bin/uic
QMAKE    = qmake
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY)
COPY_DIR = $(COPY) -r
INSTALL_FILE= $(COPY_FILE)
INSTALL_DIR = $(COPY_DIR)
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

####### Output directory

OBJECTS_DIR = .obj/

####### Files

HEADERS = data.h \
		tableintitem.h
SOURCES = main.cpp \
		data.cpp \
		tableintitem.cpp
OBJECTS = .obj/main.o \
		.obj/data.o \
		.obj/tableintitem.o \
		.obj/form2.o \
		.obj/cat.o \
		.obj/art.o \
		.obj/client.o \
		.obj/qmake_image_collection.o
FORMS = form2.ui \
		cat.ui \
		art.ui \
		client.ui
UICDECLS = .ui/form2.h \
		.ui/cat.h \
		.ui/art.h \
		.ui/client.h
UICIMPLS = .ui/form2.cpp \
		.ui/cat.cpp \
		.ui/art.cpp \
		.ui/client.cpp
SRCMOC   = .moc/moc_form2.cpp \
		.moc/moc_cat.cpp \
		.moc/moc_art.cpp \
		.moc/moc_client.cpp
OBJMOC = .obj/moc_form2.o \
		.obj/moc_cat.o \
		.obj/moc_art.o \
		.obj/moc_client.o
DIST	   = kinvoice.pro
QMAKE_TARGET = kinvoice
DESTDIR  = 
TARGET   = kinvoice

first: all
####### Implicit rules

.SUFFIXES: .c .o .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(UICDECLS) $(OBJECTS) $(OBJMOC)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJMOC) $(OBJCOMP) $(LIBS)

mocables: $(SRCMOC)
uicables: $(UICDECLS) $(UICIMPLS)

$(MOC): 
	( cd $(QTDIR)/src/moc && $(MAKE) )

Makefile: kinvoice.pro  /opt/qt/mkspecs/default/qmake.conf /opt/qt/lib/libqt-mt.prl
	$(QMAKE) -o Makefile kinvoice.pro
qmake: 
	@$(QMAKE) -o Makefile kinvoice.pro

dist: 
	@mkdir -p .obj/kinvoice && $(COPY_FILE) --parents $(SOURCES) $(HEADERS) $(FORMS) $(DIST) .obj/kinvoice/ && $(COPY_FILE) --parents images/logo.png images/button_cancel.png images/button_ok.png images/connect_established.png images/editdelete.png images/edittrash.png images/evolution-contacts.png images/filenew_1.png images/fileprint.png images/filesave_1.png images/folder_new.png images/kuser.png images/mail-config-druid-receive.png images/malehead.png images/meeting-request.png images/myevo-appointments.png images/no.png images/online.png images/print_1.png images/reload.png images/reload_page.png images/run.png images/save.png images/user_icon.png images/viewmag.png images/korganizer.png images/edit.png images/editclear.png .obj/kinvoice/ && $(COPY_FILE) --parents form2.ui.h art.ui.h .obj/kinvoice/ && ( cd `dirname .obj/kinvoice` && $(TAR) kinvoice.tar kinvoice && $(GZIP) kinvoice.tar ) && $(MOVE) `dirname .obj/kinvoice`/kinvoice.tar.gz . && $(DEL_FILE) -r .obj/kinvoice

mocclean:
	-$(DEL_FILE) $(OBJMOC)
	-$(DEL_FILE) $(SRCMOC)

uiclean:
	-$(DEL_FILE) $(UICIMPLS) $(UICDECLS)

yaccclean:
lexclean:
clean: mocclean uiclean
	-$(DEL_FILE) $(OBJECTS)
		-$(DEL_FILE) .ui/qmake_image_collection.cpp
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) $(TARGET)


FORCE:

####### Compile

.obj/main.o: main.cpp .ui/form2.h \
		data.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/main.o main.cpp

.obj/data.o: data.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/data.o data.cpp

.obj/tableintitem.o: tableintitem.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/tableintitem.o tableintitem.cpp

.ui/form2.h: form2.ui data.h \
		tableintitem.h
	$(UIC) form2.ui -o .ui/form2.h

.ui/form2.cpp: .ui/form2.h form2.ui form2.ui.h data.h \
		tableintitem.h
	$(UIC) form2.ui -i form2.h -o .ui/form2.cpp

.ui/cat.h: cat.ui 
	$(UIC) cat.ui -o .ui/cat.h

.ui/cat.cpp: .ui/cat.h cat.ui 
	$(UIC) cat.ui -i cat.h -o .ui/cat.cpp

.ui/art.h: art.ui 
	$(UIC) art.ui -o .ui/art.h

.ui/art.cpp: .ui/art.h art.ui art.ui.h 
	$(UIC) art.ui -i art.h -o .ui/art.cpp

.ui/client.h: client.ui 
	$(UIC) client.ui -o .ui/client.h

.ui/client.cpp: .ui/client.h client.ui 
	$(UIC) client.ui -i client.h -o .ui/client.cpp

.obj/form2.o: .ui/form2.cpp form2.ui.h \
		.ui/form2.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/form2.o .ui/form2.cpp

.obj/cat.o: .ui/cat.cpp .ui/cat.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/cat.o .ui/cat.cpp

.obj/art.o: .ui/art.cpp art.ui.h \
		.ui/art.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/art.o .ui/art.cpp

.obj/client.o: .ui/client.cpp .ui/client.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/client.o .ui/client.cpp

.obj/moc_form2.o: .moc/moc_form2.cpp  .ui/form2.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/moc_form2.o .moc/moc_form2.cpp

.obj/moc_cat.o: .moc/moc_cat.cpp  .ui/cat.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/moc_cat.o .moc/moc_cat.cpp

.obj/moc_art.o: .moc/moc_art.cpp  .ui/art.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/moc_art.o .moc/moc_art.cpp

.obj/moc_client.o: .moc/moc_client.cpp  .ui/client.h 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/moc_client.o .moc/moc_client.cpp

.moc/moc_form2.cpp: $(MOC) .ui/form2.h
	$(MOC) .ui/form2.h -o .moc/moc_form2.cpp

.moc/moc_cat.cpp: $(MOC) .ui/cat.h
	$(MOC) .ui/cat.h -o .moc/moc_cat.cpp

.moc/moc_art.cpp: $(MOC) .ui/art.h
	$(MOC) .ui/art.h -o .moc/moc_art.cpp

.moc/moc_client.cpp: $(MOC) .ui/client.h
	$(MOC) .ui/client.h -o .moc/moc_client.cpp

.obj/qmake_image_collection.o: .ui/qmake_image_collection.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o .obj/qmake_image_collection.o .ui/qmake_image_collection.cpp

.ui/qmake_image_collection.cpp: images/logo.png \
		images/button_cancel.png \
		images/button_ok.png \
		images/connect_established.png \
		images/editdelete.png \
		images/edittrash.png \
		images/evolution-contacts.png \
		images/filenew_1.png \
		images/fileprint.png \
		images/filesave_1.png \
		images/folder_new.png \
		images/kuser.png \
		images/mail-config-druid-receive.png \
		images/malehead.png \
		images/meeting-request.png \
		images/myevo-appointments.png \
		images/no.png \
		images/online.png \
		images/print_1.png \
		images/reload.png \
		images/reload_page.png \
		images/run.png \
		images/save.png \
		images/user_icon.png \
		images/viewmag.png \
		images/korganizer.png \
		images/edit.png \
		images/editclear.png
	$(UIC)  -embed kinvoice images/logo.png images/button_cancel.png images/button_ok.png images/connect_established.png images/editdelete.png images/edittrash.png images/evolution-contacts.png images/filenew_1.png images/fileprint.png images/filesave_1.png images/folder_new.png images/kuser.png images/mail-config-druid-receive.png images/malehead.png images/meeting-request.png images/myevo-appointments.png images/no.png images/online.png images/print_1.png images/reload.png images/reload_page.png images/run.png images/save.png images/user_icon.png images/viewmag.png images/korganizer.png images/edit.png images/editclear.png -o .ui/qmake_image_collection.cpp

####### Install

install:  

uninstall:  

