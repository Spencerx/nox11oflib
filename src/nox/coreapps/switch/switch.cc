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
#include <boost/foreach.hpp>
#include <boost/shared_array.hpp>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <stdint.h>

#include "openflow-default.hh"
#include "assert.hh"
#include "component.hh"
#include "flow.hh"
#include "fnv_hash.hh"
#include "hash_set.hh"
#include "ofp-msg-event.hh"
#include "vlog.hh"

#include "netinet++/ethernetaddr.hh"
#include "netinet++/ethernet.hh"

#include "../../../oflib/ofl-actions.h"
#include "../../../oflib/ofl-messages.h"

using namespace vigil;
using namespace vigil::container;

namespace {

struct Mac_source
{
    /* Key. */
    datapathid datapath_id;     /* Switch. */
    ethernetaddr mac;           /* Source MAC. */

    /* Value. */
    mutable int port;           /* Port where packets from 'mac' were seen. */

    Mac_source() : port(-1) { }
    Mac_source(datapathid datapath_id_, ethernetaddr mac_)
        : datapath_id(datapath_id_), mac(mac_), port(-1)
        { }
};

bool operator==(const Mac_source& a, const Mac_source& b)
{
    return a.datapath_id == b.datapath_id && a.mac == b.mac;
}

bool operator!=(const Mac_source& a, const Mac_source& b) 
{
    return !(a == b);
}

struct Hash_mac_source
{
    std::size_t operator()(const Mac_source& val) const {
        uint32_t x;
        x = vigil::fnv_hash(&val.datapath_id, sizeof val.datapath_id);
        x = vigil::fnv_hash(val.mac.octet, sizeof val.mac.octet, x);
        return x;
    }
};

Vlog_module log("switch");

class Switch
    : public Component 
{
public:
    Switch(const Context* c,
           const json_object*) 
        : Component(c) { }

    void configure(const Configuration*);

    void install();

    Disposition handle(const Event&);

private:
    typedef hash_set<Mac_source, Hash_mac_source> Source_table;
    Source_table sources;

    /* Set up a flow when we know the destination of a packet?  This should
     * ordinarily be true; it is only usefully false for debugging purposes. */
    bool setup_flows;
};

void 
Switch::configure(const Configuration* conf) {
    setup_flows = true; // default value
    BOOST_FOREACH (const std::string& arg, conf->get_arguments()) {
        if (arg == "noflow") {
            setup_flows = false;
        } else {
            VLOG_WARN(log, "argument \"%s\" not supported", arg.c_str());
        }
    }
    
    register_handler(Ofp_msg_event::get_name(OFPT_PACKET_IN), boost::bind(&Switch::handle, this, _1));
}

void
Switch::install() {

}

Disposition
Switch::handle(const Event& e)
{
   
    return CONTINUE;
}

REGISTER_COMPONENT(container::Simple_component_factory<Switch>, Switch);

} // unnamed namespace
