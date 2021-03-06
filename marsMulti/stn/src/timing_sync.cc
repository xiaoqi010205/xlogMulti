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
 * timing_sync.cc
 *
 *  Created on: 2012-10-19
 *      Author: zhouzhijie
 *  Copyright (c) 2013-2015 Tencent. All rights reserved.
 *
 */

#include "timing_sync.h"

#include "boost/bind.hpp"

#include "marsMulti/app/app.h"
#include "marsMulti/comm/thread/lock.h"
#include "marsMulti/comm/qm_xlogger/qm_xlogger.h"
#include "marsMulti/comm/platform_comm.h"
#include "marsMulti/stn/stn.h"

using namespace marsMulti::stn;
using namespace marsMulti::app;
using namespace marsMulti::comm;

#define ACTIVE_SYNC_INTERVAL (90*1000)
#define UNLOGIN_SYNC_INTERVAL (4*60*1000)
#define INACTIVE_SYNC_INTERVAL (10*60*1000)
#define NONET_SALT_RATE  (3)


#ifdef __ANDROID__
static const int kAlarmType = 119;
#endif


static int GetAlarmTime(bool _is_actived)
{
    int time = 0;
    //todo
    if (_is_actived && !::GetAccountInfo().is_logoned)
    {
        time = UNLOGIN_SYNC_INTERVAL;
    }
    else
	{
        time = _is_actived? ACTIVE_SYNC_INTERVAL : INACTIVE_SYNC_INTERVAL;
    }

    if (kNoNet == getNetInfo())
    {
        time *= NONET_SALT_RATE;
    }

    return time;
}

TimingSync::TimingSync(ActiveLogic& _active_logic)
:alarm_(boost::bind(&TimingSync::__OnAlarm, this), false)
, active_logic_(_active_logic)
{
    timing_sync_active_connection_ = _active_logic.SignalActive.connect(boost::bind(&TimingSync::OnActiveChanged, this, _1));
#ifdef __ANDROID__
    alarm_.SetType(kAlarmType);
#endif
    alarm_.Start(GetAlarmTime(active_logic_.IsActive()));
}

TimingSync::~TimingSync()
{
    alarm_.Cancel();
}

void TimingSync::OnActiveChanged(bool _is_actived)
{
    qm_xdebug_function();
    if (alarm_.IsWaiting())
    {
        alarm_.Cancel();
        alarm_.Start(GetAlarmTime(_is_actived));
    }
}

void TimingSync::OnNetworkChange()
{
    if (alarm_.IsWaiting())
    {
         alarm_.Cancel();
         alarm_.Start(GetAlarmTime(active_logic_.IsActive()));
    }
}

void TimingSync::OnLongLinkStatuChanged(LongLink::TLongLinkStatus _status, const std::string& _channel_id)
{
    qm_xverbose_function();
    if (_status == LongLink::kConnected)
        alarm_.Cancel();
    else if (_status == LongLink::kDisConnected)
        alarm_.Start(GetAlarmTime(active_logic_.IsActive()));
}

void TimingSync::__OnAlarm()
{
    qm_xdebug_function();

    if (kNoNet !=::getNetInfo())
    {
        qm_xinfo2(TSF"timing sync onRequestDoSync netinfo:%_", ::getNetInfo());
        ::RequestSync();
    }

    alarm_.Start(GetAlarmTime(active_logic_.IsActive()));
}

