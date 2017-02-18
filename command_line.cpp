#include "command_line.h"

#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options/parsers.hpp>

CommandLine::CommandLine(int argc, char** argv) :
	argc(argc),
	argv(argv),
	options_description("Options"),
	positional_options_description(),
	options(),
	error()
{
	options_description.add_options()
		("global_prefix,P", boost::program_options::value<std::string>(),              "sets the global prefix path for the files")
		("local_prefix,p",  boost::program_options::value<std::string>(),              "sets the per file prefix for the file's alias")
		("language,l",      boost::program_options::value<std::string>(),              "sets the language used by the files")
//		("append,a",        boost::program_options::value<std::string>(),              "appends the files onto the qrc file")
//		("remove,r",        boost::program_options::value<std::string>(),              "removes the files from the qrc file")
		("qrc",             boost::program_options::value<std::string>(),              "qrc file to write")
		("files",           boost::program_options::value<std::vector<std::string>>(), "files to add to qrc file")
		("help,h",          "prints this help message");

	// NOTE: if you change these options, make sure they reflect in the getUsage() method
	positional_options_description.add("qrc",    1);
	positional_options_description.add("files", -1);

	try
	{
		boost::program_options::store
		(
			boost::program_options::command_line_parser(argc, argv)
				.options(options_description)
				.positional(positional_options_description)
				.run(),
			options
		);

		boost::program_options::notify(options);

		if(!options.count("help"))
		{
			if(!options.count("qrc"))
			{
				error = "qrc file is missing";
			}
			else if(options.count("append") + options.count("delete") > 1)
			{
				error = "multiple operations selected on the qrc";
			}
			else if(options.count("files"))
			{
				for(const std::string& file : options["files"].as<std::vector<std::string>>())
				{
					if (!boost::filesystem::is_regular_file(file))
					{
						error = (boost::format("error: input file \"%1%\" does not exist") % file).str();
						break;
					}
				}
			}
		}
	}
	catch(const boost::program_options::error& e)
	{
		error = e.what();
	}
}

boost::optional<std::string> CommandLine::getGlobalPrefix() const
{
	if(options.count("global_prefix"))
	{
		return options["global_prefix"].as<std::string>();
	}
	else
	{
		return boost::none;
	}
}

boost::optional<std::string> CommandLine::getLocalPrefix() const
{
	if(options.count("local_prefix"))
	{
		return options["local_prefix"].as<std::string>();
	}
	else
	{
		return boost::none;
	}
}

boost::optional<std::string> CommandLine::getLanguage() const
{
	if(options.count("lang"))
	{
		return options["lang"].as<std::string>();
	}
	else
	{
		return boost::none;
	}
}

bool CommandLine::isAppendMode() const
{
	return options.count("append");
}

bool CommandLine::isRemoveMode() const
{
	return options.count("remove");
}

boost::optional<std::string> CommandLine::getQrcFile() const
{
	if(options.count("qrc"))
	{
		return options["qrc"].as<std::string>();
	}
	else
	{
		return boost::none;
	}
}

boost::optional<std::vector<std::string>> CommandLine::getFilePaths() const
{
	if(options.count("files"))
	{
		return options["files"].as<std::vector<std::string>>();
	}
	else
	{
		return boost::none;
	}
}

boost::optional<std::string> CommandLine::getHelpMessage() const
{
	if(options.count("help"))
	{
		return getUsage();
	}
	else
	{
		return boost::none;
	}
}

boost::optional<std::string> CommandLine::getError() const
{
	return error;
}

std::string CommandLine::getUsage() const
{
	return (boost::format("usage: %1% [options] qrc_file [files...]\n%2%") % argv[0] %options_description).str();
}