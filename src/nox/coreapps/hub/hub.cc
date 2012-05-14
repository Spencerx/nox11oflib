/* Copyright 2008 (C) Nicira, Inc.
 *
 * This file is part of NOX.
 *
 * NOX is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * NOX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with NOX.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <boost/bind.hpp>
#include <boost/shared_array.hpp>
#include <netinet/in.h>
#include "assert.hh"
#include "component.hh"
#include "flow.hh"
//#include "packet-in.hh"
#include "ofp-msg-event.hh"
#include "vlog.hh"

#include "netinet++/ethernet.hh"

#include "../../../oflib/ofl-structs.h"
#include "../../../oflib/ofl-messages.h"

namespace {

using namespace vigil;
using namespace vigil::container;

Vlog_module lg("hub");

class Hub 
    : public Component 
{
public:
     Hub(const Context* c,
         const json_object*) 
         : Component(c) { }
    
    void configure(const Configuration*) {
        
        Flow flow;
        ethernetaddr mac = "10:21:12:12:12:12";
        flow.Add_Field("tcp_dst",mac);
    }

    Disposition handler(const Event& e)
    {
       return CONTINUE;
    }

    void install()
    {
        register_handler(Ofp_msg_event::get_name(OFPT_PACKET_IN), boost::bind(&Hub::handler, this, _1));
    }
};

REGISTER_COMPONENT(container::Simple_component_factory<Hub>, Hub);

} // unnamed namespace
