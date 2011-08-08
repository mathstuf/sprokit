/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <vistk/pipeline/schedule.h>
#include <vistk/pipeline/schedule_exception.h>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

/**
 * \file schedule.cxx
 *
 * \brief Python bindings for \link vistk::schedule\endlink.
 */

using namespace boost::python;

static void translator(vistk::schedule_exception const& e);

class wrap_schedule
  : public vistk::schedule
  , public wrapper<vistk::schedule>
{
  public:
    wrap_schedule(vistk::config_t const& config, vistk::pipeline_t const& pipeline);
    ~wrap_schedule();

    void start();
    void stop();
};

BOOST_PYTHON_MODULE(schedule)
{
  register_exception_translator<
    vistk::schedule_exception>(translator);

  class_<wrap_schedule, boost::noncopyable>("Schedule", no_init)
    .def(init<vistk::config_t, vistk::pipeline_t>())
    .def("start", pure_virtual(&vistk::schedule::start))
    .def("stop", pure_virtual(&vistk::schedule::stop))
  ;
}

void
translator(vistk::schedule_exception const& e)
{
  PyErr_SetString(PyExc_RuntimeError, e.what());
}

wrap_schedule
::wrap_schedule(vistk::config_t const& config, vistk::pipeline_t const& pipeline)
  : vistk::schedule(config, pipeline)
{
}

wrap_schedule
::~wrap_schedule()
{
}

void
wrap_schedule
::start()
{
  this->get_override("start")();
}

void
wrap_schedule
::stop()
{
  this->get_override("stop")();
}