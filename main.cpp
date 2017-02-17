#include "command_line.h"

#include <iostream>

#include <boost/format.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

boost::property_tree::ptree build_ptree(const CommandLine& command_line)
{
	// initialize the property tree
	boost::property_tree::ptree ptree;

	// extract command line options
	boost::optional<std::string> global_prefix =  command_line.getGlobalPrefix();
	boost::optional<std::string> local_prefix  =  command_line.getLocalPrefix();
	boost::optional<std::string> language      =  command_line.getLanguage();
	std::vector<std::string> files             = *command_line.getFilePaths();

	// sort the file list in ascending order
	std::sort(files.begin(), files.end());

	// setup base property tree structure
	boost::property_tree::ptree& qresource_element = ptree.put("RCC.qresource", "");

	if(global_prefix)
	{
		qresource_element.put("<xmlattr>.prefix", *global_prefix);
	}
	if(language)
	{
		qresource_element.put("<xmlattr>.lang", *language);
	}

	// add in each file element into the ptree
	for(const std::string& file : files)
	{
		boost::property_tree::ptree file_element;

		file_element.put_value(file);

		if(local_prefix)
		{
			file_element.put("<xmlattr>.alias", (boost::filesystem::path(*local_prefix) / boost::filesystem::path(file).filename()).string());
		}
		else
		{
			file_element.put("<xmlattr>.alias", boost::filesystem::path(file).filename().string());
		}

		qresource_element.add_child("file", file_element);
	}

	// return the constructed ptree
	return ptree;
}

boost::property_tree::ptree update_ptree(const CommandLine& command_line)
{
	throw "feature still in development";
}

int main(int argc, char* argv[])
{
	// process command line arguments
	CommandLine command_line(argc, argv);

	boost::optional<std::string> error = command_line.getError();
	boost::optional<std::string> help  = command_line.getHelpMessage();

	if(error)
	{
		std::cerr << *error << std::endl;
		return EXIT_FAILURE;
	}
	else if(help)
	{
		std::cout << *help << std::endl;
		return EXIT_SUCCESS;
	}

	// given the command line options, we will contrucct the qrc file or updated it
	boost::property_tree::ptree qrc = command_line.isAppendMode() || command_line.isRemoveMode() ?
    update_ptree(command_line) :
    build_ptree(command_line);

	// write qrc property tree as an xml file
	try
	{
		boost::property_tree::xml_writer_settings<std::string> xml_settings('\t', 1);
		boost::property_tree::write_xml(*command_line.getQrcFile(), qrc, std::locale(), xml_settings);
	}
	catch(const boost::property_tree::xml_parser_error& e)
	{
		std::cerr << "error: " <<  e.what() << std::endl;
		return EXIT_FAILURE;
	}

	// successful program exit
	return EXIT_SUCCESS;
}