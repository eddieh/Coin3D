/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

// Important note: nearly all of this sourcefile was generated by the
// Coin/scripts/templant script from the code in SFNodeEnginePath.tpl.
//
// Only the notify() function at the bottom of the file deviates from
// the templatized code of SoSF[Node|Engine|Path].

///////////////////////////////////////////////////////////////////////////

//$ BEGIN TEMPLATE SFNodeEnginePath(Path, path)

/*!
  \class SoSFPath SoSFPath.h Inventor/fields/SoSFPath.h
  \brief The SoSFPath class is a container for a single path.
  \ingroup fields

  This field container stores a pointer to a Coin path. It takes care
  of the necessary functionality for handling copy, import and export
  operations.

  Note that the path pointer stored in a field instance of this type
  may be a \c NULL pointer.

  \sa SoPath, SoMFPath

*/

#include <Inventor/fields/SoSFPath.h>
#include <Inventor/fields/SoSubFieldP.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/engines/SoEngine.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


// Can't use SO_SFIELD_SOURCE() because we need to modify setValue()
// to ref and unref the passed path.
SO_SFIELD_REQUIRED_SOURCE(SoSFPath);


// Override from parent class.
void
SoSFPath::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFPath);
}

// (Declarations hidden in SO_[S|M]FIELD_HEADER macro in header file,
// so don't use Doxygen commenting.)
#ifndef DOXYGEN_SKIP_THIS

/* Constructor, sets initial path pointer to a \c NULL pointer. */
SoSFPath::SoSFPath(void)
{
  this->value = NULL;
#ifdef COIN_SOSFPATH_H
  this->head = NULL;
#endif // COIN_SOSFPATH_H
}

/* Destructor, dereferences the current path pointer if necessary. */
SoSFPath::~SoSFPath(void)
{
  this->enableNotify(FALSE);
  this->setValue(NULL);
}

#endif // DOXYGEN_SKIP_THIS


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Store the \a newval path pointer in this field. If \a newval is not
// \c NULL, will add 1 to the reference count of the path.
void
SoSFPath::setValue(SoPath * newval)
{
  SoPath * oldptr = this->getValue();
  if (oldptr == newval) return;

  if (oldptr) {
    oldptr->removeAuditor(this, SoNotRec::FIELD);
    oldptr->unref();
#ifdef COIN_SOSFPATH_H
    SoNode * h = oldptr->getHead();
    // The path should be audited by us at all times. So don't use
    // SoSFPath to wrap SoTempPath or SoLightPath, for instance.
    assert(h==this->head && "Path head changed without notification!");
    if (h) {
      h->removeAuditor(this, SoNotRec::FIELD);
      h->unref();
    }
#endif // COIN_SOSFPATH_H
  }

  if (newval) {
    newval->addAuditor(this, SoNotRec::FIELD);
    newval->ref();
#ifdef COIN_SOSFPATH_H
    this->head = newval->getHead();
    if (this->head) {
      this->head->addAuditor(this, SoNotRec::FIELD);
      this->head->ref();
    }
#endif // COIN_SOSFPATH_H
  }

  this->value = newval;
  this->valueChanged();
}

// Compares to see if the \a field points to the same path as this
// field does, and returns \c TRUE if this is the case.
//
// Be aware that this method does \e not check for path/subgraph
// equality if the pointers are not the same, so \c FALSE is returned
// even though the contents of the path/subgraph are equal.
SbBool
SoSFPath::operator==(const SoSFPath & field) const
{
  return (this->getValue() == field.getValue());
}

// Import path.
SbBool
SoSFPath::readValue(SoInput * in)
{
  SoBase * baseptr;
  if (!SoBase::read(in, baseptr, SoPath::getClassTypeId())) return FALSE;

  if (in->eof()) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }

  this->setValue((SoPath *)baseptr);
  return TRUE;
}

// Export path.
void
SoSFPath::writeValue(SoOutput * out) const
{
  SoBase * base = this->getValue();
  if (base) {
    if (base->isOfType(SoNode::getClassTypeId())) {
      SoWriteAction wa(out);
      wa.continueToApply((SoNode *)base);
    }
    else if (base->isOfType(SoPath::getClassTypeId())) {
      SoWriteAction wa(out);
      wa.continueToApply((SoPath *)base);
    }
    else if (base->isOfType(SoEngine::getClassTypeId())) {
      ((SoEngine *)base)->writeInstance(out);
    }
    else {
      assert(0 && "strange internal error");
    }
  }
  else {
    // This actually works for both ASCII and binary formats.
    out->write("NULL");
  }
}

#endif // DOXYGEN_SKIP_THIS


// Overridden from parent to propagate write reference counting to
// path.
void
SoSFPath::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);

  SoPath * n = this->getValue();
  // Set the "from field" flag as FALSE, as that flag is meant to be
  // used for references through field-to-field connections.
  if (n) n->addWriteReference(out, FALSE);
#ifdef COIN_SOSFPATH_H
  if (n && n->getHead()) n->getHead()->addWriteReference(out, FALSE);
#endif // COIN_SOSFPATH_H
}

// Override from parent to update our path pointer reference.
void
SoSFPath::fixCopy(SbBool copyconnections)
{
  SoPath * n = this->getValue();
  if (!n) return;

  // There's only been a bitwise copy of the pointer; no auditing has
  // been set up, no increase in the reference count. So we do that by
  // setting the value to NULL and then re-setting with setValue().
  this->value = NULL;

#if defined(COIN_SOSFNODE_H) || defined(COIN_SOSFENGINE_H)
  SoFieldContainer * fc = SoFieldContainer::findCopy(n, copyconnections);
  this->setValue((SoPath *)fc);
#endif // COIN_SOSFNODE_H || COIN_SOSFENGINE_H

#ifdef COIN_SOSFPATH_H
  this->setValue(n->copy());
#endif // COIN_SOSFPATH_H
}

// Override from SoField to check path pointer.
SbBool
SoSFPath::referencesCopy(void) const
{
  if (inherited::referencesCopy()) return TRUE;

  SoBase * n = this->getValue();
  if (!n) return FALSE;

  if (n->isOfType(SoNode::getClassTypeId()) ||
      n->isOfType(SoEngine::getClassTypeId())) {
    if (SoFieldContainer::checkCopy((SoFieldContainer *)n)) return TRUE;
  }
  else if (n->isOfType(SoPath::getClassTypeId())) {
    SoPath * p = (SoPath *)n;
    if (p->getHead() == NULL) return FALSE;
    if (SoFieldContainer::checkCopy(p->getHead())) return TRUE;
  }
  else {
    assert(0 && "strange internal error");
  }

  return FALSE;
}
//$ END TEMPLATE SFNodeEnginePath


void
SoSFPath::notify(SoNotList * l)
{
  // Detect if our path has gotten a new head :^), and if so do the
  // necessary audit setup magic.
  if (this->getValue() && this->getValue()->getHead() != this->head) {
    if (this->head) {
      this->head->removeAuditor(this, SoNotRec::FIELD);
      this->head->unref();
    }
    this->head = this->getValue()->getHead();
    if (this->head) {
      this->head->addAuditor(this, SoNotRec::FIELD);
      this->head->ref();
    }
  }

  inherited::notify(l);
}
