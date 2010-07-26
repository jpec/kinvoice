
#include "cat.h"
#include "art.h"
#include "client.h"
#include <stdio.h>
#include <stdlib.h>

// Version autoentrepreneur - Pas de TVA

// external global variables
extern QPtrList<hCat> Cats;
extern int currentCat;
extern hFacture Fact;
extern QPtrList<hClient> Clients;
extern QPtrList<hClient>* displayClients;
extern QPtrList<hFacture> Factures;
extern QString dataFile;
extern QString dataFileDefault;
extern QApplication* qApp;




// dans Form
void Form2::init()
{
  factureDate->setDate( QDate::currentDate() );
  clientSearchFields->setSelected(0, TRUE);
    
  QFileInfo file_info( dataFile );
  if ( file_info.size() > 0)
    if (!loadFromFile( dataFile )) {
      dataFile = dataFileDefault;
      loadFromFile( dataFile );
    }
    
    
  displayClients = &Clients;
    
  // set validator
  puht->setValidator( new QDoubleValidator(0.0, 9999999.0, 2, puht) );
  quantite->setValidator( new QIntValidator(1, 9999999, quantite) );
  tvaRate->setValidator( new QDoubleValidator(0.0, 99.99, 2, tvaRate));
    
    
  // tab Facture
  autoFactureRef_clicked();
  tvaRate->setText("0.00");
    
  // tab Archives
  facturesDisplay();

  // tab Reporting
  QDate today = QDate::currentDate();
  uint Year = today.year();
  bDate->setText(QString::number(Year));
  // Calcul des cumuls
  getReporting();

}




bool Form2::saveAll()
{
  if (! dataFile.isEmpty()) {
    QFile file( dataFile );
    if ( file.open( IO_WriteOnly ) ) {
      QDataStream stream( &file );
      uint i,j;
	    
      stream << QString("KINVOICE") << (uint)1;
	    
      QPixmap *pix = userLogo->pixmap();
      stream << *pix;
	    
      stream << userNom->text();
	    
      stream << Cats.count();
      for (i = 0; i < Cats.count(); ++i) {
	stream << Cats.at(i)->name << Cats.at(i)->desc << Cats.at(i)->articles.count();
	for (j = 0; j < Cats.at(i)->articles.count(); ++j) {
	  stream << Cats.at(i)->articles.at(j)->ref
		 << Cats.at(i)->articles.at(j)->desc
		 << Cats.at(i)->articles.at(j)->puht
		 << Cats.at(i)->articles.at(j)->stock
		 << Cats.at(i)->articles.at(j)->vendu;
	}
      }
	    
      stream << Clients.count();
      for (i = 0; i < Clients.count(); ++i) {
	for (j = 0; j < hClient::MAX_INFO; ++j)
	  stream << Clients.at(i)->info[j];
      }
	    
      stream << Factures.count();
      for (i = 0; i < Factures.count(); ++i) {
	stream << Factures.at(i)->date
	       << Factures.at(i)->ref
	       << Factures.at(i)->intitule
	       << Factures.at(i)->client
	       << Factures.at(i)->paiement
	       << Factures.at(i)->comment
	       << Factures.at(i)->sumHT
	       << Factures.at(i)->tva
	       << Factures.at(i)->sumTVA
	       << Factures.at(i)->sumTTC
	       << Factures.at(i)->achats.count();
	for (j = 0; j < Factures.at(i)->achats.count(); ++j) {
	  stream << Factures.at(i)->achats.at(j)->ref
		 << Factures.at(i)->achats.at(j)->desc
		 << Factures.at(i)->achats.at(j)->quantite
		 << Factures.at(i)->achats.at(j)->puht
		 << Factures.at(i)->achats.at(j)->pht
		 << Factures.at(i)->achats.at(j)->remise;
	}
      }
      file.close();
      return true;
    }
  }
  return false;
}


bool Form2::loadFromFile( QString & s)
{
  if (! s.isEmpty()) {
    QFile file( s );
	
    if ( file.open( IO_ReadOnly ) ) {
      QDataStream stream( &file );
      uint i,j,l,m;
      QPixmap pix;
      QString str;
	    
	    
      QString appName;
      uint appVersion;
	    
      stream >> appName;
      stream >> appVersion;
	    
      if (appName != "KINVOICE") {
	QMessageBox::information(this, "Kinvoice", 
				 tr("ce n'est pas un fichier Kinvoice version 1"));
	return false;
      }
	    
      stream >> pix;
      userLogo->setPixmap( pix );
	    
      stream >> str;
      userNom->setText(str);
	    
      Cats.clear();
      stream >> l;
      for (i = 0; i < l; ++i) {
	hCat* cat = new hCat();
	Cats.append(cat);
	stream >> Cats.at(i)->name >> Cats.at(i)->desc >> m;
		
	for (j = 0; j < m; ++j) {
	  hArt* art = new hArt();
	  Cats.at(i)->articles.append(art );
	  stream >> Cats.at(i)->articles.at(j)->ref
		 >> Cats.at(i)->articles.at(j)->desc
		 >> Cats.at(i)->articles.at(j)->puht
		 >> Cats.at(i)->articles.at(j)->stock
		 >> Cats.at(i)->articles.at(j)->vendu;
	}
      }
      displayCats();
	    
	    
      Clients.clear();
      stream >> l;
      for (i = 0; i < l; ++i) {
	hClient* client = new hClient();
	Clients.append( client );
	for (j = 0; j < hClient::MAX_INFO; ++j)
	  stream >> Clients.at(i)->info[j];
      }
      displayClient( Clients );
	    
	    
	    
      Factures.clear();
      stream >> l;
      for (i = 0; i < l; ++i) {
	Factures.append( new hFacture );
	stream >> Factures.at(i)->date
	       >> Factures.at(i)->ref
	       >> Factures.at(i)->intitule
	       >> Factures.at(i)->client
	       >> Factures.at(i)->paiement
	       >> Factures.at(i)->comment
	       >> Factures.at(i)->sumHT
	       >> Factures.at(i)->tva
	       >> Factures.at(i)->sumTVA
	       >> Factures.at(i)->sumTTC
			
	       >> m;
		
	for (j = 0; j < m; ++j) {
	  hAchat* achat = new hAchat();
	  Factures.at(i)->achats.append(achat );
		    
	  stream >> Factures.at(i)->achats.at(j)->ref
		 >> Factures.at(i)->achats.at(j)->desc
		 >> Factures.at(i)->achats.at(j)->quantite
		 >> Factures.at(i)->achats.at(j)->puht
		 >> Factures.at(i)->achats.at(j)->pht
		 >> Factures.at(i)->achats.at(j)->remise;
	}
      }
      facturesDisplay();
	    
      file.close();
      return true;
    }
  }
  return false;
}

