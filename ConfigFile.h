

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;

class ConfigFile {
// Data
protected:
	string myDelimiter;  // separator between key and value
	string myComment;    // separator between value and comments
	string mySentry;     // optional string to signal end of file
	std::map<string,string> myContents;  // extracted keys and values
	
	typedef std::map<string,string>::iterator mapi;
	typedef std::map<string,string>::const_iterator mapci;

// Methods
public:
	explicit ConfigFile( string filename,
	            string delimiter = "=",
	            string comment = "#",
				string sentry = "EndConfigFile" );
	ConfigFile();
	
	// Search for key and read value or optional default value
	template<class T> T read( const string& key ) const;  // call as read<T>
	template<class T> T read( const string& key, const T& value ) const;
	template<class T> bool readInto( T& var, const string& key ) const;
	template<class T>
	bool readInto( T& var, const string& key, const T& value ) const;
	
	// Modify keys and values
	template<class T> void add( string key, const T& value );
	void remove( const string& key );
	
	// Check whether key exists in configuration
	bool keyExists( const string& key ) const;
	
	// Check or change configuration syntax
	string getDelimiter() const { return myDelimiter; }
	string getComment() const { return myComment; }
	string getSentry() const { return mySentry; }
	string setDelimiter( const string& s )
		{ string old = myDelimiter;  myDelimiter = s;  return old; }  
	string setComment( const string& s )
		{ string old = myComment;  myComment = s;  return old; }
	
	// Write or read configuration
	friend std::ostream& operator<<( std::ostream& os, const ConfigFile& cf );
	friend std::istream& operator>>( std::istream& is, ConfigFile& cf );
	
protected:
	template<class T> static string T_as_string( const T& t );
	template<class T> static T string_as_T( const string& s );
	static void trim( string& s );


// Exception types
public:
	struct file_not_found {
		string filename;
		explicit file_not_found( const string& filename_ = string() )
			: filename(filename_) {} };
	struct key_not_found {  // thrown only by T read(key) variant of read()
		string key;
		explicit key_not_found( const string& key_ = string() )
			: key(key_) {} };
};


/* static */
template<class T>
string ConfigFile::T_as_string( const T& t )
{
	// Convert from a T to a string
	// Type T must support << operator
	std::ostringstream ost;
	ost << t;
	return ost.str();
}


/* static */
template<class T>
T ConfigFile::string_as_T( const string& s )
{
	// Convert from a string to a T
	// Type T must support >> operator
	T t;
	std::istringstream ist(s);
	ist >> t;
	return t;
}


/* static */
template<>
inline string ConfigFile::string_as_T<string>( const string& s )
{
	// Convert from a string to a string
	// In other words, do nothing
	return s;
}


/* static */
template<>
inline bool ConfigFile::string_as_T<bool>( const string& s )
{

	bool b = true;
	string sup = s;
	for( string::iterator p = sup.begin(); p != sup.end(); ++p )
		*p = toupper(*p);  // make string all caps
	if( sup==string("FALSE") || sup==string("F") ||
	    sup==string("NO") || sup==string("N") ||
	    sup==string("0") || sup==string("NONE") )
		b = false;
	return b;
}


template<class T>
T ConfigFile::read( const string& key ) const
{
	// Read the value corresponding to key
	mapci p = myContents.find(key);
	if( p == myContents.end() ) throw key_not_found(key);
	return string_as_T<T>( p->second );
}


template<class T>
T ConfigFile::read( const string& key, const T& value ) const
{

	mapci p = myContents.find(key);
	if( p == myContents.end() ) return value;
	return string_as_T<T>( p->second );
}


template<class T>
bool ConfigFile::readInto( T& var, const string& key ) const
{

	mapci p = myContents.find(key);
	bool found = ( p != myContents.end() );
	if( found ) var = string_as_T<T>( p->second );
	return found;
}


template<class T>
bool ConfigFile::readInto( T& var, const string& key, const T& value ) const
{

	mapci p = myContents.find(key);
	bool found = ( p != myContents.end() );
	if( found )
		var = string_as_T<T>( p->second );
	else
		var = value;
	return found;
}


template<class T>
void ConfigFile::add( string key, const T& value )
{
	// Add a key with given value
	string v = T_as_string( value );
	trim(key);
	trim(v);
	myContents[key] = v;
	return;
}

#endif  // CONFIGFILE_H


