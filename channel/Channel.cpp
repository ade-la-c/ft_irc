#include "Channel.hpp"

Channel::Channel() : name("") {
}

Channel::Channel(std::string const & name) : name(name) {
}

Channel::Channel(Channel const & cpy) : name(cpy.name) {
}

Channel::~Channel() {
}