void Form2::closeEvent(QCloseEvent* e)
{
  QString msg = QString(tr("Vous désirez sauvegarder vos données avant de quitter"));
  switch (QMessageBox::warning(this, "Kinvoice", msg, 
			       QMessageBox::Yes, QMessageBox::Cancel,QMessageBox::No)) {
  case QMessageBox::Yes:
    if (saveAll())
      e->accept();
    else
      e->ignore();
    break;
  case QMessageBox::Cancel:
    e->ignore();
    break;
  case QMessageBox::No:
    e->accept();
    break; 
  }
}










// menu bar
//
//

void Form2::fileNew()
{
  /*
    dataFile = "";
    
    userLogo->setPixmap( QPixmap::fromMimeSource( "logo.png" ) );
    
    Cats.clear();
    while (tCat->numRows() > 0) tCat->removeRow(0);
    while (tArt->numRows() > 0) tArt->removeRow(0);
    
    Clients.clear();
    while (tClient->numRows() > 0) tClient->removeRow(0);
    
    Factures.clear();
    while (tFact->numRows() > 0) tFact->removeRow(0);
  */
}

void Form2::fileOpen()
{
  QString file = QFileDialog::getOpenFileName(".", "Kinvoice (*.kiv)", this);
    
  QFileInfo file_info( file );
  if (file_info.extension( FALSE ) != "kiv")
    file += ".kiv";
    
  if (! file.isEmpty()) {
    dataFile = file;
    loadFromFile( dataFile );
  }
}

void Form2::fileSave()
{
  if (dataFile.isEmpty()) {
    QString file = QFileDialog::getSaveFileName(
						QDir::home().path(),
						"Kinvoice (*.kiv)",
						this,
						"dialogue enregistrement"
						"Donnez un nom de fichier" );
    if (! file.isEmpty()) {
      QFileInfo file_info( file );
      if (file_info.extension( FALSE ) != "kiv")
	file += ".kiv";
      dataFile = file;
    }
  }
  saveAll();
}

void Form2::fileSaveAs()
{
  QString s = QFileDialog::getSaveFileName(
					   QDir::home().path(),
					   "Kinvoice (*.kiv)",
					   this,
					   "dialogue enregistrement"
					   "Donnez un nom de fichier" );
  if (! s.isEmpty()) {
    QFileInfo file_info( s );
    if (file_info.extension( FALSE ) != "kiv")
      s += ".kiv";
    dataFile = s;
    saveAll();
  }
}

void Form2::fileExit()
{
  QString msg = QString(tr("Vous désirez sauvegarder vos données avant de quitter"));
  switch (QMessageBox::warning(this, "Kinvoice", msg, 
			       QMessageBox::Yes, QMessageBox::Cancel, QMessageBox::No)) {
  case QMessageBox::Yes:
    if (saveAll())
      qApp->quit();
    break;
  case QMessageBox::No:
    qApp->quit();
    break; 
  }
    
}

void Form2::helpAbout()
{
  QMessageBox::about(this, "Kinvoice - Licence GPL", tr("Hexysoft\n"
							"\n"
							"8 Avenue de Choisy\n"
							"75013 PARIS\n"
							"Tél: 01 45 82 42 20\n"
							"Fax: 01 56 61 17 03\n\n"
							"Contributeurs :\n"
							" v0.1 : yifeng@hexysoft.com\n"
							" v0.2 : Daniel Huhardeaux <devel@tootai.net> - Mode Auto-Entrepreneur\n"
							" v0.3 : Julien Pecqueur <julien@julienpecqueur.com>"));
}


//
//
//
// tab Articles
//
//
//


void Form2::tCat_new_clicked()
{
  Cat c;
    
  if (c.exec() == QDialog::Accepted) {
    Cats.append( new hCat(c.CatName->text(), c.CatDesc->text()));
    displayCats();
  }
}


void Form2::tCat_edit_clicked()
{
  Cat c;
  hCat* cat;
    
  //    if (tCat->currentSelection() == -1) return;
    
  cat = Cats.at(tCat->currentRow());
  c.CatName->setText(cat->name);
  c.CatDesc->setText(cat->desc);
  if (c.exec() == QDialog::Accepted) {
    cat->name = c.CatName->text();
    cat->desc = c.CatDesc->text();
	
    tCat->setText(tCat->currentRow(), 0, c.CatName->text());
    tCat->setText(tCat->currentRow(), 1, c.CatDesc->text());
  }
}


void Form2::tCat_del_clicked()
{
    
  int row = tCat->currentRow();
    
  if (row < 0) return;
    
  hCat* cat = Cats.at( row );
  QString msg = QString( tr("Êtes vous certain de vouloir supprimer la catégorie \"%1\"") )
    .arg(cat->name);
    
  switch (QMessageBox::warning(this, "", msg,
			       QMessageBox::Yes, QMessageBox::No)) {
  case QMessageBox::Yes:
    tCat->removeRow( row );
    Cats.remove( Cats.at( row ) );
	
    displayCats();
    while (tArt->numRows()) tArt->removeRow(0);
    break;
  }
}



