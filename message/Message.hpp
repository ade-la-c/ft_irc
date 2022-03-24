#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "../includes/ft_irc.hpp"

class Message {
	public:

		class IllFormedMessageException : public std::excpetion {
			public:
				virtual char const * what() const throw() {
					return "Ill-formed Message";
				}
		}

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

		void parse();

		std::string::size_type prefix(std::string::size_type pos);
		std::string::size_type command(std::string::size_type pos);
		std::string::size_type params(std::string::size_type pos);
		
		/* Member variables */

		std::string msg;

		std::string prefix;
		std::string command;
		std::vector<std::string> params;
};

#endif
