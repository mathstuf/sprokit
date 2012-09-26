/*ckwg +5
 * Copyright 2011-2012 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef VISTK_PROCESSES_UTILITIES_HOMOGRAPHY_READER_PROCESS_H
#define VISTK_PROCESSES_UTILITIES_HOMOGRAPHY_READER_PROCESS_H

#include "utilities-config.h"

#include <vistk/pipeline/process.h>

#include <boost/scoped_ptr.hpp>

/**
 * \file homography_reader_process.h
 *
 * \brief Declaration of the homography reader process.
 */

namespace vistk
{

/**
 * \class homography_reader_process
 *
 * \brief A process for reading homographies from a file.
 *
 * \process Read homographies from a file.
 *
 * \oports
 *
 * \oport{homography} The homography read from the file.
 *
 * \configs
 *
 * \config{input} The path to read homographies from.
 *
 * \reqs
 *
 * \req The \port{homography} output must be connected.
 * \req The \key{input} configuration must be a valid filepath.
 *
 * \ingroup process_utilities
 */
class VISTK_PROCESSES_UTILITIES_NO_EXPORT homography_reader_process
  : public process
{
  public:
    /**
     * \brief Constructor.
     *
     * \param config The configuration for the process.
     */
    homography_reader_process(config_t const& config);
    /**
     * \brief Destructor.
     */
    ~homography_reader_process();
  protected:
    /**
     * \brief Configure the process.
     */
    void _configure();

    /**
     * \brief Step the process.
     */
    void _step();
  private:
    class priv;
    boost::scoped_ptr<priv> d;
};

}

#endif // VISTK_PROCESSES_UTILITIES_HOMOGRAPHY_READER_PROCESS_H