void Form2::tCat_selectionChanged()
{
  int row = tCat->currentRow();
  if (row < 0) return;
    
  displayArticles( tCat->currentRow() );
}


void Form2::tArt_new_clicked()
{
  Art a;
    
  if (Cats.count() <= 0) {
    QMessageBox::information(this, "", tr("Définissez d'abord les catégories !"));
    return;
  }
    
  if (tCat->currentRow() > 0)
    a.ArtCat->setCurrentItem( tCat->currentRow());
    
  if (a.exec() == QDialog::Accepted) {
    hArt *art = new hArt(a.ArtRef->text(),
			 a.ArtDesc->text(),
			 a.ArtPuht->text(),
			 a.ArtStock->text(),
			 a.ArtVendu->text());
    Cats.at( a.ArtCat->currentItem() )->articles.append(art);
	
    displayCats();
    if (tCat->currentRow() == a.ArtCat->currentItem()) {
      displayArticles( a.ArtCat->currentItem() );
    }
  }
}


void Form2::displayArticles( int cat )
{
    
  while (tArt->numRows()) tArt->removeRow(0);
  tArt->insertRows(0, Cats.at(cat)->articles.count());
    
    
  for (uint i = 0; i < Cats.at(cat)->articles.count(); ++i) {
    hArt* a = Cats.at(cat)->articles.at(i);
    tArt->setText(i, 0, a->ref);
    tArt->setText(i, 1, a->desc);
    tArt->setText(i, 2, QString::number(a->puht));
    tArt->setText(i, 3, QString::number(a->stock));
    tArt->setText(i, 4, QString::number(a->vendu));
  }
}


void Form2::tArt_edit_clicked()
{
  Art c;
    
  //    if (tCat->currentSelection() == -1 || tArt->currentSelection() == -1) return;
    
  hArt* a = Cats.at(tCat->currentRow())->articles.at( tArt->currentRow() );
    
  c.ArtCat->setCurrentItem( tCat->currentRow() );
  c.ArtCat->setEnabled( FALSE );
    
  c.ArtRef->setText( a->ref );
  c.ArtDesc->setText( a->desc );
  c.ArtPuht->setText( QString::number(a->puht) );
  c.ArtStock->setText( QString::number(a->stock) );
  c.ArtVendu->setText( QString::number(a->vendu) );
    
  if (c.exec() == QDialog::Accepted) {
    a->ref = c.ArtRef->text();
    a->desc = c.ArtDesc->text();
    a->puht = c.ArtPuht->text().toFloat();
    a->stock = c.ArtStock->text().toUInt();
    a->vendu = c.ArtVendu->text().toUInt();
	
    tArt->setText(tArt->currentRow(), 0, a->ref);
    tArt->setText(tArt->currentRow(), 1, a->desc);
    tArt->setText(tArt->currentRow(), 2, QString::number(a->puht));
    tArt->setText(tArt->currentRow(), 3, QString::number(a->stock));
    tArt->setText(tArt->currentRow(), 4, QString::number(a->vendu));
  }
}


void Form2::tArt_del_clicked()
{
    
  int cat = tCat->currentRow();
  int art = tArt->currentRow();
    
  if (cat < 0) return;
  if (art < 0) return;
    
  hArt* a = Cats.at(cat)->articles.at(art);
  QString msg = QString( tr("Êtes vous certain de vouloir supprimer l'article :\n%1: %2") )
    .arg( a->ref )
    .arg( a->desc );
  switch (QMessageBox::warning(this, "", msg,
			       QMessageBox::Yes, QMessageBox::No)) {
  case QMessageBox::Yes:
	
    Cats.at( cat )->articles.remove( Cats.at( cat )->articles.at( art ) );
    delete a;
	
    tArt->removeRow( art );
    displayCats();
    break;
  }
}



void Form2::displayCats()
{
  while (tCat->numRows() > 0) tCat->removeRow(0);
    
  tCat->insertRows(0, Cats.count());
  for (uint i=0; i < Cats.count(); ++i) {
    tCat->setText(i, 0, Cats.at(i)->name);
    tCat->setText(i, 1, Cats.at(i)->desc);
    tCat->setText(i, 2, QString::number(Cats.at(i)->articles.count()));
  }
}





// tab Clients
// 
//


void Form2::addCl()
{
  Client c;
    
  if (c.exec() == QDialog::Accepted) {
    hClient* cl = new hClient();
	
    cl->info[hClient::NOM] = c.nom->text();
    cl->info[hClient::ADRESSE] = c.adresse->text();
    cl->info[hClient::CP] = c.cp->text();
    cl->info[hClient::VILLE] = c.ville->text();
    cl->info[hClient::PAYS] = c.pays->text();
    cl->info[hClient::TEL] = c.tel->text();
    cl->info[hClient::FAX] = c.fax->text();
    cl->info[hClient::MOBILE] = c.mobile->text();
    cl->info[hClient::EMAIL] = c.email->text();
    cl->info[hClient::SITE] = c.site->text();
    cl->info[hClient::COMMENTAIRE] = c.commentaire->text();
	
    int row = tClient->numRows();
    Clients.append(cl);
    tClient->insertRows(row);
	
    for (int i = 0; i < hClient::MAX_INFO; ++i)
      tClient->setText(row, i, cl->info[i]);
  }   
}

void Form2::delCl()
{
  int row = tClient->currentRow();
    
  if (row < 0) return;
    
  hClient* cl = Clients.at(row);
  QString msg = QString("Êtes vous certain de vouloir supprimer \"%1 - %2\"")
    .arg(row+1)
    .arg(cl->info[hClient::NOM]);
    
  switch (QMessageBox::warning(this, tr("Suppression de client"), msg, 
			       QMessageBox::Yes, QMessageBox::No)) {
  case QMessageBox::Yes:
    tClient->removeRow( row );
    Clients.remove( row );
    displayClient( Clients );
    break;
  }
}


