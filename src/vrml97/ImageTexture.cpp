/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_VRML97

/*!
  \class SoVRMLImageTexture SoVRMLImageTexture.h Inventor/VRMLnodes/SoVRMLImageTexture.h
  \brief The SoVRMLImageTexture class is used for mapping a texture file onto geometry.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  ImageTexture {
    exposedField MFString url     []
    field        SFBool   repeatS TRUE
    field        SFBool   repeatT TRUE
  }
  \endverbatim

  The ImageTexture node defines a texture map by specifying an image
  file and general parameters for mapping to geometry. Texture maps
  are defined in a 2D coordinate system (s, t) that ranges from [0.0,
  1.0] in both directions. The bottom edge of the image corresponds to
  the S-axis of the texture map, and left edge of the image
  corresponds to the T-axis of the texture map. The lower-left pixel
  of the image corresponds to s=0, t=0, and the top-right pixel of the
  image corresponds to s=1, t=1. These relationships are depicted in
  Figure 6.9.

  <center>
  <img src="http://www.web3d.org/x3d/specifications/vrml/ISO-IEC-14772-IS-VRML97WithAmendment1/Images/ImageTexture.gif">
  Figure 6.9
  </center>

  The texture is read from the URL specified by the url field. When
  the url field contains no values ([]), texturing is
  disabled. Browsers shall support the JPEG and PNG image file
  formats. In addition, browsers may support other image formats
  (e.g. CGM) which can be rendered into a 2D image. Support for the
  GIF format is also recommended (including transparency).

  Details on the url field can be found in 4.5, VRML and the World
  Wide Web.

  See 4.6.11, Texture maps
  (<http://www.web3d.org/x3d/specifications/vrml/ISO-IEC-14772-IS-VRML97WithAmendment1/part1/concepts.html#4.6.11>),
  for a general description of texture maps.

  See 4.14, Lighting model
  (<http://www.web3d.org/x3d/specifications/vrml/ISO-IEC-14772-IS-VRML97WithAmendment1/part1/concepts.html#4.14>),
  for a description of lighting equations and the interaction between
  textures, materials, and geometry appearance.

  The repeatS and repeatT fields specify how the texture wraps in the
  S and T directions. If repeatS is TRUE (the default), the texture
  map is repeated outside the [0.0, 1.0] texture coordinate range in
  the S direction so that it fills the shape. If repeatS is FALSE, the
  texture coordinates are clamped in the S direction to lie within the
  [0.0, 1.0] range. The repeatT field is analogous to the repeatS
  field.

  One common flaw with many programs that has support for exporting
  VRML or Inventor files, is that the same texture file is exported
  several times, but as different nodes. This can cause excessive
  texture memory usage and slow rendering. Below is an example program
  that fixes this by replacing all instances of the same texture with
  a pointer to the first node:

  \code

  #include <Inventor/actions/SoSearchAction.h>
  #include <Inventor/actions/SoWriteAction.h>
  #include <Inventor/VRMLnodes/SoVRMLGroup.h>
  #include <Inventor/VRMLnodes/SoVRMLImageTexture.h>
  #include <Inventor/VRMLnodes/SoVRMLAppearance.h>
  #include <Inventor/SoDB.h>
  #include <Inventor/SoInput.h>
  #include <Inventor/SoOutput.h>
  #include <assert.h>

  int main(int argc, char ** argv)
  {
    if (argc < 2) return -1;
    SoDB::init();
  
    SoInput in;
    if (!in.openFile(argv[1])) return -1;

    if (!in.isFileVRML2()) return -1; // file is not a vrml2 file 

    SoVRMLGroup * root = SoDB::readAllVRML(&in);

    if (!root) return -1;
    root->ref();

    SoSearchAction sa;
    sa.setType(SoVRMLImageTexture::getClassTypeId());
    sa.setInterest(SoSearchAction::ALL);
    sa.setSearchingAll(TRUE);
    sa.apply(root);
    SoPathList & pl = sa.getPaths();
    SbDict namedict;

    for (int i = 0; i < pl.getLength(); i++) {
      SoFullPath * p = (SoFullPath*) pl[i];
      if (p->getTail()->isOfType(SoVRMLImageTexture::getClassTypeId())) {
        SoVRMLImageTexture * tex = (SoVRMLImageTexture*) p->getTail();
        if (tex->url.getNum()) {
          // FIXME: we only check the first name here. Should really check all of them
          SbName name = tex->url[0].getString();
          unsigned long key = (unsigned long) ((void*) name.getString());
          void * tmp;
          if (!namedict.find(key, tmp)) {
            (void) namedict.enter(key, tex);
          }
          else if (tmp != (void*) tex) {
            SoNode * parent = p->getNodeFromTail(1);
            if (parent->isOfType(SoVRMLAppearance::getClassTypeId())) {
              ((SoVRMLAppearance*)parent)->texture = (SoNode*) tmp;
            }
            else {
              // not a valid VRML2 file. Print a warning or something.
            }
          }
        }
      }
    }   
    sa.reset();
    SoOutput out;
    out.setHeaderString("#VRML V2.0 utf8");
    SoWriteAction wa(&out);
    wa.apply(root);
    root->unref();
  }
  \endcode
*/

