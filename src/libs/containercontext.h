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

#ifndef _SF_CONT_
#define _SF_CONT_
#include <string>

#include "datatypes.h"
#include "k8scontext.h"
#include "sysflow.h"
#include "sysflowcontext.h"
#include "sysflowwriter.h"
#include <sinsp.h>
#define CONT_TABLE_SIZE 100
#define INCOMPLETE "incomplete"
#define INCOMPLETE_IMAGE "incomplete:incomplete"

namespace container {
class ContainerContext {
private:
  ContainerTable m_containers;
  context::SysFlowContext *m_cxt;
  writer::SysFlowWriter *m_writer;
  sfk8s::K8sContext *m_k8sCxt;
  ContainerObj *createContainer(sinsp_threadinfo *ti);
  void setContainer(ContainerObj **cont, sinsp_container_info::ptr_t container);
  void reupPod(sinsp_threadinfo *ti, ContainerObj *cont);

public:
  ContainerContext(context::SysFlowContext *cxt, writer::SysFlowWriter *writer,
                   sfk8s::K8sContext *k8sCxt);
  virtual ~ContainerContext();
  ContainerObj *getContainer(sinsp_threadinfo *ti);
  ContainerObj *getContainer(const std::string &id);
  bool exportContainer(const std::string &id);
  int derefContainer(const std::string &id);
  void clearAllContainers();
  void clearContainers();
  inline int getSize() { return m_containers.size(); }
};
} // namespace container
#endif