void Form2::editClient_clicked()
{
  int row = tClient->currentRow();
    
  if (row < 0) return;
    
  hClient* cl = displayClients->at(row);
  Client c;
  c.nom->setText(cl->info[hClient::NOM]);
  c.adresse->setText(cl->info[hClient::ADRESSE]);
  c.cp->setText(cl->info[hClient::CP]);
  c.ville->setText(cl->info[hClient::VILLE]);
  c.pays->setText(cl->info[hClient::PAYS]);
  c.tel->setText(cl->info[hClient::TEL]);
  c.fax->setText(cl->info[hClient::FAX]);
  c.mobile->setText(cl->info[hClient::MOBILE]);
  c.email->setText(cl->info[hClient::EMAIL]);
  c.site->setText(cl->info[hClient::SITE]);
  c.commentaire->setText(cl->info[hClient::COMMENTAIRE]);
    
  if (c.exec() == QDialog::Accepted) {
    cl->info[hClient::NOM] = c.nom->text();
    cl->info[hClient::ADRESSE] = c.adresse->text();
    cl->info[hClient::CP] = c.cp->text();
    cl->info[hClient::VILLE] = c.ville->text();
    cl->info[hClient::PAYS] = c.pays->text();
    cl->info[hClient::TEL] = c.tel->text();
    cl->info[hClient::FAX] = c.fax->text();
    cl->info[hClient::MOBILE] = c.mobile->text();
    cl->info[hClient::EMAIL] = c.email->text();
    cl->info[hClient::SITE] = c.site->text();
    cl->info[hClient::COMMENTAIRE] = c.commentaire->text();
	
    for (int i = 0; i < hClient::MAX_INFO; ++i)
      tClient->setText(row, i, cl->info[i]);
  }
}


void Form2::searchClient_clicked()
{
  QString text = clientSearchStr->text();
  int index = clientSearchFields->currentItem();
  uint i;
  hClient* cl;
    
  if (index < 0 || text.isEmpty()) return;
    
  QPtrList<hClient>* result = new QPtrList<hClient>;
    
  for (i = 0; i < Clients.count(); ++i) {
    cl = Clients.at(i);
    if ((*cl).info[index].contains(text, FALSE)) {
      (*result).append(cl);
    }
  }
    
  if ((*result).count()) {
    displayClient(*result);
    addClient->setEnabled(FALSE);
    delClient->setEnabled(FALSE);
  } else {
    QMessageBox::information(this, tr("Résultat de la recherche"),
			     tr("Texte non trouvé"));
  }
}


void Form2::reloadClient_clicked()
{
  displayClient( Clients );
  addClient->setEnabled( TRUE );
  delClient->setEnabled( TRUE );
}


void Form2::displayClient( QPtrList<hClient>& list )
{
  displayClients = &list;
  hClient* cl;
    
  // remove all rows
  while (tClient->numRows() > 0) tClient->removeRow(0);
    
  // insert empty rows
  uint rows = list.count();
  tClient->insertRows(0, rows);
    
  // fill cellules
  for (rows = 0; rows < list.count(); ++rows) {
    cl = list.at(rows);
    for (int i = 0; i < hClient::MAX_INFO; ++i)
      tClient->setText(rows,i,(*cl).info[i]);
  }
}














// tab Facture
// tab



void Form2::listCat_highlighted( int index )
{
  listArt->clear();
    
  for (uint i = 0; i < Cats.at(index)->articles.count(); ++i)
    listArt->insertItem( "[" + Cats.at(index)->articles.at(i)->ref + "]"
			 + Cats.at(index)->articles.at(i)->desc);
}


void Form2::listArt_highlighted( int index )
{
  int cat = listCat->currentItem();
    
  hArt* a = Cats.at(cat)->articles.at(index);
  puht->setText(QString::number(a->puht,'f',2));
  quantite->setFocus();
}





void Form2::findClient(const QString& search)
{
  hClient* cl;
  clientResult->clear();
  if (search.isEmpty()) return;
    
  int j = 1;
  for (uint i = 0; i < Clients.count(); ++i) {
    cl = Clients.at(i);
    if ((*cl).info[hClient::NOM].contains(search, FALSE)) {
      (*cl).finded = j++;
      clientResult->insertItem((*cl).info[hClient::NOM]);
    } else {
      (*cl).finded = 0;
    }
  }    
}

void Form2::clientSelected( int index)
{
  hClient* cl;
  ++index;
  for (uint i = 0; i < Clients.count(); ++i) {
    cl = Clients.at(i);
    if ((*cl).finded == index) {
      clientEdit->setText( 
			  (*cl).info[hClient::NOM].upper() + "\n" +
			  (*cl).info[hClient::ADRESSE] + "\n" +
			  (*cl).info[hClient::CP] + " " + (*cl).info[hClient::VILLE].upper() + "\n" +
			  (*cl).info[hClient::PAYS].upper() + "\n");
    }
  }
}




void Form2::addAchat()
{
  int row = tAchat->numRows();
  tAchat->insertRows(row);
    
  QString vide;
  tAchat->setItem(row,2,new hTableIntItem(tAchat, QTableItem::WhenCurrent, vide));
  tAchat->setItem(row,3,new hTableDoubleItem(tAchat, QTableItem::WhenCurrent, vide));
  tAchat->setItem(row,4,new QTableItem(tAchat, QTableItem::Never, vide));
  tAchat->setItem(row,5,new hTableIntItem(tAchat, QTableItem::WhenCurrent, vide));
    
  tAchat->clearSelection(TRUE);
  tAchat->editCell(row, 0);
  tAchat->selectRow(row);
    
  doSum();
}

void Form2::delAchat()
{
  int row = tAchat->currentRow();
    
  if (row < 0) return;
    
  if (tAchat->currentSelection() == -1 || !tAchat->numRows()) return;
    
  QString msg = QString("Êtes vous certain de vouloir supprimer %1").arg(row+1);
  switch (QMessageBox::warning(this, "", msg, QMessageBox::Yes, QMessageBox::No)) {
  case QMessageBox::Yes:
    tAchat->removeRow( row );
    break;
  }
  doSum();
}



