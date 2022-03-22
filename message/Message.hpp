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


		/* Operator overloads */

		Message & operator=(const Message & rhs);

	private:
		
		/* Member variables */

		std::string msg;

};

#endif
