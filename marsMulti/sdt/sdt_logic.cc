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
 * sdt_logic.cc
 *
 *  Created on: 2016年3月18日
 *      Author: caoshaokun
 */

#include "marsMulti/sdt/sdt_logic.h"

#include "marsMulti/baseevent/baseevent.h"
#include "marsMulti/baseevent/baseprjevent.h"
#include "marsMulti/comm/qm_xlogger/qm_xlogger.h"
#include "marsMulti/comm/bootrun.h"
#include "marsMulti/sdt/constants.h"

#include "sdt/src/sdt_core.h"

namespace marsMulti {
namespace sdt {

static Callback* sg_callback = NULL;

static const std::string kLibName = "sdt";

#define SDT_WEAK_CALL(func) \
    boost::shared_ptr<SdtCore> sdt_ptr = SdtCore::Singleton::Instance_Weak().lock();\
    if (!sdt_ptr) {\
        qm_xwarn2(TSF"sdt uncreate");\
        return;\
    }\
	sdt_ptr->func

static void onCreate() {
    qm_xinfo2(TSF"sdt oncreate");
    SdtCore::Singleton::Instance();
}

static void onDestroy() {
    qm_xinfo2(TSF"sdt onDestroy");
    SdtCore::Singleton::AsyncRelease();
}

static void __initbind_baseprjevent() {

	GetSignalOnCreate().connect(&onCreate);
	GetSignalOnDestroy().connect(5, &onDestroy);
}

BOOT_RUN_STARTUP(__initbind_baseprjevent);

//active netcheck interface
void StartActiveCheck(CheckIPPorts& _longlink_check_items, CheckIPPorts& _shortlink_check_items, int _mode, int _timeout) {
	SDT_WEAK_CALL(StartCheck(_longlink_check_items, _shortlink_check_items, _mode, _timeout));
}

void CancelActiveCheck() {
	SDT_WEAK_CALL(CancelCheck());
}

void SetCallBack(Callback* const callback) {
	sg_callback = callback;
}

#if !defined(ANDROID) || defined (CPP_CALL_BACK)
void (*ReportNetCheckResult)(const std::vector<CheckResultProfile>& _check_results)
= [](const std::vector<CheckResultProfile>& _check_results) {

};

#endif

}}
