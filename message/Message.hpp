#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "../includes/ft_irc.hpp"

class Message {
	public:

		/* Constructors and destructors */

		Message();
		Message(std::string & msg);
		Message(const Message & copy);
		~Message();

		/* Member functions */


		/* Setters and Getters */

		std::string & get_prefix() const;
		std::string & get_command() const;
		std::vector<std::string> & get_params() const;

		/* Operator overloads */

		Message & operator=(const Message & rhs);

	private:
		
		/* Member variables */

		void parse();

		std::string msg;

		std::string prefix;
		std::string command;
		std::vector<std::string> params;
};

#endif
