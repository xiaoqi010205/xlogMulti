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
 * baseprj.cpp
 *
 *  Created on: 2014-7-7
 *      Author: yerungui
 */

#include "marsMulti/baseevent/baseprjevent.h"
#include "marsMulti/baseevent/base_logic.h"

#include "marsMulti/comm/bootregister.h"
#include "marsMulti/comm/platform_comm.h"
#include "marsMulti/comm/thread/lock.h"

using namespace marsMulti::comm;

namespace marsMulti{
    namespace baseevent{

        void OnCreate()
        {
            GetSignalOnCreate()();
        }
        
        void OnInitBeforeOnCreate(int _encoder_status) {
            GetSignalOnInitBeforeOnCreate()(_encoder_status);
        }
        
        void OnDestroy()
        {
            GetSignalOnDestroy()();
        }
        
        void OnSingalCrash(int _sig)
        {
            GetSignalOnSingalCrash()(_sig);
        }
        
        void OnExceptionCrash()
        {
            GetSignalOnExceptionCrash()();
        }
        
        void OnForeground(bool _isforeground)
        {
            GetSignalOnForeground()(_isforeground);
        }
        
        void OnNetworkChange()
        {
#ifdef __APPLE__
            FlushReachability();
#endif
#ifdef ANDROID
            g_NetInfo = 0;
            
            ScopedLock lock(g_net_mutex);
            g_wifi_info.ssid.clear();
            g_wifi_info.bssid.clear();
            g_sim_info.isp_code.clear();
            g_sim_info.isp_name.clear();
            g_apn_info.nettype = kNoNet -1;
            g_apn_info.sub_nettype = 0;
            g_apn_info.extra_info.clear();
            lock.unlock();
#endif
            GetSignalOnNetworkChange()();
        }
        
        void OnNetworkDataChange(const char* _tag, int32_t _send, int32_t _recv) {
            GetSignalOnNetworkDataChange()(_tag, _send, _recv);
        }

#ifdef ANDROID
        void OnAlarm(int64_t _id) {
            GetSignalOnAlarm()(_id);
        }
#endif
    }
}

