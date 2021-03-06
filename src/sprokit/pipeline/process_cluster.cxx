/*ckwg +29
 * Copyright 2012-2013 by Kitware, Inc.
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

#include "process_cluster.h"

#include "pipeline_exception.h"
#include "process_cluster_exception.h"
#include "process_exception.h"
#include "process_registry.h"

#include <boost/foreach.hpp>

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

/**
 * \file process_cluster.cxx
 *
 * \brief Implementation for \link sprokit::process_cluster process cluster\endlink.
 */

namespace sprokit
{

process::property_t const process_cluster::property_cluster = process::property_t("_cluster");

class process_cluster::priv
{
  public:
    priv();
    ~priv();

    typedef std::pair<config::key_t, config::key_t> config_mapping_t;
    typedef std::vector<config_mapping_t> config_mappings_t;
    typedef std::map<process::name_t, config_mappings_t> config_map_t;
    typedef std::map<process::name_t, process_t> process_map_t;

    bool has_name(name_t const& name) const;
    void ensure_name(name_t const& name) const;

    config_map_t config_map;
    process_map_t processes;
    connections_t input_mappings;
    connections_t output_mappings;
    connections_t internal_connections;
};

processes_t
process_cluster
::processes() const
{
  processes_t procs;

  BOOST_FOREACH (priv::process_map_t::value_type const& process_entry, d->processes)
  {
    process_t const& proc = process_entry.second;

    procs.push_back(proc);
  }

  return procs;
}

process::connections_t
process_cluster
::input_mappings() const
{
  return d->input_mappings;
}

process::connections_t
process_cluster
::output_mappings() const
{
  return d->output_mappings;
}

process::connections_t
process_cluster
::internal_connections() const
{
  return d->internal_connections;
}

process_cluster
::process_cluster(config_t const& config)
  : process(config)
  , d(new priv)
{
}

process_cluster
::~process_cluster()
{
}

static process::name_t convert_name(process::name_t const& cluster_name, process::name_t const& process_name);

void
process_cluster
::map_config(config::key_t const& key, name_t const& name_, config::key_t const& mapped_key)
{
  if (d->has_name(name_))
  {
    throw mapping_after_process_exception(name(), key,
                                          name_, mapped_key);
  }

  priv::config_mapping_t const mapping = priv::config_mapping_t(key, mapped_key);

  d->config_map[name_].push_back(mapping);
}

void
process_cluster
::add_process(name_t const& name_, type_t const& type_, config_t const& conf)
{
  if (d->processes.count(name_))
  {
    throw duplicate_process_name_exception(name_);
  }

  typedef std::set<config::key_t> key_set_t;

  config::keys_t const cur_keys = conf->available_values();
  key_set_t ro_keys;

  config_t const new_conf = config::empty_config();

  BOOST_FOREACH (config::key_t const& key, cur_keys)
  {
    config::value_t const value = conf->get_value<config::value_t>(key);

    new_conf->set_value(key, value);

    if (conf->is_read_only(key))
    {
      ro_keys.insert(key);
    }
  }

  priv::config_mappings_t const mappings = d->config_map[name_];

  BOOST_FOREACH (priv::config_mapping_t const& mapping, mappings)
  {
    config::key_t const& key = mapping.first;
    config::key_t const& mapped_key = mapping.second;

    config::value_t const value = config_value<config::value_t>(key);

    if (ro_keys.count(mapped_key))
    {
      config::value_t const new_value = new_conf->get_value<config::value_t>(mapped_key);

      throw mapping_to_read_only_value_exception(name(), key, value, name_, mapped_key, new_value);
    }

    if (new_conf->has_value(mapped_key))
    {
      /// \todo Log a warning.
    }

    new_conf->set_value(mapped_key, value);
    // Make sure that the parameter is not reconfigured away by anything other
    // than this cluster.
    new_conf->mark_read_only(mapped_key);
  }

  BOOST_FOREACH (config::key_t const& key, ro_keys)
  {
    new_conf->mark_read_only(key);
  }

  process_registry_t const reg = process_registry::self();
  name_t const real_name = convert_name(name(), name_);

  process_t const proc = reg->create_process(type_, real_name, new_conf);

  d->processes[name_] = proc;
}

void
process_cluster
::map_input(port_t const& port, name_t const& name_, port_t const& mapped_port)
{
  d->ensure_name(name_);

  process_t const& proc = d->processes[name_];

  if (!proc->input_port_info(mapped_port))
  {
    throw no_such_port_exception(name_, mapped_port);

    return;
  }

  name_t const real_name = convert_name(name(), name_);

  BOOST_FOREACH (connection_t const& input_mapping, d->input_mappings)
  {
    port_addr_t const& process_addr = input_mapping.second;
    name_t const& process_name = process_addr.first;
    port_t const& process_port = process_addr.second;

    if ((process_name == real_name) &&
        (process_port == mapped_port))
    {
      throw port_reconnect_exception(process_name, mapped_port);
    }
  }

  port_addr_t const cluster_addr = port_addr_t(name(), port);
  port_addr_t const mapped_addr = port_addr_t(real_name, mapped_port);

  connection_t const connection = connection_t(cluster_addr, mapped_addr);

  d->input_mappings.push_back(connection);
}

void
process_cluster
::map_output(port_t const& port, name_t const& name_, port_t const& mapped_port)
{
  d->ensure_name(name_);

  /// \todo Make sure that only one process is mapped to a port.

  process_t const& proc = d->processes[name_];

  if (!proc->output_port_info(mapped_port))
  {
    throw no_such_port_exception(name_, mapped_port);

    return;
  }

  BOOST_FOREACH (connection_t const& output_mapping, d->output_mappings)
  {
    port_addr_t const& cluster_addr = output_mapping.second;
    port_t const& cluster_port = cluster_addr.second;

    if (cluster_port == port)
    {
      throw port_reconnect_exception(name(), port);
    }
  }

  name_t const real_name = convert_name(name(), name_);

  port_addr_t const cluster_addr = port_addr_t(name(), port);
  port_addr_t const mapped_addr = port_addr_t(real_name, mapped_port);

  connection_t const connection = connection_t(mapped_addr, cluster_addr);

  d->output_mappings.push_back(connection);
}

void
process_cluster
::connect(name_t const& upstream_name, port_t const& upstream_port,
          name_t const& downstream_name, port_t const& downstream_port)
{
  d->ensure_name(upstream_name);
  d->ensure_name(downstream_name);

  process_t const& up_proc = d->processes[upstream_name];

  if (!up_proc->output_port_info(upstream_port))
  {
    throw no_such_port_exception(upstream_name, upstream_port);
  }

  process_t const& down_proc = d->processes[downstream_name];

  if (!down_proc->input_port_info(downstream_port))
  {
    throw no_such_port_exception(downstream_name, downstream_port);
  }

  name_t const up_real_name = convert_name(name(), upstream_name);
  name_t const down_real_name = convert_name(name(), downstream_name);

  port_addr_t const up_addr = port_addr_t(up_real_name, upstream_port);
  port_addr_t const down_addr = port_addr_t(down_real_name, downstream_port);

  connection_t const connection = connection_t(up_addr, down_addr);

  d->internal_connections.push_back(connection);
}

void
process_cluster
::_configure()
{
}

void
process_cluster
::_init()
{
}

void
process_cluster
::_reset()
{
}

void
process_cluster
::_step()
{
  throw process_exception();
}

void
process_cluster
::_reconfigure(config_t const& conf)
{
  config::keys_t const tunable_keys = available_tunable_config();

  BOOST_FOREACH (priv::config_map_t::value_type const& config_mapping, d->config_map)
  {
    name_t const& name_ = config_mapping.first;
    priv::config_mappings_t const& mappings = config_mapping.second;

    config_t const provide_conf = config::empty_config();

    BOOST_FOREACH (priv::config_mapping_t const& mapping, mappings)
    {
      config::key_t const& key = mapping.first;

      if (!std::count(tunable_keys.begin(), tunable_keys.end(), key))
      {
        continue;
      }

      config::key_t const& mapped_key = mapping.second;

      config::value_t const& value = config_value<config::value_t>(key);

      provide_conf->set_value(mapped_key, value);
    }

    process_t const proc = d->processes[name_];

    // Grab the new subblock for the process.
    config_t const proc_conf = conf->subblock(name_);

    // Reconfigure the given process normally.
    proc->reconfigure(proc_conf);
    // Overwrite any provided configuration values which may be read-only.
    proc->reconfigure_with_provides(provide_conf);
  }

  process::_reconfigure(conf);
}

process::properties_t
process_cluster
::_properties() const
{
  properties_t base_properties = process::_properties();

  base_properties.insert(property_cluster);

  return base_properties;
}

process_cluster::priv
::priv()
  : config_map()
  , processes()
  , input_mappings()
  , output_mappings()
  , internal_connections()
{
}

process_cluster::priv
::~priv()
{
}

bool
process_cluster::priv
::has_name(name_t const& name) const
{
  return (0 != processes.count(name));
}

void
process_cluster::priv
::ensure_name(name_t const& name) const
{
  if (!has_name(name))
  {
    throw no_such_process_exception(name);
  }
}

process::name_t
convert_name(process::name_t const& cluster_name, process::name_t const& process_name)
{
  static process::name_t const sep = process::name_t("/");

  process::name_t const full_name = cluster_name + sep + process_name;

  return full_name;
}

}