/*!
  SoMFString SoVRMLImageTexture::url
  The texture file URL.
*/

#include <Inventor/VRMLnodes/SoVRMLImageTexture.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureScalePolicyElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/misc/SoGLBigImage.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/SbImage.h>
#include <Inventor/SoInput.h>
#include <Inventor/C/tidbitsp.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <assert.h>
#include <Inventor/C/glue/simage_wrapper.h>

static int imagedata_maxage = 0;
static VRMLPrequalifyFileCallback * imagetexture_prequalify_cb = NULL;
static void * imagetexture_prequalify_closure = NULL;
static SbBool imagetexture_delay_fetch = TRUE;
static SbBool imagetexture_is_exiting = FALSE;

#ifdef COIN_THREADSAFE

#include <Inventor/C/threads/sched.h>
#include <Inventor/threads/SbMutex.h>

static cc_sched * imagetexture_scheduler = NULL;

#endif // COIN_THREADSAFE

class SoVRMLImageTextureP {
public:
  SoVRMLImageTextureP(SoVRMLImageTexture * master) : master(master) { }
  SoVRMLImageTexture * master;
  
  int readstatus;
  class SoGLImage * glimage;
  SbBool glimagevalid;
  SbImage image;
  SoFieldSensor * urlsensor;
  SbBool allowprequalifycb;

  SoTimerSensor * timersensor;
  SbBool finishedloading;
  static void timersensor_cb(void * data, SoSensor * sensor);

  void readimage_cleanup(void);
  SbBool isdestructing;
#ifdef COIN_THREADSAFE
  SbMutex glimagemutex;
#endif // COIN_THREADSAFE
  SbStringList searchdirs;

  void clearSearchDirs(void) {
    int n = this->searchdirs.getLength();
    for (int i = 0; i < n; i++) {
      delete this->searchdirs[i];
    }
    this->searchdirs.truncate(0);
  }
  void setSearchDirs(const SbStringList & sl) {
    this->clearSearchDirs();
    int n = sl.getLength();
    for (int i = 0; i < n; i++) {
      this->searchdirs.append(new SbString(*sl[i]));
    }
  }
};

#ifdef COIN_THREADSAFE
#define LOCK_GLIMAGE(_thisp_) (_thisp_)->pimpl->glimagemutex.lock()
#define UNLOCK_GLIMAGE(_thisp_) (_thisp_)->pimpl->glimagemutex.unlock()
#else // COIN_THREADSAFE
#define LOCK_GLIMAGE(_thisp_)
#define UNLOCK_GLIMAGE(_thisp_)
#endif // COIN_THREADSAFE

SO_NODE_SOURCE(SoVRMLImageTexture);

static void
imagetexture_cleanup(void)
{
#ifdef COIN_THREADSAFE
  imagetexture_is_exiting = TRUE;
  if (imagetexture_scheduler) {
    cc_sched_destruct(imagetexture_scheduler);
  }
#endif // COIN_THREADSAFE
}

// Doc in parent
void
SoVRMLImageTexture::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLImageTexture, SO_VRML97_NODE_TYPE);
  imagedata_maxage = 500;

#ifdef COIN_THREADSAFE
  imagetexture_scheduler = cc_sched_construct(1);
  coin_atexit((coin_atexit_f*) imagetexture_cleanup, 0);
