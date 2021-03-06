// Tencent is pleased to support the open source community by making Mars available.
// Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

// Licensed under the MIT License (the "License"); you may not use this file except in 
// compliance with the License. You may obtain a copy of the License at
// http://opensource.org/licenses/MIT

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions and
// limitations under the License.


/*
 * netsource_timercheck.h
 *
 *  Created on: 2013-5-16
 *      Author: yanguoyue
 */

#ifndef STN_SRC_NETSOURCE_TIMERCHECK_H_
#define STN_SRC_NETSOURCE_TIMERCHECK_H_

#include "boost/signals2.hpp"

#include "marsMulti/comm/thread/thread.h"
#include "marsMulti/baseevent/active_logic.h"
#include "marsMulti/comm/socket/socketselect.h"
#include "marsMulti/comm/messagequeue/message_queue.h"

#include "net_source.h"

class CommFrequencyLimit;

namespace marsMulti {
    namespace stn {
        
class LongLink;

/*
 * If longlink is using backup, check if other ips are usable, if are usable, notify longlink to change ip
 */
class NetSourceTimerCheck {
  public:
    NetSourceTimerCheck(NetSource* _net_source, comm::ActiveLogic& _active_logic, LongLink& _longlink, comm::MessageQueue::MessageQueue_t  _messagequeue_id);
    ~NetSourceTimerCheck();
    void CancelConnect();

  public:
    boost::function<void ()> fun_time_check_suc_;

  private:
    void __Run(const std::string& _host);
    bool __TryConnnect(const std::string& _host);
    void __OnActiveChanged(bool _is_active);
    void __StartCheck();
    void __Check();
    void __StopCheck();

  private:
    comm::Thread thread_;
    boost::signals2::scoped_connection active_connection_;
    NetSource* net_source_;
    comm::SocketBreaker breaker_;
    comm::SocketSelect seletor_;
    CommFrequencyLimit* frequency_limit_;
    LongLink& longlink_;

    comm::MessageQueue::ScopeRegister asyncreg_;
    comm::MessageQueue::MessagePost_t asyncpost_;
    NetSource::DnsUtil dns_util_;
};
        
    }
}


#endif // STN_SRC_NETSOURCE_TIMERCHECK_H_
