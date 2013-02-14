/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef VISTK_PROCESSES_CLUSTERS_REGISTRATION_H
#define VISTK_PROCESSES_CLUSTERS_REGISTRATION_H

#include "clusters-config.h"

/**
 * \file clusters/registration.h
 *
 * \brief Register processes for use.
 */

extern "C"
{

/**
 * \brief Register processes.
 */
void VISTK_PROCESSES_CLUSTERS_EXPORT register_processes();

}

#endif // VISTK_PROCESSES_CLUSTERS_REGISTRATION_H