#endif // COIN_THREADSAFE
}

#define PRIVATE(x) (x)->pimpl

/*!
  Constructor.
*/
SoVRMLImageTexture::SoVRMLImageTexture(void)
{
  PRIVATE(this) = new SoVRMLImageTextureP(this);

  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLImageTexture);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(url);

  PRIVATE(this)->glimage = NULL;
  PRIVATE(this)->glimagevalid = FALSE;
  PRIVATE(this)->readstatus = 1;
  PRIVATE(this)->allowprequalifycb = TRUE;
  PRIVATE(this)->timersensor = 
    new SoTimerSensor(SoVRMLImageTextureP::timersensor_cb, PRIVATE(this));
  PRIVATE(this)->timersensor->setInterval(SbTime(1.0));
  
  // use field sensor for url since we will load an image if
  // filename changes. This is a time-consuming task which should
  // not be done in notify().
  PRIVATE(this)->urlsensor = new SoFieldSensor(urlSensorCB, this);
  PRIVATE(this)->urlsensor->setPriority(0);
  PRIVATE(this)->urlsensor->attach(&this->url);
  PRIVATE(this)->isdestructing = FALSE;
}

/*!
  Destructor.
*/
SoVRMLImageTexture::~SoVRMLImageTexture()
{
  delete PRIVATE(this)->timersensor;
#ifdef COIN_THREADSAFE
  // just wait for all threads to finish reading
  if (imagetexture_scheduler) {
    PRIVATE(this)->isdestructing = TRUE; // signal thread that we are destructing
    cc_sched_wait_all(imagetexture_scheduler);
  }
#endif // COIN_THREADSAFE  

  if (PRIVATE(this)->glimage) PRIVATE(this)->glimage->unref(NULL);
  PRIVATE(this)->clearSearchDirs();
  delete PRIVATE(this)->urlsensor;
  delete PRIVATE(this);
}

/*!
  Sets the prequalify callback for ImageTexture nodes. This is a callback
  that will be called when an image is about to be read.
*/
void
SoVRMLImageTexture::setPrequalifyFileCallBack(VRMLPrequalifyFileCallback * cb,
                                              void * closure)
{
  imagetexture_prequalify_cb = cb;
  imagetexture_prequalify_closure = closure;
}

/*!
  Sets whether the image loading is delayed until the first time the
  image is needed, or if the image is loaded immediately when the
  url field is changed/set. Default value is \e TRUE.
*/
void 
SoVRMLImageTexture::setDelayFetchURL(const SbBool onoff)
{
  imagetexture_delay_fetch = onoff;
}

/*!
  Enable prequalify file loading.
*/
void
SoVRMLImageTexture::allowPrequalifyFile(SbBool enable)
{
  PRIVATE(this)->allowprequalifycb = enable;
}

static SoGLImage::Wrap
imagetexture_translate_wrap(const SbBool repeat)
{
  if (repeat) return SoGLImage::REPEAT;
  return SoGLImage::CLAMP_TO_EDGE;
}

// Doc in parent
void
SoVRMLImageTexture::doAction(SoAction * action)
{
  SoState * state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  SoTexture3EnabledElement::set(state, this, FALSE);
  if (!PRIVATE(this)->image.hasData()) {
    if (this->url.getNum()) {
      // texture has not been loaded yet. Supply a dummy image and
      // enable SoTextureImageElement so that texture coordinates are
      // generated in generatePrimitives()
      SoTextureEnabledElement::set(state, this, TRUE);    
      static unsigned char dummydata[] = { 0xff, 0xff, 0xff, 0xff };
      SoTextureImageElement::set(state, this,
                                 SbVec2s(2,2), 1, dummydata,
                                 imagetexture_translate_wrap(this->repeatS.getValue()),
                                 imagetexture_translate_wrap(this->repeatT.getValue()),                                                           
                                 SoTextureImageElement::MODULATE,
                                 SbColor(1.0f, 1.0f, 1.0f));
      
    }
    else {
      SoTextureEnabledElement::set(state, this, FALSE);    
    }    
  }
  else {
    int nc;
    SbVec2s size;
    const unsigned char * bytes = PRIVATE(this)->image.getValue(size, nc);
    SoTextureImageElement::set(state, this,
                               size, nc, bytes,
                               imagetexture_translate_wrap(this->repeatS.getValue()),
                               imagetexture_translate_wrap(this->repeatT.getValue()),                                                           
                               SoTextureImageElement::MODULATE,
                               SbColor(1.0f, 1.0f, 1.0f));
    SoTextureEnabledElement::set(state, this, TRUE);
  }
  if (this->isOverride()) {
    SoTextureOverrideElement::setImageOverride(state, TRUE);
  }
}

