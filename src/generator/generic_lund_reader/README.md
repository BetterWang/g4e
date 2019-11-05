# Generic lund reader

Universal parser for text files from generators.

It aims for cases:

1. Files have are space/tab separated data values
2. Each event start with one or several lines of data
3. Then there are several lines with data for each particle in event

2-3. Means that for each new event we see an event record and after several particle records

This universal parser provides abstraction to deal with any generic case of it. And works for files like from pythia6, beagle, etc...

## Beagle event example

Lets look at beagle event record:

```
     BEAGLE EVENT FILE
     ============================================
     I, ievent, ...
     ============================================
     I  ISTHKK(I)  ...
     ============================================
     0          1    1   11    2    1    ... <50+ tokens>
     ============================================
      1     21         11        0       -0.000000       -0.000000       -5.000000  ... <15 tokens>
      2     21       2212        0        0.000000        0.000000       50.107677  ...
     ...
     20      1       2212        0        0.018354        0.019732       45.588296  ...
     =============== Event finished ===============
     0          2    1   11    2    1 ... < next event >
     ============================================
     1     21         11        0        0.000000        0.000000       -5.000000 ...

```

What we see here is:

    1. First 5 lines are to be skipped
    2. Lines with '====================' are to be skipped
    3. Lines with 50+ tokens like "0 1 1 11 2 1 ..." are the beginning of event data with whole vent info
    4. All the next lines like "1 21 22122 ..." are particle data lines (called record in this class)
    5. All lines in 3 and 4 need to be tokenized
    6. In other file formats comments like '#...' should be ignored

As a result of parsing one will have and event in terms of :

```python
 TextFileEvent:
     started_at_line;    // Line number at which the event has started
     event_tokens;       // All tokens like {"0", "1", "1", "11", "2", "1", ...} from 'event' related line/s
     record_tokens       // All tokens like {"1", "21", "22122", ...} from each particle or 'record' lines
```

## API

The constructor of the class is defined as:

```cpp
        TextEventFileReader(const std::string file_name,
                            TextEventFileReader::LineRecognisionFunc line_reco_func,
                            TextEventFileReader::TokensRecognisionFunc token_reco_func)
```

User provides a file name to read and two functions (usually lambdas):

* *LineRecognisionFunc* - Makes a decision based on the whole next line if the line should be processed or skipped
* *TextEventLineTypes* -  Makes a decision based on tokenized line, if this line relates to event data or individual particle data

> When a new line is read from file @see line_reco_func_ is called first with this line given as a parameter
> the function should decide if to skip the line or process it further - tokenize
>
> After string is tokenized TokensRecognisionFunc token_reco_func_ is called to make a decision:
> if this line relates to event, particle or should be skipped

To parse such files user should provide istream (like from opening a file) and 2 functions:

