/** Copyright (C) 2019 IBM Corporation.
*
* Authors:
* Frederico Araujo <frederico.araujo@ibm.com>
* Teryl Taylor <terylt@ibm.com>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/

#ifndef _SF_FILE_
#define _SF_FILE_
#include "sysflow.h"
#include "datatypes.h"
#include "sysflowwriter.h"
#include "containercontext.h"
using namespace sysflow;
namespace file {
    class FileContext {
        private:
            SysFlowWriter* m_writer;
            FileTable m_files;
            container::ContainerContext* m_containerCxt;
            void clearAllFiles();
        public:
            FileContext(container::ContainerContext* containerCxt, SysFlowWriter* writer);
            virtual ~FileContext();
            FileObj* getFile(sinsp_evt* ev, SFObjectState state, bool& created);
            FileObj *getFile(sinsp_evt *ev, const string &path, char typechar,
                             SFObjectState state, bool &created);
            FileObj *getFile(const string &key);
            FileObj* createFile(sinsp_evt* ev, string path, char typechar, SFObjectState state, string key);
            bool exportFile(const string &key);
            void clearFiles();
            inline int getSize() {
                return m_files.size();
            }
     };
     } // namespace file

#endif
