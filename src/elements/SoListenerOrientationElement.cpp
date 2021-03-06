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

/*!
  \class SoListenerOrientationElement Inventor/elements/SoListenerOrientationElement.h
  \brief The SoListenerOrientationElement holds the orientation of the current listener.

  \ingroup elements

  This orientation is set by SoListener nodes and SoCamera Nodes during
  audio rendering. When a SoListener is visited by the SoAudioRenderAction,
  it will add a new SoListenerOrientationElement to the state, holding it's
  orientation and with the setbylistener flag set. When a SoCamera is
  visited by SoAudioRenderAction, it will add a new
  SoListenerOrientationElement only if there are no previous elements with
  the setbylistener flag set.

  The SoListenerOrientationElement is used when the SoVRMLSound nodes render
  themselves.

  \COIN_CLASS_EXTENSION

  \since Coin 2.0
*/

#include <Inventor/elements/SoListenerOrientationElement.h>

#include "coindefs.h"
#include "SbBasicP.h"

#include <Inventor/nodes/SoNode.h>

/*!
  \fn SoListenerOrientationElement::orientation

  The orientation of the listener. Can be set by the SoListener class
  or the SoCamera class.
*/

SO_ELEMENT_SOURCE(SoListenerOrientationElement);

/*!
  This static method initializes static data for the
  SoListenerOrientationElement class.
*/

void
SoListenerOrientationElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoListenerOrientationElement, inherited);
}

/*!
  The destructor.
*/

SoListenerOrientationElement::~SoListenerOrientationElement(void)
{
}

/*!
  Initializes the element to it's default value. The default
  value for the orientation is (0.0f, 0.0f, 1.0f, 0.0f) and the
  default value for the setByListener flag is FALSE.
*/

void
SoListenerOrientationElement::init(SoState * state)
{
  inherited::init(state);
  this->orientation = SbRotation(0.0f, 0.0f, 1.0f, 0.0f);
  this->setbylistener = FALSE;
}

/*!
  Sets the current listener orientation, and indicates if it was set
  by a SoListener node or a SoCamera node.
*/

void
SoListenerOrientationElement::set(SoState * const state,
                                  SoNode * const COIN_UNUSED_ARG(node),
                                  const SbRotation & orientation,
                                  SbBool setbylistener)
{
  SoListenerOrientationElement *elem =
    coin_safe_cast<SoListenerOrientationElement *>
    (
     SoElement::getElement(state,classStackIndex)
     );
  if (elem) {
    elem->orientation = orientation;
    elem->setbylistener = setbylistener;
  }
}

//! Returns the current listener orientation

const SbRotation &
SoListenerOrientationElement::get(SoState * const state)
{
  const SoListenerOrientationElement * elem =
    coin_assert_cast<const SoListenerOrientationElement *>
    (
     SoElement::getConstElement(state, classStackIndex)
     );
  return elem->orientation;
}

/*!
  Returns TRUE if the orientation was set by a SoListener node,
  and FALSE if it was set by a SoCamera node
*/

SbBool
SoListenerOrientationElement::isSetByListener(SoState * const state)
{
  const SoListenerOrientationElement * elem =
    coin_assert_cast<const SoListenerOrientationElement *>
    (
     SoElement::getConstElement(state, classStackIndex)
     );
  return elem->setbylistener;
}

//! Prints the contents of the element (unimplemented)

void
SoListenerOrientationElement::print(FILE * /* file */) const
{
}
