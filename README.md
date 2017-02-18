# Qrc Editor

Overview
--------

This is a simple command line program that dynamically creates the qrc file
from a set of files and a set of options all passed through command line. Below
is a quick demo of what can be done:

	$ qrc-editor --global_prefix="/i18n" i18n.qrc \
	      build/translations/lang-en.ts \
	      build/translations/lang-pt.ts \
	      build/translations/lang-de.ts

This will produce the "i18n.qrc" with the following content:

	<?xml version="1.0" encoding="utf-8"?>
	<RCC>
	  <qresource prefix="/i18n">
	    <file alias="hello/lang-de.ts">../translations/lang-de.ts</file>
	    <file alias="hello/lang-en.ts">../translations/lang-en.ts</file>
	    <file alias="hello/lang-pt.ts">../translations/lang-pt.ts</file>
	  </qresource>
	</RCC>

Dependencies
------------

* CMake (version 3.6+)
* Boost C++ Library

Future Features
---------------

Currently this project only supports the creation of the qrc files from
scratch, however there are plans to expand and introduce the ability to append
and remove files. As the need arises more will be done to the project.
