#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# define SPECIAL "[]\\`_^{|}"
# define MAX_SIZE 512

# include "../includes/ft_irc.hpp"

class Message {
	public:

		class IllFormedMessageException : public std::exception {
			public:
				virtual char const * what() const throw() {
					return "Ill-formed Message";
				}
		};

		/* Constructors and destructors */

		Message();
		Message(std::string const & msg);
		Message(const Message & copy);
		~Message();

		/* Member functions */

		bool is_complete();
		void parse_from_str(std::string const & msg);

		static bool is_numeric(std::string str, std::string::size_type pos = 0, std::string::size_type count = std::string::npos);
		static bool is_special(int c);
		static bool is_nickname(std::string str, std::string::size_type pos = 0, std::string::size_type count = std::string::npos);
		static bool is_channel(std::string str, std::string::size_type pos = 0, std::string::size_type count = std::string::npos);
		static bool match_wildcard(std::string const & pattern, std::string const & str);

		/* Setters and Getters */

		std::string get_prefix() const;
		std::string get_command() const;
		std::vector<std::string> get_params() const;
		std::vector<std::string>::size_type get_params_count() const;

		/* Operator overloads */

		Message & operator=(const Message & rhs);

	private:

		void parse();

		bool complete;


		std::string::size_type parse_prefix(std::string::size_type end);
		std::string::size_type parse_command(std::string::size_type pos, std::string::size_type end);
		std::string::size_type parse_params(std::string::size_type pos, std::string::size_type end);

		/* Member variables */

		std::string msg;

		std::string buffer;

		std::string prefix;
		std::string command;
		std::vector<std::string> params;
		std::vector<std::string>::size_type params_count;
};

#endif
