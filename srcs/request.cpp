#include "request.hpp"

Request::Request(void) {}

Request::Request(const std::string &raw) {

	int	i = 0, end = 0;
	if (raw.empty())
		return 0;
	for (; i < (int)raw.size() && raw[i] == ' '; i++)
		;;
	while (1) {
		for (end = i; end < (int)raw.size() && raw[end] != ' '; end++)
			;;

		if (raw[i] == ':' && _command.empty() && _prefix.empty()) {
			_prefix = raw.substr(i + 1, end - (i + 1)); // + 1 to remove ':'
		} else {

			if (raw[i] == ':')
				end = raw.size();
			if (_command.empty() && raw[i] != ':')
				_command = raw.substr(i, end - i);
			else {
				if (raw[i] == ':')
					i++;
				_arguments.push_back(raw.substr(i, end - i));
			}
		}
		if (!(end < (int)raw.size()))
			break ;
		for (i = end; i < (int)raw.size() && raw[i] == ' '; i++)
			;;
		if (!(i < (int)raw.size()))
			break ;
	}

	if (_arguments.size()) {
		std::string &last = _arguments[_arguments.size() - 1];
		last.erase(last.find_last_not_of(" ") + 1);
	}
}

Request::~Request(void) {

	_prefix.clear();
	_command.clear();
	_arguments.clear();
}

bool	Request::is(const std::string &str) const { return !_command.compare(str); }
bool	Request::empty(void) const { return _command.empty(); }

const std::string	&Request::getPrefix(void) const { return _prefix; }
const std::string	&Request::getCommand(void) const { return _command; }

size_t		Request::size(void) const {

	return (!_command.empty()) + _arguments.size();
}

const std::string	&Request::operator[](const size_t &n) const {

	if (!n)
		return _command;
	if (n > _arguments.size())
		return _arguments.back();
	return _arguments[n - 1];
}
