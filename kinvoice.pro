TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS	+= data.h \
	tableintitem.h

SOURCES	+= main.cpp \
	data.cpp \
	tableintitem.cpp

FORMS	= form2.ui \
	cat.ui \
	art.ui \
	client.ui

IMAGES	= images/logo.png \
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

CODECFORTR      = UTF-8
CODECFORSRC     = UTF-8






unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
