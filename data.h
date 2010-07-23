
#include <qstring.h>
#include <qptrlist.h>
#include <qdatetime.h>

#ifndef __DATA_H__
#define __DATA_H__

class hArt {
    public:
    QString ref;
    QString desc;
    float puht;
    uint stock;
    uint vendu;
    hArt() : puht(0.0), stock(0), vendu(0) {};
    hArt(QString r, QString d, QString p, QString s, QString v) {
	ref=r;
	desc=d;
	puht=p.toFloat();
	stock=s.toUInt();
	vendu=v.toUInt();
    };
    ~hArt() {};
};

class hCat {
    public:
    QString name;
    QString desc;
    QPtrList<hArt> articles;
    hCat() {};
    hCat(QString n, QString d) : name(n), desc(d) {};
    ~hCat() { articles.clear(); };
};

class hAchat {
    public:
    QString ref;
    QString desc;
    uint quantite;
    float puht;
    float pht;
    int remise;
    hAchat() : quantite(0), puht(0.0), pht(0.0), remise(0) {};
    ~hAchat() {};
};

class hFacture {
    public:
    QDate date;
    QString ref;
    QString intitule;
    QString client;
    QPtrList<hAchat> achats;
    
    int paiement;
    enum typePaiement {
	ESPECE,
	CHEQUE,
	CB,
	AUTRE
    };
    
    QString comment;
    float sumHT;
    float tva;
    float sumTVA;
    float sumTTC;
    hFacture() : 
	    sumHT(0.0),tva(0.0),sumTVA(0.0),sumTTC(0.0) {
	date = QDate::currentDate ();
    };
    ~hFacture() {};
};


class hClient {
    public:
    enum clientTypeInfo {
	NOM=0,
	ADRESSE,
	CP,
	VILLE,
	PAYS,
	TEL,
	FAX,
	MOBILE,
	EMAIL,
	SITE,
	COMMENTAIRE,
	MAX_INFO
    };
    
    QString info[ MAX_INFO ];
    
    // pour clientResult in Facture
    int finded;
    
    hClient() : finded(0) {};
    ~hClient() {};
};
    

extern QPtrList<hCat>	Cats;
extern QPtrList<hClient>	Clients;



#endif // __DATA_H__
