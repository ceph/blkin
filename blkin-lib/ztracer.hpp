/*
 * Copyright 2014 Marios Kogias <marioskogias@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 *   1. Redistributions of source code must retain the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer.
 *   2. Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials
 *      provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef ZTRACER_H

#define ZTRACER_H

#include <sstream>

extern "C" {
#include <zipkin_c.h>
}

namespace ZTracer {
	using std::string;

	int ztrace_init(void);

	class Endpoint : private blkin_endpoint {
		private:
			string _ip;
			string _name;

			friend class Trace;
		public:
			Endpoint(const string &ip, int port, const string &name)
				: _ip(ip), _name(name)
			{
				blkin_init_endpoint(this, ip.c_str(), port, name.c_str());
			}
	};

	class Trace {
		private:
			struct blkin_trace trace;
			Endpoint *ep;
			string name;

			struct blkin_trace *get_blkin_trace()
			{
				return &trace;
			}
		public:
			Trace(const string &name, Endpoint *ep)
				: ep(ep), name(name)
			{
				blkin_init_new_trace(&trace, name.c_str(), ep);
			}

			Trace(const string &name, Trace *t)
				: ep(t->ep), name(name)
			{
				blkin_init_child(&trace, t->get_blkin_trace(), ep, name.c_str());
			}

			Trace(const string &name, Trace *t, Endpoint *ep)
				: ep(ep), name(name)
			{
				blkin_init_child(&trace, t->get_blkin_trace(), ep, name.c_str());
			}

			Trace(const string &name, Endpoint *ep, struct blkin_trace_info *info, bool child=false)
				: ep(ep), name(name)
			{
				if (child)
					blkin_init_child_info(&trace, info, ep, name.c_str());
				else {
					blkin_init_new_trace(&trace, name.c_str(), ep);
					blkin_set_trace_info(&trace, info);
				}
			}

            Endpoint* get_endpoint()
            {
                return ep;
            }

			int get_trace_info(struct blkin_trace_info *info);
			int set_trace_info(struct blkin_trace_info *info);

			void keyval(const char *key, const char *val)
			{
				BLKIN_KEYVAL_STRING(&trace, ep, key, val);
			}
			void keyval(const char *key, int64_t val)
			{
				BLKIN_KEYVAL_INTEGER(&trace, ep, key, val);
			}
			void keyval(const char *key, const char *val,
                  const Endpoint *endpoint)
			{
				BLKIN_KEYVAL_STRING(&trace, endpoint, key, val);
			}
			void keyval(const char *key, int64_t val,
                  const Endpoint *endpoint)
			{
				BLKIN_KEYVAL_INTEGER(&trace, endpoint, key, val);
			}

			void event(const char *event)
			{
				BLKIN_TIMESTAMP(&trace, ep, event);
			}
			void event(const char *event, const Endpoint *endpoint)
			{
				BLKIN_TIMESTAMP(&trace, endpoint, event);
			}
	};

}
#endif /* end of include guard: ZTRACER_H */
