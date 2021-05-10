// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "UK5Q_form.h"
#include "UK5Q_box.h"		
#include "UK5B_var.h"
#include <QFile>
#include <QMap>

UK5Q_form::UK5Q_form(QWidget *parent)
	: QMainWindow(parent)
{

	QMap<int, QWidget*> map;

	map.insert(0, ui->UK5Q_scrollAreaWidgetContents_IN);
	map.insert(1, ui->UK5Q_scrollAreaWidgetContents_IN_OUT);
	map.insert(2, ui->UK5Q_scrollAreaWidgetContents_OUT);

	
	bool x = QFile::exists("config.ini");
	int place;
//in	
	UK5B_varD vr;		vr.UK5B_setName("vr");		vr.UK5B_setValue(x, 0.);				place=vr.UK5B_getPlace();	
	UK5B_varD bb;		bb.UK5B_setName("bb");		bb.UK5B_setValue(x, 0.);				place=bb.UK5B_getPlace();
	UK5B_varD h;		h.UK5B_setName("h");			h.UK5B_setValue(x, 0.);				place=h.UK5B_getPlace();
	UK5B_varI nog;		nog.UK5B_setName("nog");		nog.UK5B_setValue(x, 0);				place=nog.UK5B_getPlace();
	UK5B_varD dog;		dog.UK5B_setName("dog");		dog.UK5B_setValue(x, 0.);			place=dog.UK5B_getPlace();
	UK5B_varD qst;		qst.UK5B_setName("qst");		qst.UK5B_setValue(x, 0.);			place=qst.UK5B_getPlace();
	UK5B_varD cct;		cct.UK5B_setName("cct");		cct.UK5B_setValue(x, 0.);			place=cct.UK5B_getPlace();
	UK5B_varI n;		n.UK5B_setName("n");			n.UK5B_setValue(x, 0);				place=n.UK5B_getPlace();
	UK5B_varD psh;		psh.UK5B_setName("psh");		psh.UK5B_setValue(x, 0.);			place=psh.UK5B_getPlace();
	UK5B_varVD l;		l.UK5B_setName("l");			l.UK5B_setValue(x, {});				place=l.UK5B_getPlace();
	UK5B_varVD b;		b.UK5B_setName("b");			b.UK5B_setValue(x, {});				place=b.UK5B_getPlace();
	UK5B_varVD hog;		hog.UK5B_setName("hog");		hog.UK5B_setValue(x, {});			place=hog.UK5B_getPlace();
//in_out
	UK5B_varD nn;		nn.UK5B_setName("nn");		nn.UK5B_setValue(x, 10.);			place=nn.UK5B_getPlace();
	UK5B_varD xn;		xn.UK5B_setName("xn");		xn.UK5B_setValue(x, 0.);				place=xn.UK5B_getPlace();
//out	
	UK5B_varD ll;		ll.UK5B_setName("ll");		ll.UK5B_setValue(false, 0.);		place=ll.UK5B_getPlace();
	UK5B_varD rll;		rll.UK5B_setName("rll");		rll.UK5B_setValue(false, 0.);		place=rll.UK5B_getPlace();
	UK5B_varD rb;		rb.UK5B_setName("rb");		rb.UK5B_setValue(false, 0.);		place=rb.UK5B_getPlace();
	UK5B_varD rw;		rw.UK5B_setName("rw");		rw.UK5B_setValue(false, 0.);		place=rw.UK5B_getPlace();
	UK5B_varD rh;		rh.UK5B_setName("rh");		rh.UK5B_setValue(false, 0.);		place=rh.UK5B_getPlace();
	UK5B_varD rl;		rl.UK5B_setName("rl");		rl.UK5B_setValue(false, 0.);		place=rl.UK5B_getPlace();
	UK5B_varD rnog;		rnog.UK5B_setName("rnog");	rnog.UK5B_setValue(false, 0.);		place=rnog.UK5B_getPlace();
	UK5B_varD c;		c.UK5B_setName("c");			c.UK5B_setValue(false, 0.);		place=c.UK5B_getPlace();
	UK5B_varD d;		d.UK5B_setName("d");			d.UK5B_setValue(false, 0.);		place=d.UK5B_getPlace();
	UK5B_varD dx;		dx.UK5B_setName("dx");		dx.UK5B_setValue(false, 0.);		place=dx.UK5B_getPlace();
	UK5B_varD dy;		dy.UK5B_setName("dy");		dy.UK5B_setValue(false, 0.);		place=dy.UK5B_getPlace();
	UK5B_varD dz;		dz.UK5B_setName("dz");		dz.UK5B_setValue(false, 0.);		place=dz.UK5B_getPlace();
		
// коммент без смысла

	ui = new Ui::MainWindow();
	ui->setupUi(this);
}
