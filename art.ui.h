/****************************************************************************
 ** ui.h extension file, included from the uic-generated form implementation.
 **
 ** If you wish to add, delete or rename functions or slots use
 ** Qt Designer which will update this file, preserving your code. Create an
 ** init() function in place of a constructor, and a destroy() function in
 ** place of a destructor.
 *****************************************************************************/

#include "data.h"
#include <qcombobox.h>
#include <qvalidator.h>

void Art::init() {
  for (uint i = 0; i < Cats.count(); ++i)
    ArtCat->insertItem( Cats.at(i)->name );
    
  ArtPuht->setValidator( new QDoubleValidator(0.0, 9999999.0, 2, ArtPuht) );
  ArtStock->setValidator(new QIntValidator(1, 9999999, ArtStock));
  ArtVendu->setValidator(new QIntValidator(1, 9999999, ArtVendu));
    
}
