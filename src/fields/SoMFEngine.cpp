/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

// Important note: this sourcefile was in full generated by the
// Coin/scripts/templant script from the code in MFNodeEnginePath.tpl.

//$ BEGIN TEMPLATE MFNodeEnginePath(ENGINE, Engine, engine)

/*!
  \class SoMFEngine SoMFEngine.h Inventor/fields/SoMFEngine.h
  \brief The SoMFEngine class is a container for engines.

  \ingroup fields

  This field container stores an array of pointers to engines. It takes
  care of the necessary functionality for handling copy, import and
  export operations.

  Note that engine pointers stored in field instances of this type may
  be \c NULL pointers.

  \sa SoEngine, SoSFEngine

*/

// Type-specific define to be able to do #ifdef tests on type.  (Note:
// used to check the header file wrapper define, but that doesn't work
// with --enable-compact build.)
#define COIN_INTERNAL_SOMFENGINE

#include <Inventor/fields/SoMFEngine.h>

#include "SbBasicP.h"

#include <Inventor/fields/SoSFEngine.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SoPath.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/nodes/SoNode.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include "fields/SoSubFieldP.h"

// These are the macros from SO_MFIELD_SOURCE_MALLOC we're
// using. What's missing is the SO_MFIELD_VALUE_SOURCE macro, which we
// need to implement "by hand" so reference counting and auditing
// comes out correctly.
SO_MFIELD_REQUIRED_SOURCE(SoMFEngine);
SO_MFIELD_CONSTRUCTOR_SOURCE(SoMFEngine);
SO_MFIELD_MALLOC_SOURCE(SoMFEngine, SoEngine *);
// Note that we're using the MALLOC versions (which just does
// bit-copying) of the macros, and not the the ALLOC versions (which
// allocates with "new", so constructors are run). The reason for this
// is that it's node/engine/path *pointers* that are simply bit-wise
// copied.


// Override from parent class.
void
SoMFEngine::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFEngine);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

//// From the SO_MFIELD_VALUE_SOURCE macro, start. ///////////////////////////

// We can't use the macro invocation, as we need to take care of doing
// ref() and unref() on the engines in the array.

int
SoMFEngine::fieldSizeof(void) const
{
  return sizeof(SoEngine *);
}

void *
SoMFEngine::valuesPtr(void)
{
  return this->values;
}

void
SoMFEngine::setValuesPtr(void * ptr)
{
  // We don't get any ref()'ing done here, or any notification
  // mechanisms set up -- so this function should _only_ be used for
  // initial setup of array memory.  In Coin, it's only used from
  // SoMField::allocValues().
  this->values = static_cast<SoEngine **>(ptr);
}

int
SoMFEngine::find(SoEngine * value, SbBool addifnotfound)
{
  for (int i=0; i < this->num; i++) if ((*this)[i] == value) return i;

  if (addifnotfound) this->set1Value(this->num, value);
  return -1;
}

void
SoMFEngine::setValues(const int start, const int numarg, const SoEngine ** newvals)
{
  // Disable temporarily, so we under any circumstances will not send
  // more than one notification about the changes.
  SbBool notificstate = this->enableNotify(FALSE);
  // Important note: the notification state is reset at the end, so
  // this function should *not* have multiple return-points.

  // ref() new engines before unref()-ing old ones, in case there are
  // common engines (we don't want any premature destruction to happen).
  { for (int i=0; i < numarg; i++) if (newvals[i]) newvals[i]->ref(); }

  // We favor simplicity of code over performance here.
  { for (int i=0; i < numarg; i++)
    this->set1Value(start+i, const_cast<SoEngine *>(newvals[i])); }

  // unref() to match the initial ref().
  { for (int i=0; i < numarg; i++) if (newvals[i]) newvals[i]->unref(); }

  // Finally, send notification.
  (void)this->enableNotify(notificstate);
  this->setChangedIndices(start, numarg);
  if (notificstate) this->valueChanged();
  this->setChangedIndices();
}

