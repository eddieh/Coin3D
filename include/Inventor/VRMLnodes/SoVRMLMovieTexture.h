/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifndef COIN_SOVRMLMOVIETEXTURE_H
#define COIN_SOVRMLMOVIETEXTURE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLTexture.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoMFString.h>

class SoVRMLMovieTextureP;

class COIN_DLL_API SoVRMLMovieTexture : public SoVRMLTexture
{
  typedef SoVRMLTexture inherited;
  SO_NODE_HEADER(SoVRMLMovieTexture);

public:
  static void initClass(void);
  SoVRMLMovieTexture(void);

  SoSFBool loop;
  SoSFFloat speed;
  SoSFTime startTime;
  SoSFTime stopTime;
  SoMFString url;
  SoSFTime duration_changed;
  SoSFBool isActive;

#ifndef COIN_BETA_VERSION
#error Consider adding fields "alpha", "model", "blendColor" and "scaleTexCoord" to be compatible with TGS OIV >= 4.0.
#endif // COIN_BETA_VERSION

  virtual void GLRender( SoGLRenderAction * action );

protected:
  virtual ~SoVRMLMovieTexture();

private:
  SoVRMLMovieTextureP * pimpl;
};

#endif // ! COIN_SOVRMLMOVIETEXTURE_H
