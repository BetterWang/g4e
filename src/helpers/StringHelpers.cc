#include <cstdlib>
#include <string>
#include <vector>

#include "StringHelpers.hh"


struct DHTMLReplace {
    std::string match;
    std::string replace;
} 

gHTMLReplaceCodes[] = {
        {"&", "&amp;"},
        {"<", "&lt;"},
        {">", "&gt;"},
        {"\"","&quot;"}
};

std::string g4e::Decode(const std::string& source )
{
    using namespace std;

    string rs = source;

    // Replace each matching token in turn
    for (auto &gHTMLReplaceCode : gHTMLReplaceCodes) {
        // Find the first match
        const string& match = gHTMLReplaceCode.replace;
        const string& repl = gHTMLReplaceCode.match;
        string::size_type start = rs.find_first_of( match );

        // Replace all matches
        while ( start != string::npos ) {
            rs.replace( start, match.size(), repl );
            // Be sure to jump forward by the replacement length
            start = rs.find_first_of( match, start + repl.size() );
        }
    }

    return rs;
}

std::string g4e::Encode( const std::string& source )
{
    using namespace std;
    string rs = source;

    // Replace each matching token in turn
    for (auto &gHTMLReplaceCode : gHTMLReplaceCodes) {
        // Find the first match
        const string& match = gHTMLReplaceCode.match;
        const string& repl = gHTMLReplaceCode.replace;
        string::size_type start = rs.find_first_of( match );

        // Replace all matches
        while ( start != string::npos ) {
            rs.replace( start, match.size(), repl );
            // Be sure to jump forward by the replacement length
            start = rs.find_first_of( match, start + repl.size() );
        }
    }

    return rs;
}

//_____________________________________________________________________________________________________________
int g4e::Replace(const std::string& pattern, const std::string& replace, const std::string& source, std::string &out)
{
    using namespace std;

    int matches = 0;
    out.assign(source);

    string::size_type start = out.find( pattern );

    // Replace all matches
    while ( start != string::npos ) {
        matches++;
        out.replace( start, pattern.size(), replace );
        // Be sure to jump forward by the replacement length
        start = out.find( pattern, start + replace.size() );
    }
    return matches;
}


//___________________________________________________________________________________________________
std::string g4e::Replace(const std::string& pattern, const std::string& replace, const std::string& source)
{
    std::string out;
    Replace(pattern, replace, source, out);
    return out;
}

//______________________________________________________________________________
bool g4e::WildCardCheck( const char* pattern, const char* source )
{
    char *cp, *mp;
    while ((*source) && (*pattern != '*'))
    {
        if ((*pattern != *source) && (*pattern != '?'))
        {
            return false;
        }

        pattern++;
        source++;
    }

    while (*source)
    {
        if (*pattern == '*')
        {
            if (!*++pattern)
            {
                return true;
            }

            mp = const_cast<char *>(pattern);
            cp = const_cast<char *>(source+1);
        }
        else if ((*pattern == *source) || (*pattern == '?'))
        {
            pattern++;
            source++;
        }
        else
        {
            pattern = mp;
            source = cp++;
        }
    }

    while (*pattern == '*')
    {
        pattern++;
    }

    return !*pattern;
}


//________________________________________________________________________________________
std::vector<std::string> g4e::Split( const std::string &s, const std::string& delimiters /*= " "*/ )
{
    std::vector<std::string> elems;
    return Split(s, elems, delimiters);
}


//______________________________________________________________________________________________________
std::vector<std::string> & g4e::Split( const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters /*= " "*/ )
{
    using namespace std;
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
    return tokens;
}


//______________________________________________________________________________
std::vector<std::string> g4e::LexicalSplit( const std::string& source )
{
    //

    /** Splits string to lexical values.
    *
    * LexicalSplit treats:
    * 1) "quoted values" as one value,
    * 2) '#' not in the beginning of the file are treated as comments to the end of the line
    * 3) skips all white space characters. All specification is in doc/ccdb_file_format.pdf
    */
    std::vector<std::string> tokens;
    LexicalSplit(tokens, source);
    return tokens;
}

//____________________________________________________________________________________________
void g4e::LexicalSplit( std::vector<std::string>& tokens, const std::string& source )
{
    /** Splits string to lexical values.
    *
    * LexicalSplit treats:
    * 1) "quoted values" as one value,
    * 2) '#' not in the beginning of the file are treated as comments to the end of the line
    * 3) skips all white space characters. All specification is in doc/ccdb_file_format.pdf
    * 
    * @remark
    * Handling inconsistencies and errors while readout parse time:
    *  ?  No ending quote . If no ending is found, string value will be taken
    *     until the end of line.
    *  ?  Comment inside a string. Comment symbol inside the line is ignored. 
    *     So if you have a record in the file info it will be read just
    *     as string
    *  ?  Stick string. In case of there is no spaces between symbols and
    *     a quote, all will be merged as one string. I.e.:
    *     John" Smith" will be parsed as one value: "John Smith"
    *     John" "Smith will be parsed as one value: "John Smith"
    *     but be careful(!) not to forget to do a spaces between columns
    *     will be parsed as one value that probably would
    *     lead to errors if these were two different columns
    *  ?  If data contains string fields they are taken into characters. All "
    *     inside string should be saved by \" symbol. All words and symbols
    *     inside "..." will be interpreted as string entity.
    *
    */
    //clear output
    tokens.clear();
    bool stringIsStarted = false; //Indicates that we meet '"' and looking for second one
    std::string readValue;
    //iterate through string
    for(size_t i=0; i<source.length(); i++)
    {
        if(CCDB_CHECK_CHAR_IS_BLANK(source[i]) && !stringIsStarted)
        {
            //we have a space! Is it a space that happens after value?
            if(readValue.length()>0)
            {
                tokens.push_back(readValue);
                readValue="";
            }
        }
        else
        {
            //it is not a blank character!
            if(source[i]=='\\' && stringIsStarted && i<(source.length()-1) && source[i+1]=='"')
            {
                //ok! we found a \" inside a string! Not a problem! At all!					

                i++; //skip this \ symbol
                readValue+=source[i]; //it is just one more symbol in value
            }
            else if(source[i]=='#' && !stringIsStarted) //lets check if it is a comment symbol that is not incide a string...
            {
                //it is a comment started...
                //lets save what we collected for now if we collected
                if(readValue.length()>0)
                {
                    tokens.push_back(readValue);
                    readValue="";
                }

                //and put there the rest of the lint(all comment) if there is something to put
                if(i<(source.length()-1))
                {
                    tokens.push_back(source.substr(i));

                    //after that gentelment should exit
                    return;
                }
            }
            else if(source[i]=='"')
            {

                //it is a beginnig or ending  of a string 
                //just set appropriate flag and continue
                stringIsStarted = !stringIsStarted;
            }
            else
            {
                //it is just one more symbol in file
                readValue+=source[i];
            }
        }

        //last we have is to check that 
        //it is not the end of the lint			
        if(i==(source.length()-1) && readValue.length()>0)
        {
            tokens.push_back(readValue);
            readValue="";
        }
    }
}


