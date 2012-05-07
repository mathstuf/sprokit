/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef VISTK_PROCESSES_SCORING_SCORE_AGGREGATION_PROCESS_H
#define VISTK_PROCESSES_SCORING_SCORE_AGGREGATION_PROCESS_H

#include "scoring-config.h"

#include <vistk/pipeline/process.h>

#include <boost/scoped_ptr.hpp>

/**
 * \file score_aggregation_process.h
 *
 * \brief A process which generates an aggregate score.
 */

namespace vistk
{

/**
 * \class score_aggregation_process
 *
 * \brief A process which generates an aggregate score.
 *
 * \process Aggregate scores from a scoring process.
 *
 * \iports
 *
 * \iport{score} The score.
 *
 * \configs
 *
 * \config{path} The path to the file to write resulting scores to.
 *
 * \reqs
 *
 * \req The input port \port{score} must be connected.
 */
class VISTK_PROCESSES_SCORING_NO_EXPORT score_aggregation_process
  : public process
{
  public:
    /**
     * \brief Constructor.
     *
     * \param config Contains config for the process.
     */
    score_aggregation_process(config_t const& config);
    /**
     * \brief Destructor.
     */
    ~score_aggregation_process();
  protected:
    /**
     * \brief Configure the subclass.
     */
    void _configure();

    /**
     * \brief Step the subclass.
     */
    void _step();
  private:
    class priv;
    boost::scoped_ptr<priv> d;
};

}

#endif // VISTK_PROCESSES_SCORING_SCORE_AGGREGATION_PROCESS_H
