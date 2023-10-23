#!/usr/bin/env python3
#
# Copyright (C) 2019 IBM Corporation.
#
# Authors:
# Frederico Araujo <frederico.araujo@ibm.com>
# Teryl Taylor <terylt@ibm.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from sysflow.reader import SFReader
import sys
import json
import sysflow
import itertools
import base64

primitives = (int, str, bool, float, bytes)

reader1 = SFReader(sys.argv[1])
reader2 = SFReader(sys.argv[2])

def compareAttributes(att1, objtype1, att2, objtype2, exit, msgs):
    if type(att1) != type(att2):
         msgs.append('Attribute types: ' + str(type(att1)) + ' and ' + str(type(att2)) + ' don\'t match.')
         exit[0] = True
         return False;
    if hasattr(att1,'__dict__'):
        f1attrs = [a for a in dir(att1) if not (a.startswith('__') or a == '_inner_dict' or a == 'RECORD_SCHEMA' or a == 'fromkeys' or a == 'get' or a == 'items' or a == 'keys' or a == 'pop' or a == 'popitem' or a == 'setdefault' or a == 'update' or a == 'values' or a == 'copy' or a == 'clear') ]
        f2attrs = [a for a in dir(att2) if not (a.startswith('__') or a == '_inner_dict' or a == 'RECORD_SCHEMA' or a == 'fromkeys' or a == 'get' or a == 'items' or a == 'keys' or a == 'pop' or a == 'popitem' or a == 'setdefault' or a == 'update' or a == 'values' or a == 'copy' or a == 'clear') ]
        if len(f1attrs) != len(f2attrs):
            msgs.append(str(objtype1) + ' does not have the same number of attributes in each file')
            return False
        for attr in f1attrs:
            a1 = getattr(att1, attr)
            a2 = getattr(att2, attr)
            if(not compareAttributes(a1, objtype1, a2, objtype2, exit, msgs) and attr != "filename"):
                msgs.append('Attribute: ' + attr + ' does not have matching values')
                return False
    elif isinstance(att1, list):
        if len(att1) != len(att2):
            msgs.append('lists are not the same length ' + str(len(att1)) + " " + str(len(att2)))
            return False
        a1 = ''.join(str(att1))
        a2 = ''.join(str(att2))
        if(not compareAttributes(a1, objtype1, a2, objtype2, exit, msgs)):
            return False;
    elif isinstance(att1, primitives):
        if att1 != att2:
            if isinstance(att1, bytes):
                 msgs.append('Attribute values do not match ' + str(base64.b64encode(att1)) +  ' ' + str(base64.b64encode(att2)))
            else:
                 msgs.append('Attribute values do not match ' + str(att1) +  ' ' + str(att2))
            return False
    elif att1 is None:
        if not att2 is None:
             return False
    else:
        print('Uh oh, class not supported!' + str(type(att1)))
        return False
    return True   
 
exit = [False]
msgs = []
recNum = 0
failures = 0

print('Beginning test cases between files: {} and {}'.format(sys.argv[1], sys.argv[2]))

for tup1, tup2 in itertools.zip_longest(reader1, reader2, fillvalue=None):
    flow1 = tup1[1]
    flow2 = tup2[1]
    objtype1 = tup1[0]
    objtype2 = tup2[0]
    if not (flow1 is None or flow2 is None):
         msgs.clear()
         if objtype1 != objtype2:
             print('[ FAILED ] Record: {} does not match object types: {} != {}'.format(recNum, objtype1, objtype2))
             recNum += 1
             continue

         if not compareAttributes(flow1, objtype1, flow2, objtype2, exit, msgs):
             print('[ FAILED ] Record: {} does not match'.format(recNum))
             failures += 1
             for m in msgs:
                 print(m)
         recNum += 1
    else:
        print('[ FAILED ]  Files do not have the same number of records')
        failures += 1

if failures > 0:
    print('[ FAILED ] Test case failed with {} failures before stopping'.format(failures));
    sys.exit(1)
else:
    print('[ PASSED ] Test case complete')
