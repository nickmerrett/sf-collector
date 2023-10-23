/** Copyright (C) 2022 IBM Corporation.
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

#ifndef __SF_CALLBACK_WRITER_
#define __SF_CALLBACK_WRITER_
#include "sysflow.h"
#include "sysflow/enums.hh"
#include "sysflowcontext.h"
#include "sysflowprocessor.h"
#include "sysflowwriter.h"

using sysflow::SysFlow;

namespace writer {
class SFCallbackWriter : public writer::SysFlowWriter {
private:
  sysflowprocessor::SysFlowProcessor *m_sysflowProc;
  SysFlowCallback m_callback;
  sysflow::SFHeader m_header;

public:
  SFCallbackWriter(context::SysFlowContext *cxt, time_t start,
                   SysFlowCallback callback,
                   sysflowprocessor::SysFlowProcessor *proc);
  virtual ~SFCallbackWriter();
  inline void write(SysFlow *flow) {}
  inline void write(SysFlow *flow, Process *proc, File *file1 = nullptr,
                    File *file2 = nullptr) {
    switch (flow->rec.idx()) {
    case SF_HEADER: {
      m_header = flow->rec.get_SFHeader();
      break;
    }
    case SF_CONT:
    case SF_FILE_OBJ:
    case SF_PROC: {
      break;
    }
    default: {
      sysflow::Container *cont = nullptr;
      if (proc != nullptr && !proc->containerId.is_null()) {
        cont = m_sysflowProc->getContainer(proc->containerId.get_string());
      }
      m_callback(&m_header, cont, proc, file1, file2, flow);
      break;
    }
    }
  }
  int initialize();
  void reset(time_t curTime);
  bool needsReset() { return false; }
};
} // namespace writer
#endif
