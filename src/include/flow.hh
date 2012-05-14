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
#ifndef FLOW_HH
#define FLOW_HH 1

#include <iostream>
#include <string>
#include <map>
#include <cstring>
#include <iosfwd>
#include "netinet++/ethernetaddr.hh"
#include "openflow/openflow.h"
//#include "openflow-pack-raw.hh"
#include "../oflib/ofl-structs.h"
#include "../oflib/oxm-match.h"

namespace vigil {

class Buffer;

class Flow {
public:
  struct ofl_match match;
    
  /** Empty constructor
   */
  Flow();
  /** Copy constructor
   */
  Flow(const Flow& flow_);
  /** Constructor from ofl_match
   */
  Flow(const struct ofl_match *match);
  /** Add an OXM TLV to the match
   */
  template<typename T> 
  void Add_Field(std::string name, T value){

    if(!fields.count(name))
       std::cout <<"Match field: "<< name << " is not supported "<< std::endl;
    else {
       ofl_structs_match_put(&this->match, fields[name], value);
    }
  
  }
  /** Add a OXM TLV to the match 
   *  with a masked value
   */  
  template<typename T> 
  void Add_Field(std::string name, T value, T mask){
    if(!fields.count(name))
       std::cout <<"Match field: "<< name << " is not supported "<< std::endl;
    else {
       ofl_structs_match_put_masked(&this->match, fields[name], value, mask);
    }
  
  }
  /** \brief String representation
   */
  const std::string to_string() const;
  /** \brief Return hash code
   */
  uint64_t hash_code() const;
private:
  void init();
};
bool operator==(const Flow& lhs, const Flow& rhs);
bool operator!=(const Flow& lhs, const Flow& rhs);
std::ostream& operator<<(std::ostream&, const Flow&);

} // namespace vigil

ENTER_HASH_NAMESPACE
template <>
struct hash<vigil::Flow> {
  std::size_t operator() (const vigil::Flow& flow) const {
    return HASH_NAMESPACE::hash<uint64_t>()(flow.hash_code());
  }
};
EXIT_HASH_NAMESPACE

#endif /* flow.hh */