void
SoMFEngine::set1Value(const int idx, SoEngine * newval)
{
  // Disable temporarily, so we under no circumstances will send more
  // than one notification about the change.
  SbBool notificstate = this->enableNotify(FALSE);
  // Important note: the notification state is reset at the end, so
  // this function should *not* have multiple return-points.

  // Don't use getNum(), getValues() or operator[] to find old values,
  // since this might trigger a recursive evaluation call if the field
  // is connected.

  // Expand array if necessary.
  if (idx >= this->num) {
#ifdef COIN_INTERNAL_SOMFPATH
    for (int i = this->num; i <= idx; i++) this->pathheads.append(NULL);
#endif // COIN_INTERNAL_SOMFPATH
    this->setNum(idx + 1);
  }

  SoEngine * oldptr = this->values[idx];
  if (oldptr != newval) {
    if (oldptr) {
#ifdef COIN_INTERNAL_SOMFPATH
      SoNode * h = oldptr->getHead();
      // The path should be audited by us at all times. So don't use
      // SoMFPath to wrap SoTempPath or SoLightPath, for instance.
      assert(h==this->pathheads[idx] &&
             "Path head changed without notification!");
      if (h) {
        h->removeAuditor(this, SoNotRec::FIELD);
        h->unref();
      }
#endif // COIN_INTERNAL_SOMFPATH
      oldptr->removeAuditor(this, SoNotRec::FIELD);
      oldptr->unref();
    }

    if (newval) {
      newval->addAuditor(this, SoNotRec::FIELD);
      newval->ref();
#ifdef COIN_INTERNAL_SOMFPATH
      SoNode * h = newval->getHead();
      if (h) {
        h->addAuditor(this, SoNotRec::FIELD);
        h->ref();
      }
#endif // COIN_INTERNAL_SOMFPATH
    }

    this->setChangedIndex(idx);
    this->values[idx] = newval;
#ifdef COIN_INTERNAL_SOMFPATH
    this->pathheads[idx] = newval ? newval->getHead() : NULL;
#endif // COIN_INTERNAL_SOMFPATH
  }

  // Finally, send notification.
  (void)this->enableNotify(notificstate);
  if (notificstate) this->valueChanged();
  this->setChangedIndices();
}

void
SoMFEngine::setValue(SoEngine * value)
{
  this->deleteAllValues();
  this->set1Value(0, value);
}

SbBool
SoMFEngine::operator==(const SoMFEngine & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SoEngine ** const lhs = this->getValues(0);
  const SoEngine ** const rhs = field.getValues(0);
  for (int i = 0; i < this->num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

/*!
  \copydoc SoMFFloat::deleteAllValues()
*/
void
SoMFEngine::deleteAllValues(void)
{
  // Don't use getNum(), but use this->num directly, since getNum()
  // might trigger a recursive evaluation call if the field
  // is connected.

  if (this->num) this->deleteValues(0);
}

// Overridden to handle unref() and removeAuditor().
void
SoMFEngine::deleteValues(int start, int numarg)
{
  // Note: this function overrides the one in SoMField, so if you do
  // any changes here, take a look at that method aswell.

  if (numarg == -1) numarg = this->num - start;
  for (int i=start; i < start+numarg; i++) {
    SoEngine * n = this->values[i];
    if (n) {
      n->removeAuditor(this, SoNotRec::FIELD);
      n->unref();
    }
#ifdef COIN_INTERNAL_SOMFPATH
    SoNode * h = this->pathheads[start];
    this->pathheads.remove(start);
    if (h) {
      h->removeAuditor(this, SoNotRec::FIELD);
      h->unref();
    }
#endif // COIN_INTERNAL_SOMFPATH
  }

  inherited::deleteValues(start, numarg);
}

// Overridden to insert NULL pointers in new array slots.
void
SoMFEngine::insertSpace(int start, int numarg)
{
  // Disable temporarily so we don't send notification prematurely
  // from inherited::insertSpace().
  SbBool notificstate = this->enableNotify(FALSE);
  // Important note: the notification state is reset at the end, so
  // this function should *not* have multiple return-points.

  inherited::insertSpace(start, numarg);
  for (int i=start; i < start+numarg; i++) {
#ifdef COIN_INTERNAL_SOMFPATH
    this->pathheads.insert(NULL, start);
#endif // COIN_INTERNAL_SOMFPATH
    this->values[i] = NULL;
  }

  // Initialization done, now send notification.
  (void)this->enableNotify(notificstate);
  if (notificstate) this->valueChanged();
}

/*!
  \copydoc SoMFFloat::copyValue()
*/
void
SoMFEngine::copyValue(int to, int from)
{
  this->values[to] = this->values[from];
}

//// From the SO_MFIELD_VALUE_SOURCE macro, end. /////////////////////////////


// Import a single engine.
SbBool
SoMFEngine::read1Value(SoInput * in, int index)
{
  SoSFEngine sfengine;
  SbBool result = sfengine.readValue(in);
  if (result) this->set1Value(index, sfengine.getValue());
  return result;
}

// Export a single engine.
void
SoMFEngine::write1Value(SoOutput * out, int idx) const
{
  // NB: This code is common for SoMFNode, SoMFPath and SoMFEngine.
  // That's why we check for the base type before writing.

  SoBase * base = this->values[idx];
  if (base) {
    if (base->isOfType(SoNode::getClassTypeId())) {
      coin_assert_cast<SoNode *>(base)->writeInstance(out);
    }
    else if (base->isOfType(SoPath::getClassTypeId())) {
      SoWriteAction wa(out);
      wa.continueToApply(coin_assert_cast<SoPath*>(base));
    }
    else if (base->isOfType(SoEngine::getClassTypeId())) {
      coin_assert_cast<SoEngine *>(base)->writeInstance(out);
    }
  }
  else {
    out->write("NULL");
  }
}

#endif // DOXYGEN_SKIP_THIS


// Overridden from parent to propagate write reference counting to
// engine.
void
SoMFEngine::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);

  for (int i = 0; i < this->getNum(); i++) {
    SoBase * base = this->values[i];
    if (base) {
      // NB: This code is common for SoMFNode, SoMFPath and SoMFEngine.
      // That's why we check the base type before writing/counting

      if (base->isOfType(SoNode::getClassTypeId())) {
        coin_assert_cast<SoNode *>(base)->writeInstance(out);
      }
      else if (base->isOfType(SoEngine::getClassTypeId())) {
        coin_assert_cast<SoEngine *>(base)->addWriteReference(out);
      }
      else if (base->isOfType(SoPath::getClassTypeId())) {
        SoWriteAction wa(out);
        wa.continueToApply(coin_assert_cast<SoPath *>(base));
      }
    }
  }
}

