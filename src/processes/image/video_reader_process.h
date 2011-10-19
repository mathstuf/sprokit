/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef VISTK_PROCESSES_IMAGE_VIDEO_READER_PROCESS_H
#define VISTK_PROCESSES_IMAGE_VIDEO_READER_PROCESS_H

#include "image-config.h"

#include <vistk/pipeline/process.h>

#include <boost/scoped_ptr.hpp>

/**
 * \file video_reader_process.h
 *
 * \brief Declaration of the video reader process.
 */

namespace vistk
{

/**
 * \class video_reader_process
 *
 * \brief A process for reading frames from a file.
 *
 * \process A process for reading video frames from a file.
 *
 * \iports
 *
 * \iport{color} The color to use for the output stamps.
 *
 * \oports
 *
 * \oport{image} The next frame read from the file.
 *
 * \configs
 *
 * \config{pixtype} The pixel type of the input images.
 * \config{pixfmt} The pixel format of the input images.
 * \config{input} The file to read the video from.
 * \config{verify} Verify frames during initialization.
 * \config{impl} The implementation of readers to support.
 *
 * \reqs
 *
 * \req The \port{image} output must be connected.
 * \req The \key{input} configuration must be a valid filepath.
 */
class VISTK_PROCESSES_IMAGE_NO_EXPORT video_reader_process
  : public process
{
  public:
    /**
     * \brief Constructor.
     *
     * \param config Contains config for the process.
     */
    video_reader_process(config_t const& config);
    /**
     * \brief Destructor.
     */
    ~video_reader_process();
  protected:
    /**
     * \brief Checks the output port connections and the configuration.
     */
    void _init();

    /**
     * \brief Pushes the next video through the output port.
     */
    void _step();
  private:
    class priv;
    boost::scoped_ptr<priv> d;
};

}

#endif // VISTK_PROCESSES_IMAGE_VIDEO_READER_PROCESS_H