#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

class UK5B_var
{

public:
	UK5B_var() = default;
	~UK5B_var() = default;
	UK5B_var(const UK5B_var& u) : place(u.place), name(u.name) {}
	UK5B_var(UK5B_var&& u)  noexcept = default;
	UK5B_var& operator = (const UK5B_var& u)
	{
		place = u.place;
		name  = u.name;
		return *this;
	}
	UK5B_var& operator = (UK5B_var&& u) noexcept = default;

	void UK5B_setName(std::string s) {
		name = std::move(s);
	}

	int UK5B_getPlace() const {
		return place;
	}
	
	void UK5B_setPlace(const int p) {
		place = (p >= 0 && p <= 3) ? p : 2;
	}
	
	std::string	UK5B_getName() const
	{
		return name;
	}
protected:
	int			place = 2;
	std::string	name;
	std::ofstream ferr;
	
};

class UK5B_varD : public UK5B_var
{

public:
	UK5B_varD() = default;
	~UK5B_varD() = default;
	UK5B_varD(const UK5B_varD&) = default;
	UK5B_varD(UK5B_varD&&)  noexcept = default;
	UK5B_varD& operator = (const UK5B_varD& u) = default;
	UK5B_varD& operator = (UK5B_varD&&) noexcept = default; 
  
	void UK5B_setValue(const bool x, const double def) {

		std::map <std::string, int> mapping;
		mapping["in"] = 0;
		mapping["in_out"] = 1;
		mapping["out"] = 2;
		mapping["final"] = 3;
		

		if (x) {
			// взять данные из config.ini

			try
			{
				boost::property_tree::ptree pt;
				read_ini("config.ini", pt);
				for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end(); ++pos)
				{
					auto f = pos->second.find(name);
					if (f != pos->second.not_found())
					{
						place = mapping[pos->first];
						value = (place != 3) ? (*f).second.get_value<double>() : def;
						if (place == 1) value2 = def;
						break;
					}
					else
					{
						value = def;
						place = 2;
					}
				}
			}
			
			catch (const boost::property_tree::ptree_error &e)
			{
				ferr.open("error.log", std::ios::out);
				ferr << e.what() << std::endl;
			}
		}
		else
		{
			value = def;
			place = 2;
		}
		
		ferr.close();
	}

	double UK5B_getValue() const		
	{
		return (value);
	}

	double UK5B_getValue2() const		
	{
		return (value2);
	}

private:
	double value = 0.;
	double value2 = 0;
};

class UK5B_varI : public UK5B_var
{

public:
	UK5B_varI() = default;
	~UK5B_varI() = default;
	UK5B_varI(const UK5B_varI&) = default;
	UK5B_varI(UK5B_varI&&)  noexcept = default;
	UK5B_varI& operator = (const UK5B_varI& u) = default;
	UK5B_varI& operator = (UK5B_varI&&) noexcept = default; 

	void UK5B_setValue(const bool x, const int def) {

		std::map <std::string, int> mapping;
		mapping["in"] = 0;
		mapping["in_out"] = 1;
		mapping["out"] = 2;

		if (x) {
			// взять данные из config.ini

			try
			{
				boost::property_tree::ptree pt;
				read_ini("config.ini", pt);
				for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end(); ++pos)
				{
					auto f = pos->second.find(name);
					if (f != pos->second.not_found())
					{
						place = mapping[pos->first];
						value = (*f).second.get_value<int>();
						if (place == 1) value2 = def;
						break;
					}
					else
					{
						value = def;
						place = 2;
					}
				}
			}

			catch (const boost::property_tree::ptree_error& e)
			{
				ferr.open("error.log", std::ios::out);
				ferr << e.what() << std::endl;
			}
		}
		else
		{
			value = def;
			place = 2;
		}
		
		ferr.close();
	}

	int UK5B_getValue() const
	{
		return (value);
	}

	int UK5B_getValue2() const
	{
		return (value2);
	}

private:
	int value = 0;
	int value2 = 0;
};

class UK5B_varVD : public UK5B_var
{

public:
	UK5B_varVD() = default;
	~UK5B_varVD() = default;
	UK5B_varVD(const UK5B_varVD&) = default;
	UK5B_varVD(UK5B_varVD&&)  noexcept = default;
	UK5B_varVD& operator = (const UK5B_varVD&) = default;
	UK5B_varVD& operator = (UK5B_varVD&&) noexcept = default;
	
