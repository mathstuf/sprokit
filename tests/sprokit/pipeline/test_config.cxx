/*ckwg +29
 * Copyright 2011-2013 by Kitware, Inc.
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

#include <test_common.h>

#include <sprokit/pipeline/config.h>

#define TEST_ARGS ()

DECLARE_TEST_MAP();

int
main(int argc, char* argv[])
{
  CHECK_ARGS(1);

  testname_t const testname = argv[1];

  RUN_TEST(testname);
}

IMPLEMENT_TEST(block_sep_size)
{
  if (sprokit::config::block_sep.size() != 1)
  {
    TEST_ERROR("Block separator is not size 1; quite a "
               "few places rest on this assumption now");
  }
}

IMPLEMENT_TEST(has_value)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");
  sprokit::config::key_t const keyb = sprokit::config::key_t("keyb");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");

  config->set_value(keya, valuea);

  if (!config->has_value(keya))
  {
    TEST_ERROR("Block does not have value which was set");
  }

  if (config->has_value(keyb))
  {
    TEST_ERROR("Block has value which was not set");
  }
}

IMPLEMENT_TEST(get_value)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");

  config->set_value(keya, valuea);

  sprokit::config::value_t const get_valuea = config->get_value<sprokit::config::value_t>(keya);

  if (valuea != get_valuea)
  {
    TEST_ERROR("Did not retrieve value that was set");
  }
}

IMPLEMENT_TEST(get_value_nested)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");
  sprokit::config::key_t const keyb = sprokit::config::key_t("keyb");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");

  config->set_value(keya + sprokit::config::block_sep + keyb, valuea);

  sprokit::config_t const nested_config = config->subblock(keya);

  sprokit::config::value_t const get_valuea = nested_config->get_value<sprokit::config::value_t>(keyb);

  if (valuea != get_valuea)
  {
    TEST_ERROR("Did not retrieve value that was set");
  }
}

IMPLEMENT_TEST(get_value_no_exist)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");
  sprokit::config::key_t const keyb = sprokit::config::key_t("keyb");

  sprokit::config::value_t const valueb = sprokit::config::value_t("valueb");

  EXPECT_EXCEPTION(sprokit::no_such_configuration_value_exception,
                   config->get_value<sprokit::config::value_t>(keya),
                   "retrieving an unset value");

  sprokit::config::value_t const get_valueb = config->get_value<sprokit::config::value_t>(keyb, valueb);

  if (valueb != get_valueb)
  {
    TEST_ERROR("Did not retrieve default when requesting unset value");
  }
}

IMPLEMENT_TEST(get_value_type_mismatch)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");
  int const valueb = 100;

  config->set_value(keya, valuea);

  EXPECT_EXCEPTION(sprokit::bad_configuration_cast_exception,
                   config->get_value<int>(keya),
                   "doing an invalid cast");

  int const get_valueb = config->get_value<int>(keya, valueb);

  if (valueb != get_valueb)
  {
    TEST_ERROR("Did not retrieve default when requesting a bad cast");
  }
}

IMPLEMENT_TEST(bool_conversion)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const key = sprokit::config::key_t("key");

  sprokit::config::value_t const lit_true = sprokit::config::value_t("true");
  sprokit::config::value_t const lit_false = sprokit::config::value_t("false");
  sprokit::config::value_t const lit_True = sprokit::config::value_t("True");
  sprokit::config::value_t const lit_False = sprokit::config::value_t("False");
  sprokit::config::value_t const lit_1 = sprokit::config::value_t("1");
  sprokit::config::value_t const lit_0 = sprokit::config::value_t("0");

  bool val;

  config->set_value(key, lit_true);
  val = config->get_value<bool>(key);

  if (!val)
  {
    TEST_ERROR("The value \'true\' did not get converted to true when read as a boolean");
  }

  config->set_value(key, lit_false);
  val = config->get_value<bool>(key);

  if (val)
  {
    TEST_ERROR("The value \'false\' did not get converted to false when read as a boolean");
  }

  config->set_value(key, lit_True);
  val = config->get_value<bool>(key);

  if (!val)
  {
    TEST_ERROR("The value \'True\' did not get converted to true when read as a boolean");
  }

  config->set_value(key, lit_False);
  val = config->get_value<bool>(key);

  if (val)
  {
    TEST_ERROR("The value \'False\' did not get converted to false when read as a boolean");
  }

  config->set_value(key, lit_1);
  val = config->get_value<bool>(key);

  if (!val)
  {
    TEST_ERROR("The value \'1\' did not get converted to true when read as a boolean");
  }

  config->set_value(key, lit_0);
  val = config->get_value<bool>(key);

  if (val)
  {
    TEST_ERROR("The value \'0\' did not get converted to true when read as a boolean");
  }
}

IMPLEMENT_TEST(unset_value)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");
  sprokit::config::key_t const keyb = sprokit::config::key_t("keyb");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");
  sprokit::config::value_t const valueb = sprokit::config::value_t("valueb");

  config->set_value(keya, valuea);
  config->set_value(keyb, valueb);

  config->unset_value(keya);

  EXPECT_EXCEPTION(sprokit::no_such_configuration_value_exception,
                   config->get_value<sprokit::config::value_t>(keya),
                   "retrieving an unset value");

  sprokit::config::value_t const get_valueb = config->get_value<sprokit::config::value_t>(keyb);

  if (valueb != get_valueb)
  {
    TEST_ERROR("Did not retrieve value when requesting after an unrelated unset");
  }
}

IMPLEMENT_TEST(available_values)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");
  sprokit::config::key_t const keyb = sprokit::config::key_t("keyb");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");
  sprokit::config::value_t const valueb = sprokit::config::value_t("valueb");

  config->set_value(keya, valuea);
  config->set_value(keyb, valueb);

  sprokit::config::keys_t keys;

  keys.push_back(keya);
  keys.push_back(keyb);

  sprokit::config::keys_t const get_keys = config->available_values();

  if (keys.size() != get_keys.size())
  {
    TEST_ERROR("Did not retrieve correct number of keys");
  }
}

IMPLEMENT_TEST(read_only)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");
  sprokit::config::value_t const valueb = sprokit::config::value_t("valueb");

  config->set_value(keya, valuea);

  config->mark_read_only(keya);

  EXPECT_EXCEPTION(sprokit::set_on_read_only_value_exception,
                   config->set_value(keya, valueb),
                   "setting a read only value");

  sprokit::config::value_t const get_valuea = config->get_value<sprokit::config::value_t>(keya);

  if (valuea != get_valuea)
  {
    TEST_ERROR("Read only value changed");
  }
}

IMPLEMENT_TEST(read_only_unset)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");

  config->set_value(keya, valuea);

  config->mark_read_only(keya);

  EXPECT_EXCEPTION(sprokit::unset_on_read_only_value_exception,
                   config->unset_value(keya),
                   "unsetting a read only value");

  sprokit::config::value_t const get_valuea = config->get_value<sprokit::config::value_t>(keya);

  if (valuea != get_valuea)
  {
    TEST_ERROR("Read only value was unset");
  }
}

IMPLEMENT_TEST(subblock)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const block_name = sprokit::config::key_t("block");
  sprokit::config::key_t const other_block_name = sprokit::config::key_t("other_block");

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");
  sprokit::config::key_t const keyb = sprokit::config::key_t("keyb");
  sprokit::config::key_t const keyc = sprokit::config::key_t("keyc");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");
  sprokit::config::value_t const valueb = sprokit::config::value_t("valueb");
  sprokit::config::value_t const valuec = sprokit::config::value_t("valuec");

  config->set_value(block_name + sprokit::config::block_sep + keya, valuea);
  config->set_value(block_name + sprokit::config::block_sep + keyb, valueb);
  config->set_value(other_block_name + sprokit::config::block_sep + keyc, valuec);

  sprokit::config_t const subblock = config->subblock(block_name);

  if (subblock->has_value(keya))
  {
    sprokit::config::value_t const get_valuea = subblock->get_value<sprokit::config::value_t>(keya);

    if (valuea != get_valuea)
    {
      TEST_ERROR("Subblock did not inherit expected keys");
    }
  }
  else
  {
    TEST_ERROR("Subblock did not inherit expected keys");
  }

  if (subblock->has_value(keyb))
  {
    sprokit::config::value_t const get_valueb = subblock->get_value<sprokit::config::value_t>(keyb);

    if (valueb != get_valueb)
    {
      TEST_ERROR("Subblock did not inherit expected keys");
    }
  }
  else
  {
    TEST_ERROR("Subblock did not inherit expected keys");
  }

  if (subblock->has_value(keyc))
  {
    TEST_ERROR("Subblock inherited unrelated key");
  }
}

IMPLEMENT_TEST(subblock_nested)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const block_name = sprokit::config::key_t("block");
  sprokit::config::key_t const other_block_name = sprokit::config::key_t("other_block");
  sprokit::config::key_t const nested_block_name = block_name + sprokit::config::block_sep + other_block_name;

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");
  sprokit::config::key_t const keyb = sprokit::config::key_t("keyb");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");
  sprokit::config::value_t const valueb = sprokit::config::value_t("valueb");

  config->set_value(nested_block_name + sprokit::config::block_sep + keya, valuea);
  config->set_value(nested_block_name + sprokit::config::block_sep + keyb, valueb);

  sprokit::config_t const subblock = config->subblock(nested_block_name);

  if (subblock->has_value(keya))
  {
    sprokit::config::value_t const get_valuea = subblock->get_value<sprokit::config::value_t>(keya);

    if (valuea != get_valuea)
    {
      TEST_ERROR("Nested subblock did not inherit expected keys");
    }
  }
  else
  {
    TEST_ERROR("Subblock did not inherit expected keys");
  }

  if (subblock->has_value(keyb))
  {
    sprokit::config::value_t const get_valueb = subblock->get_value<sprokit::config::value_t>(keyb);

    if (valueb != get_valueb)
    {
      TEST_ERROR("Nested subblock did not inherit expected keys");
    }
  }
  else
  {
    TEST_ERROR("Subblock did not inherit expected keys");
  }
}

IMPLEMENT_TEST(subblock_match)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const block_name = sprokit::config::key_t("block");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");

  config->set_value(block_name, valuea);

  sprokit::config_t const subblock = config->subblock(block_name);

  sprokit::config::keys_t const keys = subblock->available_values();

  if (!keys.empty())
  {
    TEST_ERROR("A subblock inherited a value that shared the block name");
  }
}

IMPLEMENT_TEST(subblock_prefix_match)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const block_name = sprokit::config::key_t("block");

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");

  config->set_value(block_name + keya, valuea);

  sprokit::config_t const subblock = config->subblock(block_name);

  sprokit::config::keys_t const keys = subblock->available_values();

  if (!keys.empty())
  {
    TEST_ERROR("A subblock inherited a value that shared a prefix with the block name");
  }
}

IMPLEMENT_TEST(subblock_view)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const block_name = sprokit::config::key_t("block");
  sprokit::config::key_t const other_block_name = sprokit::config::key_t("other_block");

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");
  sprokit::config::key_t const keyb = sprokit::config::key_t("keyb");
  sprokit::config::key_t const keyc = sprokit::config::key_t("keyc");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");
  sprokit::config::value_t const valueb = sprokit::config::value_t("valueb");
  sprokit::config::value_t const valuec = sprokit::config::value_t("valuec");

  config->set_value(block_name + sprokit::config::block_sep + keya, valuea);
  config->set_value(block_name + sprokit::config::block_sep + keyb, valueb);
  config->set_value(other_block_name + sprokit::config::block_sep + keyc, valuec);

  sprokit::config_t const subblock = config->subblock_view(block_name);

  if (!subblock->has_value(keya))
  {
    TEST_ERROR("Subblock view did not inherit key");
  }

  if (subblock->has_value(keyc))
  {
    TEST_ERROR("Subblock view inherited unrelated key");
  }

  config->set_value(block_name + sprokit::config::block_sep + keya, valueb);

  sprokit::config::value_t const get_valuea1 = subblock->get_value<sprokit::config::value_t>(keya);

  if (valueb != get_valuea1)
  {
    TEST_ERROR("Subblock view persisted a changed value");
  }

  subblock->set_value(keya, valuea);

  sprokit::config::value_t const get_valuea2 = config->get_value<sprokit::config::value_t>(block_name + sprokit::config::block_sep + keya);

  if (valuea != get_valuea2)
  {
    TEST_ERROR("Subblock view set value was not changed in parent");
  }

  subblock->unset_value(keyb);

  if (config->has_value(block_name + sprokit::config::block_sep + keyb))
  {
    TEST_ERROR("Unsetting from a subblock view did not unset in parent view");
  }

  config->set_value(block_name + sprokit::config::block_sep + keyc, valuec);

  sprokit::config::keys_t keys;

  keys.push_back(keya);
  keys.push_back(keyc);

  sprokit::config::keys_t const get_keys = subblock->available_values();

  if (keys.size() != get_keys.size())
  {
    TEST_ERROR("Did not retrieve correct number of keys from the subblock");
  }
}

IMPLEMENT_TEST(subblock_view_nested)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const block_name = sprokit::config::key_t("block");
  sprokit::config::key_t const other_block_name = sprokit::config::key_t("other_block");
  sprokit::config::key_t const nested_block_name = block_name + sprokit::config::block_sep + other_block_name;

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");
  sprokit::config::key_t const keyb = sprokit::config::key_t("keyb");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");
  sprokit::config::value_t const valueb = sprokit::config::value_t("valueb");

  config->set_value(nested_block_name + sprokit::config::block_sep + keya, valuea);
  config->set_value(nested_block_name + sprokit::config::block_sep + keyb, valueb);

  sprokit::config_t const subblock = config->subblock_view(nested_block_name);

  if (subblock->has_value(keya))
  {
    sprokit::config::value_t const get_valuea = subblock->get_value<sprokit::config::value_t>(keya);

    if (valuea != get_valuea)
    {
      TEST_ERROR("Nested subblock did not inherit expected keys");
    }
  }
  else
  {
    TEST_ERROR("Subblock did not inherit expected keys");
  }

  if (subblock->has_value(keyb))
  {
    sprokit::config::value_t const get_valueb = subblock->get_value<sprokit::config::value_t>(keyb);

    if (valueb != get_valueb)
    {
      TEST_ERROR("Nested subblock did not inherit expected keys");
    }
  }
  else
  {
    TEST_ERROR("Subblock did not inherit expected keys");
  }
}

IMPLEMENT_TEST(subblock_view_match)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const block_name = sprokit::config::key_t("block");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");

  config->set_value(block_name, valuea);

  sprokit::config_t const subblock = config->subblock_view(block_name);

  sprokit::config::keys_t const keys = subblock->available_values();

  if (!keys.empty())
  {
    TEST_ERROR("A subblock inherited a value that shared the block name");
  }
}

IMPLEMENT_TEST(subblock_view_prefix_match)
{
  sprokit::config_t const config = sprokit::config::empty_config();

  sprokit::config::key_t const block_name = sprokit::config::key_t("block");

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");

  config->set_value(block_name + keya, valuea);

  sprokit::config_t const subblock = config->subblock_view(block_name);

  sprokit::config::keys_t const keys = subblock->available_values();

  if (!keys.empty())
  {
    TEST_ERROR("A subblock inherited a value that shared a prefix with the block name");
  }
}

IMPLEMENT_TEST(merge_config)
{
  sprokit::config_t const configa = sprokit::config::empty_config();
  sprokit::config_t const configb = sprokit::config::empty_config();

  sprokit::config::key_t const keya = sprokit::config::key_t("keya");
  sprokit::config::key_t const keyb = sprokit::config::key_t("keyb");
  sprokit::config::key_t const keyc = sprokit::config::key_t("keyc");

  sprokit::config::value_t const valuea = sprokit::config::value_t("valuea");
  sprokit::config::value_t const valueb = sprokit::config::value_t("valueb");
  sprokit::config::value_t const valuec = sprokit::config::value_t("valuec");

  configa->set_value(keya, valuea);
  configa->set_value(keyb, valuea);

  configb->set_value(keyb, valueb);
  configb->set_value(keyc, valuec);

  configa->merge_config(configb);

  sprokit::config::value_t const get_valuea = configa->get_value<sprokit::config::value_t>(keya);

  if (valuea != get_valuea)
  {
    TEST_ERROR("Unmerged key changed");
  }

  sprokit::config::value_t const get_valueb = configa->get_value<sprokit::config::value_t>(keyb);

  if (valueb != get_valueb)
  {
    TEST_ERROR("Conflicting key was not overwritten");
  }

  sprokit::config::value_t const get_valuec = configa->get_value<sprokit::config::value_t>(keyc);

  if (valuec != get_valuec)
  {
    TEST_ERROR("New key did not appear");
  }
}
