#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class Channel {

	public:

		Channel();
		Channel(std::string const & name);
		Channel(Channel const & cpy);
		~Channel();

	private:

		std::string name;

}

#endif
