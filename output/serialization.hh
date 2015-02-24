/** Generation of serialization C++ code.
 * */
#ifndef _CCSDS_TOOL_SERIALIZATION_HH_
#define _CCSDS_TOOL_SERIALIZATION_HH_ 1
#include "data.hh"
#include <ostream>

void printSerializationMethod(std::ostream &, Structure const &);

#endif