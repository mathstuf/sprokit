#!@PYTHON_EXECUTABLE@
#ckwg +4
# Copyright 2011-2012 by Kitware, Inc. All Rights Reserved. Please refer to
# KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
# Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.


def test_import():
    try:
        import vistk.pipeline.datum
    except:
        test_error("Failed to import the datum module")


def test_new():
    from vistk.pipeline import datum

    d = datum.new('test_datum')

    if not d.type() == datum.DatumType.data:
        test_error("Datum type mismatch")

    if len(d.get_error()):
        test_error("A data datum has an error string")

    p = d.get_datum()

    if p is None:
        test_error("A data datum has None as its data")


def test_empty():
    from vistk.pipeline import datum

    d = datum.empty()

    if not d.type() == datum.DatumType.empty:
        test_error("Datum type mismatch")

    if len(d.get_error()):
        test_error("An empty datum has an error string")

    p = d.get_datum()

    if p is not None:
        test_error("An empty datum does not have None as its data")


def test_flush():
    from vistk.pipeline import datum

    d = datum.flush()

    if not d.type() == datum.DatumType.flush:
        test_error("Datum type mismatch")

    if len(d.get_error()):
        test_error("A flush datum has an error string")

    p = d.get_datum()

    if p is not None:
        test_error("A flush datum does not have None as its data")


def test_complete():
    from vistk.pipeline import datum

    d = datum.complete()

    if not d.type() == datum.DatumType.complete:
        test_error("Datum type mismatch")

    if len(d.get_error()):
        test_error("A complete datum has an error string")

    p = d.get_datum()

    if p is not None:
        test_error("A complete datum does not have None as its data")


def test_error_():
    from vistk.pipeline import datum

    err = 'An error'

    d = datum.error(err)

    if not d.type() == datum.DatumType.error:
        test_error("Datum type mismatch")

    if not d.get_error() == err:
        test_error("An error datum did not keep the message")

    p = d.get_datum()

    if p is not None:
        test_error("An error datum does not have None as its data")


def main(testname):
    if testname == 'import':
        test_import()
    elif testname == 'new':
        test_new()
    elif testname == 'empty':
        test_empty()
    elif testname == 'flush':
        test_flush()
    elif testname == 'complete':
        test_complete()
    elif testname == 'error':
        test_error_()
    else:
        test_error("No such test '%s'" % testname)


if __name__ == '__main__':
    import os
    import sys

    if not len(sys.argv) == 4:
        test_error("Expected three arguments")
        sys.exit(1)

    testname = sys.argv[1]

    os.chdir(sys.argv[2])

    sys.path.append(sys.argv[3])

    from vistk.test.test import *

    try:
        main(testname)
    except BaseException as e:
        test_error("Unexpected exception: %s" % str(e))