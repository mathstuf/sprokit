/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef VISTK_PROCESSES_IMAGE_CROP_IMAGE_PROCESS_H
#define VISTK_PROCESSES_IMAGE_CROP_IMAGE_PROCESS_H

#include "image-config.h"

#include <vistk/pipeline/process.h>

namespace vistk
{

/**
 * \class crop_image_process
 *
 * \brief A process which crops an image to a specific size.
 *
 * \process A process for cropping images.
 *
 * \iports
 *
 * \iport{image} An image to crop.
 *
 * \oports
 *
 * \oport{cropimage} The cropped image.
 *
 * \configs
 *
 * \config{pixtype} The type of image to convert.
 * \config{grayscale} Whether the inputs are grayscale or not.
 * \config{x_offset} The x offset to crop at.
 * \config{y_offset} The y offset to crop at.
 * \config{width} The width of the resulting image.
 * \config{height} The height of the resulting image.
 *
 * \reqs
 *
 * \req The \port{image} and \port{cropimage} ports must be connected.
 * \req The \key{x_offset}, \key{y_offset}, \key{width}, and \key{height}
 *      configurations must be set.
 */
class VISTK_PROCESSES_IMAGE_NO_EXPORT crop_image_process
  : public process
{
  public:
    /**
     * \brief Constructor.
     *
     * \param config Contains config for the process.
     */
    crop_image_process(config_t const& config);
    /**
     * \brief Destructor.
     */
    ~crop_image_process();
  protected:
    /**
     * \brief Initialize the process.
     */
    void _init();

    /**
     * \brief Crops an image.
     */
    void _step();
  private:
    class priv;
    boost::shared_ptr<priv> d;
};

}

#endif // VISTK_PROCESSES_IMAGE_CROP_IMAGE_PROCESS_H