/*ckwg +29
 * Copyright 2011-2012 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SPROKIT_PIPELINE_TYPES_H
#define SPROKIT_PIPELINE_TYPES_H

#include "pipeline-config.h"

#include <boost/shared_ptr.hpp>

#include <exception>
#include <string>

/**
 * \file types.h
 *
 * \brief Common types used in the pipeline library.
 */

/**
 * \brief The namespace for all sprokit-related symbols.
 */
namespace sprokit
{

/**
 * \defgroup base_classes Base classes for the pipeline.
 * \defgroup registries Registries of different types of pipeline objects.
 * \defgroup exceptions Exceptions thrown within the pipeline.
 */

class config;
/// A typedef used to handle \link config configurations\endlink.
typedef boost::shared_ptr<config> config_t;

class datum;
/// A typedef used to handle \link datum edge data\endlink.
typedef boost::shared_ptr<datum const> datum_t;

class edge;
/// A typedef used to handle \link edge edges\endlink.
typedef boost::shared_ptr<edge> edge_t;

class pipeline;
/// A typedef used to handle \link pipeline pipelines\endlink.
typedef boost::shared_ptr<pipeline> pipeline_t;

class process;
/// A typedef used to handle \link process processes\endlink.
typedef boost::shared_ptr<process> process_t;

class process_cluster;
/// A typedef used to handle \link process_cluster process clusters\endlink.
typedef boost::shared_ptr<process_cluster> process_cluster_t;

class process_registry;
/// A typedef used to handle \link process_registry process registries\endlink.
typedef boost::shared_ptr<process_registry> process_registry_t;

class scheduler;
/// A typedef used to handle \link scheduler schedulers\endlink.
typedef boost::shared_ptr<scheduler> scheduler_t;

class scheduler_registry;
/// A typedef used to handle \link scheduler_registry scheduler registries\endlink.
typedef boost::shared_ptr<scheduler_registry> scheduler_registry_t;

class stamp;
/// A typedef used to handle \link stamp stamps\endlink.
typedef boost::shared_ptr<stamp const> stamp_t;

/**
 * \class pipeline_exception types.h <sprokit/pipeline/types.h>
 *
 * \brief The base of all exceptions thrown within the pipeline.
 *
 * \ingroup exceptions
 */
class SPROKIT_PIPELINE_EXPORT pipeline_exception
  : public std::exception
{
  public:
    /**
     * \brief Constructor.
     */
    pipeline_exception() throw();
    /**
     * \brief Destructor.
     */
    virtual ~pipeline_exception() throw();

    /**
     * \brief A description of the exception.
     *
     * \returns A string describing what went wrong.
     */
    char const* what() const throw();
  protected:
    /// The text of the exception.
    std::string m_what;
};

}

#endif // SPROKIT_PIPELINE_TYPES_H
