/* kinvoice v0.3 - */
/* main.cpp */

#include <qapplication.h>
#include <qstring.h>
#include "form2.h"
#include "data.h"

QPtrList<hClient> Clients;
QPtrList<hClient>* displayClients;
QPtrList<hFacture> Factures;
QPtrList<hCat> Cats;
hFacture Fact;
QString dataDir = QDir::home().path() + "/.kinvoice";
QString dataFile;
QString dataFileDefault = dataDir +  "/kinvoice_1";
QApplication* qApp;

int main( int argc, char ** argv ) {
  QApplication a( argc, argv );
  qApp = &a;
  
  /* gestion du fichier de travail */
  QDir dir = QDir( dataDir );
  if (! dir.exists())
    dir.mkdir( dataDir );
  
  /* gestion des arguments */
  if (argv[1]) {
    dataFile = QString(argv[1]);
  } else {
    dataFile = dataFileDefault;
  }
  
  /* affichage de la fenêtre */
  Form2 w;
  w.show();
  a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
  return a.exec();

}
