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
 * httpchecker.cpp
 *
 *  Created on: 2014-7-1
 *      Author: yanguoyue caoshaokun
 */

#include "httpchecker.h"

#include "marsMulti/comm/singleton.h"
#include "marsMulti/comm/qm_xlogger/qm_xlogger.h"
#include "marsMulti/comm/qm_time_utils.h"
#include "marsMulti/sdt/constants.h"
#include "marsMulti/sdt/sdt_logic.h"

#include "sdt/src/checkimpl/httpquery.h"

using namespace marsMulti::sdt;

static std::string sg_netcheck_cgi;

namespace marsMulti {
namespace sdt {

	void SetHttpNetcheckCGI(std::string cgi) {
		sg_netcheck_cgi = cgi;
	}

}
}

HttpChecker::HttpChecker() {
    qm_xverbose_function();
}

HttpChecker::~HttpChecker() {
    qm_xverbose_function();
}

int HttpChecker::StartDoCheck(CheckRequestProfile& _check_request) {
    qm_xinfo_function();
    return BaseChecker::StartDoCheck(_check_request);
}


void HttpChecker::__DoCheck(CheckRequestProfile& _check_request) {
    qm_xinfo_function();

    for (CheckIPPorts_Iterator iter = _check_request.shortlink_items.begin(); iter != _check_request.shortlink_items.end(); ++iter) {
    	std::string host = iter->first;
    	for (std::vector<CheckIPPort>::iterator ipport = iter->second.begin(); ipport != iter->second.end(); ++ipport) {
            
            if (is_canceled_) {
                qm_xinfo2(TSF"HttpChecker is canceled.");
                return;
            }
            
    		CheckResultProfile profile;
    		profile.netcheck_type = kHttpCheck;
    		profile.network_type = comm::getNetInfo();
    		profile.ip = (*ipport).ip;
    		profile.port = (*ipport).port;

    		profile.url = (iter->first.empty() ? DEFAULT_HTTP_HOST : (iter->first));
    		profile.url.append(sg_netcheck_cgi.c_str());
    		uint64_t start_time = gettickcount();
    		std::string errmsg;
            
            if (!strutil::StartsWith(profile.url, "http://")) {
                profile.url = std::string("http://") + profile.url;
            }
            
    		int ret = SendHttpQuery(profile.url, profile.status_code, errmsg, _check_request.total_timeout);
    		uint64_t cost_time = gettickcount() - start_time;
    		profile.rtt = cost_time;

            if (ret >= 0) {
                qm_xinfo2(TSF"http check, host: %_, ret: %_", profile.url, profile.status_code);
            }
            else {
                qm_xinfo2(TSF"http check, host: %_, ret: %_", profile.url, profile.status_code);
            }

            _check_request.checkresult_profiles.push_back(profile);
            _check_request.check_status = (ret >= 0 ? kCheckContinue : kCheckFinish);

			if (_check_request.total_timeout != UNUSE_TIMEOUT) {
				_check_request.total_timeout -= cost_time;
				if (_check_request.total_timeout <= 0) {
					qm_xinfo2(TSF"http check, host: %_, timeout.", host);
					break;
				}
			}
    	}
    }
}
