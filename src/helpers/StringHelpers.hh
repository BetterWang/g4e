#ifndef G4E_STRING_HELPERS_HH
#define G4E_STRING_HELPERS_HH


#include <string>
#include <vector>
#include <algorithm>


#define CCDB_BLANK_CHARACTERS " \n\t\v\r\f"
//checks if character is blank.
//returns true if char is one of CCDB_BLANK_CHARACTERS
#define CCDB_CHECK_CHAR_IS_BLANK(character) ((character)==' ' || (character)=='\n' || (character)=='\t' || (character)=='\v' || (character)=='\r' || (character)=='\f')


#define array_length(array) (sizeof (array) / sizeof (array)[0])

namespace g4e {
    /** @brief Encodes std::string to add to DB
     * Encode
     *
     * @param     const std::string & source
     * @return   std::std::string
     */
    std::string Encode(const std::string& source);

    /** @brief Decode std::string from encoded std::string
     *
     * @param     const std::string & source
     * @return   std::std::string
     */
    std::string Decode(const std::string& source);

    /** @brief std::string Replace
     *
     * @param     const std::string & pattern
     * @param     const std::string & replace
     * @param     const std::string & source
     * @param     std::string & out
     * @return   int
     */
    int Replace(const std::string& pattern, const std::string& replace, const std::string& source, std::string &out);

    /** @brief std::string Replace
     *
     * @param     const std::string & pattern
     * @param     const std::string & replace
     * @param     const std::string & source
     * @return   std::std::string
     */
    std::string    Replace(const std::string& pattern, const std::string& replace, const std::string& source);

    /** @brief search pattern in source using * and ?
     *
     * @param pattern
     * @param source
     * @return   bool Returns:true on match, false on no match.
     */
    bool WildCardCheck(const char* pattern, const char* source);


    /** @brief
     * Split
     *
     * @param     const std::string & str
     * @param     vector<std::string> & tokens
     * @param     const std::string & delimiters
     * @return   std::vector<std::std::string> &
     */
    std::vector<std::string> &Split(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = " ");


    /** @brief
     * Split
     *
     * @param     const std::std::string & s
     * @param     char delimiters
     * @return   std::vector<std::std::string>
     */
    std::vector<std::string> Split(const std::string &s, const std::string& delimiters = " ");



    /**
     * @brief trims std::string from the both sides
     *
     * @warning the method changes the std::string
     * removes blank characters:
     * " ", "\n", "\t", "\v", "\r", "\f"
     * from the beginning and the end of the std::string
     *
     * @param [in,out] s - std::string that will be trimmed
     */
    inline void Trim( std::string& s )
    {
        // Remove leading and trailing whitespace
        static const char whitespace[] = CCDB_BLANK_CHARACTERS;
        s.erase( 0, s.find_first_not_of(whitespace) );
        s.erase( s.find_last_not_of(whitespace) + 1U );
    }

    /** Splits std::string to lexical values.
     *
     * LexicalSplit treats:
     * 1) "quoted values" as one value,
     * 2) '#' not in the beginning of the file are treated as comments to the end of the line
     * 3) skips all white space characters. All specification is in doc/ccdb_file_format.pdf
     *
     * @remark
     * Handling inconsistencies and errors while readout parse time:
     *  -  No ending quote . If no ending " is found, std::string value will be taken
     *     until the end of line.
     *  -  Comment inside a std::string. Comment symbol inside the line is ignored.
     *     So if you have a record in the file "info #4" it will be read just
     *     as "info #4" std::string
     *  -  Sticked std::string. In case of there is no spaces between symbols and
     *     an quotes, all will be merged as one std::string. I.e.:
     *     John" Smith" will be parsed as one value: "John Smith"
     *     John" "Smith will be parsed as one value: "John Smith"
     *     but be careful(!) not to forget to do a spaces between columns
     *     5.14"Smith" will be parsed as one value "5.14Smith" that probably would
     *     lead to errors if these were two different columns
     *  -  If data contains std::string fields they are taken into "..." characters. All "
     *     inside std::string should be saved by \" symbol. All words and symbols
     *     inside "..." will be interpreted as std::string entity.
     *
     */
    std::vector<std::string> LexicalSplit(const std::string& source);

    /** Splits std::string to lexical values.
   *
   * LexicalSplit treats:
   * 1) "quoted values" as one value,
   * 2) '#' not in the beginning of the file are treated as comments to the end of the line
   * 3) skips all white space characters. All specification is in doc/ccdb_file_format.pdf
   *
   * @remark
   * Handling inconsistencies and errors while readout parse time:
   *  -  No ending quote . If no ending " is found, std::string value will be taken
   *     until the end of line.
   *  -  Comment inside a std::string. Comment symbol inside the line is ignored.
   *     So if you have a record in the file "info #4" it will be read just
   *     as "info #4" std::string
   *  -  Sticked std::string. In case of there is no spaces between symbols and
   *     an quotes, all will be merged as one std::string. I.e.:
   *     John" Smith" will be parsed as one value: "John Smith"
   *     John" "Smith will be parsed as one value: "John Smith"
   *     but be careful(!) not to forget to do a spaces between columns
   *     5.14"Smith" will be parsed as one value "5.14Smith" that probably would
   *     lead to errors if these were two different columns
   *  -  If data contains std::string fields they are taken into "..." characters. All "
   *     inside std::string should be saved by \" symbol. All words and symbols
   *     inside "..." will be interpreted as std::string entity.
   *
   */
    void LexicalSplit(std::vector<std::string>& tokens, const std::string& source);


    inline bool EndsWith(const std::string &str, const char *suffix, unsigned suffixLen)
    {
        return str.size() >= suffixLen && 0 == str.compare(str.size()-suffixLen, suffixLen, suffix, suffixLen);
    }

    inline bool EndsWith(const std::string& str, const char* suffix)
    {
        return EndsWith(str, suffix, static_cast<unsigned int>(std::string::traits_type::length(suffix)));
    }

    inline bool StartsWith(const std::string &str, const char *prefix, unsigned prefixLen)
    {
        return str.size() >= prefixLen && 0 == str.compare(0, prefixLen, prefix, prefixLen);
    }

    inline bool StartsWith(const std::string &str, const char *prefix)
    {
        return StartsWith(str, prefix, static_cast<unsigned int>(std::string::traits_type::length(prefix)));
    }


    // Trim from start (in place)
    inline void LeftTrimThis(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch);}));
    }

    // Trim from end (in place)
    inline void RightTrimThis(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {return !std::isspace(ch);}).base(), s.end());
    }

    // Trim from both ends (in place)
    inline void TrimThis(std::string &s)
    {
        LeftTrimThis(s);
        RightTrimThis(s);
    }

    // Trim from start (copying)
    inline std::string LeftTrimCopy(std::string s)
    {
        LeftTrimThis(s);
        return s;
    }

    // Trim from end (copying)
    inline std::string RightTrimCopy(std::string s)
    {
        RightTrimThis(s);
        return s;
    }

    // Trim from both ends (copying)
    inline std::string TrimCopy(std::string s)
    {
        Trim(s);
        return s;
    }

    inline std::string ToLowerCopy(std::string s) {
        // This function is tested to be faster than boost::algorithm::to_lower
        // Probably it would be a problem with locales. But who cares, we are in physics
        for(auto& c : s) {
            c = tolower(c);
        }
        return s;
    }
}

#endif // G4E_STRING_HELPERS_HH