void 
SoVRMLImageTexture::rayPick(SoRayPickAction * action)
{
  SoVRMLImageTexture::doAction(action);
}

// Doc in parent
void
SoVRMLImageTexture::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  SoGLTexture3EnabledElement::set(state, this, FALSE);
  float quality = SoTextureQualityElement::get(state);

  LOCK_GLIMAGE(this);
  
  if (!PRIVATE(this)->glimagevalid) {
    SoTextureScalePolicyElement::Policy scalepolicy =
      SoTextureScalePolicyElement::get(state);      
    SbBool needbig = (scalepolicy == SoTextureScalePolicyElement::FRACTURE);

    if (needbig &&
        (PRIVATE(this)->glimage == NULL ||
         PRIVATE(this)->glimage->getTypeId() != SoGLBigImage::getClassTypeId())) {
      if (PRIVATE(this)->glimage) {
        PRIVATE(this)->glimage->setEndFrameCallback(NULL, NULL);
        PRIVATE(this)->glimage->unref(state);
      }
      PRIVATE(this)->glimage = new SoGLBigImage();
    }
    else if (!needbig &&
             (PRIVATE(this)->glimage == NULL ||
              PRIVATE(this)->glimage->getTypeId() != SoGLImage::getClassTypeId())) {
      if (PRIVATE(this)->glimage) {
        PRIVATE(this)->glimage->setEndFrameCallback(NULL, NULL);
        PRIVATE(this)->glimage->unref(state);
      }
      PRIVATE(this)->glimage = new SoGLImage();
    }

    if (scalepolicy == SoTextureScalePolicyElement::SCALE_DOWN) {
      PRIVATE(this)->glimage->setFlags(PRIVATE(this)->glimage->getFlags()|SoGLImage::SCALE_DOWN);
    }

    PRIVATE(this)->glimagevalid = TRUE;
    PRIVATE(this)->glimage->setData(&PRIVATE(this)->image,
                                    imagetexture_translate_wrap(this->repeatS.getValue()),
                                    imagetexture_translate_wrap(this->repeatT.getValue()),
                                    quality);
    // don't cache while creating a texture object
    SoCacheElement::setInvalid(TRUE);
    if (state->isCacheOpen()) {
      SoCacheElement::invalidate(state);
    }
  }

  if (PRIVATE(this)->glimage && PRIVATE(this)->glimage->getTypeId() == SoGLBigImage::getClassTypeId()) {
    SoCacheElement::invalidate(state);
  }

  UNLOCK_GLIMAGE(this);

  SoGLTextureImageElement::set(state, this,
                               PRIVATE(this)->glimagevalid ? PRIVATE(this)->glimage : NULL,
                               SoTextureImageElement::MODULATE,
                               SbColor(1.0f, 1.0f, 1.0f));

  SbBool enable = PRIVATE(this)->glimagevalid &&
    quality > 0.0f &&
    PRIVATE(this)->glimage->getImage() &&
    PRIVATE(this)->glimage->getImage()->hasData();

  SoGLTextureEnabledElement::set(state,
                                 this,
                                 enable);

  if (this->isOverride()) {
    SoTextureOverrideElement::setImageOverride(state, TRUE);
  }
}

// Doc in parent
void
SoVRMLImageTexture::callback(SoCallbackAction * action)
{
  SoVRMLImageTexture::doAction(action);
}

