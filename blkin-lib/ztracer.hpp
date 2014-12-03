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

	int ztrace_init() { return blkin_init(); }

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

	class Trace : private blkin_trace {
		private:
			string _name;

		public:
			Trace(const string &name, Endpoint *ep)
				: _name(name)
			{
				blkin_init_new_trace(this, _name.c_str(), ep);
			}

			Trace(const string &name, Trace *parent)
				: _name(name)
			{
				blkin_init_child(this, parent, parent->endpoint, _name.c_str());
			}

			Trace(const string &name, Trace *parent, Endpoint *ep)
				: _name(name)
			{
				blkin_init_child(this, parent, ep, _name.c_str());
			}

			Trace(const string &name, Endpoint *ep,
            const blkin_trace_info *i, bool child=false)
				: _name(name)
			{
				if (child)
					blkin_init_child_info(this, i, ep, _name.c_str());
				else {
					blkin_init_new_trace(this, _name.c_str(), ep);
					set_info(i);
				}
			}

      const blkin_trace_info* get_info() const { return &info; }
      void set_info(const blkin_trace_info *i) { info = *i; }

			void keyval(const char *key, const char *val)
			{
				BLKIN_KEYVAL_STRING(this, endpoint, key, val);
			}
			void keyval(const char *key, int64_t val)
			{
				BLKIN_KEYVAL_INTEGER(this, endpoint, key, val);
			}
			void keyval(const char *key, const char *val, const Endpoint *ep)
			{
				BLKIN_KEYVAL_STRING(this, ep, key, val);
			}
			void keyval(const char *key, int64_t val, const Endpoint *ep)
			{
				BLKIN_KEYVAL_INTEGER(this, ep, key, val);
			}

			void event(const char *event)
			{
				BLKIN_TIMESTAMP(this, endpoint, event);
			}
			void event(const char *event, const Endpoint *ep)
			{
				BLKIN_TIMESTAMP(this, ep, event);
			}
	};

}
#endif /* end of include guard: ZTRACER_H */
