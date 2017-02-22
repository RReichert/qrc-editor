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
		("global_prefix,P", boost::program_options::value<std::string>(),                     "qrc prefix")
		("local_prefix,p",  boost::program_options::value<std::string>(),                     "qrc prefix for file alias")
		("language,l",      boost::program_options::value<std::string>(),                     "qrc language")
		("relative_path,r", boost::program_options::value<std::string>()->default_value("."), "relative input files will be prefixed with this path")
//		("append,a",        boost::program_options::value<std::string>(),                     "appends the files onto the qrc file")
//		("remove,r",        boost::program_options::value<std::string>(),                     "removes the files from the qrc file")
		("qrc",             boost::program_options::value<std::string>(),                     "ouput qrc file path")
		("files",           boost::program_options::value<std::vector<std::string>>(),        "input files to include in output qrc file")
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
	}
	catch(const boost::program_options::error& e)
	{
		error = e.what();
		return;
	}

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
			std::vector<std::string> files(options["files"].as<std::vector<std::string>>());
			boost::filesystem::path relative_path(options["relative_path"].as<std::string>());

			for(std::string& file : files)
			{
				boost::filesystem::path file_path(file);

				if(file_path.is_relative())
				{
					file_path = relative_path / file_path;
				}

				if (!boost::filesystem::is_regular_file(file_path))
				{
					error = (boost::format("error: input file \"%1%\" does not exist") % file_path.string()).str();
					break;
				}

				file = file_path.string();
			}

			options.at("files").value() = boost::any(files);
		}
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