// Doc in parent
SbBool
SoVRMLImageTexture::readInstance(SoInput * in,
                                 unsigned short flags)
{
  PRIVATE(this)->urlsensor->detach();
  SbBool ret = inherited::readInstance(in, flags);
  this->setReadStatus((int) ret);
  if (ret) {
    // need to copy the SoInput directories, so that the texture is
    // found again if it's thrown out of memory (can happen when it's
    // a long time since it has been used)
    PRIVATE(this)->setSearchDirs(SoInput::getDirectories());
    if (!this->loadUrl()) {
      SoReadError::post(in, "Could not read texture file: %s",
                        url[0].getString());
      this->setReadStatus(FALSE);
    }
  }
  PRIVATE(this)->urlsensor->attach(&this->url);
  return ret;
}

/*!
  Returns the read status.
*/
int
SoVRMLImageTexture::getReadStatus(void) const
{
  return PRIVATE(this)->readstatus;
}

/*!
  Sets the read status.
*/
void
SoVRMLImageTexture::setReadStatus(int status)
{
  PRIVATE(this)->readstatus = status;
}

//
// Called from readInstance() or when user changes the
// filename field.
//
SbBool
SoVRMLImageTexture::loadUrl(void)
{
  PRIVATE(this)->glimagevalid = FALSE; // recreate GL image in next GLRender()

  SbBool retval = TRUE;
  if (this->url.getNum() && this->url[0].getLength()) {
    const SbStringList & sl = PRIVATE(this)->searchdirs;
    if (sl.getLength() == 0) { // will be empty if the node isn't read but created in C++
      PRIVATE(this)->setSearchDirs(SoInput::getDirectories());
    }
    if (imagetexture_delay_fetch) {
      // instruct SbImage to call image_read_cb the first time the image
      // data is requested (typically when some shape using the texture
      // is inside the view frustum).
      retval = PRIVATE(this)->image.scheduleReadFile(image_read_cb, this,
                                                     this->url[0],
                                                     sl.getArrayPtr(), sl.getLength());
      
    }
    else {
      SbString filename = SbImage::searchForFile(this->url[0],
                                                 sl.getArrayPtr(), sl.getLength());
      retval = this->readImage(filename);
    }
  }
  else {
    retval = TRUE;
  }
  return retval;
}

//
// used for multithread loading.
//
void
SoVRMLImageTexture::glimage_callback(void * closure)
{
#ifdef COIN_THREADSAFE
  SoVRMLImageTexture * thisp = (SoVRMLImageTexture*) closure;
  if (PRIVATE(thisp)->glimage) {
    int age = PRIVATE(thisp)->glimage->getNumFramesSinceUsed();
    if (age > imagedata_maxage) {
      PRIVATE(thisp)->glimagevalid = FALSE;
      assert(PRIVATE(thisp)->glimage);
      PRIVATE(thisp)->glimage->setEndFrameCallback(NULL, NULL);
      PRIVATE(thisp)->image.setValue(SbVec2s(0,0), 0, NULL);
      (void) thisp->loadUrl();
    }
  }
#endif // COIN_THREADSAFE
}

SbBool
SoVRMLImageTexture::default_prequalify_cb(const SbString & url,  void * closure, 
                                          SoVRMLImageTexture * thisp)
{
  SbBool ret = TRUE;
  if (!imagetexture_is_exiting && !PRIVATE(thisp)->isdestructing) {
    ret = PRIVATE(thisp)->image.readFile(url);
  }
  return ret;
}

// needed to pass data to a new thread
class imagetexture_thread_data {
public:
  SoVRMLImageTexture * thisp;
  SbString filename;
};

//
// multithread loading thread.
//
void
SoVRMLImageTexture::read_thread(void * closure)
{
  imagetexture_thread_data * data = (imagetexture_thread_data*) closure;
  data->thisp->readImage(data->filename);
  // we allocated this before staring the thread
  delete data;
}

// callback for SoOneShotSensor which is used to read image when
// Coin is compiled without the threads module.
void
SoVRMLImageTexture::oneshot_readimage_cb(void * closure, SoSensor * sensor)
{
  imagetexture_thread_data * data = (imagetexture_thread_data*) closure;
  data->thisp->readImage(data->filename);
  // delete both the sensor and the data
  delete sensor;
  delete data;
}