// Override from parent to update our engine pointer
// references. This is necessary so we do the Right Thing with regard
// to the copyconnections flag.
//
// Note that we have to unplug auditing and the reference counter
// addition we made during the copy process.
//
// For reference for future debugging sessions, copying of this field
// goes like this:
//
//    - copyFrom() is called (typically from SoFieldData::overlay())
//    - copyFrom() calls operator=()
//    - operator=() calls setValues()
//    - we have a local copy (ie not from SoSubField.h) of setValues()
//      that sets up auditing and references the array items
//
// <mortene@sim.no>
void
SoMFEngine::fixCopy(SbBool copyconnections)
{
  // Disable temporarily, so we under no circumstances will send more
  // than one notification about the changes.
  SbBool notificstate = this->enableNotify(FALSE);
  // Important note: the notification state is reset at the end, so
  // this function should *not* have multiple return-points.

  for (int i=0; i < this->getNum(); i++) {
    SoEngine * n = (*this)[i];
    if (n) {
#if COIN_DEBUG
      n->assertAlive();
#endif // COIN_DEBUG
      // The set1Value() call below will automatically de-audit and
      // un-ref the old pointer value node reference we have in the
      // array, *before* re-inserting a copy.

#if defined(COIN_INTERNAL_SOMFNODE) || defined(COIN_INTERNAL_SOMFENGINE)
      SoFieldContainer * fc = SoFieldContainer::findCopy(n, copyconnections);
#if COIN_DEBUG
      if (fc) fc->assertAlive();
#endif // COIN_DEBUG
      if (fc) this->set1Value(i, coin_assert_cast<SoEngine *>(fc));
#endif // COIN_INTERNAL_SOMFNODE || COIN_INTERNAL_SOMFENGINE

#ifdef COIN_INTERNAL_SOMFPATH
      this->set1Value(i, n->copy());
#endif // COIN_INTERNAL_SOMFPATH
    }
  }

  // Finally, send notification.
  (void)this->enableNotify(notificstate);
  if (notificstate) this->valueChanged();
}

// Override from SoField to check engine pointer.
SbBool
SoMFEngine::referencesCopy(void) const
{
  if (inherited::referencesCopy()) return TRUE;

  for (int i=0; i < this->getNum(); i++) {
    SoEngine * item = (*this)[i];
    if (item) {
#if defined(COIN_INTERNAL_SOMFNODE) || defined(COIN_INTERNAL_SOMFENGINE)
      if (SoFieldContainer::checkCopy(coin_assert_cast<SoFieldContainer *>(item))) return TRUE;
#endif // COIN_INTERNAL_SOMFNODE || COIN_INTERNAL_SOMFENGINE
#ifdef COIN_INTERNAL_SOMFPATH
      if (item->getHead() && SoFieldContainer::checkCopy(item->getHead())) return TRUE;
#endif // COIN_INTERNAL_SOMFPATH
    }
  }

  return FALSE;
}

// Kill the type-specific define.
#undef COIN_INTERNAL_SOMFENGINE
//$ END TEMPLATE MFNodeEnginePath


#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoMFEngine field;
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
  BOOST_CHECK_EQUAL(field.getNum(), 0);
}

#endif // COIN_TEST_SUITE