void Form2::tAchat_valueChanged( int x, int y)
{
  uint quantite;
  int remise;
  float puht;
  int matched;
    
  switch(y) {
  case 0:
    // reference value changed
    matched=0;
    for (uint i=0; i < Cats.count(); ++i)
      for (uint j=0; j < Cats.at(i)->articles.count(); ++j)
	if (Cats.at(i)->articles.at(j)->ref.upper() == tAchat->text(x,y).upper()) {
	  tAchat->setText(x,0,Cats.at(i)->articles.at(j)->ref.upper());
	  tAchat->setText(x,1,Cats.at(i)->articles.at(j)->desc);
	  tAchat->setText(x,3,QString::number(Cats.at(i)->articles.at(j)->puht,'f',2));
	  matched = 42;
	  break;
	}
    tAchat->editCell(x, (matched? 2 : 1));
    break;
  case 2:
  case 3:
  case 5:
    quantite = tAchat->text(x,2).toUInt();
    puht = tAchat->text(x,3).toFloat();
    remise = tAchat->text(x,5).toInt();
    tAchat->setText(x,3, QString::number(puht,'f',2));
    tAchat->setText(x, 4, QString::number(quantite*puht - 
					  (quantite*puht/100.0*remise),'f',2));
    doSum();
    break;
  }
}



void Form2::facturePrint_clicked()
{
  hFacture* facture = getCurrentFacture();
  if (facture) {
    QPrinter printer;
    /* JPEC : doc name = facture type _ facture number */
    QString name;
    name = intitule->text() + "_" + factureRef->text();
    printer.setColorMode( QPrinter::Color );
    printer.setDocName ( name ); 
    if (printer.setup(this)) {
      doPrint(printer, *facture);
    }
    delete facture;
  }
}











// refresh all data need updated when tabWidget change
void Form2::tab_currentChanged( QWidget * )
{
  listCat->clear();
  listArt->clear();
  for (uint i=0; i < Cats.count(); ++i)
    listCat->insertItem(Cats.at(i)->name);
}


void Form2::bAjouter_clicked()
{
  int cat = listCat->currentItem();
  int art = listArt->currentItem();
    
  if ( cat < 0 || art < 0 || puht->text().isEmpty() || quantite->text().isEmpty() ) return;
    
  hArt* aa = Cats.at(cat)->articles.at(art);
  hArt& a = *aa;
    
  int row = tAchat->numRows();    
  tAchat->insertRows(row);
  tAchat->setText(row, 0, a.ref);
  tAchat->setText(row, 1, a.desc);
  tAchat->setText(row, 2, quantite->text());
  tAchat->setText(row, 3, puht->text());
  tAchat->setText(row, 4, QString::number(puht->text().toFloat() * quantite->text().toUInt(),
					  'f', 2));
  tAchat->clearSelection(TRUE);
  tAchat->editCell(row, 3);
  tAchat->selectRow(row);
    
  quantite->setText("");
    
  doSum();
}


void Form2::userLogoChange_clicked()
{
  QString pix = QFileDialog::getOpenFileName(".", "images (*.gif *.jpg *.png *.xpm)", this);
  if (pix.isNull()) return;
  QPixmap pixmap(pix);
  userLogo->setPixmap(pixmap);
}


void Form2::doSum()
{
  float sum = 0.0;
  for (int i = 0; i < tAchat->numRows(); ++i)
    sum += tAchat->text(i,4).toFloat();
  sumHT->setText( QString::number(sum, 'f', 2) );
  sumTVA->setText( QString::number(tvaRate->text().toFloat()/100.0 * sum,'f', 2));
  factureTTC->setText( QString::number(tvaRate->text().toFloat()/100.0 * sum + sum, 'f', 2));
}


void Form2::tvaRate_textChanged( const QString & )
{
  doSum();
}




void Form2::autoFactureRef_clicked()
{
  QDate today = QDate::currentDate();
  int index = 1;
  do {
    QString ref;
    ref.sprintf("%04d%02d%02d%02d",
		today.year(), today.month(), today.day(), index);
	
    if (correctFactureRef(ref) == true) {
      factureRef->setText(ref);
      break;
    }
	
    index++;
  } while (42);	
}


bool Form2::correctFactureRef( QString & ref )
{
  for (uint i=0; i < Factures.count(); ++i)
    if (Factures.at(i)->ref == ref)
      return false;
  return true;
}


hFacture* Form2::getCurrentFacture()
{
  hFacture* facture = new hFacture();
    
  facture->date = factureDate->date();
    
  facture->ref = factureRef->text();
  facture->client = clientEdit->text();
    
  if (facture->ref.isEmpty() || facture->client.isEmpty()) {
    QMessageBox::information(this, tr("Informations Manquantes"),
			     tr("Veuillez renseigner la référence et le client de facturation"));
    return 0;
  }
    
  if (tAchat->numRows() <= 0) {
    QMessageBox::information(this, tr("Informations Manquantes"),
			     tr("La liste des achats est vide !"));
    return 0;
  }
    
  for (int i = 0; i < tAchat->numRows(); ++i) {
    hAchat* achat = new hAchat();
    achat->ref = tAchat->text(i,0);
    achat->desc = tAchat->text(i,1);
    achat->quantite = tAchat->text(i,2).toUInt();
    achat->puht = tAchat->text(i,3).toFloat();
    achat->pht = tAchat->text(i,4).toFloat();
    achat->remise = tAchat->text(i,5).toInt();
    facture->achats.append(achat);
  }
    
  if (tvaRate->text().isEmpty()) {
    QMessageBox::information(this, tr("Informations Manquantes"),
			     tr("Veuillez remplir le taux de TVA"));
    return 0;
  }
    
  facture->tva = tvaRate->text().toFloat();
    
  if (paiement_1->isOn())
    facture->paiement = hFacture::ESPECE;
  else if (paiement_2->isOn())
    facture->paiement = hFacture::CHEQUE;
  else if (paiement_3->isOn())
    facture->paiement = hFacture::CB;
  else if (paiement_4->isOn())
    facture->paiement = hFacture::AUTRE;
  else {
    QMessageBox::information(this, tr("Informations Manquantes"),
			     tr("Veuillez remplir le mode de paiement"));
    return 0;
  }
    
  facture->comment = factureComment->text();
    
  facture->sumHT = sumHT->text().toFloat();
  facture->sumTVA = sumTVA->text().toFloat();
  facture->sumTTC = factureTTC->text().toFloat();
    
  facture->intitule = intitule->text();
  if (facture->intitule.isEmpty()) facture->intitule = QString( tr("FACTURE") );
    
  return facture;
}