//
// called (from SbImage) when image data is needed.
//
SbBool
SoVRMLImageTexture::image_read_cb(const SbString & filename, SbImage * image, void * closure)
{
  SoVRMLImageTexture * thisp = (SoVRMLImageTexture*) closure;
  assert(&PRIVATE(thisp)->image == image);
  assert(PRIVATE(thisp)->glimage);
  if (!PRIVATE(thisp)->glimage) {
    return FALSE;
  }
  
  // start a timer sensor that triggers each second and tests if the
  // thread that loads images has finished loading this image.
  PRIVATE(thisp)->finishedloading = FALSE; // this will be TRUE when finished
  PRIVATE(thisp)->timersensor->schedule();

  imagetexture_thread_data * data = new imagetexture_thread_data;
  data->thisp = thisp;
  data->filename = filename;

#if defined(COIN_THREADSAFE)
  // use a separate thread to load the image  
  cc_sched_schedule(imagetexture_scheduler,
                    read_thread, data, 0);
  return TRUE;
#else // COIN_THREADSAFE
  // trigger a sensor to read the image
  SoOneShotSensor * sensor = new SoOneShotSensor(oneshot_readimage_cb, data);
  sensor->schedule();
  return TRUE;
#endif // ! COIN_THREADSAFE
}

//
// called when filename changes
//
void
SoVRMLImageTexture::urlSensorCB(void * data, SoSensor *)
{
  SoVRMLImageTexture * thisp = (SoVRMLImageTexture*) data;

  thisp->setReadStatus(1);
  if (thisp->url.getNum() && thisp->url[0].getLength() &&
      !thisp->loadUrl()) {
    SoDebugError::postWarning("SoVRMLImageTexture::urlSensorCB",
                              "Image file could not be read: %s",
                              thisp->url[0].getString());
    thisp->setReadStatus(0);
  }
  else { // empty image?
    if (thisp->url.getNum() == 0 || thisp->url[0].getLength() == 0) {
      // wait for threads to finish in case a new thread is used to
      // load the previous image, and the thread has not finished yet.
#ifdef COIN_THREADSAFE
      cc_sched_wait_all(imagetexture_scheduler);
#endif // COIN_THREADSAFE
      thisp->pimpl->image.setValue(SbVec2s(0,0), 0, NULL);
      thisp->pimpl->glimagevalid = FALSE;
      if (PRIVATE(thisp)->glimage) {
        PRIVATE(thisp)->glimage->setEndFrameCallback(NULL, NULL);
      }
    }
  }
}

// helper function that either loads the image using the default
// loader, or calls the prequalify callback
SbBool 
SoVRMLImageTexture::readImage(const SbString & filename)
{
  SbBool retval = TRUE;
  if (PRIVATE(this)->allowprequalifycb && imagetexture_prequalify_cb) {
    retval = imagetexture_prequalify_cb(filename, imagetexture_prequalify_closure,
                                        this);
  }
  else {
    retval = default_prequalify_cb(filename, NULL, this); 
  }
  if (PRIVATE(this)->glimage) {
    PRIVATE(this)->glimage->setEndFrameCallback(glimage_callback, this);
    PRIVATE(this)->glimagevalid = FALSE;
  }
  // set flag that timer sensor will test. 
  PRIVATE(this)->finishedloading = TRUE;
  return retval;
}

/*!  
  Set the image data for this node. Can be used by the prequalify
  callback to set the data in the node.  
*/
void 
SoVRMLImageTexture::setImage(const SbImage & image)
{
  PRIVATE(this)->image = image;
  PRIVATE(this)->glimagevalid = FALSE;
  this->touch(); // destroy caches using this node
}

/*!
  Returns the texture image.
*/
const SbImage *
SoVRMLImageTexture::getImage(void) const
{
  return &PRIVATE(this)->image;
}

/*!
  \COININTERNAL
*/
void
SoVRMLImageTexture::setImageDataMaxAge(const uint32_t maxage)
{
  imagedata_maxage = maxage;
}

void 
SoVRMLImageTextureP::timersensor_cb(void * data, SoSensor * sensor)
{
  SoVRMLImageTextureP * thisp = (SoVRMLImageTextureP*) data;
  
  if (thisp->finishedloading) {
    thisp->master->touch(); // trigger redraw
    thisp->timersensor->unschedule();
  }
}

#undef PRIVATE

#undef LOCK_GLIMAGE
#undef UNLOCK_GLIMAGE

#endif // HAVE_VRML97
