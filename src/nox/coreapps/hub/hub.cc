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
#include "datapath-join.hh"
#include "assert.hh"
#include "component.hh"
#include "flowmod.hh"
#include "instructions.hh"
#include "actions.hh"
//#include "packet-in.hh"
#include "ofp-msg-event.hh"
#include "vlog.hh"

#include "netinet++/ethernet.hh"

#include "../../../oflib/ofl-structs.h"
#include "../../../oflib/ofl-messages.h"

namespace {

using namespace std;
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
        
}

Disposition
handle_packet(const Event& e)
{
    const Ofp_msg_event& pi = assert_cast<const Ofp_msg_event&>(e);
    struct ofl_msg_packet_in *in = (struct ofl_msg_packet_in *)**pi.msg;
    
    cout << "packet-in" << endl;
    struct ofl_match_tlv *oft;
    struct ofl_match *m= (struct ofl_match *) in->match;
    HMAP_FOR_EACH_WITH_HASH(oft, struct ofl_match_tlv, hmap_node, hash_int(OXM_OF_IN_PORT, 0),
          &m->match_fields) {
        uint32_t value;
        memcpy(&value, oft->value,sizeof(uint32_t)); 
        cout << "Packet came from the port: "<< value << endl;
    }
    
    return CONTINUE;

}
Disposition
handle(const Event& e)
{
    const Datapath_join_event& dpj = assert_cast<const Datapath_join_event&>(e);

        FlowMod *fm = new FlowMod();
        /* Match Creation */
        Flow flow;
        flow.Add_Field("in_port", 2);
        flow.Add_Field("eth_type",(uint16_t) 0x800);
        flow.Add_Field("ip_proto",(uint8_t) 17);
        flow.Add_Field("udp_src", 1223);
        fm->AddMatch(&flow.match);
        fm->fm_msg.table_id = 0;
        /* Actions creation */
        Actions *act = new Actions(2);
        act->CreateOutput(1,0);
        //act->CreateDecTTL(OFPAT_DEC_NW_TTL);
        uint16_t* dl_type = ( uint16_t*) malloc(2);
        *dl_type = 0x86dd;
        act->CreateSetField(dl_type,"eth_type");
        /*Instruction Creation */
        Instruction *i =  new Instruction(1);
        i->CreateGoToTable((uint8_t) 2);
        //Add action to an Apply Instruction
        i->CreateApply(act);
        fm->AddInstructions(i);
        //send message to switch 
        send_openflow_msg(dpj.dpid, (struct ofl_msg_header *)&fm->fm_msg, 0/*xid*/, true/*block*/);
        return CONTINUE;        

}
    void install()
    {
         register_handler(Datapath_join_event::static_get_name(), boost::bind(&Hub::handle, this, _1));
         register_handler(Ofp_msg_event::get_name(OFPT_PACKET_IN), boost::bind(&Hub::handle_packet, this, _1));
    }
};

REGISTER_COMPONENT(container::Simple_component_factory<Hub>, Hub);

} // unnamed namespace