void Form2::factureNew_clicked()
{
  QString msg = QString(tr("Êtes vous certain de vouloir supprimer cette facture ?"));
  switch (QMessageBox::warning(this, "", msg, QMessageBox::Yes, QMessageBox::No)) {
  case QMessageBox::Yes:
	
    // suppression de la facture existante
    for (uint i = 0; i < Factures.count(); ++i)
      if (Factures.at(i)->ref == factureRef->text()) {
	Factures.remove( Factures.at(i) );
	facturesDisplay();
	break;
      }
	
	
    // restoration
    factureDate->setDate( QDate::currentDate() );
    autoFactureRef_clicked();
    clientEdit->setText("");
    while (tAchat->numRows() > 0) tAchat->removeRow(0);    
    factureComment->setText("");
    sumHT->setText("");
    sumTVA->setText("");
    factureTTC->setText("");
  }
}


void Form2::tFact_doubleClicked( int x, int , int, const QPoint & )
{
  hFacture& fact = *(Factures.at(x));
    
  QString msg = QString(tr("Vous voulez charger cette facture ?"));
  switch (QMessageBox::warning(this, "", msg, QMessageBox::Yes, QMessageBox::No)) {
  case QMessageBox::Yes:
	
    factureDate->setDate( fact.date );
    factureRef->setText( fact.ref );
    intitule->setText( fact.intitule );
    clientEdit->setText( fact.client );
	
    while (tAchat->numRows()) tAchat->removeRow(0);
    tAchat->insertRows(0, fact.achats.count());
    for (uint i = 0; i < fact.achats.count(); ++i) {
      tAchat->setText(i, 0, fact.achats.at(i)->ref);
      tAchat->setText(i, 1, fact.achats.at(i)->desc);
      tAchat->setText(i, 2, QString::number(fact.achats.at(i)->quantite));
      tAchat->setText(i, 3, QString::number(fact.achats.at(i)->puht,'f',2));
      tAchat->setText(i, 4, QString::number(fact.achats.at(i)->pht,'f',2));
      tAchat->setText(i, 5, QString::number(fact.achats.at(i)->remise));
    }
	
    paiement_1->setChecked(false);
    paiement_2->setChecked(false);
    paiement_3->setChecked(false);
    paiement_4->setChecked(false);
    if (fact.paiement == hFacture::ESPECE) {
      paiement_1->setChecked(true);
    } else if (fact.paiement == hFacture::CHEQUE) {
      paiement_2->setChecked(true);
    } else if (fact.paiement == hFacture::CB) {
      paiement_3->setChecked(true);
    } else if (fact.paiement == hFacture::AUTRE) {
      paiement_4->setChecked(true);
    }
	
    factureComment->setText(fact.comment);	
    sumHT->setText(QString::number(fact.sumHT,'f',2));	
    tvaRate->setText(QString::number(fact.tva,'f',2));
    sumTVA->setText(QString::number(fact.sumTVA,'f',2));
    factureTTC->setText(QString::number(fact.sumTTC,'f',2));
	
    // basculer vers tab Facture
    tab_6->showPage(tab_4);
	
  }   
}


void Form2::factureSave_clicked()
{
  uint i;
  QString msg;
  hFacture* fact = getCurrentFacture();
    
  if (fact) {
    for (i = 0; i < Factures.count(); ++i)
      if (Factures.at(i)->ref == fact->ref)
	break;
    if (i < Factures.count()) {
      msg = QString(tr("Voulez vous remplacer l'ancienne facture %1"))
	.arg(fact->ref);
      switch (QMessageBox::warning(this, tr("Facture"), msg, 
				   QMessageBox::Yes, QMessageBox::No)) {
      case QMessageBox::Yes:
	Factures.remove( Factures.at(i) );
	Factures.append(fact );
	facturesDisplay();
	break;
      }
	    
	    
    } else {
      msg = QString(tr("Voulez vous enregistrer cette facture ?"));
      switch (QMessageBox::warning(this, tr("Facture"), msg, 
				   QMessageBox::Yes, QMessageBox::No)) {
      case QMessageBox::Yes:
	Factures.append(fact);
	facturesDisplay();
		
	msg = QString(tr("Voulez vous comptabliser les articles vendus ?"));
	if (QMessageBox::warning(this, tr("Facture"), msg, 
				 QMessageBox::Yes, QMessageBox::No) ==
	    QMessageBox::Yes)
	  countVendu( *fact );
	break;
      }	    
    }
  }
}


void Form2::facturesDisplay()
{
  while (tFact->numRows() > 0) tFact->removeRow(0);
  tFact->insertRows(0, Factures.count() );
    
  for (uint i = 0; i < Factures.count(); ++i) {
    tFact->setText(i, 0, Factures.at(i)->ref);
    tFact->setText(i, 1, Factures.at(i)->date.toString(Qt::LocalDate));
    tFact->setText(i, 2, Factures.at(i)->client.section('\n', 0, 0));
    tFact->setText(i, 3, QString::number(Factures.at(i)->sumTTC, 'f', 2));
    QString paiementStr[] = { tr("Espèces"), tr("Chèque"),
			      tr("Carte Bleu"), tr("Autre") };
    tFact->setText(i, 4, paiementStr[Factures.at(i)->paiement]);
  }
}