	void UK5B_setValue(const bool x, const std::vector<double>& def) {

		std::map <std::string, int> mapping;
		mapping["in"] = 0;
		mapping["in_out"] = 1;
		mapping["out"] = 2;

		if (x) {
			// взять данные из config.ini

			try
			{
				boost::property_tree::ptree pt;
				read_ini("config.ini", pt);
				for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end(); ++pos)
				{
					auto f = pos->second.find(name);
					if (f != pos->second.not_found())
					{
						place = mapping[pos->first];
						auto s = (*f).second.get_value<std::string>();
						if (s.empty()) s = "0";
						boost::char_separator<char> sep(";");

						boost::tokenizer< boost::char_separator<char> > list(s, sep);
						for (auto& itr : list)

						{
							try
							{
								if (boost::algorithm::contains(itr, ":"))
								{
									boost::char_separator<char> sep2(":");
									boost::tokenizer< boost::char_separator<char> > list2(itr, sep2);
									auto it = list2.begin();
									const double first = std::stod(*it);
									const int c = std::stoi(*(++it));
									const double step = (std::distance(list2.begin(), list2.end()) == 2) ? 1. : std::stod(*(++it));
									for (int i = 1; i <= c; i += 1) value.push_back(first + ((i - 1) * step));
								}
								else
									value.push_back(stod(itr));
							}
							catch (const std::exception& e)
							{
								value.push_back(-1.);
								ferr.open("error.log", std::ios::out);
								ferr << e.what() << std::endl;
							}
						}
						if (place == 1) value2 = def;
						break;
					}
					else
					{
						value = def;
						place = 2;
					}
				}
			}

			catch (const boost::property_tree::ptree_error& e)
			{
				ferr << e.what() << std::endl;
			}
		}
		else
		{
			value = def;
			place = 2;
		}
		
		ferr.close();
	}

	std::vector<double> UK5B_getValue() const
	{
		return (value);
	}

	std::vector<double> UK5B_getValue2() const
	{
		return (value2);
	}

private:
	std::vector<double> value = {};
	std::vector<double> value2 = {};
};

class UK5B_varVI : public UK5B_var
{

public:
	UK5B_varVI() = default;
	~UK5B_varVI() = default;
	UK5B_varVI(const UK5B_varVI&) = default;
	UK5B_varVI(UK5B_varVI&&)  noexcept = default;
	UK5B_varVI& operator = (const UK5B_varVI&) = default;
	UK5B_varVI& operator = (UK5B_varVI&&) noexcept = default;

	void UK5B_setValue(const bool x, const std::vector<int>& def) {

		std::map<std::string, int> mapping;
		mapping["in"] = 0;
		mapping["in_out"] = 1;
		mapping["out"] = 2;

		if (x) {
			// взять данные из config.ini

			try
			{
				boost::property_tree::ptree pt;
				read_ini("config.ini", pt);
				for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end(); ++pos)
				{
					auto f = pos->second.find(name);
					if (f != pos->second.not_found())
					{
						place = mapping[pos->first];
						auto s = (*f).second.get_value<std::string>();
						if (s.empty()) s = "0";
						boost::char_separator<char> sep(";");
						
						boost::tokenizer< boost::char_separator<char> > list(s, sep);
						for(auto &itr : list)
						
						{
							try
							{
								if (boost::algorithm::contains(itr,":"))
								{
									boost::char_separator<char> sep2(":");
									boost::tokenizer< boost::char_separator<char> > list2(itr, sep2);
									auto it = list2.begin();
									const int first = std::stoi(*it);
									const int c = std::stoi(*(++it));
									const int step = (std::distance( list2.begin(), list2.end() ) == 2) ? 1 : std::stoi(*(++it));
									for (int i = 1; i <= c; i += 1) value.push_back(first + ((i-1)*step));
								}
								else 
									value.push_back(stoi(itr));
							}
							catch (const std::exception& e)
							{
								value.push_back(-1.);
								ferr.open("error.log", std::ios::out);
								ferr << e.what() << std::endl;
							}
						}
						
						if (place == 1) value2 = def;
						break;
					}
					else
					{
						value = def;
						place = 2;
					}
				}
			}
			
			catch(const boost::property_tree::ptree_error &e)
			{
				ferr << e.what() << std::endl;
			}
		}
		else
		{
			value = def;
			place = 2;
		}
		
		ferr.close();
	}

	std::vector<int> UK5B_getValue() const		
	{
		return (value);
	}

	std::vector<int> UK5B_getValue2() const		
	{
		return (value2);
	}

private:
	std::vector<int> value = {};
	std::vector<int> value2 = {};
};
