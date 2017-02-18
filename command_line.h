#pragma once

#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/options_description.hpp>

/**
 * Class offers the ability to process the command line arguments
 * for the project's script and provide methods to gracefully retrieve
 * information from the results.
 */
class CommandLine
{
	public:

		/**
		 * Constructor
		 *
		 * @param argc  number of command line arguments
		 * @param argv  reference to the command line arguments
		 */
		CommandLine(int argc, char** argv);

		/**
		 * @return the global prefix path for the files
		 */
		boost::optional<std::string> getGlobalPrefix() const;

		/**
		 * @return the per file prefix for the file's alias
		 */
		boost::optional<std::string> getLocalPrefix() const;

		/**
		 * @return the language value set for each language file
		 */
		boost::optional<std::string> getLanguage() const;

		/**
		 * @return if the append mode was set
		 */
		bool isAppendMode() const;

		/**
		 * @return  if the remove mode was set
		 */
		bool isRemoveMode() const;

		/**
		 * @return path to the qrc file entered via command line
		 */
		boost::optional<std::string> getQrcFile() const;

		/**
		 * @return list of all the files to be added into the qrc file (see  \ref ::getQrcFilePath())
		 */
		boost::optional<std::vector<std::string>> getFilePaths() const;

		/**
		 * @return if the command line arguments requested the help meesage to be shown, the message will be returned
		 */
		boost::optional<std::string> getHelpMessage() const;

		/**
		 * @return if there was an error, the error message will be returned, otherwise nothing
		 */
		boost::optional<std::string> getError() const;

		/**
		 * @return help message for the command
		 */
		std::string getUsage() const;

	private:

		const int argc;
		char** argv;

		boost::program_options::options_description options_description;
		boost::program_options::positional_options_description positional_options_description;

		boost::program_options::variables_map options;
		boost::optional<std::string> error;
};