void Form2::doPrint(QPrinter& printer, hFacture& facture)
{
  QPainter p;
  p.begin( &printer );
  QRect rect;
  const int numAchatByPage = 25;
    
  int totalPage = (facture.achats.count()+(numAchatByPage-1))/numAchatByPage;
    
  QBrush brushOrigin = p.brush();
  QPen penOrigin = p.pen();
    
  for (int page = 1; page <= totalPage; ++page) {
	
    p.setPen(QPen(black, 4));
	
    // zone de logo
    QPixmap* pix = userLogo->pixmap();
    p.drawPixmap(0, 0, *pix, 0, 0, 300, 300);
	
    // zone utilisateur
    p.setFont( QFont( "", 7, QFont::Normal) );
    rect = QRect(20, 230, 300, 15);
    p.drawText(rect, Qt::AlignLeft|Qt::AlignTop, QString( tr("Dispensé d'immatriculation au registre du commerce") ));
    rect = QRect(20, 245, 300, 15);
    p.drawText(rect, Qt::AlignLeft|Qt::AlignTop, QString( tr("et des sociétés (RCS) et au répertoire des métiers (RM)") ));
    p.setFont( QFont( "", 10) );
	
    // zone Num de la facture
    p.setBrush( QColor(210, 210, 210) );
    p.setPen( QColor(210, 210, 210) );
    rect = QRect(390, 10, 270, 30);
    p.drawRoundRect(rect, 99, 99);	
    p.setBrush( brushOrigin );
    p.setPen( penOrigin );
    QString fact = facture.intitule + " " + facture.ref;
    p.drawText(rect, Qt::AlignCenter, fact);
	
    // zone date
    rect = QRect(390, 60, 135, 20);
    p.drawText(rect, Qt::AlignCenter, QString( tr("DATE") ));
    p.drawRect(rect);
    rect = QRect(390, 80,  135, 30);
    p.drawText(rect, Qt::AlignCenter, facture.date.toString(Qt::LocalDate));
    p.drawRect(rect);
	
    // zone Num de page
    rect = QRect(525, 60, 135, 20);
    p.drawText(rect, Qt::AlignCenter, QString( tr("PAGE") ));
    p.drawRect(rect);
    rect = QRect(525, 80, 135, 30);
    p.drawText(rect, Qt::AlignCenter, QString("%1 / %1")
	       .arg(page)
	       .arg(totalPage));
    p.drawRect(rect);
	
	
    // zone client
    rect = QRect(390, 140, 270, 120);
    p.drawRect(rect);
    rect = QRect(400, 145, 265, 115);
    p.drawText(rect, Qt::AlignLeft|Qt::AlignTop, facture.client);
	
    // zone mode de paiement
    rect = QRect(20, 270, 270, 20);
    p.drawText(rect, Qt::AlignCenter, QString( tr("MODE DE REGLEMENT") ));
    p.drawRect(rect);
    rect = QRect(20, 290, 270, 30);
    QString paiementType[] = { tr("Espèces"), tr("Chèque"), tr("Carte Bleu"), tr("Autre") };
    QString paiement = paiementType[facture.paiement];
    if (! facture.comment.isEmpty())
      paiement += " ("+facture.comment+") ";
    p.drawText(rect, Qt::AlignCenter, paiement);
    p.drawRect(rect);
	
    // zone de référence
    rect = QRect(20, 350, 110, 30);
    p.drawText(rect, Qt::AlignCenter, QString( tr("REF") ));
    p.drawRect(rect);
    // zone de désignation
    rect = QRect(130, 350, 260, 30);
    p.drawText(rect, Qt::AlignCenter, QString( tr("DESIGNATION") ));
    p.drawRect(rect);
    // zone de quantité
    rect = QRect(390, 350, 70, 30);
    p.drawText(rect, Qt::AlignCenter, QString( tr("QUANTITE") ));
    p.drawRect(rect);
    // zone de prix unitaire HT
    rect = QRect(460, 350, 90, 30);
    p.drawText(rect, Qt::AlignCenter, QString( tr("PRIX U. HT") ));
    p.drawRect(rect);
    // remise
    rect = QRect(550, 350, 60, 30);
    p.drawText(rect, Qt::AlignCenter, QString( tr("REMISE") ));
    p.drawRect(rect);
    // zone de prix total HT
    rect = QRect(610, 350, 90, 30);
    p.drawText(rect, Qt::AlignCenter, QString( tr("PRIX HT") ));
    p.drawRect(rect);
	
	
    // liste des articles
    QString refs, descs, quantites, puhts, remises, phts;
    for (int it = 0; it < numAchatByPage && 
	   (it + ((page-1) * numAchatByPage)) < (int)facture.achats.count(); ++it) {
      int index = it + ((page-1) * numAchatByPage);
      refs += facture.achats.at(index)->ref + "\n";
      descs += facture.achats.at(index)->desc + "\n";
      quantites += QString::number(facture.achats.at(index)->quantite) + "\n";
      puhts += QString::number(facture.achats.at(index)->puht, 'f', 2) + "\n";
      if (facture.achats.at(index)->remise)
	remises += QString( tr("%1 %") ) .arg(facture.achats.at(index)->remise);
      remises += "\n";
      phts += QString::number(facture.achats.at(index)->pht, 'f', 2) + "\n";
    }
	
    rect = QRect(20, 380, 110, 400);
    p.drawRect(rect);
    rect = QRect(25, 380, 105, 400);
    p.setFont( QFont( "", 9) );
    p.drawText(rect, Qt::AlignLeft, refs);
    p.setFont( QFont( "", 10) );
	
    rect = QRect(130, 380, 260, 400);
    p.drawRect(rect);
    rect = QRect(135, 380, 255, 400);
    p.setFont( QFont( "", 9) );
    p.drawText(rect, Qt::AlignLeft, descs);
    p.setFont( QFont( "", 10) );
	
    rect = QRect(390, 380, 70, 400);
    p.drawRect(rect);
    rect = QRect(400, 380, 60, 400);
    p.drawText(rect, Qt::AlignRight, quantites);
	
    rect = QRect(460, 380, 90, 400);
    p.drawRect(rect);
    rect = QRect(470, 380, 80, 400);
    p.drawText(rect, Qt::AlignRight, puhts);
	
    rect = QRect(550, 380, 60, 400);
    p.drawRect(rect);
    rect = QRect(560, 380, 50, 400);
    p.drawText(rect, Qt::AlignRight, remises);
	
    rect = QRect(610, 380, 90, 400);
    p.drawRect(rect);
    rect = QRect(620, 380, 80, 400);
    p.drawText(rect, Qt::AlignRight, phts);
	
	
    // zone des totaux
    if (page == totalPage) {
	    
      p.setPen(QPen(black, 1));
	    
      //DH 03/2010
      //rect = QRect(x,y,l,h)
      rect = QRect(475, 800, 225, 25);
      p.drawRect(rect);
      rect = QRect(480, 800, 105, 25);
      p.drawText(rect, Qt::AlignLeft|Qt::AlignVCenter, QString( tr("TOTAL HT") ));
      rect = QRect(585, 800, 80, 25);
      p.drawText(rect, Qt::AlignRight|Qt::AlignVCenter, 
		 QString::number(facture.sumHT, 'f', 2) );
      rect = QRect(590, 800, 110, 25);
      p.drawText(rect, Qt::AlignRight|Qt::AlignVCenter, QString( tr("EUR ") ));
      rect = QRect(480, 830, 300, 160);
      p.setFont( QFont( "", 7, QFont::Normal) );
      p.drawText(rect, Qt::AlignLeft|Qt::AlignTop, QString( tr("TVA non applicable, art. 293 B du CGI") ));
      p.setFont( QFont( "", 10) );
	    
      rect = QRect(0, 990, 700, 15);
      //p.drawRoundRect(rect, 10, 10);
      p.setFont( QFont( "", 7, QFont::Normal) );
      p.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, QString( tr("Les produits demeurent la propriété du vendeur jusqu'au paiement intégral de leur valeur") ));
	    
      rect = QRect(0, 1005, 700, 15);
      p.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, userNom->text(0)+QString( tr(" "))+userNom->text(1)+QString( tr(" "))+userNom->text(2));
      rect = QRect(0, 1020, 700, 15);
      p.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, userNom->text(3)+QString( tr(" "))+userNom->text(4)+QString( tr(" "))+userNom->text(5));
      rect = QRect(0, 1035, 700, 15);
      p.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, userNom->text(6)+QString( tr(" "))+userNom->text(7)+QString( tr(" "))+userNom->text(8));
      rect = QRect(0, 1050, 700, 15);
      p.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, userNom->text(9)+QString( tr(" "))+userNom->text(10)+QString( tr(" "))+userNom->text(11));
      p.setFont( QFont( "", 10) );
	    
      p.setPen(penOrigin);
    } else {
      printer.newPage();
    }
  }
  p.end();
}


