// 	$Id$	

#include <qkeycode.h>

#include <kmessagebox.h>
#include <kglobal.h>
#include <klocale.h>

#include "ktimeedit.h" 
#include "ktimeedit.moc"

KTimeEdit::KTimeEdit(QWidget *parent, QTime qt, const char *name) 
  : QComboBox(TRUE, parent, name)
{
  setInsertionPolicy(NoInsertion);

  mTime = qt;

  // Fill combo box with selection of times in localized format.
  QTime timeEntry(0,0,0);
  do {
    insertItem(KGlobal::locale()->formatTime(timeEntry));
    timeEntry = timeEntry.addSecs(60*15);
  } while (!timeEntry.isNull());

  updateDisp();
  setFocusPolicy(QWidget::StrongFocus);

  connect(this, SIGNAL(activated(int)), this, SLOT(activ(int)));
  connect(this, SIGNAL(highlighted(int)), this, SLOT(hilit(int)));
}

KTimeEdit::~KTimeEdit()
{
}

QTime KTimeEdit::getTime(bool &ok)
{
  validateEntry();
  ok = current_display_valid;
  return mTime;
}

QSizePolicy  KTimeEdit::sizePolicy() const
{
  // Set size policy to Fixed, because edit cannot contain more text than the
  // string representing the time. It doesn't make sense to provide more space.
  QSizePolicy sizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

  return sizePolicy;
}

void KTimeEdit::setTime(QTime newTime)
{
  mTime = newTime;
  updateDisp();
}

void KTimeEdit::activ(int) 
{
  emit timeChanged(mTime,0);
}

void KTimeEdit::hilit(int )
{
  // we don't currently need to do anything here.
}

void KTimeEdit::addTime(QTime qt)
{
  // Calculate the new time.
  mTime = qt.addSecs(mTime.minute()*60+mTime.hour()*3600);
  emit timeChanged(mTime, 0);
  updateDisp();
}

void KTimeEdit::subTime(QTime qt)
{
  int h, m;

  // Note that we cannot use the same method for determining the new
  // time as we did in addTime, because QTime does not handle adding
  // negative seconds well at all.
  h = mTime.hour()-qt.hour();
  m = mTime.minute()-qt.minute();

  if(m < 0) {
    m += 60;
    h -= 1;
  }

  if(h < 0) {
    h += 24;
  }

  // store the newly calculated time.
  mTime.setHMS(h, m, 0);
  emit timeChanged(mTime, 0);
  updateDisp();
}                  

void KTimeEdit::keyPressEvent(QKeyEvent *qke)
{
  switch(qke->key()) {
  case Key_Enter:
  case Key_Return:
    validateEntry();
    break;
  case Key_Down:
    addTime(QTime(0,15,0));
    break;
  case Key_Up:
    subTime(QTime(0,15,0));
    break;
  default:
    QComboBox::keyPressEvent(qke);
    break;
  } // switch
}

void KTimeEdit::validateEntry()
{
  QTime t = KGlobal::locale()->readTime(currentText());

  if (!t.isValid()) {
    KMessageBox::sorry(this,"You must specify a valid time");
    current_display_valid = false;
  } else {
    mTime = t;
    current_display_valid = true;
  }
}

void KTimeEdit::updateDisp() 
{
  QString s = KGlobal::locale()->formatTime(mTime);
  setEditText(s);

  if (!mTime.minute() % 15) {
    setCurrentItem((mTime.hour()*4)+(mTime.minute()/15));
  }
}
