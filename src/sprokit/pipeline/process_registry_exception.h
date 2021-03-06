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

#ifndef SPROKIT_PIPELINE_PROCESS_REGISTRY_EXCEPTION_H
#define SPROKIT_PIPELINE_PROCESS_REGISTRY_EXCEPTION_H

#include "pipeline-config.h"

#include "process.h"
#include "types.h"

/**
 * \file process_registry_exception.h
 *
 * \brief Header for exceptions used within the \link sprokit::process_registry process registry\endlink.
 */

namespace sprokit
{

/**
 * \class process_registry_exception process_registry_exception.h <sprokit/pipeline/process_registry_exception.h>
 *
 * \brief The base class for all exceptions thrown from a \ref process_registry.
 *
 * \ingroup exceptions
 */
class SPROKIT_PIPELINE_EXPORT process_registry_exception
  : public pipeline_exception
{
  public:
    /**
     * \brief Constructor.
     */
    process_registry_exception() throw();
    /**
     * \brief Destructor.
     */
    virtual ~process_registry_exception() throw();
};

/**
 * \class null_process_ctor_exception process_registry_exception.h <sprokit/pipeline/process_registry_exception.h>
 *
 * \brief Thrown when a \c NULL constructor function is added to the registry.
 *
 * \ingroup exceptions
 */
class SPROKIT_PIPELINE_EXPORT null_process_ctor_exception
  : public process_registry_exception
{
  public:
    /**
     * \brief Constructor.
     *
     * \param type The type the ctor is for.
     */
    null_process_ctor_exception(process::type_t const& type) throw();
    /**
     * \brief Destructor.
     */
    ~null_process_ctor_exception() throw();

    /// The type that was passed a \c NULL constructor.
    process::type_t const m_type;
};

/**
 * \class null_process_registry_config_exception process_registry_exception.h <sprokit/pipeline/process_registry_exception.h>
 *
 * \brief Thrown when a \c NULL \ref config is passed to a process.
 *
 * \ingroup exceptions
 */
class SPROKIT_PIPELINE_EXPORT null_process_registry_config_exception
  : public process_registry_exception
{
  public:
    /**
     * \brief Constructor.
     */
    null_process_registry_config_exception() throw();
    /**
     * \brief Destructor.
     */
    ~null_process_registry_config_exception() throw();
};

/**
 * \class no_such_process_type_exception process_registry_exception.h <sprokit/pipeline/process_registry_exception.h>
 *
 * \brief Thrown when a type is requested, but does not exist in the registry.
 *
 * \ingroup exceptions
 */
class SPROKIT_PIPELINE_EXPORT no_such_process_type_exception
  : public process_registry_exception
{
  public:
    /**
     * \brief Constructor.
     *
     * \param type The type requested.
     */
    no_such_process_type_exception(process::type_t const& type) throw();
    /**
     * \brief Destructor.
     */
    ~no_such_process_type_exception() throw();

    /// The type that was requested from the \link process_registry process registry\endlink.
    process::type_t const m_type;
};

/**
 * \class process_type_already_exists_exception process_registry_exception.h <sprokit/pipeline/process_registry_exception.h>
 *
 * \brief Thrown when a type is added, but does already exists in the registry.
 *
 * \ingroup exceptions
 */
class SPROKIT_PIPELINE_EXPORT process_type_already_exists_exception
  : public process_registry_exception
{
  public:
    /**
     * \brief Constructor.
     *
     * \param type The type requested.
     */
    process_type_already_exists_exception(process::type_t const& type) throw();
    /**
     * \brief Destructor.
     */
    ~process_type_already_exists_exception() throw();

    /// The type that was requested from the \link process_registry process registry\endlink.
    process::type_t const m_type;
};

}

#endif // SPROKIT_PIPELINE_PROCESS_REGISTRY_EXCEPTION_H