bool Form2::factureExists( QString & ref )
{
  uint i;
  for (i = 0; i < Factures.count(); ++i) {
    if (Factures.at(i)->ref == ref)
      return true;
  }
  return false;
}



void Form2::countVendu( hFacture & fact )
{
  for (uint i=0; i < fact.achats.count(); ++i) {
    hAchat* a = fact.achats.at(i);
	
    for (uint j=0; j < Cats.count(); ++j)
      for (uint k=0; k < Cats.at(j)->articles.count(); ++k) {
	hArt* art = Cats.at(j)->articles.at(k);
	if (a->ref == art->ref) {
	  if (art->stock < a->quantite)
	    art->stock=0;
	  art->vendu += a->quantite;
	}
      }
  }
}


void Form2::factureNew_2_clicked()
{
  QString msg = QString(tr("Êtes vous certain de vouloir créer une nouvelle facture ?"));
  switch (QMessageBox::warning(this, "", msg, QMessageBox::Yes, QMessageBox::No)) {
  case QMessageBox::Yes:
	
    // restoration
    factureDate->setDate( QDate::currentDate() );
    autoFactureRef_clicked();
    clientEdit->setText("");
    while (tAchat->numRows() > 0) tAchat->removeRow(0);    
    factureComment->setText("");
    sumHT->setText("");
    sumTVA->setText("");
    factureTTC->setText("");
  }
}

/*
 * Tab Reporting
 */
 
void Form2::getReporting ()
{
  /* Déclarations */
  uint Year;
  double Ht, Ttc;

  // Gestion box bDate :
  Year = atoi(bDate->text());
  if (Year == 0 && bDate->text()[0]!='0')
    {
      // Erreur
      QMessageBox::information(this, tr("Erreur de saisie!"),
			       tr("Il faut entrer une année valide (ex: 2010)!"));
      Year = QDate::currentDate().year();
    }

  /* Alimentation widgets */
  bDate->setText(QString::number(Year));
  while (tCumul->numRows() > 0) tCumul->removeRow(0);
  tCumul->insertRows(0, 12);
 
  /* Calcul des cumuls */
  for (uint i = 0; i < 12; ++i) {
    tCumul->setText(i, 0, QDate::longMonthName(i+1));
    Ht = 0;
    Ttc = 0;
    for (uint j = 0; j < Factures.count(); j++) {
      if (Factures.at(j)->date.year() == Year && Factures.at(j)->date.month() == i+1) {
	Ttc = Ttc + Factures.at(j)->sumTTC;
	Ht = Ht + Factures.at(j)->sumHT;
      }
    }
    tCumul->setText(i, 1, QString::number(Ht, 'f', 2));
    tCumul->setText(i, 2, QString::number(Ttc, 'f', 2));
  }
    
}

