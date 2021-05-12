#pragma once

#include <string>
#include <fstream>
#include <map>
#include <utility>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

class UK5B_var
{

public:
	UK5B_var() = default;

	void UK5B_setName(std::string s) {
		name = std::move(s);
	}

	int UK5B_getPlace() const
	{
		return place;
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

	void UK5B_setValue(const bool x, const double def) {

		ferr.open("error.log", std::ios::out);
		boost::property_tree::ptree	pt;

		std::map <std::string, int> mapping;
		mapping["in"] = 0;
		mapping["in_out"] = 1;
		mapping["out"] = 2;

		if (x) {
			// взять данные из config.ini

			try
			{
				read_ini("config.ini", pt);
				for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end(); ++pos)
				{
					auto f = pos->second.find(name);
					if (f != pos->second.not_found())
					{
						value = (*f).second.get_value<double>();
						place = mapping[pos->first];
						break;
					}
				}
			}
			
			catch (const boost::property_tree::ptree_error &e)
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

	double UK5B_getValue() const		
	{
		return (value);
	}

private:
	double value = 0.;
};

class UK5B_varI : public UK5B_var
{

public:
	UK5B_varI() = default;

	void UK5B_setValue(const bool x, const int def) {

		ferr.open("error.log", std::ios::out);
		boost::property_tree::ptree	pt;

		std::map <std::string, int> mapping;
		mapping["in"] = 0;
		mapping["in_out"] = 1;
		mapping["out"] = 2;

		if (x) {
			// взять данные из config.ini

			try
			{
				read_ini("config.ini", pt);
				for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end(); ++pos)
				{
					auto f = pos->second.find(name);
					if (f != pos->second.not_found())
					{
						value = (*f).second.get_value<int>();
						place = mapping[pos->first];
						break;
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

	int UK5B_getValue() const
	{
		return (value);
	}

private:
	int value = 0;
};

class UK5B_varVD : public UK5B_var
{

public:
	UK5B_varVD() = default;

	void UK5B_setValue(const bool x, const std::vector<double>& def) {

		ferr.open("error.log", std::ios::out);
		boost::property_tree::ptree	pt;

		std::map <std::string, int> mapping;
		mapping["in"] = 0;
		mapping["in_out"] = 1;
		mapping["out"] = 2;

		if (x) {
			// взять данные из config.ini

			try
			{
				read_ini("config.ini", pt);
				for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end(); ++pos)
				{
					auto f = pos->second.find(name);
					if (f != pos->second.not_found())
					{
						auto s = (*f).second.get_value<std::string>();
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
								ferr << e.what() << std::endl;
							}
						}

						place = mapping[pos->first];
						break;
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

private:
	std::vector<double> value = {};
};

class UK5B_varVI : public UK5B_var
{

public:
	UK5B_varVI() = default;

	void UK5B_setValue(const bool x, const std::vector<int>& def) {

		ferr.open("error.log", std::ios::out);
		boost::property_tree::ptree	pt;
		
		std::map<std::string, int> mapping;
		mapping["in"] = 0;
		mapping["in_out"] = 1;
		mapping["out"] = 2;

		if (x) {
			// взять данные из config.ini

			try
			{
				read_ini("config.ini", pt);
				for (boost::property_tree::ptree::iterator pos = pt.begin(); pos != pt.end(); ++pos)
				{
					auto f = pos->second.find(name);
					if (f != pos->second.not_found())
					{
						auto s = (*f).second.get_value<std::string>();
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
						    	ferr << e.what() << std::endl;
							}
						}
						
						place = mapping[pos->first];
						break;
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

private:
	std::vector<int> value = {};
};
